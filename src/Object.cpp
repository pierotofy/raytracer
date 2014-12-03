#include <sstream>
#include "Object.h"
#include "BoxObject.h"
#include "SphereObject.h"
#include "CylinderObject.h"
#include "PyramidObject.h"
#include "SkySphereObject.h"
#include "InstanceObject.h"
#include "MeshObject.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{

std::ostream &operator<<(std::ostream& os, ptrt::Object& o)
{
  o.print(os);
  return os;
}

ostream& Object::print(std::ostream& os = std::cout){
	  os << "  Object {" << endl
		 << "    name: " << name << endl
		 << "    shader: " << shader
		 << "  }" << endl;
	  return os;
}

Object::~Object(){
	LOG("  (Base Object destructor called: " << name << ")");

	shader = 0; // This was just a reference, scene will dispose it
}

Object::Object(map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders){
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shape_name");
	assert( sdIter != sdMap.end() );
	name = sdIter->second.val;

	sdIter = sdMap.find("shader_ref");
	if( sdIter != sdMap.end() && sdIter->second.isSet && sdIter->second.val != "null"){
		shader = shaders[sdIter->second.val];
	}else{
		shader = 0;
	}
}

/** Default behavior, complex shapes and instances might want to override this to alter the hit structure */
bool Object::intersect(const Ray& ray, float &t_min, float &t_max, HitStructure &hit){
	bool intersected = intersect(ray, t_min, t_max);
	if (intersected){
		hit.t = t_max;
		hit.intersection_point = ray.get_collision_point(hit.t);
		hit.normal = get_normal(hit.intersection_point);
		hit.shader = shader;
		hit.obj = this;
		return true;
	}else{
		return false;
	}
}

Object* Object::create_new(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders){
    std::map<std::string, SceneDataContainer>::iterator sdIter;
    Object *result = 0;

	sdIter = sdMap.find("shape_type");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.val == "box"){
		result = new BoxObject(sdMap, shaders);
	}else if (sdIter->second.val == "sphere"){
		result = new SphereObject(sdMap, shaders);
	}else if (sdIter->second.val == "cylinder"){
		result = new CylinderObject(sdMap, shaders);
	}else if (sdIter->second.val == "pyramid"){
		result = new PyramidObject(sdMap, shaders);
	}else if (sdIter->second.val == "skysphere"){
		result = new SkySphereObject(sdMap, shaders);
	}else if (sdIter->second.val == "mesh"){
		result = new MeshObject(sdMap, shaders);
	}else if (sdIter->second.val == "instance"){
		InstanceObject *i = new InstanceObject(sdMap, shaders);
		if (i->is_valid()) result = i;
		else{
			LOG("Invalid instance " << i->get_name() << " skipping...");
			result = 0; // Some instances might refer to some shape we don't support
		}
	}else{
		std::cerr << "Warning: unknown shape type: " << sdIter->second.val << ", the program might crash soon!!!!" << std::endl;
		//assert(false);
	}

	if (result != 0) result->generate_bounding_box();

	return result;
}

}

