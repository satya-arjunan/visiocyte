/*
 * pythonConsoleTest.cpp
 *
 *  Created on: Dec 18, 2010
 *      Author: Christopher M. Bruns
 */

#include "PythonInterpreter.h"
#include "PythonConsoleWindow.h"
#include <boost/python.hpp>
#include "VISIOCYTEPythonModule.h"

using namespace visiocyte;
namespace bp = boost::python;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PythonConsoleWindow* pythonConsole = new PythonConsoleWindow();
    initVISIOCYTEPythonModule();
    pythonConsole->executeCommand("import visiocyte");

    pythonConsole->show();

    return app.exec();
}
