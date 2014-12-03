#ifndef ORTHNORMBASIS_H
#define ORTHNORMBASIS_H

#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include <cmath>

using namespace sivelab;

namespace ptrt{
	class OrthNormBasis{
		private:
			Vector3D u, v, w;
		public:
			OrthNormBasis(){}
			OrthNormBasis(const Vector3D &);
			OrthNormBasis(const Vector3D &, const Vector3D &);
			~OrthNormBasis();

			Vector3D get_u() const { return u; }
			Vector3D get_v() const { return v; }
			Vector3D get_w() const { return w; }


			static bool colinear(const Vector3D &, const Vector3D &);
	};
}
#endif
