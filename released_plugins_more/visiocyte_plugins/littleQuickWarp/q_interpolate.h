//q_interpolate.h
//
// by Lei Qu
//2012-07-08

#ifndef __Q_INTERPOLATE_H__
#define __Q_INTERPOLATE_H__

#include "../../basic_c_fun/visiocyte_basicdatatype.h"
#include "../../basic_c_fun/basic_surf_objs.h"
#include "jba_mainfunc.h"

Vol3DSimple <MYFLOAT_JBA> * linearinterp_regularmesh_3d(VISIOCYTELONG sz0, VISIOCYTELONG sz1, VISIOCYTELONG sz2, Vol3DSimple <MYFLOAT_JBA> * df_regular_grid);

bool interpolate_coord_linear(MYFLOAT_JBA * interpolatedVal, Coord3D_JBA *c, VISIOCYTELONG numCoord,
                       MYFLOAT_JBA *** templateVol3d, VISIOCYTELONG tsz0, VISIOCYTELONG tsz1, VISIOCYTELONG tsz2,
  		               VISIOCYTELONG tlow0, VISIOCYTELONG tup0, VISIOCYTELONG tlow1, VISIOCYTELONG tup1, VISIOCYTELONG tlow2, VISIOCYTELONG tup2);

#endif

