//plugin_image_registration.h
//by Lei Qu
//2011-04-08

#ifndef __PLUGIN_IMAGE_REGISTRATION_H__
#define __PLUGIN_IMAGE_REGISTRATION_H__

#include <visiocyte_interface.h>


class ImageRegistrationPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 0.9f;} // version info

    QStringList menulist() const;
    void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

    QStringList funclist() const;
    bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & visiocyte, QWidget * parent);

};


#endif
