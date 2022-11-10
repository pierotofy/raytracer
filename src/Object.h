#ifndef OBJECT_H
#define OBJECT_H

#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/png++/png.hpp"
#include "Ray.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "HitStructure.h"
#include "BoundingBox.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class Shader;

	class Object{
		protected:
			Object(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders);

			Object(const string &name) : name(name) {} // To be used only by a bvhnode

			string name;
			Shader *shader;
			BoundingBox bounding_box;

			friend ostream& operator<<(ostream&, ptrt::Object &);

			/** Only primitives need to implement this */
			virtual bool intersect(const Ray&, float &, float &) = 0;

			/* We need a reference to the intersection point in certain cases */
			virtual Vector3D get_normal(const Vector3D &) = 0;

			/* Each object needs to override this in order to compute a bounding box */
			virtual void generate_bounding_box() = 0;
		public:
			static Object* create_new(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders);
			virtual ostream& print(ostream&);

			/** Complex shapes and instance objects might want to override this */
			virtual bool intersect(const Ray&, float &, float &, HitStructure &);

			/* Return the U-V coordinates of the given point (different for each object) */
			virtual void get_uv_coords(const Vector3D &intersection_point, float &u, float &v){ assert(false); u = v = 0; }

			virtual ~Object();

			inline BoundingBox get_bounding_box() const
			{
				assert(bounding_box.is_initialized());
				return bounding_box;
			}


			string get_name() const
			{
				return name;
			}

	};
}
#endif
