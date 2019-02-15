/*
 *  montage_image_sections.h
 *  montage_image_sections
 *
 *  Created by Yang, Jinzhu on 11/22/10.
 *
 *
 */

#ifndef __MONTAGE_IMAGE_SECTION_H_
#define __MONTAGE_IMAGE_SECTION_H_

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>

#include "visiocyte_interface.h"

class MONTAGEPlugin: public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	template <class T>
	void montage_image_sections(T *apsInput, T * aspOutput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer,bool b_draw);
	template <class T>
	void montage_image_stack(T *apsInput, T * aspOutput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer,VISIOCYTELONG column,VISIOCYTELONG row, VISIOCYTELONG slice,VISIOCYTELONG n);
	//void do_computation (float *apsInput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer, VISIOCYTELONG* label){}
	//void do_computation (unsigned char *apsInput, unsigned char * aspOutput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer, VISIOCYTELONG h){}

        float getPluginVersion() const {return 1.11f;}
};
class SetsizeDialog : public QDialog
{
	Q_OBJECT

public:
	QGridLayout *gridLayout;

	QLabel *labelx;
	QLabel *labely;
	QLabel *labelz;
    QSpinBox* coord_x;
	QSpinBox* coord_y;
	QSpinBox* coord_z;

	QPushButton* ok;
	QPushButton* cancel;

	VISIOCYTELONG NX;
	VISIOCYTELONG NY;
	VISIOCYTELONG NZ;

public:
	SetsizeDialog(VISIOCYTEPluginCallback &cb, QWidget *parent)
	{
		Image4DSimple* image = cb.getImage(cb.currentImageWindow());
		QString imageName = cb.getImageName(cb.currentImageWindow());
		//create a dialog

		coord_y = new QSpinBox();
		coord_y->setMaximum(1500); coord_y->setMinimum(2); coord_y->setValue(7);

		coord_x = new QSpinBox();
		coord_x->setMaximum(1500); coord_x->setMinimum(2); coord_x->setValue(7);

		ok = new QPushButton("OK");

		cancel = new QPushButton("Cancel");

		gridLayout = new QGridLayout();

		labelx = new QLabel(QObject::tr("number of column blooks"));

		labely = new QLabel(QObject::tr("number of row blooks"));

		gridLayout->addWidget(labely, 0,0); gridLayout->addWidget(coord_y, 0,1);

		gridLayout->addWidget(labelx, 1,0); gridLayout->addWidget(coord_x, 1,1);

		gridLayout->addWidget(cancel, 6,1); gridLayout->addWidget(ok, 6,0);

		setLayout(gridLayout);

		setWindowTitle(QString("Change parameters"));
		connect(ok,     SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

		connect(coord_x,SIGNAL(valueChanged(int)), this, SLOT(update()));
		connect(coord_y,SIGNAL(valueChanged(int)), this, SLOT(update()));
	}

	~SetsizeDialog(){}

	public slots:
	void update();
};
//define a simple dialog for choose DT parameters

#endif


