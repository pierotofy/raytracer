#include <sstream>
#include "Texture.h"
#include "Globals.h"
#include <assert.h>
#include "Utils.h"

using namespace ptrt;

namespace ptrt{
	std::ostream &operator<<(std::ostream& os, ptrt::Texture& s)
	{
		s.print(os);
		return os;
	}
}

ostream& Texture::print(std::ostream& os = std::cout){
	  os << "Texture {" << endl
	     << "  name: " << name << endl
	     << "}" << endl;
	  return os;
}

Texture::Texture(const string &name, const string& filename)
 : name(name){
	textureImage.read(g_path_prefix + filename);

	width = textureImage.get_width();
	height = textureImage.get_height();
}

Texture::Texture(map<string, SceneDataContainer> &sdMap){
	std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

	sdIter = sdMap.find("texture_name");
	assert( sdIter != sdMap.end() );
	name = sdIter->second.val;

	string textureFilename;
	sdIter = sdMap.find("texture_sourcefile");
	assert( sdIter != sdMap.end() );
	textureFilename = sdIter->second.val;

	textureImage.read(g_path_prefix + textureFilename);
	width = textureImage.get_width();
	height = textureImage.get_height();
}

Vector3D Texture::get_color(float u, float v){
	int y = u * (width - 1);
	int x = v * (height - 1);

	if (y > (width - 1)) y = width - 1;
	if (y < 0) y = 0;

	if (x > (height - 1)) x = height - 1;
	if (x < 0) x = 0;

	png::rgb_pixel texelData = textureImage[x][y];
	Vector3D color((int)(texelData.red)/255.0f,
					(int)(texelData.green)/255.0f,
					(int)(texelData.blue)/255.0f);

	return color;
}


Texture::~Texture(){
	LOG("Texture destructor called: " << name);
}

