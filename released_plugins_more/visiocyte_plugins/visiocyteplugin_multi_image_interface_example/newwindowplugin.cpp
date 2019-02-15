/**
 *  100811 RZC: change to handle any type image using  Image4DProxy's value_at/put_at
 **/

#include <QtGui>

#include <math.h>
#include <stdlib.h>

#include "newwindowplugin.h"

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(newwindow, NewWindowPlugin);

void Invert(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);
void Threshold(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);
void Compute(char op, VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);

const QString title = "VISIOCYTEPluginInterface demo";
QStringList NewWindowPlugin::menulist() const
{
    return QStringList()
    << tr("Invert Color (in current window)")
    << tr("Threshold...")
    << tr("Add 2 Images...")
    << tr("Differ 2 Images...")
    << tr("Modulate 2 Images...")
    << tr("about");
}

void NewWindowPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent)
{
    if (menu_name == tr("Invert Color (in current window)"))
    {
    	Invert(visiocyte, parent);
    }
    else
    if (menu_name == tr("Threshold..."))
    {
    	Threshold(visiocyte, parent);
    }
    else
    if (menu_name == tr("Add 2 Images..."))
    {
    	Compute('+', visiocyte, parent);
    }
    else
    if (menu_name == tr("Differ 2 Images..."))
    {
    	Compute('-', visiocyte, parent);
    }
    else
    if (menu_name == tr("Modulate 2 Images..."))
    {
    	Compute('*', visiocyte, parent);
    }
    else
    {
    	QMessageBox::information(parent, title, "VISIOCYTEPluginInterface Demo version 1.2"
    			"\ndeveloped by Zongcai Ruan 2009-2010. (Janelia Farm Research Campus, HHMI)");
    }
}

void Invert(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent)
{
	visiocytehandle oldwin = visiocyte.currentImageWindow();
	Image4DSimple* image = visiocyte.getImage(oldwin);
	if (! image)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return;
	}
//	if (image->getDatatype()!=VISIOCYTE_UINT8)
//	{
//		QMessageBox::information(0, title, QObject::tr("This demo program only supports 8-bit data. Your current image data type is not supported."));
//		return;
//	}

	Image4DProxy<Image4DSimple> p(image);
//	for (uint8* ip=p.begin(); ip<=p.end(); ip++)
//	{
//		*ip = 255 - *ip;
//	}
	Image4DProxy_foreach(p, x,y,z,c)
	{
		double f = 0;
		if (p.is_inner(x,y,z,c)) f = p.value_at(x,y,z,c);

		f = 255-f;

		p.put_at(x,y,z,c, (f));
	}

	visiocyte.setImageName(oldwin, visiocyte.getImageName(oldwin)+"_invert");
	visiocyte.updateImageWindow(oldwin);
}

void Threshold(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent)
{
	visiocytehandle oldwin = visiocyte.currentImageWindow();
	Image4DSimple* image = visiocyte.getImage(oldwin);
	if (! image)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return;
	}
//	if (image->getDatatype()!=VISIOCYTE_UINT8)
//	{
//		QMessageBox::information(0, title, QObject::tr("This demo program only supports 8-bit data. Your current image data type is not supported."));
//		return;
//	}

	bool ok;
    int threshold = QInputDialog::getInteger(parent, QObject::tr("Threshold"),
                                             QObject::tr("Enter threshold:"),
                                             100, 0, 255, 1, &ok);
    if (! ok) return;

	VISIOCYTELONG N = image->getTotalBytes();
	unsigned char* newdata1d = new unsigned char[N];
	Image4DSimple tmp;
	tmp.setData(newdata1d, image->getXDim(),image->getYDim(),image->getZDim(),image->getCDim(), image->getDatatype());

	Image4DProxy<Image4DSimple> p0(image);
	Image4DProxy<Image4DSimple> p(&tmp);
//	uint8 *ip0, *ip;
//	for (ip0=p0.begin(), ip=p.begin();
//		ip<=p.end(); ip0++, ip++)
//    {
//		if (*ip0 <= threshold)  *ip = 0;
//		else *ip = *ip0;
//	}
	Image4DProxy_foreach(p, x,y,z,c)
	{
		double f = 0;
		if (p0.is_inner(x,y,z,c)) f = p0.value_at(x,y,z,c);

		if (f <= threshold) f = 0;

		p.put_at(x,y,z,c, (f));
	}

	visiocytehandle newwin = visiocyte.newImageWindow();
	visiocyte.setImage(newwin, &tmp);
	visiocyte.setImageName(newwin, visiocyte.getImageName(oldwin)+"_threshold");
    visiocyte.updateImageWindow(newwin);
}

void Compute(char op, VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent)
{
	visiocytehandleList win_list = visiocyte.getImageWindowList();
	if (win_list.size()<1)
	{
		QMessageBox::information(0, title, QObject::tr("Need at least 1 images."));
		return;
	}

	QStringList items;
	for (int i=0; i<win_list.size(); i++) items << visiocyte.getImageName(win_list[i]);

	QDialog d(parent);
	QComboBox* combo1 = new QComboBox(); combo1->addItems(items);
	QComboBox* combo2 = new QComboBox(); combo2->addItems(items);
	QPushButton* ok     = new QPushButton("OK");
	QPushButton* cancel = new QPushButton("Cancel");
	QFormLayout *formLayout = new QFormLayout;
	formLayout->addRow(QObject::tr("image1: "), combo1);
	formLayout->addRow(QObject::tr("image2: "), combo2);
	formLayout->addRow(ok, cancel);
	d.setLayout(formLayout);
	d.setWindowTitle(QString("image1 %1 image2").arg(op));

	d.connect(ok,     SIGNAL(clicked()), &d, SLOT(accept()));
	d.connect(cancel, SIGNAL(clicked()), &d, SLOT(reject()));
	if (d.exec()!=QDialog::Accepted)
		return;

	int i1 = combo1->currentIndex();
	int i2 = combo2->currentIndex();

	Image4DSimple* image1 = visiocyte.getImage(win_list[i1]);
	Image4DSimple* image2 = visiocyte.getImage(win_list[i2]);


	if (!image1 || !image2)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return;
	}
//	if (image1->getDatatype()!=VISIOCYTE_UINT8 || image2->getDatatype()!=VISIOCYTE_UINT8)
//	{
//		QMessageBox::information(0, title, QObject::tr("This demo program only supports 8-bit data. Your current image data type is not supported."));
//		return;
//	}


	VISIOCYTELONG N1 = image1->getTotalBytes();
	unsigned char* newdata1d = new unsigned char[N1];

	Image4DSimple tmp;
	tmp.setData(newdata1d, image1->getXDim(),image1->getYDim(),image1->getZDim(),image1->getCDim(), image1->getDatatype());

	Image4DProxy<Image4DSimple> p1(image1);
	Image4DProxy<Image4DSimple> p2(image2);
	Image4DProxy<Image4DSimple> p(&tmp);

	Image4DProxy_foreach(p, x,y,z,c)
	{
		double f = 0;
		double f1 = 0;
		double f2 = 0;
//		if (p1.is_inner(x,y,z,c)) f1 = (*p1.at_uint8(x,y,z,c))/255.f;
//		if (p2.is_inner(x,y,z,c)) f2 = (*p2.at_uint8(x,y,z,c))/255.f;
		if (p1.is_inner(x,y,z,c)) f1 = p1.value_at(x,y,z,c)/255.f;
		if (p2.is_inner(x,y,z,c)) f2 = p2.value_at(x,y,z,c)/255.f;

		switch (op)
		{
		case '+': f = f1 + f2; break;
		case '-': f = f1 - f2; break;
		case '*': f = f1 * f2; break;
		}
		f = fabs(f);
		if (f>1) f = 1;

//		*p.at_uint8(x,y,z,c) = (unsigned char)(f*255);
		p.put_at(x,y,z,c, (f*255));
	}


	visiocytehandle newwin = visiocyte.newImageWindow();
	visiocyte.setImage(newwin, &tmp);
	visiocyte.setImageName(newwin, "new_image_arithmetic");
    visiocyte.updateImageWindow(newwin);
}
