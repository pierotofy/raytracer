/*
 * BvhNode.h
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */

#ifndef BVHNODE_H_
#define BVHNODE_H_

#include <string>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "Object.h"
#include <vector>

using namespace std;
using namespace sivelab;

namespace ptrt {

	class BvhNode: public ptrt::Object {
	public:
		enum SubdivisionType { OBJECT_BASED, SPACE_BASED };

		BvhNode(vector<Object *> *, int, int, SubdivisionType);
		ostream& print(ostream&);
		bool intersect(const Ray&, float &, float &, HitStructure &);
		virtual ~BvhNode();

		// Keep track of the max depth of the bvh tree
		static int bvhTreeMaxDepth;
	private:
		Object *left;
		Object *right;
		vector<Object *> *objects;
		int depth;
		int axis;
		SubdivisionType type;

		friend ostream& operator<<(ostream&, ptrt::BvhNode &);

		void do_space_based_subdivision();
		void do_object_based_subdivision();
	protected:
		virtual bool intersect(const Ray&, float &, float &) { assert(false); return false; }

		virtual void generate_bounding_box();

		// A bvh node doesn't have normals */
		virtual Vector3D get_normal(const Vector3D &){ assert(false); return Vector3D(); }

	};

}
#endif /* BVHNODE_H_ */
