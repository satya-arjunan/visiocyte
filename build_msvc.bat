:: 2018-3-23 by Zongcai Ruan
:: a convenience script for building in VS???? x64 Win64 Command Prompt
::
  

cd visiocyte_main\visiocyte\

call ..\..\qmake visiocyte_msvc.pro
nmake -f Makefile.Release

copy release\visiocyte_msvc.exe ..\..\bin\  /y
cd  ..\..\bin\

copy %QT_BIN%\QtCore4.dll .\  /y
copy %QT_BIN%\QtGui4.dll   .\  /y
copy %QT_BIN%\QtOpenGL4.dll .\  /y
copy %QT_BIN%\QtXml4.dll .\  /y
copy %QT_BIN%\QtNetwork4.dll .\  /y


visiocyte_msvc.exe
