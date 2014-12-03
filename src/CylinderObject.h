/*
 * CylinderObject.h
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */

#ifndef CYLINDEROBJECT_H_
#define CYLINDEROBJECT_H_

#include "SphereObject.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"

using namespace sivelab;

namespace ptrt {

	class CylinderObject: public ptrt::SphereObject {
	protected:
		float height;
		Vector3D latest_ray_origin;

		friend ostream& operator<<(ostream&, const ptrt::CylinderObject &);
	protected:
		virtual bool intersect(const Ray&, float &, float &);
		virtual Vector3D get_normal(const Vector3D&);
	public:
		CylinderObject(std::map<std::string, SceneDataContainer> &, map<string, Shader *> &);
		virtual ~CylinderObject();
		ostream& print(ostream&);
	};

} /* namespace ptrt */
#endif /* CYLINDEROBJECT_H_ */
