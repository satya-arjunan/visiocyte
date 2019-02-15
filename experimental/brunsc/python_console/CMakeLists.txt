cmake_minimum_required(VERSION 2.6)

# If we are building 64-bit binaries on Windows, look for libraries in 
# "C:\Program Files\", not in "C:\Program Files (x86)\"
if("${CMAKE_GENERATOR}" MATCHES "Win64")
   # message("Win64")
   set(ENV{PROGRAMFILES} $ENV{ProgramW6432})
endif()

# Embedded python on Linux might need some special linking to get all the python symbols
# http://docs.python.org/py3k/extending/embedding.html
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # message("Linux")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -export-dynamic")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Xlinker -export-dynamic")
    find_library(DL_LIBRARY "dl")
endif()

# To support local test building, conditionally set certain variables
if(NOT FinishedPlugins)
    add_custom_target(FinishedPlugins)
endif()
if(NOT INSTALLATION_DIRECTORY)
    set(INSTALLATION_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
include(../../../released_plugins/CMake/PluginConfiguration.cmake)

# Default to building fat binaries, even on Leopard
if(APPLE)
    if(NOT CMAKE_OSX_ARCHITECTURES)
         set(CMAKE_OSX_ARCHITECTURES i386 x86_64)
    endif()
endif()

# Python libraries are needed to create a python interpreter
FIND_PACKAGE( PythonLibs REQUIRED )
include_directories(${PYTHON_INCLUDE_PATH})

# Boost::python is needed to expose our C++ classes in python
SET(Boost_ADDITIONAL_VERSIONS "1.46.1" "1.44.0" "1.44" "1.45.0" "1.45")
find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Qt is needed for GUI window.
find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

QT4_WRAP_UI(QT_UI_SRCS
  main_python_console.ui
)

QT4_WRAP_CPP(QT_MOC_SRCS
  PythonConsoleWindow.h
  PythonInterpreter.h
  ImageWindow.h
)

QT4_ADD_RESOURCES(QT_RCC_SRCS python_console.qrc)

# Qt dynamically generates some headers here
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB PYPLUSPLUS_SRCS "generated_code/*.cpp")
list(REMOVE_ITEM PYPLUSPLUS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/generated_code/visiocyte.main.cpp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/generated_code")
# message(${PYPLUSPLUS_SRCS})

set(VISIOCYTE_PYTHON_CONVERSION_HEADERS
    convert_qlist.h
    convert_qhash.h
    convert_c_array_struct.h
    convert_simtk_vec3.hpp
)

set(VISIOCYTE_PYTHON_CONSOLE_SRCS
    PythonConsoleWindow.cpp
    PythonInterpreter.cpp
    VISIOCYTEPythonModule.cpp
    ${QT_UI_SRCS}
    ${QT_MOC_SRCS}
    ${PYPLUSPLUS_SRCS}
    ${QT_RCC_SRCS}
    wrappable_visiocyte.cpp
    convert_qstring.cpp
    convert_qbool.cpp
    convert_simtk_vec3.cpp
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(simtk/src)
add_definitions(-DSimTK_USE_STATIC_LIBRARIES)

##################################################################
# pythonConsoleTest is a test program for use during development #
##################################################################

add_executable(pythonConsoleTest
    pythonConsoleTest.cpp
    ${VISIOCYTE_PYTHON_CONSOLE_SRCS}
)

target_link_libraries(pythonConsoleTest
    ${Boost_PYTHON_LIBRARY}
    ${PYTHON_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    SimTKcommon
)
if (DL_LIBRARY)
    target_link_libraries(pythonConsoleTest ${DL_LIBRARY})
endif()


################################################
# pythonConsole is a VISIOCYTE plugin shared library #
################################################
# ...which also doubles as a python module
# TODO - create separate libraries for python module and plugin-only code

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../visiocyte_main/basic_c_fun/"
    )

set(PLUGIN_SOURCES 
    PythonConsolePlugin.cpp
    ${VISIOCYTE_PYTHON_CONSOLE_SRCS}
)
set(PLUGIN_HEADERS 
    PythonConsolePlugin.h
    # PythonConsoleWindow.h // already MOCed in this file above
)

set(PLUGIN_DIRECTORY_NAME python_scripting/python_console)
configure_visiocyte_plugin(PythonConsolePlugin)

target_link_libraries(PythonConsolePlugin
    ${Boost_PYTHON_LIBRARY}
    ${PYTHON_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    SimTKcommon
)
if (DL_LIBRARY)
    target_link_libraries(pythonConsoleTest ${DL_LIBRARY})
endif()

find_path(PLUGINS_PARENT_PATH plugins
    PATHS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../build_visiocyte_cmake_eclipse/visiocyte"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../build_cmake/visiocyte"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../build_vstudio/visiocyte"
    PATH_SUFFIXES
        "Mac_Fat/visiocyte.app/Contents/MacOS"
        "Linux_32"
        "Linux_64"
	"Windows_MSVC_32"
	"Windows_MSVC_64"
	"Windows_MSYS"
    NO_DEFAULT_PATH
    )
    
if(PLUGINS_PARENT_PATH)
    set(PLUGINS_PATH "${PLUGINS_PARENT_PATH}/plugins")
else()
    set(PLUGINS_PATH "${CMAKE_CURRENT_BINARY_DIR}/plugins")
endif()

set(VISIOCYTE_PYTHON_CONSOLE_PLUGIN_OUTPUT_DIR
    "${PLUGINS_PATH}/${PLUGIN_DIRECTORY_NAME}"
    CACHE PATH "Where to build python console plugin")

file(MAKE_DIRECTORY "${VISIOCYTE_PYTHON_CONSOLE_PLUGIN_OUTPUT_DIR}")

set_target_properties(PythonConsolePlugin PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${VISIOCYTE_PYTHON_CONSOLE_PLUGIN_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${VISIOCYTE_PYTHON_CONSOLE_PLUGIN_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${VISIOCYTE_PYTHON_CONSOLE_PLUGIN_OUTPUT_DIR}"
)

# Set output name of plugin to visiocyte.so or whatever, so it can be loaded as a module in python,
# ... so we can use epydoc to generate documentation.
set_target_properties(PythonConsolePlugin PROPERTIES
    OUTPUT_NAME visiocyte
    PREFIX ""
)
if(WIN32)
    set_target_properties(PythonConsolePlugin PROPERTIES 
	SUFFIX ".pyd"
	PREFIX "../") # move out of Release/Debug
endif()

#####################################
# Generate bindings with pyplusplus #
#####################################

set(VISIOCYTE_PYTHON_CONSOLE_GENERATE_WRAP_CODE FALSE CACHE BOOL 
    "Whether to run pyplusplus to generate boost python wrappers")
if(VISIOCYTE_PYTHON_CONSOLE_GENERATE_WRAP_CODE)
    find_package(PythonInterp REQUIRED)
    file(GLOB VISIOCYTE_PLUGIN_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../visiocyte_main/basic_c_fun/*.h")
    set(WRAP_DEPEND_SRCS
        ${VISIOCYTE_PLUGIN_HEADERS}
        ${VISIOCYTE_PYTHON_CONVERSION_HEADERS}
        wrappable_visiocyte.h
        doxygen_doc_extractor.py
        wrap_visiocyte_boost.py)
    # message(${WRAP_DEPEND_SRCS})
    set(VISIOCYTEGEN_STAMP "${CMAKE_CURRENT_SOURCE_DIR}/generated_code/generate_visiocyte.stamp")
	# Create a command line option to signal Windows 64 bit build
	set(WRAP_ARG "")
	if("${CMAKE_GENERATOR}" MATCHES "Win64")
		set(WRAP_ARG "--win64")
	endif()
    add_custom_command(
        COMMAND "${PYTHON_EXECUTABLE}" wrap_visiocyte_boost.py
            ${WRAP_ARG}
            > "${CMAKE_CURRENT_BINARY_DIR}/wrap_visiocyte_boost.log"
            2> "${CMAKE_CURRENT_BINARY_DIR}/wrap_visiocyte_boost.err"
        COMMAND ${CMAKE_COMMAND} -E touch "${VISIOCYTEGEN_STAMP}"
        OUTPUT "${VISIOCYTEGEN_STAMP}"
        DEPENDS ${WRAP_DEPEND_SRCS}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Creating VISIOCYTE wrapper source files.  Examine wrap_visiocyte_boost.err file for details"
    )
    add_custom_target(GenerateVISIOCYTEWrapperSources
        DEPENDS "${VISIOCYTEGEN_STAMP}"
        SOURCES ${WRAP_DEPEND_SRCS}
    )
    
    add_dependencies(PythonConsolePlugin GenerateVISIOCYTEWrapperSources)
endif()

#######################################
# Generate documentation using epydoc #
#######################################

set(VISIOCYTE_PYTHON_CONSOLE_GENERATE_API_DOCS FALSE CACHE BOOL
    "Whether to (re)generate documentation for the VISIOCYTE python programming API")
if(VISIOCYTE_PYTHON_CONSOLE_GENERATE_API_DOCS)
    find_package(PythonInterp REQUIRED)
    find_program(EPYDOC_EXECUTABLE NAMES epydoc epydoc.py)
    if(EPYDOC_EXECUTABLE)
        find_program(DOT_EXECUTABLE dot)
        if(DOT_EXECUTABLE)
            set(EPYDOC_OPTIONS ${EPYDOC_OPTIONS} --graph all --dotpath ${DOT_EXECUTABLE})
        endif()
        set(DOC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/doc")
        file(MAKE_DIRECTORY "${DOC_DIR}")
        configure_file(run_epydoc.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/run_epydoc.cmake @ONLY)
        add_custom_command(
            TARGET PythonConsolePlugin POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/run_epydoc.cmake"
            COMMENT "Creating VISIOCYTE python API documentation using epydoc..."
        )
    endif()
endif()

# Update movie maker ui files
# on mac /System/Library/Frameworks/Python.framework/Versions/2.6/bin/pyuic4
find_program(PYUIC4_EXE pyuic4
    PATHS 
        /System/Library/Frameworks/Python.framework/Versions/Current/bin
)
if(PYUIC4_EXE)
    set(ui_input "${CMAKE_CURRENT_SOURCE_DIR}/scripts/movie_maker_dialog.ui")
    set(ui_output "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ui_movie_maker_dialog_pyqt4.py")
    get_filename_component(ui_input_l "${ui_input}" NAME)
    get_filename_component(ui_output_l "${ui_output}" NAME)
    add_custom_command(
        OUTPUT "${ui_output}"
        COMMAND "${PYUIC4_EXE}" -o ${ui_output_l} ${ui_input_l}
        DEPENDS "${ui_input}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts"
        COMMENT "Updating movie maker PyQt4 user interface"
    )
    add_custom_target(MovieMakerPyQt4Ui DEPENDS "${ui_output}")
    add_dependencies(PythonConsolePlugin MovieMakerPyQt4Ui)
endif()
find_program(PYSIDEUIC_EXE pyside-uic)
if(PYSIDEUIC_EXE)
    set(ui_input "${CMAKE_CURRENT_SOURCE_DIR}/scripts/movie_maker_dialog.ui")
    set(ui_output "${CMAKE_CURRENT_SOURCE_DIR}/scripts/ui_movie_maker_dialog_pyside.py")
    get_filename_component(ui_input_l "${ui_input}" NAME)
    get_filename_component(ui_output_l "${ui_output}" NAME)
    add_custom_command(
        OUTPUT "${ui_output}"
        COMMAND "${PYSIDEUIC_EXE}" -o ${ui_output_l} ${ui_input_l}
        DEPENDS "${ui_input}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts"
        COMMENT "Updating movie maker PySide user interface"
    )
    add_custom_target(MovieMakerPySideUi DEPENDS "${ui_output}")
    add_dependencies(PythonConsolePlugin MovieMakerPySideUi)
endif()
    

########################################
# Install python scripts and libraries #
########################################
# ...by piggybacking on VISIOCYTE binary_plugins mechanism
# Place files in visiocyte binary_plugins area, so they will be installed.
file(GLOB PY_SCRIPT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.py")
find_path(VISIOCYTE_BUILD_PATH visiocyte
    PATHS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../build_visiocyte_cmake_eclipse"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../build_cmake"
    PATH_SUFFIXES
    NO_DEFAULT_PATH
    )
if(VISIOCYTE_BUILD_PATH) # ${CMAKE_BINARY_DIR} for VISIOCYTE project
    # place python console plugin in binary_plugins, so it will be installed with visiocyte
    # This copy occurs at configuration time
    set(BINARY_PLUGINS_PATH "${VISIOCYTE_BUILD_PATH}/binary_plugins/${PLUGIN_DIRECTORY_NAME}")
    file(MAKE_DIRECTORY ${BINARY_PLUGINS_PATH})
    # message("${BINARY_PLUGINS_PATH}")
    foreach(SCRIPT_FILE ${PY_SCRIPT_FILES})
        get_filename_component(FNAME "${SCRIPT_FILE}" NAME)
        configure_file("${SCRIPT_FILE}" "${BINARY_PLUGINS_PATH}/${FNAME}" COPYONLY)
    endforeach()
    file(MAKE_DIRECTORY "${BINARY_PLUGINS_PATH}/icons")
    file(GLOB ICON_FILES "${CMAKE_CURRENT_SOURCE_DIR}/scripts/icons/*.png")
    foreach(ICON_FILE ${ICON_FILES})
        get_filename_component(FNAME "${ICON_FILE}" NAME)
        configure_file("${ICON_FILE}" "${BINARY_PLUGINS_PATH}/icons/${FNAME}" COPYONLY)
    endforeach()
    # Copy visiocyte python module too
    # This copy must occur at build time
    get_target_property(PLUGIN_FILE PythonConsolePlugin LOCATION)
    get_filename_component(PLG_NAME "${PLUGIN_FILE}" NAME)
    # message("${PLUGIN_FILE}")
    # message("${BINARY_PLUGINS_PATH}/${PLG_NAME}")
    add_custom_command(TARGET PythonConsolePlugin
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${PLUGIN_FILE}" "${BINARY_PLUGINS_PATH}/${PLG_NAME}"
        COMMENT "Copying python console plugin to VISIOCYTE binary_plugins directory"
    )
    # Install boost python library
    get_filename_component(BPNAME "${Boost_PYTHON_LIBRARY_RELEASE}" NAME)
    if(APPLE)
        # Put boost python library in visiocyte.app/Contents/Frameworks
        set(FRAMEWORK_DIR "${VISIOCYTE_BUILD_PATH}/visiocyte/Mac_Fat/visiocyte.app/Contents/Frameworks")
        file(MAKE_DIRECTORY "${FRAMEWORK_DIR}")
        # message("${FRAMEWORK_DIR}/${BPNAME}")
        configure_file("${Boost_PYTHON_LIBRARY}" "${FRAMEWORK_DIR}/${BPNAME}" COPYONLY)
        # TODO install_name_tool
        # message("${BPNAME}")
        # message("@executable_path/../Frameworks/${BPNAME}")
        # message("${BINARY_PLUGINS_PATH}/${PLG_NAME}")
        add_custom_command(TARGET PythonConsolePlugin
            POST_BUILD
            # COMMAND install_name_tool -change "${BPNAME}" "@executable_path/../Frameworks/${BPNAME}" "${BINARY_PLUGINS_PATH}/${PLG_NAME}"
            COMMAND install_name_tool -change "${BPNAME}" "@executable_path/../Frameworks/${BPNAME}" "${PLUGIN_FILE}"
            COMMENT "Changing install name of boost python library."
        )
    elseif(MSVC)
        # We might link to the .lib file; we want the corresponding .dll file in the install
        # Replace ".lib" with ".dll"
        string(REGEX REPLACE "\\.lib$" ".dll" BP_DLL ${Boost_PYTHON_LIBRARY_RELEASE})
        # Replace "libboost_python" with "boost_python"; This might be unnecessary.
        string(REPLACE "libboost_python" "boost_python" BP_DLL ${BP_DLL})
        if(EXISTS ${BP_DLL})
            # message(${BP_DLL})
            get_filename_component(BPNAME "${BP_DLL}" NAME)
            configure_file("${BP_DLL}" "${BINARY_PLUGINS_PATH}/${BPNAME}" COPYONLY)
        endif()
    else()
        # Put boost python library next to visiocyte python console plugin library
        configure_file("${Boost_PYTHON_LIBRARY_RELEASE}" "${BINARY_PLUGINS_PATH}/${BPNAME}" COPYONLY)
    endif()
endif()
