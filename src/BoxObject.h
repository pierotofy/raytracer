/*
 * BoxObject.h
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */

#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_

#include <string>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "TriangleObject.h"
#include <vector>
#include "BvhNode.h"

using namespace std;
using namespace sivelab;

namespace ptrt {

	class BoxObject: public ptrt::Object {
	private:
		Vector3D min_pt; // Given a coordinate system, the most negative x,y,z value
		Vector3D max_pt; // Given a coordinate system, the most positive x,y,z value

		// A box is composed of 12 triangles
		vector<Object *> triangles;

		friend ostream& operator<<(ostream&, ptrt::BoxObject &);
	protected:
		virtual bool intersect(const Ray&, float &, float &) { 	assert(false); return false; }

		// A complex object should never have the normal calculated, its components will */
		virtual Vector3D get_normal(const Vector3D &){ assert(false); return Vector3D(); }

		virtual void generate_bounding_box();
	public:
		BoxObject(std::map<std::string, SceneDataContainer> &, map<string, Shader *> &);
		ostream& print(ostream&);
		virtual ~BoxObject();

		virtual void get_uv_coords(const Vector3D &, float &, float &);
		virtual bool intersect(const Ray&, float &, float &, HitStructure &);
	};

}
#endif /* BOXOBJECT_H_ */
