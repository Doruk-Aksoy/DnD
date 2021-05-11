REM echo @off is implied here
REM for %%F in (*.bcs) do bcc "%%~nF.bcs" "..\pk3\acs\%%~nF.lmp"

if not exist "..\dnd\acs" mkdir "..\dnd\acs"
if not exist "..\monsters\acs" mkdir "..\monsters\acs"

echo Compiling acs code... If errors pop up, they will show up below:
echo ----------------------------------------------------------------
echo.
%bcs% "dnd.bcs" "..\dnd\acs\dnd.lmp"
%bcs% "dnd_database.bcs" "..\dnd\acs\dnd_db.lmp"
%bcs% "dnd_menu.bcs" "..\dnd\acs\dnd_menu.lmp"
%bcs% "dnd_mons.bcs" "..\monsters\acs\dnd_mons.lmp"
echo ----------------------------------------------------------------