/*
 * Globals.h
 *
 *  Created on: 2012-01-31
 *      Author: piero
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "cs5721Code/cs5721GraphicsLib/src/handleGraphicsArgs.h"
using namespace sivelab;

/* We need to access the command line arguments from several places in the code,
 * makes more sense to declare this global than trickling it down the class structure
 */
extern GraphicsArgs g_args;

/* A prefix to append to all resources (extracted relative to the path of the xml file) */
extern std::string g_path_prefix;

#define USE_BVH true

#endif /* GLOBALS_H_ */
