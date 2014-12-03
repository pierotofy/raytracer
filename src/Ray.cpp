/*
 * Ray.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: piero
 */

#include "Ray.h"
#include "Globals.h"
#include "Utils.h"

using namespace std;

namespace ptrt {

std::ostream &operator<<(std::ostream& os, const ptrt::Ray& r)
{
    os << "Ray {" << endl
       << "  direction: " << r.get_direction() << endl
       << "  origin: " << r.get_origin() << endl
       << "}" << endl;
	return os;
}

Ray::Ray(){
	random_u = random_v = -1.0f;
}

Ray::Ray(const Vector3D &direction, const Vector3D &origin){
	this->direction = direction;
	this->origin = origin;

	random_u = random_v = -1.0f;
}

/** Will destructively distort this ray by a given alpha (usually roughness) value */
void Ray::distorce_ray(float alpha){
	OrthNormBasis basis = OrthNormBasis(direction);
	float u = -alpha/2.0f + get_random_u() * alpha;
	float v = -alpha/2.0f + get_random_v() * alpha;

	direction = direction + u * basis.get_u() + v * basis.get_v();
}

/** Copies the random_v and random_u params from another ray */
void Ray::carry_random_uv(const Ray &ray){
	random_u = ray.random_u;
	random_v = ray.random_v;
}

/* @param t is t_max
 * @returns the 3D coordinate of the collision */
Vector3D Ray::get_collision_point(const float &t) const{
	//	p(t) = rayorigin + t raydir
	return get_origin() + t*get_direction();
}

/* Computes a ray given a camera and the current pixel coordinates being drawn
 * @param px_x horizontal pixel number
 * @param px_y vertical pixel number */
void Ray::compute_camera(const Camera &camera, float i, float j){
	// u = l + (r − l)(i + 0.5)/nx
	float u = camera.get_image_plane_left_boundary() +
			(camera.get_image_plane_right_boundary() - camera.get_image_plane_left_boundary()) *
			i / (float)g_args.width;

	// v = b + (t − b)(j + 0.5)/ny
	float v = camera.get_image_plane_bottom_boundary() +
			   (camera.get_image_plane_top_boundary() - camera.get_image_plane_bottom_boundary()) *
			   j / (float)g_args.height;

/*	cout << "u: " << u << endl
			<< "v: " << v << endl
			<< "get_w: " << camera.get_basis()->get_w() << endl
			<< "get_u: " << camera.get_basis()->get_u() << endl
			<< "get_v: " << camera.get_basis()->get_v() << endl;*/


	// direction = −d W + u U + v V ;
	direction = -camera.get_focal_length() * camera.get_basis()->get_w() +
				u * camera.get_basis()->get_u() +
				v * camera.get_basis()->get_v();
	origin = camera.get_position();
}

/* Assign to random_u and random_v new values */
void Ray::set_random_uv(){
	this->random_u = drand48();
	this->random_v = drand48();
}


/* Computes a ray given a viewing ray (going from the object to the camera) and the object's normal
 * @param viewing_direction vector pointing from the object to the viewer (incoming ray)
 */
void Ray::compute_reflection(const Vector3D &viewing_direction, const Vector3D &normal, const Vector3D &origin){
	// r = −v + 2(v · n)n
	direction -= viewing_direction;
	direction += 2 * (viewing_direction.dot(normal)) * normal;
	this->origin = origin;
}

bool Ray::compute_refraction(const Vector3D &incoming_direction, const Vector3D &normal, const Vector3D &origin, float n_index, Ray &refraction_ray){
	float n_air = 1.0f;

	Vector3D d = incoming_direction;
	d.normalize();

	float d_dot_n = d.dot(normal);
	float square = sqrt(1.0f - (n_air*n_air * (1.0f - d_dot_n*d_dot_n))/(n_index*n_index));

	if (square < 0){
		return false;
	}else{
		Vector3D t = (n_air * (d - normal * d_dot_n))/n_index - normal*square;
		t.normalize();
		refraction_ray = Ray(t, origin);
		return true;
	}
}

Ray::~Ray() {

}

}
