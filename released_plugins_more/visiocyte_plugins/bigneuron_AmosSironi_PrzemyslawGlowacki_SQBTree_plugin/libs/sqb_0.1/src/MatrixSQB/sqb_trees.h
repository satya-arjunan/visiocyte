#ifndef __SQB_TREES_H__
#define __SQB_TREES_H__


#include <SQB/Core/RegTree.h>
#include <SQB/Core/Utils.h>

#include <SQB/Core/Booster.h>

#include <SQB/Core/LineSearch.h>

#include <fstream>

#include <visiocyte_interface.h>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageDuplicator.h"
#include "itkTIFFImageIOFactory.h"
#include "itkPNGImageIOFactory.h"

#include "../libs/sqb_0.1/src/MatrixSQB/visiocyte_link.h"

#include "../libs/regression/sep_conv.h"
#include "../libs/regression/util.h"

int mockTrainAndTest(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output);

#endif

