@echo off

echo Test.bat -- Based on deguix's dnd_pk7_build.bat
echo -----------------------------------------------
echo.
echo This batch file will create .pk7 in non-solid mode for testing (bad on
echo compression, but good in update speed). Also doesn't use versioning.
echo.
echo If this bat file doesn't create the pk7, or acs are missing, make sure:
echo - Just-install is installed (see https://just-install.it/).
echo - you ran "just-install exeproxy 7zip git bcc" from command promt as admin.
echo.

cd acs
call compile_acs.bat
cd ..

echo.
echo Packing all files into pk7's...

7z u -t7z dnd.pk7 -uq0 .\dnd\* -mx=9 -ms=off >nul
7z u -t7z dnd_monsters.pk7 -uq0 .\monsters\* -mx=9 -ms=off >nul
7z u -t7z dnd_onlyammo.pk7 -uq0 .\onlyammo\* -mx=9 -ms=off >nul

echo Done.
@echo on