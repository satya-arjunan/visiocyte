// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrappable_visiocyte.h"
#include "visiocyte_enumerations.pypp.hpp"

namespace bp = boost::python;

void register_enumerations(){

    bp::enum_< Qt::AspectRatioMode>("AspectRatioMode")
        .value("IgnoreAspectRatio", Qt::IgnoreAspectRatio)
        .value("KeepAspectRatio", Qt::KeepAspectRatio)
        .value("KeepAspectRatioByExpanding", Qt::KeepAspectRatioByExpanding)
        .export_values()
        ;

    bp::enum_< SimTK::BodyOrSpaceType>("BodyOrSpaceType")
        .value("BodyRotationSequence", SimTK::BodyRotationSequence)
        .value("SpaceRotationSequence", SimTK::SpaceRotationSequence)
        .export_values()
        ;

    bp::enum_< Qt::FillRule>("FillRule")
        .value("OddEvenFill", Qt::OddEvenFill)
        .value("WindingFill", Qt::WindingFill)
        .export_values()
        ;

    bp::enum_< ImagePixelType>("ImagePixelType")
        .value("VISIOCYTE_UNKNOWN", VISIOCYTE_UNKNOWN)
        .value("VISIOCYTE_UINT8", VISIOCYTE_UINT8)
        .value("VISIOCYTE_UINT16", VISIOCYTE_UINT16)
        .value("VISIOCYTE_FLOAT32", VISIOCYTE_FLOAT32)
        .export_values()
        ;

    bp::enum_< PxLocationMarkerShape>("PxLocationMarkerShape")
        .value("pxUnset", pxUnset)
        .value("pxSphere", pxSphere)
        .value("pxCube", pxCube)
        .value("pxCircleX", pxCircleX)
        .value("pxCircleY", pxCircleY)
        .value("pxCircleZ", pxCircleZ)
        .value("pxSquareX", pxSquareX)
        .value("pxSquareY", pxSquareY)
        .value("pxSquareZ", pxSquareZ)
        .value("pxLineX", pxLineX)
        .value("pxLineY", pxLineY)
        .value("pxLineZ", pxLineZ)
        .value("pxTriangle", pxTriangle)
        .value("pxDot", pxDot)
        .export_values()
        ;

    bp::enum_< PxLocationUsefulness>("PxLocationUsefulness")
        .value("pxUnknown", pxUnknown)
        .value("pxLocaNotUseful", pxLocaNotUseful)
        .value("pxLocaUseful", pxLocaUseful)
        .value("pxLocaUnsure", pxLocaUnsure)
        .value("pxTemp", pxTemp)
        .export_values()
        ;

    bp::enum_< TimePackType>("TimePackType")
        .value("TIME_PACK_NONE", TIME_PACK_NONE)
        .value("TIME_PACK_Z", TIME_PACK_Z)
        .value("TIME_PACK_C", TIME_PACK_C)
        .export_values()
        ;

}
