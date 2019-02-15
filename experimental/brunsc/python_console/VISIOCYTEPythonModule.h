/*
 * VISIOCYTEPythonModule.h
 *
 *  Created on: Dec 22, 2010
 *      Author: Christopher M. Bruns
 */

#ifndef VISIOCYTE_PYTHON_MODULE_H_
#define VISIOCYTE_PYTHON_MODULE_H_

#include <cstddef> // NULL
#include "visiocyte_qt_environment.h"

namespace visiocyte {

// Run initVISIOCYTEPythonModule *AFTER*
// Py_Initialize() has been called.  i.e., after
// PythonInterpreter instance has been created.
// You will still need to "import visiocyte" in python to
// use the visiocyte module.
void initVISIOCYTEPythonModule(VISIOCYTEPluginCallback2 *visiocyte = NULL);

}

#endif /* VISIOCYTE_PYTHON_MODULE_H_ */
