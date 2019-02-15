# #####################################################################
# Updated: 060924 visiocyte v0.12
# Updated: 080131: change from Qt 4.1.4 to 4.3.3 and then back to 4.1.4, as 4.3.3 has a crash on the program exit. sent bug report to Qt already.
# Updated: 080302. Use the cpp version of mg_image and mg_utilities. Also include the basic_memory functions
# Updated: 080314. Add the straightening codes.
# Updated: 080318. Add -lnewmat
# Updated: 080321. Add rotate_image.cpp
# Updated: 080323. Add dialog_rotate.xxx
# Updated: 080326. Add laff
# Updated: 080401: Add img_definition.h
# Updated: 080403: Add spline_cubic.h and .cpp
# Updated: 080411: add thres_regist.h
# Updated: 080414: add volimg_proc.h
# Updated: 080422: try to add Zhao Ting's corr functions
# 080429: try Qt 4.3.4
# 080611: add histeq.h
# 080720: qt 4.4.0
# 080723: add visiocyte_compile_constraints.h
# 080730: add compiling option to specify if display warning etc
# 080806: add viz codes
# 080817: change laff to jba
# 080818: change JBA dir structure
# 080822: add the lobeseg function
# 080823: add the cellseg functions
# 080824: add colormap
# 081016: add load ano file
# 081111: add convert_type2uint8.cpp
# 081115: add visiocyte_version_info.h/cpp
# 081123: add autotab.h, atlas_viewer.h/cpp
# 081127: add jba_match_landmarks.cpp and remove laff_features.cpp
# 081207: add my4dimage.cpp
# 081208: add landmark property dialog files
# 090108: add GMM fit
# 090118: add curve_trace_dialog.ui
# 090121: add the brainseg deformable model
# 090206: add v_neuronswc.cpp
# 090208: add dij_bgl.cpp
# 090219: add surfaceobj dialog and ui
# 090220: add surfaceobj_geometry_dialog.ui
# 090306: add some neuron basic operation files
# 090310: add the neuron_sim_score.h/cpp
# 090318: add jba_affine_xform.h/cpp
# 090426: add visiocyte_global_preference.ui
# 090426: add template_matching_seg.h/cpp
# 090428: add template_matching_cellseg.ui
# 090428: add template_matching_cellseg_dialog.h
# 090501: add visiocyte_global_preference_dialog.h
# 090507: add import image list dialog.ui and .h
# 090512: add dijkstra algorithm
# 090516: add neuron_tracing subproject and head files, move file locations
# 090516: add visiocyte_message
# 090516: add filelist importer and related h/cpp files
# 090517: add linker loader for APO files and also rename them with vano_prefix
# 090517: add point cloud linker loader and related h/c++ files
# 090518: add point cloud atlas h/cpp io functions
# 090519: add compute_win_pca.h
# 090523: add basic_surf_objs.cpp
# 090605: move load_ano_file.h/cpp to basic_c_fun, merge the o_objects.h with basic_surf_objs.h
# 090605: merge load_ano_file.h into basic_surf_objs.h, and change name of load_ano_file.cpp to io_ano_file.cpp
# 090609: update the pointcloud_atlas_io.h/cpp location
# 090629: modified for macx to make free either in ../ or in ../visiocyte/, it means you can copy it to ../ then qmake it from ../
# 090705: add ../neuron_editing/apo_xform.h/cpp
# 090710: add the kill_wrongmatchcode
# 090718: add histogramsimple.hcpp
# 090812: add visiocyte plugins
# 090819: separate the import_tiff_series.cpp from visiocyte_core.cpp
# 091114: add visiocyteimg_proc_neuron.cpp
# 100119: remove     ../basic_c_fun/io_ano_file.cpp
# 100601: add visiocyte_global_proference.h
# 100801: separate module_flseg.cpp
# 100801: separate module_jba.cpp
# 100801: separate module_bdb.cpp
# 100802: remove the three modules
# 100807: remove some redundant libs
# 100810: add xformwidget.h extracted from visiocyte_core.h
# 100820: add restriction of the win32 to use MYLIB_TIFF
# 101008: add the imaging module
# 101008: (CMB) add web url download
# 101019: (CMB) add dialog_url_entry.ui
# 101025: (CMB) add dialog_update_visiocyte.ui
# 101119: add bundle test for mac
# 101129: (CMB) add link to CoreServices framework on Snow Leopard
# 101201: (CMB) add dialog_update_list.ui
# 101213: (CMB) add more update dialogs
# 110806: (Hang) add ../custom_toolbar/visiocyte_custom_toolbar.h/.cpp
# 110906: By Hanchuan Peng, produce a visiocyte_essential.pro for better maintainance of the project codebase for additional modules
# 150506: by PHC, add asc_to_swc
# 150507: by PHC, add nrrd reading support
# 150510: by PHC, Qt5 with success
# ######################################################################

TEMPLATE = app
TARGET +=
DEPENDPATH += . visiocyte
INCLUDEPATH += .


#DEFINES += USE_Qt5  #this might be invoked from commandline like "~/Qt5.4.1/5.4/clang_64/bin/qmake DEFINES+=USE_Qt5 visiocyte64.pro", however it seems there is some bug


# commented the -app_bundle as on Mac the not-automatically closed terminal is quite annoying!
# macx: CONFIG-=app_bundle #by PHC, 101119

# cross-OS-platform, cross-Qt-version
QT_DIR = $$[QT_INSTALL_PREFIX]
LOCAL_DIR = ../common_lib/ 				# unix-liked platform: macx, unix, win32-msys-mingw

MINGW_DIR = /mingw # platform: win32-msys-mingw
win32 { # platform: win32-command-mingw
	MINGW_DIR = c:/mingw
	LOCAL_DIR = ../common_lib/      # c:/msys/local
	CONFIG = $$unique(CONFIG)
	CONFIG -= debug # for Qt-win32 which only has release install(no debug)
    CONFIG += console
LIBS += -L$$MINGW_DIR/lib \
	-L$$LOCAL_DIR/lib_win32
    DEFINES += __ALLOW_VR_FUNCS__

}


INCLUDEPATH += $$LOCAL_DIR/include #./basic_c_fun
LIBS += -L$$LOCAL_DIR/lib

USE_Qt5 {
  INCLUDEPATH += $$QT_DIR/lib/QtConcurrent.framework/Versions/5/Headers  # for QtConcurrent, by PHC 2015May
  #SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets
  SHARED_FOLDER = ./painting/shared/ # for arthurwidgets
  include($$SHARED_FOLDER/shared.pri)
  INCLUDEPATH += $$SHARED_FOLDER
  LIBS += -L$$SHARED_FOLDER
} else {
  SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets
  include($$SHARED_FOLDER/shared.pri)
  INCLUDEPATH += $$SHARED_FOLDER
  LIBS += -L$$SHARED_FOLDER
}

win32:LIBS += -L$$SHARED_FOLDER/release # for Qt-win32 which only has release install(no debug)

macx {
    # Mac possible location of arthurwidgets.h with official Qt 4.7 installer
    QTINST_SHARED_FOLDER = /Developer/Examples/Qt/Demos/shared
    include($$QTINST_SHARED_FOLDER/shared.pri)
    INCLUDEPATH += $$QTINST_SHARED_FOLDER
    LIBS += -L$$QTINST_SHARED_FOLDER
    # For faster neuron toggling. CMB 20 June 2011
    QMAKE_CXXFLAGS+=-O3
    CXXFLAGS+=-O3
    # On mountain lion use clang compiler, not broken llvm/gcc compiler
#    QMAKE_CXX=c++  #commented this on 2013-08-09 by HP as this sentence will make the building on Qt 4.7 and OSX 10.7 fail.
                    #Indeed it seems the compiler is better to be set by a symbolic link on the local machine, instead of in
                    #the generic pro file here. See the Visiocyte Google Code page http://code.google.com/p/visiocyte/wiki/BuildVisiocyte
                    #at the section "What you need: Qt & C++ compilers"

#    for Mac OS X 10.9
#    QMAKE_CXXFLAGS += " -stdlib=libstdc++"
#    QMAKE_LFLAGS += " -stdlib=libstdc++"
}

unix:!macx {
    # Ubuntu linux possible location of arthurwidgets.h with official Qt install
    # QTINST_SHARED_FOLDER = /usr/lib/qt4/demos/shared
    QTINST_SHARED_FOLDER = /usr/lib64/qt4/demos/shared
    include($$QTINST_SHARED_FOLDER/shared.pri)
    INCLUDEPATH += $$QTINST_SHARED_FOLDER
    LIBS += -L$$QTINST_SHARED_FOLDER
}

# the following trick was figured out by Ruan Zongcai
CONFIG += warn_off  # only work for complier
#CONFIG += debug  # Fabrice and Luis tracking a bug. disable by PHC, 100819
#macx: QMAKE_LFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden # turn off this type warnings
#macx: QMAKE_CXXFLAGS += -gdwarf-2 # turn off visibility warnings
# need Qt 4.5.0 above and reCreate Makefile, this will be automatic.
CONFIG += thread

#CONFIG += console

# Input
HEADERS += ../basic_c_fun/mg_utilities.h \
    ../basic_c_fun/mg_image_lib.h \
    ../basic_c_fun/basic_memory.h \
    ../basic_c_fun/stackutil.h \
    ../basic_c_fun/img_definition.h \
    ../basic_c_fun/volimg_proc_declare.h \
    ../basic_c_fun/volimg_proc.h \
    ../basic_c_fun/visiocyte_message.h \
    ../basic_c_fun/color_xyz.h \
    ../basic_c_fun/basic_surf_objs.h \
    ../basic_c_fun/basic_4dimage.h \
    ../basic_c_fun/basic_landmark.h \
    ../basic_c_fun/visiocyte_interface.h \
    ../basic_c_fun/visiocyte_global_preference.h \
    ../basic_c_fun/customary_structs/visiocyte_imaging_para.h \
    ../basic_c_fun/basic_thread.h \
    ../basic_c_fun/basic_view3d.h \
    ../plugin_loader/pluginDialog.h \
    ../plugin_loader/visiocyte_plugin_loader.h \
    ../graph/graph.h \
    ../graph/graph_basic.h \
    ../graph/dijk.h \
    ../gmm/fit_gmm.h \
    ../neuron_editing/apo_xforms.h \
    ../neuron_editing/neuron_xforms.h \
    ../neuron_editing/neuron_sim_scores.h \
    ../neuron_editing/v_neuronswc.h \
    ../neuron_editing/neuron_format_converter.h \
    ../neuron_tracing/neuron_tracing.h \
    ../3drenderer/visiocyter_mainwindow.h \
    ../3drenderer/visiocyter_glwidget.h \
    ../3drenderer/qtr_widget.h \
    ../3drenderer/GLee_r.h \
    ../3drenderer/renderer.h \
    ../3drenderer/renderer_gl1.h \
    ../3drenderer/visiocyter_surfaceDialog.h \
    ../3drenderer/ItemEditor.h \
    ../3drenderer/renderer_gl2.h \
    ../3drenderer/visiocyter_colormapDialog.h \
    ../3drenderer/gradients.h \
 \ #    ../3drenderer/visiocyte_hoverpoints.h \
    ../3drenderer/barFigureDialog.h \
    ../3drenderer/line_box_intersection_check.h \
    ../neuron_tracing/heap.h \
    ../neuron_tracing/fastmarching_linker.h \
	../imaging/visiocyte_imaging.h \
    ../neuron_toolbox/visiocyte_neurontoolbox.h \
	../multithreadimageIO/visiocyte_multithreadimageIO.h \
    visiocyte_global_preference_dialog.h \
    visiocyte_compile_constraints.h \
    visiocyte_version_info.h \
    visiocyte_application.h \
    colormap.h \
    ChannelTable.h \
    rotate_image.h \
    dialog_rotate.h \
    dialog_curve_trace_para.h \
    template_matching_cellseg_dialog.h \
    opt_rotate.h \
    landmark_property_dialog.h \
    dialog_keypoint_features.h \
    histogramsimple.h \
    compute_win_pca.h \
    compute_win_diff.h \
    surfaceobj_annotation_dialog.h \
    surfaceobj_geometry_dialog.h \
    import_filelist_dialog.h \
    import_images_tool_dialog.h \
    vano_linker_loader_dialog.h \
    dialog_imagecrop_bbox.h \
    dialog_imageresample.h \
    dialog_maskroi.h \
    atlas_viewer.h \
    visiocyte_core.h \
    xformwidget.h \
    mainwindow.h \
    mapview.h \
    DownloadManager.h \
    visiocyte_actions.h \
    visiocyte_commandlineparser.h \
    pluginfunchandler.h \
    vr_visiocyte_call.h \
    ../worm_straighten_c/bdb_minus.h \
    ../worm_straighten_c/mst_prim_c.h \
    ../worm_straighten_c/bfs.h \
    ../worm_straighten_c/spline_cubic.h \
    ../cellseg/template_matching_seg.h \
    ../jba/c++/jba_mainfunc.h \
    ../jba/c++/jba_match_landmarks.h \
    ../jba/c++/wkernel.h \
    ../jba/c++/histeq.h \
    ../jba/c++/convert_type2uint8.h \
    ../jba/c++/jba_affine_xform.h \
    ../jba/c++/remove_nonaffine_points.h \
    ../custom_toolbar/visiocyte_custom_toolbar.h \
    ../io/io_bioformats.h \
    ../io/asc_to_swc.h \
    ../io/visiocyte_nrrd.h \
    ../terafly/src/presentation/theader.h \
    CustomDefine.h
#    ./painting/shared/arthurstyle.h \
#    ./painting/shared/arthurwidgets.h


unix:HEADERS += ../basic_c_fun/imageio_mylib.h
#macx:HEADERS += ../basic_c_fun/imageio_mylib.h

SOURCES += ../basic_c_fun/mg_utilities.cpp \
    ../basic_c_fun/mg_image_lib.cpp \
    ../basic_c_fun/stackutil.cpp \
    ../basic_c_fun/basic_memory.cpp \
    ../basic_c_fun/visiocyte_message.cpp \
    ../basic_c_fun/basic_surf_objs.cpp \
    ../basic_c_fun/basic_4dimage.cpp \
    ../basic_c_fun/basic_4dimage_create.cpp \
    ../plugin_loader/visiocyte_plugin_loader.cpp \
    ../plugin_loader/pluginDialog.cpp \
    ../graph/dijk.cpp \
    ../neuron_editing/apo_xforms.cpp \
    ../neuron_editing/neuron_xforms.cpp \
    ../neuron_editing/neuron_sim_scores.cpp \
    ../neuron_editing/v_neuronswc.cpp \
    ../neuron_editing/neuron_format_converter.cpp \
    ../neuron_editing/global_feature_compute.cpp \
    ../neuron_tracing/dij_bgl.cpp \
    ../gmm/fit_gmm.cpp \
    ../cellseg/template_matching_seg.cpp \
    ../3drenderer/visiocyter_mainwindow.cpp \
    ../3drenderer/visiocyter_glwidget.cpp \
    ../3drenderer/GLee_r.c \
    ../3drenderer/renderer.cpp \
    ../3drenderer/renderer_tex.cpp \
    ../3drenderer/renderer_obj.cpp \
    ../3drenderer/renderer_hit.cpp \
    ../3drenderer/nstroke.cpp \
    ../3drenderer/nstroke_tracing.cpp \
    ../3drenderer/renderer_labelfield.cpp \
    ../3drenderer/visiocyter_surfaceDialog.cpp \
    ../3drenderer/ItemEditor.cpp \
    ../3drenderer/renderer_gl2.cpp \
    ../3drenderer/visiocyter_colormapDialog.cpp \
    ../3drenderer/gradients.cpp \
    ../3drenderer/visiocyter_control_signal.cpp \
 \ #    ../3drenderer/visiocyte_hoverpoints.cpp \
    ../3drenderer/barFigureDialog.cpp \
        ../imaging/visiocyte_imaging.cpp \
    ../neuron_toolbox/visiocyte_neurontoolbox.cpp \
	../multithreadimageIO/visiocyte_multithreadimageIO.cpp \
    visiocyte_version_info.cpp \
    visiocyte_application.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_interface.cpp \
    mapview.cpp \
    visiocyte_core.cpp \
    visiocyteimgproc_entry.cpp \
    my4dimage.cpp \
    visiocyteimg_proc_neuron.cpp \
    colormap.cpp \
    ChannelTable.cpp \
    rotate_image.cpp \
    dialog_rotate.cpp \
    landmark_property_dialog.cpp \
    dialog_keypoint_features.cpp \
    histogramsimple.cpp \
    surfaceobj_geometry_dialog.cpp \
    import_filelistname.cpp \
    import_tiffseries.cpp \
    vano_linker_loader_dialog.cpp \
    atlas_viewer.cpp \
    DownloadManager.cpp \
    visiocyte_actions.cpp \
    visiocyte_commandlineparser.cpp \
    pluginfunchandler.cpp \
    vr_visiocyte_call.cpp \
    ../worm_straighten_c/bdb_minus.cpp \
    ../worm_straighten_c/mst_prim_c.cpp \
    ../worm_straighten_c/bfs_1root.cpp \
    ../worm_straighten_c/spline_cubic.cpp \
    ../jba/c++/histeq.cpp \
    ../custom_toolbar/visiocyte_custom_toolbar.cpp \
    ../io/io_bioformats.cpp \
    ../io/asc_to_swc.cpp \
    ../io/visiocyte_nrrd.cpp \
    ../3drenderer/glsl_r.cpp

#    ./painting/shared/arthurstyle.cpp \
#    ./painting/shared/arthurwidgets.cpp

unix:SOURCES += ../basic_c_fun/imageio_mylib.cpp
#macx:SOURCES += ../basic_c_fun/imageio_mylib.cpp

win32 {
INCLUDEPATH += ..\common_lib\include\SDL/ #for VR, by PHC 20170615
INCLUDEPATH += ..\common_lib\include\glew/  #for VR, by PHC 20170615
INCLUDEPATH += ..\common_lib\include\openvr/   #for VR, by PHC 20170615

HEADERS += \
    ../vrrenderer/visiocyter_gl_vr.h \
    ../vrrenderer/Cylinder.h \
    ../vrrenderer/Sphere.h \
    ../vrrenderer/Matrices.h \
    ../vrrenderer/lodepng.h \
    ../vrrenderer/RenderableObject.h \
    ../vrrenderer/VRFinger.h \
    ../vrrenderer/VisiocyteR_Communicator.h \
    ../vrrenderer/VR_MainWindow.h 

SOURCES += \
    ../vrrenderer/visiocyter_gl_vr.cpp \
    ../vrrenderer/Cylinder.cpp \
    ../vrrenderer/Sphere.cpp \
    ../vrrenderer/Matrices.cpp \
    ../vrrenderer/lodepng.cpp \
    ../vrrenderer/RenderableObject.cpp \
    ../vrrenderer/VRFinger.cpp \
    ../vrrenderer/VisiocyteR_Communicator.cpp \
    ../vrrenderer/VR_MainWindow.cpp
}

FORMS += landmark_property.ui \
    surface_obj_annotation.ui \
    surfaceobj_geometry_dialog.ui \
    dialog_curve_trace.ui \
    template_matching_cellseg.ui \
    visiocyte_global_preference.ui \
    import_filelist_dialog.ui \
    import_images_tool.ui \
    dialog_vano_linkerloader.ui \
    dialog_pointcloudatlas_linkerloader.ui \
    dialog_imagecrop_bbox.ui \
    dialog_imageresample.ui \
    dialog_maskroi.ui \
    dialog_keypoint_features.ui \
    dialog_url_entry.ui \
    dialog_update_visiocyte.ui \
    dialog_update_list.ui \
    dialog_update_options.ui \
    dialog_update_downloading.ui \
    dialog_update_checking.ui



RESOURCES += visiocyte.qrc
RESOURCES += ../3drenderer/3drenderer.qrc
QT += opengl
QT += network
QT += xml svg

LIBS += -L../jba/c++

unix:!macx {
  LIBS += -L../common_lib/lib
  LIBS += -lm -lvisiocytetiff
  LIBS += -lvisiocytenewmat
#unix:LIBS += -L/usr/lib/qt4/demos/shared -ldemo_shared
  LIBS += -L../common_lib/src_packages/mylib_tiff -lmylib
  LIBS += -L../common_lib/lib_unix64 -lteem  -lbz2 -lz  -lGLU #for nrrd support
}

#added 20140324 to cope with centos 64bit GL library issue. by HP
#add -lglut -lGLU to fix the GL referencing issue on Ubuntu, otherwise it complains 
unix!macx:LIBS += -lglut -lGLU

macx:LIBS += -L../common_lib/lib_mac32
macx:LIBS += -lm -lvisiocytetiff -lvisiocytenewmat
#    -framework GLUT
macx:LIBS += -L../common_lib/src_packages/mylib_tiff -lmylib
macx:LIBS += -L../common_lib/lib_mac64 -lteem  -lbz2 -lz  #for nrrd support
# CMB Nov 29 2010 Snow leopard GLee_r.o requires CoreServices framework
macx:LIBS += -framework CoreServices

# @ADDED by Alessandro on 2015-05-09. Method to get the path-based URL from the file-based URL
#CONFIG += dragdropfix
macx:dragdropfix{
    DEFINES += _ENABLE_MACX_DRAG_DROP_FIX_
    OBJECTIVE_SOURCES += yosemiteFileURLfix.mm
    QMAKE_LFLAGS += -F /System/Library/Frameworks/Foundation.framework/
    LIBS += -framework Foundation
}


# NOT REALLY USED, LIBS are replaced by visiocyte_msvc.pro
win32:LIBS += -lm -lvisiocytetiff \
    -lvisiocytenewmat
win32:LIBS += -L../common_lib/winlib64 -lteem  -lbz2 -lz  #for nrrd support
#    -lglut32 # win32-mingw, on unix link libglut.a
#win32:LIBS += -L../common_lib/src_packages/mylib_tiff -lmylib	#for win32 disable using MYLIB


INCLUDEPATH += ../common_lib/include
DEFINES *= TEEM_STATIC
QMAKE_CXXFLAGS += -DTEEM_STATIC

#removed LIBS+=./??? for Eclipse IDE using customized Build-command or Make-target instead, by RZC 20110709
INCLUDEPATH = $$unique(INCLUDEPATH)
#LIBS = $$unique(LIBS) # @FIXED by Alessandro on 2015-05-11. Proven buggy on Qt 4.7.1/MacOSX10.10 since it removed some -framework from LIBS.
# CONFIG = $$unique(CONFIG) # this only DOESN'T work on macx, very strange, by RZC 20080923
message(CONFIG=$$CONFIG)