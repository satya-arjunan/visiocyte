:: batch build all released plugin projects using a VC Studio complier's nmake function
:: (window version) by Hanchuan Peng
:: 2010-05-20
:: Best add release argument to make release\visiocyte.exe can recongnize plugin
:: revised from the original bat file

set PATH=%PATH%;

cd visiocyte_plugins
call :makepro %*
cd ..
goto :eof

:makepro
for /D %%i in ( * ) do (
  cd %%i
  if exist *.pro (
  	qmake
    nmake clean
  	nmake -f Makefile.Release
  ) 
  cd ..
)
goto :eof
