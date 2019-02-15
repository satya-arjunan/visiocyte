cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

project(visiocyte_extra_plugins) # those plugins are distributed in seperate repo (visiocyte_tools)  than the truck code


find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

find_library(TIFF_LIBRARY
    NAMES libvisiocytetiff.a visiocytetiff libtiff
    PATHS "${VISIOCYTEMAINDIR}/common_lib/lib")
if (NOT TIFF_LIBRARY)
	find_package(TIFF REQUIRED)
endif()
include_directories( "${TIFF_INCLUDE_DIR}" )

include(CTest)

# include(CPack) # save cpack for parent directory

find_path(VISIOCYTE_BUILD_DIR  DOC "The binary dir where you CMake build the visiocyte trunk." REQUIRED)

find_path(VISIOCYTE_BINARY_DIR  DOC " This points to the location of the visiocyte executable, where the plugins subfolder is lcoated." REQUIRED)
if (WIN32)
   # to be packed into the installer together with the trunk plugins
   set(INSTALLATION_DIRECTORY ${VISIOCYTE_BUILD_DIR}/binary_plugins)
else()
   set(INSTALLATION_DIRECTORY ${VISIOCYTE_BINARY_DIR}/plugins)
endif()

find_path(VISIOCYTE_BASIC_C_FUN_SOURCE_DIR visiocyte_interface.h DOC "The trunk basic code that contains visiocyte_interface.h." REQUIRED)
if( VISIOCYTE_BASIC_C_FUN_SOURCE_DIR )
  set( VISIOCYTE_BASIC_C_FUN_BINARY_DIR  ${PROJECT_BINARY_DIR}/Source/basic_c_fun )
endif( VISIOCYTE_BASIC_C_FUN_SOURCE_DIR )

set(VISIOCYTEMAINDIR  "${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR}/../")  # the visiocyte_main folder

find_path(VISIOCYTE_PLUGIN_LIBRARIES_PATH
  NAMES libVISIOCYTEInterface.a
  PATHS  "${VISIOCYTE_BUILD_DIR}/released_plugins/bin" 
  DOC  "The binary dir where all the main visiocyte libraries are stored."
  )
find_library(VISIOCYTEInterface_LIBRARY NAMES VISIOCYTEInterface  HINTS ${VISIOCYTE_PLUGIN_LIBRARIES_PATH} DOC "The visiocyte interface library")

if(WIN32)
    message("Looking for fftw library")
    # message("$ENV{PROGRAMW6432}/fftw64_3.3.1")
    find_library(FFTW_LIBRARY
        NAMES libfftw3f.a visiocytefftw3f libfftw3f.a fftw3f libfftw3f-3
        PATHS
            "$ENV{PROGRAMFILES}/fftw"
            "$ENV{PROGRAMW6432}/fftw64_3.3.1"
            "$ENV{PROGRAMW6432}/fftw-3.3.3"
    )
    find_library(FFTW_THREADS_LIBRARY
        NAMES libfftw3f_threads.a visiocytefftw3f_threads libfftw3f_threads.a fftw3f_threads )
    #
    if(FFTW_LIBRARY)
        get_filename_component(FFTW_LIB_DIR "${FFTW_LIBRARY}" PATH)
        find_path(FFTW_INCLUDE_DIR
            NAME "fftw3.h"
            PATHS
                "${VISIOCYTEMAINDIR}/common_lib/include"
                /usr/local/include
                ${FFTW_LIB_DIR}
        )
        include_directories("${FFTW_INCLUDE_DIR}")
    endif()
else()
    # Build FFTW as an external project
    set(fftw_dir "${CMAKE_CURRENT_BINARY_DIR}/fftw")
    include(ExternalProject)
    # AMD 64 requires -fPIC to link static libraries on linux
    set(fftw_cflags "")
    if(UNIX AND NOT APPLE)
      if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
        set(fftw_cflags "-fPIC ${fftw_cflags}")
      endif()
    endif()
    ExternalProject_Add(
        LibFFTW
        URL "${VISIOCYTEMAINDIR}/common_lib/src_packages/fftw-3.1.2.tar.gz"
        SOURCE_DIR ${fftw_dir}/src
        BINARY_DIR ${fftw_dir}/build
        INSTALL_DIR ${fftw_dir}/install
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND <SOURCE_DIR>/configure
            --prefix=<INSTALL_DIR>
            --enable-float
            --enable-threads
            --enable-static
            CFLAGS=${fftw_cflags}
    )
    set(FFTW_INCLUDE_DIR "${fftw_dir}/install/include" CACHE PATH "" FORCE)
    include_directories("${FFTW_INCLUDE_DIR}")
    set(lib_suffix "a")
    set(lib_prefix "lib")
    if(WIN32)
      set(lib_suffix "lib")
      set(lib_prefix "")
    endif()
    set(FFTW_LIBRARY "${fftw_dir}/install/lib/${lib_prefix}fftw3f.${lib_suffix}" CACHE PATH "" FORCE)

endif()

set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin
    CACHE INTERNAL "Output directory for the shared libraries.")

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin
    CACHE INTERNAL "Output directory for the executables.")


include_directories( ${VISIOCYTE_BASIC_C_FUN_SOURCE_DIR} )

# CMB Nov-11-2010 Need to wait until all plugins are built before running macdeployqt
add_custom_target(FinishedPlugins
    COMMENT "All plugins have been built")

include(${PROJECT_SOURCE_DIR}/CMake/PluginConfiguration.cmake)

add_subdirectory( visiocyte_plugins )
add_subdirectory( special_visiocyte_plugins )

