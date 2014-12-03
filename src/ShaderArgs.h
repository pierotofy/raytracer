/*
 * ShaderArgs.h
 *
 *  Created on: 2012-02-14
 *      Author: piero
 */

#ifndef SHADERARGS_H_
#define SHADERARGS_H_
#include <vector>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "HitStructure.h"

using namespace std;
using namespace sivelab;

namespace ptrt{

class Light;
class Object;

/* Arguments passed on a call to apply_shader */
struct ShaderArgs{
	const HitStructure &hit;
	const Ray ray;
	const vector<Light *> lights;
	const vector<Object *> objects;
	int recursion_call_depth;
	float t_value;
	bool computing_reflection;

	ShaderArgs(const HitStructure &hit, const Ray &ray, const vector<Light *> &lights, const vector<Object *> &objects, int recursion_call_depth, float t_value, bool computing_reflection)
	  : hit(hit), ray(ray), lights(lights), objects(objects), recursion_call_depth(recursion_call_depth), t_value(t_value), computing_reflection(computing_reflection){
	}
};

}

#endif /* SHADERARGS_H_ */
