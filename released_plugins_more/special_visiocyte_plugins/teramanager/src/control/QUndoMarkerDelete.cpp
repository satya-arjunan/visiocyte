#include "QUndoMarkerDelete.h"
#include "../control/CViewer.h"
#include "visiocyter_glwidget.h"
#include "../presentation/PAnoToolBar.h"

itm::QUndoMarkerDelete::QUndoMarkerDelete(itm::CViewer* _source, LocationSimple _marker) :  QUndoCommand()
{
    source = _source;
    marker = _marker;
    redoFirstTime = true;
}

// undo and redo methods
void itm::QUndoMarkerDelete::undo()
{
    /**/itm::debug(itm::LEV1, 0, __itm__current__function__);

    // get markers from Visiocyte
    QList<LocationSimple> visiocyteMarkers = source->VISIOCYTE_env->getLandmark(source->window);

    // add previously deleted marker
    visiocyteMarkers.push_back(marker);

    // set new markers
    source->VISIOCYTE_env->setLandmark(source->window, visiocyteMarkers);
    source->VISIOCYTE_env->pushObjectIn3DWindow(source->window);

    //update visible markers
    PAnoToolBar::instance()->buttonMarkerRoiViewChecked(PAnoToolBar::instance()->buttonMarkerRoiView->isChecked());

    // end select mode
    //source->view3DWidget->getRenderer()->endSelectMode();
}

void itm::QUndoMarkerDelete::redo()
{
    /**/itm::debug(itm::LEV1, itm::strprintf("redoFirstTime = %s", redoFirstTime ? "true" : "false").c_str(), __itm__current__function__);

    // first time redo's call is aborted: we don't want it to be called once the command is pushed into the QUndoStack
    if(!redoFirstTime)
    {
        // get markers from Visiocyte
        QList<LocationSimple> visiocyteMarkers = source->VISIOCYTE_env->getLandmark(source->window);

        // remove again the marker previosly deleted
        for(int i=0; i<visiocyteMarkers.size(); i++)
            if(visiocyteMarkers[i].x == marker.x && visiocyteMarkers[i].y == marker.y && visiocyteMarkers[i].z == marker.z)
                visiocyteMarkers.removeAt(i);

        // set new markers
        source->VISIOCYTE_env->setLandmark(source->window, visiocyteMarkers);
        source->VISIOCYTE_env->pushObjectIn3DWindow(source->window);

        //update visible markers
        PAnoToolBar::instance()->buttonMarkerRoiViewChecked(PAnoToolBar::instance()->buttonMarkerRoiView->isChecked());

        // end select mode
        //source->view3DWidget->getRenderer()->endSelectMode();
    }
    else
        redoFirstTime = false;
}
