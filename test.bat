@echo off

echo Test.bat -- Based on deguix's dnd_pk7_build.bat
echo -----------------------------------------------
echo.
echo This batch file will create .pk7 in non-solid mode for testing (bad on
echo compression, but good in update speed). Also doesn't use versioning.
echo.
echo If this bat file doesn't create the pk7, or acs are missing, make sure:
echo - bcc environment variable set to the full path of bcc executable.
echo - 7zip installed in its default path (change bat if not).
echo.

cd acs
call compile_acs.bat
cd ..

echo.
echo Packing all files into pk7's...

SET sevenzip="%PROGRAMFILES%\7-zip\7z.exe"
%sevenzip% u -t7z dnd.pk7 .\dnd\* -mx=9 -ms=off >nul
%sevenzip% u -t7z dnd_monsters.pk7 .\monsters\* -mx=9 -ms=off >nul
%sevenzip% u -t7z dnd_onlyammo.pk7 .\onlyammo\* -mx=9 -ms=off >nul

echo Done.
@echo on