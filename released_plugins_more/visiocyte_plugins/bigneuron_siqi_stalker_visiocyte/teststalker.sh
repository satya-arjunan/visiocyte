#!/bin/bash
# The unit test for Stalker Model
visiocyte=../../../../visiocyte_external/bin/visiocyte;
#visiocyte=/home/siqi/Visiocyte_Ubuntu_64bit_v3.045/visiocyte;
qmake;
make -j4;

#$visiocyte -x NeuroStalker -f tracing_func -i test/testdata/test.little-soma.visiocyteraw -p 1 3 3 | tee runlog.txt
$visiocyte -x NeuroStalker -f tracing_func -i test/09-2902-04R-01C-60x_merge_c1.visiocytepbd -p 1 1 1 5 5 30 | tee runlog.txt

#The following lines are four parameters
#$visiocyte -x NeuroStalker -f tracing_func -i test/09-2902-04R-01C-60x_merge_c1.visiocytepbd -p 1 1 1 3 5 50 | tee runlog.txt
#$visiocyte -x NeuroStalker -f tracing_func -i test/09-2902-04R-01C-60x_merge_c1.visiocytepbd -p 1 1 1 1 5 70 | tee runlog.txt
#$visiocyte -x NeuroStalker -f tracing_func -i test/09-2902-04R-01C-60x_merge_c1.visiocytepbd -p 1 1 1 5 20 10 | tee runlog.txt
#$visiocyte -x NeuroStalker -f tracing_func -i test/09-2902-04R-01C-60x_merge_c1.visiocytepbd -p 1 1 1 2 10 90 | tee runlog.txt


#$visiocyte -x NeuroStalker -f tracing_func -i ~/Desktop/OP1.visiocyteraw -p 1 3 3 | tee runlog.txt
#matlab -nodesktop -nosplash -r "run('test/plotall.m')";
