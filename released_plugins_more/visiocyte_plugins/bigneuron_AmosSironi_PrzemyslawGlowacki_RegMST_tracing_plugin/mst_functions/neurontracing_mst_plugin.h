/* neurontracing_mst_plugin.h
 * This is a test plugin, you can use it as a demo.
 * 2014-10-30 : by Zhi Zhou
 */
 
#ifndef __NEURONTRACING_MST_PLUGIN_H__
#define __NEURONTRACING_MST_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>


//#ifndef __MST_PARA__
//#define __MST_PARA__
struct MST_PARA
{
    QString inimg_file;
    VISIOCYTELONG channel;
    VISIOCYTELONG Ws;
    double th;
    double pos_thres;
};
//#endif

void autotrace_mst(VISIOCYTEPluginCallback2 &callback, QWidget *parent, MST_PARA &PARA, bool bmenu);
template <class T> QList<NeuronSWC> seed_detection(T* data1d,
                                      VISIOCYTELONG *in_sz,
                                      unsigned int Ws,
                                      unsigned int c,
                                      double th,
                                      double thresh_pos);

template <class T> T pow2(T a)
{
    return a*a;

}
NeuronTree post_process(NeuronTree nt);


//class neurontracing_mst : public QObject, public VISIOCYTEPluginInterface2_1
//{
//	Q_OBJECT
//	Q_INTERFACES(VISIOCYTEPluginInterface2_1);

//public:
//	float getPluginVersion() const {return 1.1f;}

//	QStringList menulist() const;
//	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

//	QStringList funclist() const ;
//	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
//};

#endif

