#include <sstream>
#include <assert.h>
#include "Utils.h"
#include "BoundingBox.h"

using namespace ptrt;
using namespace std;

namespace ptrt{

std::ostream &operator<<(std::ostream& os, ptrt::BoundingBox& o)
{
	  os << "  BoundingBox {" << endl
		 << "    max_pt: " << o.max_pt << endl
		 << "    min_pt: " << o.min_pt << endl
	     << "  }" << endl;
	  return os;
}

BoundingBox::BoundingBox(const Vector3D &max_pt, const Vector3D &min_pt)
 : max_pt(max_pt), min_pt(min_pt), initialized(true){

}

// returns 1.0f for positive floats, -1.0f for negative floats
// for zero it does not work (may gives 1.0f or -1.0f), but it's faster
inline float fast_sign_nozero(float f) {
    float r = 1.0f;
    (int&)r |= ((int&)f & 0x80000000); // mask sign bit in f, set it in r if necessary
    return r;
}

bool BoundingBox::hit(const Ray &ray){
	Vector3D origin = ray.get_origin();
	Vector3D direction = ray.get_direction();
	float tx_min, ty_min, tz_min, tx_max, ty_max, tz_max;
	float a;

	a = 1.0f / direction[0];
	if (a >= 0.0f){
		tx_min = (min_pt[0] - origin[0]) * a;
		tx_max = (max_pt[0] - origin[0]) * a;
	}else{
		tx_min = (max_pt[0] - origin[0]) * a;
		tx_max = (min_pt[0] - origin[0]) * a;
	}

	a = 1.0f / direction[1];
	if (a >= 0.0f){
		ty_min = (min_pt[1] - origin[1]) * a;
		ty_max = (max_pt[1] - origin[1]) * a;
	}else{
		ty_min = (max_pt[1] - origin[1]) * a;
		ty_max = (min_pt[1] - origin[1]) * a;
	}

	// Check here... we might save a few multiply and divisions
	if (!(tx_min > ty_max || ty_min > tx_max)){
		return true;
	}

	a = 1.0f / direction[2];
	if (a >= 0.0f){
		tz_min = (min_pt[2] - origin[2]) * a;
		tz_max = (max_pt[2] - origin[2]) * a;
	}else{
		tz_min = (max_pt[2] - origin[2]) * a;
		tz_max = (min_pt[2] - origin[2]) * a;
	}

	if ((tz_min > ty_max || ty_min > tz_max) &&
		(tx_min > tz_max || tz_min > tx_max)){
		return false;
	}else{
		return true;
	}

//	if ((tx_min > ty_max || ty_min > tx_max) &&
//		(tz_min > ty_max || ty_min > tz_max) &&
//		(tx_min > tz_max || tz_min > tx_max)){
//		return false;
//	}else{
//		return true;
//	}
}

BoundingBox BoundingBox::merge(const BoundingBox &box) const{
	Vector3D merge_max_pt;
	Vector3D merge_min_pt;

	merge_max_pt[0] = max(max_pt[0], box.max_pt[0]);
	merge_max_pt[1] = max(max_pt[1], box.max_pt[1]);
	merge_max_pt[2] = max(max_pt[2], box.max_pt[2]);

	merge_min_pt[0] = min(min_pt[0], box.min_pt[0]);
	merge_min_pt[1] = min(min_pt[1], box.min_pt[1]);
	merge_min_pt[2] = min(min_pt[2], box.min_pt[2]);

	return BoundingBox(merge_max_pt, merge_min_pt);
}

BoundingBox BoundingBox::axis_aligned_transform(const Matrix4 &transform) const{
	// Getting the old values so that we can use the existing merge method.
	Vector3D transformed_max_pt = max_pt;
	Vector3D transformed_min_pt = min_pt;
	Vector3D current_corner;

	current_corner = min_pt;
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	// min,min,max
	current_corner[2] = max_pt[2];
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	// min max max
	current_corner[1] = max_pt[1];
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	 // min max min
	current_corner[2] = min_pt[2];
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	// max max min
	current_corner[0] = max_pt[0];
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	// max max max
	current_corner[2] = max_pt[2];
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	// max min max
	current_corner[1] = min_pt[1];
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	// max min min
	current_corner[2] = min_pt[2];
	transformed_max_pt = make_ceil(transform.multiply(current_corner, 1.0f), transformed_max_pt);
	transformed_min_pt = make_floor(transform.multiply(current_corner, 1.0f), transformed_min_pt);

	return BoundingBox(transformed_max_pt, transformed_min_pt);
}

Vector3D BoundingBox::make_ceil(const Vector3D &v1, const Vector3D &v2) const{
	Vector3D result;
	result[0] = max(v1[0], v2[0]);
	result[1] = max(v1[1], v2[1]);
	result[2] = max(v1[2], v2[2]);
	return result;
}

Vector3D BoundingBox::make_floor(const Vector3D &v1, const Vector3D &v2) const{
	Vector3D result;
	result[0] = min(v1[0], v2[0]);
	result[1] = min(v1[1], v2[1]);
	result[2] = min(v1[2], v2[2]);
	return result;
}


BoundingBox::~BoundingBox(){
//	LOG("  (BoundingBox destructor called: " << this << ")");
}

}

