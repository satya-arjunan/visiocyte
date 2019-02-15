/* recenterimageplugin.h
 * 2009-08-14: created by Yang Yu
 * 2010-05-20: by PHC for compatability to VC compiler
 * 2011-09-16: change interface from "VISIOCYTESingleImageInterface2_1" to "VISIOCYTEPluginInterface2_1" by Yang Yu
 */


#ifndef __RECENTERIMAGEPLUGIN_H__
#define __RECENTERIMAGEPLUGIN_H__

//  recenter subject image into the center of target image
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

#if defined (_MSC_VER)  //2010-05-20, by PHC for compatability to VC compiler
#include "vcdiff.h"
#else
#endif

class ReCenterImagePlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1)

public:
    float getPluginVersion() const {return 1.1f;}
    
    QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
	QStringList funclist() const;
	bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent);
};

#endif



