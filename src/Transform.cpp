#include <sstream>
#include <cmath>
#include "Transform.h"
#include "Globals.h"
#include <assert.h>
#include <stack>
#include "Utils.h"

using namespace ptrt;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::Transform& s)
	{
		s.print(os);
		return os;
	}
}

ostream& Transform::print(std::ostream& os = std::cout){
	  os << "Transform {" << endl
	     << "  name: " << name << endl
	     << "  matrix: " << endl << matrix << endl
	     << "}" << endl;
	  return os;
}

Transform::Transform(map<string, SceneDataContainer> &sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

    std::stack<Matrix4> transforms;
    matrix = Matrix4::Identity();

	sdIter = sdMap.find("transform_name");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.isSet){
		buf.str(sdIter->second.val);
		name = sdIter->second.val;
		buf.clear();
	}else{
		name = "unnamed transform";
	}

#define DEGREES_TO_RADIANS 0.0174532925f

	for (sdIter = sdMap.begin(); sdIter != sdMap.end(); sdIter++){
		if (sdIter->second.name == "translate"){
			Vector3D translate;
			buf.str(sdIter->second.val);
			buf >> translate;
			buf.clear();

			transforms.push(Matrix4::Translation(translate));
		}else if (sdIter->second.name == "scale"){
			Vector3D scale;
			buf.str(sdIter->second.val);
			buf >> scale;
			buf.clear();

			transforms.push(Matrix4::Scale(scale));
		}else if (sdIter->second.name == "rotateX"){
			float rotate_x;
			buf.str(sdIter->second.val);
			buf >> rotate_x;
			buf.clear();

			transforms.push(Matrix4::RotationX(rotate_x * DEGREES_TO_RADIANS));
		}else if (sdIter->second.name == "rotateY"){
			float rotate_y;
			buf.str(sdIter->second.val);
			buf >> rotate_y;

			// Flip it
			rotate_y = -rotate_y;

			buf.clear();

			transforms.push(Matrix4::RotationY(rotate_y * DEGREES_TO_RADIANS));
		}else if (sdIter->second.name == "rotateZ"){
			float rotate_z;
			buf.str(sdIter->second.val);
			buf >> rotate_z;
			buf.clear();

			transforms.push(Matrix4::RotationZ(rotate_z * DEGREES_TO_RADIANS));
		}
	}

	while(!transforms.empty()){
		//matrix = transforms.top() * matrix;
		matrix = matrix * transforms.top();

		transforms.pop();
	}
}


Transform::~Transform(){
	LOG("Transform destructor called: " << name);
}

