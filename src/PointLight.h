#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <string>
#include "Light.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	/* Point light */
	class PointLight : public Light{
		private:

			friend ostream& operator<<(ostream&, const ptrt::PointLight &);
		public:
			PointLight(map<string, SceneDataContainer> &);
			virtual ~PointLight();

			virtual ostream& print(ostream&) const;
			virtual Vector3D get_point_on_light(float, float) const{
				return position;
			}
	};


}
#endif
