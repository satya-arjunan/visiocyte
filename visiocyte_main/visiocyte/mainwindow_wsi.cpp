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

/****************************************************************************
 ***
	by Yang Yu
	May 3, 2011

 **
 ****************************************************************************/

#include <QtGui>

#include "mainwindow.h"

#include "../basic_c_fun/basic_surf_objs.h"
#include "../plugin_loader/visiocyte_plugin_loader.h"

#include "visiocyte_core.h"
#include "../3drenderer/visiocyter_mainwindow.h"

#include "../3drenderer/visiocyter_glwidget.h"

#ifdef __VISIOCYTEWSDEVELOP__

bool is3DviewerFunc(char *funcname)
{
	return ( (string(funcname) == "visiocyteopenfile3dwrot") ||
			(string(funcname) == "visiocyteopenfile3dwzoom") ||
			(string(funcname) == "visiocyteopenfile3dwshift") ||
			(string(funcname) == "visiocytevolumeColormapDialog") ||
			(string(funcname) == "visiocytesurfaceDialogHide") ||
			(string(funcname) == "visiocyteabsoluteRotPose") ||
			(string(funcname) == "visiocyteresetZoomShift") ||
			(string(funcname) == "visiocytesetBackgroundColor") ||
			(string(funcname) == "visiocytesetBright") ||
			(string(funcname) == "visiocytetoggleCellName") ||
			(string(funcname) == "visiocytetoggleMarkerName") ||
			(string(funcname) == "visiocytecreateSurfCurrentR") ||
			(string(funcname) == "visiocytecreateSurfCurrentG") ||
			(string(funcname) == "visiocytecreateSurfCurrentB") ||
			(string(funcname) == "visiocyteloadObjectListFromFile") ||
			(string(funcname) == "visiocyteloadObjectFromFile") ||
			(string(funcname) == "visiocytesaveSurfFile") ||
			(string(funcname) == "visiocytetogglePolygonMode") ||
			(string(funcname) == "visiocytetoggleLineType") ||
			(string(funcname) == "visiocytetoggleObjShader") ||
			(string(funcname) == "visiocytechangeLineOption") ||
			(string(funcname) == "visiocytechangeVolShadingOption") ||
			(string(funcname) == "visiocytechangeObjShadingOption") ||
			(string(funcname) == "visiocytetoggleTexFilter") ||
			(string(funcname) == "visiocytetoggleTex2D3D") ||
			(string(funcname) == "visiocytetoggleTexCompression") ||
			(string(funcname) == "visiocytetoggleTexStream") ||
			(string(funcname) == "visiocytetoggleShader") ||
			(string(funcname) == "visiocyteshowGLinfo") ||
			(string(funcname) == "visiocyteupdateWithTriView") ||
			(string(funcname) == "visiocyteupdateImageData") ||
			(string(funcname) == "visiocytereloadData") ||
			(string(funcname) == "visiocytecancelSelect") ||
			(string(funcname) == "visiocytesetThickness") ||
			(string(funcname) == "visiocytesetVolumeTimePoint") ||
			(string(funcname) == "visiocytesetCSTransparent") ||
			(string(funcname) == "visiocytesetCurChannel") ||
			(string(funcname) == "visiocytesurfaceSelectDialog") ||
			(string(funcname) == "visiocytesurfaceSelectTab") ||
			(string(funcname) == "visiocytesetFrontCut") ||
			(string(funcname) == "visiocytesetShowMarkers") ||
			(string(funcname) == "visiocytesetShowSurfObjects") ||
			(string(funcname) == "visiocytesetMarkerSize") ||
			(string(funcname) == "visiocyteincVolumeTimePoint") ||
			(string(funcname) == "visiocytesetRenderMode_Mip") ||
			(string(funcname) == "visiocytesetRenderMode_Alpha") ||
			(string(funcname) == "visiocytesetRenderMode_Cs3d") ||
			(string(funcname) == "visiocytesetChannelR") ||
			(string(funcname) == "visiocytesetChannelG") ||
			(string(funcname) == "visiocytesetChannelB") ||
			(string(funcname) == "visiocytesetVolCompress") ||
			(string(funcname) == "visiocyteresetRotation") ||
			(string(funcname) == "visiocyteenableFrontSlice") ||
			(string(funcname) == "visiocyteenableXSlice") ||
			(string(funcname) == "visiocyteenableYSlice") ||
			(string(funcname) == "visiocyteenableZSlice") ||
			(string(funcname) == "visiocytesetXCutLock") ||
			(string(funcname) == "visiocytesetYCutLock") ||
			(string(funcname) == "visiocytesetZCutLock") ||
			(string(funcname) == "visiocyteenableShowAxes") ||
			(string(funcname) == "visiocyteenableShowBoundingBox") ||
			(string(funcname) == "visiocyteenableOrthoView") ||
			(string(funcname) == "visiocyteenableMarkerLabel") ||
			(string(funcname) == "visiocyteenableSurfStretch") ||
			(string(funcname) == "visiocyteannotationDialog") ||
			(string(funcname) == "visiocytemodelRotation") ||
			(string(funcname) == "visiocyteviewRotation") ||
			(string(funcname) == "visiocytesetCut0") ||
			(string(funcname) == "visiocytesetCut1") ||
			(string(funcname) == "visiocytesetCS") ||
			(string(funcname) == "visiocytesetClip0") ||
			(string(funcname) == "visiocytesetClip1") ||
			(string(funcname) == "visiocytelookAlong") ||
            (string(funcname) == "visiocyteget3dvinfo") );

}

// template function
void MainWindow::switch3dviewercontrol(VisiocyteR_MainWindow *existing_3dviewer)
{
	if(string(pSoapPara->str_func) == "visiocyteopenfile3dwrot")
	{			
		existing_3dviewer->getGLWidget()->doAbsoluteRot(pSoapPara->visiocytemsgxyzf->x, pSoapPara->visiocytemsgxyzf->y, pSoapPara->visiocytemsgxyzf->z);
	}
	else if(string(pSoapPara->str_func) == "visiocyteopenfile3dwzoom")
	{
		existing_3dviewer->getGLWidget()->setZoom(pSoapPara->visiocytemsgf->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteopenfile3dwshift")
	{
		existing_3dviewer->getGLWidget()->setXShift(pSoapPara->visiocytemsgxyzf->x);
		existing_3dviewer->getGLWidget()->setYShift(pSoapPara->visiocytemsgxyzf->y);
		existing_3dviewer->getGLWidget()->setZShift(pSoapPara->visiocytemsgxyzf->z);
	}
	else if(string(pSoapPara->str_func) == "visiocytevolumeColormapDialog")
	{
		existing_3dviewer->getGLWidget()->volumeColormapDialog();
	}
	else if(string(pSoapPara->str_func) == "visiocytesurfaceDialogHide")
	{
		existing_3dviewer->getGLWidget()->surfaceDialogHide();
	}
	else if(string(pSoapPara->str_func) == "visiocyteabsoluteRotPose")
	{
		existing_3dviewer->getGLWidget()->absoluteRotPose();
	}
	else if(string(pSoapPara->str_func) == "visiocyteresetZoomShift")
	{
		existing_3dviewer->getGLWidget()->resetZoomShift();
	}
	else if(string(pSoapPara->str_func) == "visiocytesetBackgroundColor")
	{
		existing_3dviewer->getGLWidget()->setBackgroundColor();
	}
	else if(string(pSoapPara->str_func) == "visiocytesetBright")
	{
		existing_3dviewer->getGLWidget()->setBright();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleCellName")
	{
		existing_3dviewer->getGLWidget()->toggleCellName();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleMarkerName")
	{
		existing_3dviewer->getGLWidget()->toggleMarkerName();
	}
	else if(string(pSoapPara->str_func) == "visiocytecreateSurfCurrentR")
	{
		existing_3dviewer->getGLWidget()->createSurfCurrentR();
	}
	else if(string(pSoapPara->str_func) == "visiocytecreateSurfCurrentG")
	{
		existing_3dviewer->getGLWidget()->createSurfCurrentG();
	}
	else if(string(pSoapPara->str_func) == "visiocytecreateSurfCurrentB")
	{
		existing_3dviewer->getGLWidget()->createSurfCurrentB();
	}
	else if(string(pSoapPara->str_func) == "visiocyteloadObjectListFromFile")
	{
		existing_3dviewer->getGLWidget()->loadObjectListFromFile();
	}
	else if(string(pSoapPara->str_func) == "visiocyteloadObjectFromFile")
	{
		existing_3dviewer->getGLWidget()->loadObjectFromFile();
	}
	else if(string(pSoapPara->str_func) == "visiocytesaveSurfFile")
	{
		existing_3dviewer->getGLWidget()->saveSurfFile();
	}
	else if(string(pSoapPara->str_func) == "visiocytetogglePolygonMode")
	{
		existing_3dviewer->getGLWidget()->togglePolygonMode();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleLineType")
	{
		existing_3dviewer->getGLWidget()->toggleLineType();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleObjShader")
	{
		existing_3dviewer->getGLWidget()->toggleObjShader();
	}
	else if(string(pSoapPara->str_func) == "visiocytechangeLineOption")
	{
		existing_3dviewer->getGLWidget()->changeLineOption();
	}
	else if(string(pSoapPara->str_func) == "visiocytechangeVolShadingOption")
	{
		existing_3dviewer->getGLWidget()->changeVolShadingOption();
	}
	else if(string(pSoapPara->str_func) == "visiocytechangeObjShadingOption")
	{
		existing_3dviewer->getGLWidget()->changeObjShadingOption();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleTexFilter")
	{
		existing_3dviewer->getGLWidget()->toggleTexFilter();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleTex2D3D")
	{
		existing_3dviewer->getGLWidget()->toggleTex2D3D();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleTexCompression")
	{
		existing_3dviewer->getGLWidget()->toggleTexCompression();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleTexStream")
	{
		existing_3dviewer->getGLWidget()->toggleTexStream();
	}
	else if(string(pSoapPara->str_func) == "visiocytetoggleShader")
	{
		existing_3dviewer->getGLWidget()->toggleShader();
	}
	else if(string(pSoapPara->str_func) == "visiocyteshowGLinfo")
	{
		existing_3dviewer->getGLWidget()->showGLinfo();
	}
	else if(string(pSoapPara->str_func) == "visiocyteupdateWithTriView")
	{
		existing_3dviewer->getGLWidget()->updateWithTriView();
	}
	else if(string(pSoapPara->str_func) == "visiocyteupdateImageData")
	{
		existing_3dviewer->getGLWidget()->updateImageData();
	}
	else if(string(pSoapPara->str_func) == "visiocytereloadData")
	{
		existing_3dviewer->getGLWidget()->reloadData();
	}
	else if(string(pSoapPara->str_func) == "visiocytecancelSelect")
	{
		existing_3dviewer->getGLWidget()->cancelSelect();
	}
	else if(string(pSoapPara->str_func) == "visiocytesetThickness")
	{
		existing_3dviewer->getGLWidget()->setThickness(pSoapPara->visiocytemsgd->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetVolumeTimePoint")
	{
		existing_3dviewer->getGLWidget()->setVolumeTimePoint(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetCSTransparent")
	{
		existing_3dviewer->getGLWidget()->setCSTransparent(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetCurChannel")
	{
		existing_3dviewer->getGLWidget()->setCurChannel(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesurfaceSelectDialog")
	{
		existing_3dviewer->getGLWidget()->surfaceSelectDialog(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesurfaceSelectTab")
	{
		existing_3dviewer->getGLWidget()->surfaceSelectTab(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetFrontCut")
	{
		existing_3dviewer->getGLWidget()->setFrontCut(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetShowMarkers")
	{
		existing_3dviewer->getGLWidget()->setShowMarkers(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetShowSurfObjects")
	{
		existing_3dviewer->getGLWidget()->setShowSurfObjects(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetMarkerSize")
	{
		existing_3dviewer->getGLWidget()->setMarkerSize(pSoapPara->visiocytemsgi->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteincVolumeTimePoint")
	{
		existing_3dviewer->getGLWidget()->incVolumeTimePoint(pSoapPara->visiocytemsgf->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetRenderMode_Mip")
	{
		existing_3dviewer->getGLWidget()->setRenderMode_Mip(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetRenderMode_Alpha")
	{
		existing_3dviewer->getGLWidget()->setRenderMode_Alpha(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetRenderMode_Cs3d")
	{
		existing_3dviewer->getGLWidget()->setRenderMode_Cs3d(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetChannelR")
	{
		existing_3dviewer->getGLWidget()->setChannelR(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetChannelG")
	{
		existing_3dviewer->getGLWidget()->setChannelG(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetChannelB")
	{
		existing_3dviewer->getGLWidget()->setChannelB(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetVolCompress")
	{
		existing_3dviewer->getGLWidget()->setVolCompress(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteresetRotation")
	{
		existing_3dviewer->getGLWidget()->resetRotation(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableFrontSlice")
	{
		existing_3dviewer->getGLWidget()->enableFrontSlice(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableXSlice")
	{
		existing_3dviewer->getGLWidget()->enableXSlice(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableYSlice")
	{
		existing_3dviewer->getGLWidget()->enableYSlice(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableZSlice")
	{
		existing_3dviewer->getGLWidget()->enableZSlice(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetXCutLock")
	{
		existing_3dviewer->getGLWidget()->setXCutLock(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetYCutLock")
	{
		existing_3dviewer->getGLWidget()->setYCutLock(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetZCutLock")
	{
		existing_3dviewer->getGLWidget()->setZCutLock(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableShowAxes")
	{
		existing_3dviewer->getGLWidget()->enableShowAxes(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableShowBoundingBox")
	{
		existing_3dviewer->getGLWidget()->enableShowBoundingBox(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableOrthoView")
	{
		existing_3dviewer->getGLWidget()->enableOrthoView(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableMarkerLabel")
	{
		existing_3dviewer->getGLWidget()->enableMarkerLabel(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteenableSurfStretch")
	{
		existing_3dviewer->getGLWidget()->enableSurfStretch(pSoapPara->visiocytemsgb->s);
	}
	else if(string(pSoapPara->str_func) == "visiocyteannotationDialog")
	{
		existing_3dviewer->getGLWidget()->annotationDialog(pSoapPara->visiocytemsgad->dataClass, pSoapPara->visiocytemsgad->surfaceType, pSoapPara->visiocytemsgad->index);
	}
	else if(string(pSoapPara->str_func) == "visiocytemodelRotation")
	{
		existing_3dviewer->getGLWidget()->modelRotation(pSoapPara->visiocytemsgxyzi->x, pSoapPara->visiocytemsgxyzi->y, pSoapPara->visiocytemsgxyzi->z);
	}
	else if(string(pSoapPara->str_func) == "visiocyteviewRotation")
	{
		existing_3dviewer->getGLWidget()->viewRotation(pSoapPara->visiocytemsgxyzi->x, pSoapPara->visiocytemsgxyzi->y, pSoapPara->visiocytemsgxyzi->z);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetCut0")
	{
		existing_3dviewer->getGLWidget()->setXCut0(pSoapPara->visiocytemsgxyzi->x);
		existing_3dviewer->getGLWidget()->setYCut0(pSoapPara->visiocytemsgxyzi->y);
		existing_3dviewer->getGLWidget()->setZCut0(pSoapPara->visiocytemsgxyzi->z);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetCut1")
	{
		existing_3dviewer->getGLWidget()->setXCut1(pSoapPara->visiocytemsgxyzi->x);
		existing_3dviewer->getGLWidget()->setYCut1(pSoapPara->visiocytemsgxyzi->y);
		existing_3dviewer->getGLWidget()->setZCut1(pSoapPara->visiocytemsgxyzi->z);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetCS")
	{
		existing_3dviewer->getGLWidget()->setXCS(pSoapPara->visiocytemsgxyzi->x);
		existing_3dviewer->getGLWidget()->setYCS(pSoapPara->visiocytemsgxyzi->y);
		existing_3dviewer->getGLWidget()->setZCS(pSoapPara->visiocytemsgxyzi->z);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetClip0")
	{
		existing_3dviewer->getGLWidget()->setXClip0(pSoapPara->visiocytemsgxyzi->x);
		existing_3dviewer->getGLWidget()->setYClip0(pSoapPara->visiocytemsgxyzi->y);
		existing_3dviewer->getGLWidget()->setZClip0(pSoapPara->visiocytemsgxyzi->z);
	}
	else if(string(pSoapPara->str_func) == "visiocytesetClip1")
	{
		existing_3dviewer->getGLWidget()->setXClip1(pSoapPara->visiocytemsgxyzi->x);
		existing_3dviewer->getGLWidget()->setYClip1(pSoapPara->visiocytemsgxyzi->y);
		existing_3dviewer->getGLWidget()->setZClip1(pSoapPara->visiocytemsgxyzi->z);
	}
	else if(string(pSoapPara->str_func) == "visiocytelookAlong")
	{
		existing_3dviewer->getGLWidget()->modelRotation(pSoapPara->visiocytemsgxyzf->x, pSoapPara->visiocytemsgxyzf->y, pSoapPara->visiocytemsgxyzf->z);
	}
    else if(string(pSoapPara->str_func) == "visiocyteget3dvinfo")
	{        
        pSoapPara->visiocytemsggetinfo->xrot = existing_3dviewer->getGLWidget()->xRot();
        pSoapPara->visiocytemsggetinfo->yrot = existing_3dviewer->getGLWidget()->yRot();
        pSoapPara->visiocytemsggetinfo->zrot = existing_3dviewer->getGLWidget()->zRot();
        
        pSoapPara->visiocytemsggetinfo->zoom = existing_3dviewer->getGLWidget()->zoom();
        
        pSoapPara->visiocytemsggetinfo->xshift = existing_3dviewer->getGLWidget()->xShift();
        pSoapPara->visiocytemsggetinfo->yshift = existing_3dviewer->getGLWidget()->yShift();
        pSoapPara->visiocytemsggetinfo->zshift = existing_3dviewer->getGLWidget()->zShift();
        
        pSoapPara->visiocytemsggetinfo->gotinfo = true;
        
	}
    else
    {
        visiocyte_msg(QString("Unknown operation."), 1);
        return;
    }
	
    return;
}

void MainWindow::do3dfunc()
{
	QString fileName(pSoapPara->str_message);
	
	if (!fileName.isEmpty()) 
	{
		// find triview window
		XFormWidget *existing_imgwin = findMdiChild(fileName);	
		
		// find 3d main window
		VisiocyteR_MainWindow *existing_3dviewer = find3DViewer(fileName);
		
		// handling
		if(existing_imgwin)
		{
			if(!existing_3dviewer)
			{
				existing_imgwin->doImage3DView();
				existing_3dviewer = find3DViewer(fileName);
			}
			
			switch3dviewercontrol(existing_3dviewer);
		}
		else
		{
			this->loadVISIOCYTEFile(pSoapPara->str_message, true, true);
			
			existing_imgwin = findMdiChild(fileName);
			if(!existing_imgwin) 
			{
				// try open image file in currrent dir
				QString tryFileName = QDir::currentPath() + "/" + fileName;
				visiocyte_msg(QString("Try to open the file [%1].").arg(tryFileName), 1);
				
				this->loadVISIOCYTEFile(tryFileName, true, true);
				
				if(!existing_imgwin) 
				{
					visiocyte_msg(QString("The file [%1] does not exist! Do nothing.").arg(fileName), 1);
					return;	
				}
			}
			
			if(!existing_3dviewer)
			{
				existing_imgwin->doImage3DView();
				existing_3dviewer = find3DViewer(fileName);
			}
			
			switch3dviewercontrol(existing_3dviewer);
		}
		
	}
	else
	{
		visiocyte_msg(QString("The file [%1] does not exist! Do nothing.").arg(fileName), 1);
		return;	
	}
}


// slot function for init web service thread
void MainWindow::initWebService(VISIOCYTEWebService *pws)
{
	connect(pws, SIGNAL(finished()), pws, SLOT(deleteLater()));
	pws->start();
}

// slot function for quit web service thread
void MainWindow::quitWebService(VISIOCYTEWebService *pws)
{	
	pws->exit();
}

// slot function for passing soap parameters
void MainWindow::setSoapPara(soappara *pSoapParaInput)
{
	pSoapPara = pSoapParaInput;
}

// slot function for response web service
void MainWindow::webserviceResponse()
{	
	qDebug()<<"web service response here ...";
	
	this->setSoapPara(visiocytews->getSoapPara());
	
	if(pSoapPara)
	{
		if(string(pSoapPara->str_func) == "helloworld")
		{
			QMessageBox::information((QWidget *)0, QString("title: visiocyte web service"), QString(pSoapPara->str_message));
		}
		else if(string(pSoapPara->str_func) == "visiocyteopenfile")
		{
			this->loadVISIOCYTEFile(pSoapPara->str_message, true, false);
		}
		else if(is3DviewerFunc(pSoapPara->str_func))
		{			
			do3dfunc();
		}
		else if(string(pSoapPara->str_func) == "visiocytesavefile") // plugin method call
		{
			// is the same file name, ignore cases
			if(QString(pSoapPara->visiocytemsgsave->saveName).toUpper() == QString(pSoapPara->str_message).toUpper())
			{
				visiocyte_msg(QString("The saving file [%1] is the same to the original file [%2].").arg(pSoapPara->visiocytemsgsave->saveName).arg(pSoapPara->str_message), 1);
				return;
			}
			
			//
			QString fileName(pSoapPara->str_message);
			
			// load image
			if (!fileName.isEmpty()) 
			{
				// replace the existing file ?
				if (QFile::exists(pSoapPara->visiocytemsgsave->saveName))
				{
					if(QMessageBox::No == QMessageBox::question (0, "", QString("Do you want to replace the existing file [%1]?").arg(pSoapPara->visiocytemsgsave->saveName), QMessageBox::Yes, QMessageBox::No))
					{
						return;
					}
				}
				
				// find triview window
				XFormWidget *existing_imgwin = findMdiChild(fileName);	
				
				// handling
				if(!existing_imgwin)
				{
					this->loadVISIOCYTEFile(pSoapPara->str_message, true, false); // 3d viewer is not opened by default
					
					existing_imgwin = findMdiChild(fileName);
					if(!existing_imgwin) 
					{
						// try open image file in currrent dir
						QString tryFileName = QDir::currentPath() + "/" + fileName;
						visiocyte_msg(QString("Try to open the file [%1].").arg(tryFileName), 1);
						
						this->loadVISIOCYTEFile(tryFileName, true, false);
						
						if(!existing_imgwin) 
						{
							visiocyte_msg(QString("The file [%1] does not exist!.").arg(fileName), 1);
							return;	
						}
					}					
				}
				
				//save
				if (activeMdiChild()->saveFile(pSoapPara->visiocytemsgsave->saveName))
				{
					setCurrentFile(pSoapPara->visiocytemsgsave->saveName);
					statusBar()->showMessage(tr("File saved"), 2000);
				}
			}
			else
			{
				visiocyte_msg(QString("The file [%1] does not exist! Do nothing.").arg(fileName), 1);
				return;	
			}
		}		
		else if(string(pSoapPara->str_func) == "visiocytewscallpluginmethod") // plugin method call
		{			
			QString fileName(pSoapPara->str_message);
			
			// setting plugin name and method call
			setBooleanCLplugin(true);
			setPluginName(pSoapPara->visiocytepluginm->pluginName);
			setPluginMethod(pSoapPara->visiocytepluginm->pluginMethod);
			
			// load image
			if (!fileName.isEmpty()) 
			{
				// find triview window
				XFormWidget *existing_imgwin = findMdiChild(fileName);	
				
				// handling
				if(!existing_imgwin)
				{
					this->loadVISIOCYTEFile(pSoapPara->str_message, true, false); // 3d viewer is not opened by default
					
					existing_imgwin = findMdiChild(fileName);
					if(!existing_imgwin) 
					{
						// try open image file in currrent dir
						QString tryFileName = QDir::currentPath() + "/" + fileName;
						visiocyte_msg(QString("Try to open the file [%1].").arg(tryFileName), 1);
						
						this->loadVISIOCYTEFile(tryFileName, true, false);
						
						if(!existing_imgwin) 
						{
							visiocyte_msg(QString("The file [%1] does not exist! Try run plugin directly.").arg(fileName), 1);
							emit imageLoaded2Plugin();
							return;	
						}
					}
				}
				
				// call plugin
				emit imageLoaded2Plugin();
			}
			else
			{
				visiocyte_msg(QString("The file [%1] does not exist! Do nothing.").arg(fileName), 1);
				return;	
			}
		}
        else if(string(pSoapPara->str_func) == "visiocytewscallpluginfunc") // plugin func call
		{
			QString fileName(pSoapPara->str_message);
			
			// setting plugin name and method call
			setBooleanCLplugin(true);
			setPluginName(pSoapPara->visiocytepluginf->pluginName);
			setPluginFunc(pSoapPara->visiocytepluginf->pluginFunc);
            
            visiocyteclp.pluginfunc = pluginfunc;
			
			// load image
			if (!fileName.isEmpty()) 
			{
                visiocyteclp.fileList.clear();
                visiocyteclp.fileList.push_back(pSoapPara->str_message);
                
                if(pSoapPara->visiocytepluginf->parameters)
                {
                    visiocyteclp.cmdArgList.clear();
                    visiocyteclp.cmdArgList.push_back(pSoapPara->visiocytepluginf->parameters);
                }
                
                if(pSoapPara->visiocytepluginf->outputFile)
                {
                    visiocyteclp.outputList.clear();
                    visiocyteclp.outputList.push_back(pSoapPara->visiocytepluginf->outputFile);
                }
                
				// call plugin
				emit imageLoaded2Plugin();
			}
			else
			{
				visiocyte_msg(QString("The file [%1] does not exist! Do nothing.").arg(fileName), 1);
				return;	
			}

		}
		else
		{
			QMessageBox::information((QWidget *)0, QString("title: visiocyte web service"), QString("Wrong function to invoke!"));
		}
	}
	
}

#endif //__visiocytewebservice__