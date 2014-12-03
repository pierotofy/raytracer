#include <sstream>
#include "Camera.h"
#include "Utils.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, const ptrt::Camera& c)
	{
	  os << "Camera {" << endl
	     << "  name: " << c.name << endl
	     << "  position: " << c.position << endl
	     << "  view_direction: " << c.view_direction << endl
	     << "  look_at_point: " << c.look_at_point << endl
	     << "  focal_length: " << c.focal_length << endl
	     << "  image_plane_width: " << c.image_plane_width << endl
	     << "  image_plane_height: " << c.image_plane_height << endl
	     << "  image_plane_left_boundary: " << c.image_plane_left_boundary << endl
	     << "  image_plane_right_boundary: " << c.image_plane_right_boundary << endl
	     << "  image_plane_bottom_boundary: " << c.image_plane_bottom_boundary << endl
	     << "  image_plane_top_boundary: " << c.image_plane_top_boundary << endl
	     << "  aspect_ratio: " << c.aspect_ratio << endl
		 << "}" << endl;

	  return os;
	}
}

Camera::Camera(float aspect_ratio){
	this->aspect_ratio = aspect_ratio;
}

Camera::~Camera(){
	RELEASE_SAFELY(basis);
	LOG("Destroyed camera: " << name);
}


/* Initialize data members from xml file data
 * Call always after constructor
 */
void Camera::initialize(map<string, SceneDataContainer> &sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("camera_name");
	assert( sdIter != sdMap.end() );
	name = sdIter->second.val;

	sdIter = sdMap.find("camera_position");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> position;
	buf.clear();

	sdIter = sdMap.find("camera_viewDir");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> view_direction;
	buf.clear();

	sdIter = sdMap.find("camera_lookatPoint");
	assert( sdIter != sdMap.end() );
	if (sdIter->second.isSet){
		buf.str(sdIter->second.val);
		buf >> look_at_point;
		buf.clear();

		view_direction = look_at_point - position;
		view_direction.normalize();
	}

	sdIter = sdMap.find("camera_focalLength");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> focal_length;
	buf.clear();

	sdIter = sdMap.find("camera_imagePlaneWidth");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> image_plane_width;
	buf.clear();

	// Calculate remaining data members
	image_plane_height = image_plane_width * aspect_ratio;

	// b = (0, 1, 0) (ideal)
	basis = new OrthNormBasis(view_direction, Vector3D(0, 1, 0));

	image_plane_left_boundary = -(image_plane_width / 2);
	image_plane_right_boundary = (image_plane_width / 2);
	image_plane_bottom_boundary = (image_plane_height / 2);
	image_plane_top_boundary = -(image_plane_height / 2);
}

