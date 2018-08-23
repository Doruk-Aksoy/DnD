#!/bin/bash

echo "Test.sh -- Based on deguix's dnd_pk7_build.bat
----------------------------------------------

This bash file will create .pk7 in non-solid mode for testing (bad on
compression, but good in update speed). Also doesn't use versioning.

If this sh file doesn't create the pk7, or acs are missing, make sure:
- bcc environment variable set to the full path of bcc executable.
- p7zip package is installed in system.
"

cd acs
./compile_acs.sh
cd ..

echo "
Packing all files into pk7's..."

7z u -t7z dnd.pk7 ./dnd/* -mx=9 -ms=off >/dev/null
7z u -t7z dnd_monsters.pk7 ./monsters/* -mx=9 -ms=off >/dev/null
7z u -t7z dnd_onlyammo.pk7 ./onlyammo/* -mx=9 -ms=off >/dev/null

echo "Done."