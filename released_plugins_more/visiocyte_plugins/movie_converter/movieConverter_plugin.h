/* Movie_plugin.h
 * Movie Generator
 * 2013-13-10 : by Zhi Zhou
 */
 
#ifndef __MOVIE_PLUGIN_H__
#define __MOVIE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>

class MovieConverter : public QObject, public VISIOCYTEPluginInterface2_1
{
	Q_OBJECT
	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:
    float getPluginVersion() const {return 1.2f;}

	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const ;
	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

QString getAppPath();

class controlPanel: public QDialog
{
    Q_OBJECT

public:
    controlPanel(VISIOCYTEPluginCallback2 &visiocyte, QWidget *parent);
    ~controlPanel();

public:
    long m_lframeind;
    QLineEdit *m_pLineEdit_filepath;
    QLineEdit *m_pLineEdit_fps;
    QLineEdit *m_pLineEdit_filename;
    QLineEdit *m_pLineEdit_ffmpegpath;
    VISIOCYTEPluginCallback2 &m_visiocyte;
    static controlPanel *m_pLookPanel;
    QCheckBox* check_compress;
    QComboBox* combo_type;

private slots:
    void _slot_start();
    void _slot_close();
    void _slots_openFileDlg_output();
    void _slots_openFileDlg_ffmpeg();
};

#endif

