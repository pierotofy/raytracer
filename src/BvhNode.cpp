/*
 * BvhNode.cpp
 *
 *  Created on: 2012-01-27
 *      Author: piero
 */
#include <sstream>
#include "Object.h"
#include "BvhNode.h"
#include <assert.h>
#include "Utils.h"
#include <algorithm>
#include <limits.h>

using namespace ptrt;
using namespace std;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::BvhNode& b)
	{
	  return b.print(os);
	}

	ostream& BvhNode::print(std::ostream& os = std::cout){
		  os << "BvhNode {" << endl;
		  if (left != NULL) os << "Left: { " << *left << "}" << endl;
		  if (right != NULL) os << "Right: { " << *right << "}" << endl;
		  os   << "}" << endl;

		  return os;
	}
}

BvhNode::BvhNode(vector<Object *> *objects, int axis, int depth, SubdivisionType type)
	: objects(objects), depth(depth), axis(axis), type(type), Object("BvhNode") {
	left = right = NULL;
	assert(objects->size() > 0);

	// Update statistics variable
	if (depth > BvhNode::bvhTreeMaxDepth){
		BvhNode::bvhTreeMaxDepth = depth;
	}

	if (type == OBJECT_BASED){
		do_object_based_subdivision();
	}else if (type == SPACE_BASED){
		do_space_based_subdivision();
	}
}

int compare_axis = 0;
bool object_comparator(Object *i, Object *j) {

	return (i->get_bounding_box().get_midpoint(compare_axis) <
			j->get_bounding_box().get_midpoint(compare_axis));
}

void BvhNode::do_object_based_subdivision(){
	int n = objects->size();
	if (n == 1){
		left = (*objects)[0];
		bounding_box = left->get_bounding_box();
		right = NULL;
	}else if (n == 2){
		left = (*objects)[0];
		right = (*objects)[1];
		bounding_box = left->get_bounding_box().merge(right->get_bounding_box());
	}else{
		// Sort elements by axis
		compare_axis = axis;
		sort(objects->begin(), objects->end(), object_comparator);

		int i = 0;

		// Half on the left branch
		vector<Object *> *left_objects = new vector<Object *>();
		vector<Object *> *right_objects = new vector<Object *>();

		for (; i < objects->size() / 2; i++){
			left_objects->push_back((*objects)[i]);
		}

		// Half on the right branch
		for (; i < objects->size(); i++){
			right_objects->push_back((*objects)[i]);
		}

		left = new BvhNode(left_objects, (axis + 1) % 3, depth + 1, type);
		right = new BvhNode(right_objects, (axis + 1) % 3, depth + 1, type);
		bounding_box = left->get_bounding_box().merge(right->get_bounding_box());
	}
}

void BvhNode::do_space_based_subdivision(){
	int n = objects->size();
	if (n == 1){
		left = (*objects)[0];
		bounding_box = left->get_bounding_box();
		right = NULL;
	}else if (n == 2){
		left = (*objects)[0];
		right = (*objects)[1];
		bounding_box = left->get_bounding_box().merge(right->get_bounding_box());
	}else{
		// Find midpoint of this node's boundingbox

		// Calculate bounding box of all objects in the list
		BoundingBox objects_bounding_box = (*objects)[0]->get_bounding_box();
		for (int i = 1; i < objects->size(); i++){
			objects_bounding_box = objects_bounding_box.merge((*objects)[i]->get_bounding_box());
		}

		// Find optimal axis that will produce the most balanced cut
		int best_axis = 0;
		int node_difference[3] = {INT_MAX, INT_MAX, INT_MAX};

		for (int current_axis = 0; current_axis < 3; current_axis++){
			// Never choose the same axis twice
			if (current_axis == axis) continue;

			float midpoint = objects_bounding_box.get_midpoint(current_axis);
			int left_nodes = 0;
			int right_nodes = 0;

			for (int i = 0; i < objects->size(); i++){
				// Find each object's midpoint
				float object_midpoint = (*objects)[i]->get_bounding_box().get_midpoint(current_axis);
				if (object_midpoint <= midpoint){
					left_nodes++;
				}else{
					right_nodes++;
				}
			}

			node_difference[current_axis] = abs(left_nodes - right_nodes);
		}

		int best_node_difference = INT_MAX;
		for (int i = 0; i < 3; i++){
			if (node_difference[i] < best_node_difference){
				best_node_difference = node_difference[i];
				best_axis = i;
			}
		}

		// Partition
		vector<Object *> *left_objects = new vector<Object *>();
		vector<Object *> *right_objects = new vector<Object *>();
		float midpoint = objects_bounding_box.get_midpoint(best_axis);
		bool pushLeftOnMidpoint = drand48() > 0.5f;

		for (int i = 0; i < objects->size(); i++){

			// Find each object's midpoint
			float object_midpoint = (*objects)[i]->get_bounding_box().get_midpoint(best_axis);

			if (object_midpoint < midpoint){
				left_objects->push_back((*objects)[i]);
			}else if (object_midpoint > midpoint){
				right_objects->push_back((*objects)[i]);
			}else{
				if (pushLeftOnMidpoint)	left_objects->push_back((*objects)[i]);
				else right_objects->push_back((*objects)[i]);
			}
		}

		bool left_has_objects = left_objects->size() > 0;
		bool right_has_objects = right_objects->size() > 0;

		if (left_has_objects && right_has_objects){

			left = new BvhNode(left_objects, best_axis, depth + 1, type);
			right = new BvhNode(right_objects, best_axis, depth + 1, type);
			bounding_box = left->get_bounding_box().merge(right->get_bounding_box());

		}else if (left_has_objects){

			left = new BvhNode(left_objects, best_axis, depth + 1, type);
			RELEASE_SAFELY(right);
			bounding_box = left->get_bounding_box();
		}else if (right_has_objects){

			right = new BvhNode(right_objects, best_axis, depth + 1, type);
			RELEASE_SAFELY(left);
			bounding_box = right->get_bounding_box();
		}else{
			assert(false);
		}
	}
}

void BvhNode::generate_bounding_box(){
	assert(false);
}

bool BvhNode::intersect(const Ray& ray, float &t_min, float &t_max, HitStructure &hit){
	if (bounding_box.hit(ray)){
		HitStructure left_hit;
		HitStructure right_hit;
		bool left_intersected = false;
		bool right_intersected = false;

		// Do we have branches? If so, compute their intersection
		if (left != NULL){
			left_intersected = left->intersect(ray, t_min, t_max, left_hit);
		}

		if (right != NULL){
			right_intersected = right->intersect(ray, t_min, t_max, right_hit);
		}

		// Both hit?
		if (left_intersected && right_intersected){
			if (left_hit.t < right_hit.t){
				hit = left_hit;
			}else{
				hit = right_hit;
			}
			return true;

		// Left hit?
		}else if (left_intersected){
			hit = left_hit;
			return true;

		// Right hit?
		}else if (right_intersected){
			hit = right_hit;
			return true;

		// No child hit?
		}else{
			return false;
		}
	}else{
		// Didn't hit our bounding box
		return false;
	}
}

BvhNode::~BvhNode() {
	LOG("Destroyed bvhnode " << this);
	RELEASE_SAFELY(objects);
	RELEASE_SAFELY(left);
	RELEASE_SAFELY(right);
} /* namespace ptrt */

