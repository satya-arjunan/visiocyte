// downsample a 3D volume by a factor along each dimension
// F. Long
// 20080507
// 20080830 update

#ifndef __DOWN_SAMPLE3D__
#define __DOWN_SAMPLE3D__

#include "volimg_proc.h"
#include "img_definition.h"
#include "stackutil.h"

#include "FL_defType.h"

// tag indicate whether pixels in the downsampled image should be the average of neighboring pixels in the original image (tag = 0)
// or sample pixlels in the original image directly (tag other than 0 )

template <class T> bool downsample3dvol(T ***outdata, T *** indata, VISIOCYTELONG *szin, double *dfactor, unsigned char tag)
{	
	
	VISIOCYTELONG szout[3];
	for (VISIOCYTELONG i=0; i<3; i++)
	{
		szout[i] = (VISIOCYTELONG)(floor(double(szin[i]) / double(dfactor[i])));
		if (szout[0] <= 0)
		{
			fprintf(stderr, "The dfactor is not properly set, -- the downsampled size is too small. Do nothing. [%s][%d].\n", __FILE__, __LINE__);
			return false;
		}
	}
	
	if (tag == 0)
	{
		for (VISIOCYTELONG k=0;k<szout[2];k++)
		{
            VISIOCYTELONG k2low=(VISIOCYTELONG)(floor(k*dfactor[2])), k2high=(VISIOCYTELONG)(floor((k+1)*dfactor[2]-1));
			if (k2high>szin[2]) k2high = szin[2];
			VISIOCYTELONG kw = k2high - k2low + 1;
			
			for (VISIOCYTELONG j=0;j<szout[1];j++)
			{
                VISIOCYTELONG j2low=(VISIOCYTELONG)(floor(j*dfactor[1])), j2high=(VISIOCYTELONG)(floor((j+1)*dfactor[1]-1));
				if (j2high>szin[1]) j2high = szin[1];
				VISIOCYTELONG jw = j2high - j2low + 1;
				
				for (VISIOCYTELONG i=0;i<szout[0];i++)
				{
                    VISIOCYTELONG i2low=(VISIOCYTELONG)(floor(i*dfactor[0])), i2high=(VISIOCYTELONG)(floor((i+1)*dfactor[0]-1));
					if (i2high>szin[0]) i2high = szin[0];
					VISIOCYTELONG iw = i2high - i2low + 1;
					
					double cubevolume = double(kw) * jw * iw;
					//cout<<cubevolume <<" ";
					
					double s=0.0;
					for (VISIOCYTELONG k1=k2low;k1<=k2high;k1++)
					{
						for (VISIOCYTELONG j1=j2low;j1<=j2high;j1++)
						{
							for (VISIOCYTELONG i1=i2low;i1<=i2high;i1++)
							{
								s += indata[k1][j1][i1];
							}
						}
					}
					
					outdata[k][j][i] = (T)(s/cubevolume);
	//				if (outdata[k][j][i]>0)
	//					printf("%f\n", s/cubevolume);
				}
			}
		}
	}
	else
	{
		for (VISIOCYTELONG k=0;k<szout[2]; k++)
		{
            VISIOCYTELONG k2low=(VISIOCYTELONG)(floor(k*dfactor[2])), k2high=(VISIOCYTELONG)(floor((k+1)*dfactor[2]-1));
			if (k2high>szin[2]) k2high = szin[2];
			VISIOCYTELONG kw = k2high - k2low + 1;
			
			for (VISIOCYTELONG j=0;j<szout[1];j++)
			{
                VISIOCYTELONG j2low=(VISIOCYTELONG)(floor(j*dfactor[1])), j2high=(VISIOCYTELONG)(floor((j+1)*dfactor[1]-1));
				if (j2high>szin[1]) j2high = szin[1];
				VISIOCYTELONG jw = j2high - j2low + 1;
				
				for (VISIOCYTELONG i=0;i<szout[0];i++)
				{
                    VISIOCYTELONG i2low=(VISIOCYTELONG)(floor(i*dfactor[0])), i2high=(VISIOCYTELONG)(floor((i+1)*dfactor[0]-1));
					if (i2high>szin[0]) i2high = szin[0];
					VISIOCYTELONG iw = i2high - i2low + 1;
					
					outdata[k][j][i] = indata[int((k2low+k2high)/2)][int((j2low+j2high)/2)][int((i2low+i2high)/2)]; 
					
				}
			}
		}
	}
	
	return true;
}

// 20110128 add this function, note that the implmentation of tag != 1 is different from those of the other two functions
// tag == 0: using averaging to generate new pixel value
// tag == 1: using subsampling to generate new pixel value
template <class T> bool downsample3dvol(T *&outdata, T *indata, VISIOCYTELONG *szout, VISIOCYTELONG *szin, double *dfactor, unsigned char tag)
{	
	

//	VISIOCYTELONG szout[3];
	for (VISIOCYTELONG i=0; i<3; i++)
	{
		szout[i] = (VISIOCYTELONG)(floor(double(szin[i]) / double(dfactor[i])));
		if (szout[0] <= 0)
		{
			fprintf(stderr, "The dfactor is not properly set, -- the downsampled size is too small. Do nothing. [%s][%d].\n", __FILE__, __LINE__);
			return false;
		}
	}
	
	outdata = new T [szout[0]*szout[1]*szout[2]];
	
	long szout_01 = szout[0]*szout[1];
	long szin_01 = szin[0]*szin[1];
	
	if (tag == 0)
	{
		for (VISIOCYTELONG k=0;k<szout[2];k++)
		{
			long tt1 = k*szout_01;
			
            VISIOCYTELONG k2low=(VISIOCYTELONG)(floor(k*dfactor[2])), k2high=(VISIOCYTELONG)(floor((k+1)*dfactor[2]-1));
			if (k2high>szin[2]) k2high = szin[2];
			VISIOCYTELONG kw = k2high - k2low + 1;
			
			for (VISIOCYTELONG j=0;j<szout[1];j++)
			{
				long tt2 = j*szout[0];
				
                VISIOCYTELONG j2low=(VISIOCYTELONG)(floor(j*dfactor[1])), j2high=(VISIOCYTELONG)(floor((j+1)*dfactor[1]-1));
				if (j2high>szin[1]) j2high = szin[1];
				VISIOCYTELONG jw = j2high - j2low + 1;
				
				for (VISIOCYTELONG i=0;i<szout[0];i++)
				{
					long idx_out = tt1 + tt2 + i;
					
                    VISIOCYTELONG i2low=(VISIOCYTELONG)(floor(i*dfactor[0])), i2high=(VISIOCYTELONG)(floor((i+1)*dfactor[0]-1));
					if (i2high>szin[0]) i2high = szin[0];
					VISIOCYTELONG iw = i2high - i2low + 1;
					
					double cubevolume = double(kw) * jw * iw;
					//cout<<cubevolume <<" ";
					
//printf("%ld,%ld, %ld\n", i,j,k);
					double s=0.0;
					for (VISIOCYTELONG k1=k2low;k1<=k2high;k1++)
					{
						long tmp1 = k1*szin_01;
						
						for (VISIOCYTELONG j1=j2low;j1<=j2high;j1++)
						{
							long tmp2 = j1*szin[0];
							
							for (VISIOCYTELONG i1=i2low;i1<=i2high;i1++)
							{
								long idx_in = tmp1+ tmp2 + i1;
								s += indata[idx_in];
//								if (indata[idx_in]>0)
//									printf("%d, %d, %d, %d, %d, %d, %d\n", k, j, i, k1, j1, i1, indata[idx_in]);
							}
						}
					}
					
					outdata[idx_out] = (T)(s/cubevolume);
//					if (outdata[idx_out]>0)
//						printf("*******%f\n", s/cubevolume);
				}
			}
		}
	}
	else
	{
		for (VISIOCYTELONG k=0;k<szout[2]; k++)
		{
			
			long tt1 = k*szout_01;

			VISIOCYTELONG k2=VISIOCYTELONG(floor(k*dfactor[2])); //20110128
			
//			VISIOCYTELONG k2low=VISIOCYTELONG(floor(k*dfactor[2])), k2high=VISIOCYTELONG(floor((k+1)*dfactor[2]-1));
//			if (k2high>szin[2]) k2high = szin[2];
//			VISIOCYTELONG kw = k2high - k2low + 1;
			
			for (VISIOCYTELONG j=0;j<szout[1];j++)
			{
				
				long tt2 = j*szout[0];
				
				VISIOCYTELONG j2=VISIOCYTELONG(floor(j*dfactor[1])); //20110128
				
//				VISIOCYTELONG j2low=VISIOCYTELONG(floor(j*dfactor[1])), j2high=VISIOCYTELONG(floor((j+1)*dfactor[1]-1));
//				if (j2high>szin[1]) j2high = szin[1];
//				VISIOCYTELONG jw = j2high - j2low + 1;
				
				for (VISIOCYTELONG i=0;i<szout[0];i++)
				{
					
					long idx_out = tt1 + tt2 + i;
					
					VISIOCYTELONG i2=VISIOCYTELONG(floor(i*dfactor[0]));

					long idx_in = k2*szin_01 + j2*szin[0] + i2;
					
//					VISIOCYTELONG i2low=VISIOCYTELONG(floor(i*dfactor[0])), i2high=VISIOCYTELONG(floor((i+1)*dfactor[0]-1));
//					if (i2high>szin[0]) i2high = szin[0];
//					VISIOCYTELONG iw = i2high - i2low + 1;
					
//					outdata[k][j][i] = indata[int((k2low+k2high)/2)][int((j2low+j2high)/2)][int((i2low+i2high)/2)]; 

					outdata[idx_out] = indata[idx_in];
				}
			}
		}
	}
	
	return true;
}


template <class T> bool downsample3dvol(Vol3DSimple<T> * &outimg, Vol3DSimple<T> * inimg, double *dfactor, unsigned char tag)
{
	// check arguments
	if (outimg || !inimg || !inimg->valid())
	{
		fprintf(stderr, "The outimg pointer is not initialized as NULL or get NULL input image pointer [%s][%d].\n", __FILE__, __LINE__);
		return false;
	}
	
	for (VISIOCYTELONG i=0; i<3; i++)
	{
		if (int(dfactor[i])<1 || int(dfactor[i]) >32)
		{
			fprintf(stderr, "The downsampling factor must be >=1 and <= 32 [%s][%d] dfactor=%.2f.\n", __FILE__, __LINE__, dfactor);
			return false;
		}
	}
	
	VISIOCYTELONG sz[3];
	sz[0] = inimg->sz0();
	sz[1] = inimg->sz1();
	sz[2] = inimg->sz2();
	
	VISIOCYTELONG cur_sz[3];
	for (VISIOCYTELONG i=0; i<3; i++)
	{
		cur_sz[i] = (VISIOCYTELONG)(floor(double(sz[i]) / double(dfactor[i])));
		if (cur_sz[0] <= 0)
		{
			fprintf(stderr, "The dfactor is not properly set, -- the downsampled size is too small. Do nothing. [%s][%d].\n", __FILE__, __LINE__);
			return false;
		}
	}
	
	outimg = new Vol3DSimple<T> (cur_sz[0], cur_sz[1], cur_sz[2]);
	
	if (!outimg)
	{
		fprintf(stderr, "Fail to allocate memory. [%s][%d].\n", __FILE__, __LINE__);
		return false;
	}
	
	T *** out_tmp3d = outimg->getData3dHandle();
	T *** in_tmp3d = inimg->getData3dHandle();
	
	downsample3dvol(out_tmp3d, in_tmp3d, sz, dfactor, tag);
	
	return true;
}




#endif
