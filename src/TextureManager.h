#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <map>
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include <assert.h>
#include "Utils.h"
#include "Texture.h"

using namespace std;
using namespace sivelab;

namespace ptrt{
	class TextureManager{
		private:
			map<string, Texture *> textures;
		public:
			static TextureManager *instance;
			static TextureManager *getSingleton(){ return ptrt::TextureManager::instance; }
			static void Initialize(){
				assert(ptrt::TextureManager::instance == 0);

				ptrt::TextureManager::instance = new TextureManager();
			}
			static void Destroy(){
				RELEASE_SAFELY(ptrt::TextureManager::instance);
			}

			TextureManager();
			virtual ~TextureManager();
			void add_texture(Texture *);
			Texture *get_texture(const string &);
	};
}

#endif
