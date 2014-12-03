#include <sstream>
#include <assert.h>
#include "Object.h"
#include "InstanceObject.h"
#include "InstanceManager.h"
#include "Transform.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{

std::ostream &operator<<(std::ostream& os, ptrt::InstanceObject& o)
{
  o.print(os);
  return os;
}

ostream& InstanceObject::print(std::ostream& os = std::cout){
	  os << "  InstanceObject {" << endl
		 << "    name: " << name << endl
		 << "    instance: " << *instance << endl
		 << "    matrix: " << endl << transform_matrix << endl
		 << "  }" << endl;
	  return os;
}

InstanceObject::~InstanceObject(){
	LOG("  (Instance object destructor called: " << name << ")");

	instance = 0; // This was just a reference, instance manager will dispose it
}

InstanceObject::InstanceObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders)
 : Object(sdMap, shaders){
    std::map<std::string, SceneDataContainer>::iterator sdIter;
	sdIter = sdMap.find("shape_name");
	assert( sdIter != sdMap.end() );
	name = sdIter->second.val;

	// Find instance from instance manager
	string instanceId;
	sdIter = sdMap.find("shape_id");
	assert( sdIter != sdMap.end() );
	instanceId = sdIter->second.val;

	instance = InstanceManager::getSingleton()->get_instance(instanceId);

	// Do we have a shader specified or do we use the default one?
	if (shader == 0){
		if (instance->get_default_shader() != 0){
			shader = instance->get_default_shader();
		}else{
			log_exception("No shader specified and no default shader for this instance", true);
		}
	}

	// Read transforms
	Transform t(sdMap);
	transform_matrix = t.get_matrix();
	transform_matrix_inverse = transform_matrix.Inverse();
	transform_matrix_inverse_transpose = transform_matrix_inverse.Transpose();
}

bool InstanceObject::intersect(const Ray& ray, float &t_min, float &t_max, HitStructure &hit){
	Ray transformed_ray(transform_matrix_inverse.multiply(ray.get_direction(), 0.0f), // 0.0f = direction
			transform_matrix_inverse.multiply(ray.get_origin(), 1.0f)); // 1.0f = point

	bool intersected = instance->get_object()->intersect(transformed_ray, t_min, t_max, hit);
	if (intersected){
		// Already have intersection point (and a normal, but they need to be transformed)

		Vector3D transformed_intersection_point = transform_matrix.multiply(hit.intersection_point, 1.0f);
		Vector3D transformed_normal = transform_matrix_inverse_transpose.multiply(hit.normal, 0.0f);
		transformed_normal.normalize();

		hit.intersection_point = transformed_intersection_point;
		hit.normal = transformed_normal;
		hit.shader = shader;
		hit.obj = this;

		return true;
	}else{
		return false;
	}
}

void InstanceObject::get_uv_coords(const Vector3D &intersection_point, float &u, float &v){
	Vector3D transformed_intersection_point = transform_matrix_inverse.multiply(intersection_point, 1.0f);
	instance->get_object()->get_uv_coords(transformed_intersection_point, u, v);
}

void InstanceObject::generate_bounding_box(){
	bounding_box = instance->get_object()->get_bounding_box().axis_aligned_transform(transform_matrix);
}

}

