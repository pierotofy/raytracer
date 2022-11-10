#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include "Utils.h"

void log_exception(const char *msg, bool abort){
	std::cerr << "Exception: " << msg << " | last errno: " << strerror(errno) << std::endl;
	if (abort){
		std::cerr << "Exiting with error code 1" << std::endl;
		exit(1);
	}
}

void log_exception(const string& msg, bool abort){
	log_exception(msg.c_str(), abort);
}


