/*
 * NormalMapShader.h
 *
 *  Created on: Feb 7, 2012
 *      Author: piero
 */

#ifndef NORMALMAPSHADER_H_
#define NORMALMAPSHADER_H_

#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "Shader.h"

namespace ptrt {

class NormalMapShader: public ptrt::Shader {
private:
	friend ostream& operator<<(ostream&, ptrt::NormalMapShader &);
public:
	NormalMapShader(map<string, SceneDataContainer> &);
	virtual ~NormalMapShader();
	Vector3D apply_shader(const ShaderArgs &sa);
	Vector3D apply_shader_for_light(const ShaderArgs &, Light *, const Vector3D &, const Vector3D &, const Vector3D &){ assert(false); return Vector3D(); }
	ostream& print(ostream&);
};

}

#endif /* NORMALMAPSHADER_H_ */
