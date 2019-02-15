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




//by Hanchuan Peng
//081115: separated from the visiocyte_core.cpp
//last update: 090801 by PHC
//last update: 130830 by PHC

#include "visiocyte_version_info.h"
#include "visiocyte_compile_constraints.h"
#include "../basic_c_fun/visiocyte_message.h"
#include "../plugin_loader/visiocyte_plugin_loader.h"
#include "visiocyte_actions.h"

#if defined(USE_Qt5_VS2015_Win7_81) || defined(USE_Qt5_VS2015_Win10_10_14393)
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QNetworkReply>
// #include <QXmlSchema> // Qt 4.6 or later only
#include <QtXml>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QPalette>

namespace visiocyte {
    // Set current version here.

    VersionInfo thisVersionOfVISIOCYTE("3.594");

    QString versionnumber = "Visiocyte (3D Visualization-Assisted Analysis) (" +
        thisVersionOfVISIOCYTE.toQString() +
        "), Visiocyte-Neuron (2.1), Visiocyte Plugin Interface (2.12) ";
}


void visiocyte_aboutinfo()
{
    QString helptext =
        "<H3>Visiocyte: A Swiss army knife for 3D/4D/5D volume image and surface visualization and processing, developed by Hanchuan Peng, Zongcai Ruan, Fuhui Long, et al. All rights reserved.</H3> "
//      "<H3><span style=\"color:#FF0000\">If you are seeing this red color information, you are using an alpha-testing version of Visiocyte. If you experience any problem, please contact Hanchuan Peng. </span></H3> "
        "If you have used Visiocyte, please cite as the following: <br><br>"
        "<span style=\"color:#0000FF\">Peng, H., Ruan, Z., Long, F., Simpson, J.H., and Myers, E.W. (2010) \"VISIOCYTE enables real-time 3D visualization and quantitative analysis of large-scale biological image data sets,\" Nature Biotechnology, Vol. 28, No. 4, pp.348-353. (http://visiocyte.org) </span><br> "
        "<br>"
        "<span style=\"color:#0000FF\">Peng, H., Bria, A., Zhou, Z., Iannello, G., and Long, F. (2014) \"Extensible visualization and analysis for multidimensional images using Visiocyte,\" Nature Protocols, Vol. 9, No. 1, pp. 193-208. (http://visiocyte.org) </span><br> "
        "<br>"
        "<span style=\"color:#0000FF\">Peng, H., et al. (2014) \"Virtual finger boosts three-dimensional imaging and microsurgery as well as terabyte volume image visualization and analysis,\" Nature Communications, Vol. 5, No. 4342, DOI: 10.1038/ncomms5342. </span><br> "
        "<br>"
        "<span style=\"color:#0000FF\">Bria, A., et al. (2016) \"TeraFly: real-time 3D visualization and 3D annotation of terabytes of multidimensional volumetric images,\" Nature Methods, Vol. 13, pp. 192-194, DOI: 10.1038/nmeth.3767.</span><br> "
        "<br>"

        "For the latest information/help and further documentations, visit the Visiocyte website <a href=\"http://visiocyte.org\">http://visiocyte.org</a><br>"
        "<br>============================================="
        "<H1>Help Information</H1>"
        "=============================================<br>"
        "Most functions of this software should be intuitive and easy to use. A few short-cuts are listed below.<br>"
        "<H3>Supported file types</H3>"
        "You can also download a Matlab toolbox to read and write most of the basic data formats at the Visiocyte website <a href=\"http://visiocyte.org\">http://visiocyte.org</a>."
        "<br><table border=\"1\">"
        "<tr><td>.TIF, .TIFF, .LSM (Tiff and LSM stacks) </td><td>3D image stacks. </td></tr>"
        "<tr><td>.RAW (Visiocyte's RAW), .VISIOCYTERAW (Visiocyte's RAW), .VISIOCYTERAW (Visiocyte's RAW), .visiocytepbd (Visiocyte's RAW with compression) </td><td>a simple raw file which supports 8-bit unsigned char, 16-bit unsigned short, 32-bit single-precision float 4D image stacks. </td></tr>"
        "<tr><td>.TIF (Series 2D tiff section) </td><td>the Leica scope produced series tiff files can be imported. </td></tr>"
        "<tr><td>.SWC </td><td>reconstructed neurons or other relational data which can be described as a graph. </td></tr>"
        "<tr><td>.APO </td><td>point cloud file used in WANO to describe the cells and any other image objects. </td></tr>"
        "<tr><td>.VISIOCYTES, .OBJ </td><td>surface object files. OBJ file is ASCII description of surface objects, which will be slow in opening but can be read by other software; VISIOCYTES is binary representation of surface and much faster to load. </td></tr>"
        "<tr><td>.ANO </td><td>the linker file which contains file names of other heterogeneous files. </td></tr>"
        "<tr><td>.MARKER </td><td>the landmark recording file (which is just a plain csv file with a specified format). </td></tr>"
        "<tr><td>.ATLAS </td><td>the linker file which contains a list of image-stack file names that can be opened using the atlas manager. </td></tr>"
        "<tr><td>.PC_ATLAS </td><td>the linker file which contains a list of point cloud files names for the point cloud atlas. </td></tr>"
                                    "<tr><td>.ZIP </td><td>on Mac and Linux the .zip files of the above Visiocyte file formats can be opened automatically. </td></tr>"
        "</table><br>"
        "<H3>Keyboard operations</H3>"
        "When any view of images is active, the following short-cut keys can be used. </td></tr>"
        "<br><table border=\"1\">"
        //"<tr><td>N/. </td><td>Move to the next focal plane.</td></tr>"
        //"<tr><td>B/, </td><td>Move to the previous focal plane.</td></tr>"
        "<tr><td>M </td><td>Add a new landmark. This can also be doen quickly by pressing Shift key and Left Mouse Key at the same time.</td></tr>"
        "<tr><td>I </td><td>Zoom-in by a factor 2.</td></tr>"
        "<tr><td>O </td><td>Zoom-out by a a factor 2 (will not be effective if any displayed dimension of an image is <=1 pixel.</td></tr>"
        "<tr><td>1/2/3/4 </td><td>set the tri-view zoom to x1 (no zoom-in or out) or x2, x3, x4 zoom. The windows may be zipped together but can be expaned by either slightly resizing the window boundary or click the cascade menu</td></tr>"
        "<tr><td>Ctrl-O (Windows, Linux) or Cmd-O (Mac) </td><td>Open an image stack. Note that you can also directly drag and drop an image file (Tiff stack or LSM or Hanchuan Peng's RAW files) into Visiocyte to open it. </td></tr>"
        "<tr><td>Ctrl-I (Windows, Linux) or Cmd-I (Mac) </td><td>Import Leica tiff series as an image stack. </td></tr>"
        "<tr><td>Ctrl-S (Windows, Linux) or Cmd-S (Mac) </td><td>Save image file. </td></tr>"
        "<tr><td>Ctrl-C (Windows, Linux) or Cmd-C (Mac) </td><td>Crop image based on the Region Of Interest (ROI) defined (see the Ctrl-click mouse operation to edit the ROI). </td></tr>"
        "<tr><td>Shift-C </td><td>Switch among the three preset colormaps for an indexed/mask image (unsigned short int, 16bit). </td></tr>"
        //"<tr><td>Ctrl-D (Windows, Linux) or Cmd-D (Mac) </td><td>Delete the last added corner point of the ROI. </td></tr>"
        "<tr><td>Ctrl-R (Windows, Linux) or Cmd-R (Mac) </td><td>Rotate image. </td></tr>"
        "<tr><td>Ctrl-P (Windows, Linux) or Cmd-P (Mac) </td><td>Pop up the image processing dialog box. </td></tr>"
        "<tr><td>Ctrl-V (Windows, Linux) or Cmd-V (Mac) </td><td>3D view of an image stack (will downsize to 512x512x256 if bigger than that size). </td></tr>"
        "<tr><td>Shift-V </td><td>3D view of the region of interest (roi) defined for an image stack (if no roi defined, the  use entire image. auto downsize to 512x512x256 if bigger than that size). </td></tr>"
        "<tr><td>Ctrl-Shift-V (Windows, Linux) or Cmd-Shift-V (Mac) </td><td>3D view of an entire image stack (no image downsizing, may cause a video-memory crash if there is hardware limitation). </td></tr>"
        "<tr><td>Ctrl-A (Windows, Linux) or Cmd-A (Mac) </td><td>Launch atlas-viewer/image blender (together with the landmark manager). </td></tr>"
        "<tr><td>Ctrl-F (Windows, Linux) or Cmd-F (Mac) </td><td>Launch the landmark finder (together with atlas-viewer/image blender). </td></tr>"
        "<tr><td>Ctrl-Z (Windows, Linux) or Cmd-Z (Mac) </td><td>Undo last tracing operation.</td></tr>"
        "<tr><td>Y </td><td>ON/OFF switch of image operation timer.</td></tr>"
        "</table><br>"
        "<H3>Mouse operations</H3>"
        "<br><table border=\"1\">"
        "<tr><td>Double-click anywhere close to any existing landmark </td><td>choose to move or delete a landmark.</td></tr>"
        "<tr><td>Shift-click anywhere (while not selecting a marker yet) will quickly define and display a marker, without entering the marker definition dialog.</td></tr>"
        "<tr><td>Shift-click anywhere after selecting to move a landmark </td><td>move the specified landmark to a new location.</td></tr>"
        "<tr><td>Ctrl-click  (Windows, Linux) or Cmd-click (Mac) anywhere </td><td>add a new corner point to an ROI.</td></tr>"
        "<tr><td>Alt-click anywhere </td><td>remove the last added corner point in an ROI.</td></tr>"
        "</table><br>"
        "<H3>Wheel operations</H3>"
        "<br><table border=\"1\">"
        "<tr><td>When click-in any of the XY, YZ, or XZ view of an image stack, you can scroll through different depths.</td></tr>"
        "</table><br>"
        "<H3>Special debugging keys</H3>"
        "<br><table border=\"1\">"
        //"<tr><td>Ctrl-E (Windows, Linux) or Cmd-E (Mac) </td><td>Change the landmark match method (useful for image registration). </td></tr>"
        //"<tr><td>Shift-E </td><td>Change the Displacement Field computing method. </td></tr>"
        //"<tr><td>W </td><td>short-cut to do find the best matching point for 1 single lamdmark. </td></tr>"
        "<tr><td>T </td><td>short-cut to trace a path between two landmarks (if landmarks defined). </td></tr>"
        "</table><br>"

        "<H3>For 3D view window short keys</H3>"
        "<br><table border=\"1\">"
        "<tr><td>Ctrl-W (Windows, Linux) or Cmd-W (Mac) </td><td>close current 3D viewer Window.</td></tr>"
        "<tr><td>Ctrl-A (Windows, Linux) or Cmd-A (Mac) </td><td>toggle Animating the rotation of objects.</td></tr>"
        "<tr><td>Ctrl-M (Windows, Linux) or Cmd-M (Mac) </td><td>generate Movie.</td></tr>"
        "<tr><td>Ctrl-B (Windows, Linux) or Cmd-B (Mac) </td><td>change Brightness of whole view.</td></tr>"
        "<tr><td>Ctrl-R (Windows, Linux) or Cmd-R (Mac) </td><td>Reload data set (return to the initial data set, but user-controlled parameters isn't changed).</td></tr>"
        "<tr><td>Ctrl-U (Windows, Linux) or Cmd-U (Mac) </td><td>Update landmarks and traced curves from associated tri-view.</td></tr>"
        "<tr><td>Ctrl-P (Windows, Linux) or Cmd-P (Mac) </td><td>toggle the Polygon-fill/line/point/transparent display mode of surface object.</td></tr>"
        "<tr><td>Alt-P                                  </td><td>change Polygon display options of surface object.</td></tr>"
        "<tr><td>L                                      </td><td>invoke quick curveline detector.</td></tr>"
        "<tr><td>Ctrl-L (Windows, Linux) or Cmd-L (Mac) </td><td>toggle skeleton-view and tube-view of SWC objects (e.g. a neuron).</td></tr>"
        "<tr><td>Alt-L                                  </td><td>change skeleton-view options of SWC objects (e.g. a neuron).</td></tr>"
        "<tr><td>Ctrl-Z (Windows, Linux) or Cmd-Z (Mac) </td><td>Undo last tracing/editing operation of SWC objects (e.g. a neuron).</td></tr>"
        "<tr><td>Ctrl-X (Windows, Linux) or Cmd-X (Mac) </td><td>Redo last tracing/editing operation of SWC objects (e.g. a neuron).</td></tr>"
        "<tr><td>Ctrl-N (Windows, Linux) or Cmd-N (Mac) </td><td>toggle Name of APO object (e.g. point cloud).</td></tr>"
        "<tr><td>Shift-N </td><td>toggle Names of markers objects (not their labels!).</td></tr>"
        "<tr><td>Ctrl-V (Windows, Linux) or Cmd-V (Mac) </td><td>update Volume image from tri-view.</td></tr>"
        "<tr><td>Alt-V                                  </td><td>change Volume rendering advance options.</td></tr>"
        "<tr><td>Ctrl-F (Windows, Linux) or Cmd-F (Mac) </td><td>toggle volume texture Filter.</td></tr>"
        "<tr><td>Mouse-drag </td>                         <td>Rotation around x/y-axis of view space, like track ball.</td></tr>"
        "<tr><td>Shift-Mouse-wheel, [ and ] </td>         <td>Rotation around z-axis of view space.</td></tr>"
        "<tr><td>Alt-Mouse, Alt-arrows </td>              <td>Rotation around axes of objects itself.</td></tr>"
        "<tr><td>\\ </td>                                 <td>Reset rotation.</td></tr>"
        "<tr><td>Ctrl-\\ (Windows, Linux) or Cmd-\\ (Mac) </td><td>toggle Parallel projection (Scale bar).</td></tr>"
        "<tr><td>Shift-Mouse, Shift-arrows </td>     <td>Shift the objects around.</td></tr>"
        "<tr><td>Left, Right, Up, Down arrows </td>       <td>Shift the view window around.</td></tr>"
        "<tr><td>Mouse-wheel, - and = </td>               <td>Zoom-out and zoom-in.</td></tr>"
        "<tr><td>Backspace </td>                          <td>Reset zoom and shift.</td></tr>"
        "<tr><td>, and . </td>                            <td>Move the volume front/f-slice plane.</td></tr>"
        "<tr><td>/ </td>                                  <td>Reset volume front plane and cross-section planes.</td></tr>"
        "<tr><td>Mouse-right-button </td>                 <td>Select object and active popup menu of the selected object.</td></tr>"
        "<tr><td>[n]-holding-Mouse-right-button </td>     <td>Marker on [n]th channel when define marker or curve.</td></tr>"
        "<tr><td>Shift - and Shift = </td>                <td>Decrease and increase marker size.</td></tr>"
        "<tr><td>Escape </td>                             <td>Exit from status of defining markers or curves.</td></tr>"
        "<tr><td>Shift , and Shift . </td>                <td>Backward and forward volume time point.</td></tr>"
        "<tr><td>Shift / </td>                            <td>Reset volume time point.</td></tr>"
		"<tr><td>Shift-W </td>                            <td>Decrease the size of drag window in curve dragging mode.</td></tr>"
		"<tr><td>Alt-W </td>                              <td>Increase the size of drag window in curve dragging mode.</td></tr>"
        "</table><br>"
        "<H3>Volume colormap control</H3>"
        "Rectangle point is start or end of colormap curve, and its horizontal coordinate is locked at left or right side. Circle point can be moved freely.<br>"
        "<br><table border=\"1\">"
        "<tr><td>Mouse-left-click </td>     <td>Insert a control point to colormap curve.</td></tr>"
        "<tr><td>Mouse-right-click </td>    <td>Delete specified control point of colormap curve.</td></tr>"
        "<tr><td>Mouse-drag </td>           <td>Move specified control point of colormap curve.</td></tr>"
        "</table><br>"

        "<H3>For neuron tracing/editing short keys</H3>"
        "<br><table border=\"1\">"
        "<tr><td>Alt-B </td><td>invoke the curve drawing function using serial BBoxes.</td></tr>"
        "<tr><td>Alt-C </td><td>invoke the multiple neuron-segments Connection function.</td></tr>"
        "<tr><td>Alt-D </td><td>invoke the multiple neuron-segments Deleting function.</td></tr>"
        "<tr><td>Alt-E </td><td>toggle Edit/non-edit mode.</td></tr>"
        "<tr><td>Alt-G </td><td>invoke the curve drawing function using Global optimal.</td></tr>"
        "<tr><td>Alt-S </td><td>invoke the multiple neuron-segments Splitting/breaking function.</td></tr>"
        "<tr><td>Alt-T </td><td>invoke the multiple neuron-segments reTyping function.</td></tr>"
        "<tr><td>Alt-Y </td><td>invoke 3D polYline defining function using series of right-clicks.</td></tr>"
        "</table><br>"
        "Note:if drawing fuction can't work well where the weak branches are influenced by strong branches, you can try 'shift' shortcut to adjust the threshold of image intensity under drawing <br>"
        "function using serial BBoxes(Alt+ B shortcut) or drawing function using Global optimal (Alt+G shortcut).Firstly,entering into 'Alt+B' or 'Alt+ G' drawing function.Then,pressing 'shift' <br>"
        "when you are drawing lines.After that,a spinbox named 'Intensity Threshold 1%-100%' will be popped in your window and you can input a number between 1 to 100 (e.g. number 60 means that <br>"
        "the intensity of the seaching area less than sixty percentage will be considered.                                                                                                        <br>"

        "<H3>Some known problems</H3>"
        "So far Visiocyte has been tested on hundreds of machines with different software/hardware environments; in most cases (Mac, Linux, Windows) the software has a nice performance. Yet the following are some known problems.<br>"
        "<br><table border=\"1\">"
	    "<tr><td>Mac machines with Snow Leopard (Mac OS X 10.6.8) and try to run a Linux version of Visiocyte throught X-Window: </td>     <td>The 3D viewer window cannot be launched properly. This seems to be a Snow Leopard bug with X-Windows, as the Linux version of Visiocyte's 3D viewer can run w/o a problem for other Mac OS (e.g. Leopard, i.e. OS X 10.5) and other tested Linux machines (e.g. redhar). </td></tr>"
        "<tr><td>Mac machines with Tiger (Mac OS X 10.4) and defective NVIDIA GeForce 7300GT video cards</td>     <td>GeForce 7300GT does not support multisampling while it claims to be. This incompatibility makes 3D viewer run very slow. A special Visiocyte version disables the multipling support on this video card solves the problem.</td></tr>"
        "<tr><td>MacBook Pro with Tiger (Mac OS X 10.4) and defective NVIDIA GeForce 8600MGT video card driver</td>     <td>Sometimes the labels of markers may be hidden in the 3D volume image rendering. Upgrage to Leopard OS (and thus also upgrade Mac OpenGL driver) solves the problem.</td></tr>"
        "<tr><td>MacBook Pro with Tiger (Mac OS X 10.4) and defective NVIDIA GeForce 8600MGT video card driver</td>     <td>The 5D rendering displays abnormal patterns due to a problem in 3D texture of the video card driver. </td></tr>"
        "<tr><td>Some Windows machines cannot display the 3D viewer</td>     <td>Some video card drivers report that it supports OpenGL 2.0, but actually it cannot support. Try to press Ctrl-Shift-G to disable OpenGL 2.0 functions used.  </td></tr>"
        "<tr><td>Neuron editing: the 'merge to nearby segments' function may produce extra root-nodes</td>     <td>The neuron editing function can become quite complicated as we'd like to support any graph. if you see unwanted roots, you may choose 'reverse' nearby segments to correct; but this may make other segments' directions change.  Suggest use 'break' combined by 'tracing' instead of direct 'merge'.</td></tr>"
        "</table><br>"
        "<H2>Key releases:</H2><br> Feb 2006, Aug 16 (v1.1), Sept (v2.400), 2009. "
        "Jan 31 (v2.449), Feb-Dec (v2.4-v2.5), 2010; Jan - Aug, 2011 (v2.6). 2012-2014 (v2.8). "
        "All rights reserved.<br><br> "
        "Version Note: if you see a version number followed by a letter (e.g. 1.80a), it is a customized build for a particular research lab. "
        "Visiocyte also have additional image analysis modules, for further information, contact Hanchuan Peng.<br><br>";

    //QMessageBox::information(0, versionnumber, versionnumber + "<br>" + helptext);

    QString visiocyte_compile_info = "";
    if (COMPILE_TARGET_LEVEL==2)
        visiocyte_compile_info = "Advantage (alpha)"; //full version with all toolboxes
    else if (COMPILE_TARGET_LEVEL==1)
        visiocyte_compile_info = "Pro";
    else //COMPILE_TARGET_LEVEL==0
        visiocyte_compile_info = "Lite";

    QString build_info = QString("build time: ")+BUILD_TIME+", "+BUILD_OS_INFO+", "+BUILD_QT_INFO+" "+BUILD_BITS;


    QTextEdit *p_mytext = new QTextEdit(visiocyte::versionnumber + visiocyte_compile_info +
                            "<br>" + build_info +
                            "<br><br>" + helptext);
    p_mytext->setDocumentTitle(visiocyte::versionnumber);
    p_mytext->resize(700, 700); //use the QWidget function
    p_mytext->setReadOnly(true);
    p_mytext->setFontPointSize(12);
    p_mytext->show();
}


void visiocyte_Lite_info()
{
	QString helptext =      "<H2><span style=\"color:#0000FF\">You are using the </span><span style=\"color:#FF0000\">Visiocyte Lite</span>.<span style=\"color:#0000FF\"> "
							"Most 3D visualization modules have been enabled. However the 3D data/image processing modules have been disabled. "
							"To enable these tools (and disable this message), you can <span style=\"color:#FF0000\">register as a Visiocyte Pro user for free</span> at http://penglab.janelia.org/proj/visiocyte </span></H2>";

	//QMessageBox::information(0, versionnumber, versionnumber + "<br>" + helptext);

	QString visiocyte_compile_info = "";
	if (COMPILE_TARGET_LEVEL==2)
		visiocyte_compile_info = "Advantage (alpha)"; //full version with all toolboxes
	else if (COMPILE_TARGET_LEVEL==1)
		visiocyte_compile_info = "Pro";
	else //COMPILE_TARGET_LEVEL==0
		visiocyte_compile_info = "Lite";

	QString build_info = QString("build time: ")+BUILD_TIME+", "+BUILD_OS_INFO+", "+BUILD_QT_INFO+" "+BUILD_BITS;


	visiocyte_msg(visiocyte::versionnumber + visiocyte_compile_info +
			"<br>" + build_info +
			"<br><br>" + helptext);
//	p_mytext->setDocumentTitle(versionnumber);
//	p_mytext->resize(300, 300); //use the QWidget function
//	p_mytext->setReadOnly(true);
//	p_mytext->setFontPointSize(12);
//	p_mytext->show();
//
}

namespace visiocyte {


////////////////////////
// UpdateItem methods //
////////////////////////

// Whether to try installing this item.  Settable in Update List dialog.
void UpdateItem::setInstall( int state )
{
    bDoInstall = (state == Qt::Checked);
    // qDebug() << "setInstall";// visiocyte_msg("setInstall",0);
}

// Slot to start updating this item from the remote server
void UpdateItem::startUpdate(DownloadingUpdatesDialog* p)
{
    if (p->wasCanceled())
        return;

    progressDialog = p;
    progressDialog->informativeTextLabel->setText(tr("Downloading ") + relativeName + "...");
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    // When the download is complete, write the file to disk
    connect(nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishedDownloadSlot(QNetworkReply*)));
    // visiocyte_msg(remoteUrl.toString(),0);
    nam->get(QNetworkRequest(remoteUrl));

    // Each item gets to add a total of 2 to the progress dialog.
    // One at the beginning:
    progressDialog->progressBar->setValue(progressDialog->progressBar->value() + 1);
}

// Slot after download has completed, before local file is updated
void UpdateItem::finishedDownloadSlot(QNetworkReply* reply)
{
    // Construct local file name.
    // (note that localFile might not be populated, especially if this
    //  is a new plugin).
    QDir visiocyteDir(QCoreApplication::applicationDirPath());
    QString fullpath = visiocyteDir.absoluteFilePath(relativeName);
    QFile file(fullpath);
    QFileInfo fileInfo(file);

    QDir dir = fileInfo.absoluteDir();
    QString currentName = dir.relativeFilePath(fullpath);
    QString newName = currentName + ".new";
    QString oldName = currentName + ".old";

    QFile newFile(dir.filePath(newName));
    QFile oldFile(dir.filePath(oldName));

    bool succeeded = true; // start optimistic

    if (progressDialog->wasCanceled()) { // user said stop
        visiocyte_msg("Download was canceled",0);
        succeeded = false;
    }

    if (succeeded && (reply->error() != QNetworkReply::NoError)) { // http error
        visiocyte_msg("HTTP error",0);
        succeeded = false;
    }

    if (succeeded && reply->header(QNetworkRequest::ContentLengthHeader).toLongLong() <= 0) {
        // download has no content
        visiocyte_msg("Download has no content",0);
        succeeded = false;
    }

    if (succeeded) { // try to create directory
        // Might need to create directory
        dir = fileInfo.absoluteDir();
        if (! dir.mkpath(".") ) {
            succeeded = false;
            visiocyte_msg("Failed to create directory",0);
        }
    }

    // Try saving web file as "foo.new"
    if (succeeded && newFile.exists()) {
        if (! newFile.remove()) {
            visiocyte_msg("Unable to remove old .new file",0);
            succeeded = false;
        }
    }

    if (succeeded)
    {
        if (newFile.open(QIODevice::WriteOnly)) {
            progressDialog->setWindowTitle(tr("Saving ") + relativeName + "...");
            newFile.write(reply->readAll());
            newFile.close();
            // Second increment of two on progress bar
            progressDialog->progressBar->setValue(progressDialog->progressBar->value() + 1);
            // visiocyte_msg("Wrote file",0);
        }
        else {
            succeeded = false;
            visiocyte_msg("Failed to write .new file.",0);
        }
    }

    // Especially for visiocyte.exe, permissions must include read and execute
    if (succeeded && file.exists()) {
        QFile::Permissions oldPermissions = file.permissions();
        QFile::Permissions newPermissions = newFile.permissions();
        if (oldPermissions != newPermissions) {
            newFile.setPermissions(oldPermissions);
        }
    }

    // Rename old file to "foo.old"
    if (succeeded && oldFile.exists()) {
        if (! oldFile.remove()) {
            visiocyte_msg("Unable to remove old .old file",0);
            succeeded = false;
        }
    }
    if (succeeded && file.exists()) {
        if (! dir.rename(currentName, oldName) ) {
            visiocyte_msg("Unable to rename old file to .old",0);
            succeeded = false;
        }
    }

    if (succeeded) {
        if (! dir.rename(newName, currentName)) {
            visiocyte_msg("Unable to rename new file",0);
            succeeded = false;
        }
    }

    // TODO - might want to remove remaining foo.old file

    // Summarize failure
    if (! succeeded) {
        // TODO - how to report this upstream...
        visiocyte_msg("Download failed: " + relativeName, 0);
        QMessageBox::warning(NULL, tr("Download failed"),
                tr("There was a problem downloading ") + relativeName
                + "\nTry again later.");
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    reply->disconnect();
    reply->deleteLater();

    emit updateComplete(progressDialog, succeeded);
}

////////////////////////////
// VersionChecker methods //
////////////////////////////

// static
QString VISIOCYTEVersionChecker::getPlatformString()
{
	// Might have been defined in cmake configuration...
#ifdef VISIOCYTE_PLATFORM_NAME
	return QString(VISIOCYTE_PLATFORM_NAME);
#else
    return QString(BUILD_OS_INFO) + "_" + BUILD_BITS;
#endif
}

// static
QString VISIOCYTEVersionChecker::getDefaultVISIOCYTEVersionXmlFileName() {
    // return "visiocyte_version_"+getPlatformString()+".xml";
    return "visiocyte_version.xml";
}

// static
QString VISIOCYTEVersionChecker::getDefaultVISIOCYTEVersionUrl()
{
//      return "http://brunsc-wm1.janelia.priv/~brunsc/visiocyte/stable_version/" // testing

//    return "http://penglab.janelia.org/proj/visiocyte/stable_version/"
//            + getPlatformString() + "/"
//            + getDefaultVISIOCYTEVersionXmlFileName();

    //use the following instead
    return "http://home.penglab.com/proj/visiocyte/current_release/visiocyte_version.xml";
}

VISIOCYTEVersionChecker::VISIOCYTEVersionChecker(QWidget *guiParent_param)
    : guiParent(guiParent_param)
{}

// no argument version of createVersionXml pops a dialog to ask for file.
void VISIOCYTEVersionChecker::createVersionXml()
{
    // Default location is same as visiocyte.exe...
    QDir defaultDir = QDir(QCoreApplication::applicationDirPath());
    // ...except on Mac, where location is above visiocyte.app
    if (defaultDir.dirName() == "MacOS") { // App bundle
        defaultDir.cdUp(); // Contents
        defaultDir.cdUp(); // visiocyte.app
        defaultDir.cdUp(); // parent of visiocyte.app
    }
    // defaultDir.cdUp();
    // defaultDir.cdUp();
    QString defaultPath = defaultDir.filePath(getDefaultVISIOCYTEVersionXmlFileName());

    QString xmlFile = QFileDialog::getSaveFileName(guiParent,
            tr("Save Visiocyte version xml file"),
            defaultPath,
            tr("XML files (*.xml)"));

    if (xmlFile.length() > 0)
        createVersionXml(xmlFile);
}

// This method depends on visiocyte binary being run from it's correct (relative) location.
// Should be run from a visiocyte installation like that found on a web server.
void VISIOCYTEVersionChecker::createVersionXml(QString xmlFileName)
{
    QFileInfo xmlFileInfo(xmlFileName);
    xmlFileInfo.makeAbsolute();
    QDir xmlDir = xmlFileInfo.dir();

    // We want to create relative URLs, for ease of testing and deployment.
    // To construct a relative URL, determine the relative path of the
    // VISIOCYTE executable, relative to the location of the xmlFile.
    QDir visiocyteDir = QDir(QCoreApplication::applicationDirPath());
    QString relativeUrlBase = xmlDir.relativeFilePath(visiocyteDir.absolutePath());

    // Create a fresh view of local plugins/visiocyte
    updateItems.clear();
    populateLocalUpdateItems();

    QFile xmlFile(xmlFileName);
    if (xmlFile.open(QIODevice::WriteOnly))
    {
        QTextStream xmlOut(&xmlFile);
        xmlOut << "<?xml version=\"1.0\"?>\n<visiocyte_version>\n";
        UpdateItemsType::const_iterator i;
        for (i = updateItems.begin(); i != updateItems.end(); ++i)
        {
            const UpdateItem& item = *(i->second);
            if (item.relativeName.length() < 1) continue;
            QString relativeUrl = relativeUrlBase + "/" + item.relativeName;
            if (relativeUrlBase.length() == 0)
                relativeUrl = item.relativeName; // no slash please
            // Create a new visiocyte_component element for this plugin
            xmlOut << QString("<visiocyte_component\n");
            xmlOut << QString("    name=\"%1\"\n").arg(item.relativeName);
            xmlOut << QString("    version=\"%1\"\n").arg(item.localVersion);
            xmlOut << QString("    platform=\"%1\"\n").arg(getPlatformString());
            xmlOut << QString("    href=\"%1\"\n").arg(relativeUrl);
            xmlOut << QString("/>\n");
        }
        xmlOut << "</visiocyte_version>\n";
        xmlFile.close();
    }
    else {
        QMessageBox::warning(guiParent, "XML file creation failed.", "XML file creation failed");
    }
}

void VISIOCYTEVersionChecker::checkForLatestVersion(bool b_verbose)
{
    this->b_showAllMessages = b_verbose;

    QUrl xmlFileUrl(getDefaultVISIOCYTEVersionUrl()); // begin by assuming default
    //visiocyte_msg(xmlFileUrl.toString(),0);

    // Is there already a setting for the url?
	QSettings settings("HHMI", "Visiocyte");
    QString versionUrl = settings.value("versionUrl").toString();
    if (versionUrl.length() > 0)
        xmlFileUrl = versionUrl;

    // Remember xml url, for use in constructing absolute urls from relative ones.
    visiocyte_msg(xmlFileUrl.toString(),0);
    xmlPathUrl = xmlFileUrl.resolved(QUrl("."));
    //visiocyte_msg(xmlPathUrl.toString(),0);
    //visiocyte_msg(xmlFileUrl.toString(),0);

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(gotVersion(QNetworkReply*)));

    // Prepare data structure for update items.
    updateItems.clear();
    populateLocalUpdateItems();

    nam->get(QNetworkRequest(xmlFileUrl));

    if (b_showAllMessages) {
        checkingDialog = new visiocyte::CheckingForUpdatesDialog(guiParent);
        checkingDialog->exec();
    }
}

// If the user is running a version of VISIOCYTE that is newer than the
// one she was using when she clicked "Never (automatically update)",
// we want to give the user another chance to verify the "Never".
bool VISIOCYTEVersionChecker::userSelectedNeverUpdateInAnOlderVersionOfVISIOCYTE()
{
    QSettings settings("HHMI", "Visiocyte");
    QVariant checkIntervalVariant =
            settings.value("updateCheckInterval");
    if (! checkIntervalVariant.isValid())
        return false; // update frequency is not even set
    int checkInterval = checkIntervalVariant.toInt();
    if (checkInterval >= 0)
        return false; // update frequency is not "Never"
    QVariant latestNeverUpdateVersionVariant =
            settings.value("latestNeverUpdateVersion");
    if (! latestNeverUpdateVersionVariant.isValid())
        return true; // older version did not even set latestNeverUpdateVersion
    double latestNeverUpdateVersion =
            latestNeverUpdateVersionVariant.toDouble();
    if ((latestNeverUpdateVersion + 0.0001) < visiocyte::thisVersionOfVISIOCYTE.toFloat())
        return true; // user selected never in an older version
    return false;
}

/* static */ void VISIOCYTEVersionChecker::never_update()
{
    QSettings settings("HHMI", "Visiocyte");
    settings.setValue("updateCheckInterval", -1); // never
    settings.setValue( "latestNeverUpdateVersion",
            (double)(visiocyte::thisVersionOfVISIOCYTE.toFloat()) );
}

// Examines last time version updater was queried to decide whether it might
// be time to check again now.
bool VISIOCYTEVersionChecker::shouldCheckNow()
{
    // Use similar logic to Fiji updater
    // http://pacific.mpi-cbg.de/wiki/index.php/Update_Fiji
    // 1) Was VISIOCYTE just started? Well, don't call this method otherwise. called from main.cpp
    // 2) Was VISIOCYTE started without parameters? - this logic needs to be in caller. see main.cpp
    // 3) Can files be updated by current user? also in main.cpp
    // 4) Does network work? -- managed in gotVersion()


    bool bCheckNow = true; // default is to check for updates
    // Load relevant persistent variables from Qt cache
	QSettings settings("HHMI", "Visiocyte");
    QVariant latestCheckVariant = settings.value("timeOfLatestUpdateCheck");
    QVariant checkIntervalVariant = settings.value("updateCheckInterval");
    // Use default behavior unless persistent variables are set
    if ( latestCheckVariant.isValid() && checkIntervalVariant.isValid())
    {
        // we know how long it has been and how long it should be
        int checkInterval = checkIntervalVariant.toInt();
        if (checkInterval == 0) // zero means "always"
            bCheckNow = true; // redundant, as true is the default
        else if (checkInterval < 0) { // negative means "never"
            visiocyte_msg("Visiocyte is set to NEVER check for updates.",0);
            bCheckNow = false;
        }
        else {
            // finite interval needs comparison
            QDateTime latestCheck = latestCheckVariant.toDateTime();
            int elapsedInterval = latestCheck.secsTo(QDateTime::currentDateTime());
            if (elapsedInterval < checkInterval) {
                visiocyte_msg("Timeout for next update check has not yet elapsed.",0);
                bCheckNow = false; // user does not wish to be bothered now
            }
            else {
                bCheckNow = true; // redundant, true is default
            }
        }
    }
    return bCheckNow;
}


void VISIOCYTEVersionChecker::gotVersion(QNetworkReply* reply)
{
    // User might have pressed "cancel"
    if (b_showAllMessages) {
        if (checkingDialog && checkingDialog->wasCanceled()) {
            checkingDialog->close();
            return;
        }
    }

    if (reply->error() != QNetworkReply::NoError) {
        qDebug("Problem downloading latest version information");
        if (b_showAllMessages) {
            if (checkingDialog) checkingDialog->close();
            QMessageBox::information(guiParent,
                    "Unable to connect to Visiocyte server",
                    "Could not get latest version information.\n"
                    "Please try again later;\nOr double check the version URL in the Updates->Options... menu.");
        }
        return; // error occurred, so don't bother
    }

    // CMB Oct-22-2010
    // If we get this far, consider the update "checked" for the purpose
    // of update check timeout interval.  In particular, even if the update
    // file is unparsable, don't necessarily check again immediately.
    QSettings settings("HHMI", "Visiocyte");
    settings.setValue("timeOfLatestUpdateCheck", QDateTime::currentDateTime());

    // CMB Oct-22-2010
    // You might wonder why I would use an xml file for such a simple version file.
    // The reason is that we might in the future want to have different versions
    // for different
    // platforms or for different versions of Qt.  This way we can maintain
    // backwards compatibility.  We can add new elements and new attributes
    // to the xml for this purpose, and older versions of visiocyte will just ignore
    // them, looking only at the major and minor versions.

    // Qt 4.6 or later is required for validation with xml schema
    QDomDocument versionDoc("visiocyte_version");
    versionDoc.setContent(reply->readAll());

    if (b_showAllMessages) {
        if (checkingDialog) checkingDialog->close();
    }

    processVersionXmlFile(versionDoc);
}

void VISIOCYTEVersionChecker::processVersionXmlFile(const QDomDocument& versionDoc)
{
    visiocyte_msg(QString("Process obtained version info."), 0);

    QDomElement root = versionDoc.documentElement();
    if (root.tagName() != "visiocyte_version") {
        qDebug() << "Unrecognized root element " << root.tagName();
        if (b_showAllMessages) {
            visiocyte_msg("Unable to parse version information or "
                    "Could not parse latest version information.\n"
                    "Please try again later.");
        }
        return; // silently continue on version finding error
    }

    QString remote_version = "0";
    // Parse update xml file
    QDomNode node = root.firstChild();
    while (!node.isNull())
    {
        QDomElement elem = node.toElement();
        if (!elem.isNull())
        {
            // Check plugin versions
            if (elem.tagName() == "visiocyte_component")
            {
                remote_version = elem.attribute("version", "0");
                break;
            }
        }
        node = node.nextSibling();
    }

    visiocyte_msg(QString("remote version=[%1] current version=[%2]\n").arg(remote_version.toFloat()).arg(visiocyte::thisVersionOfVISIOCYTE.toFloat()), 0);

    if (remote_version.toFloat() > visiocyte::thisVersionOfVISIOCYTE.toFloat())
    {
        visiocyte_msg(QString("There is a newer version [%1] Visiocyte. You should consider either download from the Visiocyte website (http://visiocyte.org) or check out the latest source code to build.").arg(remote_version));
    }
    else
    {
        visiocyte_msg(QString("Your Visiocyte is NOT older than the current binary release. You do NOT have to update for now."), b_showAllMessages);
    }

    return;

    //do NOT use the following code for now as it is over-complicated. Commented by PHC, 20130830.
/*
    QDomElement root = versionDoc.documentElement();
    if (root.tagName() != "visiocyte_version") {
        qDebug() << "Unrecognized root element " << root.tagName();
        if (b_showAllMessages) {
            QMessageBox::information(guiParent,
                    "Unable to parse version information",
                    "Could not parse latest version information.\n"
                    "Please try again later.");
        }
        return; // silently continue on version finding error
    }

    // Parse update xml file
    QDomNode node = root.firstChild();
    while (!node.isNull())
    {
        QDomElement elem = node.toElement();
        if (!elem.isNull())
        {
            // Check plugin versions
            if (elem.tagName() == "visiocyte_component")
            {
                QString plugin_platform = elem.attribute("platform", "");
                if (plugin_platform == getPlatformString()) { // must be same platform
                    // visiocyte_msg(QString("Found plugin platform ") + getPlatformString(), 0);
                    QString pluginName = elem.attribute("name", "");
                    QString pluginVersion = elem.attribute("version", "0");
                    QString pluginUrl = elem.attribute("href", "");
                    if (pluginName.length() > 0) { // name must not be empty
                        // Populate updateItems structure
                        // Create entry if not already present
                        if (updateItems.find(pluginName) == updateItems.end())
                            updateItems[pluginName] = new UpdateItem(this);
                        UpdateItem& item = *(updateItems[pluginName]);
                        item.relativeName = pluginName;
                        bool converted;
                        item.remoteVersion = pluginVersion.toFloat(&converted);
                        if (! converted)
                            // Use NaN to mean unknown
                            item.remoteVersion = std::numeric_limits<float>::quiet_NaN();
                        item.remoteUrl = QUrl(pluginUrl);
                        // Resolve relative URLs using location of xml file
                        item.remoteUrl = xmlPathUrl.resolved(item.remoteUrl);
                        // visiocyte_msg("Found update information for " + item.relativeName,0);
                    }
                }
            }
        }

        node = node.nextSibling();
    }

    // Decide whether new updates are available
    bool bHaveNewUpdates = false;
    bool bHaveFutureVersions = false;
    if (getUpdatableItems().size() > 0)
        bHaveNewUpdates = true;
    if (getFutureItems().size() > 0)
        bHaveFutureVersions = true;

    // Set the update flag on updateable items,
    // so they will be set to update by default.
    std::vector<UpdateItem*> v = getUpdatableItems();
    std::vector<UpdateItem*>::iterator i;
    for (i = v.begin(); i != v.end(); i++) {
        UpdateItem& item = **i;
        item.bDoInstall = true;
    }

    if ( bHaveNewUpdates )
    {
        UpdatesAvailableDialog* availableDialog = new UpdatesAvailableDialog(guiParent);
        // Connect to version checker method
        connect(availableDialog, SIGNAL(yes_update()), this, SLOT(show_update_list()));
        availableDialog->exec();
    }
    else if ( bHaveFutureVersions ) { // You have an advanced version
        if (b_showAllMessages)
        {
            QMessageBox *upToDateBox = new QMessageBox(
                    QMessageBox::Information,
                    "Visiocyte is more than up to date",
                    "You are using a future version of Visiocyte or plugins",
                    QMessageBox::Ok,
                    guiParent);
            QPushButton* openButton = upToDateBox->addButton(tr("Open updater anyway..."), QMessageBox::ActionRole);
            upToDateBox->exec();
            if (upToDateBox->clickedButton() == openButton) {
                // Open updater anyway
                show_update_list();
            }
        }
        // qDebug("VISIOCYTE or plugin version is higher than the stable version");
    }
    else // there are no new updates
    {
        if (b_showAllMessages)
        {
            QMessageBox *upToDateBox = new QMessageBox(
                    QMessageBox::Information,
                    "Visiocyte is up to date",
                    "You are using the latest version of Visiocyte",
                    QMessageBox::Ok,
                    guiParent);
            QPushButton* openButton = upToDateBox->addButton(tr("Open updater anyway..."), QMessageBox::ActionRole);
            upToDateBox->exec();
            if (upToDateBox->clickedButton() == openButton) {
                // Open updater anyway
                show_update_list();
            }
        }
        qDebug("VISIOCYTE version is up to date");
    }

*/
}

void VISIOCYTEVersionChecker::show_update_list()
{
    UpdatesListDialog* listDialog = new UpdatesListDialog(guiParent, this);
    listDialog->exec();
    connect(listDialog, SIGNAL(update_install()), this, SLOT(install_updates()));
}

void VISIOCYTEVersionChecker::install_updates()
{
    // Connect chain of download signals
    // So updates will occur in series
    // TODO - keep track of errors that occur
    UpdateItem *previousUpdateItem = NULL;
    UpdateItem *firstUpdateItem = NULL;
    int installCount = 0;
    std::map<QString, UpdateItem*>::iterator itemIter;
    for (itemIter = updateItems.begin(); itemIter != updateItems.end(); ++itemIter)
    {
        UpdateItem *item = itemIter->second;
        if (item->bDoInstall) {
            if (!firstUpdateItem)
                firstUpdateItem = item;
            // Link update items together in a chain
            if (previousUpdateItem)
                connect(previousUpdateItem, SIGNAL(updateComplete(visiocyte::DownloadingUpdatesDialog*,bool)),
                        item, SLOT(startUpdate(visiocyte::DownloadingUpdatesDialog*)));
            installCount++;
            previousUpdateItem = itemIter->second;
        }
    }

    if ( (installCount == 0) || (! firstUpdateItem) ) {
        QMessageBox::information(guiParent,
                tr("No updates to install"),
                tr("There were no updates to install"));
        return;
    }

    connect(previousUpdateItem, SIGNAL(updateComplete(visiocyte::DownloadingUpdatesDialog*,bool)),
            this, SLOT(finishUpdates(visiocyte::DownloadingUpdatesDialog*)));

    DownloadingUpdatesDialog *progressDialog = new DownloadingUpdatesDialog(guiParent);
    // When the user clicks "Cancel", stop downloading.
    progressDialog->setModal(true);
    // progressDialog->setAutoClose(true);
    // progressDialog->setMinimumDuration(500);
    // Each item gets to increment progress value by 2
    progressDialog->progressBar->setMaximum(2 * installCount); // zero means "unknown"
    progressDialog->progressBar->setValue(0);

    qDebug() << "installing Visiocyte software updates...";

    // Kick off the first update
    firstUpdateItem->startUpdate(progressDialog);

    // progressDialog->exec();
    progressDialog->exec();
}

void VISIOCYTEVersionChecker::finishUpdates(DownloadingUpdatesDialog* progressDialog)
{
    progressDialog->close();
    QMessageBox::information(
            guiParent,
            tr("Updates were installed"),
            tr("Updates were installed.\n")
            + QString(tr("Please restart VISIOCYTE!")) );
}

void VISIOCYTEVersionChecker::populateLocalUpdateItems()
{
    // VISIOCYTE executable itself
    QString visiocytePath = QCoreApplication::applicationFilePath();
    QString relName = QFileInfo(visiocytePath).fileName();
    if (updateItems.find(relName) == updateItems.end())
        updateItems[relName] = new UpdateItem(this);
    UpdateItem& item = *(updateItems[relName]);
    item.relativeName = relName;
    item.localFile = QFileInfo(visiocytePath);
    item.localVersion = visiocyte::thisVersionOfVISIOCYTE.toFloat();

    // Scan all plugins
    QList<QDir> pluginsDirList = Visiocyte_PluginLoader::getPluginsDirList();
    foreach (QDir dir, pluginsDirList) {
        populateLocalPluginsFiles(dir);
        populateLocalPluginsDirs(dir);
    }
}

void VISIOCYTEVersionChecker::populateLocalPluginsDirs(const QDir& pluginsDir)
{
        QStringList dirList = pluginsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (QString dirName, dirList)
        {
            QDir subDir = pluginsDir;
            subDir.cd(dirName);
            populateLocalPluginsFiles(subDir);
            populateLocalPluginsDirs(subDir);
        }
}

void VISIOCYTEVersionChecker::populateLocalPluginsFiles(const QDir& pluginsDir)
{
        // Name should be relative to visiocyte executable
        QDir visiocyteDir(QCoreApplication::applicationDirPath());

        QStringList fileList = pluginsDir.entryList(QDir::Files);
        foreach (QString fileName, fileList)
        {
            QString fullpath = pluginsDir.absoluteFilePath(fileName);
			// Skip plugins with ".old" suffix
			if (fullpath.endsWith(".old")) continue;
			if (fullpath.endsWith(".new")) continue;

            QString relativePath = visiocyteDir.relativeFilePath(fullpath);
            // Can the plugin be loaded?
            QPluginLoader* loader = new QPluginLoader(fullpath);
            if (! loader) return;
            QObject *plugin = loader->instance(); //a new instance
            if (plugin)
            {
                float pluginVersion = std::numeric_limits<float>::quiet_NaN();
                // Can we determine the plugin version?
                VISIOCYTEPluginInterface2_1 *if21 =
                        dynamic_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
                VISIOCYTESingleImageInterface2_1 *sif21 =
                        dynamic_cast<VISIOCYTESingleImageInterface2_1 *>(plugin);
                if (if21) pluginVersion = if21->getPluginVersion();
                else if (sif21) pluginVersion = sif21->getPluginVersion();

                // Create entry if it is not already there
                if (updateItems.find(relativePath) == updateItems.end())
                    updateItems[relativePath] = new UpdateItem(this);
                UpdateItem& item = *(updateItems[relativePath]);
                // Update local fields
                item.relativeName = relativePath;
                item.localFile = QFileInfo(fullpath);
                item.localVersion = pluginVersion;
            }
            loader->unload();
        }
}

std::vector<UpdateItem*> VISIOCYTEVersionChecker::getUpdatableItems() {
    std::vector<UpdateItem*> answer;
    std::map<QString, UpdateItem*>::iterator itemIter;
    for (itemIter = updateItems.begin(); itemIter != updateItems.end(); ++itemIter)
    {
        UpdateItem& item = *(itemIter->second);
        if (item.remoteUrl.isEmpty()) continue;
        if (! item.remoteUrl.isValid()) continue;
        // At this point, there is a file available at the update site, presumably.
        if (! item.localFile.exists()) {
            answer.push_back(&item); // we don't have that file
            continue;
        }
        if (item.remoteVersion == item.remoteVersion) { // not NaN, so we really know the version
            if (item.localVersion == item.localVersion) {
                if (item.remoteVersion > item.localVersion) { // remote is newer
                    answer.push_back(&item);
                    continue;
                }
            } else {
                // known remote version, unknown local version
                answer.push_back(&item);
            }
        }
    }
    return answer;
}

// Items with a more recent local version than the remote version
std::vector<UpdateItem*> VISIOCYTEVersionChecker::getFutureItems() {
    std::vector<UpdateItem*> answer;
    std::map<QString, UpdateItem*>::iterator itemIter;
    for (itemIter = updateItems.begin(); itemIter != updateItems.end(); ++itemIter)
    {
        UpdateItem& item = *(itemIter->second);
        if (item.remoteUrl.isEmpty()) continue;
        if (! item.remoteUrl.isValid()) continue;
        if (! item.localFile.exists()) continue;
        if (item.localVersion == item.localVersion) { // finite local version
            // if remote version is less, or is unknown, we have a future version
            if (item.remoteVersion == item.remoteVersion) { // finite remote version
                if (item.localVersion > item.remoteVersion)
                    answer.push_back(&item);
            }
            else {
                // local finite, remote unknown => local is newer...
                answer.push_back(&item);
            }
        }
        else { // unknown local version
            continue;
        }
    }
    return answer;
}

// Label cells of software update table
class UpdateTableLabel : public QTableWidgetItem
{
public:
    UpdateTableLabel(QString str)
        : QTableWidgetItem(str)
    {
        setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    }
};

// Action cell of software update table
class UpdateTableAction : public QTableWidgetItem
{
public:
    UpdateTableAction()
        : QTableWidgetItem()
    {
        setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
    }
};

void VISIOCYTEVersionChecker::populateQTableWidget(QTableWidget& tableWidget)
{
    tableWidget.setSortingEnabled(false);

    std::vector<UpdateItem*> items = getUpdatableItems();
    tableWidget.setRowCount(items.size());
    std::vector<UpdateItem*>::const_iterator itemIter;
    int row = 0;
    for (itemIter = items.begin(); itemIter != items.end(); ++itemIter) {
        const UpdateItem& item = **itemIter;

        // First column is current local version
        QString localVersion = QString("%1").arg(item.localVersion);
        if (! item.localFile.exists()) // no local file
            localVersion = QString("None");
        else if (! (item.localVersion == item.localVersion)) // NaN => uninitialized
            localVersion = QString("Unknown");
        tableWidget.setItem(row, 0, new UpdateTableLabel(localVersion));

        // Second column is new remote version
        QString remoteVersion = QString("%1").arg(item.remoteVersion);
        if (item.remoteUrl.isEmpty())
            remoteVersion = QString("None");
        if (! (item.remoteVersion == item.remoteVersion)) // NaN => uninitialized
            remoteVersion = QString("Unknown");
        tableWidget.setItem(row, 1, new UpdateTableLabel(remoteVersion));

        // Third column is action to be taken
        QCheckBox *cb = new QCheckBox();
        tableWidget.setCellWidget(row, 2, cb);
        if (item.bDoInstall)
            cb->setCheckState( Qt::Checked );
        else
            cb->setCheckState( Qt::Unchecked );
        connect(cb, SIGNAL(stateChanged(int)), &item, SLOT(setInstall(int)));

        // Fourth column is name of plugin/component
        tableWidget.setItem(row, 3, new UpdateTableLabel(item.relativeName));

        row++;
    }

    tableWidget.setSortingEnabled(true);
}



CheckForUpdatesDialog::CheckForUpdatesDialog(QWidget* guiParent) : QDialog(guiParent)
{
    setupUi(this);

    // Rename buttons
    QPushButton* okButton = buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setText(tr("Check now"));
        connect(okButton, SIGNAL(clicked()), this, SLOT(check_now()));
    }

    // Add options button
    QPushButton* optionsButton = buttonBox->addButton(tr("Options..."), QDialogButtonBox::ActionRole);
    connect(optionsButton, SIGNAL(clicked()), this, SLOT(show_options()));
}

void CheckForUpdatesDialog::check_now()
{
    visiocyte::VISIOCYTEVersionChecker *versionChecker = new visiocyte::VISIOCYTEVersionChecker(parentWidget());
    versionChecker->checkForLatestVersion(true);
}

void CheckForUpdatesDialog::show_options() {
    UpdateOptionsDialog(this).exec();
}


UpdatesListDialog::UpdatesListDialog(QWidget* guiParent, VISIOCYTEVersionChecker *checker)
        : QDialog(guiParent), versionChecker(checker)
{
    setupUi(this);

    // Rename buttons
    QPushButton* okButton = buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setText("Update/Install");
        connect(okButton, SIGNAL(clicked()), checker, SLOT(install_updates()));
    }

    checker->populateQTableWidget(*tableWidget);
}

UpdateOptionsDialog::UpdateOptionsDialog(QWidget* guiParent)
    : QDialog(guiParent)
{
    setupUi(this);

    validator = new VersionUrlValidator(this);

    // Want to color invalid URLs red.
    blackPalette = new QPalette(lineEdit->palette());
    redPalette = new QPalette(lineEdit->palette());
    // redPalette->setColor(foregroundRole(), QColor(255, 0, 0));
    redPalette->setColor(QPalette::Text, QColor(255, 0, 0)); // red text

    // Sync with current update frequency
    QSettings settings("HHMI", "Visiocyte");
    QVariant checkIntervalVariant = settings.value("updateCheckInterval");
    if ( checkIntervalVariant.isValid() )
    {
        int checkInterval = checkIntervalVariant.toInt();
        if (checkInterval < 0)
            comboBox->setCurrentIndex(comboBox->findText(tr("never")));
        else if (checkInterval == 0)
            comboBox->setCurrentIndex(comboBox->findText(tr("every time")));
        else if (checkInterval == 86400)
            comboBox->setCurrentIndex(comboBox->findText(tr("once a day")));
        else if (checkInterval == 604800)
            comboBox->setCurrentIndex(comboBox->findText(tr("once a week")));
        else if (checkInterval == 2592000)
            comboBox->setCurrentIndex(comboBox->findText(tr("once a month")));
        else
            qDebug() << "Error: unrecognized interval";
    }

    QPushButton* openButton = buttonBox->button(QDialogButtonBox::Open);
    if (openButton) {
        visiocyte::OpenVisiocyteWebPageAction *action = new
                visiocyte::OpenVisiocyteWebPageAction(this);
        openButton->setText(tr("Open Visiocyte download page"));
        connect(openButton, SIGNAL(clicked()), action, SLOT(trigger()));
    }

    QPushButton* saveButton = buttonBox->button(QDialogButtonBox::Save);
    if (saveButton) {
        saveButton->setText(tr("Save new version xml file"));
        connect(saveButton, SIGNAL(clicked()), this, SLOT(save_xml_file()));
    }

    // Set default button to "Close"
    QPushButton* closeButton = buttonBox->button(QDialogButtonBox::Close);
    closeButton->setDefault(true);

    // QRegExp urlRegExp("^https?://\\S+/.*\\.xml$");
    // lineEdit->setValidator(new QRegExpValidator(urlRegExp, this));
    lineEdit->setValidator(validator);

    QString versionUrl = settings.value("versionUrl").toString();
    if (versionUrl.length() == 0)
        versionUrl = VISIOCYTEVersionChecker::getDefaultVISIOCYTEVersionUrl();

    lineEdit->setText(versionUrl);

    // Implement "use default version file" button
    connect(useDefaultVersionButton, SIGNAL(clicked()),
            this, SLOT(use_default_version_file()));
}

void UpdateOptionsDialog::use_default_version_file()
{
    QString defUrl = VISIOCYTEVersionChecker::getDefaultVISIOCYTEVersionUrl();
    lineEdit->setText(defUrl);
    // Use default setting in the future, as well.
	QSettings settings("HHMI", "Visiocyte");
    settings.setValue("versionUrl", defUrl);
}

void UpdateOptionsDialog::save_xml_file()
{
    visiocyte::VISIOCYTEVersionChecker *versionChecker = new visiocyte::VISIOCYTEVersionChecker(parentWidget());
    versionChecker->createVersionXml();
}

void UpdateOptionsDialog::on_comboBox_currentIndexChanged(const QString& updateFrequency)
{
    // Store update interval in the persistent cache
    qDebug() << "Changing update frequency to " << updateFrequency;
    QSettings settings("HHMI", "Visiocyte");
    if (updateFrequency == tr("never"))
        VISIOCYTEVersionChecker::never_update();
    else if (updateFrequency == tr("every time"))
        settings.setValue("updateCheckInterval", 0);
    else if (updateFrequency == tr("once a day"))
        settings.setValue("updateCheckInterval", 86400);
    else if (updateFrequency == tr("once a week"))
        settings.setValue("updateCheckInterval", 604800);
    else if (updateFrequency == tr("once a month"))
        settings.setValue("updateCheckInterval", 2592000);
    else
        qDebug() << "Error: unrecognized interval";
}

// User changed the url
void UpdateOptionsDialog::on_lineEdit_editingFinished() {
    // Save new url
    // visiocyte_msg("New version file = " + lineEdit->text(), 0);
	QSettings settings("HHMI", "Visiocyte");
    settings.setValue("versionUrl", lineEdit->text());
}

void UpdateOptionsDialog::on_lineEdit_textChanged() // user changes version xml url
{
    if (validator->validate(lineEdit->text()) == QValidator::Acceptable)
        lineEdit->setPalette(*blackPalette);
    else
        lineEdit->setPalette(*redPalette);
}


CheckingForUpdatesDialog::CheckingForUpdatesDialog(QWidget *parentObject)
        : QDialog(parentObject), bWasCanceled(false)
{
    setupUi(this);
    connect(this, SIGNAL(rejected()), this, SLOT(canceled()));
}


UpdatesAvailableDialog::UpdatesAvailableDialog(QWidget *parent)
        : QMessageBox(parent)
{
    // Use VISIOCYTE application icon
    QIcon appIcon(":/pic/visiocyteIcon128.png");
    QPixmap pIconPixmap = appIcon.pixmap(75,75);
    setIconPixmap(pIconPixmap);
    setText(tr("There are updates available."));
    setInformativeText("Do you want to start the Visiocyte updater now?");
    // setDetailedText("The VISIOCYTE team periodically makes software improvements. "
    //         "Click 'Yes' to install recent improvements now.");
    setWindowTitle(tr("Visiocyte software update check"));
    setSizeGripEnabled(false);

    QPushButton *noButton = addButton( tr("Remind me later"), QMessageBox::RejectRole);
    connect(noButton, SIGNAL(clicked()), this, SLOT(remind_me_later()));

    QPushButton *neverButton = addButton( tr("Never"), QMessageBox::DestructiveRole);
    connect(neverButton, SIGNAL(clicked()), this, SLOT(never_update()));

    yesButton = addButton( tr("Yes, update"), QMessageBox::AcceptRole);
    connect(yesButton, SIGNAL(clicked()), this, SIGNAL(yes_update()));
}

void UpdatesAvailableDialog::never_update()
{
    VISIOCYTEVersionChecker::never_update();
}

void UpdatesAvailableDialog::remind_me_later()
{
    QSettings settings("HHMI", "Visiocyte");
    int interval = settings.value("updateCheckInterval").toInt();
    if (interval < 0) // if set to never
        // run again when user starts VISIOCYTE, but not for at least 5 minutes.
        settings.setValue("updateCheckInterval", 300);
}


DownloadingUpdatesDialog::DownloadingUpdatesDialog(QWidget *parentObject)
        : QDialog(parentObject), bWasCanceled(false)
{
    setupUi(this);
    // Use VISIOCYTE application icon
    QIcon appIcon(":/pic/visiocyteIcon128.png");
    QPixmap pIconPixmap = appIcon.pixmap(75,75);
    iconLabel->setPixmap(pIconPixmap);
    connect(this, SIGNAL(rejected()), this, SLOT(canceled()));

    setModal(true);
}


} // namespace visiocyte