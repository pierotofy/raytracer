/*
 * GlazeShader.h
 *
 *  Created on: 2012-02-16
 *      Author: piero
 */

#ifndef GLAZESHADER_H_
#define GLAZESHADER_H_

#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "LambertianShader.h"

using namespace std;
using namespace sivelab;

namespace ptrt {

class GlazeShader: public ptrt::Shader {
private:
	float mirror_coefficent;

	friend ostream& operator<<(ostream&, ptrt::GlazeShader &);
public:
	GlazeShader(map<string, SceneDataContainer> &);
	Vector3D apply_shader_for_light(const ShaderArgs &, Light *, const Vector3D &, const Vector3D &, const Vector3D &);
	virtual ~GlazeShader();
	ostream& print(ostream&);
};

} /* namespace ptrt */
#endif
