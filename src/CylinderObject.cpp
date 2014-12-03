/*
 * CylinderObject.cpp
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */
#include <sstream>
#include <cmath>
#include "Object.h"
#include "Shader.h"
#include "CylinderObject.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::CylinderObject& s)
	{
	  return s.print(os);
	}
}

ostream& CylinderObject::print(std::ostream& os = std::cout){
	  os << "CylinderObject {" << endl;
	  SphereObject::print(os);
	  os << "  height: " << radius << endl
	     << "}" << endl;

	  return os;
}

CylinderObject::CylinderObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders)
	: SphereObject(sdMap, shaders) {

	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shape_height");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> height;
	buf.clear();
}

/* A cylinder is a sphere, but we don't care about the Y component */
bool CylinderObject::intersect(const Ray& ray, float &t_min, float &t_max){
	Ray original_ray = ray; // Need to make a copy, const complains about calling get_collision_point

	Vector3D rd = ray.get_direction();
	Vector3D ro = ray.get_origin();
	rd[1] = 0.0f;
	ro[1] = 0.0f;

	// We save the origin of the ray for the next normal calculation
	latest_ray_origin = ro;

	float front_t_min = t_min;
	float front_t_max = t_max;

	Ray y_less_ray(rd, ro);

	if (SphereObject::intersect(y_less_ray, front_t_min, front_t_max)){
		Vector3D front_collision_point = original_ray.get_collision_point(front_t_max);
		if (fabs(front_collision_point[1] - center[1]) < height/2.0f){
			// Hit the front part of the cylinder and within height
			t_max = front_t_max;
			return true;
		}else{
			// We hit the front part of the "infinite" cylinder and we are too tall

			// Did we also hit the back part of the cylinder?
			float back_t_min = front_t_max + SMALL_EPS;
			float back_t_max = FLT_MAX;

			if (SphereObject::intersect(y_less_ray, back_t_min, back_t_max)){

				// We hit the back part
				Vector3D back_collision_point = original_ray.get_collision_point(back_t_max);
				if (fabs(back_collision_point[1] - center[1]) < height/2.0f){
					t_max = back_t_max;
					return true; // Inside height range
				}else{
					return false; // Outside of height range
				}
			}else{
				return false;  // No back hit
			}
		}
	}else{
		return false;
	}
}

/* Again, we don't care about the Y component */
Vector3D CylinderObject::get_normal(const Vector3D& intersection_point){
	Vector3D y_less_intersection_point = intersection_point;
	y_less_intersection_point[1] = 0.0f;

	Vector3D y_less_center = center;
	y_less_center[1] = 0.0f;

	Vector3D normal = y_less_intersection_point - y_less_center;
	normal.normalize();

	if (fabs(latest_ray_origin[2] - center[2]) < fabs(latest_ray_origin[2] - intersection_point[2])){
		// Back normal, need to flip the Z coordinate (so that we can see the inside of the cylinder)
		normal[2] = -normal[2];
	}

	return normal;
}

CylinderObject::~CylinderObject() {
	LOG("Destroyed cylinder shape: " << name);
}

