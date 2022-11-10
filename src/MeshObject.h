/*
 * MeshObject.h
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */

#ifndef MESHOBJECT_H_
#define MESHOBJECT_H_

#include <string>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "TriangleObject.h"
#include <vector>
#include "BvhNode.h"

using namespace std;
using namespace sivelab;

namespace ptrt {

	class MeshObject: public ptrt::Object {
	private:

		// A mesh is composed of triangles
		vector<Object *> triangles;
		BvhNode *root_node;

		friend ostream& operator<<(ostream&, ptrt::MeshObject &);
	protected:
		virtual bool intersect(const Ray&, float &, float &) { 	assert(false); return false; }

		virtual void generate_bounding_box();

		// A complex object should never have the normal calculated, its components will */
		virtual Vector3D get_normal(const Vector3D &){ assert(false); return Vector3D(); }
	public:
		MeshObject(std::map<std::string, SceneDataContainer> &, map<string, Shader *> &);
		ostream& print(ostream&);
		virtual ~MeshObject();

		virtual bool intersect(const Ray&, float &, float &, HitStructure &);
	};

}
#endif /* MESHOBJECT_H_ */
