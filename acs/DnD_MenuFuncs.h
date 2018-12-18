#include "DnD_MenuTables.h"

void SetPage(int option, bool useSound) {
	SetInventory("MenuUD", 0);
	SetInventory("MenuOption", option);
	if(useSound)
		LocalAmbientSound("RPG/MenuChoose", 127);
}

void UpdateMenuPosition(int option) {
	if(CheckInventory("MenuOption") != option) {
		menu_stack_T? stack = GetMenuStack(PlayerNumber());
		PushStack(PlayerNumber(), option);
	}
	SetPage(option, true);
}

void UpdateMenuPosition_NoSound(int option) {
	if(CheckInventory("MenuOption") != option) {
		menu_stack_T? stack = GetMenuStack(PlayerNumber());
		PushStack(PlayerNumber(), option);
	}
	SetPage(option, false);
}

void UpdateMenuFromStack(int option) {
	SetPage(option, true);
}

void ReturnToMain() {
	SetInventory("MenuOption", MENU_MAIN);
	SetInventory("MenuPosX", 0);
	LocalAmbientSound("RPG/MenuChoose", 127);
}

// includes left right shortcuts
// works clientside
void ListenInput(int listenflag, int condx_min, int condx_max) {
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS);
	int obpress = GetPlayerInput(-1, INPUT_OLDBUTTONS);
	int curposx = CheckInventory("MenuPosX");
	bool p = 0;
	// if waiting for sell confirmation do not let movement in menu
	if(!CheckInventory("DnD_SellConfirm")) {
		if(listenflag & LISTEN_LEFT) {
			if(!(listenflag & LISTEN_FASTLR))
				p = IsButtonPressed(bpress, obpress, settings[1][0]);
			else
				p = IsButtonHeld(bpress, settings[1][0]) && !CheckInventory("Menu_LRCooldown");
			if(p) {
				if(curposx > condx_min) {
					TakeInventory("MenuPosX", 1);
					LocalAmbientSound("RPG/MenuMove", 127);
					GiveInventory("Menu_LRCooldown", 1);
				}
				SetInventory("MenuInput", DND_MENUINPUT_PREVBUTTON);
			}
		}
		if(listenflag & LISTEN_RIGHT) {
			if(!(listenflag & LISTEN_FASTLR))
				p = IsButtonPressed(bpress, obpress, settings[3][0]);
			else
				p = IsButtonHeld(bpress, settings[3][0]) && !CheckInventory("Menu_LRCooldown");
			if(p) {
				if(curposx < condx_max) {
					GiveInventory("MenuPosX", 1);
					LocalAmbientSound("RPG/MenuMove", 127);
					GiveInventory("Menu_LRCooldown", 1);
				}
				SetInventory("MenuInput", DND_MENUINPUT_NEXTBUTTON);
			}
		}
	}
	
	if(!CheckInventory("DnD_ClickTicker")) {
		if(IsButtonPressed(bpress, obpress, BT_USE) || IsButtonPressed(bpress, obpress, BT_ATTACK)) {
			GiveInventory("DnD_ClickTicker", 1);
			if(CheckInventory("DnD_SellConfirm"))
				ACS_NamedExecuteAlways("DnD Menu Sell Popup Clear", 0);
			SetInventory("MenuInput", DND_MENUINPUT_LCLICK);
		}
		else if(IsButtonPressed(bpress, obpress, BT_ALTATTACK)) {
			GiveInventory("DnD_ClickTicker", 1);
			SetInventory("MenuInput", DND_MENUINPUT_RCLICK);
		}
	}
}

// Meant to be used entirely clientside only, for scrolling up and down (used when server doesnt need to know about this)
void ListenScroll(int condx_min, int condx_max) {
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS);
	// up is 1, down is 2
	// opposite buttons because view should go up
	if(IsButtonHeld(bpress, settings[0][0])) {
		if(ScrollPos < condx_max)
			++ScrollPos;
		SetInventory("MenuUD", 1);
	}
	if(IsButtonHeld(bpress, settings[2][0])) {
		if(ScrollPos > condx_min)
			--ScrollPos;
		SetInventory("MenuUD", 2);
	}
}

void ShowPopup(int popupid, bool isSell, int activebox) {
	ACS_NamedExecuteAlways("DnD Menu Popup", 0, Playernumber() | (popupid << 16), isSell, activebox);
}

void ShowActorPopup(int pnum, int popupid, bool isSell, int activebox) {
	ACS_NamedExecuteAlways("DnD Menu Popup", 0, pnum | (popupid << 16), isSell, activebox);
}

int CalculateWisdomBonus(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_EXPGAIN_INCREASE) + GetDataFromOrbBonus(pnum, OBI_WISDOMPERCENT, -1) + CheckInventory("Perk_Wisdom") * BASE_WISDOM_GAIN;
}

int CalculateGreedBonus(int pnum) {
	return GetPlayerAttributeValue(pnum, INV_CREDITGAIN_INCREASE) + GetDataFromOrbBonus(pnum, OBI_GREEDPERCENT, -1) + CheckInventory("Perk_Greed") * BASE_WISDOM_GAIN;
}

str CurrentWeapon() {
	for(int i = 0; i < MAXWEPS; ++i)
		if(CheckWeapon(Weapons[i][WEAPON_TAG]))
			return Weapons[i][WEAPON_TAG];
	return " ";
}

int IsSpecialFixWeapon(int id) {
    for(int i = 0; i < MAX_SPECIALAMMOFIX_WEAPONS; ++i)
        if(SpecialAmmoFixWeapons[i][0] == id)
            return i;
    return -1;
}

int GetItemFlags(int itemid) {
	int type = GetItemType(itemid);
	switch(type) {
		case TYPE_WEAPON:
		return WeaponDrawInfo[itemid - SHOP_WEAPON_BEGIN].flags;
		case TYPE_AMMO:
		return AmmoDrawInfo[itemid - SHOP_FIRSTAMMO_INDEX].flags;
		case TYPE_ARTI:
		return ArtifactDrawInfo[itemid - SHOP_FIRSTARTI_INDEX].flags;
		case TYPE_ABILITY:
		return AbilityDrawInfo[itemid - SHOP_ABILITY1_BEGIN].flags;
		case TYPE_ACCOUNT:
		return OBJ_HASCHOICE | OBJ_RESEARCH;
	}
	return 0;
}

int CheckItemRequirements (int req_id, int constraint, int flags) {
	int res = !(flags & OBJ_RESEARCH_ATLEASTONE); // if it has at least one flag set, the logic is inverted here
	for(int i = 0; i < MAX_RESEARCH_REQUIREMENTS; ++i) {
		if(ItemResearchRequirements[req_id][i] == -1)
			continue;
		if(CheckResearchStatus(ItemResearchRequirements[req_id][i]) < constraint) {
			if(flags & OBJ_RESEARCH_ATLEASTONE)
				continue;
			res = 0;
			break;
		}
		else if(flags & OBJ_RESEARCH_ATLEASTONE) // found one, just return
			return 1;
	}
	return res;
}

str GetTextWithResearch(str success, str fail, int req_id, int constraint, int flags) {
	if(req_id == -1 || CheckItemRequirements(req_id, constraint, flags))
		return success;
	return fail;
}

int ParseInt(str InputString) {
    int ReturnValue = 0;
    int TempValue = 0;
    int TempInt = 0;
    int char = -65536;
    
    if(InputString == "") 
		return 0;
    
    for(int i = 0; i < StrLen(InputString); ++i) {
        char = GetChar(InputString, i);
        if(char != '1' && char != '2' &&
            char != '3' && char != '4' &&
            char != '5' && char != '6' &&
            char != '7' && char != '8' &&
            char != '9' && char != '0')
        { return 0; }
        else {
            switch(char) {
                case '0':
                    TempInt = 0;
                break;
                
                case '1':
                    TempInt = 1;
                break;
                
                case '2':
                    TempInt = 2;
                break;
                
                case '3':
                    TempInt = 3;
                break;
                
                case '4':
                    TempInt = 4;
                break;
                
                case '5':
                    TempInt = 5;
                break;
                
                case '6':
                    TempInt = 6;
                break;
                
                case '7':
                    TempInt = 7;
                break;
                
                case '8':
                    TempInt = 8;
                break;
                
                case '9':
                    TempInt = 9;
                break;
            }
            
            for(int x = ((StrLen(InputString) - 1) - i); x > 0; --x) { TempInt *= 10; }
            TempValue += TempInt;
        }
    }
    ReturnValue = TempValue;
    return ReturnValue;
}

void ShowNeedResearchPopup() {
	ShowPopup(POPUP_NEEDRESEARCH, false, 0);
}

void DrawCornerText(int opt, int boxid) {
	str toshow = "";
	if(CheckInventory("ActiveMainBox") != MAINBOX_NONE)
		toshow = MainBoxTexts[CheckInventory("ActiveMainBox")];
	else if(opt == MENU_PERK) {
		switch (boxid) {
			case MBOX_1:
				toshow = "Increase Damage by 5%";
			break;
			case MBOX_2:
				toshow = "Reduce Damage Taken by 3.5%";
			break;
			case MBOX_3:
				toshow = "Increase exp gain by 10%";
			break;
			case MBOX_4:
				toshow = "Increase credit gain by 10%";
			break;
			case MBOX_5:
				toshow = "Increase Healing by 10%";
			break;
			case MBOX_6:
				toshow = "Increase Ammo Gain by 10%";
			break;
			case MBOX_7:
				toshow = "Gives 2% Chance to Critical Strike";
			break;
            case MBOX_8:
                toshow = "Gives 10% extra damage to your crits";
            break;
			case MBOX_9:
				toshow = "15% increased drop chance";
			break;
			default:
				toshow = "";
			break;
		}
	}
	else if(opt == MENU_SHOP_TALENT)
		toshow = StrParam(s:"Talent Points: \cd", d:CheckInventory("TalentPoint"));
	DeleteTextRange(RPGMENUDAMAGETYPEID, RPGMENUDAMAGETYPEID + 3);
	SetHudClipRect(62, 24, 72, 96, 72, 1);
	HudMessage(s:toshow; HUDMSG_PLAIN, RPGMENUHELPCORNERID, CR_CYAN, 62.1, 32.1, 0.0, 0.0);
	SetHudClipRect(0, 0, 0, 0);
}

void DrawDamageTypes(int opt, int posy, int req_id, int constraint, int flags) {
	DeleteTextRange(RPGMENUDAMAGETYPEID, RPGMENUDAMAGETYPEID + 3);
	if(req_id == -1 || CheckItemRequirements(req_id, constraint, flags)) {
		int j = 0;
		for(int i = 0; i < MAX_DAMAGE_TYPES; ++i) {
			if(IsSet(WeaponDamageTypes[req_id], i)) {
				SetFont(DamageTypeIcons[i]);
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUDAMAGETYPEID + j, CR_CYAN, 62.1 + (j++) * 30.0, 84.1, 0.0, 0.0);
			}
		}
		SetFont("SMALLFONT");
	}
}

int GetWeaponEnchantDisplay(int pnum, int wep) {
	return Player_Weapon_Infos[pnum][wep].enchants + GetDataFromOrbBonus(pnum, OBI_WEAPON_ENCHANT, wep);
}

int GetCritChanceDisplay(int pnum, int wep) {
	int base = (100 * Player_Weapon_Infos[pnum][wep].wep_bonuses[WEP_BONUS_CRIT].amt);
	base += (100 * GetDataFromOrbBonus(pnum, OBI_WEAPON_CRIT, wep));
	base += FixedMul(base, Player_Weapon_Infos[pnum][wep].wep_bonuses[WEP_BONUS_CRITPERCENT].amt);
	// truncate
	return ftrunc(base);
}

bool HasCritDamageBonus(int pnum, int wep) {
	return Player_Weapon_Infos[pnum][wep].wep_bonuses[WEP_BONUS_CRITDMG].amt + GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, wep);
}

int GetCritDamageDisplay(int pnum, int wep) {
	int base = 100.0 + 100 * (Player_Weapon_Infos[pnum][wep].wep_bonuses[WEP_BONUS_CRITDMG].amt + GetDataFromOrbBonus(pnum, OBI_WEAPON_CRITDMG, wep));
	return ftrunc(base);
}

int GetBonusDamageDisplay(int pnum, int wep) {
	int base = 100 * Player_Weapon_Infos[PlayerNumber()][wep].wep_bonuses[WEP_BONUS_DMG].amt;
	base += 100 * GetDataFromOrbBonus(PlayerNumber(), OBI_WEAPON_DMG, wep);
	return ftrunc(base);
}

int GetDamageTypeBonus(int pnum, int dtype) {
	return MapTalentToPercentBonus(pnum, dtype) + GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, dtype);
}

int GetWeaponPage(int boxid) {
	switch(boxid) {
		case MBOX_1:
		return MENU_SHOP_WEAPON1;
		case MBOX_2:
		return MENU_SHOP_WEAPON2;
		case MBOX_3:
		return MENU_SHOP_WEAPON3_1;
		case MBOX_4:
		return MENU_SHOP_WEAPON4_1;
		case MBOX_5:
		return MENU_SHOP_WEAPON5_1;
		case MBOX_6:
		return MENU_SHOP_WEAPON6_1;
		case MBOX_7:
		return MENU_SHOP_WEAPON7;
		case MBOX_8:
		return MENU_SHOP_WEAPON8;
	}
	return 0;
}

int GetWeaponBeginIndexFromOption(int curopt) {
	switch(curopt) {
		case MENU_SHOP_WEAPON1:
		return SHOP_WEAPON1_BEGIN;
		case MENU_SHOP_WEAPON2:
		return SHOP_WEAPON2_BEGIN;
		case MENU_SHOP_WEAPON3_1:
		return SHOP_WEAPON31_BEGIN;
		case MENU_SHOP_WEAPON3_2:
		return SHOP_WEAPON32_BEGIN;
		case MENU_SHOP_WEAPON4_1:
		return SHOP_WEAPON41_BEGIN;
		case MENU_SHOP_WEAPON4_2:
		return SHOP_WEAPON42_BEGIN;
		case MENU_SHOP_WEAPON5_1:
		return SHOP_WEAPON51_BEGIN;
		case MENU_SHOP_WEAPON5_2:
		return SHOP_WEAPON52_BEGIN;
		case MENU_SHOP_WEAPON6_1:
		return SHOP_WEAPON61_BEGIN;
		case MENU_SHOP_WEAPON6_2:
		return SHOP_WEAPON62_BEGIN;
		case MENU_SHOP_WEAPON7:
		return SHOP_WEAPON7_BEGIN;
		case MENU_SHOP_WEAPON8:
		return SHOP_WEAPON8_BEGIN;
	}
	return 0;
}

int GetWeaponEndIndexFromOption(int curopt) {
	switch(curopt) {
		case MENU_SHOP_WEAPON1:
		return SHOP_WEAPON_SLOT1END;
		case MENU_SHOP_WEAPON2:
		return SHOP_WEAPON_SLOT2END;
		case MENU_SHOP_WEAPON3_1:
		return SHOP_WEAPON_SLOT31END;
		case MENU_SHOP_WEAPON3_2:
		return SHOP_WEAPON_SLOT32END;
		case MENU_SHOP_WEAPON4_1:
		return SHOP_WEAPON_SLOT41END;
		case MENU_SHOP_WEAPON4_2:
		return SHOP_WEAPON_SLOT42END;
		case MENU_SHOP_WEAPON5_1:
		return SHOP_WEAPON_SLOT51END;
		case MENU_SHOP_WEAPON5_2:
		return SHOP_WEAPON_SLOT52END;
		case MENU_SHOP_WEAPON6_1:
		return SHOP_WEAPON_SLOT61END;
		case MENU_SHOP_WEAPON6_2:
		return SHOP_WEAPON_SLOT62END;
		case MENU_SHOP_WEAPON7:
		return SHOP_WEAPON_SLOT7END;
		case MENU_SHOP_WEAPON8:
		return SHOP_WEAPON_SLOT8END;
	}
	return 0;
}

int GetWeaponSlotFromWeaponID(int wepid) {
	return ParseInt(Weapons[wepid][WEAPON_SLOT]);
}

str GetWeaponToTake(int wepid) {
	int slot = GetWeaponSlotFromWeaponID(wepid);
	if(slot != 8) {
		if(!ParseInt(ShopItemNames[wepid][SHOPNAME_TYPE])) {
			if(wepid >= SHOP_WEAPON1CSAW_BEGIN && wepid <= SHOP_WEAPON1CSAW_END)
				return ShopWeaponTake[0];
			else if(wepid >= SHOP_WEAPON2PISTOL_BEGIN && wepid <= SHOP_WEAPON2PISTOL_END)
				return ShopWeaponTake[1];
			else if(wepid >= SHOP_WEAPON3SG_BEGIN && wepid <= SHOP_WEAPON3SG_END) {
				// little hack here to fix special ammos after refunding weapon
				SetInventory("SpecialAmmoMode_3", AMMO_BASICSHELL);
				return ShopWeaponTake[2];
			}
			else if(wepid >= SHOP_WEAPON3SSG_BEGIN && wepid <= SHOP_WEAPON3SSG_END)
				return ShopWeaponTake[3];
			else if(wepid >= SHOP_WEAPON4CG_BEGIN && wepid <= SHOP_WEAPON4CG_END) {
				SetInventory("SpecialAmmoMode_4", AMMO_BULLET);
				return ShopWeaponTake[4];
			}
			else if(wepid >= SHOP_WEAPON5RL_BEGIN && wepid <= SHOP_WEAPON5RL_END)
				return ShopWeaponTake[5];
			else if(wepid >= SHOP_WEAPON6PL_BEGIN && wepid <= SHOP_WEAPON6PL_END)
				return ShopWeaponTake[6];
			else if(wepid >= SHOP_WEAPON7BFG_BEGIN && wepid <= SHOP_WEAPON7BFG_END)
				return ShopWeaponTake[7];
		}
		return "";
	}
	return "";
}

void HandleWeaponPropertyImages(int curopt, int boxid, int ypos) {
	int wid = GetWeaponBeginIndexFromOption(curopt);
	int imgcount = 0;
	wid += boxid - 1;
	DeleteTextRange(RPGMENUWEAPONPANELID - 8, RPGMENUWEAPONPANELID - 1);
	if(CheckItemRequirements(wid, RES_KNOWN, WeaponDrawInfo[wid - SHOP_WEAPON_BEGIN].flags)) {
		SetHudSize(HUDMAX_X * 2, HUDMAX_Y * 2, 1);
		for(int i = 0; i < MAX_WEAPON_PROPERTIES; ++i) {
			if(IsSet(WeaponProperties[wid], i)) {
				SetFont(WeaponPropertyImages[i]);
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUWEAPONPANELID - i - 1, -1, -64.0 + (imgcount & 1) * 64.0, -160.0 + ypos * 8.0 + (imgcount / 2) * 40.0, 0.0, 0.0);
				++imgcount;
			}
		}
	}
}

void HandleWeaponInfoPanel(int curopt, int animcounter, int boxid) {
	static int ypos = 0;
	bool mode = 0;
	// pull down = 1
	if(curopt >= MENU_SHOP_WEAPON1 && curopt <= MENU_SHOP_WEAPON8)
		mode = 1;
	if(mode) {
		if(ypos < 25)
			++ypos;
	}
	else {
		if(ypos > 0)
			--ypos;
	}
	SetFont("DND_PANL");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUWEAPONPANELID, -1, -12.0, -64.0 + ypos * 4.0, 0.0, 0.0);
	
	if(boxid != MAINBOX_NONE)
		HandleWeaponPropertyImages(curopt, boxid, ypos);
	else
		DeleteTextRange(RPGMENUWEAPONPANELID - 8, RPGMENUWEAPONPANELID - 1);
	
	SetFont("SMALLFONT");
}

void ShowWeaponIcon(int wep, int i, int k) {
	SetHudSize(640, 480, 1);
	SetFont(Weapons[wep][WEAPON_ICON]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - DND_MENU_LOADOUTWEPITEMS * i - 5, CR_WHITE, 280.1, 126.1 + 72.0 * i + 24.0 * k + 6.0 * ScrollPos, 0.0, 0.0);
	SetFont("SMALLFONT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void ShowDamageTypeIcon(int dmg) {
	SetHudSize(640, 480, 1);
	SetFont(DamageTypeIcons[dmg]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_HELPTEXT_DAMAGETYPES - dmg, CR_WHITE, 384.1, 88.1 + 120.0 * dmg + 6.0 * ScrollPos, 0.0, 0.0);
	SetFont("SMALLFONT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void ShowWeaponPropertyIcon(int id) {
	int offset = 0.0;
	SetHudSize(640, 480, 1);
	SetHudClipRect(192, 72, 320, 384, 320, 1);
	SetFont(WeaponPropertyImages[id]);
	
	if(id == 2)
		offset = -24.0;
	else if(id == 3)
		offset = -16.0;
	else if(id == 6)
		offset = -8.0;
	
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_WEAPON_PROPERTIES - id, CR_WHITE, 436.1, 76.1 + offset + 104.0 * id + 6.0 * ScrollPos, 0.0, 0.0);
	SetFont("SMALLFONT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void ShowOrbIcon(int id) {
	SetFont(Item_Images[id + ITEM_IMAGE_ORB_BEGIN]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_ORBS - id - 2, CR_WHITE, 237.4, 60.1 + 96.0 * id + 8.0 * ScrollPos, 0.0, 0.0);
	SetFont("SMALLFONT");
}

void ShowLegendaryMonsterIcon(int id, int j) {
	SetFont(LegendaryMonsterIcons[id]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * id - 1, CR_WHITE, 256.4, 56.1 + 144.0 * j + 4.0 * ScrollPos, 0.0, 0.0);
	SetFont("SMALLFONT");
}

void ShowBobby() {
	SetHudSize(1280, 864, 1);
	SetFont("BBYSNC");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 69, CR_WHITE, 848.4, 140.1, 0.0, 0.0);
	SetFont("SMALLFONT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

str GetWeaponShopIcon(int id) {
	// skip classic weapon icons
	int skip = 0;
	if(id >= SHOP_WEAPON1_BEGIN)
		skip += 2; // fist & chainsaw
	if(id >= SHOP_WEAPON2_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON31_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON3SSG_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON41_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON51_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON61_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON7_BEGIN)
		skip += 1;
	return Weapons[id + skip][WEAPON_ICON];
}

int GetItemBeginIndex(int opt) {
	if(opt >= MENU_SHOP_WEAPON1 && opt <= MENU_SHOP_WEAPON8)
		return WeaponBeginIndexes[opt - MENU_SHOP_WEAPON1];
	if(opt >= SHOP_FIRSTAMMO_PAGE && opt <= SHOP_LASTAMMO_PAGE)
		return MenuAmmoIndexMap[opt - SHOP_FIRSTAMMO_PAGE][0];
	if(opt == MENU_SHOP_ARMOR2)
		return PAGE1_ARMOR_COUNT;
	return 0;
}

void DrawHelpCorner (int opt, int boxid) {
	str toshow = "";
	int beginindex = 0;
	
	if(CheckInventory("ActiveMainBox") != MAINBOX_NONE || (opt == MENU_PERK) || (opt == MENU_SHOP_TALENT)) {
		DrawCornerText(opt, boxid);
		return;
	}
	else
		DeleteText(RPGMENUHELPCORNERIDMAIN);
	if(boxid == MAINBOX_NONE) {
		DeleteTextRange(RPGMENUDAMAGETYPEID, RPGMENUDAMAGETYPEID + 3);
		HudMessage(s:""; HUDMSG_PLAIN, RPGMENUHELPCORNERID, CR_CYAN, 92.1, 56.1, 0.0, 0.0);
		return;
	}
	--boxid; // convert to 0 based
	auto CurrentPane = GetPane();
	if(boxid < CurrentPane.cursize) {
		// some exceptions here
		if(opt >= SHOP_FIRSTAMMO_PAGE && opt <= SHOP_LASTAMMO_PAGE) {
			int shopindex = 0;
			if(opt != MENU_SHOP_AMMO_SPECIAL1) {
				shopindex = MenuAmmoIndexMap[opt - SHOP_FIRSTAMMO_PAGE][boxid] - SHOP_FIRSTAMMO_INDEX;
				toshow = GetTextWithResearch(AmmoInfo_Str[opt - SHOP_FIRSTAMMO_PAGE][boxid + beginindex][AMMOINFO_ICON], "", AmmoDrawInfo[shopindex].res_id, RES_KNOWN, AmmoDrawInfo[shopindex].flags);
			}
			else {
				// skip ahead previous ammos
				shopindex = MAXSHOPNORMALAMMOS + boxid;
				toshow = GetTextWithResearch(SpecialAmmoInfo_Str[boxid][AMMOINFO_ICON], "", AmmoDrawInfo[shopindex].res_id, RES_KNOWN, AmmoDrawInfo[shopindex].flags);
			}
		}
		else {
			beginindex = GetItemBeginIndex(opt);
			if(opt >= MENU_SHOP_WEAPON1 && opt <= MENU_SHOP_WEAPON8) {
				DrawDamageTypes(opt, boxid, boxid + beginindex, RES_KNOWN, WeaponDrawInfo[boxid + beginindex].flags); // draw damage types as well
				toshow = GetTextWithResearch(GetWeaponShopIcon(boxid + beginindex), "", WeaponDrawInfo[boxid + beginindex].res_id, RES_KNOWN, WeaponDrawInfo[boxid + beginindex].flags);
			}
			else if(opt == MENU_SHOP_ARTIFACT)
				toshow = GetTextWithResearch(ArtifactInfo[boxid][ARTI_ICON], "", ArtifactDrawInfo[boxid].res_id, RES_KNOWN, ArtifactDrawInfo[boxid].flags);
			else if(opt >= MENU_SHOP_ARMOR1 && opt <= MENU_SHOP_ARMOR2)
				toshow = GetTextWithResearch(ArmorImages[boxid + beginindex], "", ArmorDrawInfo[boxid + beginindex].res_id,  RES_KNOWN, ArmorDrawInfo[boxid + beginindex].flags);
		}
	}
	else
		toshow = "";
	if(StrLen(toshow)) {
		SetFont(toshow);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUHELPCORNERID, CR_CYAN, 92.1, 56.1, 0.0, 0.0);
		SetFont("SMALLFONT");
	}
}

void ShowAccessoryIcon(int acc, int i) {
	SetHudSize(640, 480, 1);
	SetFont(AccessoryInfo[acc][ACCESSORY_ICON]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 3 * i - 2, CR_WHITE, 421.4, 75.1 + 48.0 * i, 0.0, 0.0);
	SetFont("SMALLFONT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

int CurrentAccessoryIndex () {
	return CheckInventory("Accessory_Index");
}

// gets item type
int GetItemType(int id) {
	if(id <= SHOP_LASTWEP_INDEX)
		return TYPE_WEAPON;
	else if(id <= SHOP_LASTAMMO_SPECIALINDEX)
		return TYPE_AMMO;
	else if(id <= SHOP_LASTABILITY_INDEX)
		return TYPE_ABILITY;
	else if(id <= SHOP_LASTTALENT_INDEX)
		return TYPE_TALENT;
	else if(id <= SHOP_LASTARMOR_INDEX)
		return TYPE_ARMOR;
	else if(id <= SHOP_ACCOUNT_END)
		return TYPE_ACCOUNT;
	else
		return TYPE_ARTI;
	return TYPE_WEAPON;
}

int ShopScale(int amount, int id) {
	int type = GetItemType(id);
	switch(type) {
		case TYPE_WEAPON:
		return amount * Clamp_Between(GetCVar("dnd_shop_wep_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_AMMO:
		return amount * Clamp_Between(GetCVar("dnd_shop_ammo_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_ABILITY:
		return amount * Clamp_Between(GetCVar("dnd_shop_ability_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_TALENT:
		return amount * Clamp_Between(GetCVar("dnd_shop_talent_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_ARTI:
		return amount * Clamp_Between(GetCVar("dnd_shop_artifact_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_ARMOR:
		return amount * Clamp_Between(GetCVar("dnd_shop_armor_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_ACCOUNT:
		return amount * Clamp_Between(GetCVar("dnd_shop_account_scale"), 1, SHOP_SCALE_MAX);
	}
	return 1;
}

int GetShopPrice (int id, int priceflag) {
	int res = 0, chr = 0, shop_scale = Clamp_Between(GetCVar("dnd_shop_scale"), 1, SHOP_SCALE_MAX);
	if(id < MAXSHOPITEMS)
		res = ShopInfo[id][SHOPINFO_PRICE] * shop_scale;
	res = ShopScale(res, id);
	if(GetItemType(id) == TYPE_TALENT) {
		res += TALENT_COST_INCREASE * CheckInventory(TalentNames[id - SHOP_TALENT_BEGIN][TALENT_TAG]) * shop_scale;
		if(CheckInventory(TalentNames[id - SHOP_TALENT_BEGIN][TALENT_TAG]) >= TALENT_SCALE_DOUBLER_MARK)
			res *= 2 * CheckInventory(TalentNames[id - SHOP_TALENT_BEGIN][TALENT_TAG]) / TALENT_SCALE_DOUBLER_MARK;
	}
	if(priceflag & PRICE_CHARISMAREDUCE) {
		chr = Clamp_Between(GetCharisma(), 0, DND_STAT_FULLMAX);
		if(chr > 100)
			res -= res / 2 + (res * (chr - 100)) / (100 * CHARISMA_REDUCE_AFTER100);
		else
			res -= (res * chr) / (100 * CHARISMA_REDUCE);
	}
	return res;
}

int GetMenuTalentBonus(int posy) {
	if(posy != TALENT_OCCULT)
		return DND_TALENT_INCREASE + DND_DEX_GAIN * CheckInventory("PSTAT_Dexterity");
	return DND_TALENT_INCREASE + DND_INT_GAIN * CheckInventory("PSTAT_Intellect");
}

// returns 0 for buy being possible
bool CanResearch(int respage, int posx) {
	int status = CheckResearchStatus(ResearchInfo[respage][posx].res_id);
	bool finish_check = status != RES_DONE;
	bool found_check = status == RES_KNOWN;
	bool budget_check = CheckInventory("Budget") >= ResearchInfo[respage][posx].res_cost;
	if(finish_check && found_check && budget_check)
		return false;
	GiveInventory("DnD_PopupError", 1);
	if(!finish_check)
		SetInventory("DnD_PopupId", POPUP_ALREADYRESEARCHED);
	else if(!found_check)
		SetInventory("DnD_PopupId", POPUP_NEEDDISCOVER);
	else
		SetInventory("DnD_PopupId", POPUP_NOBUDGET);
	return true;
}

// returns 0 for buy being possible, read end of function for other details
int CanTrade (int id, int tradeflag, int price) {
	int credit = CheckInventory("Credit"), cond1, cond2, cond3 = 1, isammo = 0, res = 0, type = 0;
	
	if(!price)
		price = GetShopPrice(id, PRICE_CHARISMAREDUCE);
		
	cond1 = credit >= price;
		
	str item = "";
	str wepcheck = "";
	
	type = GetItemType(id);
	
	if(type == TYPE_ARTI)
		item = ArtifactInfo[id - SHOP_FIRSTARTI_INDEX][ARTI_NAME]; // put it in the artifact info range
	else if(type == TYPE_TALENT)
		item = ShopItemNames[id][SHOPNAME_ITEM];
	else if(type == TYPE_ARMOR)
		item = "Armor";
	else {
		item = ShopItemNames[id][SHOPNAME_ITEM];
		wepcheck = ShopItemNames[id][SHOPNAME_CONDITION];
	}
	
	if(tradeflag & TRADE_BUY) {
		if(type == TYPE_AMMO) // ammo
			cond2 = (CheckInventory(item) < GetAmmoCapacity(item));
		else if(type == TYPE_TALENT) // talent
			cond2 = CheckInventory(item) < TALENT_CAP && CheckInventory("TalentPoint");
		else if(type == TYPE_ARMOR) // armor
			cond2 = CheckInventory(item) < GetArmorSpecificCap(ArmorBaseAmounts[id - SHOP_FIRSTARMOR_INDEX + 1]);
		else if(type != TYPE_WEAPON && type != TYPE_ABILITY) { // item
			if(id != SHOP_ARTI_BACKPACK)
				cond2 = (CheckInventory(item) < ShopInfo[id][SHOPINFO_MAX]) && !IsSet(CheckInventory("DnD_Artifact_MapBits"), id - SHOP_FIRSTARTI_INDEX);
			else
				cond2 = ACS_ExecuteWithResult(994, 0) && !IsSet(CheckInventory("DnD_Artifact_MapBits"), id - SHOP_FIRSTARTI_INDEX);
		}
		else { // weapon
			cond3 = !CheckInventory(item);
			cond2 = cond3 && !CheckInventory(wepcheck);
		}
		
		if(cond1) { // I have credits
			if(!cond3)
				res = POPUP_ALREADYOWN;
			else if(!cond2) { // got credit but maxed
				if(type == TYPE_TALENT && CheckInventory(item) < TALENT_CAP) // we don't have the points
					res = POPUP_NOTALENTPOINT;
				else
					res = POPUP_CANTBUY;
			}
		}
		else {
			if(!cond3) // no credits but already owning the item
				res = POPUP_ALREADYOWN;
			else if(!cond2)
				res = POPUP_CANTBUY;
			else
				res = POPUP_NOFUNDS; // no credits
		}
		
		if(res) {
			GiveInventory("DnD_PopupError", 1);
			SetInventory("DnD_PopupId", res);
		}
	}
	else { // SELL
		res = CheckInventory(item); // if you have the weapon, you can sell it so return 1 if you have the item
	}

	return res;
}

void DrawToggledLabel(str label, int afterlabel, int boxid, int boxcheck, int drawid, str color, str activecolor, int hudx, int hudy) {
	if(boxid == boxcheck)
		color = activecolor;
	if(afterlabel != -1)
		HudMessage(s:color, s:label, s:": \c-", d:afterlabel; HUDMSG_PLAIN, drawid, CR_WHITE, hudx, hudy, 0.0, 0.0);
	else
		HudMessage(s:color, s:label, s:": "; HUDMSG_PLAIN, drawid, CR_WHITE, hudx, hudy, 0.0, 0.0);
}

// Draws a toggled image that changes color depending on given scenarios
// By default, if insufficient credits occur, it will be gray. If credits are sufficient but objectflag has OBJ_HASCHOICE, that means you have to choose between one of the options
// of it's kind. For example, there are 2 weapons that replace the shotgun. You can have only one, so you set this flag, and set choicename to P_Slot3Replaced. One of the two will be
// red if the other is bought. The item that is bought will be green.
void DrawToggledImage(int itemid, int boxid, int onposy, int objectflag, int offcolor, int oncolor, str choicename, int choicecount, int choicecolor) {
	// first of all check research (assuming the player can't own this item without having it researched)
	int res_state = 1;
	if(objectflag & OBJ_RESEARCH) {
		res_state = CheckItemRequirements(itemid, RES_KNOWN, objectflag); // if not known, draw nothing
	}
	int curposy = boxid - 1;
	if(!res_state) {
		if(curposy == onposy) {
			HudMessage(s:"\c[B1]---- N/A ----"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
			HudMessage(s:"\c[B1]--> $ ???"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, CR_WHITE, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
		else {
			HudMessage(s:"\c[G8]---- N/A ----"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
			HudMessage(s:"\c[G8]$ ???"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, CR_WHITE, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
	}
	else {
		res_state = CheckItemRequirements(itemid, RES_DONE, objectflag); // check if it is actually done
		bool sellstate = false;
		int color = offcolor;
		str toshow = "\c[Y5]", colorprefix = "\cj", weptype = ""; // textcolor colors don't work for some reason
		int price = GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
		int curcredit = CheckInventory("Credit");
		str itemtag = ShopItemNames[itemid][SHOPNAME_TAG];
		str itemname = ShopItemNames[itemid][SHOPNAME_ITEM];
		
		if(objectflag & OBJ_RESEARCH && !res_state) {
			toshow = "\c[W3]";
			colorprefix = "\c[W3]";
		}
		else {
			// if not ammo, talent or armor
			if(!(objectflag & (OBJ_AMMO | OBJ_TALENT | OBJ_ARMOR))) {
				// if not artifact and owning it (basically has weapon)
				if(!(objectflag & (OBJ_ARTI | OBJ_ACCOUNT)) && CheckInventory(itemname)) {
					color = oncolor;
					colorprefix = "\c[M3]";
					toshow = "\c[M3]";
					sellstate = true;
				}
				else { // no?
					if(price > curcredit) {
						color = CR_BLACK;
						colorprefix = "\c[G8]";
						toshow = "\c[G8]";
					} // if I have options color others
					else if( (objectflag & OBJ_HASCHOICE && CheckInventory(choicename) == choicecount) || 
							 (itemid == SHOP_ARTI_BACKPACK && !ACS_ExecuteWithResult(994, 0)) ||
							 (objectflag & OBJ_ARTI && IsSet(CheckInventory("DnD_Artifact_MapBits"), itemid - SHOP_FIRSTARTI_INDEX))
						   ) 
					{
						// if has choice and count met or this is a backpack and limit reached on it (backpack limit has to be checked dynamically due to classic backpack cvar)
						color = choicecolor;
						colorprefix = "\c[Q2]";
						toshow = "\c[Q2]";
					}
				}
			}
			else { // ammo, talent or armor
				if(price > curcredit || (objectflag & OBJ_TALENT && !CheckInventory("TalentPoint"))) {
					color = CR_BLACK;
					colorprefix = "\c[G8]";
					toshow = "\c[G8]";
				}
				else if(!(objectflag & OBJ_AMMO) && CheckInventory(choicename) >= choicecount) {
					color = choicecolor;
					colorprefix = "\c[Q2]";
					toshow = "\c[Q2]";
				}
			}
		}
		
		if(objectflag & OBJ_WEP) {
			if(!ParseInt(ShopItemNames[itemid][SHOPNAME_TYPE]))
				weptype = "\c[J7][\c[S7]R\c[J7]] ";
			else
				weptype = "\c[J7][\c[E3]L\c[J7]] ";
		}
		
		if(curposy == onposy) {
			// this part could be grouped into just deciding on a string to use, but I want to keep variable amount low here (already a lot)
			if(objectflag & OBJ_WEP) {
				if(sellstate)
					HudMessage(s:"\c[M1]--> Sells for:\c- $", d:GetShopPrice(itemid, 0) / 2; HUDMSG_PLAIN, RPGMENUITEMID - 41, CR_WHITE, 192.1, 200.1, 0.0, 0.0);
				SetHudClipRect(184, 216, 256, 64, 256, 1);
				if(objectflag & OBJ_USESCROLL)
					HudMessage(s:"* ", s:WeaponExplanation[itemid]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 184.1, 216.1 + 1.0 * ScrollPos, 0.0, 0.0);
				else
					HudMessage(s:"* ", s:WeaponExplanation[itemid]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 184.1, 216.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(objectflag & OBJ_AMMO) {
				SetHudClipRect(192, 224, 256, 64, 256, 1);
				int amt = AmmoCounts[itemid - SHOP_FIRSTAMMO_INDEX];
				HudMessage(s:"* ", s:"Gives \cf", d:amt + ACS_ExecuteWithResult(918, 0, 1, amt), s:"\c- ", s:AmmoExplanation[itemid - SHOP_FIRSTAMMO_INDEX]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 232.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(objectflag & OBJ_ARTI) {
				SetHudClipRect(192, 224, 256, 64, 256, 1);
				HudMessage(s:"* ", s:ArtifactExplanation[curposy]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 232.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(objectflag & OBJ_TALENT) {
				SetHudClipRect(192, 224, 256, 64, 256, 1);
				HudMessage(s:"* ", s:"Increases damage of ", s:TalentNames[onposy][TALENT_NAME], s:" damage weapons by \cf", f:GetMenuTalentBonus(onposy), s:"%\c-."; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 232.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(objectflag & OBJ_ARMOR) {
				SetHudClipRect(192, 224, 256, 64, 256, 1);
				HudMessage(s:"* ", s:ArmorExplanation[itemid - SHOP_FIRSTARMOR_INDEX]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 232.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(objectflag & OBJ_ABILITY) {
				SetHudClipRect(192, 208, 256, 64, 256, 1);
				if(objectflag & OBJ_USESCROLL)
					HudMessage(s:"* ", s:AbilityHelpText[itemid - SHOP_ABILITY1_BEGIN]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1 * 1.0 + ScrollPos, 0.0, 0.0);
				else
					HudMessage(s:"* ", s:AbilityHelpText[itemid - SHOP_ABILITY1_BEGIN]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(objectflag & OBJ_ACCOUNT) {
				SetHudClipRect(192, 208, 256, 64, 256, 1);
				if(objectflag & OBJ_USESCROLL)
					HudMessage(s:"* ", s:AccountPurchaseExplanation[itemid - SHOP_ACCOUNT_BEGIN]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1 * 1.0 + ScrollPos, 0.0, 0.0);
				else
					HudMessage(s:"* ", s:AccountPurchaseExplanation[itemid - SHOP_ACCOUNT_BEGIN]; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			HudMessage(s:weptype, s:"\c[B1]", s:itemtag; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
			HudMessage(s:colorprefix, s:"--> $", d:price; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, color, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
		else {
			HudMessage(s:weptype, s:toshow, s:itemtag; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
			HudMessage(s:colorprefix, s:"$", d:price; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, color, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
	}
}

void DrawAccessory(int id, int boxid, int page, menu_pane_T& CurrentPane) {
	int pos = id - ACCESSORY_PER_PAGE * (page - MENU_FIRST_ACCESSORY_PAGE);
	if(CheckInventory(AccessoryInfo[id][ACCESSORY_NAME])) {
		if(boxid == pos + 1) {
			HudMessage(s:"\c[B1]", s:AccessoryInfo[id][ACCESSORY_TAG]; HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_GREEN, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
			
			SetHudClipRect(192, 32, 256, 256, 256, 1);
			HudMessage(s:StrParam(s:"+ ", s:AccessoryInfo[id][ACCESSORY_PRO], s:"\n\c[D4]- ", s:AccessoryInfo[id][ACCESSORY_CON]); HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos - 1, CR_GREEN, 192.1, 212.1, 0.0, 0.0);
			SetHudClipRect(0, 0, 0, 0, 0, 0);
		}
		else if(IsAccessoryEquipped(0, 1 << id))
			HudMessage(s:"\c[C5]", s:AccessoryInfo[id][ACCESSORY_TAG]; HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_GREEN, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
		else
			HudMessage(s:"\c[Y5]", s:AccessoryInfo[id][ACCESSORY_TAG]; HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_WHITE, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
		ShowAccessoryIcon(id, pos);
	}
	else {
		if(boxid == pos + 1)
			HudMessage(s:"\c[B1]--- N / A ---"; HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_WHITE, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
		else
			HudMessage(s:"--- N / A ---"; HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_WHITE, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
	}
}

void ResetWeaponStats(int wepid) {
	int pnum = PlayerNumber();
	Player_Weapon_Infos[pnum][wepid].enchants = 0;
	Player_Weapon_Infos[pnum][wepid].wep_bonuses[WEP_BONUS_CRIT].amt = 0;
	Player_Weapon_Infos[pnum][wepid].wep_bonuses[WEP_BONUS_CRITDMG].amt = 0;
	Player_Weapon_Infos[pnum][wepid].wep_bonuses[WEP_BONUS_CRITPERCENT].amt = 0;
	Player_Weapon_Infos[pnum][wepid].wep_bonuses[WEP_BONUS_DMG].amt = 0;
	SyncClientsideVariable_Orb(DND_SYNC_WEAPONENHANCE, wepid);
	SyncClientsideVariable_Orb(DND_SYNC_WEPBONUS_CRIT, wepid);
	SyncClientsideVariable_Orb(DND_SYNC_WEPBONUS_CRITDMG, wepid);
	SyncClientsideVariable_Orb(DND_SYNC_WEPBONUS_CRITPERCENT, wepid);
	SyncClientsideVariable_Orb(DND_SYNC_WEPBONUS_DMG, wepid);
}

// will process item selections depending on given valid range
// support for selling other stuff is here, it's just a few extra lines in the serverside script to handle the process
void ProcessTrade (int pnum, int posy, int low, int high, int tradeflag, bool givefull) {
	int itemid, price, buystatus;
	int loopnumber = 0;
	str totake;
	if(tradeflag & TRADE_BUY) {
		itemid = low + posy;
		if(itemid <= high) {
			// consider the research if this item has any
			if(!CheckItemRequirements(itemid, RES_DONE, GetItemFlags(itemid))) {
				ShowNeedResearchPopup();
			}
			else {
				if(CheckInventory("StatbuffCounter_ForbidArmor") && (tradeflag & TRADE_ARMOR)) {
					ShowPopup(POPUP_CANTBUY, false, 0);
					return;
				}
				// loop 
				do {
					loopnumber++;
					// now consider money and other things as factors
					price = GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
					buystatus = CanTrade(itemid, tradeflag, price);
					if(!buystatus) {
						// consider researches before handing out
						TakeInventory("Credit", price);
						// for money quest
						GiveInventory("DnD_MoneySpentQuest", price);
						if(tradeflag & TRADE_ARMOR) { // armors are handled differently (+1 below is because armor_type considers armor bonus)
							HandleArmorPickup(itemid - SHOP_FIRSTARMOR_INDEX + 1, ArmorBaseAmounts[itemid - SHOP_FIRSTARMOR_INDEX + 1], tradeflag & TRADE_ARMOR_REPLACE);
						}
						else
							GiveInventory(ShopItemNames[itemid][SHOPNAME_ITEM], 1);
						if(tradeflag & TRADE_WEAPON) {
							totake = GetWeaponToTake(itemid);
							if(StrCmp(totake, ""))
								TakeInventory(totake, 1);	
							GiveInventory(ShopItemNames[itemid][SHOPNAME_CONDITION], 1);
							SetWeapon(ShopItemNames[itemid][SHOPNAME_ITEM]);
							// fix special ammo cursor
							int fix = IsSpecialFixWeapon(itemid);
							if(fix != -1) {
								int weptype = SpecialAmmoFixWeapons[fix][1];
								SetInventory(StrParam(s:"SpecialAmmoMode", s:GetSpecialAmmoSuffix(weptype)), SpecialAmmoRanges[SpecialAmmoFixWeapons[fix][2]][0]);
							}
						}
						if(tradeflag & TRADE_ARTIFACT)
							SetInventory("DnD_Artifact_MapBits", SetBit(CheckInventory("DnD_Artifact_MapBits"), itemid - SHOP_FIRSTARTI_INDEX));
						if(tradeflag & TRADE_TALENT)
							TakeInventory("TalentPoint", 1);
					}
				} while (givefull && !buystatus);
				//sound (mostly)
				if (buystatus && loopnumber == 1)
					ShowPopup(buystatus, false, 0);
				else {
					if(tradeflag & TRADE_WEAPON)
						LocalAmbientSound("weapons/pickup", 127);
					else if(tradeflag & TRADE_AMMO)
						LocalAmbientSound("items/ammo", 127);
					else if(tradeflag & (TRADE_ABILITY | TRADE_ARTIFACT | TRADE_TALENT | TRADE_ACCOUNT))
						LocalAmbientSound("Bonus/Received", 127);
					else if(tradeflag & TRADE_ARMOR)
						LocalAmbientSound("items/armor", 127);
				}
			}
		}
	}
	else if(tradeflag & TRADE_SELL) {
		itemid = low + posy;
		if(itemid <= high) {
			if(!CheckInventory("DnD_SellConfirm")) { // confirmation screen did not pop
				buystatus = CanTrade(itemid, TRADE_SELL, 0);
				if(buystatus) {
					LocalAmbientSound("RPG/MenuSellConfirm", 127);
					ShowPopup(posy + 1, true, itemid);
					GiveInventory("DnD_SellConfirm", 1);
				}
				else
					ShowPopup(POPUP_DONTOWN, false, 0);
			}
			else {
				// activepopupbox holds the boxid
				itemid = low + CheckInventory("ActivePopupBox") - 1;
				price = GetShopPrice(itemid, 0) / 2;
				TakeInventory("DnD_SellConfirm", 1);
				TakeInventory("DnD_PopupSell", 1);
				TakeInventory("DnD_ShowSellPopup", 1);
				SetInventory("DnD_PopupID", 0);
				SetInventory("ActivePopupBox", 0);
				totake = GetWeaponToTake(itemid);
				if(StrCmp(totake, ""))
					GiveInventory(totake, 1);
				TakeInventory(ShopItemNames[itemid][SHOPNAME_CONDITION], 1);
				TakeInventory(ShopItemNames[itemid][SHOPNAME_ITEM], 1);
				// reset buffs of weapon
				ResetWeaponStats(itemid);
				GiveInventory("Credit", price);
				ACS_NamedExecuteAlways("DnD Menu Sell Popup Clear", 0);
			}
		}
	}
}

void DrawHighLightBar (int posy, int drawstate) {
	if(posy == MAINBOX_NONE)
		HudMessage(s:""; HUDMSG_PLAIN, RPGMENUHIGHLIGHTID, -1, 47.1, 99.1, 0.0, 0.0);
	else if(posy < FIRST_CLICKABLE_BOXID) {
		--posy;
		int yadd = 0;
		if(posy > 0)
			yadd = 11 * posy;
		if(posy > 3)
			yadd += 8;
		SetHudSize(384, 200, 1);
		// trick to make this light up when both stats and perks are up
		if(drawstate & 1 || drawstate & 4)
			SetFont("BARHIGHL");
		else
			SetFont("BARHIGH");
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUHIGHLIGHTID, -1, 47.1, 99.1 + 1.0 * yadd, 0.0, 0.0);
	}
	else
		HudMessage(s:""; HUDMSG_PLAIN, RPGMENUHIGHLIGHTID, -1, 47.1, 99.1, 0.0, 0.0);
	// restore default
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("SMALLFONT");
}

void HandleAmmoPurchase(int slot, int boxid, int index_beg, bool givefull, bool isSpecialAmmo) {
	int itemid;
	// special ammo order is normal
	if(!isSpecialAmmo)
		itemid = MenuAmmoIndexMap[slot][boxid - 1];
	else
		itemid = index_beg + boxid - 1;
	
	if(!CheckItemRequirements(itemid, RES_DONE, GetItemFlags(itemid))) {
		// not done, so we can't give this
		ShowNeedResearchPopup();
	}
	else {
		int price = GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
		int buystatus = CanTrade(itemid, TRADE_BUY, price);
		
		if(!buystatus) {
			int amt = AmmoCounts[index_beg - SHOP_FIRSTAMMO_INDEX], count = 1;
			amt += ACS_ExecuteWithResult(918, 0, 1, amt);
			
			// if we are maxing the ammo
			if(givefull) {
				if(slot != -1)
					count += (GetAmmoCapacity(AmmoInfo_Str[slot][boxid - 1][AMMOINFO_NAME]) - CheckInventory(AmmoInfo_Str[slot][boxid - 1][AMMOINFO_NAME])) / amt;
				else
					count += (GetAmmoCapacity(SpecialAmmoInfo_Str[boxid - 1][AMMOINFO_NAME]) - CheckInventory(SpecialAmmoInfo_Str[boxid - 1][AMMOINFO_NAME])) / amt;
				price = price * count;
				if(price > CheckInventory("Credit")) {
					count = CheckInventory("Credit") / GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
					price = count * GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
				}
			}
			// check how much of this we can really afford
			TakeInventory("Credit", price);
			LocalAmbientSound("items/ammo", 127);
			if(slot != -1)
				GiveInventory(AmmoInfo_Str[slot][boxid - 1][AMMOINFO_NAME], amt * count);
			else
				GiveInventory(SpecialAmmoInfo_Str[boxid - 1][AMMOINFO_NAME], amt * count);
			
			GiveInventory("DnD_MoneySpentQuest", price);
		}
		else
			ShowPopup(buystatus, false, 0);
	}
}

int GetCursorPos(int input, int mt) {
	int res = 0, speed, ds;
	switch(mt) {
		case MOUSE_INPUT_X:
			res = CheckInventory("Mouse_X");
			speed = FixedDiv(1.0, FixedMul(GetCVar("m_yaw"), GetCVar("mouse_sensitivity")));
			speed = FixedMul(speed * 2, HUDMAX_XF) / (HUDMAX_X * 100);
			ds = input * speed;
			res = Clamp_Between(res + ds, 0, HUDMAX_XF);
		break;
		case MOUSE_INPUT_Y:
			res = CheckInventory("Mouse_Y");
			speed = FixedDiv(1.0, FixedMul(GetCVar("m_pitch"), GetCVar("mouse_sensitivity")));
			speed = FixedMul(speed * 2, HUDMAX_YF) / (HUDMAX_X / 2 * 100);
			if (GetCVar("invertmouse"))
				speed *= -1;
			ds = input * speed;
			res = Clamp_Between(res + ds, 0, HUDMAX_YF);
		break;
	}
	return res;
}

void DrawCursor() {
	static int cursor_anim = 0;
	if(cursor_anim < 8)
		SetFont("DND_CUR5");
	else
		SetFont(StrParam(s:"DND_CUR", d:cursor_anim / 4 - 1));
	cursor_anim = (cursor_anim + 1) % 24;
	//Log(f:CheckInventory("Mouse_X"), s: " ", f:CheckInventory("Mouse_Y"));
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCURSORID, -1, HUDMAX_XF - ((CheckInventory("Mouse_X") & MMASK)) + 0.1, HUDMAX_YF - ((CheckInventory("Mouse_Y") & MMASK)) + 0.1, 0.2, 0.0);
}

// since we use top left corner as 1:1, directions are changed
bool point_in_box(rect_T? box, int mx, int my, int yoffset) {
	return (mx <= box.topleft_x && mx >= box.botright_x && my <= box.topleft_y - yoffset && my >= box.botright_y - yoffset);
}

bool point_in_inventory_box(rect_T? box, int mx, int my) {
	return (mx <= box.topleft_x && mx >= box.botright_x && my <= box.topleft_y && my >= box.botright_y);
}

menu_pane_T& GetPane() {
	static menu_pane_T pane;
	return pane;
}

menu_inventory_T& GetInventoryPane() {
	static menu_inventory_T pane;
	return pane;
}

menu_trade_T& GetTradePane() {
	static menu_trade_T pane;
	return pane;
}

void DisableBoxesInPane(menu_inventory_T& p, int beg, int end) {
	for(int i = beg; i <= end; ++i)
		p.MenuRectangles[i].topleft_x = -1;
}

void DisableBoxInPane(menu_inventory_T& p, int i) {
	p.MenuRectangles[i].topleft_x = -1;
}

void EnableBoxWithPoints(menu_inventory_T& p, int box, int tx, int ty, int bx, int by) {
	p.MenuRectangles[box].topleft_x = tx;
	p.MenuRectangles[box].topleft_y = ty;
	p.MenuRectangles[box].botright_x = bx;
	p.MenuRectangles[box].botright_y = by;
}

// deepcopy to avoid accidental overriding
void AddBoxToPane(menu_pane_T& p, rect_T& box) {
	if(p.cursize < MAX_MENU_BOXES) {
		p.MenuRectangles[p.cursize].topleft_x = box.topleft_x;
		p.MenuRectangles[p.cursize].topleft_y = box.topleft_y;
		p.MenuRectangles[p.cursize].botright_x = box.botright_x;
		p.MenuRectangles[p.cursize].botright_y = box.botright_y;
		p.cursize++;
	}
	else
		Log(s:"Menu box limit exceeded.");
}

void AddBoxToTrade(menu_trade_T? p, rect_T& box) {
	if(p.cursize < 3 * MAX_INVENTORY_BOXES + 2) {
		p.MenuRectangles[p.cursize].topleft_x = box.topleft_x;
		p.MenuRectangles[p.cursize].topleft_y = box.topleft_y;
		p.MenuRectangles[p.cursize].botright_x = box.botright_x;
		p.MenuRectangles[p.cursize].botright_y = box.botright_y;
		p.cursize++;
	}
	else
		Log(s:"Menu box limit exceeded.");
}

void AddBoxToInventory(menu_inventory_T? p, rect_T& box) {
	if(p.cursize < MAX_INVENTORY_BOXES) {
		p.MenuRectangles[p.cursize].topleft_x = box.topleft_x;
		p.MenuRectangles[p.cursize].topleft_y = box.topleft_y;
		p.MenuRectangles[p.cursize].botright_x = box.botright_x;
		p.MenuRectangles[p.cursize].botright_y = box.botright_y;
		p.cursize++;
	}
	else
		Log(s:"Menu box limit exceeded.");
}

void AddBoxToPane_Points(menu_pane_T& p, int tx, int ty, int bx, int by) {
	if(p.cursize < MAX_MENU_BOXES) {
		p.MenuRectangles[p.cursize].topleft_x = tx;
		p.MenuRectangles[p.cursize].topleft_y = ty;
		p.MenuRectangles[p.cursize].botright_x = bx;
		p.MenuRectangles[p.cursize].botright_y = by;
		p.cursize++;
	}
	else
		Log(s:"Menu box limit exceeded.");
}

void ResetPane(menu_pane_T& p) {
	p.cursize = 0;
}

void ResetInventoryPane(menu_inventory_T& p) {
	p.cursize = 0;
}

void SetPaneSize(menu_pane_T& p, int s) {
	p.cursize = s;
}

rect_T& GetMainBox(int id) {
	static rect_T MainBoxList[MAX_MAIN_BOXES] = { 
		{ 419.0, 158.0, 348.0, 147.0 },
		{ 419.0, 141.0, 348.0, 130.0 },
		{ 419.0, 124.0, 348.0, 113.0 },
		{ 419.0, 107.0, 348.0, 96.0  },
		
		{ 419.0, 75.0,  348.0, 64.0  },
		{ 419.0, 58.0,  348.0, 47.0  },
		{ 419.0, 41.0,  348.0, 30.0  },
		
		// buttons
		{ 228.0, 35.0, 187.0, 16.0 },
		{ 187.0, 35.0, 146.0, 16.0 },
		{ 146.0, 35.0, 105.0, 16.0 }
	};
	return MainBoxList[id];
}

rect_T& LoadRect(int menu_page, int id) {
	static rect_T bp[MAX_MENU_BOXPAGES][MAX_MENU_BOXES] = {
		// null
		{
			{ -1, -1, -1, -1 }
		},
		// stat 1
		{
			{ 288.0, 196.0, 108.0, 188.0 }, // str
			{ 288.0, 180.0, 108.0, 172.0 }, // dex
			{ 288.0, 164.0, 108.0, 156.0 }, // bul
			{ 288.0, 148.0, 108.0, 140.0 }, // chr
			{ 288.0, 132.0, 108.0, 124.0 }, // vit
			{ 288.0, 116.0, 108.0, 108.0 }, // int
			{ -1, -1, -1, -1 }
		},
		// stat 2
		{
			{ -1, -1, -1, -1 }
		},
		// stat 3
		{
			{ -1, -1, -1, -1 }
		},
		// perk
		{
			{ 296.0, 245.0, 184.0, 237.0 }, // sharp
			{ 296.0, 229.0, 214.0, 221.0 }, // end
			{ 296.0, 213.0, 243.0, 205.0 }, // wis
			{ 296.0, 197.0, 247.0, 189.0 }, // greed
			{ 296.0, 181.0, 252.0, 173.0 }, // med
			{ 296.0, 165.0, 211.0, 157.0 }, // mun
			{ 296.0, 149.0, 209.0, 141.0 }, // ded
			{ 296.0, 133.0, 209.0, 125.0 }, // sav
			{ 296.0, 117.0, 209.0, 109.0 }, // luck
			{ -1, -1, -1, -1 }
		},
		// loadout
		{
			{ 289.0, 229.0, 223.0, 222.0 }, // loadout 1
			{ 289.0, 213.0, 248.0, 206.0 }, // loadout 2
			{ 289.0, 197.0, 227.0, 190.0 }, // loadout 3
			{ 289.0, 181.0, 214.0, 174.0 }, // loadout 4
			{ 289.0, 165.0, 245.0, 158.0 }, // loadout 5
			{ 289.0, 149.0, 234.0, 142.0 }, // loadout 6
			{ 289.0, 133.0, 222.0, 126.0 }, // loadout 7
			{ 289.0, 117.0, 197.0, 110.0 }, // loadout 8
			{ 289.0, 101.0, 229.0, 94.0 }, // loadout 9
			{ -1, -1, -1, -1 }
		},
		// loadout 1
		{
			{ -1, -1, -1, -1 }
		},
		// loadout 2
		{
			{ -1, -1, -1, -1 }
		},
		// loadout 3
		{
			{ -1, -1, -1, -1 }
		},
		// loadout inventory
		{
			{ -1, -1, -1, -1 }
		},
		// loadout stash
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for charms
		{
			{ 290.0, 125.0, 250.0, 87.0 },
			{ 226.0, 125.0, 186.0, 87.0 },
			{ 162.0, 125.0, 122.0, 87.0 },
			{ 98.0, 125.0, 58.0, 87.0 },
			{ 258.0, 197.0, 218.0, 143.0 },
			{ 129.0, 197.0, 89.0, 143.0 },
			{ 194.0, 268.0, 153.0, 184.0 },
			{ 220.0, 76.0, 106.0, 68.0 },
			{ -1, -1, -1, -1 }
		},
		// loadout for crafting
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for crafting - 1
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for crafting - 2
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for accessories below
		{
			{ 226.0, 252.0, 102.0, 242.0 },
			{ 224.0, 220.0, 107.0, 210.0 },
			{ 210.0, 188.0, 119.0, 178.0 },
			{ 240.0, 156.0, 87.0, 146.0 },
			{ 216.0, 124.0, 113.0, 114.0 },
			{ -1, -1, -1, -1 },
		},
		{
			{ 240.0, 252.0, 90.0, 242.0 },
			{ 213.0, 220.0, 116.0, 210.0 },
			{ 208.0, 188.0, 120.0, 178.0 },
			{ 207.0, 156.0, 120.0, 146.0 },
			{ 226.0, 124.0, 102.0, 114.0 },
			{ -1, -1, -1, -1 }
		},
		{
			{ 242.0, 252.0, 86.0, 242.0 },
			{ 220.0, 220.0, 108.0, 210.0 },
			{ 197.0, 188.0, 131.0, 178.0 },
			{ 232.0, 156.0, 97.0, 146.0 },
			{ 226.0, 124.0, 106.0, 114.0 },
			{ -1, -1, -1, -1 }
		},
		{
			{ 244.0, 252.0, 86.0, 242.0 },
			{ 236.0, 220.0, 96.0, 210.0 },
			{ -1, -1, -1, -1 }
		},
		// trade 1
		{
			{ -1, -1, -1, -1 }
		},
		// trade 2
		{
			{ -1, -1, -1, -1 }
		},
		// trade 3
		{
			{ -1, -1, -1, -1 }
		},
		// trade 4
		{
			{ -1, -1, -1, -1 }
		},
		// trade 5
		{
			{ -1, -1, -1, -1 }
		},
		// shop
		{
			{ 289.0, 229.0, 179.0, 221.0 }, // wep
			{ 289.0, 213.0, 162.0, 205.0 }, // ammo
			{ 289.0, 197.0, 178.0, 189.0 }, // ability
			{ 289.0, 181.0, 169.0, 173.0 }, // arti
			{ 289.0, 165.0, 183.0, 157.0 }, // talent
			{ 289.0, 149.0, 188.0, 141.0 }, // armor
			{ 289.0, 133.0, 183.0, 125.0 }, // account
			{ -1, -1, -1, -1 }
		},
		// wep
		{
			{ 289.0, 229.0, 240.0, 220.0 }, // s1
			{ 289.0, 213.0, 240.0, 204.0 }, // s2
			{ 289.0, 197.0, 240.0, 189.0 }, // s3
			{ 289.0, 181.0, 240.0, 172.0 }, // s4
			{ 289.0, 165.0, 240.0, 156.0 }, // s5
			{ 289.0, 149.0, 240.0, 140.0 }, // s6
			{ 289.0, 133.0, 240.0, 124.0 }, // s7
			{ 289.0, 117.0, 240.0, 108.0 }, // s8
			{ -1, -1, -1, -1 }
		},
		// wep 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ -1, -1, -1, -1 }
		},
		// wep 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ -1, -1, -1, -1 }
		},
		// wep 3 - 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ -1, -1, -1, -1 }
		},
		// wep 3 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ -1, -1, -1, -1 }
		},
		// wep 4 - 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ -1, -1, -1, -1 }
		},
		// wep 4 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ -1, -1, -1, -1 }
		},
		// wep 5 - 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ -1, -1, -1, -1 }
		},
		// wep 5 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ -1, -1, -1, -1 }
		},
		// wep 6 - 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ -1, -1, -1, -1 }
			
		},
		// wep 6 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ -1, -1, -1, -1 }
		},
		// wep 7
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ -1, -1, -1, -1 }
		},
		// wep 8
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ -1, -1, -1, -1 }
		},
		// ammo select
		{
			{ 289.0, 229.0, 179.0, 222.0 }, // 1
			{ 289.0, 211.0, 162.0, 206.0 }, // 2
			{ 289.0, 195.0, 178.0, 190.0 }, // 3
			{ 289.0, 181.0, 169.0, 174.0 }, // 4
			{ 289.0, 167.0, 169.0, 160.0 }, // 5 - special
			{ -1, -1, -1, -1 }
		},
		// ammo 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 289.0, 117.0, 120.0, 111.0 }, // w9
			{ 289.0, 101.0, 104.0, 95.0 }, // w10
			{ -1, -1, -1, -1 }
		},
		// ammo 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 289.0, 117.0, 120.0, 111.0 }, // w9
			{ -1, -1, -1, -1 }
		},
		// ammo 3
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ -1, -1, -1, -1 }
		},
		// ammo 4
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 289.0, 117.0, 120.0, 111.0 }, // w9
			{ 289.0, 101.0, 104.0, 95.0 }, // w10
			{ -1, -1, -1, -1 }
		},
		// ammo special
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ -1, -1, -1, -1 }
		},
		// ability shop - 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ -1, -1, -1, -1 }
		},
		// ability shop - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ -1, -1, -1, -1 }
		},
		// artifact shop
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 289.0, 117.0, 120.0, 111.0 }, // w9
			{ 289.0, 101.0, 104.0, 95.0 }, // w10
			{ -1, -1, -1, -1 }
		},
		// talent
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ -1, -1, -1, -1 }
		},
		// armor 1
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 289.0, 117.0, 120.0, 111.0 }, // w9
			{ -1, -1, -1, -1 }
		},
		// armor 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ -1, -1, -1, -1 }
		},
		// account
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ -1, -1, -1, -1 }
		},
		// research - main
		{
			{ 289.0, 229.0, 96.0, 221.0 }, // 1
			{ 289.0, 211.0, 96.0, 203.0 }, // 2
			{ 289.0, 195.0, 96.0, 187.0 }, // 3
			{ 289.0, 181.0, 80.0, 173.0 }, // 4
			{ -1, -1, -1, -1 }
		},
		// research - guns
		{
			{ 289.0, 229.0, 96.0, 221.0 }, // 1
			{ 289.0, 211.0, 48.0, 203.0 }, // 2
			{ -1, -1, -1, -1 }
		},
		// research - body
		{
			{ 202.0, 84.0, 128.0, 76.0 }, // res
			{ -1, -1, -1, -1 }
		},
		// research - ammo
		{
			{ 202.0, 84.0, 128.0, 76.0 }, // res
			{ -1, -1, -1, -1 }
		},
		// research - slotguns
		{
			{ 202.0, 84.0, 128.0, 76.0 }, // res
			{ -1, -1, -1, -1 }
		},
		// research - luxury guns
		{
			{ 202.0, 84.0, 128.0, 76.0 }, // res
			{ -1, -1, -1, -1 }
		},
		// research - utility
		{
			{ 202.0, 84.0, 128.0, 76.0 }, // res
			{ -1, -1, -1, -1 }
		},
		// main
		{
			{ -1, -1, -1, -1 }
		},
		// help
		{
			{ 289.0, 229.0, 179.0, 222.0 }, // char
			{ 289.0, 213.0, 162.0, 206.0 }, // weapon prop
			{ 289.0, 197.0, 178.0, 190.0 }, // res
			{ 289.0, 181.0, 178.0, 174.0 }, // dmg
			{ 289.0, 165.0, 178.0, 158.0 }, // orb
			{ 289.0, 149.0, 179.0, 142.0 }, // leg
			{ 289.0, 133.0, 179.0, 126.0 }, // mods
			{ 296.0, 81.0, 182.0, 73.0 }, // show info
			{ -1, -1, -1, -1 }
		},
		// help char
		{
			{ -1, -1, -1, -1 }
		},
		// help weapon properties
		{
			{ -1, -1, -1, -1 }
		},
		// help res
		{
			{ -1, -1, -1, -1 }
		},
		// help damage
		{
			{ -1, -1, -1, -1 }
		},
		// help orbs
		{
			{ -1, -1, -1, -1 }
		},
		// help leg
		{
			{ -1, -1, -1, -1 }
		},
		// help monster mods
		{
			{ -1, -1, -1, -1 }
		},
		// ability
		{
			{ 289.0, 69.0, 104.0, 61.0 }, // dash
			{ -1, -1, -1, -1 }
		}
	};
	return bp[menu_page][id];
}

// specialized one for inventory view
rect_T& LoadInventoryViewRect(int id) {
	// this is the top left box, ie 0, 0
	static rect_T bp[MAX_INVENTORY_BOXES];
	
	// construct for first time, these can be constructed easily from base coordinates
	if(!IsSet(PaneSetup, INVENTORY_SETUP_BIT)) {
		for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
			for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_x = INVENTORYBOX_BASEX_RECT - 32.0 * j;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_y = INVENTORYBOX_BASEY_RECT - 32.0 * i;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_x = INVENTORYBOX_BASEX_RECT - 32.0 * j - 32.0;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_y = INVENTORYBOX_BASEY_RECT - 32.0 * i - 32.0;
			}
		PaneSetup = SetBit(PaneSetup, INVENTORY_SETUP_BIT);
	}
	return bp[id];
}

rect_T& LoadStashViewRect(int id) {
	// this is the top left box, ie 0, 0
	// left and right boxes
	static rect_T bp[2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES];
	int i, j;
	// construct for first time, these can be constructed easily from base coordinates
	if(!IsSet(PaneSetup, STASH_SETUP_BIT)) {
		// stash
		for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
			for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_x = STASHBOX_TOP_BASEX_RECT - TRADEBOX_SKIP * j;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_y = INVENTORYBOXTRADE_TOP_BASEY_RECT - TRADEBOX_SKIP * i;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_x = STASHBOX_TOP_BASEX_RECT - TRADEBOX_SKIP * j - TRADEBOX_SKIP;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_y = INVENTORYBOXTRADE_TOP_BASEY_RECT - TRADEBOX_SKIP * i - TRADEBOX_SKIP;
			}
		// inventory
		for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
			for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].topleft_x = STASHBOX_TOP_BASEX_RECT - TRADEBOX_SKIP * j;
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].topleft_y = INVENTORYBOXTRADE_BOT_BASEY_RECT - TRADEBOX_SKIP * i;
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].botright_x = STASHBOX_TOP_BASEX_RECT - TRADEBOX_SKIP * j - TRADEBOX_SKIP;
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].botright_y = INVENTORYBOXTRADE_BOT_BASEY_RECT - TRADEBOX_SKIP * i - TRADEBOX_SKIP;
			}
		for(i = 0; i < MAX_EXTRA_INVENTORY_PAGES; ++i) {
			bp[2 * MAX_INVENTORY_BOXES + i].topleft_x = STASHTAB_BUTTON_TOPLEFT_X - i * STASHTAB_BUTTON_INCREMENT_X;
			bp[2 * MAX_INVENTORY_BOXES + i].topleft_y = STASHTAB_BUTTON_TOPLEFT_Y;
			bp[2 * MAX_INVENTORY_BOXES + i].botright_x = STASHTAB_BUTTON_BOTRIGHT_X - i * STASHTAB_BUTTON_INCREMENT_X;
			bp[2 * MAX_INVENTORY_BOXES + i].botright_y = STASHTAB_BUTTON_BOTRIGHT_Y;
		}
		PaneSetup = SetBit(PaneSetup, STASH_SETUP_BIT);
	}
	return bp[id];
}

rect_T& LoadTradeViewRect(int id) {
	// this is the top left box, ie 0, 0
	// + 2 for confirm and cancel buttons
	static rect_T bp[3 * MAX_INVENTORY_BOXES + 2];
	int i, j;
	// construct for first time, these can be constructed easily from base coordinates
	if(!IsSet(PaneSetup, TRADEVIEW_SETUP_BIT)) {
		// other player offering
		for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
			for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_x = INVENTORYBOXTRADE_TOP_BASEX_RECT - TRADEBOX_SKIP * j;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_y = INVENTORYBOXTRADE_TOP_BASEY_RECT - TRADEBOX_SKIP * i;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_x = INVENTORYBOXTRADE_TOP_BASEX_RECT - TRADEBOX_SKIP * j - TRADEBOX_SKIP;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_y = INVENTORYBOXTRADE_TOP_BASEY_RECT - TRADEBOX_SKIP * i - TRADEBOX_SKIP;
			}
		// our offering
		for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
			for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].topleft_x = INVENTORYBOXTRADE_BOT_BASEX_RECT - TRADEBOX_SKIP * j;
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].topleft_y = INVENTORYBOXTRADE_BOT_BASEY_RECT - TRADEBOX_SKIP * i;
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].botright_x = INVENTORYBOXTRADE_BOT_BASEX_RECT - TRADEBOX_SKIP * j - TRADEBOX_SKIP;
				bp[MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].botright_y = INVENTORYBOXTRADE_BOT_BASEY_RECT - TRADEBOX_SKIP * i - TRADEBOX_SKIP;
			}
		// our inventory
		for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
			for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
				bp[2 * MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].topleft_x = INVENTORYBOXTRADE_MID_BASEX_RECT - TRADEBOX_SKIP * j;
				bp[2 * MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].topleft_y = INVENTORYBOXTRADE_MID_BASEY_RECT - TRADEBOX_SKIP * i;
				bp[2 * MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].botright_x = INVENTORYBOXTRADE_MID_BASEX_RECT - TRADEBOX_SKIP * j - TRADEBOX_SKIP;
				bp[2 * MAX_INVENTORY_BOXES + i * MAXINVENTORYBLOCKS_VERT + j].botright_y = INVENTORYBOXTRADE_MID_BASEY_RECT - TRADEBOX_SKIP * i - TRADEBOX_SKIP;
			}
		// add confirm and cancel buttons
		// confirm
		bp[TRADECONFIRM_BOXID].topleft_x = INVENTORYBOXTRADE_TRADECONFIRM_BASEX_RECT;
		bp[TRADECONFIRM_BOXID].topleft_y = INVENTORYBOXTRADE_TRADECONFIRM_BASEY_RECT;
		bp[TRADECONFIRM_BOXID].botright_x = INVENTORYBOXTRADE_TRADECONFIRM_BASEX_RECT - INVENTORYBOXTRADE_TRADEBOXSKIP_X;
		bp[TRADECONFIRM_BOXID].botright_y = INVENTORYBOXTRADE_TRADECONFIRM_BASEY_RECT - INVENTORYBOXTRADE_TRADEBOXSKIP_Y;
		// cancel
		bp[TRADECANCEL_BOXID].topleft_x = INVENTORYBOXTRADE_TRADECANCEL_BASEX_RECT;
		bp[TRADECANCEL_BOXID].topleft_y = INVENTORYBOXTRADE_TRADECANCEL_BASEY_RECT;
		bp[TRADECANCEL_BOXID].botright_x = INVENTORYBOXTRADE_TRADECANCEL_BASEX_RECT - INVENTORYBOXTRADE_TRADEBOXSKIP_X;
		bp[TRADECANCEL_BOXID].botright_y = INVENTORYBOXTRADE_TRADECANCEL_BASEY_RECT - INVENTORYBOXTRADE_TRADEBOXSKIP_Y;
		PaneSetup = SetBit(PaneSetup, TRADEVIEW_SETUP_BIT);
	}
	return bp[id];
}

rect_T& LoadCraftingViewRect(int id) {
	// this is the top left box, ie 0, 0
	static rect_T bp[MAX_CRAFTING_BOXES];
	
	// construct for first time, these can be constructed easily from base coordinates
	if(!IsSet(PaneSetup, CRAFTING_SETUP_BIT)) {
		// for all the boxes, load them with -1 to denote "disabled" boxes
		// we'll manually load points for eligible boxes in respective crafting regions
		for(int i = 0; i < MAX_CRAFTING_BOXES; ++i) {
			bp[i].topleft_x = -1;
			bp[i].topleft_y = -1;
			bp[i].botright_x = -1;
			bp[i].botright_y = -1;
		}
		// weapon crafting box
		bp[CRAFTING_WEAPON_BOXID].topleft_x = 455.0;
		bp[CRAFTING_WEAPON_BOXID].topleft_y = 292.0;
		bp[CRAFTING_WEAPON_BOXID].botright_x = 321.0;
		bp[CRAFTING_WEAPON_BOXID].botright_y = 285.0;
		// inventory crafting box
		bp[CRAFTING_INVENTORY_BOXID].topleft_x = 455.0;
		bp[CRAFTING_INVENTORY_BOXID].topleft_y = 276.0;
		bp[CRAFTING_INVENTORY_BOXID].botright_x = 310.0;
		bp[CRAFTING_INVENTORY_BOXID].botright_y = 269.0;
		PaneSetup = SetBit(PaneSetup, CRAFTING_SETUP_BIT);
	}
	return bp[id];
}

void LoadTradeView(menu_trade_T& p) {
	p.cursize = 0;
	for(int i = 0; i < 3 * MAX_INVENTORY_BOXES + 2; ++i) {
		auto r = LoadTradeViewRect(i);
		if(r.topleft_x != -1) {
			// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
			AddBoxToTrade(p, r);
		}
		else
			break;
	}
}

void LoadInventoryView(menu_inventory_T& p) {
	p.cursize = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto r = LoadInventoryViewRect(i);
		if(r.topleft_x != -1) {
			// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
			AddBoxToInventory(p, r);
		}
		else
			break;
	}
}

void LoadStashView(menu_trade_T& p) {
	p.cursize = 0;
	for(int i = 0; i < 2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES; ++i) {
		auto r = LoadStashViewRect(i);
		if(r.topleft_x != -1) {
			// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
			AddBoxToTrade(p, r);
		}
		else
			break;
	}
}

void LoadCraftingView(menu_inventory_T& p) {
	p.cursize = 0;
	for(int i = 0; i < MAX_CRAFTING_BOXES; ++i) {
		auto r = LoadCraftingViewRect(i);
		// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
		AddBoxToInventory(p, r);
	}
}

void LoadPane(menu_pane_T& p, int menu_page) {
	p.cursize = 0;
	for(int i = 0; i < MAX_MENU_BOXES; ++i) {
		auto r = LoadRect(menu_page, i);
		if(r.topleft_x != -1) {
			// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
			AddBoxToPane(p, r);
		}
		else
			break;
	}
}

int GetPageScrollOffset(int page) {
	int base = 0;
	switch (page) {
		default:
			base = 0;
		break;
	}
	return ScrollPos * base;
}

int GetTriggeredBoxOnPane(menu_pane_T& p, int mx, int my, int curopt) {
	if(mx >= 348.0 || my >= 270.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_box(p.MenuRectangles[i], mx, my, GetPageScrollOffset(curopt)))
			return i + 1;
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnInventoryPane(menu_inventory_T& p, int mx, int my) {
	if(mx >= 400.0 || my <= 64.0 || mx <= 82.0 || my >= 260.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnCraftingPane(menu_inventory_T& p, int mx, int my) {
	if(mx >= 470.0 || my <= 24.0 || mx <= 4.0 || my >= 293.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my)) {
			return i + 1;
		}
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnTradePane(menu_trade_T& p, int mx, int my) {
	if(mx >= 465.0 || mx <= 30.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnStashPane(menu_trade_T& p, int mx, int my) {
	if(mx >= 304.0 || my <= 27.0 || mx <= 68.0 || my >= 293.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnMainPane(int mx, int my) {
	if(mx < 105.0 || my < 16.0)
		return MAINBOX_NONE;
	for(int i = 0; i < MAX_MAIN_BOXES; ++i) {
		if(point_in_box(GetMainBox(i), mx, my, 0))
			return i + 1;
	}
	return MAINBOX_NONE;
}

void DrawBoxText(str msg, int boxid, int thisboxid, int id, int x, int y, str hcolor, str lcolor) {
	if(boxid == thisboxid)
		HudMessage(s:hcolor, s:msg, s:"\c-"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
	else
		HudMessage(s:lcolor, s:msg, s:"\c-"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
}

void DrawClickableButton(str image, int boxid, int thisboxid, int id, int x, int y, str hoverimage, str clickimage) {
	if(boxid >= FIRST_CLICKABLE_BOXID && boxid <= LAST_CLICKABLE_BOXID) {
		if(CheckInventory(ButtonTimers[thisboxid - FIRST_CLICKABLE_BOXID])) {
			SetFont(clickimage);
			HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
		}
		else if(boxid == thisboxid) {
			SetFont(hoverimage);
			HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
		}
		else {
			SetFont(image);
			HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
		}
	}
	else {
		SetFont(image);
		HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
	}
}

void HandleClickableButtonFrames() {
	for(int i = 0; i < MAX_TIMED_BUTTONS; ++i)
		if(CheckInventory(ButtonTimers[i]))
			SetInventory(ButtonTimers[i], (CheckInventory(ButtonTimers[i]) + 1) % ButtonFrameCounts[i]);
}

menu_stack_T& GetMenuStack(int pnum) {
	static menu_stack_T stack[MAXPLAYERS];
	return stack[pnum];
}

void FlushStack(int pnum) {
	menu_stack_T? stack = GetMenuStack(pnum);
	for(int i = 0; i < MAX_STACK_ELEMS; ++i)
		stack.stack_elems[i] = MENU_MAIN;
	stack.cursize = 1;
	stack.stackptr = 0;
}

void PushStack(int pnum, int val) {
	menu_stack_T? stack = GetMenuStack(pnum);
	if(stack.cursize < MAX_STACK_ELEMS) {
		stack.stack_elems[stack.cursize] = val;
		stack.stackptr = stack.cursize++;
	}
	else {
		// shift elements to left
		for(int i = 0; i < MAX_STACK_ELEMS - 1; ++i)
			stack.stack_elems[i] = stack.stack_elems[i + 1];
		stack.stack_elems[MAX_STACK_ELEMS - 1] = val;
		stack.stackptr = MAX_STACK_ELEMS - 1;
	}
}

void HandleButtonClick(int boxid) {
	if(boxid >= FIRST_CLICKABLE_BOXID && boxid <= LAST_CLICKABLE_BOXID) {
		if(!CheckInventory(ButtonTimers[boxid - FIRST_CLICKABLE_BOXID]))
			SetInventory(ButtonTimers[boxid - FIRST_CLICKABLE_BOXID], 1); // kick starts the animation
		
		// all other boxes now have some other behavior
		// flush stack if return is pressed
		menu_stack_T? stack = GetMenuStack(PlayerNumber());
		if(boxid == MAINBOX_RET) {
			FlushStack(PlayerNumber());
			ReturnToMain();
		}
		else if(boxid == MAINBOX_LARR) {
			stack.stackptr = stack.stackptr > 0 ? stack.stackptr - 1 : 0;
			UpdateMenuFromStack(stack.stack_elems[stack.stackptr]);
		}
		else if(boxid == MAINBOX_RARR) {
			stack.stackptr = stack.stackptr < stack.cursize - 1 ? stack.stackptr + 1 : stack.cursize - 1;
			UpdateMenuFromStack(stack.stack_elems[stack.stackptr]);
		}
	}
}

// opt is menu page, multipage is for when one option can lead to multiple weapon pages
void HandleWeaponPageDraw(int opt, int multipage, int slotid, int boxid, int scrollamt) {
	int begin = GetWeaponBeginIndexFromOption(opt);
	int end = GetWeaponEndIndexFromOption(opt);
	
	if(multipage > 0)
		HudMessage(s:"\c[Y5]=>"; HUDMSG_PLAIN, RPGMENUPAGEID, CR_CYAN, 436.1, 44.0, 0.0, 0.0);
	if(multipage > 1 || multipage < 0)
		HudMessage(s:"\c[Y5]<="; HUDMSG_PLAIN, RPGMENUPAGEID - 1, CR_CYAN, 184.1, 44.0, 0.0, 0.0);
	
	if(boxid != -1 && WeaponDrawInfo[begin + boxid - 1].flags & OBJ_USESCROLL)
		ListenScroll(scrollamt, 0);
	
	// negative indicates last page
	if(multipage < 0)
		multipage = -multipage;

	if(multipage)
		HudMessage(s:"--- SLOT ", d:slotid, s:" (", d:multipage, s:") ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	else
		HudMessage(s:"--- SLOT ", d:slotid, s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	HudMessage(s:"\c[Y5]Credits: \c-$", d:CheckInventory("Credit"); HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 264.1, 64.0, 0.0, 0.0);
	
	for(int i = begin; i <= end; ++i)
		DrawToggledImage(i, boxid, i - begin, WeaponDrawInfo[i - SHOP_WEAPON_BEGIN].flags, CR_WHITE, CR_GREEN, ShopItemNames[i][SHOPNAME_CONDITION], 1, CR_RED);
}

void HandleAmmoPageDraw(int opt, int boxid, int slot, int multipage, bool specialammo) {
	int shopindex = 0;
	int i;
	
	if(multipage > 0)
		HudMessage(s:"\c[Y5]=>"; HUDMSG_PLAIN, RPGMENUPAGEID, CR_CYAN, 436.1, 44.0, 0.0, 0.0);
	if(multipage > 1 || multipage < 0)
		HudMessage(s:"\c[Y5]<="; HUDMSG_PLAIN, RPGMENUPAGEID - 1, CR_CYAN, 184.1, 44.0, 0.0, 0.0);
	
	// negative indicates last page
	if(multipage < 0)
		multipage = -multipage;
	
	if(!specialammo)
		HudMessage(s:"--- AMMO CATEGORY ", d:slot + 1, s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	else
		HudMessage(s:"--- SPECIAL AMMO ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	
	HudMessage(s:"\c[Y5]Credits: \c-$", d:CheckInventory("Credit"); HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 264.1, 64.0, 0.0, 0.0);

	if(!specialammo) {
		for(i = 0; i < MAX_AMMOTYPES_PER_SLOT && AmmoInfo[slot][i].initial_capacity != -1; ++i) {
			shopindex = MenuAmmoIndexMap[slot][i];
			DrawToggledImage(shopindex, boxid, i, AmmoDrawInfo[shopindex - SHOP_FIRSTAMMO_INDEX].flags, CR_WHITE, CR_GREEN, ShopItemNames[shopindex][SHOPNAME_CONDITION], 1, CR_RED);
		}
	}
	else {
		for(i = 0; i < MAX_SPECIAL_AMMOS; ++i) {
			shopindex = SHOP_FIRSTAMMOSPECIAL_INDEX + i;
			DrawToggledImage(shopindex, boxid, i, AmmoDrawInfo[shopindex - SHOP_FIRSTAMMO_INDEX].flags, CR_WHITE, CR_GREEN, ShopItemNames[i][SHOPNAME_CONDITION], 1, CR_RED);
		}
	}
}

void HandleWeaponPageInput(int pnum, int boxid, int wbegin, int wend, int pageprev, int pagenext) {
	if(HasLeftClicked(pnum)) {
		if(boxid != MAINBOX_NONE)
			ProcessTrade(pnum, boxid - 1, wbegin, wend, TRADE_BUY | TRADE_WEAPON, false);
		ClearPlayerInput(pnum, true);
	}
	else if(HasRightClicked(pnum) && boxid != MAINBOX_NONE) {
		ProcessTrade(pnum, boxid - 1, wbegin, wend, TRADE_SELL, false);
		ClearPlayerInput(pnum, true);
	}
	if(HasPressedLeft(pnum)) {
		ClearPlayerInput(pnum, true);
		if(pageprev == -1) // no page for left, use default
			UpdateMenuPosition(MENU_SHOP_WEAPON);
		else
			UpdateMenuPosition(pageprev);
	}
	else if(HasPressedRight(pnum) && pagenext != -1) {
		ClearPlayerInput(pnum, true);
		UpdateMenuPosition(pagenext);
	}
}

void HandleAmmoPageInput(int pnum, int slot, int boxid, int pageprev, int pagenext, bool IsSpecialAmmo) {
	int beginindex = MenuAmmoIndexMap[slot][boxid - 1];
	
	if(IsSpecialAmmo) {
		beginindex = SHOP_FIRSTAMMOSPECIAL_INDEX;
		slot = -1;
	}
	
	if(HasLeftClicked(pnum)) {
		if(boxid != MAINBOX_NONE)
			HandleAmmoPurchase(slot, boxid, beginindex, false, IsSpecialAmmo);
		ClearPlayerInput(pnum, true);
	} // ammos have alternate functionality for sell
	else if(HasRightClicked(pnum) && boxid != MAINBOX_NONE) {
		HandleAmmoPurchase(slot, boxid, beginindex, true, IsSpecialAmmo);
		ClearPlayerInput(pnum, true);
	}
	
	if(HasPressedLeft(pnum)) {
		ClearPlayerInput(pnum, true);
		if(pageprev == -1) // no page for left, use default
			UpdateMenuPosition(MENU_SHOP_AMMOSELECT);
		else
			UpdateMenuPosition(pageprev);
	}
	else if(HasPressedRight(pnum) && pagenext != -1) {
		ClearPlayerInput(pnum, true);
		UpdateMenuPosition(pagenext);
	}
}

void HandleResearchPageDraw(int page, int boxid) {
	int posx = CheckInventory("MenuPosX");
	int status = CheckResearchStatus(ResearchInfo[page][posx].res_id);
	int budget = CheckInventory("Budget");

	HudMessage(s:"--- RESEARCH PANEL ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);

	if(posx)
		HudMessage(s:"\c[Y5]<="; HUDMSG_PLAIN, RPGMENUPAGEID - 1, CR_CYAN, 184.1, 44.0, 0.0, 0.0);
	else
		DeleteText(RPGMENUPAGEID - 1);
	if(ResearchInfo[page][posx + 1].res_id != -1)
		HudMessage(s:"\c[Y5]=>"; HUDMSG_PLAIN, RPGMENUPAGEID, CR_CYAN, 436.1, 44.0, 0.0, 0.0);
	else
		DeleteText(RPGMENUPAGEID);

	if(budget)
		HudMessage(s:"\c[Y5]Budget: \c-", d:budget, s:"\cjK"; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 280.1, 64.0, 0.0, 0.0);
	else
		HudMessage(s:"\c[Y5]Budget: \c-0"; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 280.1, 64.0, 0.0, 0.0);

	HudMessage(s:"\c[Y5]Entry\c- #", d:ResearchInfo[page][posx].res_number; HUDMSG_PLAIN, RPGMENUITEMID - 11, CR_WHITE, 280.1, 80.0, 0.0, 0.0);
	if(status != RES_NA)
		HudMessage(s:"\c[Y5]Cost: \cj$\c-", d:ResearchInfo[page][posx].res_cost, s:"k"; HUDMSG_PLAIN, RPGMENUITEMID - 12, CR_WHITE, 280.1, 96.0, 0.0, 0.0);
	else
		HudMessage(s:"\c[Y5]Cost: ???\c-"; HUDMSG_PLAIN, RPGMENUITEMID - 12, CR_WHITE, 280.1, 96.0, 0.0, 0.0);

	if(status < RES_DONE)
		SetFont("RESBLAK");
	else
		SetFont("RESDONE");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 13, CR_WHITE, 192.1, 96.0, 0.0, 0.0);

	if(status == RES_NA)
		SetFont("RESNONE");
	else
		SetFont(ResearchStringInfo[page][posx].res_icon);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 14, CR_WHITE, 199.1, 96.0, 0.0, 0.0);

	SetFont("SMALLFONT");
	if(status != RES_NA) {
		SetHudClipRect(192, 144, 256, 96, 256, 1);
		HudMessage(s:ResearchStringInfo[page][posx].res_desc; HUDMSG_PLAIN, RPGMENUITEMID - 15, CR_WHITE, 192.1, 152.1, 0.0, 0.0);
		SetHudClipRect(0, 0, 0, 0, 0);
	}

	DrawBoxText("Research!", boxid, MBOX_1, RPGMENUITEMIDEND + 2, 316.0, 240.0, "\c[B1]", "\c[Y5]");
}

void HandleResearchPageInput(int pnum, int page, int boxid) {
	bool buystatus = 0;
	int curposx = MenuInputData[pnum][DND_MENUINPUT_LRPOS];
	if(HasLeftClicked(pnum)) {
		if(boxid == MBOX_1) {
			buystatus = CanResearch(page, curposx);
			if(!buystatus) {
				TakeInventory("Budget", ResearchInfo[page][curposx].res_cost);
				LocalAmbientSound("items/research", 127);
				CompleteResearch(ResearchInfo[page][curposx].res_id);
			}
			else
				ShowPopup(CheckInventory("DnD_PopupId"), false, 0);
		}
		ClearPlayerInput(pnum, false);
	}
}

void ResetInventoryLitState() {
	for(int i = 0; i < MAXLITBOXES; ++i)
		InventoryBoxLit[i] = BOXLIT_STATE_OFF;
}

void CleanInventoryInfo() {
	DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 14, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES);
}

int GetUniqueCharmXOffsetForDisplay(int charm_id) {
	switch(charm_id) {
		case UITEM_ELEMENTALBULWARK:
		return 4.0;
		case UITEM_GRAVECALLER:
		return 6.0;
		case UITEM_WELLOFPOWER:
		return 5.0;
		case UITEM_PELLETSTORM:
		return 2.0;
		case UITEM_ANCIENTGEMSTONE:
		return 10.0;
		case UITEM_OAKHEART:
		return 4.0;
		case UITEM_SHELLSHOCK:
		return 6.0;
		case UITEM_DEATHSPARK:
		return 4.0;
	}
	return 0;
}

void DrawCharmBox(int charm_type, int boxid, int thisboxid, int hudx, int hudy) {
	str charmborderpic = CharmBoxLabels[charm_type][boxid == thisboxid];
	str charmpic = "";
	int pnum = PlayerNumber();
	
	// fixes background being lit on first row boxes
	if(CheckInventory("DnD_InventoryView"))
		charmborderpic = CharmBoxLabels[charm_type][0];
	
	// if there is a charm here
	if(Charms_Used[pnum][thisboxid - 1].item_type != DND_ITEM_NULL) {
		charmpic = Charms_Used[pnum][thisboxid - 1].item_image;
		SetFont(Item_Images[charmpic]);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid - 1, CR_WHITE, hudx + GetUniqueCharmXOffsetForDisplay((Charms_Used[pnum][thisboxid - 1].item_type >> 16) - 1), hudy, 0.0, 0.0);
		
		if(boxid == thisboxid && !CheckInventory("DnD_InventoryView")) {
			CleanInventoryInfo();
			DrawInventoryInfo(thisboxid - 1, DND_SYNC_ITEMSOURCE_CHARMUSED, -1, HUDMAX_X, HUDMAX_Y);
		}
	}
	SetFont(charmborderpic);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid, CR_WHITE, hudx, hudy, 0.0, 0.0);
	SetFont("SMALLFONT");
}

int GetTradeViewItemOffset(int itype, int bid, int curoffset) {
	// side bar has 2 * MAX_INVENTORY_BOXES start anything else is less
	if(itype > UNIQUE_BEGIN) {
		if(bid == TRADE_BOXOFFSET)
			return curoffset + 4.0;
		if((itype >> 16) - 1 == UITEM_LIFELEECH)
			return curoffset;
		else if((itype >> 16) - 1 == UITEM_IRONBARK)
			return curoffset + 1.0;
		else if((itype >> 16) - 1 == UITEM_ANCIENTGEMSTONE)
			return curoffset - 4.0;
		return curoffset - 4.0;
	}
	else if(bid == TRADE_BOXOFFSET) {
		if(itype == DND_ITEM_CHARM)
			return curoffset + 4.0;
		else if(itype == DND_ITEM_ELIXIR)
			return curoffset + 4.0;
		else if(itype == DND_ITEM_ORB)
			return curoffset + 10.0;
		else if(itype == DND_ITEM_CHESTKEY)
			return curoffset + 10.0;
	}
	else if(itype == DND_ITEM_ELIXIR)
		return curoffset - 10.0;
	else if(itype == DND_ITEM_CHESTKEY)
		return curoffset - 6.0;
	return curoffset;
}

void DrawInventoryBlock(int idx, int idy, int bid, bool hasItem, int basex, int basey, int skip, int idbase, int source, int pnum, int boff, int itemskipx, int itemskipy, int scale) {
	int offset = 0;
	int temp;
	int img = GetItemSyncValue(DND_SYNC_ITEMIMAGE, bid, (pnum + 1) << 16, source);
	// inventory icon
	if(hasItem) {
		offset = (GetItemSyncValue(DND_SYNC_ITEMHEIGHT, bid, (pnum + 1) << 16, source) - 1) * scale;
		SetFont(Item_Images[img]);
		if(scale == TRADEITEMOFFSETSCALE) {
			// stash view
			if(basex == STASHBOX_BASEX_UP) {
				HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + offset - 4.0 + Item_ImageOffsets[img][IOFFSET_Y], 0.0, 0.0);
			}
			else {
				if((source & 0xFFFF) == DND_SYNC_ITEMSOURCE_TRADEVIEW) {
					temp = GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, (pnum + 1) << 16, source);
					HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - MAX_INVENTORY_BOXES - 2, CR_WHITE, GetTradeViewItemOffset(temp, boff, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + 3.0 - Item_ImageOffsets[img][IOFFSET_X]), basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + offset - 4.0 + Item_ImageOffsets[img][IOFFSET_Y], 0.0, 0.0);
				}
				else {
					if(boff == TRADE_BOXOFFSET) {
						temp = GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, (pnum + 1) << 16, source);
						HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - MAX_INVENTORY_BOXES - 2, CR_WHITE, GetTradeViewItemOffset(temp, boff, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx - 4.0 - Item_ImageOffsets[img][IOFFSET_X]), basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + offset - 4.0 + Item_ImageOffsets[img][IOFFSET_Y], 0.0, 0.0);
					}
					else
						HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx - 4.0 + Item_ImageOffsets[img][IOFFSET_X], basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + offset - 4.0 + Item_ImageOffsets[img][IOFFSET_Y], 0.0, 0.0);
				}
			}
		}
		else
			HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + offset, 0.0, 0.0);
	
		offset = GetItemSyncValue(DND_SYNC_ITEMSTACK, bid, (pnum + 1) << 16, source);
		if(offset) {
			SetFont("SMALLFONT");
			if(scale == TRADEITEMOFFSETSCALE) {
				if(basex == STASHBOX_BASEX_UP) {
					HudMessage(d:offset; HUDMSG_PLAIN, idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + 14.2, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + 20.0, 0.0, 0.0);
				}
				else {
					// trade but offerings side
					if((source & 0xFFFF) == DND_SYNC_ITEMSOURCE_TRADEVIEW)
						HudMessage(d:offset; HUDMSG_PLAIN, idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx - 8.2, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + 20.0, 0.0, 0.0);
					else {
						// inventory side in trade
						if(boff == TRADE_BOXOFFSET)
							HudMessage(d:offset; HUDMSG_PLAIN, idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + 14.2, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + 20.0, 0.0, 0.0);
						else // just inventory
							HudMessage(d:offset; HUDMSG_PLAIN, idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + 22.2, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + 20.0, 0.0, 0.0);
					}
				}
			}
			else
				HudMessage(d:offset; HUDMSG_PLAIN, idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + 14.2, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + 9.0, 0.0, 0.0);
		}
		else
			DeleteText(idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2);
	}
	else { // refresh picture if swapped
		DeleteText(idbase - bid - boff - MAX_INVENTORY_BOXES - 2);
		DeleteText(idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2);
	}
	// gray inventory chunks
	if(InventoryBoxLit[bid + boff] == BOXLIT_STATE_CURSORON)
		SetFont("LDTBOXS");
	else if(CheckActorInventory(pnum + P_TIDSTART, "DnD_Trade_Confirmed"))
		SetFont("LDTBOXCN");
	else if(InventoryBoxLit[bid + boff] == BOXLIT_STATE_CLICK)
		SetFont("LDTBOXC");
	else if(GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, (pnum + 1) << 16, source))
		SetFont("LDTBOXO");
	else
		SetFont("LDTBOX");
	HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - 1, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * skip, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * skip, 0.0, 0.0);
}

void DrawInventoryInfo(int topboxid, int source, int pnum, int dimx, int dimy) {
	int pn, mx, my, offset, stack = 0;
	if(pnum == -1)
		pn = (PlayerNumber() + 1) << 16;
	else
		pn = (pnum + 1) << 16;
	if(CheckInventory("DnD_SelectedCharmBox"))
		DrawInventoryInfo_Field(CheckInventory("DnD_SelectedCharmBox") - 1, DND_SYNC_ITEMSOURCE_CHARMUSED, 24.4, 0.1, true);
	int itype = GetItemSyncValue(DND_SYNC_ITEMTYPE, topboxid, pn, source);
	if(GetItemSyncValue(DND_SYNC_ITEMTYPE, topboxid, pn, source) != DND_ITEM_NULL) {
		mx = HUDMAX_XF - (CheckInventory("Mouse_X") & MMASK) + 16.1 , my = HUDMAX_YF - (CheckInventory("Mouse_Y") & MMASK) + 16.1;
		SetFont("LDTITINF");
		// to force them to appear in window
		if(dimx == HUDMAX_X) {
			if(mx > INVENTORYINFO_NORMALVIEW_WRAPX)
				mx = INVENTORYINFO_NORMALVIEW_WRAPX + 0.1;
			if(my > INVENTORYINFO_NORMALVIEW_WRAPY)
				my = INVENTORYINFO_NORMALVIEW_WRAPY + 0.1;
		}
		else {
			// tradeview
			if(mx > INVENTORYINFO_TRADEVIEW_WRAPX)
				mx = INVENTORYINFO_TRADEVIEW_WRAPX + 0.1;
			if(my > INVENTORYINFO_TRADEVIEW_WRAPY)
				my = INVENTORYINFO_TRADEVIEW_WRAPY + 0.1;
		}
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES, CR_WHITE, mx, my, 0.0, INVENTORY_INFO_ALPHA);
		stack = GetItemSyncValue(DND_SYNC_ITEMSTACK, topboxid, pn, source);
		if(stack) {
			SetFont("SMALLFONT");
			HudMessage(d:stack; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 14, CR_GREEN, mx + 200.1, my + 16.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
		}
		mx += 64.0;
		my += 40.0;
		mx &= MMASK;
		my &= MMASK;
		mx += 0.4;
		my += 0.1;
		// show item details
		if(itype == DND_ITEM_ORB || itype == DND_ITEM_ELIXIR)
			offset = 16.0;
		else if(itype == DND_ITEM_CHESTKEY)
			offset = 8.0;
		SetFont(Item_Images[GetItemSyncValue(DND_SYNC_ITEMIMAGE, topboxid, pn, source)]);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 1, CR_WHITE, mx + 40.0, my - 32.0 + offset, 0.0, INVENTORY_INFO_ALPHA);
		SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
		mx *= 3; mx /= 2;
		my *= 3; my /= 2;
		mx &= MMASK;
		my &= MMASK;
		mx += 0.4;
		my += 0.1;
		SetHudClipRect(-72 + (mx >> 16), -48 + (my >> 16), 256, 192, 256, 1);
		DrawInventoryInfoText(topboxid, source, pn, mx, my, itype);
		SetHudClipRect(0, 0, 0, 0, 0);
		SetHudSize(dimx, dimy, 1);
	}
}

void DrawInventoryInfoText(int topboxid, int source, int pn, int mx, int my, int itype) {
	int i, j, temp, val;
	SetFont("SMALLFONT");
	if(itype == DND_ITEM_CHARM) {
		HudMessage(s:Charm_Tiers[GetItemSyncValue(DND_SYNC_ITEMLEVEL, topboxid, pn, source) / CHARM_ATTRIBLEVEL_SEPERATOR], s: " ", s:Charm_TypeName[GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, topboxid, pn, source)], s:" Charm"; 
			HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 2, CR_WHITE, mx + 56.0, my - 36.1, 0.0, INVENTORY_INFO_ALPHA
		);
		i = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, topboxid, pn, source);
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j | pn, source);
			val = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j | pn, source);
			HudMessage(s:ItemAttributeString(temp, val); HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3 - j, CR_WHITE, mx + 56.0, my + 24.0 * j, 0.0, INVENTORY_INFO_ALPHA);
		}
	}
	else if(itype == DND_ITEM_ORB || itype == DND_ITEM_CHESTKEY || itype == DND_ITEM_ELIXIR) {
		temp = GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, topboxid, -1, source) + GetInventoryInfoOffset(itype);
		HudMessage(s:InventoryInfo[temp][SITEM_DESC]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3, CR_WHITE, mx + 56.0, my + 24.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
	}
	else if(itype > UNIQUE_BEGIN) {
		temp = itype & 0xFFFF;
		itype >>= UNIQUE_BITS;
		--itype;
		// itype holds unique position, temp is the actual item type
		HudMessage(s:"\c[A1]", s:UniqueItemNames[itype]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 2, CR_WHITE, mx + 56.0, my - 36.1, 0.0, INVENTORY_INFO_ALPHA);
		HudMessage(s:"\c[D1]Unique ", s:Charm_TypeName[GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, topboxid, pn, source)], s:" Charm"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3, CR_WHITE, mx + 56.0, my - 20.1, 0.0, INVENTORY_INFO_ALPHA);
		i = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, topboxid, pn, source);
		// itype will count the skipped properties (the helper attributes)
		itype = 0;
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j | pn, source);
			val = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j | pn, source);
			if(val > 0) {
				// dont show this, skip to next attribute's detail
				if(temp == INV_EX_CHANCE) {
					++j;
					++itype;
					HudMessage(s:ExoticAttributeString(GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, topboxid, j | pn, source), val, GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, topboxid, j | pn, source)); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
				}
				else
					HudMessage(s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
			}
			else if(!val) {
				// unique item doesn't have numeric attribute to show
				HudMessage(s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
			}
			else
				HudMessage(s:"- ", s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
		}
	}
}

void DoInventoryBoxDraw(int boxid, int prevclick, int bh, int bw, int basex, int basey, int skip, int idbase, int offset, int source, int pnum, int dimx, int dimy) {
	int topboxid = -1;
	int bid = bh * MAXINVENTORYBLOCKS_VERT + bw;
	int ht, wt;
	int temp;
	int s, p;
	
	if(boxid != MAINBOX_NONE)
		topboxid = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - offset, (pnum + 1) << 16, source) - 1;
	
	bool hasItem = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, bid, (pnum + 1) << 16, source) - 1 == bid && GetItemSyncValue(DND_SYNC_ITEMTYPE, bid, (pnum + 1) << 16, source) != DND_ITEM_NULL;
	
	// click coloring
	if(prevclick != -1) {
		// we need offset taken here because data is in 0 - 44 cells
		temp = GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, prevclick - offset, (pnum + 1) << 16, source) - 1;
		if(temp != -1) {
			// use topbox here
			ht = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, temp, (pnum + 1) << 16, source);
			wt = GetItemSyncValue(DND_SYNC_ITEMWIDTH, temp, (pnum + 1) << 16, source);
			// in the part below, use the real value of actual box clicked for highlighting
			for(p = 0; p < ht; ++p) {
				for(s = 0; s < wt; ++s)
					InventoryBoxLit[temp + offset + s + p * MAXINVENTORYBLOCKS_VERT] = BOXLIT_STATE_CLICK;
			}
		}
		else {
			if(((source & 0xFFFF) == DND_SYNC_ITEMSOURCE_STASH && CheckInventory("DnD_PlayerPreviousPage") == CheckInventory("DnD_PlayerCurrentPage")) || (source & 0xFFFF) != DND_SYNC_ITEMSOURCE_STASH)
				InventoryBoxLit[prevclick] = BOXLIT_STATE_CLICK;
		}
	}
	
	// highlight checking
	if(boxid - 1 == bid + offset) {
		InventoryBoxLit[boxid - 1] = BOXLIT_STATE_CURSORON;
		if(topboxid != -1) {
			CleanInventoryInfo();
			DrawInventoryInfo(topboxid, source, pnum, dimx, dimy);
		}
	}
	if(topboxid != -1) {
		ht = GetItemSyncValue(DND_SYNC_ITEMHEIGHT, topboxid, (pnum + 1) << 16, source);
		wt = GetItemSyncValue(DND_SYNC_ITEMWIDTH, topboxid, (pnum + 1) << 16, source);
		// all boxes in range of this should be highlighted
		for(p = 0; p < ht; ++p) {
			for(s = 0; s < wt; ++s)
				InventoryBoxLit[topboxid + s + p * MAXINVENTORYBLOCKS_VERT + offset] = BOXLIT_STATE_CURSORON;
		}
	}
	
	if(dimx == HUDMAX_X)
		DrawInventoryBlock(bh, bw, bid, hasItem, basex, basey, skip, idbase, source, pnum, offset, 32.0, 32.0, 16.0);
	else
		DrawInventoryBlock(bh, bw, bid, hasItem, basex, basey, skip, idbase, source, pnum, offset, 32.0, 32.0, TRADEITEMOFFSETSCALE);
}

void HandleInventoryView(int boxid) {
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	SetFont("LDTSCRN");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID, CR_WHITE, HUDMAX_XF / 2, HUDMAX_YF / 2, 0.0, 0.0);
	ResetInventoryLitState();
	CleanInventoryInfo();
	for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX, INVENTORYBOX_BASEY, 32.0, RPGMENUINVENTORYID, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerNumber(), HUDMAX_X, HUDMAX_Y);
		}
	}
	SetFont("SMALLFONT");
}

// This is mainly used for stack editing
void HandleM2Inputs(int boxid, int source, int seloffset, int prevsource) {
	int ipos, epos, bid;
	if(CheckInventory("DnD_SelectedInventoryBox")) {
		ipos = CheckInventory("DnD_SelectedInventoryBox") - 1 - seloffset;
		source = prevsource;
	}
	else
		ipos = boxid - 1;
	int istack = GetItemSyncValue(DND_SYNC_ITEMSTACK, ipos, -1, source);
	if(istack > 1) {
		// no previously selected box, just right click => halve a stack
		if(CheckInventory("DnD_SelectedInventoryBox")) {
			// always leave the odd one out on the new stack
			bid = istack / 2;
			epos = CloneItem(ipos, source, true);
			SetItemSyncValue(DND_SYNC_ITEMSTACK, epos, -1, bid, source);
			if(istack > 2 && (istack & 1))
				++bid;
			SetItemSyncValue(DND_SYNC_ITEMSTACK, ipos, -1, bid, source);
			// create a new item of this type, halve the stack on it
			SyncItemData(epos, source, -1, -1);
		}
		else {
			// we need to have previously clicked on the item for this, then we take only one out of it
			SetItemSyncValue(DND_SYNC_ITEMSTACK, ipos, -1, istack - 1, source);
			epos = CloneItem(ipos, source, true);
			SetItemSyncValue(DND_SYNC_ITEMSTACK, epos, -1, 1, source);
			SyncItemData(epos, source, -1, -1);
		}
		
		if(GetItemSyncValue(DND_SYNC_ITEMSTACK, ipos, -1, source)) {
			// sync just stack as there are more left here
			SyncItemStack(ipos, source);
		}
		else {
			// sync the whole item after freeing
			FreeItem(ipos, source, false);
		}
	}
	SetInventory("DnD_SelectedInventoryBox", 0);
}

void HandleInventoryViewClicks(int boxid, int choice) {
	int bid;
	int epos, ipos;
	if(choice == DND_MENUINPUT_LCLICK) {
		if(boxid != MAINBOX_NONE) {
			// m1
			if(!CheckInventory("DnD_SelectedInventoryBox")) {
				SetInventory("DnD_SelectedInventoryBox", boxid);
			}
			else if(boxid != CheckInventory("DnD_SelectedInventoryBox")) {
				// if neither are empty spots
				int pnum = PlayerNumber();
				bool boxidon = PlayerInventoryList[pnum][boxid - 1].item_type != DND_ITEM_NULL;
				bool prevselecton = PlayerInventoryList[pnum][CheckInventory("DnD_SelectedInventoryBox") - 1].item_type != DND_ITEM_NULL;
				if(boxidon && prevselecton) {
					// if both of them point to the same pointer, we need to move this item instead
					if(PlayerInventoryList[pnum][boxid - 1].topleftboxid == PlayerInventoryList[pnum][CheckInventory("DnD_SelectedInventoryBox") - 1].topleftboxid) {
						ipos = CheckInventory("DnD_SelectedInventoryBox") - 1;
						epos = boxid - 1;
						if(IsFreeSpot(ipos, epos))
							MoveItem(ipos, epos);
					}
					else
						SwapItems(boxid - 1, CheckInventory("DnD_SelectedInventoryBox") - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
				}
				else {
					// find which one has an item, and move it
					if(!boxidon) {
						epos = boxid - 1;
						ipos = CheckInventory("DnD_SelectedInventoryBox") - 1;
					}
					else {
						epos = CheckInventory("DnD_SelectedInventoryBox") - 1;
						ipos = boxid - 1;
					}
					// epos holds the empty position now
					// make sure we aren't both empty slots
					if((boxidon || prevselecton) && IsFreeSpot(ipos, epos)) {
						MoveItem(ipos, epos);
					}
				}
				SetInventory("DnD_SelectedInventoryBox", 0);
			}
			else
				SetInventory("DnD_SelectedInventoryBox", 0);
		}
		else if(CheckInventory("DnD_SelectedInventoryBox") && GetItemSyncValue(DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) != DND_ITEM_NULL) {
			// drop selected item
			DropItemToField(PlayerNumber(), CheckInventory("DnD_SelectedInventoryBox") - 1, true, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			ACS_NamedExecuteAlways("DnD Save Player Item Data", PlayerNumber() | (CheckInventory("DnD_CharacterID") << 16), CheckInventory("DnD_SelectedInventoryBox") - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("DnD_SelectedInventoryBox", 0);
			ActivatorSound("Items/Drop", 127);
		}
	}
	else if(choice == DND_MENUINPUT_RCLICK) {
		HandleM2Inputs(boxid, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	}
	LocalAmbientSound("RPG/MenuChoose", 127);
}

void HandleItemPageInputs(int pnum, int boxid) {
	int charm_sel, charm_type, topboxid;
	if(HasLeftClicked(pnum)) {
		if(boxid != MAINBOX_NONE) {
			// we pressed a charm box or view inventory box
			if(!CheckInventory("DnD_InventoryView")) {
				GiveInventory("DnD_InventoryView", 1);
				LocalAmbientSound("RPG/MenuChoose", 127);
				if(boxid != MBOX_8) // mark our previous box selection if we clicked on a charm
					SetInventory("DnD_SelectedCharmBox", boxid);
			}
			else {
				// now track our choices made in the inventory view
				charm_sel = CheckInventory("DnD_SelectedCharmBox");
				charm_type = 0;
				topboxid = PlayerInventoryList[pnum][boxid - 1].topleftboxid - 1;
				if(charm_sel) {
					if(charm_sel <= MBOX_4) // small charm replacement
						charm_type = DND_CHARM_SMALL;
					else if(charm_sel <= MBOX_6) // medium charm replacement
						charm_type = DND_CHARM_MEDIUM;
					else // grand charm replacement
						charm_type = DND_CHARM_LARGE;
					--charm_sel;
					if(topboxid != -1 && (PlayerInventoryList[pnum][topboxid].item_type & 0xFFFF) == DND_ITEM_CHARM) {
						if(MakeCharmUsed(charm_sel, topboxid, charm_type)) {
							LocalAmbientSound("Items/CharmDrop", 127);
							TakeInventory("DnD_InventoryView", 1);
							SetInventory("DnD_SelectedCharmBox", 0);
						}
						else
							ShowPopup(POPUP_CHARMMISMATCH, false, 0);
					}
					else {
						if(topboxid == -1)
							ShowPopup(POPUP_NOITEMTHERE, false, 0);
						else
							ShowPopup(POPUP_ITEMTYPEMISMATCH, false, 0);
					}
				}
				else {
					// normal clicking functionality on inventory view
					// 1 is for m1
					HandleInventoryViewClicks(boxid, 1);
				}
			}
		}
		else {
			// we clicked outside bounds
			if(CheckInventory("DnD_InventoryView")) {
				// ok we are in inventory view
				if(CheckInventory("DnD_SelectedInventoryBox")) {
					// we have selected a box previously, if this has an item drop it otherwise clear it
					if(GetItemSyncValue(DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) != DND_ITEM_NULL) {
						// drop selected item
						DropItemToField(PlayerNumber(), CheckInventory("DnD_SelectedInventoryBox") - 1, true, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
						ACS_NamedExecuteAlways("DnD Save Player Item Data", PlayerNumber() | (CheckInventory("DnD_CharacterID") << 16), CheckInventory("DnD_SelectedInventoryBox") - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
						ActivatorSound("Items/Drop", 127);
					}
					// clear selection
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
				else { 
					// just exit if nothing was selected
					TakeInventory("DnD_InventoryView", 1);
					LocalAmbientSound("RPG/MenuClose", 127);
				}
			}
		}
		ClearPlayerInput(pnum, true);
	}
	else if(HasRightClicked(pnum)) {
		// mbox 8 is the view inventory button
		if(!CheckInventory("DnD_InventoryView") && boxid != MAINBOX_NONE && boxid != MBOX_8 && Charms_Used[pnum][boxid - 1].item_type != DND_ITEM_NULL) {
			// try to drop item
			charm_sel = GetFreeSpotForItem(boxid - 1, pnum, DND_SYNC_ITEMSOURCE_CHARMUSED);
			if(charm_sel != -1) {
				LocalAmbientSound("Items/CharmDrop", 127);
				RemoveItemFeatures(boxid - 1, DND_SYNC_ITEMSOURCE_CHARMUSED);
				MoveItemTrade(boxid - 1, charm_sel, DND_SYNC_ITEMSOURCE_CHARMUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				// force a damage cache recalc
				ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, PlayerNumber());
			}
			else
				ShowPopup(POPUP_NOSPOTFORITEM, false, 0);
		}
		ClearPlayerInput(pnum, true);
	}
}

void EngageTrade(int engager, int recipient) {
	if(engager > 31)
		SetActorInventory(recipient + P_TIDSTART, "DnD_TradeEngaged_2", SetBit(CheckActorInventory(recipient + P_TIDSTART, "DnD_TradeEngaged_2"), engager - 32));
	else
		SetActorInventory(recipient + P_TIDSTART, "DnD_TradeEngaged_1", SetBit(CheckActorInventory(recipient + P_TIDSTART, "DnD_TradeEngaged_1"), engager));
	
	if(recipient > 31)
		SetActorInventory(engager + P_TIDSTART, "DnD_TradeEngaged_2", SetBit(CheckActorInventory(engager + P_TIDSTART, "DnD_TradeEngaged_2"), recipient - 32));
	else
		SetActorInventory(engager + P_TIDSTART, "DnD_TradeEngaged_1", SetBit(CheckActorInventory(engager + P_TIDSTART, "DnD_TradeEngaged_1"), recipient));
}

void ToggleMuteTrade(int mutenum) {
	if(mutenum > 31)
		SetInventory("DnD_TradeMute_2", XorBit(CheckInventory("DnD_TradeMute_2"), mutenum - 32));
	else
		SetInventory("DnD_TradeMute_1", XorBit(CheckInventory("DnD_TradeMute_1"), mutenum));
}

bool TradeMuteStatus(int source, int target) {
	bool res = false;
	int check = CheckActorInventory(source + P_TIDSTART, "DnD_TradeMute_1");
	if(target > 31) {
		check = CheckActorInventory(source + P_TIDSTART, "DnD_TradeMute_2");
		target -= 32;
	}
	return IsSet(check, target);
}

bool IsTrading(int pnum) {
	return CheckActorInventory(pnum + P_TIDSTART, "DnD_TradeEngaged_1") || CheckActorInventory(pnum + P_TIDSTART, "DnD_TradeEngaged_2");
}

bool IsTradingWith(int pnum, int other) {
	str check1 = "DnD_TradeEngaged_1";
	str check2 = "DnD_TradeEngaged_1";
	int bit1 = pnum, bit2 = other;
	if(pnum > 31) {
		check1 = "DnD_TradeEngaged_2";
		bit1 -= 32;
	}
	if(other > 31) {
		check2 = "DnD_TradeEngaged_2";
		bit2 -= 32;
	}
	return IsSet(CheckActorInventory(pnum + P_TIDSTART, check1), bit2) && IsSet(CheckActorInventory(other + P_TIDSTART, check2), bit1);
}

int GetTradee() {
	int i;
	for(i = 0; i < 32; ++i)
		if(IsSet(CheckInventory("DnD_TradeEngaged_1"), i))
			return i;
	for(i = 0; i < 32; ++i)
		if(IsSet(CheckInventory("DnD_TradeEngaged_2"), i))
			return i + 32;
	return -1;
}

void HandleTradeBoxDraw(int boxid, int dimx, int dimy) {
	int bid, prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int pnum = PlayerNumber();
	int i, j;
	
	CleanInventoryInfo();
	ResetInventoryLitState();
	// draw other player offering up top
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX_TRADEUP, INVENTORYBOX_BASEY_TRADEUP, 32.0, RPGMENUINVENTORYID - 4, 0, DND_SYNC_ITEMSOURCE_TRADEVIEW, GetTradee(), dimx, dimy);
		}
	}
	ResetInventoryLitState();
	// draw our offering below
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX_TRADEDOWN, INVENTORYBOX_BASEY_TRADEDOWN, 32.0, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 6, MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_TRADEVIEW, pnum, dimx, dimy);
		}
	}
	ResetInventoryLitState();
	// draw our inventory to the side
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX_TRADEMID, INVENTORYBOX_BASEY_TRADEMID, 32.0, RPGMENUINVENTORYID - 6 * MAX_INVENTORY_BOXES - 12, 2 * MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, pnum, dimx, dimy);
		}
	}
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("TRADEBAR");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 7, CR_WHITE, 110.4, 160.0, 0.0, 0.0);
	
	SetFont("SMALLFONT");
	--boxid;
	if(CheckInventory("DnD_Trade_ConfirmButtonPress"))
		HudMessage(s:"\cdConfirm"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 10, CR_WHITE, 52.4, 160.0, 0.0, 0.0);
	else if(boxid == TRADECONFIRM_BOXID)
		HudMessage(s:"\c[M3]Confirm"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 10, CR_WHITE, 52.4, 160.0, 0.0, 0.0);
	else
		HudMessage(s:"Confirm"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 10, CR_WHITE, 52.4, 160.0, 0.0, 0.0);
	
	if(CheckInventory("DnD_Trade_CancelButtonPress"))
		HudMessage(s:"\cdCancel"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 11, CR_WHITE, 168.4, 160.0, 0.0, 0.0);
	else if(boxid == TRADECANCEL_BOXID)
		HudMessage(s:"\c[M3]Cancel"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 11, CR_WHITE, 168.4, 160.0, 0.0, 0.0);
	else
		HudMessage(s:"Cancel"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 11, CR_WHITE, 168.4, 160.0, 0.0, 0.0);
	
	if(CheckInventory("DnD_Trade_ConfirmButtonPress"))
		SetFont("TRADBTNC");
	else if(CheckInventory("DnD_Trade_Confirmed"))
		SetFont("TRADBTNO");
	else if(boxid == TRADECONFIRM_BOXID)
		SetFont("TRADBTNH");
	else
		SetFont("TRADBTN");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 8, CR_WHITE, 52.4, 160.0, 0.0, 0.0);
	
	if(CheckInventory("DnD_Trade_CancelButtonPress"))
		SetFont("TRADBTNC");
	else if(boxid == TRADECANCEL_BOXID)
		SetFont("TRADBTNH");
	else
		SetFont("TRADBTN");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 9, CR_WHITE, 168.4, 160.0, 0.0, 0.0);
}

void HandleInventoryViewTrade(int boxid) {
	SetFont("LDTSCRN");
	SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 1, CR_WHITE, 0.1, 120.0, 0.0, 0.0);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 2, CR_WHITE, 0.1, 360.0, 0.0, 0.0);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 3, CR_WHITE, 364.1, 260.0, 0.0, 0.0);
	SetFont("SMALLFONT");
	HudMessage(n:GetTradee() + 1, s:"\c[W3]'s Offering"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 4, CR_WHITE, 160.4, 30.0, 0.0, 0.0);
	HudMessage(s:"\c[W3]Your Offering"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 5, CR_WHITE, 160.4, 270.0, 0.0, 0.0);
	HudMessage(s:"\c[W3]Your Inventory"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 6, CR_WHITE, 528.4, 170.0, 0.0, 0.0);
	
	HandleTradeBoxDraw(boxid, HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2);
	
	SetFont("SMALLFONT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

// Just takes care of backend trade details
void CancelTrade(int pnum) {
	int tid = pnum + P_TIDSTART;
	TakeActorInventory(tid, "InTradeView", 1);
	TakeActorInventory(tid, "DnD_TradeSpaceFit", 1);
	TakeActorInventory(tid, "DnD_TradeAcceptWindow", 1);
	TakeActorInventory(tid, "DnD_Trade_Confirmed", 1);
	TakeActorInventory(tid, "DnD_TradeSpaceFit", 1);
	SetActorInventory(tid, "DnD_TradeEngaged_1", 0);
	SetActorInventory(tid, "DnD_TradeEngaged_2", 0);
	SetActorInventory(tid, "DnD_SelectedInventoryBox", 0);
}

void CancelTradeRoutine(int tradee) {
	ReturnTradeItems(PlayerNumber());
	ReturnTradeItems(tradee);
	CancelTrade(PlayerNumber());
	CancelTrade(tradee);
}

// returns allocated items in trade views to this player
void ReturnTradeItems(int pnum) {
	// from trade view of this player, move items to their new places
	int bid;
	for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
		for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			if(TradeViewList[pnum][bid].item_type != DND_ITEM_NULL) {
				int pos = GetFreeSpotForItem(bid, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW);
				if(pos != -1)
					MoveItemTrade(bid, pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				else {
					// drop this item, just in case
					// for now, since this isn't very likely to happen, just log an error message
					Log(s:"Attempted to return item from trade view but no fitting slot found! Drop item.");
				}
			}
		}
}

void HandleTradeCountdown(int p1, int p2) {
	SetActivator(p1 + P_TIDSTART);
	ACS_NamedExecuteAlways("DnD Trade Countdown", 0, p1);
	SetActivator(p2 + P_TIDSTART);
	ACS_NamedExecuteAlways("DnD Trade Countdown", 0, p2);
	ACS_NamedExecuteAlways("DnD Trade Counter", 0, p1, p2);
}

void HandleTradeViewButtonClicks(int pnum, int boxid) {
	int bid = GetTradee();
	int ioffset = 0, isource, soffset = 0, ssource;
	
	if(!PlayerInGame(bid))
		CancelTrade(PlayerNumber());
	
	if(HasLeftClicked(pnum)) {
		// we aren't allowed the click the top side (other player's view)
		if(boxid > MAX_INVENTORY_BOXES) {
			if(boxid == 3 * MAX_INVENTORY_BOXES + 1) {
				// confirm button
				// if we haven't pressed already
				if(!CheckInventory("DnD_Trade_Confirmed") && !CheckInventory("DnD_Trade_Cooldown")) {
					// check if we have space for all the offerings in our inventory
					bool ok_from_us = true, ok_from_tradee = true;
					if(!CheckInventory("DnD_TradeSpaceFit")) {
						ok_from_us = ConfirmSpaceForOfferings(bid, PlayerNumber());
						GiveInventory("DnD_TradeSpaceFit", 1);
					}
					
					if(!CheckActorInventory(bid + P_TIDSTART, "DnD_TradeSpaceFit")) {
						ok_from_tradee = ConfirmSpaceForOfferings(PlayerNumber(), bid);
						GiveActorInventory(bid + P_TIDSTART, "DnD_TradeSpaceFit", 1);
					}
					
					if(ok_from_tradee && ok_from_us) {
						// if we do, make our confirmation real
						GiveInventory("DnD_Trade_Confirmed", 1);
						GiveInventory("DnD_Trade_ConfirmButtonPress", 1);
						
						// if other side's confirmation is set, transfer items
						if(CheckActorInventory(bid + P_TIDSTART, "DnD_Trade_Confirmed")) {
							// start countdown, when it's ready, proceed
							HandleTradeCountdown(bid, PlayerNumber());
						}
					}
					else {
						// unconfirm the trade
						if(!ok_from_tradee) {
							SetActorInventory(bid + P_TIDSTART, "DnD_Trade_Confirmed", 0);
							ShowActorPopup(bid, POPUP_NOSPACEFORTRADE, false, 0);
						}
						else {
							SetInventory("DnD_Trade_Confirmed", 0);
							// show popup for not enough space
							ShowPopup(POPUP_NOSPACEFORTRADE, false, 0);
						}
					}
				}
				else {
					// cancel confirmation
					TakeInventory("DnD_Trade_Confirmed", 1);
					TakeInventory("DnD_TradeSpaceFit", 1);
					TakeActorInventory(bid + P_TIDSTART, "DnD_Trade_Confirmed", 1);
					TakeActorInventory(bid + P_TIDSTART, "DnD_TradeSpaceFit", 1);
				}
				LocalAmbientSound("RPG/MenuChoose", 127);
			}
			else if(boxid == 3 * MAX_INVENTORY_BOXES + 2) {
				// cancel button
				GiveInventory("DnD_Trade_CancelButtonPress", 1);
				GiveInventory("DnD_Trade_Cooldown", 1);
				ACS_NamedExecuteAlways("DnD Trade Cancel Button", 0, bid);
				LocalAmbientSound("RPG/MenuChoose", 127);
			}
			else if(boxid != MAINBOX_NONE) {
				if(!CheckInventory("DnD_Trade_Confirmed") && !CheckInventory("DnD_Trade_CancelButtonPress")) {
					// we pressed a box, and we didn't confirm trade yet
					// normal clicking functionality on inventory view
					if(!CheckInventory("DnD_SelectedInventoryBox"))
						SetInventory("DnD_SelectedInventoryBox", boxid);
					else if(boxid != CheckInventory("DnD_SelectedInventoryBox")) {
						int epos, ipos;
						// i is for current click, s for previous selection
						isource = DND_SYNC_ITEMSOURCE_TRADEVIEW;
						ioffset = MAX_INVENTORY_BOXES;
						ssource = DND_SYNC_ITEMSOURCE_TRADEVIEW;
						soffset = MAX_INVENTORY_BOXES;
						
						// set up source and offsets
						if(boxid > 2 * MAX_INVENTORY_BOXES) {
							isource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
							ioffset = 2 * MAX_INVENTORY_BOXES;
						}
						
						if(CheckInventory("DnD_SelectedInventoryBox") > 2 * MAX_INVENTORY_BOXES) {
							ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
							soffset = 2 * MAX_INVENTORY_BOXES;
						}
						
						int temp;
						bool boxidon = GetItemSyncValue(DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL;
						bool prevselecton = GetItemSyncValue(DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource) != DND_ITEM_NULL;
						if(boxidon && prevselecton) {
							// if both of them point to the same pointer, we need to move this item instead
							if(isource == ssource && GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - ioffset, -1, isource) == GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource)) {
								ipos = CheckInventory("DnD_SelectedInventoryBox") - 1;
								epos = boxid - 1;
								/*
								// swap because the ipos place is changed
								temp = ssource;
								ssource = isource;
								isource = temp;
								temp = soffset;
								soffset = ioffset;
								ioffset = temp;
								*/
								if(IsFreeSpot_Trade(ipos - ioffset, epos - soffset, isource, ssource))
									MoveItemTrade(ipos - ioffset, epos - soffset, isource, ssource);
							}
							else
								SwapItems(boxid - 1 - ioffset, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, isource, ssource, false);
						}
						else {
							// find which one has an item, and move it
							if(!boxidon) {
								epos = boxid - 1;
								ipos = CheckInventory("DnD_SelectedInventoryBox") - 1;
								// swap because the ipos place is changed
								temp = ssource;
								ssource = isource;
								isource = temp;
								temp = soffset;
								soffset = ioffset;
								ioffset = temp;
							}
							else {
								epos = CheckInventory("DnD_SelectedInventoryBox") - 1;
								ipos = boxid - 1;
							}
							// epos holds the empty position now
							// make sure we aren't both empty slots
							if((boxidon || prevselecton) && IsFreeSpot_Trade(ipos - ioffset, epos - soffset, isource, ssource)) {
								MoveItemTrade(ipos - ioffset, epos - soffset, isource, ssource);
							}
						}
						SetInventory("DnD_SelectedInventoryBox", 0);
					}
					else
						SetInventory("DnD_SelectedInventoryBox", 0);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(CheckInventory("DnD_SelectedInventoryBox")) {
					ssource = DND_SYNC_ITEMSOURCE_TRADEVIEW;
					soffset = MAX_INVENTORY_BOXES;
					
					if(CheckInventory("DnD_SelectedInventoryBox") > 2 * MAX_INVENTORY_BOXES) {
						ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
						soffset = 2 * MAX_INVENTORY_BOXES;
					}
					
					if(GetItemSyncValue(DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource) != DND_ITEM_NULL) {
						// drop selected item
						DropItemToField(PlayerNumber(), CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, true, ssource);
						ACS_NamedExecuteAlways("DnD Save Player Item Data", PlayerNumber() | (CheckInventory("DnD_CharacterID") << 16), CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, ssource);
						SetInventory("DnD_SelectedInventoryBox", 0);
						ActivatorSound("Items/Drop", 127);
					}
				}
			}
		}
	}
	else if(HasRightClicked(pnum) && boxid <= 3 * MAX_INVENTORY_BOXES && boxid != MAINBOX_NONE) {
		isource = DND_SYNC_ITEMSOURCE_TRADEVIEW;
		ioffset = MAX_INVENTORY_BOXES;
		ssource = DND_SYNC_ITEMSOURCE_TRADEVIEW;
		soffset = MAX_INVENTORY_BOXES;
		
		// set up source and offsets
		if(boxid > 2 * MAX_INVENTORY_BOXES) {
			isource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
			ioffset = 2 * MAX_INVENTORY_BOXES;
		}
		
		if(CheckInventory("DnD_SelectedInventoryBox") > 2 * MAX_INVENTORY_BOXES) {
			ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
			soffset = 2 * MAX_INVENTORY_BOXES;
		}
		HandleM2Inputs(boxid - ioffset, isource, soffset, ssource);
		LocalAmbientSound("RPG/MenuChoose", 127);
	}
}

void HandleStashView(int boxid) {
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int pnum = PlayerNumber(), i, j;
	int curpages = CheckInventory("DnD_PlayerCurrentPage") - 1;
	int color;
	SetFont("LDTSCRN");
	SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 1, CR_WHITE, 452.4, 120.0, 0.0, 0.0);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 2, CR_WHITE, 452.4, 360.0, 0.0, 0.0);
	
	ResetInventoryLitState();
	CleanInventoryInfo();
	
	// draw stash at top
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, STASHBOX_BASEX_UP, INVENTORYBOX_BASEY_TRADEUP, 32.0, RPGMENUINVENTORYID - 4, 0, DND_SYNC_ITEMSOURCE_STASH | (curpages << 16), pnum, TRADE_RES_X, TRADE_RES_Y);
		}
	}
	// draw our inventory below
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, STASHBOX_BASEX_UP, INVENTORYBOX_BASEY_TRADEDOWN, 32.0, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 6, MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, pnum, TRADE_RES_X, TRADE_RES_Y);
		}
	}
	
	// stash buttons
	for(i = 0; i < CheckInventory("DnD_PlayerInventoryPages"); ++i) {
		if(CheckInventory(StrParam(s:"DnD_ButtonPress_", d:i + 1)) || curpages == i) {
			SetFont("DND_SBC");
			color = CR_GREEN;
		}
		else if(boxid == STASHBUTTON_BOXID_START + i) {
			SetFont("DND_SBO");
			color = CR_GREEN;
		}
		else {
			SetFont("DND_SBN");
			color = CR_CYAN;
		}
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 2 * i, CR_WHITE, 388.4 + 15.0 * i, 210.0, 0.0, 0.0);
		SetFont("SMALLFONT");
		HudMessage(d:i + 1; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 1 - 2 * i, color, 388.4 + 15.0 * i, 210.0, 0.0, 0.0);
	}
	
	SetFont("SMALLFONT");
	HudMessage(s:"\c[W3]Stash"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 2, CR_WHITE, 452.4, 30.0, 0.0, 0.0);
	HudMessage(s:"\c[W3]Inventory"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 3, CR_WHITE, 452.4, 270.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void HandleStashViewClicks(int boxid, int choice) {
	int temp = 0;
	int cpage, ppage, ssource, isource;
	int ioffset = 0;
	int soffset = 0;
	if(choice == DND_MENUINPUT_LCLICK) {
		if(!CheckInventory("DnD_SelectedInventoryBox") && boxid < STASHBUTTON_BOXID_START) {
			SetInventory("DnD_SelectedInventoryBox", boxid);
			LocalAmbientSound("RPG/MenuChoose", 127);
			SetInventory("DnD_PlayerPreviousPage", CheckInventory("DnD_PlayerCurrentPage"));
		}
		else if(boxid != CheckInventory("DnD_SelectedInventoryBox")) {
			if(boxid >= STASHBUTTON_BOXID_START) {
				// because fucking ACS is stupid somehow this is 2 when evaluated normally...
				temp = -(STASHBUTTON_BOXID_START - boxid);
				if(temp < CheckInventory("DnD_PlayerInventoryPages")) {
					GiveInventory(StrParam(s:"DnD_ButtonPress_", d:temp + 1), 1);
					SetInventory("DnD_PlayerCurrentPage", temp + 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
			}
			else if(boxid != MAINBOX_NONE) {
				cpage = CheckInventory("DnD_PlayerCurrentPage") - 1;
				ppage = CheckInventory("DnD_PlayerPreviousPage") - 1;
				ssource = DND_SYNC_ITEMSOURCE_STASH | (ppage << 16);
				isource = DND_SYNC_ITEMSOURCE_STASH | (cpage << 16);
				
				// set up source and offsets
				if(boxid > MAX_INVENTORY_BOXES) {
					isource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
					ioffset = MAX_INVENTORY_BOXES;
				}
				
				if(CheckInventory("DnD_SelectedInventoryBox") > MAX_INVENTORY_BOXES) {
					ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
					soffset = MAX_INVENTORY_BOXES;
				}
				
				// if neither are empty spots
				int pnum = PlayerNumber();
				int epos, ipos;
				bool boxidon = GetItemSyncValue(DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL;
				bool prevselecton = GetItemSyncValue(DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource) != DND_ITEM_NULL;
				if(boxidon && prevselecton) {
					// if both of them point to the same pointer, we need to move this item instead
					if(isource == ssource && GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - ioffset, -1, isource) == GetItemSyncValue(DND_SYNC_ITEMTOPLEFTBOX, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource)) {
						ipos = CheckInventory("DnD_SelectedInventoryBox") - 1;
						epos = boxid - 1;
						/*
						// swap because the ipos place is changed
						temp = ssource;
						ssource = isource;
						isource = temp;
						temp = soffset;
						soffset = ioffset;
						ioffset = temp;
						*/
						if(IsFreeSpot_Trade(ipos - ioffset, epos - soffset, isource, ssource))
							MoveItemTrade(ipos - ioffset, epos - soffset, isource, ssource);
					}
					else
						SwapItems(boxid - 1 - ioffset, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, isource, ssource, false);
				}
				else {
					// find which one has an item, and move it
					if(!boxidon) {
						epos = boxid - 1;
						ipos = CheckInventory("DnD_SelectedInventoryBox") - 1;
						// swap because the ipos place is changed
						temp = ssource;
						ssource = isource;
						isource = temp;
						temp = soffset;
						soffset = ioffset;
						ioffset = temp;
					}
					else {
						epos = CheckInventory("DnD_SelectedInventoryBox") - 1;
						ipos = boxid - 1;
					}
					// epos holds the empty position now
					// make sure we aren't both empty slots
					if((boxidon || prevselecton) && IsFreeSpot_Trade(ipos - ioffset, epos - soffset, isource, ssource))
						MoveItemTrade(ipos - ioffset, epos - soffset, isource, ssource);
				}
				SetInventory("DnD_SelectedInventoryBox", 0);
				LocalAmbientSound("RPG/MenuChoose", 127);
			}
			else if(CheckInventory("DnD_SelectedInventoryBox")) {
				ppage = CheckInventory("DnD_PlayerPreviousPage") - 1;
				ssource = DND_SYNC_ITEMSOURCE_STASH | (ppage << 16);
				soffset = 0;
				
				if(CheckInventory("DnD_SelectedInventoryBox") > MAX_INVENTORY_BOXES) {
					ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
					soffset = MAX_INVENTORY_BOXES;
				}
				
				if(GetItemSyncValue(DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource) != DND_ITEM_NULL) {
					// drop selected item
					DropItemToField(PlayerNumber(), CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, true, ssource);
					ACS_NamedExecuteAlways("DnD Save Player Item Data", PlayerNumber() | (CheckInventory("DnD_CharacterID") << 16), CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, ssource);
					SetInventory("DnD_SelectedInventoryBox", 0);
					ActivatorSound("Items/Drop", 127);
				}
			}
		}
		else {
			SetInventory("DnD_SelectedInventoryBox", 0);
			SetInventory("DnD_PlayerPreviousPage", CheckInventory("DnD_PlayerCurrentPage"));
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
	}
	else if(boxid != MAINBOX_NONE && boxid < STASHBUTTON_BOXID_START) {
		cpage = CheckInventory("DnD_PlayerCurrentPage") - 1;
		ppage = CheckInventory("DnD_PlayerPreviousPage") - 1;
		ssource = DND_SYNC_ITEMSOURCE_STASH | (ppage << 16);
		isource = DND_SYNC_ITEMSOURCE_STASH | (cpage << 16);
		
		// set up source and offsets
		if(boxid > MAX_INVENTORY_BOXES) {
			isource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
			ioffset = MAX_INVENTORY_BOXES;
		}
		
		if(CheckInventory("DnD_SelectedInventoryBox") > MAX_INVENTORY_BOXES) {
			ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
			soffset = MAX_INVENTORY_BOXES;
		}
		HandleM2Inputs(boxid - ioffset, isource, soffset, ssource);
		LocalAmbientSound("RPG/MenuChoose", 127);
	}
}

void HandleMaterialDraw(menu_inventory_T& p, int boxid, int curopt, int k) {
	int mcount = CountCraftingMaterials();
	int i;
	int tx, ty, bx, by;
	int page = CheckInventory("DnD_Crafting_MaterialPage");
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	if(mcount) {
		if(!k || CheckInventory("DnD_RefreshPane")) {
			// add predefined size boxes for use
			for(i = 0; i < MAX_CRAFTING_MATERIALBOXES * (page + 1) && i < mcount - MAX_CRAFTING_MATERIALBOXES * page; ++i) {
				tx = CRAFTING_MATERIALBOX_X - (32.0 + CRAFTING_MATERIALBOX_SKIPX) * (i % 2);
				ty = CRAFTING_MATERIALBOX_Y - (32.0 + CRAFTING_MATERIALBOX_SKIPY) * (i / 2);
				bx = tx - 32.0;
				by = ty - 32.0;
				EnableBoxWithPoints(p, i + MATERIALBOX_OFFSET_BOXID, tx, ty, bx, by);
			}
		}
		i = 0;
		for(ty = 0; ty < MAX_CRAFTITEMTYPES; ++ty) {
			// by holds currently visited unique item's order, it's unique to each item type so we reset it, we don't reset i
			for(by = 0; i < MAX_CRAFTING_MATERIALBOXES && i < mcount - MAX_CRAFTING_MATERIALBOXES * page; ++i, ++by) {
				tx = GetNextUniqueCraftingMaterial(CraftItemTypes[ty], by + MAX_CRAFTING_MATERIALBOXES * page);
				if(tx != -1) {
					bx = GetTotalStackOfMaterial(tx);
					if(boxid - 1 == MATERIALBOX_OFFSET_BOXID + i) {
						//Log(s:"update item boxlit material ", d:tx);
						DrawCraftingInventoryInfo(CraftItemTypes[ty], tx, bx);
						MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK1;
						MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] |= tx << DND_MENU_ITEMSAVEBITS1;
						//SetInventory("DnD_PlayerItemIndex", tx);
						SetFont("CRFBX_H2");
					}
					else if(prevclick == MATERIALBOX_OFFSET_BOXID + i) {
						//Log(s:"update prev item in material ", d:tx);
						//SetInventory("DnD_PlayerPrevItemIndex", tx);
						MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK2;
						MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] |= tx << DND_MENU_ITEMSAVEBITS2;
						SetFont("CRFBX_H2");
					}
					else
						SetFont("CRFBX_N2");
					HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i, CR_CYAN, 404.0 + 44.0 * (i % 2), 72.0 + 36.0 * (i / 2), 0.0, 0.0);
					if(tx != -1) {
						SetFont(Item_Images[PlayerInventoryList[PlayerNumber()][tx].item_image]);
						HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i - 1, CR_CYAN, 404.0 + 44.0 * (i % 2), 72.0 + 36.0 * (i / 2), 0.0, 0.0);
						if(bx > 0) {
							SetFont("SMALLFONT");
							HudMessage(d:bx; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i - 2, CR_WHITE, 412.0 + 44.0 * (i % 2), 80.0 + 36.0 * (i / 2), 0.0, 0.0);
						}
					}
				}
				else
					break;
			}
		}
		SetFont("SMALLFONT");
		// draw next page button and enable it for use
		if(mcount > MAX_CRAFTING_MATERIALBOXES) {
			if(page) {
				// remember to send -1 of the actual box here
				EnableBoxWithPoints(p, MATERIALARROW_ID - 1, CRAFTING_LARR_X, CRAFTING_LARR_Y, CRAFTING_LARR_X - CRAFTING_ARROW_X_SKIP, CRAFTING_LARR_Y - CRAFTING_ARROW_Y_SKIP);
				DrawBoxText("<=", boxid, MATERIALARROW_ID, RPGMENUID - MATERIALARROW_HUDID - 1, 388.0, 284.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, MATERIALARROW_ID - 1);
			if(mcount - MAX_CRAFTING_MATERIALBOXES * (page + 1) > 0) {
				EnableBoxWithPoints(p, MATERIALARROW_ID, CRAFTING_RARR_X, CRAFTING_LARR_Y, CRAFTING_RARR_X - CRAFTING_ARROW_X_SKIP, CRAFTING_LARR_Y - CRAFTING_ARROW_Y_SKIP);
				DrawBoxText("=>", boxid, MATERIALARROW_ID + 1, RPGMENUID - MATERIALARROW_HUDID, 464.0, 284.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, MATERIALARROW_ID);
		}
	}
	SetFont("SMALLFONT");
}

void HandleCraftingWeaponDraw(menu_inventory_T& p, int boxid, int k) {
	int i, j = 0, mcount = GetWeaponCount();
	int tx, ty, bx, by;
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int page = CheckInventory("DnD_Crafting_ItemPage");
	if(mcount) {
		if(!k || CheckInventory("DnD_RefreshPane")) {
			// add predefined size boxes for use
			for(i = 0; i < MAX_CRAFTING_ITEMBOXES * (page + 1) && i < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
				tx = CRAFTING_ITEMBOX_X - (64.0 + CRAFTING_ITEMBOX_SKIPX) * (i % 4);
				ty = CRAFTING_ITEMBOX_Y - (64.0 + CRAFTING_ITEMBOX_SKIPY) * (i / 4);
				bx = tx - 64.0;
				by = ty - 64.0;
				EnableBoxWithPoints(p, i, tx, ty, bx, by);
			}
		}
		// first count over the weapons we must skip
		for(i = 0; i < MAXWEPS && j < MAX_CRAFTING_ITEMBOXES * page; ++i)
			if(CheckInventory(Weapons[i][WEAPON_NAME]))
				++j;
		if(j)
			++i;
		j = 0;
		// i will count onwards from here
		for(; i < MAXWEPS && j < MAX_CRAFTING_ITEMBOXES && j < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
			if(CheckInventory(Weapons[i][WEAPON_NAME])) {
				if(boxid - 1 == j) {
					//Log(s:"update item boxlit ", d:i);
					MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK1;
					MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] |= i << DND_MENU_ITEMSAVEBITS1;
					// self note: don't use the piece of shit inventories in clientside...
					//SetInventory("DnD_PlayerItemIndex", i);
					DrawCraftingInventoryInfo(DND_ITEM_WEAPON, i, 0);
					SetFont("CRFBX_H");
				}
				else if(prevclick == j) {
					//Log(s:"update prev item ", d:i);
					MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK2;
					MenuInputData[PlayerNumber()][DND_MENUINPUT_PLAYERCRAFTCLICK] |= i << DND_MENU_ITEMSAVEBITS2;
					//SetInventory("DnD_PlayerPrevItemIndex", i);
					SetFont("CRFBX_H");
				}
				else
					SetFont("CRFBX_N");
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 5 - 3 * j, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (j % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (j / 4), 0.0, 0.0);
				SetFont(Weapons[i][WEAPON_ICON64]);
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 6 - 3 * j, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (j % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (j / 4), 0.0, 0.0);
				++j;
			}
		}
		SetFont("SMALLFONT");
		// draw next page button and enable it for use
		if(mcount > MAX_CRAFTING_ITEMBOXES) {
			if(mcount - MAX_CRAFTING_ITEMBOXES * (page + 1) > 0) {
				EnableBoxWithPoints(p, CRAFTING_PAGEARROW_ID, CRAFTING_PAGEARROWR_X, CRAFTING_PAGEARROWL_Y, CRAFTING_PAGEARROWR_X - CRAFTING_PAGEARROW_XSIZE, CRAFTING_PAGEARROWL_Y - CRAFTING_PAGEARROW_YSIZE);
				DrawBoxText("=>", boxid, CRAFTING_PAGEARROW_ID + 1, RPGMENUID - 4, 340.1, 288.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, CRAFTING_PAGEARROW_ID);
		}
	}
	SetFont("SMALLFONT");
}

void HandleCraftingInventoryDraw(menu_inventory_T& p, int boxid, int k) {
	int i, j = 0, mcount = GetCraftableItemCount();
	int tx, ty, bx, by;
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int page = CheckInventory("DnD_Crafting_ItemPage");
	if(mcount) {
		if(!k || CheckInventory("DnD_RefreshPane")) {
			// add predefined size boxes for use
			for(i = 0; i < MAX_CRAFTING_ITEMBOXES * (page + 1) && i < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
				tx = CRAFTING_ITEMBOX_X - (64.0 + CRAFTING_ITEMBOX_SKIPX) * (i % 4);
				ty = CRAFTING_ITEMBOX_Y - (64.0 + CRAFTING_ITEMBOX_SKIPY) * (i / 4);
				bx = tx - 64.0;
				by = ty - 64.0;
				EnableBoxWithPoints(p, i, tx, ty, bx, by);
			}
		}
		// first count over the items we must skip
		bx = PlayerNumber();
		for(i = 0; i < MAX_INVENTORY_BOXES && j < MAX_CRAFTING_ITEMBOXES * page; ++i)
			if(IsCraftableItem(PlayerInventoryList[bx][i].item_type) && PlayerInventoryList[bx][i].height)
				++j;
		if(j)
			++i;
		j = 0;
		for(i = 0; i < MAX_CRAFTING_ITEMBOXES && i < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
			tx = GetNextUniqueCraftableMaterial(i + MAX_CRAFTING_ITEMBOXES * page);
			if(tx != -1) {
				if(boxid - 1 == j) {
					DrawCraftingInventoryInfo(PlayerInventoryList[bx][tx].item_type, tx, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
					//Log(s:"update cur item inv ", d:tx);
					MenuInputData[bx][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK1;
					MenuInputData[bx][DND_MENUINPUT_PLAYERCRAFTCLICK] |= i << DND_MENU_ITEMSAVEBITS1;
					//SetInventory("DnD_PlayerItemIndex", tx);
					SetFont("CRFBX_H");
				}
				else if(prevclick == j) {
					//Log(s:"update prev item inv ", d:tx);
					//SetInventory("DnD_PlayerPrevItemIndex", tx);
					MenuInputData[bx][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK2;
					MenuInputData[bx][DND_MENUINPUT_PLAYERCRAFTCLICK] |= i << DND_MENU_ITEMSAVEBITS2;
					SetFont("CRFBX_H");
				}
				else
					SetFont("CRFBX_N");
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 5 - 3 * j, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (j % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (j / 4), 0.0, 0.0);
				SetFont(Item_Images[PlayerInventoryList[bx][tx].item_image]);
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 6 - 3 * j, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (j % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (j / 4), 0.0, 0.0);
				++j;
			}
		}
		SetFont("SMALLFONT");
		// draw next page button and enable it for use
		if(mcount > MAX_CRAFTING_ITEMBOXES) {
			if(mcount - MAX_CRAFTING_ITEMBOXES * (page + 1) > 0) {
				EnableBoxWithPoints(p, CRAFTING_PAGEARROW_ID, CRAFTING_PAGEARROWR_X, CRAFTING_PAGEARROWL_Y, CRAFTING_PAGEARROWR_X - CRAFTING_PAGEARROW_XSIZE, CRAFTING_PAGEARROWL_Y - CRAFTING_PAGEARROW_YSIZE);
				DrawBoxText("=>", boxid, CRAFTING_PAGEARROW_ID + 1, RPGMENUID - 4, 340.1, 288.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, CRAFTING_PAGEARROW_ID);
		}
	}
	SetFont("SMALLFONT");
}

// extra1 is used for item position in inventory list
// extra1 is topboxid for items, extra2 works as source for inventory related things (used charms vs player inventory etc.)
void DrawCraftingInventoryInfo(int itype, int extra1, int extra2) {
	int offset, stack = 0;
	int mx = HUDMAX_XF - (CheckInventory("Mouse_X") & MMASK) + 16.1; 
	int my = HUDMAX_YF - (CheckInventory("Mouse_Y") & MMASK) + 16.1;
	int pn = PlayerNumber();
	SetFont("LDTITINF");
	// to force them to appear in window
	if(mx > INVENTORYINFO_NORMALVIEW_WRAPX)
		mx = INVENTORYINFO_NORMALVIEW_WRAPX + 0.1;
	if(my > INVENTORYINFO_NORMALVIEW_WRAPY)
		my = INVENTORYINFO_NORMALVIEW_WRAPY + 0.1;
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES, CR_WHITE, mx, my, 0.0, INVENTORY_INFO_ALPHA);
	if(GetStackValue(itype) && extra2) {
		SetFont("SMALLFONT");
		HudMessage(d:extra2; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 14, CR_GREEN, mx + 200.1, my + 16.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
	}
	mx += 64.0;
	my += 40.0;
	mx &= MMASK;
	my &= MMASK;
	mx += 0.4;
	my += 0.1;
	// show item details
	if(itype == DND_ITEM_ORB || itype == DND_ITEM_ELIXIR)
		offset = 16.0;
	if(itype != DND_ITEM_WEAPON)
		SetFont(Item_Images[PlayerInventoryList[pn][extra1].item_image]);
	else
		SetFont(Weapons[extra1][WEAPON_ICON64]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 1, CR_WHITE, mx + 40.0, my - 32.0 + offset, 0.0, INVENTORY_INFO_ALPHA);
	SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
	mx *= 3; mx /= 2;
	my *= 3; my /= 2;
	mx &= MMASK;
	my &= MMASK;
	mx += 0.4;
	my += 0.1;
	SetHudClipRect(-72 + (mx >> 16), -48 + (my >> 16), 256, 192, 256, 1);
	DrawCraftingInventoryText(itype, extra1, extra2, mx, my);
	SetHudClipRect(0, 0, 0, 0, 0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void DrawCraftingInventoryText(int itype, int extra1, int extra2, int mx, int my) {
	int i, j, temp, val;
	SetFont("SMALLFONT");
	if(itype == DND_ITEM_CHARM) {
		HudMessage(s:Charm_Tiers[GetItemSyncValue(DND_SYNC_ITEMLEVEL, extra1, -1, extra2) / CHARM_ATTRIBLEVEL_SEPERATOR], s: " ", s:Charm_TypeName[GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, extra1, -1, extra2)], s:" Charm"; 
			HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 2, CR_WHITE, mx + 56.0, my - 36.1, 0.0, INVENTORY_INFO_ALPHA
		);
		i = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, extra1, -1, extra2);
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, extra1, j, extra2);
			val = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, extra1, j, extra2);
			HudMessage(s:ItemAttributeString(temp, val); HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3 - j, CR_WHITE, mx + 56.0, my + 24.0 * j, 0.0, INVENTORY_INFO_ALPHA);		}
	}
	else if(itype == DND_ITEM_ORB || itype == DND_ITEM_ELIXIR) {
		temp = GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, extra1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) + GetInventoryInfoOffset(itype);
		HudMessage(s:InventoryInfo[temp][SITEM_DESC]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3, CR_WHITE, mx + 56.0, my + 24.0, INVENTORY_HOLDTIME, INVENTORY_FADETIME, INVENTORY_INFO_ALPHA);
	}
	else if(itype == DND_ITEM_WEAPON) {
		j = PlayerNumber();
		i = 0;
		HudMessage(s:"\c[R5]", s:Weapons[extra1][WEAPON_TAG], s:":\c- \c[Y5]Slot - ", s:Weapons[extra1][WEAPON_SLOT]; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3, CR_WHITE, mx + 56.0, my + 24.0 + 16.0 * i, 0.0, 0.0);
		temp = GetWeaponEnchantDisplay(j, extra1);
		// make sure quality text isn't on the weapon name
		++i;
		if(temp) {
			HudMessage(s:"\c[Y5]* Quality: \c[Q9]+", d:temp, s:"%"; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4, CR_WHITE, mx + 56.0, my + 24.0 + 16.0 * i, 0.0, 0.0);
			++i;
		}
		temp = GetCritChanceDisplay(j, extra1);
		if(temp) {
			HudMessage(s:"\c[Y5]* Crit Chance: \c[Q9]", f:temp, s:"%"; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 5, CR_WHITE, mx + 56.0, my + 24.0 + 16.0 * i, 0.0, 0.0);
			++i;
		}
		if(HasCritDamageBonus(j, extra1)) {
			HudMessage(s:"\c[Y5]* Crit Damage: \c[Q9]", f:GetCritDamageDisplay(j, extra1), s:"%"; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 6, CR_WHITE, mx + 56.0, my + 24.0 + 16.0 * i, 0.0, 0.0);
			++i;
		}
		temp = GetBonusDamageDisplay(j, extra1);
		if(temp) {
			if(temp > 0)
				HudMessage(s:"\c[Y5]* Damage Bonus: \c[Q9]", f:temp, s:"%"; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 7, CR_WHITE, mx + 56.0, my + 24.0 + 16.0 * i, 0.0, 0.0);
			else
				HudMessage(s:"\c[Y5]* Damage Bonus: \c[Q2]", f:temp, s:"%"; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 8, CR_WHITE, mx + 56.0, my + 24.0 + 16.0 * i, 0.0, 0.0);
			++i;
		}
	}
	else if(itype > UNIQUE_BEGIN) {
		temp = itype & 0xFFFF;
		itype >>= UNIQUE_BITS;
		--itype;
		// itype holds unique position, temp is the actual item type
		HudMessage(s:"\c[A1]", s:UniqueItemNames[itype]; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 2, CR_WHITE, mx + 56.0, my - 36.1, 0.0, INVENTORY_INFO_ALPHA);
		HudMessage(s:"\c[D1]Unique ", s:Charm_TypeName[GetItemSyncValue(DND_SYNC_ITEMSUBTYPE, extra1, -1, extra2)], s:" Charm"; HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3, CR_WHITE, mx + 56.0, my - 20.1, 0.0, INVENTORY_INFO_ALPHA);
		i = GetItemSyncValue(DND_SYNC_ITEMSATTRIBCOUNT, extra1, -1, extra2);
		// itype will count the skipped properties (the helper attributes)
		itype = 0;
		for(j = 0; j < i; ++j) {
			temp = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, extra1, j, extra2);
			val = GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, extra1, j, extra2);
			if(val > 0) {
				// dont show this, skip to next attribute's detail
				if(temp == INV_EX_CHANCE) {
					++j;
					++itype;
					HudMessage(s:ExoticAttributeString(GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_ID, extra1, j, extra2), val, GetItemSyncValue(DND_SYNC_ITEMATTRIBUTES_VAL, extra1, j, extra2)); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
				}
				else
					HudMessage(s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
			}
			else if(!val) {
				// unique item doesn't have numeric attribute to show
				HudMessage(s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
			}
			else
				HudMessage(s:"- ", s:ExoticAttributeString(temp, val, 0); HUDMSG_PLAIN | HUDMSG_FADEOUT, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 -  (j - itype), CR_WHITE, mx + 56.0, my + 24.0 * (j - itype), 0.0, INVENTORY_INFO_ALPHA);
		}
	}
}

void HandleCraftingView(menu_inventory_T& p, int boxid, int curopt, int k) {
	int i;
	HudMessage(s:"\c[Y5]Materials"; HUDMSG_PLAIN, RPGMENUID - 2, CR_CYAN, 424.0, 32.0, 0.0, 0.0);
	
	// load rectangles
	if(!k || CheckInventory("DnD_RefreshPane")) {
		ResetInventoryPane(p);
		LoadCraftingView(p);
	}
	
	CleanInventoryInfo();
	HandleMaterialDraw(p, boxid, curopt, k);
	if(curopt == MENU_LOAD_CRAFTING) {
		DrawBoxText("Weapons Crafting", boxid, CRAFTING_WEAPON_BOXID + 1, RPGMENUID - 3, 24.1, 32.0, "\c[B1]", "\c[Y5]");
		DrawBoxText("Inventory Crafting", boxid, CRAFTING_INVENTORY_BOXID + 1, RPGMENUID - 4, 24.1, 48.0, "\c[B1]", "\c[Y5]");
	}
	else if(curopt == MENU_LOAD_CRAFTING_WEAPON) {
		if(!k || CheckInventory("DnD_RefreshPane"))
			EnableBoxWithPoints(p, CRAFTING_PAGEARROW_ID - 1, CRAFTING_PAGEARROWL_X, CRAFTING_PAGEARROWL_Y, CRAFTING_PAGEARROWL_X - CRAFTING_PAGEARROW_XSIZE, CRAFTING_PAGEARROWL_Y - CRAFTING_PAGEARROW_YSIZE);
		DrawBoxText("<=", boxid, CRAFTING_PAGEARROW_ID, RPGMENUID - 3, 16.1, 288.0, "\c[B1]", "\c[Y5]");
		HandleCraftingWeaponDraw(p, boxid, k);
	}
	else if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
		if(!k || CheckInventory("DnD_RefreshPane"))
			EnableBoxWithPoints(p, CRAFTING_PAGEARROW_ID - 1, CRAFTING_PAGEARROWL_X, CRAFTING_PAGEARROWL_Y, CRAFTING_PAGEARROWL_X - CRAFTING_PAGEARROW_XSIZE, CRAFTING_PAGEARROWL_Y - CRAFTING_PAGEARROW_YSIZE);
		DrawBoxText("<=", boxid, CRAFTING_PAGEARROW_ID, RPGMENUID - 3, 16.1, 288.0, "\c[B1]", "\c[Y5]");
		HandleCraftingInventoryDraw(p, boxid, k);
	}
	SetFont("SMALLFONT");
}

void HandleCraftingInputs(int boxid, int curopt) {
	int pnum = PlayerNumber();
	int prevselect;
	if(HasPlayerClicked(pnum)) {
		int itemindex = ((boxid >> DND_MENU_ITEMSAVEBITS1) & DND_MENU_ITEMSAVEBITS1_MASK);
		int previtemindex = (boxid >> DND_MENU_ITEMSAVEBITS2);
		boxid = (boxid & DND_MENU_ITEMSAVEBITS1_MASK);
		if(boxid != MAINBOX_NONE && boxid != CheckInventory("DnD_SelectedInventoryBox")) {
			if(HasLeftClicked(pnum)) {
				// arrows in material part, left and right respectively
				if(boxid == MATERIALARROW_ID) {
					TakeInventory("DnD_Crafting_MaterialPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				if(boxid == MATERIALARROW_ID + 1) {
					GiveInventory("DnD_Crafting_MaterialPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				
				// marking a material in case they need something to be used on
				if(boxid > MATERIALBOX_OFFSET_BOXID && boxid <= MATERIALBOX_OFFSET_BOXID + MAX_CRAFTING_MATERIALBOXES) {
					SetInventory("DnD_SelectedInventoryBox", boxid);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				
				// bottom arrows for crafting
				if(boxid == CRAFTING_PAGEARROW_ID) {
					if(!CheckInventory("DnD_Crafting_ItemPage")) {
						if(curopt == MENU_LOAD_CRAFTING_WEAPON || curopt == MENU_LOAD_CRAFTING_INVENTORY)
							UpdateMenuPosition(MENU_LOAD_CRAFTING);
					}
					else {
						TakeInventory("DnD_Crafting_ItemPage", 1);
						GiveInventory("DnD_RefreshPane", 1);
						LocalAmbientSound("RPG/MenuChoose", 127);
					}
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
				if(boxid == CRAFTING_PAGEARROW_ID + 1) {
					GiveInventory("DnD_Crafting_ItemPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					SetInventory("DnD_SelectedInventoryBox", 0);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				
				
				if(curopt == MENU_LOAD_CRAFTING) {
					if(boxid == CRAFTING_WEAPON_BOXID + 1)
						UpdateMenuPosition(MENU_LOAD_CRAFTING_WEAPON);
					else if(boxid == CRAFTING_INVENTORY_BOXID + 1)
						UpdateMenuPosition(MENU_LOAD_CRAFTING_INVENTORY);
				}
				else if(curopt == MENU_LOAD_CRAFTING_WEAPON) {
					if(boxid > 0 && boxid <= MAX_CRAFTING_ITEMBOXES) {
						SetInventory("DnD_SelectedInventoryBox", boxid);
						LocalAmbientSound("RPG/MenuChoose", 127);
					}
				}
				else if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
					if(boxid > 0 && boxid <= MAX_CRAFTING_ITEMBOXES) {
						SetInventory("DnD_SelectedInventoryBox", boxid);
						LocalAmbientSound("RPG/MenuChoose", 127);
					}
				}
			}
			else if(HasRightClicked(pnum)) {
				// using an orb in material part
				if(boxid > MATERIALBOX_OFFSET_BOXID && boxid <= MATERIALBOX_OFFSET_BOXID + MAX_CRAFTING_MATERIALBOXES) {
					if(IsSelfUsableItem(PlayerInventoryList[pnum][itemindex].item_type, PlayerInventoryList[pnum][itemindex].item_subtype)) {
						if(HandleMaterialUse(pnum, itemindex, 0, DND_ITEM_NULL)) {
							GiveInventory("DnD_RefreshPane", 1);
							UsePlayerItem(pnum, itemindex);
						}
						else
							ShowPopup(POPUP_MATERIALCANTUSE, false, 0);
					}
					else if(!CheckInventory("DnD_SelectedInventoryBox"))
						ShowPopup(POPUP_ITEMNEEDSTARGET, false, 0);
					else {
						// figure out which type of item this is
						// sort out selected box from boxid etc., we need boxid to be on material
						prevselect = CheckInventory("DnD_SelectedInventoryBox") - 1;
						if(prevselect >= 0 && prevselect < MAX_CRAFTING_ITEMBOXES) {
							if(curopt == MENU_LOAD_CRAFTING_WEAPON) {
								if(HandleMaterialUse(pnum, itemindex, previtemindex, DND_ITEM_WEAPON)) {
									//Log(d:DND_WEAPON_MFG, s: " vs ", d:previtemindex);
									GiveInventory("DnD_RefreshPane", 1);
									UsePlayerItem(pnum, itemindex);
								}
								else
									ShowPopup(POPUP_MATERIALCANTUSE, false, 0);
							}
							else if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
								if(HandleMaterialUse(pnum, itemindex, previtemindex, DND_ITEM_CHARM)) {
									GiveInventory("DnD_RefreshPane", 1);
									UsePlayerItem(pnum, itemindex);
								}
								else
									ShowPopup(POPUP_MATERIALCANTUSE, false, 0);
							}
						}
					}
				}
			}
		}
		else
			SetInventory("DnD_SelectedInventoryBox", 0);
		ClearPlayerInput(pnum, true);
	}
}

// returns success of use of item
bool HandleMaterialUse(int pnum, int itemindex, int target, int targettype) {
	bool res = false;
	int itype = PlayerInventoryList[pnum][itemindex].item_type;
	int isubtype = PlayerInventoryList[pnum][itemindex].item_subtype;
	// self usable materials usually go here as they have targettype NULL
	if(!IsCraftableItem(targettype)) {
		if(itype == DND_ITEM_ORB) {
			res = CanUseOrb(isubtype, 0, 0);
			if(res) {
				ACS_NamedExecuteAlways("DND Orb Use", 0, isubtype, 0);
			}
		}
		else if(itype == DND_ITEM_ELIXIR) {
			res = true;
			ACS_NamedExecuteAlways("DnD Elixir Use", 0, isubtype);
		}
	}
	else if(itype == DND_ITEM_ORB) {
		// targettype is a craftable item
		res = CanUseOrb(isubtype, target, targettype);
		if(res) {
			ACS_NamedExecuteAlways("DND Orb Use", 0, isubtype, target);
		}
	}
	
	return res;
}

bool HasPlayerClicked(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_LCLICK || MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_RCLICK;
}

bool HasLeftClicked(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_LCLICK;
}

bool HasRightClicked(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_RCLICK;
}

bool HasPressedLeft(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_PREVBUTTON;
}

bool HasPressedRight(int pnum) {
	return MenuInputData[pnum][DND_MENUINPUT] == DND_MENUINPUT_NEXTBUTTON;
}

void ClearPlayerInput(int pnum, bool cleanLR) {
	MenuInputData[pnum][DND_MENUINPUT] = 0;
	if(cleanLR)
		MenuInputData[pnum][DND_MENUINPUT_LRPOS] = 0;
}

void GetInputOnMenuPage(int opt) {
	if(opt == MENU_MAIN)
		ListenInput(0, 0, 0);
	else if(opt == MENU_RESEARCH || opt == MENU_RESEARCH_GUNS)
		ListenInput(LISTEN_LEFT | LISTEN_RIGHT, 0, 0);
	else if(opt >= SHOP_RESPAGE_BEGIN && opt <= SHOP_RESPAGE_END) {
		if(ResearchInfo[opt - SHOP_RESPAGE_BEGIN][CheckInventory("MenuPosX") + 1].res_id != -1)
			ListenInput(LISTEN_LEFT | LISTEN_RIGHT | LISTEN_FASTLR, 0, MAX_RESEARCHES - 1);
		else
			ListenInput(LISTEN_LEFT | LISTEN_FASTLR, 0, MAX_RESEARCHES - 1);
	}
	else
		ListenInput(LISTEN_LEFT | LISTEN_RIGHT, 0, 0);
}