/*
 * LambertianShader.cpp
 *
 *  Created on: 2012-01-31
 *      Author: piero
 */

#include <sstream>
#include <math.h>
#include "BlinnphongShader.h"
#include "Utils.h"

using namespace std;

namespace ptrt {

std::ostream &operator<<(std::ostream& os, ptrt::BlinnphongShader& s)
{
  s.print(os);
  return os;
}

ostream& BlinnphongShader::print(std::ostream& os = std::cout){
	  os << "BlinnphongShader {" << endl;
	  LambertianShader::print(os);
	  os << "  specular: " << specular << endl
		 << "  phong_exp: " << phong_exp << endl
		 << "  mirror_coefficent: " << mirror_coefficent << endl
	     << "}" << endl;
	  return os;
}

BlinnphongShader::BlinnphongShader(map<string, SceneDataContainer> &sdMap)
 : LambertianShader(sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shader_specular");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> specular;
	buf.clear();


	sdIter = sdMap.find("shader_phongExp");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.isSet){
		buf.str(sdIter->second.val);
		buf >> phong_exp;
		buf.clear();
	}else{
		phong_exp = 0.0f;
	}
}

Vector3D BlinnphongShader::apply_shader_for_light(const ShaderArgs &sa, Light *light, const Vector3D &l, const Vector3D &normal, const Vector3D &v){
	Vector3D L = LambertianShader::apply_shader_for_light(sa, light, l, normal, v);

	// ks*I*max(0, n · h)^p
	// ks = specular energy
	// p = phong exponent
	// h = bisector between l (direction to light) and v (direction to incoming ray)
	// I = light source intensity

	Vector3D h = v + l;
	h.normalize();

	// Add to lambertian color
	L += specular * light->get_intensity() * pow(max(0.0f, (float)normal.dot(h)), phong_exp);

	return L;
}

BlinnphongShader::~BlinnphongShader() {
	LOG("Destroyed blinnphong shader: " << name);
}

} /* namespace ptrt */
