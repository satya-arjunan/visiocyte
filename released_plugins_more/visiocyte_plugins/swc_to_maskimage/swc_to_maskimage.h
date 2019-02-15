/*
 * swc_to_maskimg.h
 *
 *  Created by Yang, Jinzhu on 11/27/10.
 *  Last change: by Hanchuan Peng, 2010-Dec-7. change some writing style of this plugin header
 *
 */

#ifndef __SWC_TO_MASKIMAGE_H_
#define __SWC_TO_MASKIMAGE_H

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include "visiocyte_interface.h"
#include "filter_dialog.h"


class SWC_TO_MASKIMAGElugin: public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.1f;}
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

};

void swc_to_maskimage(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
bool swc_to_maskimage(VISIOCYTEPluginCallback2 & callback,const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void swc_filter_image(VISIOCYTEPluginCallback2 & callback,const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
void printHelp();


class SetsizeDialog : public QDialog
{
	Q_OBJECT

public:
	QGridLayout *gridLayout;
	QLabel *labelx;
	QLabel *labely;
	QLabel *labelz;
    QLabel *info;
    QSpinBox* coord_x;
	QSpinBox* coord_y;
	QSpinBox* coord_z;
    QDialog mydialog;
	QPushButton* ok;
	QPushButton* cancel;

public:
	SetsizeDialog(VISIOCYTEPluginCallback2 &cb, QWidget *parent)
	{
//		Image4DSimple* image = cb.getImage(cb.currentImageWindow());
//		QString imageName = cb.getImageName(cb.currentImageWindow());

        info= new QLabel;
        info->setText("The minimum dimensions of generated mask image are"
                      " set as default.Please input new size. <p>"
                      "Warning: size significantly below the default size"
                      " will result in incomplete mask image. ");
        info->setWordWrap(true);
        info->setFixedWidth(400);

		//create a dialog

		coord_x= new QSpinBox();
		coord_y = new QSpinBox();
		coord_z = new QSpinBox();

        coord_x->setMaximum(4000); coord_x->setMinimum(1); coord_x->setValue(11);
        coord_y->setMaximum(4000); coord_y->setMinimum(1); coord_y->setValue(11);
        coord_z->setMaximum(4000); coord_z->setMinimum(1); coord_z->setValue(11);

		ok     = new QPushButton("OK");
		cancel = new QPushButton("Cancel");
		gridLayout = new QGridLayout();

        labelx = new QLabel(QObject::tr("Image X dimension:"));
        labely = new QLabel(QObject::tr("Image Y dimension:"));
        labelz = new QLabel(QObject::tr("Image Z dimension:"));

        info->setMargin(10);
        gridLayout->addWidget(info,0,0,4,2);
        gridLayout->addWidget(labelx, 4,0,1,1); gridLayout->addWidget(coord_x,4,1,1,1);
        gridLayout->addWidget(labely, 5,0,1,1); gridLayout->addWidget(coord_y,5,1,1,1);
        gridLayout->addWidget(labelz, 6,0,1,1); gridLayout->addWidget(coord_z,6,1,1,1);
        gridLayout->addWidget(cancel, 7,1,1,1); gridLayout->addWidget(ok, 7,0,1,1);
		setLayout(gridLayout);
		setWindowTitle(QString("Change parameters"));

		connect(ok,     SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	}

	~SetsizeDialog(){}

};

#endif


