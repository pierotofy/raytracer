#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <map>
#include "OrthNormBasis.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"

using namespace std;
using namespace sivelab;

namespace ptrt{

	/* Perspective camera */
	class Camera{
		private:
			string name;
	    	Vector3D position; // e
	    	Vector3D view_direction; // gaze
	    	Vector3D look_at_point; // if this is set, calculate view_direction
	    	float focal_length; // d
			float image_plane_width;
			float image_plane_height;
			float image_plane_left_boundary;
			float image_plane_right_boundary;
			float image_plane_bottom_boundary;
			float image_plane_top_boundary;
			float aspect_ratio;
			OrthNormBasis *basis;

			friend ostream& ptrt::operator<<(ostream&, const ptrt::Camera &);
		public:
			Camera(float aspect_ratio);
			virtual ~Camera();

			void initialize(map<string, SceneDataContainer> &);

		Vector3D get_position() const
		{
			return position;
		}

		void set_position(const Vector3D &position){
			this->position = position;
		}

		float get_focal_length() const
		{
			return focal_length;
		}

	    OrthNormBasis *get_basis() const
	    {
	        return basis;
	    }

	    float get_image_plane_bottom_boundary() const
	    {
	        return image_plane_bottom_boundary;
	    }

	    float get_image_plane_left_boundary() const
	    {
	        return image_plane_left_boundary;
	    }

	    float get_image_plane_right_boundary() const
	    {
	        return image_plane_right_boundary;
	    }

	    float get_image_plane_top_boundary() const
	    {
	        return image_plane_top_boundary;
	    }


	};
}

#endif
