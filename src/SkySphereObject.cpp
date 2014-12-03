/*
 * SkySphereObject.cpp
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */
#include <sstream>
#include "SkySphereObject.h"
#include <cmath>
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::SkySphereObject& s)
	{
	  return s.print(os);
	}
}

ostream& SkySphereObject::print(std::ostream& os = std::cout){
	  os << "SkySphereObject {" << endl;
	  SphereObject::print(os);
	  os << "}" << endl;

	  return os;
}

SkySphereObject::SkySphereObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders)
	: SphereObject(sdMap, shaders) {

}

bool SkySphereObject::intersect(const Ray& ray, float &t_min, float &t_max, HitStructure &hit){
	bool intersected = Object::intersect(ray, t_min, t_max, hit);
	if (intersected){
		hit.background_component = true;
		return true;
	}else{
		return false;
	}
}

/** A sky sphere has the normals pointing inward instead of outward */
Vector3D SkySphereObject::get_normal(const Vector3D& intersection_point){
	Vector3D normal = SphereObject::get_normal(intersection_point);
	Vector3D negative_normal;
	negative_normal -= normal;
	return negative_normal;
}


SkySphereObject::~SkySphereObject() {
	LOG("Destroyed sky sphere shape: " << name);
}

