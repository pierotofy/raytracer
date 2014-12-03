#include "OrthNormBasis.h"
#include "Utils.h"

using namespace ptrt;
using namespace sivelab;

/* Given a vector A, creates a Orthonormal Basis for it */
OrthNormBasis::OrthNormBasis(const Vector3D &a){
	w-=a;
	w.normalize();

	Vector3D t = w;

	// Find smallest component of w
	int smallest_index = 0;
	int smallest_value = w[0];
	for (int i = 1; i<3; i++){
		if (w[i] < smallest_value){
			smallest_value = w[i];
			smallest_index = i;
		}
	}

	t[smallest_index] = 1;

	if (colinear(t, a)){
		// do what?
		LOG("WARNING! Colinearity found in orthnormbasis");
	}

	u = t.cross(w);
	u.normalize();
	v = w.cross(u);
}

/* Given a vector gaze and b, creates a Orthonormal Basis for it */
OrthNormBasis::OrthNormBasis(const Vector3D &gaze, const Vector3D &b){
	Vector3D bvec = b;

	w -= gaze;
	w.normalize();

	if (colinear(bvec, w)){
		bvec[2] += SMALL_EPS;
		LOG("Colinearity collision fixed");

		assert(!colinear(bvec, w));
	}

	u = bvec.cross(w);
	u.normalize();

	v = w.cross(u);


}

OrthNormBasis::~OrthNormBasis(){

}

/* Checks whether two vectors are colinear */
bool OrthNormBasis::colinear(const Vector3D &a, const Vector3D &b){
	Vector3D a_copy = a;
	Vector3D b_copy = b;

	a_copy.normalize();
	b_copy.normalize();

	return FEQUALVECTORS(a, b);
}


