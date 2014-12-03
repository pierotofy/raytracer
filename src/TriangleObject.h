/*
 * TriangleObject.h
 *
 *  Created on: 2012-01-31
 *      Author: piero
 */

#ifndef TRIANGLEOBJECT_H_
#define TRIANGLEOBJECT_H_

#include "Object.h"

namespace ptrt {

class TriangleObject: public ptrt::Object {
private:

	// Use clockwise order a,b,c
	Vector3D a_vertex, b_vertex, c_vertex;
	Vector3D normal;

	// Some values precalculated for intersection
	float a, b, c, d, e, f;
	float ae, dc, bf, ce, fa, bd;

	friend ostream& ptrt::operator<<(ostream&, ptrt::TriangleObject &);
protected:
	virtual bool intersect(const Ray&, float &, float &);
	virtual Vector3D get_normal(const Vector3D&);
	virtual void generate_bounding_box();
public:
	TriangleObject(std::map<std::string, SceneDataContainer> &, map<string, Shader *> &, const Vector3D &, const Vector3D &, const Vector3D &);
	ostream& print(ostream&);
	virtual ~TriangleObject();
};

} /* namespace ptrt */
#endif /* TRIANGLEOBJECT_H_ */
