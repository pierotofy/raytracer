#ifndef AREALIGHT_H
#define AREALIGHT_H

#include <string>
#include "Light.h"
#include "OrthNormBasis.h"
#include "BoundingBox.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class AreaLight : public Light{
		private:
			Vector3D normal;
			float width;
			float length;
			OrthNormBasis basis;

			friend ostream& ptrt::operator<<(ostream&, const ptrt::AreaLight &);
		public:
			AreaLight(map<string, SceneDataContainer> &);
			virtual ~AreaLight();

			virtual ostream& print(ostream&) const;

			virtual Vector3D get_point_on_light(float, float) const;
	};


}
#endif
