# Death and Decay
## Building from Source
### Windows
#### Command Prompt Notes
* How to run it:
  * Windows 10: Click the search button and type: `Command Prompt` and click on the desktop app. Run as admin if requested.
  * Any other Windows: Click **Start**, **Programs**, **Windows System**, **Command Prompt**. Run as admin if requested.
* To switch to another drive in the prompt: type the driver letter then `:`.

#### Preparing

1. There are 2 ways to download and install the dependencies:
   1. Using just-install:
      1. Run the command prompt as **admin** - it's easier to see if an installation failed this way.
      2. Type `msiexec.exe /i https://just-install.github.io/stable/just-install.msi`
      3. Type `just-install exeproxy 7zip git bcc` (exeproxy has to come first! Do this even if you have those programs installed already).
   2. Manually:
      1. Download and install [7zip](https://www.7-zip.org/download.html) to its default folder (select to download the x64 on x64 systems and x86 on x86 systems).
      2. Download and extract [bcc](https://github.com/wormt/bcc/releases) to %PROGRAMFILES%/bcc (for compatibility with Just-install).
      3. Download and install Git for Windows in default folder, making sure it uses Windows command prompt with just git. Download [git-for-windows](https://git-scm.com/download/win). Just do the default options, but you may select your favorite text editor.
2. Add the paths to the PATH environment variable:
   1. Open the Advanced System Settings dialog:
      1. Either go to `Control Panel/System` then `Advanced System Settings` OR
      2. If you have search enabled, you can type: `View Advanced System Settings` (both ways go directly to dialog).
   2. Go `Advanced` tab.
   3. Click `Environment Variables...`.
   4. On the `System variables` section, select the `Path` environment variable, then click `Edit...`
   5. Add the paths:
      1. On Windows 10 OCT/18 build and newer: Press `New`, then type: %PROGRAMFILES%\7-Zip`. Press `New` again, and type `%SYSTEMDRIVE%\Shims`. Now press `OK`.
      2. On any other Windows: On the edit field, add: `;%PROGRAMFILES%\7-Zip;%SYSTEMDRIVE%\Shims`, then click `OK`.
   6. Then click `OK`, and click `OK` again.

#### To continue, there are 2 useable git tools:
#### Either Git-for-windows (default)
Git-for-windows is for those who like the command prompt.

##### Installing DnD
1. Run the command prompt.
2. CD to the folder where you would like DnD to be downloaded: `cd path/to/contain`
3. Type the following to download the files: `git clone https://github.com/Doruk-Aksoy/DnD.git`

##### Updating DnD (not needed after installing)
1. Run the command prompt.
2. CD to the DnD folder: `cd path/to/DnD`
3. Type `git pull` - If it doesn't fail, go to step 5.
4. It failed, changes need to be reset. It will succeed if all following is typed:
```
git fetch --all
git reset --hard origin/master
git pull
```
5. Rebuild - follow **How to Build** instructions again.

#### Or Tortoise Git
Provides very nice context menus.

##### Preparing

- **If you used Just-install:** Go to cmd prompt with admin priviledges, type `just-install tortoisegit` (if you don't have it installed already).
- **Otherwise:** Download and install [Tortoise Git](https://tortoisegit.org/download/) to its default folder (select to download the x64 on x64 systems and x86 on x86 systems).

##### Installing DnD
1. Open Windows Explorer.
2. Go to folder where you would like dnd to be downloaded.
3. Right click on background (don't right click any items).
4. Select `Git Clone...`
5. On URL field type: `https://github.com/Doruk-Aksoy/DnD`
6. Click `Close` when finished.
 
##### Updating DnD (not needed after installing)
1. Open your DnD folder on Windows Explorer.
2. Right click on the background of the folder, then click Tortoise Git, then click Pull.
3. Click OK.
4. If anything red shows up, something failed. Regardless of the outcome, click close. If succeeded, go to step 10.
5. If something failed, right click Tortoise Git. On the menu that opens, click Fetch. Then click OK. Then click close.
7. Right click again on folder background. Click Tortoise Git. Click Show Log.
8. Select commit with (Master) tag, then right click it, and click "reset to this commit". Then select **Hard** reset type. Then click OK. Then click close.
9. Right click again on folder background. Click Tortoise Git. Click Pull. Click OK. Click close.
10. Rebuild - follow **Building the DnD pk7** instructions again.

#### Building the DnD pk7s
1. First make sure nothing is using the DnD PK7 (like a server or the game client).
2. Run the command prompt.
3. CD to the DnD folder: `cd path/to/DnD`.
4. Type either `test.bat` (if updating files often) or `build.bat` (if releasing).
5. Make sure when creating server/playing offline that you're using these files.

If the test.bat created files that are different to the ones being hosted online (as in, the versionless dnd*.pk7 files) when you know those being hosted are the newest:
- Follow **Updating DnD** section again.
- If it still doesn't work, follow **Preparing** section again (reinstalling the dependencies), but skipping any PATH related instructions.

### Linux
#### Preparing
1. Install `p7zip` and `git` packages.
2. Download [bcc](https://github.com/wormt/bcc/releases), and extract it (easiest to extract it on your home folder).

#### Installing
1. Open the terminal.
2. Type the following to download the files: `git clone https://github.com/Doruk-Aksoy/DnD.git`

#### Updating (not needed after installing)
1. Open the terminal.
2. CD to the DnD folder: `cd path/to/DnD`
3. Do git pull - If it doesn't fail, go to step 5: `git pull`
4. It failed, changes need to be reset. It will succeed if all following is typed:
```
git fetch --all
git reset --hard origin/master
git pull
```
5. Rebuild - follow **How to Build** instructions again.

#### Building the pk7s
1. Open the terminal.
2. CD to the DnD folder: `cd path/to/DnD`
3. Set the bcc environment variable (just needed once in a terminal for any amount of builds). Type - where fullpathtobccexecutable is the full path to executable: `export bcc=fullpathtobccexecutable`
4. Type `./test.sh` (if updating files often) or `./build.sh` (if releasing).
5. Make sure when creating server/playing offline that you're using these files.

## Recommendations
- Add the main DnD folder to the wad paths. This prevents having to move the wad whenever adjusting the source code or new versions.
- If no changes to files are done, you don't have to share the pk7 with anyone. Just let them know of these instructions!