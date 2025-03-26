#!/bin/bash

mkdir -p ../DnD/acs
mkdir -p ../monsters/acs

echo "Compiling acs code... If errors pop up, they will show up below:
----------------------------------------------------------------
"

$bcs -D __ZANDRONUM__ "./DnD.bcs" "../DnD/acs/dnd.o"
$bcs -D __ZANDRONUM__ "./DnD_Database.bcs" "../DnD/acs/dnd_db.o"
$bcs -D __ZANDRONUM__ "./DnD_Menu.bcs" "../DnD/acs/dnd_menu.o"
$bcs -D __ZANDRONUM__ "./DnD_Mons.bcs" "../DnD/acs/dnd_mons.o"
echo "----------------------------------------------------------------"