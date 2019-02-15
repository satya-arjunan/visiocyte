/*
 * VISIOCYTEPythonModule.cpp
 *
 *  Created on: Dec 22, 2010
 *      Author: Christopher M. Bruns
 */

#include "VISIOCYTEPythonModule.h"
#include <boost/python.hpp>
#include "visiocyte_interface.h"
#include <exception>
#include <iostream>

// Store a permanent pointer to the callback the VISIOCYTEConsolePlugin was launched with.
VISIOCYTEPluginCallback2 *visiocyte_callbackPtr;

#include "generated_code/visiocyte.main.cpp"

namespace visiocyte {

VISIOCYTEPluginCallback2* get_plugin_callback() {
    return visiocyte_callbackPtr;
}

void initVISIOCYTEPythonModule(VISIOCYTEPluginCallback2 *callback)
{
    // load module of automatically generated wrappers
    initvisiocyte();

    if (callback) visiocyte_callbackPtr = callback;
}

} // namespace visiocyte
