#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "cs5721Code/cs5721GraphicsLib/src/SceneDataContainer.h"
#include "cs5721Code/cs5721GraphicsLib/src/XMLSceneParser.h"
#include "cs5721Code/cs5721GraphicsLib/src/png++/png.hpp"
#include "Light.h"
#include "Camera.h"
#include "Shader.h"
#include "AmbientShader.h"
#include "Object.h"
#include "Transform.h"
#include "TextureManager.h"
#include "InstanceManager.h"
#include "BvhNode.h"

using namespace sivelab;
using namespace std;

namespace ptrt{
	class Scene : public SceneElementCreator{
		private:
			vector<Light *> lights;
			vector<Camera *> cameras;
			vector<Object *> objects;

			map<string, Shader *> shaders;
			map<string, Transform *> transforms;

		protected:
			BvhNode *generate_bvh_tree();

			png::image< png::rgb_pixel > render();
			png::image< png::rgb_pixel > renderRightEye();
			png::image< png::rgb_pixel > renderLeftEye();
		public:
			Scene(const string&);
			virtual ~Scene();

			void render(const string &);
			void render3D(const string &);

			void instance(std::map<std::string, SceneDataContainer> &);

			inline vector<Camera*> getCameras() const
			{
				return cameras;
			}

			inline vector<Light*> getLights() const
			{
				return lights;
			}

			inline vector<Object*> getObjects() const
			{
				return objects;
			}

			inline map<string,Shader*> getShaders() const
			{
				return shaders;
			}

			inline map<string,Transform*> getTransforms() const
			{
				return transforms;
			}
	};
}
#endif
