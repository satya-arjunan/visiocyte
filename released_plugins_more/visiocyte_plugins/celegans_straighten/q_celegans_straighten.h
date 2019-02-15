// q_celegans_straighten.h
// by Lei Qu
// 2010-08-11

#ifndef __Q_CELEGANS_STRAIGHTEN_H__
#define __Q_CELEGANS_STRAIGHTEN_H__

#include <vector>
using namespace std;

#include "visiocyte_interface.h"

class CSParas
{
public:
	//file i/o
	bool 	b_imgfromVISIOCYTE;
	bool 	b_markerfromVISIOCYTE;
	//skeleton detection
	VISIOCYTELONG 	l_refchannel;
	double 	d_downsampleratio;
	VISIOCYTELONG 	l_ctlpts_num;
	VISIOCYTELONG	l_radius_openclose;
	double	d_fgthresh_xy;
	double	d_fgthresh_xz;
	//straighten
	VISIOCYTELONG 	l_radius_cuttingplane;
	//visualize
	bool 	b_showinVISIOCYTE_pts;

	CSParas()
	{
		b_imgfromVISIOCYTE=1;
		b_markerfromVISIOCYTE=1;
		l_refchannel=1;
		d_downsampleratio=4;
		l_ctlpts_num=10;
		l_radius_openclose=0;
		d_fgthresh_xy=3;
		d_fgthresh_xz=0;
		l_radius_cuttingplane=80;
		b_showinVISIOCYTE_pts=0;
	}
};



//simply straighten along the given markers which defined along the central line of the worm
bool q_celegans_straighten_manual(VISIOCYTEPluginCallback &callback,const CSParas &paras,
		const unsigned char *p_img_input,const VISIOCYTELONG sz_img_input[4],
		const vector< vector<double> > vec2d_markers,
        unsigned char *&p_img_output,VISIOCYTELONG sz_img_output[4]);


bool q_celegans_straighten(VISIOCYTEPluginCallback &callback,const CSParas &paras,
		const unsigned char *p_img_input,const VISIOCYTELONG sz_img_input[4],
		const vector< vector<double> > vec2d_markers,
        unsigned char *&p_img_output,VISIOCYTELONG sz_img_output[4]);


bool q_celegans_restacking_xy(
		const unsigned char *p_inputimg,const VISIOCYTELONG *sz_inputimg,
		const QList<ImageMarker> &ql_marker,const VISIOCYTELONG l_width,
		unsigned char *&p_strimg,VISIOCYTELONG *&sz_strimg);
bool q_celegans_restacking_xy(
		const unsigned char *p_inputimg,const VISIOCYTELONG *sz_inputimg,
		const QList<ImageMarker> &ql_marker,const VISIOCYTELONG l_width,
		unsigned char *&p_strimg,VISIOCYTELONG *&sz_strimg,
		vector< vector< vector< vector<VISIOCYTELONG> > > > &vec4d_mappingfield_str2ori);
bool q_celegans_restacking_xz(
		const unsigned char *p_inputimg,const VISIOCYTELONG *sz_inputimg,
		const QList<ImageMarker> &ql_marker,const VISIOCYTELONG l_width,
		unsigned char *&p_strimg,VISIOCYTELONG *&sz_strimg);
bool q_celegans_restacking_xz(
		const unsigned char *p_inputimg,const VISIOCYTELONG *sz_inputimg,
		const QList<ImageMarker> &ql_marker,const VISIOCYTELONG l_width,
		unsigned char *&p_strimg,VISIOCYTELONG *&sz_strimg,
		vector< vector< vector< vector<VISIOCYTELONG> > > > &vec4d_mappingfield_str2ori);

bool q_restacking_alongbcurve_xy(
		const unsigned char *p_inputimg,const VISIOCYTELONG *sz_inputimg,
        const double *pos_curve_x,const double *pos_curve_y,const double *alpha_curve,const VISIOCYTELONG length_curve,
		const VISIOCYTELONG l_width,
		unsigned char *&p_strimg, VISIOCYTELONG *&sz_strimg,
		vector< vector< vector< vector<VISIOCYTELONG> > > > &vec4d_mappingfield_str2ori);
bool q_restacking_alongbcurve_xz(
		const unsigned char *p_inputimg,const VISIOCYTELONG *sz_inputimg,
        const double *pos_curve_x,const double *pos_curve_y,const double *alpha_curve,const VISIOCYTELONG length_curve,
		const VISIOCYTELONG l_width,
		unsigned char *&p_strimg, VISIOCYTELONG *&sz_strimg,
		vector< vector< vector< vector<VISIOCYTELONG> > > > &vec4d_mappingfield_str2ori);

#endif
