#ifndef __MAPVIEW_H__
#define __MAPVIEW_H__

#include <string>
#include "../basic_c_fun/stackutil.h"
using namespace std;

struct ImageMapView
{
	VISIOCYTELONG L, M, N;                   // the block number in X, Y, Z dimension
	VISIOCYTELONG l, m, n;                   // the block size
	string prefix;

	ImageMapView();
	void setPara(string _prefix, VISIOCYTELONG _L, VISIOCYTELONG _M, VISIOCYTELONG _N, VISIOCYTELONG _l, VISIOCYTELONG _m, VISIOCYTELONG _n);
	void getImage(VISIOCYTELONG level, unsigned char * & outimg1d, VISIOCYTELONG x0, VISIOCYTELONG y0, VISIOCYTELONG z0, VISIOCYTELONG sz0, VISIOCYTELONG sz1, VISIOCYTELONG sz2);

	void getImageSize(VISIOCYTELONG level, VISIOCYTELONG & ts0, VISIOCYTELONG & ts1, VISIOCYTELONG & ts2, VISIOCYTELONG &bs0, VISIOCYTELONG &bs1, VISIOCYTELONG &bs2);

};

#endif
