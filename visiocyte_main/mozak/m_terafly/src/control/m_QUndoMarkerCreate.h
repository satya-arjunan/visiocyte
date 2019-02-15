#ifndef QUNDOMARKERCREATE_H
#define QUNDOMARKERCREATE_H

#include "visiocyte_interface.h"
#include <QUndoCommand>
#include "m_CPlugin.h"

class teramanager::QUndoMarkerCreate : public QUndoCommand
{
    private:

        itm::CViewer* source;       //where the command has been applied
        LocationSimple marker;              //the marker being created
        bool redoFirstTime;                 //to disable redo's first call

    public:

        QUndoMarkerCreate(itm::CViewer* _source, LocationSimple _marker);

        // undo and redo methods
        virtual void undo();
        virtual void redo();
    
};

class teramanager::QUndoVisiocyteNeuron : public QUndoCommand
{
    private:

        itm::CViewer* source;       //where the command has been applied
        bool redoFirstTime;         //to disable redo's first call

    public:

        QUndoVisiocyteNeuron(itm::CViewer* _source);

        // undo and redo methods
        virtual void undo();
        virtual void redo();

};

#endif // QUNDOMARKERCREATE_H
