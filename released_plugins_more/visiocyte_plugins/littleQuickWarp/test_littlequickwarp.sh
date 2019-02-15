#!/bin/bash

#littlequickwarp

#mac
dir_exe=/Users/qul/work/visiocyte_tools/released_plugins/visiocyte_plugins/littleQuickWarp
dir_data=/Users/qul/Desktop/testdata/

#linux
#dir_exe=/groups/peng/home/qul/myhome/work/assembla/qu_lei/littleQuickWarp/
#dir_data=/groups/peng/home/qul/myhome/work/testdata/

#256*256*108*1------------------------------------------------------------------------------
#uint8 test
$dir_exe/main -S $dir_data/output_global.tiff -t $dir_data/output_target.marker -s $dir_data/output_subject.marker -o $dir_data/output_warp_littlequick.visiocyteraw -I 1 -i 0 -p 0

#float test
#$dir_exe/main -S $dir_data/output_global_float32.visiocyteraw -t $dir_data/output_target.marker -s $dir_data/output_subject.marker -o $dir_data/output_warp_littlequick.visiocyteraw -p 1



#512*512*216*1------------------------------------------------------------------------------
#uint8 test
#$dir_exe/main -S $dir_data/output_global_512_uint8.visiocyteraw -t $dir_data/output_target.marker -s $dir_data/output_subject.marker -o $dir_data/output_warp_littlequick.visiocyteraw -p 1

#float test
#$dir_exe/main -S $dir_data/output_global_512_float32.visiocyteraw -t $dir_data/output_target.marker -s $dir_data/output_subject.marker -o $dir_data/output_warp_littlequick.visiocyteraw -p 1


#1024*1024*1024*1------------------------------------------------------------------------------
#uint8 test
#$dir_exe/main -S $dir_data/output_global_1024_uint8.visiocyteraw -t $dir_data/output_target.marker -s $dir_data/output_subject.marker -o $dir_data/output_warp_littlequick.visiocyteraw -p 0

#float test
#$dir_exe/main -S $dir_data/output_global_1024_float32.visiocyteraw -t $dir_data/output_target.marker -s $dir_data/output_subject.marker -o $dir_data/output_warp_littlequick.visiocyteraw -p 1




