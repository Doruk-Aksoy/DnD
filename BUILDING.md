# Death and Decay
## Building from Source
### Windows
#### How to prepare

**Note:** Remember, whenever using cd at command prompt to switch to the drive where the files are contained, by typing the drive letter then colon ":" as in "d:".

1. Install [7zip](https://www.7-zip.org/download.html) x64 on x64 systems and x86 on x86 systems at the path indicated by installer (otherwise change path in build.bat or test.bat).
2. Download [bcc](https://github.com/wormt/bcc/releases), and extract it.
3. (Optional, but extremely recommended) Set environment variable bcc to the full path to bcc executable. Example: `C:/games/bcc/bcc.exe`. How to do it in detail:
   1. Either go to `Control Panel/System` then `Advanced System Settings` OR
      If you have search enabled, you can type: `View Advanced System Settings`.
   2. Go `Advanced` tab.
   3. Click `Environment Variables...`.
   4. On the `User variables for username` section, click `New...`.
   5. Variable name is `bcc`, then click `Browse File...` and look for the bcc.exe (which is in the bcc folder you unpacked).
   6. Then click `OK`.
4. Install Git for Windows in default folder, making sure it uses Windows command prompt with just git.
   Download [git-for-windows](https://git-scm.com/download/win). Just do the default options, but you may select your favorite text editor. Now pick between the Tortoise Git way, or the git for windows way:
   1. The Tortoise Git way - provides very nice context menus on Windows (Extremely recommended):
      1. Download and install [Tortoise Git](https://tortoisegit.org/download/) (select x64 on x64 systems, otherwise select x86)
      2. Open Windows Explorer.
      3. Go to folder where you would like dnd to be downloaded.
      4. Right click on background (don't right click any items).
      5. Select `Git Clone...`
      6. Under URL type: `https://github.com/Doruk-Aksoy/DnD`
      7. Click `Close` when finished.
   2. The git-for-windows way - for those who like the command prompt.
      1. Run the command prompt.
      2. CD to the folder where you would like DnD to be downloaded: `cd path/to/contain`
      3. Type the follows to download the files: `git clone https://github.com/Doruk-Aksoy/DnD.git`

#### How to build

1. Run the command prompt.
2. CD to the DnD folder: `cd path/to/DnD`
3. (If not done the Optional 3rd preparation step part) Type this (where fullpathtobccexecutable is the full path to bcc executable): `SET bcc=fullpathtobccexecutable.`
4. Run either test.bat (if updating files often) or build.bat (if releasing): `test.bat`
5. Make sure when creating server/playing offline that you're using these files.

#### How to update
1. Run the command prompt.
2. CD to the DnD folder: `cd path/to/DnD`
3. Do git pull - If it doesn't fail, go to step 5: `git pull`
4. It failed, changes need to be reset. It will succeed if all following is typed:
```
git fetch --all
git reset --hard origin/master
git pull
```
5. Rebuild - follow **How to Build** instructions again.

### Linux
#### How to prepare

1. Install `p7zip` and `git` packages.
2. Download [bcc](https://github.com/wormt/bcc/releases), and extract it (easiest to extract it on your home folder).
3. Open the terminal. Type the follows to download the files: `git clone https://github.com/Doruk-Aksoy/DnD.git`

#### How to build
1. Open the terminal.
2. CD to the DnD folder: `cd path/to/DnD`
3. Set the bcc environment variable (just needed once in a terminal for any amount of builds). Type - where fullpathtobccexecutable is the full path to executable: `export bcc=fullpathtobccexecutable`
4. Run the test.sh (if updating files often) or build.sh (if releasing). Type: `./test.sh`
5. Make sure when creating server/playing offline that you're using these files.

#### How to update
1. Run the terminal.
2. CD to the DnD folder: `cd path/to/DnD`
3. Do git pull - If it doesn't fail, go to step 5: `git pull`
4. It failed, changes need to be reset. It will succeed if all following is typed:
```
git fetch --all
git reset --hard origin/master
git pull
```
5. Rebuild - follow **How to Build** instructions again.

## Recommendations
- Add the main DnD folder to the wad paths. This prevents having to move the wad whenever adjusting the source code or new versions.
- If no changes to files are done, you don't have to share the pk7 with anyone. Just let them know of these instructions!