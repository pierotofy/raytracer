#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <string>
#include <map>
#include "Matrix4.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class Transform{
		protected:
			string name;
			Matrix4 matrix;

			friend ostream& operator<<(ostream&, ptrt::Transform &);
		public:
			Transform(const string&);
			Transform(map<string, SceneDataContainer> &);
			virtual ~Transform();
			virtual ostream& print(ostream&);

			string get_name() const
			{
				return name;
			}

			Matrix4 get_matrix() const{
				return matrix;
			}
	};
}

#endif
