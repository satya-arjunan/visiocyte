
//by Hanchuan Peng
//081111

#ifndef __CONVERT_TYPE2UINT8_H__
#define __CONVERT_TYPE2UINT8_H__

#include "visiocyte_basicdatatype.h"

bool convert_type2uint8_3dimg_1dpt(void * &img, VISIOCYTELONG * sz, int datatype);
template <class T> bool rescale_to_0_255_and_copy(T *inimg, VISIOCYTELONG datalen, double & minn, double & maxx, unsigned char *outimg);

template <class T> bool rescale_to_0_255_and_copy(T *inimg, VISIOCYTELONG datalen, double & minn, double & maxx, unsigned char *outimg)
{
	if (!inimg || !outimg || datalen<=0) return false;
	
	minn = maxx = inimg[0];
	for (VISIOCYTELONG i=0;i<datalen;i++)
	{
		T v = inimg[i];
		if (v<minn) minn=v;
		else if (v>maxx) maxx=v;
	}
	
	if (minn!=maxx)
	{
		double s = 255.0/(maxx-minn);
		
		for (VISIOCYTELONG i=0;i<datalen;i++)
		{
			outimg[i] = (unsigned char)(s*(inimg[i]-minn));
		}
	}
	else
	{
		for (VISIOCYTELONG i=0;i<datalen;i++)
		{
			outimg[i] = 0;
		}
	}

	return true;
}

#endif

