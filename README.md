
Build on Ubuntu
===============
*sudo apt install qt4-qmake qt4-dev-tools qt4-demos libglm-dev libjpeg-dev libtiff-dev libhdf5-dev
*export QT_SELECT=qt4
*git clone --recursive git@github.com:/satya-arjunan/visiocyte.git
*mkdir build
*cd build
*cmake -DCMAKE_BUILD_TYPE:STRING=Release -DQT4_DEMOS_LIBRARY=/usr/lib/qt4/demos/shared/libdemo_shared.a ../
*make -j2
