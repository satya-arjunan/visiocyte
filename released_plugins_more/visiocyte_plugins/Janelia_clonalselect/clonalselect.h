/* clonalselect.h
 * 2013-01-13: create this program by Yang Yu
 */


#ifndef __CLONALSELECT_H__
#define __CLONALSELECT_H__

//
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

class ClonalSelectPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);
	
public:
    float getPluginVersion() const {return 1.0f;} // version info
    
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
	QStringList funclist() const;
	bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent);
	
};

#endif // __CLONALSELECT_H__



