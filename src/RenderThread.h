#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "cs5721Code/cs5721GraphicsLib/src/png++/png.hpp"
#include "cs5721Code/cs5721GraphicsLib/src/Vector3D.h"
#include "Ray.h"
#include "Thread.h"
#include "Scene.h"

using namespace sivelab;
using namespace std;

namespace ptrt{
	struct PixelSchedule{
		size_t i;
		size_t j;

		PixelSchedule(size_t i, size_t j) :
			i(i), j(j){}
	};

	class RenderThread : public Thread{
		private:
			float number_of_samples;
			float number_of_samples_squared;
			Ray ray;

			Scene *scene;
			png::image< png::rgb_pixel > *imData;
			vector<PixelSchedule> pixels_queue;
			int thread_id;

		public:
			RenderThread(Scene *scene, png::image< png::rgb_pixel > *imData);
			virtual ~RenderThread();

			void schedule(size_t i, size_t j);

			virtual void run();

			static int instances_count;
	};
}
#endif
