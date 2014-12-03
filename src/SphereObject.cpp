/*
 * SphereObject.cpp
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */
#include <sstream>
#include "Object.h"
#include "Shader.h"
#include "SphereObject.h"
#include <cmath>
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::SphereObject& s)
	{
	  return s.print(os);
	}
}

ostream& SphereObject::print(std::ostream& os = std::cout){
	  os << "SphereObject {" << endl;
	  Object::print(os);
	  os << "  center: " << center << endl
	     << "  radius: " << radius << endl
	     << "}" << endl;

	  return os;
}

SphereObject::SphereObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders)
	: Object(sdMap, shaders) {

	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shape_center");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> center;
	buf.clear();

	sdIter = sdMap.find("shape_radius");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> radius;
	buf.clear();

	radius_squared = radius*radius; // Calculate the square right away, it will save time later
}

bool SphereObject::intersect(const Ray& ray, float &t_min, float &t_max){
	//(rd ·rd )t^2 +2rd ·(ro −center )t+(ro −center )·(ro −center )−R^2 = 0

	double a = ray.get_direction().dot(ray.get_direction());
	double b = (2.0f * ray.get_direction()).dot(ray.get_origin() - center);
	double c = (ray.get_origin() - center).dot(ray.get_origin() - center) - radius_squared;

	double discriminant = b*b - 4.0f*a*c;

	if (discriminant < 0.0f) return false;
	else{
		double t1 = (-b + sqrt(discriminant))/(2.0f*a);
		double t2 = (-b - sqrt(discriminant))/(2.0f*a);

		bool acceptable_t1 = false;
		bool acceptable_t2 = false;

		if (t1 < t_max && t1 > t_min){
			// t1 is valid
			acceptable_t1 = true;
		}

		if (t2 < t_max && t2 > t_min){
			// t2 is also valid
			acceptable_t2 = true;
		}

		// Both acceptable?
		if (acceptable_t1 && acceptable_t2){
			if (t1 < t2){
				t_max = t1;
				return true;
			}else if (t2 < t1){
				t_max = t2;
				return true;
			}
		}else if (acceptable_t1){
			t_max = t1;
			return true;
		}else if (acceptable_t2){
			t_max = t2;
			return true;
		}else{
			return false;
		}
	}
}

Vector3D SphereObject::get_normal(const Vector3D& intersection_point){
	Vector3D normal = intersection_point - center;
	normal.normalize();
	return normal;
}

void SphereObject::get_uv_coords(const Vector3D &intersection_point, float &u, float &v){
	#define PI 3.14159265f

	float theta = acos((intersection_point[2] - center[2])/radius);
	float phi = atan2(intersection_point[1] - center[1], intersection_point[0] - center[0]);

	u = 0.5f - (phi / (2 * PI));
	v = (PI - theta) / PI;

}

void SphereObject::generate_bounding_box(){
	// Start at the center
	Vector3D max_pt;
	Vector3D min_pt;

	// Y components
	max_pt[1] = center[1] + radius;
	min_pt[1] = center[1] - radius;

	// X components
	max_pt[0] = center[0] + radius;
	min_pt[0] = center[0] - radius;

	// Z components
	max_pt[2] = center[2] + radius;
	min_pt[2] = center[2] - radius;

	bounding_box = BoundingBox(max_pt, min_pt);
}

SphereObject::~SphereObject() {
	LOG("Destroyed sphere shape: " << name);
}

