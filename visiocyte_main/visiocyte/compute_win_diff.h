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




//compute_win_diff.h
//by Yang Yu
//090621

#ifndef __COMPUTE_WIN_DIFF_H__
#define __COMPUTE_WIN_DIFF_H__

template <class T, class TF> bool compute_win3d_diff(T ***img3d, TF *** &diff3d, VISIOCYTELONG sx, VISIOCYTELONG sy, VISIOCYTELONG sz, VISIOCYTELONG x0, VISIOCYTELONG y0, VISIOCYTELONG z0, VISIOCYTELONG rx, VISIOCYTELONG ry, VISIOCYTELONG rz)
{
	if (!img3d || sx<=0 || sy<=0 || sz<=0 ||
		x0<0 || x0>=sx || y0<0 || y0>=sy || z0<0 || z0>=sz ||
		rx<0 || ry<0 || rz<0)
		return false;
	
	//get the boundary
	VISIOCYTELONG xb=x0-rx; if(xb<0) xb=0; else if (xb>=sx) xb=sx-1;
	VISIOCYTELONG xe=x0+rx; if(xe<0) xe=0; else if (xe>=sx) xe=sx-1;
	VISIOCYTELONG yb=y0-ry; if(yb<0) yb=0; else if (yb>=sy) yb=sy-1;
	VISIOCYTELONG ye=y0+ry; if(ye<0) ye=0; else if (ye>=sy) ye=sy-1;
	VISIOCYTELONG zb=z0-rz; if(zb<0) zb=0; else if (zb>=sz) zb=sz-1;
	VISIOCYTELONG ze=z0+rz; if(ze<0) ze=0; else if (ze>=sz) ze=sz-1;
	
	//compute diff along x, y, z & filling 0 out of bounder & mapping into 0-255
	VISIOCYTELONG sz_x = (2*rx+1), sz_y = (2*ry+1), sz_z = (2*rz+1); 
	TF mean_val_n = 0, mean_val_p = 0;
	
	//diff along x
	for (VISIOCYTELONG k=zb;k<=ze;k++)
	{
		for (VISIOCYTELONG j=yb;j<=ye;j++)
		{
			for (VISIOCYTELONG i=xb;i<=xe;i++)
			{
				VISIOCYTELONG x_f = (VISIOCYTELONG)(i+rx), x_b = (VISIOCYTELONG)(i-rx);
				
				if (x_b<xb) x_b=xb; 
				if (x_f>xe) x_f=xe;
				
				mean_val_n = 0; mean_val_p = 0; //initialization before statistics
				
				for(VISIOCYTELONG loop = x_b; loop < i; loop++)
					mean_val_n += img3d[k][j][loop];
				for(VISIOCYTELONG loop = i+1; loop <= x_f; loop++)
					mean_val_p += img3d[k][j][loop];
				
				TF tmpx = (mean_val_p - mean_val_n)/TF(2*rx); 

				//diff along y
				VISIOCYTELONG y_f = (VISIOCYTELONG)(j+ry), y_b = (VISIOCYTELONG)(j-ry);
				
				if (y_b<yb) y_b=yb; 
				if (y_f>ye) y_f=ye;
				
				mean_val_n = 0; mean_val_p = 0; //initialization before statistics
				
				for(VISIOCYTELONG loop = y_b; loop < j; loop++)
					mean_val_n += img3d[k][loop][i];
				for(VISIOCYTELONG loop = j+1; loop <= y_f; loop++)
					mean_val_p += img3d[k][loop][i];
				
				TF tmpy = (mean_val_p - mean_val_n)/TF(2*ry); 

				//diff along z
				VISIOCYTELONG z_f = (VISIOCYTELONG)(k+rz), z_b = (VISIOCYTELONG)(k-rz);
				
				if (z_b<zb) z_b=zb; 
				if (z_f>ze) z_f=ze;
				
				mean_val_n = 0; mean_val_p = 0; //initialization before statistics
				
				for(VISIOCYTELONG loop = z_b; loop < k; loop++)
					mean_val_n += img3d[loop][j][i];
				for(VISIOCYTELONG loop = k+1; loop <= z_f; loop++)
					mean_val_p += img3d[loop][j][i];
				TF tmpz = (mean_val_p - mean_val_n)/TF(2*rz);

				TF tmp = sqrt(tmpx*tmpx + tmpy*tmpy + tmpz*tmpz);

				diff3d[k][j][i] = tmp;
			}
		}
	}
	
	return true;
}


#endif

