#include <assert.h>
#include <cstdlib>
#include "RenderThread.h"
#include "Globals.h"


using namespace ptrt;
using namespace std;

int RenderThread::instances_count = 0;

RenderThread::RenderThread(Scene *scene, png::image< png::rgb_pixel > *imData){
	this->scene = scene;
	this->imData = imData;

	thread_id = RenderThread::instances_count;
	RenderThread::instances_count++;

	number_of_samples = (float)g_args.rpp;

	// Compute in advance
	number_of_samples_squared = number_of_samples*number_of_samples;
}


void RenderThread::run(){
	// Count progress
	int percentage_completed = 0;

	for (unsigned int c = 0; c < pixels_queue.size(); c++){
		size_t i = pixels_queue[c].i;
		size_t j = pixels_queue[c].j;

		Vector3D color = Vector3D(0.0f, 0.0f, 0.0f);

		// Jittering loop
		for (int p = 0; p < (int)number_of_samples; p++){
			for (int q = 0; q < (int)number_of_samples; q++){
				ray.compute_camera(*(scene->getCameras()[0]),
								(float)i + ((float)p + drand48())/number_of_samples,
								(float)j + ((float)q + drand48())/number_of_samples);
				ray.set_random_uv();

				float t_min = 1;
				float t_max = FLT_MAX;
				HitStructure hit;
				Shader::intersect_all_objects(scene->getObjects(), ray, t_min, t_max, hit);

				if (hit.obj != 0){ // Hit?
					// Create shader args structure
					ShaderArgs sa(hit, ray, scene->getLights(), scene->getObjects(), 0, t_max, false);

					color += hit.shader->apply_shader(sa);
				}else{
					// Background
					color += Shader::background_color;
				}
			}
		}

		// Average all colors sampled
		color = color/number_of_samples_squared;

		// Set to pixel
		(*imData)[j][i] = Shader::get_rgb_from_color(color);


		// Print out progress
		if (thread_id == 0){
			percentage_completed = (int)((float)c/(float)pixels_queue.size() * 100.0f);
			cout << percentage_completed << '%' << '\r';
			cout.flush();
		}
	}

	RenderThread::instances_count--;
}

void RenderThread::schedule(size_t i, size_t j){
	pixels_queue.push_back(PixelSchedule(i, j));
}

RenderThread::~RenderThread(){

}

