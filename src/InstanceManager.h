#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include <assert.h>
#include "Utils.h"
#include "Instance.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class InstanceManager{
		private:
			map<string, Instance *> instance_objects;
		public:
			static InstanceManager *instance;
			static InstanceManager *getSingleton(){ return ptrt::InstanceManager::instance; }
			static void Initialize(){
				assert(ptrt::InstanceManager::instance == 0);

				ptrt::InstanceManager::instance = new InstanceManager();
			}
			static void Destroy(){
				RELEASE_SAFELY(ptrt::InstanceManager::instance);
			}

			InstanceManager();
			virtual ~InstanceManager();
			void add_instance(Instance *);
			Instance *get_instance(const string &);
	};
}

#endif
