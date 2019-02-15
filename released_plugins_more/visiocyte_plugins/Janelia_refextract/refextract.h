/* refextract.h
 * created by Yang Yu, Dec 16, 2011
 */


#ifndef __REFEXTRACT_H__
#define __REFEXTRACT_H__

//
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

class RefExtractPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

#endif



