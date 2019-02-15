/* calculate_reliability_score_plugin.h
 * This plugin will generate the reliability score of neuron reconstruction.
 * 2015-4-27 : by Hanbo Chen, Hang Xiao, Hanchuan Peng
 */
 
#ifndef __CALCULATE_RELIABILITY_SCORE_PLUGIN_H__
#define __CALCULATE_RELIABILITY_SCORE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class neuronScore : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
	float getPluginVersion() const {return 1.1f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

class neuronScoreDialog : public QDialog
{
    Q_OBJECT

public:
    neuronScoreDialog(VISIOCYTEPluginCallback2 &callback, QWidget *parent);
    ~neuronScoreDialog();

private:
    VISIOCYTEPluginCallback2* callback;
    QWidget *parent;

private:
    void creat();
    void checkButtons();
    void initDlg();

public slots:
    void set_img();
    void set_swc();
    void set_output();
    void accept();

public:
    bool isaccept;
    QLineEdit *edit_img, *edit_swc, *edit_output;
    QPushButton *btn_img, *btn_swc, *btn_output;
    QDoubleSpinBox *spin_radiusRate;
    QComboBox *cb_scoreType;
    QPushButton *btn_quit, *btn_ok;
    QTextEdit *edit_info;
};

void doCalculateScore(VISIOCYTEPluginCallback2 &callback, QString fname_img, QString fname_swc, QString fname_output, int score_type, float radius_factor, bool is_gui);
NeuronTree calculateScoreTerafly(VISIOCYTEPluginCallback2 &callback,QString fname_img, NeuronTree nt, int score_type, float radius_factor);

void printHelp();

#endif

