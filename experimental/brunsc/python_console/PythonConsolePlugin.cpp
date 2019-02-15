
#include "PythonConsolePlugin.h"
#include "PythonConsoleWindow.h"
#include "VISIOCYTEPythonModule.h"
#include <iostream>

using namespace std;

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(PythonConsole, PythonConsolePlugin);

PythonConsoleWindow* PythonConsolePlugin::pythonConsoleWindow = 0;

QString getMovieMakerScriptPath() 
{
    QString path = QCoreApplication::applicationDirPath() + 
        "/plugins/python_scripting/python_console/movie_gui.py";
    cerr << path.toStdString() << endl;
    if (QFile(path).exists())
        return path;
    else return "";
}

QStringList PythonConsolePlugin::menulist() const
{
    QStringList answer = QStringList();
    if (getMovieMakerScriptPath().length() > 0)
        answer << tr("Movie maker...");
    answer
        << tr("Run script...")
		<< tr("Open VISIOCYTE python console...")
		<< tr("About VISIOCYTE python console...");
    return answer;
}

void createPythonConsole(VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent) 
{
	if (PythonConsolePlugin::pythonConsoleWindow) return;
	PythonConsolePlugin::pythonConsoleWindow = new PythonConsoleWindow(parent);
	visiocyte::initVISIOCYTEPythonModule(&visiocyte);
	PythonConsolePlugin::pythonConsoleWindow->executeCommand("import visiocyte");
}

void launchMovieMaker(VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
    QString movieScriptPath = getMovieMakerScriptPath();
    if (movieScriptPath.length() > 0) {
	    // PythonConsolePlugin::pythonConsoleWindow->show(); // TODO - possibly remove this line?
        PythonConsolePlugin::pythonConsoleWindow->pythonInterpreter->
            runScriptFile(movieScriptPath);
    }
}

void PythonConsolePlugin::domenu(const QString & menu_name,
		VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
	if (menu_name == tr("Open VISIOCYTE python console...")) {
		createPythonConsole(visiocyte, parent);
		pythonConsoleWindow->show();
	}
	else if (menu_name == tr("Movie maker...")) {
    	createPythonConsole(visiocyte, parent);
		launchMovieMaker(visiocyte, parent);
	}
	else if (menu_name == tr("Run script...")) {
		createPythonConsole(visiocyte, parent);
		PythonConsolePlugin::pythonConsoleWindow->runScript();
	}
	else
	{ // User selected "About..."
        QString msg = QString("VISIOCYTE python console plugin version %1\n"
        		"Developed by Christopher M. Bruns. \n"
        		"(Janelia Farm Research Campus, HHMI)")
                .arg(getPluginVersion(), 1, 'f', 1);
		QMessageBox::information(parent, "About the VISIOCYTE python console", msg);
	}
}
