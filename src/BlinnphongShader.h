/*
 * Blinnphong.h
 *
 *  Created on: 2012-01-31
 *      Author: piero
 */

#ifndef BLINNPHONGSHADER_H_
#define BLINNPHONGSHADER_H_

#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "LambertianShader.h"

using namespace std;
using namespace sivelab;

namespace ptrt {

// Blinnphong is a shader on top of a lambertian shader
class BlinnphongShader: public ptrt::LambertianShader {
private:
	Vector3D specular;
	float phong_exp;

	friend ostream& operator<<(ostream&, ptrt::BlinnphongShader &);
public:
	BlinnphongShader(map<string, SceneDataContainer> &);
	Vector3D apply_shader_for_light(const ShaderArgs &, Light *, const Vector3D &, const Vector3D &, const Vector3D &);
	virtual ~BlinnphongShader();
	ostream& print(ostream&);
};

} /* namespace ptrt */
#endif
