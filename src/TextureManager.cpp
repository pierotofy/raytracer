#include "TextureManager.h"


namespace ptrt{


TextureManager *ptrt::TextureManager::instance = 0;

TextureManager::TextureManager()
{
	LOG("Instantiated texture manager");
}


TextureManager::~TextureManager()
{
	map<string, Texture *>::iterator it;
	for (it=textures.begin(); it != textures.end(); it++)
	   RELEASE_SAFELY((*it).second);
	textures.clear();

	LOG("Destroyed texture manager");
}

void TextureManager::add_texture(Texture *texture)
{
	textures[texture->get_name()] = texture;
}


Texture *TextureManager::get_texture(const string & name)
{
	assert(textures.count(name) == 1);

	return textures[name];
}

}



