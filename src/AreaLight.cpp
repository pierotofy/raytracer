#include <sstream>
#include "AreaLight.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, const ptrt::AreaLight& l)
	{
	  l.print(os);
	  return os;
	}

	ostream& AreaLight::print(std::ostream& os = std::cout) const{
		  os << "AreaLight {" << endl;
		  os << "Normal: " << normal << endl
			  << "Width: " << width << endl
			  << "Length: " << length << endl;
		  Light::print(os);
	      os << "}" << endl;

		  return os;
	}
}

AreaLight::AreaLight(std::map<std::string, SceneDataContainer> &sdMap)
 : Light(sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("light_normal");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> normal;
	buf.clear();

	sdIter = sdMap.find("light_width");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> width;
	buf.clear();

	sdIter = sdMap.find("light_length");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> length;
	buf.clear();

	basis = OrthNormBasis(normal);
}


Vector3D AreaLight::get_point_on_light(float random_u, float random_v) const
{
	return position +
			(0.5f - random_u) * basis.get_u() * width  +
			(0.5f - random_v) * basis.get_v() * length;
}


AreaLight::~AreaLight(){
	LOG("Destroyed area light: " << id);
}


