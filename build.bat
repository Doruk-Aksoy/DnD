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
7z u -t7z dnd_monsters%version%.pk7 -uq0 .\monsters\* -mx=9 >nul

rem Renaming files that have invalid chars on Windows:
7z d dnd_monsters%version%.pk7 .\Sprites\Monsters\ArchVile\WILE[1.png .\Sprites\Monsters\ArchVile\WILE[2.png .\Sprites\Monsters\ArchVile\WILE[3.png .\Sprites\Monsters\ArchVile\WILE[4.png .\Sprites\Monsters\ArchVile\WILE[5.png .\Sprites\Monsters\ArchVile\WILE[6.png .\Sprites\Monsters\ArchVile\WILE[7.png .\Sprites\Monsters\ArchVile\WILE[8.png >/dev/null
7z rn dnd_monsters%version%.pk7 .\Sprites\Monsters\ArchVile\1.png .\Sprites\Monsters\ArchVile\WILE[1.png .\Sprites\Monsters\ArchVile\2.png .\Sprites\Monsters\ArchVile\WILE[2.png .\Sprites\Monsters\ArchVile\3.png .\Sprites\Monsters\ArchVile\WILE[3.png .\Sprites\Monsters\ArchVile\4.png .\Sprites\Monsters\ArchVile\WILE[4.png .\Sprites\Monsters\ArchVile\5.png .\Sprites\Monsters\ArchVile\WILE[5.png .\Sprites\Monsters\ArchVile\6.png .\Sprites\Monsters\ArchVile\WILE[6.png .\Sprites\Monsters\ArchVile\7.png .\Sprites\Monsters\ArchVile\WILE[7.png .\Sprites\Monsters\ArchVile\8.png .\Sprites\Monsters\ArchVile\WILE[8.png >/dev/null

7z d dnd_monsters%version%.pk7 .\Sprites\Monsters\ArchVile\WILE]1.png .\Sprites\Monsters\ArchVile\WILE]2.png .\Sprites\Monsters\ArchVile\WILE]3.png .\Sprites\Monsters\ArchVile\WILE]4.png .\Sprites\Monsters\ArchVile\WILE]5.png .\Sprites\Monsters\ArchVile\WILE]6.png .\Sprites\Monsters\ArchVile\WILE]7.png .\Sprites\Monsters\ArchVile\WILE]8.png >/dev/null
7z rn dnd_monsters%version%.pk7 .\Sprites\Monsters\ArchVile\1.png .\Sprites\Monsters\ArchVile\WILE]1.png .\Sprites\Monsters\ArchVile\2.png .\Sprites\Monsters\ArchVile\WILE]2.png .\Sprites\Monsters\ArchVile\3.png .\Sprites\Monsters\ArchVile\WILE]3.png .\Sprites\Monsters\ArchVile\4.png .\Sprites\Monsters\ArchVile\WILE]4.png .\Sprites\Monsters\ArchVile\5.png .\Sprites\Monsters\ArchVile\WILE]5.png .\Sprites\Monsters\ArchVile\6.png .\Sprites\Monsters\ArchVile\WILE]6.png .\Sprites\Monsters\ArchVile\7.png .\Sprites\Monsters\ArchVile\WILE]7.png .\Sprites\Monsters\ArchVile\8.png .\Sprites\Monsters\ArchVile\WILE]8.png >/dev/null

7z u -t7z dnd_onlyammo%version%.pk7 -uq0 .\onlyammo\* -mx=9 >nul

echo Done.
@echo on