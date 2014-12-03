/*
 * SkySphereObject.h
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */

#ifndef SKYSPHEREOBJECT_H_
#define SKYSPHEREOBJECT_H_

#include "SphereObject.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"

using namespace sivelab;

namespace ptrt {

	class SkySphereObject: public ptrt::SphereObject {
	protected:
		friend ostream& operator<<(ostream&, const ptrt::SkySphereObject &);
		virtual Vector3D get_normal(const Vector3D&);
	public:
		SkySphereObject(std::map<std::string, SceneDataContainer> &, map<string, Shader *> &);
		virtual ~SkySphereObject();
		ostream& print(ostream&);
		bool intersect(const Ray&, float &, float &, HitStructure &);
	};

} /* namespace ptrt */
#endif /* SPHEREOBJECT_H_ */
