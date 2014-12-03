/*
 * HitStructure.h
 *
 *  Created on: 2012-02-14
 *      Author: piero
 */

#ifndef HITSTRUCTURE_H_
#define HITSTRUCTURE_H_
#include <vector>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"

using namespace std;
using namespace sivelab;

namespace ptrt{

class Shader;
class Object;

/* Arguments passed from a call to intersect */
struct HitStructure{
	Vector3D intersection_point;
	Vector3D normal;
	float t;
	Shader *shader;
	Object *obj;
	bool background_component;

	HitStructure() { obj = 0; background_component = false;  }
};

}

#endif /* HITSTRUCTURE_H_ */
