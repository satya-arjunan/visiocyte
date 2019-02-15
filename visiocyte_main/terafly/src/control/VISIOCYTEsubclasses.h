#ifndef VISIOCYTESUBCLASSES_H
#define VISIOCYTESUBCLASSES_H

#include "CPlugin.h"
#include "renderer_gl1.h"
#include "renderer_gl2.h"
#include "visiocyter_glwidget.h"
#include "visiocyter_mainwindow.h"
#include "visiocyte_imaging_para.h"

/**********************************************************************************
* Visiocyte subclasses needed to access/override protected members/methods
***********************************************************************************/

class terafly::myRenderer_gl1 : public Renderer_gl1
{
    public:

        //gives public access to members
        friend class CViewer;
        friend class myVisiocyteR_GLWidget;

        //converts mouse 2D position into image 3D point
        XYZ get3DPoint(int x, int y);

        //casting
        //--- note ---: dynamic_cast would be better, but needs too many Visiocyte sources to be included
        static myRenderer_gl1* cast(Renderer_gl1* instance){return static_cast<myRenderer_gl1*>(instance);}
        static myRenderer_gl1* cast(Renderer* instance){return static_cast<myRenderer_gl1*>(instance);}
};

class terafly::myVisiocyteR_GLWidget : public VisiocyteR_GLWidget
{
    Q_OBJECT

    public:

        //gives public access to members
        friend class CViewer;

        //casting
        //--- note ---: dynamic_cast would be better, but needs too many Visiocyte sources to be included
        static myVisiocyteR_GLWidget* cast(VisiocyteR_GLWidget* instance){return static_cast<myVisiocyteR_GLWidget*>(instance);}

        //@Override
        void wheelEventO(QWheelEvent *event);

        //void wheelEvent(QWheelEvent *event) {qDebug()<<"myVisiocyteR_GLWidget::wheelEvent ... ..."; wheelEventO(event);}

        //zoomIn method(s)
        void zoomIn(const char* method);

        //fast version (without displaying progress bar) of updateImageData method
        void updateImageDataFast();

    public:

        //@Override
        void setZoomO(int zr);
        void setZoomO(float zr);
};

class terafly::myVisiocyteR_MainWindow : public VisiocyteR_MainWindow
{
    public:

        //gives public access to members
        friend class CViewer;

        //casting
        //--- note ---: dynamic_cast would be better, but needs too many Visiocyte sources to be included
        static myVisiocyteR_MainWindow* cast(VisiocyteR_MainWindow* instance){return static_cast<myVisiocyteR_MainWindow*>(instance);}
};

class terafly::myImage4DSimple : Image4DSimple
{
    public:

        //gives public access to members
        friend class CViewer;
        friend class myVisiocyteR_GLWidget;

        //casting
        //--- note ---: dynamic_cast would be better, but needs too many Visiocyte sources to be included
        static myImage4DSimple* cast(Image4DSimple* instance){return static_cast<myImage4DSimple*>(instance);}
};

#endif // VISIOCYTESUBCLASSES_H
