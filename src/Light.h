#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class Light{
		protected:
			int id;
			static int instances_count;

			Vector3D position;
			Vector3D intensity;

			friend ostream& operator<<(ostream&, const ptrt::Light &);
		public:
			Light(map<string, SceneDataContainer> &);
			virtual ~Light();
			static Light* create_new(std::map<std::string, SceneDataContainer> &);
			virtual ostream& print(ostream&) const;

			Vector3D get_intensity() const
			{
				return intensity;
			}

			Vector3D get_position() const
			{
				return position;
			}

			virtual Vector3D get_point_on_light(float, float) const = 0;

	};


}
#endif
