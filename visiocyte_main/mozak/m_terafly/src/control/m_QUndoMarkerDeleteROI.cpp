#include "visiocyter_glwidget.h"
#include "m_QUndoMarkerDeleteROI.h"
#include "m_CViewer.h"
#include "m_PAnoToolBar.h"

itm::QUndoMarkerDeleteROI::QUndoMarkerDeleteROI(itm::CViewer* _source, QList<LocationSimple> _markers) :  QUndoCommand()
{
    source = _source;
    markers = _markers;
    redoFirstTime = true;

    printf("QUndoMarkerDeleteROI created with markers ");
    for(int i=0; i<markers.size(); i++)
        printf("(%.1f, %.1f, %.1f) ", markers[i].x, markers[i].y, markers[i].z);
    printf("\n");
}

// undo and redo methods
void itm::QUndoMarkerDeleteROI::undo()
{
    /**/itm::debug(itm::LEV1, 0, __itm__current__function__);

    // get markers from Visiocyte
    QList<LocationSimple> visiocyteMarkers = source->VISIOCYTE_env->getLandmark(source->window);

    // add previously deleted markers
    for(int i=0; i<markers.size(); i++)
        visiocyteMarkers.push_back(markers[i]);

    // set new markers
    source->VISIOCYTE_env->setLandmark(source->window, visiocyteMarkers);
    source->VISIOCYTE_env->pushObjectIn3DWindow(source->window);

    //update visible markers
    PAnoToolBar::instance()->buttonMarkerRoiViewChecked(PAnoToolBar::instance()->buttonMarkerRoiView->isChecked());


    //QMessageBox::information(0, "Info", itm::strprintf("QUndoMarkerDeleteROI::undo()\n\nAdded %d markers", markers.size()).c_str());
    // end select mode
    //source->view3DWidget->getRenderer()->endSelectMode();
}

void itm::QUndoMarkerDeleteROI::redo()
{
    /**/itm::debug(itm::LEV1, itm::strprintf("redoFirstTime = %s", redoFirstTime ? "true" : "false").c_str(), __itm__current__function__);

    // first time redo's call is aborted: we don't want it to be called once the command is pushed into the QUndoStack
    if(!redoFirstTime)
    {
        // get markers from Visiocyte
        QList<LocationSimple> visiocyteMarkers = source->VISIOCYTE_env->getLandmark(source->window);

        // remove again the markers previosly deleted
        int count = 0;
        for(int j=0; j<markers.size(); j++)
        {
            bool removed = false;
            for(int i=0; i<visiocyteMarkers.size() && !removed; i++)
            {
                if(visiocyteMarkers[i].x == markers[j].x && visiocyteMarkers[i].y == markers[j].y && visiocyteMarkers[i].z == markers[j].z)
                {
                    visiocyteMarkers.removeAt(i);
                    count++;
                    removed = true;
                }
            }
//            if(!removed)
//                QMessageBox::warning(0, "warning", itm::strprintf("cannot find marker at (%.1f, %.1f, %.1f)", markers[j].x, markers[j].y, markers[j].z).c_str());
        }

        // set new markers
        source->VISIOCYTE_env->setLandmark(source->window, visiocyteMarkers);
        source->VISIOCYTE_env->pushObjectIn3DWindow(source->window);

        //update visible markers
        PAnoToolBar::instance()->buttonMarkerRoiViewChecked(PAnoToolBar::instance()->buttonMarkerRoiView->isChecked());

        //QMessageBox::information(0, "Info", itm::strprintf("QUndoMarkerDeleteROI::undo()\n\nRemoved %d markers",count).c_str());

        // end select mode
        //source->view3DWidget->getRenderer()->endSelectMode();
    }
    else
        redoFirstTime = false;
}
