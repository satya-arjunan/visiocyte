/* istitch.h
 * 2010-02-08: create this program by Yang Yu
 * 2011-07-11: add dofunc function call group stitching by Yang Yu
 */


#ifndef __ISTITCH_H__
#define __ISTITCH_H__

//
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

//----------------------------------------------------------------------
#ifdef COMPILE_TO_COMMANDLINE
#else

class IStitchPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

#endif
//----------------------------------------------------------------------

#endif



