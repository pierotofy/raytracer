#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <cmath>
#include <stdlib.h>
#include "float.h"

//#define DEBUG 1

using namespace std;

#define RELEASE_SAFELY(__POINTER) { delete __POINTER; __POINTER = NULL; }

#ifdef DEBUG
#define LOG(__MSG) { std::cout << "DEBUG: " << __MSG << std::endl; }
#else
#define LOG(__MSG) { }
#endif

#define SMALL_EPS 0.001f
#define FEQUAL(a,b) (fabs((a) - (b)) < SMALL_EPS)
#define FEQUALZERO(a) (fabs(a) < SMALL_EPS)
#define FEQUALVECTORS(a,b) ( ( FEQUAL(a[0], b[0]) ) && ( FEQUAL(a[1], b[1]) ) && ( FEQUAL(a[2], b[2]) ))


void log_exception(const char *, bool);
void log_exception(const string&, bool);

#endif
