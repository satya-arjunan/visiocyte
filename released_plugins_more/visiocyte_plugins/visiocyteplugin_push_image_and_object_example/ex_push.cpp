/* ex_push.cpp
 * an example program to test the push function in the plugin interface
 * 2010-08-3: by Hanchuan Peng
 */


#include "ex_push.h"
#include "visiocyte_message.h"

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(ex_push, ExPushPlugin);

void dopush(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent, int method_code);

class lookPanel : public QDialog
	{
	public:
		QSpinBox* box1;
		QSpinBox* box2;
		QSpinBox* box3;
		VISIOCYTEPluginCallback2 &visiocyte;
		static lookPanel* panel;

		virtual ~lookPanel()
		{
			panel = 0;
		}
		lookPanel(VISIOCYTEPluginCallback2 &_visiocyte, QWidget *parent) : QDialog(parent),
		visiocyte(_visiocyte)
		{
			panel = this;

			box1 = new QSpinBox(); box1->setRange(-100,100);
			box2 = new QSpinBox(); box2->setRange(-100,100);
			box3 = new QSpinBox(); box3->setRange(-100,100);
			QPushButton* ok     = new QPushButton("OK");
			QPushButton* cancel = new QPushButton("Cancel");
			QFormLayout *formLayout = new QFormLayout;
			formLayout->addRow(QObject::tr("look along X: "), box1);
			formLayout->addRow(QObject::tr("look along Y: "), box2);
			formLayout->addRow(QObject::tr("look along Z: "), box3);
			formLayout->addRow(ok, cancel);

			//QDialog d(parent);
			setLayout(formLayout);
			setWindowTitle(QString("look along vector"));

			connect(ok,     SIGNAL(clicked()), this, SLOT(accept()));
			connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
		}
		virtual void accept()
		{
			int i1 = box1->value();
			int i2 = box2->value();
			int i3 = box3->value();

			visiocytehandle curwin = visiocyte.currentImageWindow();
			if (curwin)//ensure the 3d viewer window is open; if not, then open it
			{
				visiocyte.open3DWindow(curwin);

				View3DControl *view = visiocyte.getView3DControl(curwin);
				if (view)  view->lookAlong(i1,i2,i3);

				visiocyte.updateImageWindow(curwin);
			}
		}
	};

lookPanel* lookPanel::panel = 0;

VISIOCYTELONG panel(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent)
{
	if (lookPanel::panel)
	{
		lookPanel::panel->show();
		return -1;
	}

	lookPanel* p = new lookPanel(visiocyte, parent);
	if (p)	p->show();
	return (VISIOCYTELONG)p;
}




//plugin funcs
const QString title = "Example for pushing image and objects";
QStringList ExPushPlugin::menulist() const
{
    return QStringList()
		<< tr("Close and Open 3D viewer and Push Image")
		<< tr("Push Object and Screenshot of global 3d viewer and also change 3d viewer")
		<< tr("Push time point of 3d viewer")
		<< tr("Look along direction in 3d viewer")
		<< tr("About");
}

void ExPushPlugin::domenu(const QString & menu_name, VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent)
{
    if (menu_name == tr("Close and Open 3D viewer and Push Image"))
    {
    	dopush(visiocyte, parent, 1);
    }
	else if (menu_name == tr("Push Object and Screenshot of global 3d viewer and also change 3d viewer"))
	{
    	dopush(visiocyte, parent, 2);
	}
	else if (menu_name == tr("Push time point of 3d viewer"))
	{
    	dopush(visiocyte, parent, 3);
	}
	else if (menu_name == tr("Look along direction in 3d viewer"))
	{
    	dopush(visiocyte, parent, 4);
	}
	else
	{
        QString msg = QString("Push Plugin version %1\ndeveloped by Hanchuan Peng & Zongcai Ruan. (Janelia Farm Research Campus, HHMI)")
                .arg(getPluginVersion(), 1, 'f', 1);
		QMessageBox::information(parent, "Version info", msg);
	}
}

//lookPanel* panel(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);


void dopush(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent, int method_code)
{
	visiocytehandle curwin = visiocyte.currentImageWindow();
	if (!curwin)
	{
		visiocyte_msg("You don't have any image open in the main window.");
		return;
	}
	Image4DSimple *oldimg = visiocyte.getImage(curwin);
	VISIOCYTELONG oldsz0=oldimg->getXDim(), oldsz1=oldimg->getYDim(), oldsz2=oldimg->getZDim();

	if (method_code==1) //push image
	{
		//close the current window
		visiocyte.close3DWindow(curwin);

//		//for (int j=1; j<1000; j++) //try to empty all existing events
//		{
//			QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
//		}

		//now should be able to open the new window
		visiocyte.open3DWindow(curwin);

		//now push the data to the 3d viewer's display
		for (int curloop=0; curloop<1; curloop++)
		{
//			Image4DSimple * oldimg = visiocyte.getImage(curwin);
//
			Image4DSimple p4DImage;
			unsigned char * pData = 0;
			VISIOCYTELONG sz0=oldsz0*0.9, sz1=oldsz1*.9, sz2=oldsz2*.9, sz3=oldimg->getCDim();
			VISIOCYTELONG totallen = sz0*sz1*sz2*sz3;
			pData = new unsigned char [totallen];
			memcpy(pData, oldimg->getRawData(), totallen);

			p4DImage.setData((unsigned char*)pData, sz0, sz1, sz2, sz3, VISIOCYTE_UINT8);

			visiocyte.setImage(curwin, &p4DImage);
			visiocyte.setImageName(curwin, QString("push now %1").arg(curloop));
			visiocyte.updateImageWindow(curwin);

			visiocyte.pushImageIn3DWindow(curwin);
            qDebug()<<"-------------------"<<curloop;
		}
	}
	else if (method_code==2) //push marker and swc
	{
		//ensure the 3d viewer window is open; if not, then open it
		visiocyte.open3DWindow(curwin);

		//now push the data to the 3d viewer's display
		for (int curloop=0; curloop<=100; curloop++)
		{
			LandmarkList curlist;
			for (int i=0;i<20; i++)
			{
				LocationSimple s;
				s.x = rand()%oldsz0;
				s.y = rand()%oldsz1;
				s.z = rand()%oldsz2;
				if (i<10)
					s.radius = 10*i;
				else {
					s.radius=s.radius*0.5;
				}

				curlist << s;
			}

			visiocyte.setLandmark(curwin, curlist);
			visiocyte.setImageName(curwin, QString("push now %1").arg(curloop));

			visiocyte.pushObjectIn3DWindow(curwin);

			//100812 RZC: put here in loop is more safe, every loop checking makes sure that the view3d is open.
			View3DControl *view = visiocyte.getView3DControl(curwin);
			if (view)  view->setXRotation(curloop*360/100);
			if (view)  view->setYRotation(curloop*360/100);
			if (view)  view->setZRotation(curloop*360/100);

			visiocyte.updateImageWindow(curwin);

//			QString BMPfilename = QString("aaa_%1").arg(curloop);
//			visiocyte.screenShot3DWindow(curwin, BMPfilename);
		}
	}
	else if (method_code==3) //push timepoint
	{
		VISIOCYTELONG szt = oldimg->getTDim();

		//ensure the 3d viewer window is open; if not, then open it
		visiocyte.open3DWindow(curwin);

		//now push the time point in the 3d viewer's display
		for (int t=0; t<szt; t++)
		{
			visiocyte.pushTimepointIn3DWindow(curwin, t);
			visiocyte.updateImageWindow(curwin);
		}
	}
	else if (method_code==4) //look along
	{
		panel(visiocyte, parent);
	}
}


