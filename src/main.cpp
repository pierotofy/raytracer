#include <iostream>
#include <cstdlib>
#include <time.h>

#include "cs5721Code/cs5721GraphicsLib/src/Random.h"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include <boost/filesystem/path.hpp>

#include "Globals.h"
#include "Scene.h"
#include "OrthNormBasis.h"
#include "Matrix4.h"
#include "TextureManager.h"
#include "InstanceManager.h"

using namespace sivelab;
using namespace ptrt;
GraphicsArgs g_args;
std::string g_path_prefix;

int main(int argc, char *argv[])
{
  // Find 3D flag
  bool render3D = false;
  for (int i = 1; i < argc; i++){
	  if (string(argv[i]) == "-3"){
		  render3D = true;
	  }
  }

  // Take arguments
  g_args.process(argc, argv);

  // Initialize random seed
  srand(time(NULL));
  srand48(time(NULL));

  TextureManager::Initialize();
  InstanceManager::Initialize();

  // Extract path prefix
  boost::filesystem::path input_file_path(g_args.inputFileName);
  g_path_prefix = input_file_path.branch_path().string() + "/";

  if (g_args.outputFileName != ""){
	  if (g_args.inputFileName != ""){
			Scene scene(g_args.inputFileName);

			if (render3D) scene.render3D(g_args.outputFileName);
			else scene.render(g_args.outputFileName);
	  }else{
			std::cerr << "Need input file!" << std::endl;
			exit(EXIT_FAILURE);
	  }
  }else{
		std::cerr << "Need output file!" << std::endl;
		exit(EXIT_FAILURE);
  }

  InstanceManager::Destroy();
  TextureManager::Destroy();

  return EXIT_SUCCESS;
}
