@echo off

echo Test.bat -- Based on deguix's dnd_pk7_build.bat
echo -----------------------------------------------
echo.
echo This batch file will create .pk7 in non-solid mode for testing (bad on
echo compression, but good in update speed). Also doesn't use versioning.
echo.
echo If this bat file doesn't create the pk7, or acs are missing, make sure,
echo while running cmd prompt as admin, to run prepare.bat.
echo.

cd acs
call compile_acs.bat
cd ..

echo.
echo Packing all files into pk3's...

7z u -tzip dnd.pk3 -uq0 .\dnd\* -mx=9 >nul
7z u -tzip dnd_monsters.pk3 -uq0 .\monsters\* -mx=9 >nul
7z u -tzip dnd_onlyammo.pk3 -uq0 .\onlyammo\* -mx=9 >nul

echo Done.
@echo on