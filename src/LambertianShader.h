/*
 * LambertianShader.h
 *
 *  Created on: 2012-01-31
 *      Author: piero
 */

#ifndef LAMBERTIANSHADER_H_
#define LAMBERTIANSHADER_H_

#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "Shader.h"

using namespace std;
using namespace sivelab;

namespace ptrt {

class LambertianShader: public ptrt::Shader {
private:

	friend ostream& operator<<(ostream&, ptrt::LambertianShader &);
public:
	LambertianShader(map<string, SceneDataContainer> &);
	virtual Vector3D apply_shader_for_light(const ShaderArgs &, Light *, const Vector3D &, const Vector3D &, const Vector3D &);
	virtual ~LambertianShader();
	ostream& print(ostream&);
};

} /* namespace ptrt */
#endif /* LAMBERTIANSHADER_H_ */
