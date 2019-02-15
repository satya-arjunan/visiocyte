/* movieZCswitch.h
 * 2009-09-22: create this program by Yang Yu
 */

#ifndef __MOVIEZCSWITCH_H__
#define __MOVIEZCSWITCH_H__

//CHANGES MOVIE STACK STORING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <visiocyte_interface.h>

class MovieZCswitchPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1)

public:
    float getPluginVersion() const {return 1.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
    QStringList funclist() const;
     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

};

#endif



