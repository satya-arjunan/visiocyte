:: 2018-04-27 RZC
:: a convenience script for MSVC nmake

cd visiocyte_main\common_lib\winlib64
copy /y libnewmat_VS2010.lib libnewmat.lib
cd ..\..\..
  
cd visiocyte_main\visiocyte
call ..\..\qmake -v
call ..\..\qmake  visiocyte_msvc.pro

copy/b visiocyte_version_info.cpp+,,
nmake -f Makefile.Release

cd release
dir visiocyte_msvc.exe


