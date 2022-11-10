#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/png++/png.hpp"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class Texture{
		protected:
			string name;
			png::image<png::rgb_pixel> textureImage;
			int width;
			int height;

			friend ostream& operator<<(ostream&, ptrt::Texture &);
		public:
			Texture(const string &, const string&);
			Texture(map<string, SceneDataContainer> &);
			Vector3D get_color(float u, float v);
			virtual ~Texture();
			virtual ostream& print(ostream&);

			string get_name() const
			{
				return name;
			}

	};
}

#endif
