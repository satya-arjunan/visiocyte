#ifndef NVINTERFACE_H
#define NVINTERFACE_H

#include <visiocyte_interface.h>
#include "zstack.hxx"

namespace NVInterface {

Mc_Stack* makeStack(const Image4DSimple *visiocyteImage);

}

#endif // NVINTERFACE_H
