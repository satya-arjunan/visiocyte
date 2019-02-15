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


/*

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


*/
//void swc_to_maskimage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);
//void mrskimage_originalimage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);

void BoundNeuronCoordinates(NeuronTree & neuron,
							bool b_subtractMinFromAllNonnegatives,
							double & output_xmin,
							double & output_xmax,
							double & output_ymin,
							double & output_ymax,
							double & output_zmin,
							double & output_zmax
);

void ComputemaskImage(NeuronTree neurons,
					  unsigned char* pImMask, //output mask image
					  unsigned char* ImMark,  //an indicator image to show whether or not a pixel has been visited/processed
					  VISIOCYTELONG sx,
					  VISIOCYTELONG sy,
					  VISIOCYTELONG sz,
					  int method_code
);

QHash<VISIOCYTELONG, VISIOCYTELONG> NeuronNextPn(const NeuronTree &neurons);
//void swc_to_maskimage_toolbox(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginCallback2 & callback, QWidget * parent);
//bool swc_to_maskimage(VISIOCYTEPluginCallback2 & callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);
//void printHelp();

/*

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
	SetsizeDialog(VISIOCYTEPluginCallback2 &cb, QWidget *parent)
	{
		Image4DSimple* image = cb.getImage(cb.currentImageWindow());
		QString imageName = cb.getImageName(cb.currentImageWindow());
		//create a dialog
		coord_x= new QSpinBox();
		coord_y = new QSpinBox();
		coord_z = new QSpinBox();

		coord_x->setMaximum(1500); coord_x->setMinimum(1); coord_x->setValue(11);
		coord_y->setMaximum(1500); coord_y->setMinimum(1); coord_y->setValue(11);
		coord_z->setMaximum(1500); coord_z->setMinimum(1); coord_z->setValue(11);

		ok     = new QPushButton("OK");
		cancel = new QPushButton("Cancel");
		gridLayout = new QGridLayout();

		labelx = new QLabel(QObject::tr("Image X dimension"));
		labely = new QLabel(QObject::tr("Image Y dimension"));
		labelz = new QLabel(QObject::tr("Image Z dimension"));

		gridLayout->addWidget(labelx, 0,0); gridLayout->addWidget(coord_x, 0,1);
		gridLayout->addWidget(labely, 1,0); gridLayout->addWidget(coord_y, 1,1);
		gridLayout->addWidget(labelz, 2,0); gridLayout->addWidget(coord_z, 2,1);

		gridLayout->addWidget(cancel, 6,1); gridLayout->addWidget(ok, 6,0);
		setLayout(gridLayout);
		setWindowTitle(QString("Change parameters"));

		connect(ok,     SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

		//slot interface
		connect(coord_x, SIGNAL(valueChanged(int)), this, SLOT(update()));
		connect(coord_y,SIGNAL(valueChanged(int)), this, SLOT(update()));
		connect(coord_z,SIGNAL(valueChanged(int)), this, SLOT(update()));
	}

	~SetsizeDialog(){}

	public slots:
	void update();
};

*/




void ComputemaskImage_score(NeuronTree neurons,
					  unsigned char* pImMask, //output mask image
					  unsigned char* ImMark,  //an indicator image to show whether or not a pixel has been visited/processed
					  VISIOCYTELONG sx,
					  VISIOCYTELONG sy,
					  VISIOCYTELONG sz,
					  int method_code,
					  double *score
);


#endif


