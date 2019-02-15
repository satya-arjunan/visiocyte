/*
 * Copyright (c)2006-2010  Hanchuan Peng (Janelia Farm, Howard Hughes Medical Institute).  
 * All rights reserved.
 */


/************
                                            ********* LICENSE NOTICE ************

This folder contains all source codes for the VISIOCYTE project, which is subject to the following conditions if you want to use it. 

You will ***have to agree*** the following terms, *before* downloading/using/running/editing/changing any portion of codes in this package.

1. This package is free for non-profit research, but needs a special license for any commercial purpose. Please contact Hanchuan Peng for details.

2. You agree to appropriately cite this work in your related studies and publications.

Peng, H., Ruan, Z., Long, F., Simpson, J.H., and Myers, E.W. (2010) “VISIOCYTE enables real-time 3D visualization and quantitative analysis of large-scale biological image data sets,” Nature Biotechnology, Vol. 28, No. 4, pp. 348-353, DOI: 10.1038/nbt.1612. ( http://penglab.janelia.org/papersall/docpdf/2010_NBT_VISIOCYTE.pdf )

Peng, H, Ruan, Z., Atasoy, D., and Sternson, S. (2010) “Automatic reconstruction of 3D neuron structures using a graph-augmented deformable model,” Bioinformatics, Vol. 26, pp. i38-i46, 2010. ( http://penglab.janelia.org/papersall/docpdf/2010_Bioinfo_GD_ISMB2010.pdf )

3. This software is provided by the copyright holders (Hanchuan Peng), Howard Hughes Medical Institute, Janelia Farm Research Campus, and contributors "as is" and any express or implied warranties, including, but not limited to, any implied warranties of merchantability, non-infringement, or fitness for a particular purpose are disclaimed. In no event shall the copyright owner, Howard Hughes Medical Institute, Janelia Farm Research Campus, or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; reasonable royalties; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

4. Neither the name of the Howard Hughes Medical Institute, Janelia Farm Research Campus, nor Hanchuan Peng, may be used to endorse or promote products derived from this software without specific prior written permission.

*************/




// compute region properties: area, pixelIdxList, centroid
// F. Long
// 20081027

#ifndef __REGION_PROPS__
#define __REGION_PROPS__

#include "./local_basic_c_fun/volimg_proc.h"

class Props
{
public:

	VISIOCYTELONG area;
	VISIOCYTELONG *pixelIdxList;
	VISIOCYTELONG *centroid;

public:
	Props()
	{
		area = 0;
		pixelIdxList = 0;
		centroid = 0;
	}
		
	// 2d
	template <class T> VISIOCYTELONG getArea(T **img, int rgnidx, const VISIOCYTELONG *sz);
	template <class T> VISIOCYTELONG * getPixelIdxList(T **img, int rgnidx, const VISIOCYTELONG *sz);
	template <class T> VISIOCYTELONG * getCentroid(T **img, int rgnidx, const VISIOCYTELONG *sz);
	
	//3d
	template <class T> VISIOCYTELONG getArea(T ***img, int rgnidx, const VISIOCYTELONG *sz);
	template <class T> VISIOCYTELONG * getPixelIdxList(T ***img, int rgnidx, const VISIOCYTELONG *sz);
	template <class T> VISIOCYTELONG * getCentroid(T ***img, int rgnidx, const VISIOCYTELONG *sz);

	~Props()
	{
		if (pixelIdxList) {delete [] pixelIdxList; pixelIdxList = 0;}
		if (centroid) {delete [] centroid; centroid = 0;}		
	}
	
};

//Props::Props()
//{
//	area = 0;
//	pixelIdxList = 0;
//	centroid = 0;
//}

template <class T> VISIOCYTELONG Props::getArea(T **img, int rgnidx, const VISIOCYTELONG *sz)
{
	area = 0;
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (img[j][i] == rgnidx)
			area++;
	}
	
	return area;
}

template <class T> VISIOCYTELONG * Props::getPixelIdxList(T **img, int rgnidx, const VISIOCYTELONG *sz)
{
	Props:getArea(img, rgnidx, sz);
	pixelIdxList = new VISIOCYTELONG [area];
	
	int count = 0;
	
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (img[j][i] == rgnidx)
		{	count ++;
			pixelIdxList[count] = j*sz[0] + i;
		}
	}
	
	return pixelIdxList;
	
}

template <class T> VISIOCYTELONG * Props::getCentroid(T **img, int rgnidx, const VISIOCYTELONG *sz)
{	
	centroid = new VISIOCYTELONG [2];
	centroid[0] = 0;
	centroid[1] = 0;
	
	int count = 0;
	
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (img[j][i] == rgnidx)
		{	count ++;
			centroid[0] = centroid[0] + i;
			centroid[1] = centroid[1] + j;
		}
	}
	centroid[0] = centroid[0]/count;
	centroid[1] = centroid[1]/count;
	
	return centroid;	 	
}

template <class T> VISIOCYTELONG Props::getArea(T ***img, int rgnidx, const VISIOCYTELONG *sz)
{
	area = 0;
	for (int k=0; k<sz[2]; k++)
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (img[k][j][i] == rgnidx)
			area++;
	}
	
	return area;
}

template <class T> VISIOCYTELONG * Props::getPixelIdxList(T ***img, int rgnidx, const VISIOCYTELONG *sz)
{
	Props:getArea(img, rgnidx, sz);
	pixelIdxList = new VISIOCYTELONG [area];
	
	int count = 0;
	
	for (int k=0; k<sz[2]; k++)
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (img[k][j][i] == rgnidx)
		{	count ++;
			pixelIdxList[count] = k*sz[1]*sz[0] + j*sz[0] + i;
		}
	}

	return pixelIdxList;

}

template <class T> VISIOCYTELONG * Props::getCentroid(T ***img, int rgnidx, const VISIOCYTELONG *sz)
{
	centroid = new VISIOCYTELONG [3];
	
	for (int i=0; i<2; i++)
		centroid[i] = 0;
	
	int count = 0;
	
	for (int k=0; k<sz[2]; k++)
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (img[k][j][i] == rgnidx)
		{	count ++;
			centroid[0] = centroid[0] + i;
			centroid[1] = centroid[1] + j;
			centroid[2] = centroid[2] + k;
			
		}
	}
	
	for (int i=0; i<2; i++)
		centroid[i] = centroid[i]/count;
		
	return centroid;

}


//Props::~Props()
//{
//	if (pixelIdxList) {delete [] pixelIdxList; pixelIdxList = 0;}
//	if (centroid) {delete [] centroid; centroid = 0;}
//	
//}

template <class T> Props *  regionProps(T **labelimage, const VISIOCYTELONG *sz)
{
	Props * rgnProps;
	T maxval;
		
	img2d_max(maxval, labelimage, sz[0], sz[1]);
	
	rgnProps = new Props [maxval];
	
	// find all label values, indicaated by tag
	unsigned char *tag = new unsigned char [maxval];
	for (int cnt =0; cnt<maxval; cnt++)
	{
		tag[cnt] = 0;
	}
	
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (labelimage[j][i]>0)
			tag[labelimage[j][i]] = 1;
	}	
	
	// process each region
	for (int cnt =0; cnt<maxval; cnt++)
	{
		if (tag[cnt]>0)
		{
			rgnProps[cnt].area = getArea(labelimage, cnt, sz);
			rgnProps[cnt].pixelIdxList = getPixelIdxList(labelimage, cnt, sz);
			rgnProps[cnt].centroid = getCentroid(labelimage, cnt, sz);
			
		}
	}
	
	return rgnProps;
}

template <class T> Props * regionProps(T ***labelimage, const VISIOCYTELONG *sz)
{
	Props * rgnProps;
	T maxval;
	
	vol3d_max(maxval, labelimage, sz[0], sz[1], sz[2]);
		
	rgnProps = new Props [maxval];
	
	// find all label values, indicaated by tag
	unsigned char *tag = new unsigned char [maxval];
	
	for (int cnt =0; cnt<maxval; cnt++)
	{
		tag[cnt] = 0;
	}

	for (int k=0; k<sz[2]; k++)		
	for (int j=0; j<sz[1]; j++)
	for (int i=0; i<sz[0]; i++)
	{
		if (labelimage[k][j][i]>0)
			tag[labelimage[k][j][i]] = 1;
	}	
	
	// process each region
	for (int cnt =0; cnt<maxval; cnt++)
	{
		if (tag[cnt]>0)
		{
			rgnProps[cnt].area = getArea(labelimage, cnt, sz);
			rgnProps[cnt].pixelIdxList = getPixelIdxList(labelimage, cnt, sz);
			rgnProps[cnt].centroid = getCentroid(labelimage, cnt, sz);

			
		}
	}
	
	return rgnProps;
}


#endif 

