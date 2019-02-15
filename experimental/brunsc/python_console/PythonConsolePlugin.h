
#ifndef VISIOCYTE_PYTHON_CONSOLE_PLUGIN_H
#define VISIOCYTE_PYTHON_CONSOLE_PLUGIN_H

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include "visiocyte_interface.h"
class PythonConsoleWindow;

class PythonConsolePlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	QStringList menulist() const;
	void domenu(const QString & menu_name, VISIOCYTEPluginCallback2 & visiocyte,
			QWidget * parent);

	QStringList funclist() const {return QStringList();}
	bool dofunc(const QString & func_name,
			const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,
			VISIOCYTEPluginCallback2 & visiocyte,  QWidget * parent)
	{
		return true;
	}

    float getPluginVersion() const {return 0.60f;}

    static PythonConsoleWindow* pythonConsoleWindow;
};

#endif // VISIOCYTE_PYTHON_CONSOLE_PLUGIN_H

