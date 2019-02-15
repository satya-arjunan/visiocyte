#ifndef VISIOCYTE_MOVIE_MAKER_H_
#define VISIOCYTE_MOVIE_MAKER_H_

#include "visiocyte_interface.h"

class MovieMakerPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT;
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:

    QStringList menulist() const;
    void domenu(const QString & menu_name, VISIOCYTEPluginCallback2 & visiocyte,  QWidget * parent);

    QStringList funclist() const {return QStringList();}
    bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,
                 VISIOCYTEPluginCallback2 & visiocyte,  QWidget * parent) {return true;}

    float getPluginVersion() const {return 1.1f;}
};

#endif /* VISIOCYTE_MOVIE_MAKER_H_ */

