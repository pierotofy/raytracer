/*
 * SphereObject.h
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */

#ifndef SPHEREOBJECT_H_
#define SPHEREOBJECT_H_

#include "Object.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"

using namespace sivelab;

namespace ptrt {

	class SphereObject: public ptrt::Object {
	protected:
		float radius;
		float radius_squared;
		Vector3D center;

		friend ostream& operator<<(ostream&, const ptrt::SphereObject &);\
	protected:
		virtual bool intersect(const Ray&, float &, float &);
		virtual Vector3D get_normal(const Vector3D&);
		virtual void generate_bounding_box();
	public:
		SphereObject(std::map<std::string, SceneDataContainer> &, map<string, Shader *> &);
		virtual ~SphereObject();
		ostream& print(ostream&);
		virtual void get_uv_coords(const Vector3D &, float &, float &);


	};

} /* namespace ptrt */
#endif /* SPHEREOBJECT_H_ */
