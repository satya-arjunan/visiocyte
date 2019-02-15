#!/bin/bash
mkdir -p test_data;
export TESTIMGZIP=./test_data/test.tif.zip;
export TESTIMG=./test_data/test.tif;
export TESTURL=https://s3-ap-southeast-2.amazonaws.com/rivulet/test.tif.zip;
if [ ! -f $TESTIMG ];
then
  rm -rf test_data/*;
  echo "Downloading test image from $TESTURL";
  wget -P ./test_data/ $TESTURL;
  unzip $TESTIMGZIP -d ./test_data;
fi

export VISIOCYTEPATH=../../../../visiocyte_external
export LD_LIBRARY_PATH=$VISIOCYTEPATH/visiocyte_main/common_lib/lib
export visiocyte=$VISIOCYTEPATH/bin/visiocyte; # Assume visiocyte
qmake;
make -j8;
echo "Build Finish"

echo "visiocyte -x Rivulet -f tracing_func -i <inimg_file> -p <channel> <threshold>"
$visiocyte -x Rivulet -f tracing_func -i $TESTIMG -o $TESTIMG.r2.swc -p 1 8 0 1;
