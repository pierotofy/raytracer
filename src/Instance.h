#ifndef INSTANCE_H
#define INSTANCE_H

#include "Object.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include <map>

using namespace std;
using namespace sivelab;

namespace ptrt{
	class Shader;

	class Instance{
		protected:
			string name;
			Object *object;
			Shader *default_shader;

			friend ostream& ptrt::operator<<(ostream&, ptrt::Instance &);
		public:
			Instance(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders);
			virtual ~Instance();
			virtual ostream& print(ostream&);

			Shader *get_default_shader() const
			{
				return default_shader;
			}

			Object *get_object() const
			{
				return object;
			}

			string get_name() const
			{
				return name;
			}

			bool is_valid() const{
				return object != 0;
			}

	};
}
#endif
