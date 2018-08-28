# Death and Decay
## Building from Source
### Windows
#### Command Prompt Notes
* How to run it:
  * Windows 10: 1. Click the search button and type: `Command Prompt` and click on the desktop app.
  * Windows 7 and below: Click **Start**, then click **Run...**, type `cmd`).
* To switch to another drive in the prompt: type the driver letter then `:`.

#### Preparing
1. Go to https://just-install.it/ and go to cmd promt with admin priviledges.
2. Type `just-install exeproxy 7zip git bcc` (or just install the components you don't have installed - exeproxy has to come first!).

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
1. Go to cmd prompt with admin priviledges, type `just-install tortoisegit` (if you don't have it installed already).

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
10. Rebuild - follow **How to Build** instructions again.

#### Building the DnD pk7s
1. Run the command prompt.
2. CD to the DnD folder: `cd path/to/DnD`
4. Type either `test.bat` (if updating files often) or `build.bat` (if releasing).
5. Make sure when creating server/playing offline that you're using these files.

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