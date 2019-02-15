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




#ifndef _NR_UTILS_H_
#define _NR_UTILS_H_

#include "../basic_c_fun/visiocyte_basicdatatype.h" //2010-05-20, PHC

static float sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

static double dsqrarg;
#define DSQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)

static double dmaxarg1,dmaxarg2;
#define DMAX(a,b) (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1) > (dmaxarg2) ?\
        (dmaxarg1) : (dmaxarg2))

static double dminarg1,dminarg2;
#define DMIN(a,b) (dminarg1=(a),dminarg2=(b),(dminarg1) < (dminarg2) ?\
        (dminarg1) : (dminarg2))

static float maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
        (maxarg1) : (maxarg2))

static float minarg1,minarg2;
#define FMIN(a,b) (minarg1=(a),minarg2=(b),(minarg1) < (minarg2) ?\
        (minarg1) : (minarg2))

static VISIOCYTELONG lmaxarg1,lmaxarg2;
#define LMAX(a,b) (lmaxarg1=(a),lmaxarg2=(b),(lmaxarg1) > (lmaxarg2) ?\
        (lmaxarg1) : (lmaxarg2))

static VISIOCYTELONG lminarg1,lminarg2;
#define LMIN(a,b) (lminarg1=(a),lminarg2=(b),(lminarg1) < (lminarg2) ?\
        (lminarg1) : (lminarg2))

static int imaxarg1,imaxarg2;
#define IMAX(a,b) (imaxarg1=(a),imaxarg2=(b),(imaxarg1) > (imaxarg2) ?\
        (imaxarg1) : (imaxarg2))

static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
        (iminarg1) : (iminarg2))

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

#if defined(__STDC__) || defined(ANSI) || defined(NRANSI) /* ANSI */

void nrerror(char error_text[]);
float *vector(VISIOCYTELONG nl, VISIOCYTELONG nh);
int *ivector(VISIOCYTELONG nl, VISIOCYTELONG nh);
unsigned char *cvector(VISIOCYTELONG nl, VISIOCYTELONG nh);
unsigned VISIOCYTELONG *lvector(VISIOCYTELONG nl, VISIOCYTELONG nh);
double *dvector(VISIOCYTELONG nl, VISIOCYTELONG nh);
float **matrix(VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
double **dmatrix(VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
int **imatrix(VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
float **submatrix(float **a, VISIOCYTELONG oldrl, VISIOCYTELONG oldrh, VISIOCYTELONG oldcl, VISIOCYTELONG oldch,
	VISIOCYTELONG newrl, VISIOCYTELONG newcl);
float **convert_matrix(float *a, VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
float ***f3tensor(VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch, VISIOCYTELONG ndl, VISIOCYTELONG ndh);
void free_vector(float *v, VISIOCYTELONG nl, VISIOCYTELONG nh);
void free_ivector(int *v, VISIOCYTELONG nl, VISIOCYTELONG nh);
void free_cvector(unsigned char *v, VISIOCYTELONG nl, VISIOCYTELONG nh);
void free_lvector(unsigned VISIOCYTELONG *v, VISIOCYTELONG nl, VISIOCYTELONG nh);
void free_dvector(double *v, VISIOCYTELONG nl, VISIOCYTELONG nh);
void free_matrix(float **m, VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
void free_dmatrix(double **m, VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
void free_imatrix(int **m, VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
void free_submatrix(float **b, VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
void free_convert_matrix(float **b, VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch);
void free_f3tensor(float ***t, VISIOCYTELONG nrl, VISIOCYTELONG nrh, VISIOCYTELONG ncl, VISIOCYTELONG nch,
	VISIOCYTELONG ndl, VISIOCYTELONG ndh);

#else /* ANSI */
/* traditional - K&R */

void nrerror();
float *vector();
float **matrix();
float **submatrix();
float **convert_matrix();
float ***f3tensor();
double *dvector();
double **dmatrix();
int *ivector();
int **imatrix();
unsigned char *cvector();
unsigned VISIOCYTELONG *lvector();
void free_vector();
void free_dvector();
void free_ivector();
void free_cvector();
void free_lvector();
void free_matrix();
void free_submatrix();
void free_convert_matrix();
void free_dmatrix();
void free_imatrix();
void free_f3tensor();

#endif /* ANSI */

#endif /* _NR_UTILS_H_ */
