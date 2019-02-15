//plugin_nonrigid_registration.h
//by Lei Qu
//2010-02-26

#ifndef __PLUGIN_NONRIGID_REGISTRATION_H__
#define __PLUGIN_NONRIGID_REGISTRATION_H__

#include <visiocyte_interface.h>


class NonrigidRegistrationPlugin : public QObject, public VISIOCYTEPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface);

public:
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback &callback, QWidget *parent);

	QStringList funclist() const {return QStringList();}
	void dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, QWidget *parent) {}
};


#endif
