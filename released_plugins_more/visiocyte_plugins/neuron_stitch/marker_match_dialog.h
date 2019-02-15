#ifndef MARKER_MATCH_DIALOG_H
#define MARKER_MATCH_DIALOG_H

#include <QDialog>
#include <visiocyte_interface.h>
#include "neuron_stitch_func.h"

class marker_match_dialog: public QDialog
{
    Q_OBJECT

public:
    marker_match_dialog(VISIOCYTEPluginCallback2 * cb, LandmarkList * mList);

private:
    LandmarkList * mList;
    QList<int> m0, m1;
    QList<int> mm0, mm1;

public:
    QGridLayout *gridLayout;
    QListWidget * list_pairs;
    QPushButton * btn_add;
    QPushButton * btn_free;
    QPushButton * btn_done;
    QComboBox * combobox_n0;
    QComboBox * combobox_n1;

private slots:
    void add();
    void free();
};

#endif // MARKER_MATCH_DIALOG_H
