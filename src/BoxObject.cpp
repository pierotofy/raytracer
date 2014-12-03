/*
 * BoxObject.cpp
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */
#include <sstream>
#include "Object.h"
#include "BoxObject.h"
#include "Utils.h"
#include "Globals.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::BoxObject& b)
	{
	  return b.print(os);
	}

	ostream& BoxObject::print(std::ostream& os = std::cout){
		  os << "BoxObject {" << endl;
		  Object::print(os);

//		  for (int i = 0; i<triangles.size(); i++){
//			  os << *triangles[i];
//		  }

		  os << "  min_pt: " << min_pt << endl
		     << "  max_pt: " << max_pt << endl
		     << "}" << endl;

		  return os;
	}
}

BoxObject::BoxObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders)
	: Object(sdMap, shaders) {
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shape_minPt");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> min_pt;
	buf.clear();

	sdIter = sdMap.find("shape_maxPt");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> max_pt;
	buf.clear();

	// Create triangles

	// Top face
	triangles.push_back(new TriangleObject(sdMap, shaders,
			Vector3D(max_pt), Vector3D(max_pt[0], max_pt[1], min_pt[2]), Vector3D(min_pt[0], max_pt[1], min_pt[2])));

	triangles.push_back(new TriangleObject(sdMap, shaders,
				Vector3D(max_pt), Vector3D(min_pt[0], max_pt[1], min_pt[2]), Vector3D(min_pt[0], max_pt[1], max_pt[2])));

	// Bottom face
	triangles.push_back(new TriangleObject(sdMap, shaders,
			Vector3D(min_pt), Vector3D(max_pt[0], min_pt[1], max_pt[2]), Vector3D(min_pt[0], min_pt[1], max_pt[2])));
	triangles.push_back(new TriangleObject(sdMap, shaders,
				Vector3D(min_pt), Vector3D(max_pt[0], min_pt[1], min_pt[2]), Vector3D(max_pt[0], min_pt[1], max_pt[2])));

	// Left face
	triangles.push_back(new TriangleObject(sdMap, shaders,
			Vector3D(min_pt), Vector3D(min_pt[0], min_pt[1], max_pt[2]), Vector3D(min_pt[0], max_pt[1], max_pt[2])));
	triangles.push_back(new TriangleObject(sdMap, shaders,
				Vector3D(min_pt), Vector3D(min_pt[0], max_pt[1], max_pt[2]), Vector3D(min_pt[0], max_pt[1], min_pt[2])));

	// Right face
	triangles.push_back(new TriangleObject(sdMap, shaders,
			Vector3D(max_pt), Vector3D(max_pt[0], min_pt[1], max_pt[2]), Vector3D(max_pt[0], min_pt[1], min_pt[2])));

	triangles.push_back(new TriangleObject(sdMap, shaders,
				Vector3D(max_pt), Vector3D(max_pt[0], min_pt[1], min_pt[2]), Vector3D(max_pt[0], max_pt[1], min_pt[2])));

	// Front face
	triangles.push_back(new TriangleObject(sdMap, shaders,
			Vector3D(max_pt), Vector3D(min_pt[0], min_pt[1], max_pt[2]), Vector3D(max_pt[0], min_pt[1], max_pt[2])));

	triangles.push_back(new TriangleObject(sdMap, shaders,
				Vector3D(max_pt), Vector3D(min_pt[0], max_pt[1], max_pt[2]), Vector3D(min_pt[0], min_pt[1], max_pt[2])));

	// Back face
	triangles.push_back(new TriangleObject(sdMap, shaders,
			Vector3D(min_pt), Vector3D(max_pt[0], max_pt[1], min_pt[2]), Vector3D(max_pt[0], min_pt[1], min_pt[2])));
	triangles.push_back(new TriangleObject(sdMap, shaders,
				Vector3D(min_pt), Vector3D(min_pt[0], max_pt[1], min_pt[2]), Vector3D(max_pt[0], max_pt[1], min_pt[2])));

}

bool BoxObject::intersect(const Ray& ray, float &t_min, float &t_max, HitStructure &hit){
	Object *first = 0;

	for (int i = 0; i<triangles.size(); i++){
		if (triangles[i]->intersect(ray, t_min, t_max, hit)){
			first = triangles[i];
		}
	}

	if (first != 0){
		hit.obj = this;
		return true;
	}else{
		return false;
	}
}

void BoxObject::generate_bounding_box(){
	bounding_box = BoundingBox(max_pt, min_pt);
}

void BoxObject::get_uv_coords(const Vector3D &intersection_point, float &u, float &v){
	if (FEQUAL(intersection_point[2], max_pt[2]) ||
		FEQUAL(intersection_point[2], min_pt[2])){
		// X Y plane
		u = (intersection_point[0] - min_pt[0]) / (max_pt[0] - min_pt[0]);
		v = (intersection_point[1] - min_pt[1]) / (max_pt[1] - min_pt[1]);
	}else if (FEQUAL(intersection_point[1], max_pt[1]) ||
			FEQUAL(intersection_point[1], min_pt[1])){
		// X Z plane
		u = (intersection_point[0] - min_pt[0]) / (max_pt[0] - min_pt[0]);
		v = (intersection_point[2] - min_pt[2]) / (max_pt[2] - min_pt[2]);

	}else if (FEQUAL(intersection_point[0], max_pt[0]) ||
			FEQUAL(intersection_point[0], min_pt[0])){
		// Y Z plane
		u = (intersection_point[1] - min_pt[1]) / (max_pt[1] - min_pt[1]);
		v = (intersection_point[2] - min_pt[2]) / (max_pt[2] - min_pt[2]);
	}else{
		assert(false);
	}
}

BoxObject::~BoxObject() {
	for (int i = 0; i<triangles.size(); i++)
		RELEASE_SAFELY(triangles[i]);
	triangles.clear();

	LOG("Destroyed box shape: " << name);
} /* namespace ptrt */

