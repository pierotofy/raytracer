/*
 * MeshObject.cpp
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */
#include <sstream>
#include "cs5721Code/cs5721GraphicsLib/src/model_obj.h"
#include "MeshObject.h"
#include <assert.h>
#include "Utils.h"
#include "Globals.h"

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::MeshObject& m)
	{
	  return m.print(os);
	}

	ostream& MeshObject::print(std::ostream& os = std::cout){
		  os << "MeshObject {" << endl;
		  Object::print(os);
		  os << "}" << endl;

		  return os;
	}
}

MeshObject::MeshObject(std::map<std::string, SceneDataContainer> &sdMap, map<string, Shader *> &shaders)
	: Object(sdMap, shaders) {
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

    string obj_file;
	sdIter = sdMap.find("shape_file");
	assert( sdIter != sdMap.end() );
	buf.str(sdIter->second.val);
	buf >> obj_file;
	buf.clear();

	ModelOBJ obj;
	if (!obj.import((g_path_prefix + obj_file).c_str())){
		cerr << "Cannot open file " << g_path_prefix + obj_file << " (is the path correct?)" << endl;
		log_exception("Invalid obj file specified", true);
	}

	const ModelOBJ::Mesh *mesh = 0;
	const ModelOBJ::Material *material = 0;
	const ModelOBJ::Vertex *vertices = 0;

	// The index buffer is the list of indices used to reference the vertices in
	// the list of vertices (also known as a vertex buffer).
	const int *indexBuffer = obj.getIndexBuffer();

	// Walk over all of the meshes associated with this OBJ file
	for (int i = 0; i < obj.getNumberOfMeshes(); i++)
	{
	   // For each mesh, get a reference to the mesh, a pointer to the material
	   // associated with the mesh, and a pointer to the vertex buffer segment associated
	   // with the mesh.
	   mesh = &obj.getMesh(i);
	   material = mesh->pMaterial;
	   vertices = obj.getVertexBuffer();

	   // With the material pointer, you'll likely want to infer what type of shader to
	   // create.  Take a look at the pMaterial class declaration/definition in the
	   // model_obj.h file for more detailed information.
	   // material->diffuse[0], pMaterial->diffuse[1], pMaterial->diffuse[2]

	   // With the mesh, you have a starting index to use in the index array.  You
	   // also have the number of triangles in the mesh.  Using the two you can iterate
	   // over the indices and access the vertices in the vertex buffer appropriately.
	   for (int i = mesh->startIndex; i < (mesh->startIndex + mesh->triangleCount*3); i+=3){
	      // With the starting index, we can access three indices in the index buffer
	      // to get the indices into the vertex buffer, and thus, the vertices of a
	      // triangle.
	      ModelOBJ::Vertex v0, v1, v2;
	      v0 = vertices[ indexBuffer[i] ];
	      v1 = vertices[ indexBuffer[i+1] ];
	      v2 = vertices[ indexBuffer[i+2] ];

	      // With a vertex, you have several pieces of data (see the model_obj.h file for
	      // more detail about what is stored in ModelOBJ::Vertex.  You certainly have
	      // positions (for instance v2.position[0], v2.position[1], v2.position[2]).  You
	      // also have smoothed normals if you want really nice looking OBJ renderings:
	      // v0.normal[0],v0.normal[1],v0.normal[2].

		  triangles.push_back(new TriangleObject(sdMap, shaders,
					Vector3D(v0.position[0], v0.position[1], v0.position[2]),
					Vector3D(v1.position[0], v1.position[1], v1.position[2]),
					Vector3D(v2.position[0], v2.position[1], v2.position[2])
			));
	   }
	}

	if (USE_BVH){
		root_node = new BvhNode(&triangles, 0, 0, BvhNode::SPACE_BASED);
	}else{
		root_node = NULL;
	}
}

bool MeshObject::intersect(const Ray& ray, float &t_min, float &t_max, HitStructure &hit){
	if (root_node != NULL){
		return root_node->intersect(ray, t_min, t_max, hit);
	}else{
		Object *first = 0;

		for (int i = 0; i<triangles.size(); i++){
			if (triangles[i]->intersect(ray, t_min, t_max, hit)){
				first = triangles[i];
			}
		}

		return first != 0;
	}
}

void MeshObject::generate_bounding_box(){
	assert(triangles.size() > 0);

	if (root_node != NULL){
		bounding_box = root_node->get_bounding_box();
	}else{
		bounding_box = triangles[0]->get_bounding_box();
		for (int i = 1; i<triangles.size(); i++){
			bounding_box = bounding_box.merge(triangles[i]->get_bounding_box());
		}
	}
}


MeshObject::~MeshObject() {
	for (int i = 0; i<triangles.size(); i++)
		RELEASE_SAFELY(triangles[i]);
	triangles.clear();

	if (root_node) RELEASE_SAFELY(root_node);

	LOG("Destroyed mesh shape: " << name);
} /* namespace ptrt */

