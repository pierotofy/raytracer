/*
 * Ray.h
 *
 *  Created on: Feb 1, 2012
 *      Author: piero
 */

#ifndef RAY_H_
#define RAY_H_

#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include <vector>
#include "Camera.h"

using namespace std;
using namespace sivelab;

namespace ptrt {

class Ray {
private:
	Vector3D direction;
	Vector3D origin;
	float random_u;
	float random_v;

	friend ostream& operator<<(ostream&, const ptrt::Ray &);
public:
	Ray();
	Ray(const Vector3D &, const Vector3D &);

	void compute_camera(const Camera &camera, float i, float j);
	void set_random_uv();
	void carry_random_uv(const Ray &);
	void compute_reflection(const Vector3D &, const Vector3D &, const Vector3D &);
    void distorce_ray(float alpha);
	static bool compute_refraction(const Vector3D &, const Vector3D &, const Vector3D &, float, Ray &);
	Vector3D get_collision_point(const float &) const;
	virtual ~Ray();

    inline Vector3D get_direction() const
    {
        return direction;
    }

    inline Vector3D get_origin() const
    {
        return origin;
    }

    inline float get_random_u() const{
    	assert(random_u != -1.0f);

    	return random_u;
    }

    inline float get_random_v() const{
    	assert(random_v != -1.0f);

    	return random_v;
    }



};

}

#endif /* RAY_H_ */
