/*
 * NormalMapShader.cpp
 *
 *  Created on: Feb 7, 2012
 *      Author: piero
 */

#include "NormalMapShader.h"
#include "Utils.h"

using namespace std;

namespace ptrt {

std::ostream &operator<<(std::ostream& os, ptrt::NormalMapShader& s)
{
  s.print(os);
  return os;
}

ostream& NormalMapShader::print(std::ostream& os = std::cout){
	  os << "NormalMapShader {" << endl;
	  Shader::print(os);
	  os << "}" << endl;
	  return os;
}

NormalMapShader::NormalMapShader(map<string, SceneDataContainer> &sdMap)
 : Shader(sdMap){

}

Vector3D NormalMapShader::apply_shader(const ShaderArgs &sa){
	Vector3D normal = sa.hit.normal;

	return Vector3D((normal[0]*0.5f + 0.5f),
			(normal[1]*0.5f + 0.5f),
			(normal[2]*0.5f + 0.5f));
}

NormalMapShader::~NormalMapShader() {
	LOG("Destroyed normal map shader: " << name);
}


}
