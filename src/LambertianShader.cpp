/*
 * LambertianShader.cpp
 *
 *  Created on: 2012-01-31
 *      Author: piero
 */

#include <iostream>
#include "TriangleObject.h"
#include "LambertianShader.h"
#include "Utils.h"
#include <algorithm>

using namespace std;

namespace ptrt {

std::ostream &operator<<(std::ostream& os, ptrt::LambertianShader& s)
{
  s.print(os);
  return os;
}

ostream& LambertianShader::print(std::ostream& os = std::cout){
	  os << "LambertianShader {" << endl;
	  Shader::print(os);
	  os << "}" << endl;
	  return os;
}

LambertianShader::LambertianShader(map<string, SceneDataContainer> &sdMap)
 : Shader(sdMap){
}

Vector3D LambertianShader::apply_shader_for_light(const ShaderArgs &sa, Light *light, const Vector3D &l, const Vector3D &normal, const Vector3D &v){
	// kd = diffuse component
	// I = light source intensity
	// n = surface normal
	// l = vector in direction of light
	// L = sum(kd*I*max(0, n dot l)
	return (get_diffuse(sa.hit.intersection_point, sa.hit.obj) * light->get_intensity()) * max(0.0f, (float)normal.dot(l));
}

LambertianShader::~LambertianShader() {
	LOG("Destroyed lambertian shader: " << name);
}

} /* namespace ptrt */
