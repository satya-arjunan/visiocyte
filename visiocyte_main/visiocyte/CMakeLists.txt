set(VISIOCYTE_SRCS
  # main.cpp
  dialog_rotate.cpp
  # dialog_watershed_para.cpp
  idrawmain.cpp
  import_filelistname.cpp
  import_tiffseries.cpp
  # dialog_pointcloudatlas_linkerloader.cpp
  glwidget.cpp # duplicate symbol in arthurwidget.cpp
  histogramsimple.cpp
  mdichild.cpp
  my4dimage.cpp
  rotate_image.cpp
  visiocyteimgproc_entry.cpp
  visiocyteimg_proc_neuron.cpp
  # vano_linker_loader_dialog.cpp
  atlas_viewer.cpp
  # atlas_window.cpp   looks for image_window.h : non-existing file : FIXME
  colormap.cpp
  mainwindow.cpp
  ../custom_toolbar/visiocyte_custom_toolbar.cpp
  mainwindow_interface.cpp
  visiocyte_core.cpp
  dialog_keypoint_features.cpp
  landmark_property_dialog.cpp
  DownloadManager.cpp
  CommandManager.cpp
  visiocyte_application.cpp
  ../cell_counter/CellCounter3D.cpp
  visiocyte_commandlineparser.cpp
  pluginfunchandler.cpp
  ChannelTable.cpp
  mapview.cpp
  # ../neuron_toolbox/visiocyte_neurontoolbox.cpp
  ../basic_c_fun/basic_4dimage_create.cpp
  ../neuron_annotator/analysis/AlignerUtils.cpp
  ../io/io_bioformats.cpp
  visiocyte_actions.cpp
  )

set(ARTHUR_SRCS "")

set(VISIOCYTE_RCCS
    visiocyte.qrc
    ../3drenderer/3drenderer.qrc
    ../terafly/icons.qrc
    ../neuron_annotator/resources.qrc)

set(VISIOCYTE_UI_SRCS
    visiocyte_global_preference.ui
    template_matching_cellseg.ui
    landmark_property.ui
    surface_obj_annotation.ui
    surfaceobj_geometry_dialog.ui
    dialog_update_visiocyte.ui
    dialog_update_list.ui
    dialog_update_options.ui
    dialog_update_downloading.ui
    dialog_update_checking.ui
    dialog_curve_trace.ui
    dialog_imagecrop_bbox.ui
    dialog_imageresample.ui
    dialog_keypoint_features.ui
    dialog_maskroi.ui
    # dialog_pointcloudatlas_linkerloader.ui
    # dialog_vano_linkerloader.ui
    # FL_levelsetSegPara.ui
    # FL_watershedSegPara.ui
    import_filelist_dialog.ui
    import_images_tool.ui
    dialog_url_entry.ui
)

set(VISIOCYTE_MOC_SRCS
    atlas_viewer.h
    dialog_rotate.h
    # dialog_watershed_para.h
    dialog_imagecrop_bbox.h
    dialog_maskroi.h
    dialog_imageresample.h
    dialog_curve_trace_para.h
    dialog_keypoint_features.h
    DownloadManager.h
    glwidget.h
    idrawmain.h
    import_filelist_dialog.h
    import_images_tool_dialog.h
    landmark_property_dialog.h
    mdichild.h
    mainwindow.h
    xformwidget.h
    visiocyte_core.h
    visiocyte_global_preference_dialog.h
    visiocyte_version_info.h
    visiocyte_actions.h
    visiocyte_application.h
    ChannelTable.h
    ../custom_toolbar/visiocyte_custom_toolbar.h
  )

if(Qt5Core_FOUND)
  set(ARTHUR_MOC_HEADERS
      ${arthurwidgets_path}/arthurwidgets.h
      ${arthurwidgets_path}/arthurstyle.h
  )
  set(ARTHUR_SRCS
      ${arthurwidgets_path}/arthurwidgets.cpp
      ${arthurwidgets_path}/arthurstyle.cpp
  )
endif()

if(NOT Qt5Core_FOUND)
  QT4_WRAP_UI( QT_UI_SRCS ${VISIOCYTE_UI_SRCS} )
  QT4_WRAP_CPP(QT_MOC_SRCS ${VISIOCYTE_MOC_SRCS})
  QT4_ADD_RESOURCES(VISIOCYTE_RCC_SRCS ${VISIOCYTE_RCCS})
else()
  QT5_WRAP_UI( QT_UI_SRCS ${VISIOCYTE_UI_SRCS} )
  QT5_WRAP_CPP(QT_MOC_SRCS ${VISIOCYTE_MOC_SRCS})
  QT5_WRAP_CPP(ARTHUR_MOC_SRCS ${ARTHUR_MOC_HEADERS})
  QT5_ADD_RESOURCES(VISIOCYTE_RCC_SRCS ${VISIOCYTE_RCCS})
endif()

include_directories( BEFORE
  "${CMAKE_CURRENT_BINARY_DIR}/../visiocytebase"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}/../neuron_annotator"
  ../basic_c_fun
  ../basic_c_fun/customary_structs
  ../3drenderer
  ../neuron_annotator
  ../mozak/terafly/src/core/imagemanager
  ../mozak/terafly/src/presentation
  ../mozak/m_terafly/src/control
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )

if(Qt5Core_FOUND)
  add_library(arthurwidgets ${ARTHUR_MOC_SRCS} ${ARTHUR_SRCS} )
endif()

add_library(visiocytebase ${QT_UI_SRCS} ${QT_MOC_SRCS})

target_link_libraries( visiocytebase visiocytebase1 jba newmat11 multithreadimageIO ${QT_LIBRARIES} )


### boost
SET(Boost_ADDITIONAL_VERSIONS "1.46.0" "1.57.0" "1.56.1" "1.56.0" "1.54" "1.54.0")
find_package(Boost COMPONENTS system regex)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIRS})
endif()



##########################
# NRRD IO support
include (ExternalProject)

add_library(visiocytebase2 ${VISIOCYTE_SRCS})

add_dependencies(visiocytebase2 visiocytebase)
add_dependencies(visiocytebase2 NeuronAnnotatorLib)
add_dependencies(visiocytebase NeuronAnnotatorLib)

if (WIN32) # Windows MSYS build
  # All this MYS stuff is just to compile in a program icon for windows
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(CMAKE_RC_COMPILE_OBJECT
    "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
  add_executable( visiocyte # *not* WIN32, which causes MSVC to call WinMain() instead of main()
      main.cpp
      ${VISIOCYTE_RCC_SRCS}
      visiocyte_win.rc)
else()

  add_executable( visiocyte
      main.cpp
      ${VISIOCYTE_RCC_SRCS})
endif()

if(VISIOCYTE_PACKAGE_AS_NEURON_ANNOTATOR)
    set_target_properties(visiocyte PROPERTIES
        OUTPUT_NAME NeuronAnnotator)
else()
    set_target_properties(visiocyte PROPERTIES
        OUTPUT_NAME visiocyte)
endif()

target_link_libraries( visiocyte
  visiocytebase2
  visiocytebase
  VISIOCYTEInterface
  visiocyte_plugin_loader
  3drenderer
  neuron_tracing
  worm_straighten_c
  terafly
  # mylib_tiff
  cellseg
  ${OPENGL_glu_LIBRARY}
  )

if(MSVC)
  target_link_libraries(visiocyte ${TIFF_LIBRARY})
else()
  target_link_libraries(visiocyte mylib_tiff)
endif()
add_dependencies(visiocyte visiocytebase2)


if(UNIX)
  find_package(ZLIB)
  find_package(JPEG)
  mark_as_advanced(CLEAR ZLIB_LIBRARY JPEG_LIBRARY)
  target_link_libraries(visiocyte ${ZLIB_LIBRARY} -lz) #${JPEG_LIBRARY})
endif()

# CMB 11-Oct-2010
if(NOT Qt5Core_FOUND)
  find_package(HDF5)
  target_link_libraries(visiocyte ${QT4_DEMOS_LIBRARY})
  target_link_libraries(visiocyte ${QT_QTNETWORK_LIBRARY})
  target_link_libraries(visiocyte ${QT_QTGUI_LIBRARY})
  target_link_libraries(visiocyte ${QT_QTCORE_LIBRARY})
  target_link_libraries(visiocyte ${ZLIB_LIBRARY})
  target_link_libraries(visiocyte ${HDF5_LIBRARIES})
else()
  target_link_libraries(visiocyte arthurwidgets)
  target_link_libraries(visiocyte Qt5::Widgets Qt5::Core Qt5::Network Qt5::Xml
                            Qt5::OpenGL Qt5::Concurrent)
endif()

if(APPLE)
    # There are two ways to build VISIOCYTE on Mac.
    # Either as an application bundle in directory "visiocyte.app",
    # or as a command line tool in directory "visiocyte".
    # Redefine VISIOCYTE_MAC_CREATE_BUNDLE variable, in case this is not built
    # from very top trunk directory.
    set(VISIOCYTE_MAC_CREATE_BUNDLE TRUE CACHE BOOL
        "Create VISIOCYTE.app application bundle, instead of visiocyte command line tool")
    if (VISIOCYTE_MAC_CREATE_BUNDLE)
        # Build app bundle in place at build time
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/visiocyte")
        if(VISIOCYTE_PACKAGE_AS_NEURON_ANNOTATOR)
            set(BUNDLE_APP_NAME NeuronAnnotator.app)
        else()
            set(BUNDLE_APP_NAME Visiocyte.app)
        endif()
        if(VISIOCYTE_STAGING_BASE_DIR)
            set(BUNDLE_BUILD_DIR "${VISIOCYTE_STAGING_BASE_DIR}/${BUNDLE_APP_NAME}" CACHE PATH "Where to build Mac OS X application bundle" FORCE)
        else()
            set(BUNDLE_BUILD_DIR "${CMAKE_BINARY_DIR}/visiocyte/${BUNDLE_APP_NAME}" CACHE PATH "Where to build Mac OS X application bundle" FORCE)
        endif()
        set(VISIOCYTE_BUILD_BINARY_DIR "${BUNDLE_BUILD_DIR}/Contents/MacOS" CACHE INTERNAL "" FORCE)
        # file(REMOVE_RECURSE "${BUNDLE_BUILD_DIR}/Contents/Frameworks") # Dangerous...
        # file(REMOVE_RECURSE "${BUNDLE_BUILD_DIR}/Contents/Plugins") # Dangerous...
        file(MAKE_DIRECTORY "${BUNDLE_BUILD_DIR}"
                            "${BUNDLE_BUILD_DIR}/Contents"
                            "${BUNDLE_BUILD_DIR}/Contents/MacOS"
                            "${BUNDLE_BUILD_DIR}/Contents/Resources"
        )
        # MACOSX_BUNDLE_WHATEVER parameters must be set before configure_file statement on Info.plist is called
        # MACOSX_BUNDLE_EXECUTABLE_NAME should match CPACK_BUNDLE_NAME
        # Launch script is called visiocyte_script (not just 'visiocyte'), because...
        # ...actual binary must be called "visiocyte", so macdeployqt tool can find it.
        set(MACOSX_BUNDLE_EXECUTABLE_NAME visiocyte_script)
        set(MACOSX_BUNDLE_INFO_STRING "visiocyte")
        set(MACOSX_BUNDLE_ICON_FILE "visiocyte.icns")
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.hhmi.janelia.visiocyte")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING ${VISIOCYTE_VERSION})
        set(MACOSX_BUNDLE_BUNDLE_NAME visiocyte)
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VISIOCYTE_VERSION})
        set(MACOSX_BUNDLE_BUNDLE_VERSION ${PACKAGE_VERSION})
        set(MACOSX_BUNDLE_COPYRIGHT "(c) 2006-2012 Hanchuan Peng")
        configure_file(
          ${CMAKE_CURRENT_SOURCE_DIR}/../CMake/package/MacOSXBundleInfo.plist.in
          ${BUNDLE_BUILD_DIR}/Contents/Info.plist
          @ONLY
        )
        if(VISIOCYTE_PACKAGE_AS_NEURON_ANNOTATOR)
            configure_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/../CMake/package/neuron_annotator_mac.sh"
                "${BUNDLE_BUILD_DIR}/Contents/MacOS/${MACOSX_BUNDLE_EXECUTABLE_NAME}"
                COPYONLY)
        else()
            configure_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/../CMake/package/visiocyte_mac.sh"
                "${BUNDLE_BUILD_DIR}/Contents/MacOS/${MACOSX_BUNDLE_EXECUTABLE_NAME}"
                COPYONLY)
        endif()
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/../visiocyte/visiocyte.icns"
            "${BUNDLE_BUILD_DIR}/Contents/Resources/visiocyte.icns"
            COPYONLY)
        install(DIRECTORY ${BUNDLE_BUILD_DIR}
            DESTINATION "."
            USE_SOURCE_PERMISSIONS)
    else(VISIOCYTE_MAC_CREATE_BUNDLE)
        # Don't create a bundle, just a visiocyte folder containing visiocyte and plugins
        # Place visiocyte in a folder called "visiocyte"
        set(VISIOCYTE_INSTALL_DIR "visiocyte" CACHE INTERNAL "Relative path where VISIOCYTE will be installed")
        install(TARGETS visiocyte
            DESTINATION ${VISIOCYTE_INSTALL_DIR}
            COMPONENT visiocyte)
    endif(VISIOCYTE_MAC_CREATE_BUNDLE)

else(APPLE)

    # On non-apple platforms, install executable, libraries, and plugins individually
    set(VISIOCYTE_INSTALL_DIR "bin" CACHE INTERNAL "Relative path where VISIOCYTE will be installed")
    install(TARGETS visiocyte
        DESTINATION ${VISIOCYTE_INSTALL_DIR}
        COMPONENT visiocyte)

    # Also install shared libraries that are needed by visiocyte
    if(WIN32) # well, MSYS, mostly
      get_filename_component(CXX_BINDIR "${CMAKE_CXX_COMPILER}" PATH)
      set(LIB_PATHS
        "${TIFF_INCLUDE_DIR}/../bin"
        "${QT_LIBRARY_DIR}/../bin"
        "${CXX_BINDIR}")
      if(Qt5Core_FOUND)
        set(SHLIBS jpeg62.dll libtiff3.dll Qt5Core.dll Qt5Gui.dll Qt5Network.dll
                    Qt5OpenGL.dll Qt5Xml.dll )
      else()
        set(SHLIBS jpeg62.dll libtiff3.dll QtCore4.dll QtGui4.dll QtNetwork4.dll
                    QtOpenGL4.dll QtXml4.dll )
      endif()

      # Install Qt plugins for image formats, at least on Windows
      if (QT_PLUGINS_DIR)
          file(GLOB QT_IMAGEFORMAT_PLUGINS "${QT_PLUGINS_DIR}/imageformats/*.dll")
          install(PROGRAMS ${QT_IMAGEFORMAT_PLUGINS}
          DESTINATION "${VISIOCYTE_INSTALL_DIR}/imageformats"
          COMPONENT visiocyte)
      endif()
      if(USE_FFMPEG)
          file(GLOB FFMPEG_DLLS "${av_dir}/bin/*.dll")
          install(PROGRAMS ${FFMPEG_DLLS}
          DESTINATION "${VISIOCYTE_INSTALL_DIR}"
          COMPONENT visiocyte)
      endif()
      if(USE_HDF5)
          file(GLOB HDF5_DLLS "${hdf_dir}/install/bin/*.dll")
          install(PROGRAMS ${HDF5_DLLS}
          DESTINATION "${VISIOCYTE_INSTALL_DIR}"
          COMPONENT visiocyte)
      endif()
    endif()
    # message("LIB_PATHS = ${LIB_PATHS}")
    foreach(SHLIB ${SHLIBS})
        set(lib_var_name "${SHLIB}_LIB_FILE")
        find_file(${lib_var_name}
            NAMES ${SHLIB}
            PATHS "${LIB_PATHS}"
            )
        if(${lib_var_name}_NOT_FOUND)
           find_file(${lib_var_name}
           NAMES ${SHLIB}
           PATHS /usr/lib /lib
           "${TIFF_INCLUDE_DIR}/../bin" # ${LIB_PATHS} does not work if spaces are in name...
           # Yes default paths, if above find_library failed
           )
        endif()

        set(lib_name "${${lib_var_name}}")
	#message("set ${lib_name} = ${${lib_var_name}} ")
        # Follow symlinks to the real file
        # So we don't just install symlinks
        if(lib_name)
            # message("Found ${lib_name}")
        else()
            # message("${SHLIB} (${lib_name}) not found")

        endif()
        get_filename_component(real_lib ${lib_name} REALPATH)
        get_filename_component(lib_name ${lib_name} NAME) # Strip path

        if(real_lib)
            #message("install  ${real_lib}")
            install(PROGRAMS
                ${real_lib}
                DESTINATION ${VISIOCYTE_INSTALL_DIR}
                COMPONENT RuntimeLibraries
                RENAME ${lib_name}
        )
        endif()
	#mark_as_advanced(${lib_var_name})
    endforeach()

endif(APPLE)


# Enforce a particular location for visiocyte.exe at build time.  So it can be next to plugins, for testing  before install.
# This must be defined after above check for BUNDLE_BUILD_DIR
if (VISIOCYTE_BUILD_BINARY_DIR)
    # Build visiocyte directly into nascent app bundle
    set_target_properties(visiocyte PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${VISIOCYTE_BUILD_BINARY_DIR}")
    if (MSVC)
        # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
        # But the PREFIX trick does not work with MSVC 2010
        # set_target_properties (visiocyte PROPERTIES PREFIX "../")
        if(VISIOCYTE_BUILD_BINARY_DIR)
            set_target_properties(visiocyte PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${VISIOCYTE_BUILD_BINARY_DIR})
            set_target_properties(visiocyte PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${VISIOCYTE_BUILD_BINARY_DIR})
            set_target_properties(visiocyte PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${VISIOCYTE_BUILD_BINARY_DIR})
            set_target_properties(visiocyte PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${VISIOCYTE_BUILD_BINARY_DIR})
            set_target_properties(visiocyte PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${VISIOCYTE_BUILD_BINARY_DIR})
            set_target_properties(visiocyte PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${VISIOCYTE_BUILD_BINARY_DIR})
        endif()
    endif()
endif()

target_link_libraries(visiocyte NeuronAnnotatorLib)

if(VISIOCYTE_MAC_CREATE_BUNDLE)
    # Install Qt libraries into bundle
    find_program(MAC_DEPLOY_QT macdeployqt)
    add_custom_command(TARGET visiocyte POST_BUILD
        COMMAND "${MAC_DEPLOY_QT}" "${BUNDLE_BUILD_DIR}"
        DEPENDS visiocyte
        COMMENT "Inserting Qt libraries into visiocyte.app")
endif()


