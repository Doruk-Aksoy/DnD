#!/bin/bash

mkdir -p ../dnd/acs
mkdir -p ../monsters/acs

echo "Compiling acs code... If errors pop up, they will show up below:
----------------------------------------------------------------
"

$bcc "./DnD.bcs" "../dnd/acs/dnd.lmp"
$bcc "./DnD_Database.bcs" "../dnd/acs/dnd_db.lmp"
$bcc "./DnD_Menu.bcs" "../dnd/acs/dnd_menu.lmp"
$bcc "./DnD_Shared.bcs" "../dnd/acs/dnd_shrd.lmp"
$bcc "./DnD_Mons.bcs" "../monsters/acs/dnd_mons.lmp"
echo "----------------------------------------------------------------"