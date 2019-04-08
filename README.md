
Build on Ubuntu
===============
1. sudo apt install qt4-qmake qt4-dev-tools qt4-demos libglm-dev libjpeg-dev libtiff-dev libhdf5-dev
2. export QT_SELECT=qt4
3. git clone --recursive git@github.com:/satya-arjunan/visiocyte.git
4. mkdir build
5. cd build
6. cmake -DCMAKE_BUILD_TYPE:STRING=Release -DQT4_DEMOS_LIBRARY=/usr/lib/qt4/demos/shared/libdemo_shared.a ../
7. make -j2
