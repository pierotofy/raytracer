#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <pthread.h>

class Thread{
private:
	pthread_t thread;
	pthread_attr_t tattr;

	static void *_run(void *threadObj);
public:
	Thread();
	virtual ~Thread();

	virtual void run() = 0;
	void start();
	void join();
};

 #endif
