/* bioFormat_ex.h
 * 2009-09-20: create this program by Yang Yu
 */

#ifndef __BIOFORMAT_EX_H__
#define __BIOFORMAT_EX_H__

//EXAMPLES USING BIOFORMAT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <visiocyte_interface.h>

class BioFormat_ExPlugin : public QObject, public VISIOCYTEPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface)

public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback &callback, QWidget *parent);
	
	QStringList funclist() const {return QStringList();}
	void dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, QWidget *parent) {}
};

#endif



