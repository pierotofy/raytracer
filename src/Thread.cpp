
#include "Utils.h"
#include "Thread.h"
#include <pthread.h>


Thread::Thread()
{
	/* initialized with default attributes */
	pthread_attr_init(&tattr);

	/* call an appropriate functions to alter a default value */
	//pthread_attr_*(&tattr,SOME_ATRIBUTE_VALUE_PARAMETER);
}

Thread::~Thread()
{
}

void* Thread::_run(void *threadObj){
	((Thread *)threadObj)->run();

	pthread_exit(0);

	return 0;
}

void Thread::join(){
	pthread_join(thread, NULL);
}


void Thread::start()
{
	pthread_create(&thread, &tattr, _run, this);
}

