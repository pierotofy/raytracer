/*
 * PyramidObject.cpp
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */
#include <sstream>
#include "Object.h"
#include "PyramidObject.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::PyramidObject& b)
	{
	  return b.print(os);
	}

	ostream& PyramidObject::print(std::ostream& os = std::cout){
		  os << "PyramidObject {" << endl;
		  Object::print(os);

		  for (int i = 0; i<triangles.size(); i++){
			  os << *triangles[i];
		  }

		  os << "  center: " << center << endl
		     << "  height: " << height << endl
		     << "  baselength: " << baselength << endl
		     << "}" << endl;

		  return os;
	}
}

PyramidObject::PyramidObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders)
	: Object(sdMap, shaders) {
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shape_center");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> center;
	buf.clear();

	sdIter = sdMap.find("shape_height");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> height;
	buf.clear();

	sdIter = sdMap.find("shape_baselength");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> baselength;
	buf.clear();

	// Create triangles
	Vector3D a = center;
	a[0] -= baselength/2.0f;
	a[2] -= baselength/2.0f;

	Vector3D b = a;
	b[0] += baselength;

	Vector3D c = b;
	c[2] += baselength;

	Vector3D d = c;
	d[0] -= baselength;

	Vector3D top = center;
	top[1] += height;

	// Bottom
	triangles.push_back(new TriangleObject(sdMap, shaders, a, c, d));
	triangles.push_back(new TriangleObject(sdMap, shaders, a, c, b));

	// Front
	triangles.push_back(new TriangleObject(sdMap, shaders, a, top, b));

	// Right
	triangles.push_back(new TriangleObject(sdMap, shaders, b, top, c));

	// Back
	triangles.push_back(new TriangleObject(sdMap, shaders, c, top, d));

	// Left
	triangles.push_back(new TriangleObject(sdMap, shaders, d, top, a));

}

bool PyramidObject::intersect(const Ray& ray, float &t_min, float &t_max, HitStructure &hit){
	Object *first = 0;

	for (int i = 0; i<triangles.size(); i++){
		if (triangles[i]->intersect(ray, t_min, t_max, hit)){
			first = triangles[i];
		}
	}

	return first != 0;
}

void PyramidObject::generate_bounding_box(){
	Vector3D max_pt;
	Vector3D min_pt;

	max_pt = center;
	max_pt[0] += baselength/2.0f;
	max_pt[1] += height;
	max_pt[2] += baselength/2.0f;

	min_pt = center;
	min_pt[0] -= baselength/2.0f;
	min_pt[2] -= baselength/2.0f;

	bounding_box = BoundingBox(max_pt, min_pt);
}

PyramidObject::~PyramidObject() {
	for (int i = 0; i<triangles.size(); i++)
		RELEASE_SAFELY(triangles[i]);
	triangles.clear();

	LOG("Destroyed pyramid shape: " << name);
} /* namespace ptrt */

