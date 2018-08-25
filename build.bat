@echo off

echo Build.bat -- Based on deguix's dnd_pk7_build.bat
echo ------------------------------------------------
echo.
echo This batch file will create .pk7 in solid mode for releasing (good on
echo compression, slow in update speed). Also will use current git head tag.
echo.
echo If this bat file doesn't create the pk7, or acs are missing, make sure:
echo - bcc environment variable set to the full path of bcc executable.
echo - 7zip installed in its default path (change bat if not).
echo - git-for-windows installed in default path with Windows console mode if using
echo Windows console prompt. Otherwise, use the git bash.
echo.

cd acs
call compile_acs.bat
cd ..

echo.
echo Packing all files into pk7's...

FOR /F "tokens=*" %%g in ('git describe --tags') do (SET version=%%g)
7z u -t7z dnd%version%.pk7 -uq0 .\dnd\* -mx=9 >nul
7z u -t7z dnd_monsters%version%.pk7 -uq0 .\monsters\* -mx=9 >nul
7z u -t7z dnd_onlyammo%version%.pk7 -uq0 .\onlyammo\* -mx=9 >nul

echo Done.
@echo on