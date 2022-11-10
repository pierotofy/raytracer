#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "Ray.h"
#include <map>
#include "Matrix4.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class BoundingBox{
		private:
			void compute_midpoint();
			bool initialized;

			Vector3D make_ceil(const Vector3D &, const Vector3D &) const;
			Vector3D make_floor(const Vector3D &, const Vector3D &) const;

		protected:
			Vector3D min_pt;
			Vector3D max_pt;

			friend ostream& operator<<(ostream&, ptrt::BoundingBox &);
		public:
			BoundingBox(){ initialized = false; }
			BoundingBox(const Vector3D &, const Vector3D &);

			inline Vector3D get_min_pt() const { return min_pt; }
			inline Vector3D get_max_pt()const { return max_pt; }
			inline float get_midpoint(int axis){
				assert(is_initialized());

				return get_min_pt()[axis] +
							((get_max_pt()[axis] - get_min_pt()[axis]) / 2.0f);
			}

			bool hit(const Ray &);
			bool is_initialized() const { return initialized; }


			BoundingBox merge(const BoundingBox &box) const;
			BoundingBox axis_aligned_transform(const Matrix4 &transform) const;
			virtual ~BoundingBox();

			BoundingBox &operator=(const BoundingBox &rhs)
		    {
		      min_pt = rhs.min_pt;
		      max_pt = rhs.max_pt;
		      initialized = rhs.initialized;
		      return *this;
		    }
	};
}
#endif
