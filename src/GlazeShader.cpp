/*
 * GlazeShader.cpp
 *
 *  Created on: 2012-02-15
 *      Author: piero
 */

#include <sstream>
#include <math.h>
#include "GlazeShader.h"
#include "Utils.h"

using namespace std;

namespace ptrt {

std::ostream &operator<<(std::ostream& os, ptrt::GlazeShader& s)
{
  s.print(os);
  return os;
}

ostream& GlazeShader::print(std::ostream& os = std::cout){
	  os << "GlazeShader {" << endl;
	  Shader::print(os);
	  os << "  mirror_coefficent: " << mirror_coefficent << endl
	     << "}" << endl;
	  return os;
}

GlazeShader::GlazeShader(map<string, SceneDataContainer> &sdMap)
 : Shader(sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shader_mirrorCoef");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.isSet){
		buf.str(sdIter->second.val);
		buf >> mirror_coefficent;
		buf.clear();
	}else{
		mirror_coefficent = 0.0f;
	}
}

Vector3D GlazeShader::apply_shader_for_light(const ShaderArgs &sa, Light *light, const Vector3D &l, const Vector3D &normal, const Vector3D &v){
	Vector3D L;

	Vector3D light_intensity_avg;
	for (int i = 0; i<sa.lights.size(); i++){
		light_intensity_avg += sa.lights[i]->get_intensity();
	}
	light_intensity_avg /= sa.lights.size();

	L += diffuse_component * light->get_intensity() * light_intensity_avg;

	return L;
}

GlazeShader::~GlazeShader() {
	LOG("Destroyed glaze shader: " << name);
}

} /* namespace ptrt */
