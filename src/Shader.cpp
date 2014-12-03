#include <sstream>
#include <cmath>
#include "Shader.h"
#include "TextureManager.h"
#include "LambertianShader.h"
#include "BlinnphongShader.h"
#include "NormalMapShader.h"
#include "GlazeShader.h"
#include "Globals.h"
#include "Utils.h"

using namespace ptrt;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::Shader& s)
	{
		s.print(os);
		return os;
	}
}

// Defines the default background color
Vector3D Shader::background_color = Vector3D(0.15f, 0.15f, 0.15f);

// A small value of epsilon (10^-3)
#define SMALL_EPS 0.001f

ostream& Shader::print(std::ostream& os = std::cout){
	  os << "Shader {" << endl
	     << "  name: " << name << endl
	     << "  diffuse: " << diffuse_component << endl
	     << "  diffuse_texture: " << diffuse_texture << endl
	     << "  pointer: " << this << endl
	     << "}" << endl;
	  return os;
}

Shader* Shader::create_new(std::map<std::string, SceneDataContainer> &sdMap)
  {
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shader_type");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.val == "Lambertian"){
		return new LambertianShader(sdMap);
	}else if (sdIter->second.val == "BlinnPhong"){
		return new BlinnphongShader(sdMap);
	}else if (sdIter->second.val == "Normal"){
		return new NormalMapShader(sdMap);
	}else if (sdIter->second.val == "Glaze"){
		return new GlazeShader(sdMap);
	}else{
		std::cout << "Warning: unknown shader type: " << sdIter->second.val << std::endl;
		assert(false);
	}

	return 0; // Never executed
}


Shader::Shader(map<string, SceneDataContainer> &sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shader_name");
	assert( sdIter != sdMap.end() );
	name = sdIter->second.val;

	sdIter = sdMap.find("shader_diffuse");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> diffuse_component;
	buf.clear();

	sdIter = sdMap.find("shader_mirrorCoef");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.isSet){
		buf.str(sdIter->second.val);
		buf >> mirror_coefficent;
		buf.clear();
	}else{
		mirror_coefficent = 0.0f;
	}

//	sdIter = sdMap.find("shader_refractionCoef");
//	assert( sdIter != sdMap.end() );
//	if (sdIter->second.isSet){
//		buf.str(sdIter->second.val);
//		buf >> refraction_coefficent;
//		buf.clear();
//	}else{
		refraction_coefficent = 0.0f;
//	}

	sdIter = sdMap.find("shader_diffusetex");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.isSet){
		string diffuse_tex_name;
		buf.str(sdIter->second.val);
		buf >> diffuse_tex_name;
		buf.clear();

		diffuse_texture = TextureManager::getSingleton()->get_texture(diffuse_tex_name);
	}else{
		diffuse_texture = 0;
	}

	sdIter = sdMap.find("shader_roughness");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.isSet){
		buf.str(sdIter->second.val);
		buf >> roughness;
		buf.clear();
	}else{
		roughness = 0.0f;
	}

	cast_shadows = g_args.useShadow;
}

png::rgb_pixel Shader::get_rgb_from_color(const Vector3D &color){
	Vector3D adjusted_color = color;
	for (int i = 0; i<3; i++)
		if (adjusted_color[i] > 1.0f) adjusted_color[i] = 1.0f; // Cap values bigger than 1

	float r = 255.0f * adjusted_color[0];
	float g = 255.0f * adjusted_color[1];
	float b = 255.0f * adjusted_color[2];

	return png::rgb_pixel((int)r, (int)g, (int)b);
}

Vector3D Shader::apply_shader(const ShaderArgs &sa){
	Vector3D L;

	// Don't compute all this stuff for background components
	if (!sa.hit.background_component){
		Vector3D normal = sa.hit.normal;

		Vector3D v; // Direction to incoming ray
		v -= sa.ray.get_direction();
		v.normalize();

		Vector3D d = sa.ray.get_direction(); // Direction of incoming ray
		d.normalize();

		// Calculate reflection
		Ray reflection_ray;
		reflection_ray.compute_reflection(v, normal, sa.hit.intersection_point);
		reflection_ray.carry_random_uv(sa.ray);
		Vector3D reflection_color = Shader::apply_reflection(sa.objects, sa.lights, reflection_ray, sa.recursion_call_depth);

		for (int i = 0; i<sa.lights.size(); i++){
			// l = vector in direction of light
			Vector3D l = sa.lights[i]->get_position() - sa.hit.intersection_point;
			l.normalize();

			if (!in_shadow(sa.hit.intersection_point, sa.ray, sa.lights[i], sa.objects)){
				L += apply_shader_for_light(sa, sa.lights[i], l, normal, v);
			}
		}

		// Interpolate with reflective component
		L *= (1 - mirror_coefficent);

		L += mirror_coefficent * reflection_color;

		if (refraction_coefficent > 0.0f && !sa.computing_reflection){
			L += get_refraction_color(sa, d, normal, reflection_color);
		}

		L += get_diffuse(sa.hit.intersection_point, sa.hit.obj) * AmbientShader::intensity;

	}else{
		// Background component have static lighting, no reflection, no refraction
		L += get_diffuse(sa.hit.intersection_point, sa.hit.obj);
	}

	// Cap values bigger than 1 (RGB is in the 0-1 range)
	for (int j = 0; j<3; j++)
		if (L[j] > 1.0f) L[j] = 1.0f;

	return L;
}

/** Returns the diffuse component for this shader */
Vector3D Shader::get_diffuse(const Vector3D &intersection_point, Object *obj){
	if (diffuse_texture){
		// Find u,v, calculate color from texture
		assert(obj != 0);
		float u, v;
		obj->get_uv_coords(intersection_point, u, v);

		return diffuse_texture->get_color(u, v);

	}else return diffuse_component; // By default we return the value provided in the XML file
}

/* For a particular intersection_point for a particular light, check if it's in shadow
 * @return true if sa.obj is in shadow, false otherwise */
bool Shader::in_shadow(const Vector3D &intersection_point, const Ray &ray, Light *light, const vector<Object *> &objects){
	if (!cast_shadows) return false;

	// Send a shadow ray toward the light
	Vector3D direction = light->get_point_on_light(ray.get_random_u(), ray.get_random_v()) - intersection_point;
	//direction.normalize();
	Ray shadow_ray(direction, intersection_point);

	/* Numerical imprecision may result in object shadowing itself
	 * Thus, set tmin = eps for some small eps = 1.0e − 3
	 */
	float t_min = SMALL_EPS;
	float t_max = 1.0f;
	HitStructure hit;
	Shader::intersect_all_objects(objects, shadow_ray, t_min, t_max, hit);

	return (hit.obj != 0 && hit.shader->get_refraction_coefficent() == 0.0f); // Hit and not a dialectric object
}

/* Calculates the color of a reflection ray */
Vector3D Shader::apply_reflection(const vector<Object *> &objects, const vector<Light *> &lights, Ray &reflection_ray, int depth){

	#define MAX_REFLECTION_DEPTH 7
	if (depth < MAX_REFLECTION_DEPTH){
		// Do we need to bend the reflected ray?
		if (roughness > 0.0f){
			reflection_ray.distorce_ray(roughness);
		}

		float t_min = SMALL_EPS;
		float t_max = FLT_MAX;

		HitStructure hit;
		Shader::intersect_all_objects(objects, reflection_ray, t_min, t_max, hit);

		if (hit.obj != 0){ // Hit?
			// Create shader args structure
			ShaderArgs sa(hit, reflection_ray, lights, objects, depth + 1, t_max, true);

			Vector3D color = hit.shader->apply_shader(sa);
			return color;
		}
	}

	return Vector3D(0.0f, 0.0f, 0.0f);
	//return Shader::background_color;
}

Vector3D Shader::get_refraction_color(const ShaderArgs &sa, const Vector3D &d, const Vector3D &normal, const Vector3D &reflection_color){
	float n_index = 1.61f; // Glass
	Ray refraction_ray;

	if (d.dot(normal) < 0){
		// Refraction ray going into the object
		Ray::compute_refraction(d, normal, sa.hit.intersection_point, 1.0f/n_index, refraction_ray);
		return Shader::apply_refraction(sa.objects, sa.lights, refraction_ray, sa.recursion_call_depth);
	}else{
		// Refraction ray coming out of the object
		Vector3D negative_normal;
		negative_normal -= normal;
		if (Ray::compute_refraction(d, negative_normal, sa.hit.intersection_point, n_index, refraction_ray)){
			return Shader::apply_refraction(sa.objects, sa.lights, refraction_ray, sa.recursion_call_depth);
		}else{
			return reflection_color;
		}
	}
}

Vector3D Shader::apply_refraction(const vector<Object *> &objects, const vector<Light *> &lights, Ray &refraction_ray, int depth){

	#define MAX_REFRACTION_DEPTH 20
	if (depth < MAX_REFRACTION_DEPTH){
		float t_min = SMALL_EPS;
		float t_max = FLT_MAX;

		HitStructure hit;
		Shader::intersect_all_objects(objects, refraction_ray, t_min, t_max, hit);

		if (hit.obj != 0){ // Hit?
			// Create shader args structure
			ShaderArgs sa(hit, refraction_ray, lights, objects, depth + 1, t_max, false);

			Vector3D color = hit.shader->apply_shader(sa);
			return color;
		}
	}

	return Vector3D(0.0f, 0.0f, 0.0f);
}

/* Iterates through the list of objects and intersects all of them.
 * If an object is hit during the way, it will be stored in hit.obj
 */
void Shader::intersect_all_objects(const vector<Object *> &objects, const Ray &ray, float &t_min, float &t_max, HitStructure &hit){
	for (int i = 0; i<objects.size(); i++){
		objects[i]->intersect(ray, t_min, t_max, hit);
	}
}


/* Certain shaders do not need to be read from file */
Shader::Shader(const string& name){
	this->name = name;
}

Shader::~Shader(){
	diffuse_texture = 0;
	LOG("  (Base Shader destructor called: " << name << ")");
}

