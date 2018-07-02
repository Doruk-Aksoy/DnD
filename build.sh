echo "Build.sh -- Based on deguix's dnd_pk7_build.bat
-----------------------------------------------
 
This bash file will create .pk7 in solid mode for releasing (good on
compression, slow in update speed). Also will use current git head tag.

If this sh file doesn't create the pk7, or acs are missing, make sure:
- bcc environment variable set to the full path of bcc executable.
- p7zip package is installed in system.
- git package is installed (for versioning only).
"

cd acs
./compile_acs.sh
cd ..

echo "
echo Packing all files into pk7's..."

version=`git describe --tags`
7z u -t7z dnd$version.pk7 ./dnd/* -mx=9 >/dev/null
7z u -t7z dnd_monsters$version.pk7 ./monsters/* -mx=9 >/dev/null
7z u -t7z dnd_onlyammo$version.pk7 ./onlyammo/* -mx=9 >/dev/null

echo "Done."