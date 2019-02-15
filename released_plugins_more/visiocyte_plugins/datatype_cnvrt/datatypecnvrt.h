/* datatypecnvrt.h
 * 2011-01-21: create this program by Yang Yu
 */

// Upgraded to VISIOCYTEPluginInterface2_1 by Jianlong Zhou, 2012-04-05
//last change 2013-11-05 by Hanchuan Peng.

#ifndef __DATATYPECNVRT_H__
#define __DATATYPECNVRT_H__

//
#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

class DTCPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.31f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
	bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,
          VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent);
};

class DTCDialog : public QDialog
{
	Q_OBJECT

public:
	DTCDialog(VISIOCYTEPluginCallback2 &callback, QWidget *parent)
	{
		visiocytehandleList win_list = callback.getImageWindowList();

		visiocytehandle wincurr = callback.currentImageWindow(); //
		QString itemcurr = callback.getImageName(wincurr);
		int idxcurr = 0;

		QStringList items;
		for (int i=0; i<win_list.size(); i++)
		{
			QString item = callback.getImageName(win_list[i]);

			items << item;

			if(item.compare(itemcurr) == 0)
				idxcurr = i;
		}

		combo_subject =  new QComboBox(); combo_subject->addItems(items);
		combo_subject->setCurrentIndex(idxcurr);

		label_subject = new QLabel(QObject::tr("Image: "));

		isub = combo_subject->currentIndex();

		//
		label_sub_dt = new QLabel(QObject::tr("Which datatype convert to: "));

		QStringList dtList;
		dtList << "VISIOCYTE_UINT8" << "VISIOCYTE_UINT16" << "VISIOCYTE_FLOAT32";

		combo_dt = new QComboBox(); combo_dt->addItems(dtList);

		//
		ok     = new QPushButton("OK");
		cancel = new QPushButton("Cancel");

		// gridlayout
		QGridLayout* gridLayout = new QGridLayout();

		gridLayout->addWidget(label_subject, 0,0,1,1); gridLayout->addWidget(combo_subject, 0,1,1,4);
		gridLayout->addWidget(label_sub_dt, 1,0); gridLayout->addWidget(combo_dt, 1,1);

		gridLayout->addWidget(cancel, 3,3,Qt::AlignRight); gridLayout->addWidget(ok, 3,4,Qt::AlignRight);

		setLayout(gridLayout);
		setWindowTitle(QString("Image Stitching"));

		// signal and slot
		connect(ok,     SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

		connect(combo_subject, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

	}

	~DTCDialog(){}

public slots:
	void update()
	{
		isub = combo_subject->currentIndex();
		dt = combo_dt->currentIndex();
	}

public:

	int isub;
	int dt;

	QLabel* label_subject;
	QComboBox* combo_subject;

	QLabel* label_sub_dt;
	QComboBox* combo_dt;

	QPushButton* ok;
	QPushButton* cancel;

};

#endif



