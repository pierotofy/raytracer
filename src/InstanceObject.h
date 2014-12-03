#ifndef INSTANCEOBJECT_H
#define INSTANCEOBJECT_H

#include "Object.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include <map>
#include "Instance.h"
#include "Matrix4.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class Shader;

	class InstanceObject : public Object{
		protected:

			string name;
			Instance *instance;
			Matrix4 transform_matrix; // Transformation matrix
			Matrix4 transform_matrix_inverse;
			Matrix4 transform_matrix_inverse_transpose;

			friend ostream& ptrt::operator<<(ostream&, ptrt::InstanceObject &);
		protected:
			virtual bool intersect(const Ray&, float &, float &) { assert(false); return false; } // Always use instance object's intersect methods
			virtual Vector3D get_normal(const Vector3D&){ assert(false); return Vector3D(); } // Always use instance object's get normal methods

			virtual void generate_bounding_box();
		public:
			InstanceObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders);
			virtual bool intersect(const Ray&, float &, float &, HitStructure &); // We will make changes to the hit structure
			virtual ~InstanceObject();
			virtual ostream& print(ostream&);
			string get_name() const
			{
				return name;
			}

			bool is_valid(){ return instance->is_valid(); }
			virtual void get_uv_coords(const Vector3D &, float &, float &);
	};
}
#endif
