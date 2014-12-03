/*
 * TriangleObject.cpp
 *
 *  Created on: 2012-01-31
 *      Author: piero
 */

#include <sstream>
#include "Object.h"
#include "TriangleObject.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::TriangleObject& b)
	{
	  return b.print(os);
	}

	ostream& TriangleObject::print(std::ostream& os = std::cout){
		  os << "TriangleObject {" << endl;
		  Object::print(os);
		  os << "a: " << a_vertex << endl
		     << "b: " << b_vertex << endl
		     << "c: " << c_vertex << endl
		     << "}" << endl;
		  return os;
	}
}

bool TriangleObject::intersect(const Ray& ray, float &t_min, float &t_max){
	Vector3D rd = ray.get_direction();
	Vector3D ro = ray.get_origin();

	float g = rd[0]; float h = rd[1]; float i = rd[2];
	float j = a_vertex[0] - ro[0]; float k = a_vertex[1] - ro[1]; float l = a_vertex[2] - ro[2];

	// Determinant
	float M = ae*i + dc*h + g*bf
			- ce*g - fa*h - i*bd;

	if (M == 0) return false;
	assert(M != 0);

/*
	t=−(f (ak − jb) + e(jc − al) + d(bl − kc))/M
	γ= (i (ak − jb) + h(jc − al) + g (bl − kc))/M
	β= (j(ei − hf ) + k(gf − di ) + l(dh − eg ))/M
*/
	float ak = a*k;
	float jb = j*b;
	float jc = j*c;
	float al = a*l;
	float bl = b*l;
	float kc = k*c;

	float t = -(f*(ak - jb) + e*(jc - al) + d*(bl - kc)) / M;
	if (t <= t_min || t >= t_max){
		return false;
	}

	float y = (i*(ak - jb) + h*(jc - al) + g*(bl - kc)) / M;
	if (y < 0.0f || y > 1.0f){
		return false;
	}

	float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g)) / M;
	if (beta < 0.0f || beta > 1.0f - y){
		return false;
	}

	t_max = t;
	return true;
}

/* @param a,b,c vertices of the triangle */
TriangleObject::TriangleObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders, const Vector3D &a_vertex, const Vector3D &b_vertex, const Vector3D &c_vertex)
 : Object(sdMap, shaders){
	name = "vertex";

	this->a_vertex = a_vertex;
	this->b_vertex = b_vertex;
	this->c_vertex = c_vertex;

	this->normal = (b_vertex - a_vertex).cross(c_vertex - a_vertex);
	this->normal.normalize();

	generate_bounding_box();

	// Precalculate a few values for the intersection
	a = a_vertex[0] - b_vertex[0];
	b = a_vertex[1] - b_vertex[1];
	c = a_vertex[2] - b_vertex[2];
	d = a_vertex[0] - c_vertex[0];
	e = a_vertex[1] - c_vertex[1];
	f = a_vertex[2] - c_vertex[2];

	ae = a*e;
	dc = d*c;
	bf = b*f;
	ce = c*e;
	fa = f*a;
	bd = b*d;
}

/* We never use intersection point as a triangle's normal remains unchanged */
Vector3D TriangleObject::get_normal(const Vector3D& intersection_point){
	return normal;
}

void TriangleObject::generate_bounding_box(){
	Vector3D max_pt;
	Vector3D min_pt;

	max_pt[0] = max(max(a_vertex[0], b_vertex[0]), c_vertex[0]);
	max_pt[1] = max(max(a_vertex[1], b_vertex[1]), c_vertex[1]);
	max_pt[2] = max(max(a_vertex[2], b_vertex[2]), c_vertex[2]);

	min_pt[0] = min(min(a_vertex[0], b_vertex[0]), c_vertex[0]);
	min_pt[1] = min(min(a_vertex[1], b_vertex[1]), c_vertex[1]);
	min_pt[2] = min(min(a_vertex[2], b_vertex[2]), c_vertex[2]);

	bounding_box = BoundingBox(max_pt, min_pt);
}

TriangleObject::~TriangleObject() {
	LOG("Destroyed triangle object " << name);
}

