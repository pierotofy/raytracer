#include <sstream>
#include "PointLight.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, const ptrt::PointLight& l)
	{
	  l.print(os);
	  return os;
	}

	ostream& PointLight::print(std::ostream& os = std::cout) const{
		  os << "PointLight {" << endl;
		  Light::print(os);
	      os << "}" << endl;

		  return os;
	}
}

PointLight::PointLight(std::map<std::string, SceneDataContainer> &sdMap)
 : Light(sdMap){

}

PointLight::~PointLight(){
	LOG("Destroyed point light: " << id);
}


