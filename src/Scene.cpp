#include <assert.h>
#include "Scene.h"
#include "Utils.h"
#include "Globals.h"
#include "RenderThread.h"
#include "Ray.h"
#include <stdlib.h>
#include <time.h>

using namespace ptrt;
using namespace std;

int BvhNode::bvhTreeMaxDepth = 0;

Scene::Scene(const string& scene_xml_file){
	assert(scene_xml_file != "");

	// Read objects from XML
	XMLSceneParser xmlScene;

	// register object creation function with xmlScene
	xmlScene.registerCallback("camera", this);
	xmlScene.registerCallback("light", this);
	xmlScene.registerCallback("shader", this);
	xmlScene.registerCallback("shape", this);
	xmlScene.registerCallback("sceneprops", this);
	xmlScene.registerCallback("transform", this);
	xmlScene.registerCallback("texture", this);
	xmlScene.registerCallback("instance", this);

    xmlScene.parseFile(scene_xml_file);
}

/* Renders the scene onto a png buffer */
png::image< png::rgb_pixel > Scene::render(){
	assert(cameras.size() > 0);

	png::image< png::rgb_pixel > imData(g_args.width, g_args.height);

	if (USE_BVH){
		LOG("Generating BVH Tree...");

		// Will be dellocated when clearing objects
		BvhNode *root_node = generate_bvh_tree();
		objects.clear();
		objects.push_back(root_node);
		LOG("Completed!");
		LOG("Depth of the tree: " << BvhNode::bvhTreeMaxDepth);
		LOG("Start shooting rays...");
	}

	// Initialize threads
	vector<RenderThread *> render_threads;
	for (int i = 0; i < g_args.numCpus; i++){
		render_threads.push_back(new RenderThread(this, &imData));
	}

	// Assign pixels
	unsigned long c = 0;
	for (size_t i = 0; i < imData.get_width(); i++){
		for (size_t j = 0; j < imData.get_height(); j++){
			RenderThread *t = render_threads[c++ % render_threads.size()];
			t->schedule(i, j);
		}
	}

	// Wait for threads to start and finish
	for (unsigned int i = 0; i < render_threads.size(); i++){
		render_threads[i]->start();
	}

	for (unsigned int i = 0; i < render_threads.size(); i++){
		render_threads[i]->join();
	}

	return imData;
}

/* Renders the scene onto a file */
void Scene::render(const string& outfile){
	png::image< png::rgb_pixel > imData = render();
	cout << "Writing image to " << outfile << endl;
	imData.write(outfile);
}

#define EYE_DISTANCE 0.20f

png::image< png::rgb_pixel > Scene::renderRightEye(){
	cout << "Rendering right eye image..." << endl;
	Vector3D camera_pos = cameras[0]->get_position();
	camera_pos[0] += EYE_DISTANCE * 2;
	cameras[0]->set_position(camera_pos);

	png::image< png::rgb_pixel > image = render();

	for (size_t i = 0; i < image.get_width(); i++){
		for (size_t j = 0; j < image.get_height(); j++){
			png::rgb_pixel pixel = image.get_pixel(j, i);
			pixel.green = pixel.blue = (int)((pixel.red + pixel.green + pixel.blue) / 3.0f);
			pixel.red = 0;
			image.set_pixel(j, i, pixel);
		}
	}

	return image;
}

png::image< png::rgb_pixel > Scene::renderLeftEye(){
	cout << "Rendering left eye image..." << endl;

	Vector3D camera_pos = cameras[0]->get_position();
	camera_pos[0] -= EYE_DISTANCE;
	cameras[0]->set_position(camera_pos);

	png::image< png::rgb_pixel > image = render();

	for (size_t i = 0; i < image.get_width(); i++){
		for (size_t j = 0; j < image.get_height(); j++){
			png::rgb_pixel pixel = image.get_pixel(j, i);
			pixel.red = (int)((pixel.red + pixel.green + pixel.blue) / 3.0f);
			pixel.green = 0;
			pixel.blue = 0;
			image.set_pixel(j, i, pixel);
		}
	}

	return image;
}

void Scene::render3D(const string &outfile){
	png::image< png::rgb_pixel > left_eye_image = renderLeftEye();
	png::image< png::rgb_pixel > right_eye_image = renderRightEye();
	png::image< png::rgb_pixel > final_image(g_args.width, g_args.height);

	LOG("Merging...");

	// Merge
	for (size_t i = 0; i < left_eye_image.get_width(); i++){
		for (size_t j = 0; j < left_eye_image.get_height(); j++){
			png::rgb_pixel left_eye_pixel = left_eye_image.get_pixel(j, i);
			png::rgb_pixel right_eye_pixel = right_eye_image.get_pixel(j, i);

			png::rgb_pixel final_pixel;
			final_pixel.red = (left_eye_pixel.red + right_eye_pixel.red);
			final_pixel.blue = (left_eye_pixel.blue + right_eye_pixel.blue);
			final_pixel.green = (left_eye_pixel.green + right_eye_pixel.green);

			final_image.set_pixel(j, i, final_pixel);
		}
	}


	cout << "Writing image to " << outfile << endl;
	final_image.write(outfile);
}

/* Callback handler for XML Parser
 * Instantiates scene objects based on the XML file */
void Scene::instance(std::map<std::string, SceneDataContainer> &sdMap)
 {
	if (sdMap.size() > 0){
		std::map<std::string, SceneDataContainer>::const_iterator map = sdMap.begin();

    	if (m_otype == CAMERA){
    		Camera *c = new Camera(g_args.aspectRatio);
    		c->initialize(sdMap);
    		LOG("Adding " << *c);
    		cameras.push_back(c);
		}
    	else if (m_otype == LIGHT){
			Light *l = Light::create_new(sdMap);
			LOG("Adding " << *l);
			lights.push_back(l);
    	}else if (m_otype == SHADER){
			Shader *s = Shader::create_new(sdMap);
			LOG("Adding " << *s);
			shaders[s->get_name()] = s;
    	}else if (m_otype == SHAPE){
			Object *o = Object::create_new(sdMap, shaders);
			if (o != 0){
				LOG("Adding " << *o);
				objects.push_back(o);
			}
    	}else if (m_otype == TRANSFORM){
    		log_exception("I do not support transformation referenced yet!", true);
			Transform *t = new Transform(sdMap);
			LOG("Adding " << *t);
			transforms[t->get_name()] = t;
    	}else if (m_otype == TEXTURE){
			Texture *t = new Texture(sdMap);
			LOG("Adding " << *t);
			TextureManager::getSingleton()->add_texture(t);
    	}else if (m_otype == INSTANCE){
    		Instance *i = new Instance(sdMap, shaders);
			LOG("Instancing " << *i);
			InstanceManager::getSingleton()->add_instance(i);
    	}else if (m_otype == SCENEPROPS){
    		std::map<std::string, SceneDataContainer>::iterator sdIter;
    		std::istringstream buf;

    		sdIter = sdMap.find("scene_bgcolor");
    		if( sdIter != sdMap.end() ){
        		buf.str(sdIter->second.val);
        		buf >> Shader::background_color;
        		buf.clear();
    		}
    	}
	}
 }

BvhNode *Scene::generate_bvh_tree(){
	// Make a copy of our object list

	vector<Object *> *all_objects = new vector<Object *>();
	for (int i = 0; i < objects.size(); i++){
		all_objects->push_back(objects[i]);
	}

	return new BvhNode(all_objects, 0, 0, BvhNode::SPACE_BASED);
}

Scene::~Scene(){
	for (int i = 0; i<cameras.size(); i++)
		RELEASE_SAFELY(cameras[i]);
	cameras.clear();

	for (int i = 0; i<lights.size(); i++)
		RELEASE_SAFELY(lights[i]);
	lights.clear();

	for (int i = 0; i<objects.size(); i++)
		RELEASE_SAFELY(objects[i]);
	objects.clear();

	map<string, Shader *>::iterator its;
	for (its=shaders.begin(); its != shaders.end(); its++)
	   RELEASE_SAFELY((*its).second);
	shaders.clear();

	map<string, Transform *>::iterator itt;
	for (itt=transforms.begin(); itt != transforms.end(); itt++)
	   RELEASE_SAFELY((*itt).second);
	transforms.clear();

	LOG("Destroyed scene");
}

