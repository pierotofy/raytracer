#include <sstream>
#include "Light.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, const ptrt::Light& l)
	{
		l.print(os);
		return os;
	}

	ostream& Light::print(std::ostream& os = std::cout) const{
		  os << "Light {" << endl
		     << "  id: " << id << endl
		     << "  position: " << position << endl
		     << "  intensity: " << intensity << endl
			 << "}" << endl;
		  return os;
	}
}

int Light::instances_count = 0;

Light::Light(std::map<std::string, SceneDataContainer> &sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("light_position");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> position;
	buf.clear();

	sdIter = sdMap.find("light_intensity");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> intensity;
	buf.clear();

	id = Light::instances_count++;
}

Light* Light::create_new(std::map<std::string, SceneDataContainer> &sdMap){
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("light_type");
	if (sdIter != sdMap.end()){
		if (sdIter->second.val == "point"){
			return new PointLight(sdMap);
		}else if (sdIter->second.val == "area"){
			return new AreaLight(sdMap);
		}
	}else{

		// Default is always point light
		return new PointLight(sdMap);
	}
}


Light::~Light(){
	LOG("Destroyed light: " << id);
}


