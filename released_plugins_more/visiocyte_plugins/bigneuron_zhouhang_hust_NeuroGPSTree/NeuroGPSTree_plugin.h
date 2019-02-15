/* NeuroGPSTree_plugin.h
 * Trace dense network with multi-Neuron
 * Permit to provide soma swc file for the position of somas
 * 2016-1-1 : by ZhouHang
 * Main algorithm: Quan Tingwei, Zhou Hang, Zeng Shaoqun
 */

#ifndef __NEUROGPSTREE_PLUGIN_H__
#define __NEUROGPSTREE_PLUGIN_H__

#include <QtGui>
#include <visiocyte_interface.h>
#include <Eigen/Core>
#include "ngtypes/basetypes.h"
#include "ngtypes/volume.h"
#include "ngtypes/soma.h"
#include "ngtypes/tree.h"
typedef unsigned char UChar;
class BinarySettingsDockWidget;
class NeuroGLWidget;
class INeuronDataObject;
class BinaryFilter;
class TraceFilter;
class BridgeBreaker;
class NeuroTreeCluster;
#ifdef _WIN32
typedef std::tr1::shared_ptr<BinaryFilter> NGBinaryFilter;
typedef std::tr1::shared_ptr<TraceFilter> NGTraceFilter;
typedef std::tr1::shared_ptr<BridgeBreaker> NGBridgeBreaker;
typedef std::tr1::shared_ptr<NeuroTreeCluster> NGNeuronTreeCluster;
#else
typedef std::shared_ptr<BinaryFilter> NGBinaryFilter;
typedef std::shared_ptr<TraceFilter> NGTraceFilter;
typedef std::shared_ptr<BridgeBreaker> NGBridgeBreaker;
typedef std::shared_ptr<NeuroTreeCluster> NGNeuronTreeCluster;
#endif


class NeuroGPSTreePlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:

    float getPluginVersion() const {return 1.1f;}

    QStringList menulist() const;
    void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

    QStringList funclist() const ;
    bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

private:
    struct input_PARA
    {
        QString inimg_file;
        VISIOCYTELONG channel;
        double xRes_, yRes_, zRes_;
        double binaryThreshold;
        double traceValue;
        double enhanceValue;
        std::string swcfile;
        int threadNum;//TODO:for openmp accelerate
        input_PARA():channel(1),xRes_(1),yRes_(1),zRes_(1),binaryThreshold(6),swcfile(""),threadNum(1){}
    };

    int histgram[1021];

protected:

    void reconstruction_func(VISIOCYTEPluginCallback2 &callback, QWidget *parent, input_PARA &PARA, bool bmenu);
#ifdef _WIN32
    bool ConvertVisiocyteImg2NGImg(UChar*, int, int, int, double, double, double,
        std::tr1::shared_ptr<INeuronDataObject>);
#else
    bool ConvertVisiocyteImg2NGImg(UChar*, int, int, int, double, double, double,
        std::shared_ptr<INeuronDataObject>);
#endif

    void SetRealResolution(double res, int origSz, int& scale, int& sz);

    bool SaveSeperateTree(const QString& filename, const QStringList &infostring);

    void AutoBinaryImage();

    void AutoTrace();
    void GetHist(int z);
    void FillBlackArea(int z);
private:
    int X, Y, Z;
    double xres_, yres_, zres_;
    int xScale_, yScale_, zScale_;

    input_PARA PARA;

    NGBinaryFilter filter;
    NGTraceFilter traceFilter;
    NGBridgeBreaker breaker;
    NGNeuronTreeCluster treeCluster;
#ifdef _WIN32
    std::tr1::shared_ptr<INeuronDataObject> OrigImage;
    std::tr1::shared_ptr<INeuronDataObject> BinImage;
    std::tr1::shared_ptr<INeuronDataObject> BackImage;
    std::tr1::shared_ptr<VectorVec3i> binPtSet ;
    std::tr1::shared_ptr<INeuronDataObject> soma;
    std::tr1::shared_ptr<INeuronDataObject> tree;
    std::tr1::shared_ptr<INeuronDataObject> treeConInfo;
    std::tr1::shared_ptr<INeuronDataObject> seperateTree;
#else
    std::shared_ptr<INeuronDataObject> OrigImage;
    std::shared_ptr<INeuronDataObject> BinImage;
    std::shared_ptr<INeuronDataObject> BackImage;
    std::shared_ptr<VectorVec3i> binPtSet ;
    std::shared_ptr<INeuronDataObject> soma;
    std::shared_ptr<INeuronDataObject> tree;
    std::shared_ptr<INeuronDataObject> treeConInfo;
    std::shared_ptr<INeuronDataObject> seperateTree;
#endif

};

#endif

