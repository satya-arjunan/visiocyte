/* hdrfilter.h
 * 2011-03-21: create this program by Yang Yu
 */


#ifndef __HDRFILTER_H__
#define __HDRFILTER_H__

//
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

class HDRFilterPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);
	
public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
	QStringList funclist() const {return QStringList();}
	bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,
				VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent) {return true;}
	float getPluginVersion() const {return 1.01f;} // version info 
};

#endif



