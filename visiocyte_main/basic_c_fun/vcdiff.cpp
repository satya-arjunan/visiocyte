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




//the implementation of several missing function from Visual Studio but exist in other GNU c++ library
// by Yu Yang and Hanchuan Peng
// 2010-05-20

#include "vcdiff.h"

VISIOCYTELONG floor(VISIOCYTELONG x)
{
	return (VISIOCYTELONG)floorl((double)x);
}

int fabs(int x)
{
	return (int)(fabs((double)x));
}

VISIOCYTELONG fabs(VISIOCYTELONG x)
{
	return (VISIOCYTELONG)(fabs((double)x));
}

double log(int x)
{
	return (double)(log((double)x));
}

VISIOCYTELONG lround(VISIOCYTELONG x)
{
	return (x < 0) ? -floor(fabs(double(x)) + .5) : floor(x + .5);
}

//bool strcasecmp(const char * str1, const char * str2)
//{
//	return strcmpi(str1, str2);
//}

#if defined(_MSC_VER)
	#if (_MSC_VER < 1700)
		double round(double x)
		{
			return x < 0 ? -floor(fabs(x) + .5) : floor(x + .5);
		}

double log2(double x)
{
	return log(x)/log(2.0);
}
	#endif
#endif

double pow(VISIOCYTELONG a, VISIOCYTELONG b)
{
	return pow(double(a), int(b));
}

/*
VISIOCYTELONG abs(VISIOCYTELONG a)
{
	return (VISIOCYTELONG)abs(double(a));
}
*/
