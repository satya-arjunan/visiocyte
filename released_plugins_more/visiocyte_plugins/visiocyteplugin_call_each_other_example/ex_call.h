/* ex_push.h
 * an example program to test the push function in the plugin interface
* 2010-08-3: by Hanchuan Peng
 */


#ifndef __EX_CALL_H__
#define __EX_CALL_H__

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>

#include "visiocyte_interface.h"

class ExCallPlugin : public QObject, public VISIOCYTEPluginInterface2_1
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

class matrixPanel : public QDialog
{
	Q_OBJECT;
public:
	QDoubleSpinBox* a[3][3];
	QDoubleSpinBox* b[3][3];
	QLabel* c[3][3];
	VISIOCYTEPluginCallback2 &visiocyte;
	double A[3][3], B[3][3], C[3][3];

	virtual ~matrixPanel() {};
	matrixPanel(VISIOCYTEPluginCallback2 &_visiocyte, QWidget *parent);
	void begin();
	void end();

public slots:
	void rand_a();
	void rand_b();

	void add();
	void mul();
	void at();
	void bt();
};


#endif

