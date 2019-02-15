/*
 *  SimpleTracing: NEURON_TRACING.h
 *
 *  Created by Yang, Jinzhu on 20/5/11.
 *  Last update: by PHC, 20110826
 *  Last update: by PHC, 2012-1-25
 *
 */

#ifndef __SIMPLE_NEURON_TRACING__H__
#define __SIMPLE_NEURON_TRACING__H__

#include <QtGui>
#include <stdio.h>
#include <stdlib.h>
#include "visiocyte_interface.h"
#include <vector>
using namespace std;

//#define BACKGROUND -1 //
#define LINETAG -100 // 
//#define DISOFENDS 100  //

// 
typedef struct tagSpacePoint_t
{
	VISIOCYTELONG m_x;
	VISIOCYTELONG m_y;
	VISIOCYTELONG m_z;
	double r;
	VISIOCYTELONG sd;
	VISIOCYTELONG mark;
	VISIOCYTELONG max_ray;
	int ray_value[26];
	int ray_number[26];
	int max_direct;
	VISIOCYTELONG node_id;
	VISIOCYTELONG seg_id;
} SpacePoint_t;

// 
typedef struct tagDFBPoint_t
{
	VISIOCYTELONG m_x;
	VISIOCYTELONG m_y;
	VISIOCYTELONG m_z;
	VISIOCYTELONG m_d;
} DFBPoint_t;

//
typedef struct tagDFSPoint_t
{
	VISIOCYTELONG m_x;
	VISIOCYTELONG m_y;
	VISIOCYTELONG m_z;
	VISIOCYTELONG m_l; 
} DFSPoint_t;

class NeuronPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1);

public:	
	float getPluginVersion() const {return 0.9f;}
	QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
	
	QStringList funclist() const ;
	
	bool dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback, QWidget * parent);

public:
	
	void SetImageInfo1D(VISIOCYTELONG* data, VISIOCYTELONG count, VISIOCYTELONG width, VISIOCYTELONG height);

	void Initialize1D();

	void Set_Seed(SpacePoint_t seed);
	
	void SetEndPoints(vector<SpacePoint_t> points);

		
	void SearchEndPoints();
	
    NeuronTree NeuronTracing_Rollerball_SWC(unsigned char *apsInput, VISIOCYTELONG sx, VISIOCYTELONG sy, VISIOCYTELONG sz);
    NeuronTree NeuronTracing_Ray_SWC(unsigned char *apsInput, VISIOCYTELONG sx, VISIOCYTELONG sy, VISIOCYTELONG sz);
	
	void NeuronTracing_Ray_cast(VISIOCYTEPluginCallback &callback, QWidget *parent);
	
	void NeuronTracing_Ray_D(VISIOCYTEPluginCallback &callback, QWidget *parent);
	
	void Dilation3D(unsigned char *apsInput, unsigned char *aspOutput, VISIOCYTELONG sz, VISIOCYTELONG sy, VISIOCYTELONG sx);
	
	void ExtractSingleCenterpath(DFSPoint_t endP);
	vector<SpacePoint_t> ExtractCenterpath_Ray_Cast(SpacePoint_t points);
	vector<SpacePoint_t> ExtractCenterpath_Ray_D(SpacePoint_t Cpoints,SpacePoint_t NSpoint);
	
	void NueronTree_Ray_D(SpacePoint_t point,int n);
	void NueronTree2(SpacePoint_t point,int n);
	
	void SaveSwcTree_cast(vector<SpacePoint_t> centerpath,VISIOCYTELONG pn_id,VISIOCYTELONG node_count);

	SpacePoint_t Coumpere_ball_center(SpacePoint_t points, int direct, double r);
	SpacePoint_t Coumpere_Next_Point(SpacePoint_t points, int direct, double r);
	
	vector<SpacePoint_t> Rivise_centerpath(vector<SpacePoint_t> centerpath);
	
	SpacePoint_t Rivise_point(SpacePoint_t Rpoint);
	
	SpacePoint_t Center_mass(SpacePoint_t point);
	
	bool PathMask(vector<SpacePoint_t> centerpath);
	
	SpacePoint_t Raycasting2(SpacePoint_t startpoint);
	
	SpacePoint_t GetCenterPiont(SpacePoint_t startpoint,int direction);
	
	int Getmaxderection(SpacePoint_t startpoint);

	void NeuronSeg(VISIOCYTEPluginCallback &callback, QWidget *parent);
	
	vector<SpacePoint_t> BranchDetect2(vector<SpacePoint_t> centerpath);

	
	double fitRadiusPercent(VISIOCYTELONG **img3d, VISIOCYTELONG dim0, VISIOCYTELONG dim1, VISIOCYTELONG dim2, double imgTH, double bound_r,
							float x, float y, float z, float zthickness, bool b_est_in_xyplaneonly);
	
	
	double fitRadiusPercent_cast(VISIOCYTELONG **img3d, VISIOCYTELONG dim0, VISIOCYTELONG dim1, VISIOCYTELONG dim2, double imgTH, double bound_r,
								 float x, float y, float z, float zthickness, bool b_est_in_xyplaneonly);
	
	void Set_local_DFB();
	
	void Set_local_DFS();
	
	void Clear();
	void GetOrgPoint(short *apsInput, VISIOCYTELONG iImgLayer, VISIOCYTELONG iImgHei, VISIOCYTELONG iImgWid, VISIOCYTELONG cnt, SpacePoint_t &pt);

	//entry point functions
    bool Tracing_DistanceField_entry_func(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback &callback);
	void Tracing_DistanceField_entry_func(VISIOCYTEPluginCallback &callback, QWidget *parent);
	template <class T> NeuronTree NeuronTracing_Distance_SWC(T *apsInput, 
                                                             VISIOCYTELONG sx, VISIOCYTELONG sy, VISIOCYTELONG sz, VISIOCYTELONG sc, 
                                                             VISIOCYTELONG ch_tracing, VISIOCYTELONG mx ,VISIOCYTELONG my, VISIOCYTELONG mz);
	
    bool Tracing_Ray_SWC(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback &callback);
	
    bool Tracing_Ball_SWC(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback &callback);
	
    bool Neuron_Seg(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output,VISIOCYTEPluginCallback &callback);
	
	void DFS(bool** matrix, VISIOCYTELONG* neworder, VISIOCYTELONG node, VISIOCYTELONG* id, VISIOCYTELONG siz, int* numbered, int* group);
	
	bool SortSWC(QList<NeuronSWC> & neurons, QList<NeuronSWC> & result, VISIOCYTELONG newrootid, double thres);
	
	double computeDist2(const NeuronSWC & s1, const NeuronSWC & s2);
	
	QHash<VISIOCYTELONG, VISIOCYTELONG> ChildParent(QList<NeuronSWC> &neurons, const QList<VISIOCYTELONG> & idlist, const QHash<VISIOCYTELONG,VISIOCYTELONG> & LUT);
   
	QHash<VISIOCYTELONG, VISIOCYTELONG> getUniqueLUT(QList<NeuronSWC> &neurons);
	
	bool Output_SWC(QList<NeuronSWC> & neurons,  NeuronTree & result);
	
public:
	
	SpacePoint_t                  m_sptSeed;
	
	NeuronTree                    SS;
	QList <NeuronSWC>             listNeuron;
	
	VISIOCYTELONG num ;
	
	VISIOCYTELONG N_first ;
	
//	SpacePoint_t                  direc_point;
	
	vector<DFSPoint_t>            m_vdfsptEndPoint;
	
	vector<DFBPoint_t>            m_vdfbptSurface;
	
	vector<SpacePoint_t>          m_vsptCenterpath;
	
	vector<SpacePoint_t>          m_Markerpoint;
	
	vector<vector<SpacePoint_t> > m_vvsptCenterpaths;
	
	vector<vector<SpacePoint_t> > m_StatisticalPiont; //
	
	vector<vector<SpacePoint_t> > m_GaussFilterPiont;
	
	vector<vector<SpacePoint_t> > m_DetectionPiont;
	
	vector<vector<SpacePoint_t> > centerpathall;
	
	vector<vector<SpacePoint_t> > centerpathall1;
	int count;
	
	//unsigned char**						  m_ppsImgData;
	VISIOCYTELONG**                             m_ppsImgData;
	unsigned char *					      m_ppsMaskData;
	
	unsigned char *					      m_ppsMaskBranch;
	
    VISIOCYTELONG*			              m_ppsOriData1D;
	
	VISIOCYTELONG*	                        m_psTemp;
	
	VISIOCYTELONG                           m_iImgWidth;
	
	VISIOCYTELONG                           m_iImgHeight;
	
	VISIOCYTELONG                           m_OiImgWidth;
	
	VISIOCYTELONG                           m_OiImgHeight;
	
	VISIOCYTELONG                           m_iImgSize;
	
	VISIOCYTELONG                           m_iImgCount;
	
	VISIOCYTELONG                          m_ulVolumeSize;
	
	VISIOCYTELONG                            m_iMinLength;
	
	VISIOCYTELONG*                               m_piDFB;
	VISIOCYTELONG*                               m_piDFS;
	VISIOCYTELONG*                               m_piMskDFS;
	
	QList <ImageMarker> listMarker;
		
};

class NeuronDialog : public QDialog
{
	Q_OBJECT
	
public:
	QGridLayout *gridLayout;
	
	QLabel *labelx;
	QLabel *labely;
    QSpinBox* Ddistance; 
	QSpinBox* Dnumber;
	
	QPushButton* ok;
	QPushButton* cancel;
	
	VISIOCYTELONG Dn;
	VISIOCYTELONG Dh;
	
public:
	NeuronDialog(VISIOCYTEPluginCallback &cb, QWidget *parent)
	{
		Image4DSimple* image = cb.getImage(cb.currentImageWindow());
		QString imageName = cb.getImageName(cb.currentImageWindow());		
		//create a dialog
		Ddistance= new QSpinBox();
		Dnumber = new QSpinBox();
	
		
		Dnumber->setMaximum(255); Dnumber->setMinimum(1); Dnumber->setValue(3);
		Ddistance->setMaximum(255); Ddistance->setMinimum(1); Ddistance->setValue(5);
		
		ok     = new QPushButton("OK");
		cancel = new QPushButton("Cancel");
		gridLayout = new QGridLayout();
		
		labelx = new QLabel(QObject::tr("sampling interval"));
		labely = new QLabel(QObject::tr("number of sampling points"));
		
		gridLayout->addWidget(labelx, 0,0); gridLayout->addWidget(Ddistance, 0,1);
		gridLayout->addWidget(labely, 1,0); gridLayout->addWidget(Dnumber, 1,1);
		
		gridLayout->addWidget(cancel, 6,1); gridLayout->addWidget(ok, 6,0);
		setLayout(gridLayout);
		setWindowTitle(QString("Change parameters"));
		
		connect(ok,     SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
		
		//slot interface
		connect(Ddistance, SIGNAL(valueChanged(int)), this, SLOT(update()));
		connect(Dnumber,SIGNAL(valueChanged(int)), this, SLOT(update()));
	}
	
	~NeuronDialog(){}
	
	public slots:
	void update();
};


//interface to the neuron foreground extraction
bool do_seg(short *pData, VISIOCYTELONG sx, VISIOCYTELONG sy, VISIOCYTELONG sz, int & iVesCnt, bool b_binarization);
void Neuron_segment_entry_func(VISIOCYTEPluginCallback &callback, QWidget *parent);
void Neuron_segment_entry_func(const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback &callback, bool b_binarization);



#endif

