#ifndef SHADER_H
#define SHADER_H

#include "Object.h"
#include "Light.h"
#include "AmbientShader.h"
#include "ShaderArgs.h"
#include "Texture.h"
#include "HitStructure.h"
#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/png++/png.hpp"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class Shader{
		protected:
			string name;
	    	Vector3D diffuse_component;
	    	float mirror_coefficent;
	    	float refraction_coefficent;
	    	float roughness;
	    	bool cast_shadows;
	    	Texture *diffuse_texture;

			friend ostream& ptrt::operator<<(ostream&, ptrt::Shader &);
		public:
			Shader(const string&);
			Shader(map<string, SceneDataContainer> &);
			virtual ~Shader();
			virtual ostream& print(ostream&);
			virtual Vector3D apply_shader(const ShaderArgs &);
			virtual Vector3D apply_shader_for_light(const ShaderArgs &, Light *, const Vector3D &, const Vector3D &, const Vector3D &) = 0;
			Vector3D get_refraction_color(const ShaderArgs &, const Vector3D &, const Vector3D &, const Vector3D &);
			Vector3D apply_reflection(const vector<Object *> &, const vector<Light *> &, Ray &, int);
			Vector3D apply_refraction(const vector<Object *> &, const vector<Light *> &, Ray &, int);
			virtual Vector3D get_diffuse(const Vector3D &, Object *);
			bool in_shadow(const Vector3D &, const Ray &, Light *, const vector<Object *> &);
			float get_refraction_coefficent() { return refraction_coefficent; }
			static Shader* create_new(std::map<std::string, SceneDataContainer> &);
			static png::rgb_pixel get_rgb_from_color(const Vector3D &);
			static void intersect_all_objects(const vector<Object *> &, const Ray &, float &, float &, HitStructure &);

			static Vector3D background_color;

			string get_name() const
			{
				return name;
			}

			void set_name(string name)
			{
				this->name = name;
			}
	};
}

#endif
