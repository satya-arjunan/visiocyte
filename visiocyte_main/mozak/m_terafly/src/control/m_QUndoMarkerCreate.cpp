#include "visiocyter_glwidget.h"
#include "m_QUndoMarkerCreate.h"
#include "m_CViewer.h"
#include "m_PAnoToolBar.h"

itm::QUndoMarkerCreate::QUndoMarkerCreate(itm::CViewer* _source, LocationSimple _marker) : QUndoCommand()
{
    source = _source;
    marker = _marker;
    redoFirstTime = true;
}

// undo and redo methods
void itm::QUndoMarkerCreate::undo()
{
    /**/itm::debug(itm::LEV1, 0, __itm__current__function__);

    // get markers from Visiocyte
    QList<LocationSimple> visiocyteMarkers = source->VISIOCYTE_env->getLandmark(source->window);

    // remove the marker just created
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

void itm::QUndoMarkerCreate::redo()
{
    /**/itm::debug(itm::LEV1, itm::strprintf("redoFirstTime = %s", redoFirstTime ? "true" : "false").c_str(), __itm__current__function__);

    // first time redo's call is aborted: we don't want it to be called once the command is pushed into the QUndoStack
    if(!redoFirstTime)
    {
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
    else
        redoFirstTime = false;
}



itm::QUndoVisiocyteNeuron::QUndoVisiocyteNeuron(itm::CViewer* _source) : QUndoCommand()
{
    source = _source;
    redoFirstTime = true;
}

// undo and redo methods
void itm::QUndoVisiocyteNeuron::undo()
{
    /**/itm::debug(itm::LEV1, 0, __itm__current__function__);

    if (visiocyter_getImage4d(source->view3DWidget->_idep) && source->view3DWidget->renderer)
    {
        visiocyter_getImage4d(source->view3DWidget->_idep)->proj_trace_history_undo();
        visiocyter_getImage4d(source->view3DWidget->_idep)->update_3drenderer_neuron_view(source->view3DWidget, (Renderer_gl1*)source->view3DWidget->renderer);//090924

        source->view3DWidget->update();
    }
}

void itm::QUndoVisiocyteNeuron::redo()
{
    /**/itm::debug(itm::LEV1, itm::strprintf("redoFirstTime = %s", redoFirstTime ? "true" : "false").c_str(), __itm__current__function__);

    // first time redo's call is aborted: we don't want it to be called once the command is pushed into the QUndoStack
    if(!redoFirstTime)
    {
        if (visiocyter_getImage4d(source->view3DWidget->_idep) && source->view3DWidget->renderer)
        {
            visiocyter_getImage4d(source->view3DWidget->_idep)->proj_trace_history_redo();
            visiocyter_getImage4d(source->view3DWidget->_idep)->update_3drenderer_neuron_view(source->view3DWidget, (Renderer_gl1*)source->view3DWidget->renderer);//090924

            source->view3DWidget->update();
        }
    }
    else
        redoFirstTime = false;
}
