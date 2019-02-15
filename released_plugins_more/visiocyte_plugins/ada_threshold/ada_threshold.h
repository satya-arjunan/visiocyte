/*
 *  ada_threshold.h
 *
 *  Created by Yang, Jinzhu on 11/22/10.
 *  Add dofunc() interface by Jianlong  Zhou, 2012-04-18
 */

#ifndef __ADAPTIVE_THRESHOLDING__H__
#define __ADAPTIVE_THRESHOLDING__H__

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>

#include "visiocyte_interface.h"

class ThPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	template <class T>
	void BinaryProcess(T *apsInput, T * aspOutput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer, VISIOCYTELONG h, VISIOCYTELONG d){}

     float getPluginVersion() const {return 1.1f;}
};

//define a simple dialog for choose DT parameters
class AdaTDialog : public QDialog
{
	Q_OBJECT

public:
	QGridLayout *gridLayout;

	QLabel *labelx;
	QLabel *labely;
    QSpinBox* Ddistance;
	QSpinBox* Dnumber;

	QPushButton* ok;
	QPushButton* cancel;

	VISIOCYTELONG Dn;
	VISIOCYTELONG Dh;

public:
	AdaTDialog(VISIOCYTEPluginCallback2 &cb, QWidget *parent)
	{
		Image4DSimple* image = cb.getImage(cb.currentImageWindow());
		QString imageName = cb.getImageName(cb.currentImageWindow());
		//create a dialog
		Ddistance= new QSpinBox();
		Dnumber = new QSpinBox();


		Dnumber->setMaximum(255); Dnumber->setMinimum(1); Dnumber->setValue(3);
		Ddistance->setMaximum(255); Ddistance->setMinimum(1); Ddistance->setValue(5);

		ok     = new QPushButton("OK");
		cancel = new QPushButton("Cancel");
		gridLayout = new QGridLayout();

		labelx = new QLabel(QObject::tr("sampling interval"));
		labely = new QLabel(QObject::tr("number of sampling points"));

		gridLayout->addWidget(labelx, 0,0); gridLayout->addWidget(Ddistance, 0,1);
		gridLayout->addWidget(labely, 1,0); gridLayout->addWidget(Dnumber, 1,1);

		gridLayout->addWidget(cancel, 6,1); gridLayout->addWidget(ok, 6,0);
		setLayout(gridLayout);
		setWindowTitle(QString("Change parameters"));

		connect(ok,     SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

		//slot interface
		connect(Ddistance, SIGNAL(valueChanged(int)), this, SLOT(update()));
		connect(Dnumber,SIGNAL(valueChanged(int)), this, SLOT(update()));
	}

	~AdaTDialog(){}

	public slots:
	void update();
};

#endif


