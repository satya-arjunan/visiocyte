/*
 * neuron_feature.h
 *
 *  Created by Yinan Wan on 07/07/11.
 *  Last change: 
 *
 */

#ifndef __NEURON_FEATURE_H
#define __NEURON_FEATURE_H

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include "visiocyte_interface.h"
#include "basic_surf_objs.h"

class GNFPlugin: public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);
	
public:
	float getPluginVersion() const {return 1.0f;}
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	QStringList funclist() const ;
	bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback, QWidget * parent);
};

void global_neuron_feature(VISIOCYTEPluginCallback &callback, QWidget *parent, int method_code);

#endif


