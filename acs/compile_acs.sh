#!/bin/bash

mkdir -p ../DnD/acs

echo "Compiling acs code... If errors pop up, they will show up below:
----------------------------------------------------------------
"

$bcs -target any -D __ZANDRONUM__ "./DnD.bcs" "../DnD/acs/dnd.o"
$bcs -target any -D __ZANDRONUM__ "./Misc.bcs" "../DnD/acs/Misc.o"
echo "----------------------------------------------------------------"