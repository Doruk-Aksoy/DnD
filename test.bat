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
echo Packing all files into pk7's...

7z u -t7z dnd.pk7 -uq0 .\dnd\* -mx=9 -ms=off >nul
7z u -t7z dnd_monsters.pk7 -uq0 .\monsters\* -mx=9 -ms=off >nul

rem Renaming files that have invalid chars on Windows:
7z d dnd_monsters.pk7 .\Sprites\Monsters\ArchVile\WILE[1.png .\Sprites\Monsters\ArchVile\WILE[2.png .\Sprites\Monsters\ArchVile\WILE[3.png .\Sprites\Monsters\ArchVile\WILE[4.png .\Sprites\Monsters\ArchVile\WILE[5.png .\Sprites\Monsters\ArchVile\WILE[6.png .\Sprites\Monsters\ArchVile\WILE[7.png .\Sprites\Monsters\ArchVile\WILE[8.png >/dev/null
7z rn dnd_monsters.pk7 .\Sprites\Monsters\ArchVile\1.png .\Sprites\Monsters\ArchVile\WILE[1.png .\Sprites\Monsters\ArchVile\2.png .\Sprites\Monsters\ArchVile\WILE[2.png .\Sprites\Monsters\ArchVile\3.png .\Sprites\Monsters\ArchVile\WILE[3.png .\Sprites\Monsters\ArchVile\4.png .\Sprites\Monsters\ArchVile\WILE[4.png .\Sprites\Monsters\ArchVile\5.png .\Sprites\Monsters\ArchVile\WILE[5.png .\Sprites\Monsters\ArchVile\6.png .\Sprites\Monsters\ArchVile\WILE[6.png .\Sprites\Monsters\ArchVile\7.png .\Sprites\Monsters\ArchVile\WILE[7.png .\Sprites\Monsters\ArchVile\8.png .\Sprites\Monsters\ArchVile\WILE[8.png >/dev/null

7z d dnd_monsters.pk7 .\Sprites\Monsters\ArchVile\WILE]1.png .\Sprites\Monsters\ArchVile\WILE]2.png .\Sprites\Monsters\ArchVile\WILE]3.png .\Sprites\Monsters\ArchVile\WILE]4.png .\Sprites\Monsters\ArchVile\WILE]5.png .\Sprites\Monsters\ArchVile\WILE]6.png .\Sprites\Monsters\ArchVile\WILE]7.png .\Sprites\Monsters\ArchVile\WILE]8.png >/dev/null
7z rn dnd_monsters.pk7 .\Sprites\Monsters\ArchVile\1.png .\Sprites\Monsters\ArchVile\WILE]1.png .\Sprites\Monsters\ArchVile\2.png .\Sprites\Monsters\ArchVile\WILE]2.png .\Sprites\Monsters\ArchVile\3.png .\Sprites\Monsters\ArchVile\WILE]3.png .\Sprites\Monsters\ArchVile\4.png .\Sprites\Monsters\ArchVile\WILE]4.png .\Sprites\Monsters\ArchVile\5.png .\Sprites\Monsters\ArchVile\WILE]5.png .\Sprites\Monsters\ArchVile\6.png .\Sprites\Monsters\ArchVile\WILE]6.png .\Sprites\Monsters\ArchVile\7.png .\Sprites\Monsters\ArchVile\WILE]7.png .\Sprites\Monsters\ArchVile\8.png .\Sprites\Monsters\ArchVile\WILE]8.png >/dev/null

7z u -t7z dnd_onlyammo.pk7 -uq0 .\onlyammo\* -mx=9 -ms=off >nul

echo Done.
@echo on

