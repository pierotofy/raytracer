#include <sstream>
#include <assert.h>
#include "Object.h"
#include "Instance.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{

std::ostream &operator<<(std::ostream& os, ptrt::Instance& o)
{
  o.print(os);
  return os;
}

ostream& Instance::print(std::ostream& os = std::cout){
	  os << "  Instance {" << endl
		 << "    name: " << name << endl;

	  if (object != 0) os  << "    object: " << object;
	  else os << "    object is not supported" << endl;

	  os << "  }" << endl;
	  return os;
}

Instance::~Instance(){
	LOG("  (Instance destructor called: " << name << ")");

	object = 0; // This was just a reference, scene will dispose it
}

Instance::Instance(map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders){
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("shape_name");
	assert( sdIter != sdMap.end() );
	name = sdIter->second.val;

	// Sometimes we have a default shader specified in the instance (which will be used if none is specified in one of the instances)
	sdIter = sdMap.find("shader_ref");
	if(sdIter != sdMap.end() && sdIter->second.isSet && sdIter->second.val != "null"){
		default_shader = shaders[sdIter->second.val];
	}else{
		default_shader = 0;
	}

    object = Object::create_new(sdMap, shaders);
}

}

