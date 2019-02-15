/* iBioformatIO.h
 * 2011-12-02: create this program by Yang Yu
 */


#ifndef __IBIOFORMATIO_H__
#define __IBIOFORMATIO_H__

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

class IBioformatIOPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);
	
public:
    float getPluginVersion() const {return 1.1f;} // version info
    
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
	QStringList funclist() const;
	bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent);
	
};


QString getAppPath();

#endif



