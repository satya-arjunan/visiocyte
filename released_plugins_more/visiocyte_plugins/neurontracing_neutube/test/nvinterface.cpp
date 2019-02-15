#include "nvinterface.h"


Mc_Stack *NVInterface::makeStack(const Image4DSimple *visiocyteImage)
{
    int width = visiocyteImage->getXDim();
    int height = visiocyteImage->getYDim();
    int depth = visiocyteImage->getZDim();
    int channelCount = visiocyteImage->getCDim();

    int kind = GREY;
    switch (visiocyteImage->getDatatype()) {
    case VISIOCYTE_UINT8:
        kind = GREY;
        break;
    case VISIOCYTE_UINT16:
        kind = GREY16;
        break;
    case VISIOCYTE_THREEBYTE:
        kind = COLOR;
        break;
    case VISIOCYTE_FLOAT32:
        kind = FLOAT32;
        break;
    default:
        return NULL;
    }

    Mc_Stack stack;
    C_Stack::setAttribute(&stack, kind, width, height, depth, channelCount);
    stack.array = const_cast<unsigned char*>(visiocyteImage->getRawData());
    Mc_Stack *out = C_Stack::clone(&stack);

    return out;
}
