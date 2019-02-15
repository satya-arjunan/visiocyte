/* neuron_connector_plugin.h
 * This plugin will connect the segments in swc file that meet the creterian. Only tips will be connected to other segments.
 * 2014-11-03 : by Hanbo Chen
 */
 
#ifndef __NEURON_CONNECTOR_PLUGIN_H__
#define __NEURON_CONNECTOR_PLUGIN_H__

#include <QtGui>
#include <QDialog>
#include <visiocyte_interface.h>

class neuron_connector_dialog : public QDialog
{
    Q_OBJECT

public:
    neuron_connector_dialog();
    ~neuron_connector_dialog();
private:
    void creat();
    void initDlg();
    bool checkbtn();
    void addinfo(QString info, bool bold=false);

public slots:
    bool load0();
    bool load1();
    void myconfig();
    void run();

public:
    QGridLayout *gridLayout;
    QPushButton *btn_load0, *btn_load1;
    QLabel *label_load0, *label_load1;
    QLineEdit *edit_load0, *edit_load1;
    QDoubleSpinBox *spin_zscale, *spin_xscale, *spin_yscale, *spin_ang, *spin_dis;
    QSpinBox *spin_rootid;
    QPushButton *btn_quit, *btn_run;
    QComboBox *cb_distanceType, *cb_matchType, *cb_conf;
    QTextEdit* text_info;
};



class neuron_connector_swc : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

    void doconnect(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

//void printHelp();
//void connectall(NeuronTree* nt, QList<NeuronSWC>& newNeuron, double xscale, double yscale, double zscale, double angThr, double disThr, int matchtype, bool minusradius, int rootID);
//double getswcdiameter(NeuronTree* nt);
//bool export_list2file(const QList<NeuronSWC>& lN, QString fileSaveName);

#endif

