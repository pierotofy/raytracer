#include "InstanceManager.h"

namespace ptrt{


InstanceManager *ptrt::InstanceManager::instance = 0;

InstanceManager::InstanceManager()
{
	LOG("Instantiated instance manager");
}


InstanceManager::~InstanceManager()
{
	map<string, Instance *>::iterator it;
	for (it=instance_objects.begin(); it != instance_objects.end(); it++)
	   RELEASE_SAFELY((*it).second);
	instance_objects.clear();

	LOG("Destroyed instance manager");
}

void InstanceManager::add_instance(Instance *obj)
{
	instance_objects[obj->get_name()] = obj;
}


Instance *InstanceManager::get_instance(const string & name)
{
	assert(instance_objects.count(name) == 1);

	return instance_objects[name];
}

}



