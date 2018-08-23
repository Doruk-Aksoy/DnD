# DEATH AND DECAY
## Introduction
- Welcome to Death and Decay, a monster/weapon replacer mod. If you liked Hard Doom and mods of that type, then you might like this mod. If not, it's still alright, this mod is a bit different compared to the others.
- The very first difference is that your character will constantly be evolving and improving in terms of both the stats and weapon efficiency. You'll be earning credits through monsters kills or completing a few special tasks. (These will be mentioned below) You can later use the hard earned cash to buy some very valuable items.
- This mod incorporates the popular survival features used in servers nowadays such as the revenant tracer fix, shared keys etc.
- Make sure you bind a key to the menu of DnD. It has lots of information regarding your weapons, stats, loadout, the shop etc. A must have!

## Screenshots
<img src="http://i.imgur.com/7Z9BpOY.png"/>
<img src="http://i.imgur.com/dOPqpvL.png"/>

## Releases
Stable builds are located at [Releases section](https://github.com/Doruk-Aksoy/DnD/releases).
Instructions on git building are located [here](./BUILDING.md).

## Support
For any questions concerning the mod, join our [discord](https://discord.gg/Gffvznu).
### Known Bugs
- DM only weapons/items/whatever will still spawn in Coop/Survival. Zandronum bug, don't bother me about this.
### Zandronum account
*  Required for Hardcore/Softcore modes.
1. Make an account here: [Click](http://auth.zandronum.com/)
2. When you are in game for a hardcore server, do "login username password". (The same name and password you entered for 1)
3. When the game prompts you to load your character, you can do "loadme x" where x is your character id.
Remember, your character is NOT saved until the map is officially over. This means, if you spectate at any point before the map ends, your progress is gone. You die, your character is entirely wiped out.
### Creating Server
- Add the pk7 in the following order (on Linux, use proper casing - if building from source using build.bat/sh, remember to use the versioned files):
```
map packs...
dnd.pk7
dnd_monsters.pk7
dnd_onlyammo.pk7
```

- Flags for servers:

```
DMFlags = 3227652
DMFlags2 = 256
DMFlags3 = 1104
CompatFlags2 = 2
```

- **BIG WARNING:** Remove death exit only maps from rotations in hardcore mode! Your characters can die and will be lost!