/*
 * BoxObject.h
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */

#ifndef PYRAMIDOBJECT_H_
#define PYRAMIDOBJECT_H_

#include <string>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "TriangleObject.h"
#include <vector>

using namespace std;
using namespace sivelab;

namespace ptrt {

	class PyramidObject: public ptrt::Object {
	private:
		Vector3D center;
		float height;
		float baselength;

		// A pyramid is composed of 6 triangles
		vector<Object *> triangles;

		friend ostream& operator<<(ostream&, ptrt::PyramidObject &);
	protected:
		virtual bool intersect(const Ray&, float &, float &) { assert(false); return false; }

		virtual void generate_bounding_box();

		// A complex object should never have the normal calculated, its components will */
		virtual Vector3D get_normal(const Vector3D &){ assert(false); return Vector3D(); }
	public:
		PyramidObject(std::map<std::string, SceneDataContainer> &, map<string, Shader *> &);
		ostream& print(ostream&);
		virtual ~PyramidObject();

		virtual bool intersect(const Ray&, float &, float &, HitStructure &);
	};

}
#endif /* PYRAMIDOBJECT_H_ */
