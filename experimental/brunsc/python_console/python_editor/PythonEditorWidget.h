#ifndef VISIOCYTE_PYTHON_EDITOR_H_
#define VISIOCYTE_PYTHON_EDITOR_H_

#include "ui_python_editor.h"
#include <QMainWindow>

class PythonEditorWidget : public QMainWindow, Ui::python_editor
{
        Q_OBJECT
public:
        PythonEditorWidget(QWidget *parent = NULL);
};

#endif /* VISIOCYTE_PYTHON_EDITOR_H_ */

