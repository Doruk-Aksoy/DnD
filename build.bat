@echo off

echo Build.bat -- Based on deguix's dnd_pk7_build.bat
echo ------------------------------------------------
echo.
echo This batch file will create .pk7 in solid mode for releasing (good on
echo compression, slow in update speed). Also will use current git head tag.
echo.
echo If this bat file doesn't create the pk7, or acs are missing, make sure,
echo while running cmd prompt as admin, to run prepare.bat.
echo.

cd acs
call compile_acs.bat
cd ..

echo.
echo Packing all files into pk7's...

FOR /F "tokens=*" %%g in ('git describe --tags') do (SET version=%%g)
7z u -t7z dnd%version%.pk7 -uq0 .\dnd\* -mx=9 >nul

echo Done.
@echo on