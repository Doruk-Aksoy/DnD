#include "DnD_MenuTables.h"

Script "DnD Refresh Request" (int pnum, int state) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum)
		Terminate;
	
	Delay(const:1);
	GiveInventory("DnD_RefreshPane", 1);
}

void SetPage(int option, bool useSound) {
	SetInventory("MenuUD", 0);
	SetInventory("MenuOption", option);
	if(useSound)
		LocalAmbientSound("RPG/MenuChoose", 127);
}

void UpdateMenuPosition(int option) {
	if(CheckInventory("MenuOption") != option) {
		menu_stack_T module& stack = GetMenuStack(PlayerNumber());
		PushStack(PlayerNumber(), option);
	}
	GiveInventory("DnD_HighlightBlocker", 1);
	SetPage(option, true);
}

void UpdateMenuPosition_NoSound(int option) {
	if(CheckInventory("MenuOption") != option) {
		menu_stack_T module& stack = GetMenuStack(PlayerNumber());
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

void ClearTempItemInventory() {
	SetInventory("DnD_ItemSelectTemp", 0);
	SetInventory("DnD_ItemCursorMsg", 0);
}

// includes left right shortcuts
// works clientside
void ListenInput(int listenflag, int condx_min, int condx_max, int curr_res_page = -1) {
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS);
	int obpress = GetPlayerInput(-1, INPUT_OLDBUTTONS);
	int curposx = CheckInventory("MenuPosX");
	bool p = 0;
	// if waiting for sell confirmation do not let movement in menu
	if(!CheckInventory("DnD_SellConfirm")) {
		if(listenflag & LISTEN_LEFT) {
			if(!(listenflag & LISTEN_FASTLR))
				p = IsButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_LEFT));
			else
				p = IsButtonHeld(bpress, GetMovementButton(MVMT_BT_LEFT)) && !CheckInventory("Menu_LRCooldown");
			if(p) {
				if(curposx > condx_min) {
					if(!(listenflag & LISTEN_SKIPKNOWNRES))
						TakeInventory("MenuPosX", 1);
					else {
						// skip as far ahead as we are able
						do {
							--curposx;
						} while(curposx > 0 && ResearchInfo[curr_res_page][curposx].res_id != -1 && CheckResearchStatus(ResearchInfo[curr_res_page][curposx].res_id) == RES_DONE);
						
						if(CheckResearchStatus(ResearchInfo[curr_res_page][curposx].res_id) != RES_DONE)
							SetInventory("MenuPosX", curposx);
					}
					LocalAmbientSound("RPG/MenuMove", 127);
					GiveInventory("Menu_LRCooldown", 1);
				}
				SetInventory("MenuInput", DND_MENUINPUT_PREVBUTTON);
				
				// reset these in page transitions
				if(PlayerCursorData.itemDragged != -1)
					ResetCursorDragData();
				if(PlayerCursorData.itemHovered != -1)
					ResetCursorHoverData();
			}
		}
		if(listenflag & LISTEN_RIGHT) {
			if(!(listenflag & LISTEN_FASTLR))
				p = IsButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_RIGHT));
			else
				p = IsButtonHeld(bpress, GetMovementButton(MVMT_BT_RIGHT)) && !CheckInventory("Menu_LRCooldown");
			if(p) {
				if(curposx + 1 < condx_max && ResearchInfo[curr_res_page][curposx + 1].res_id != -1) {
					if(!(listenflag & LISTEN_SKIPKNOWNRES))
						GiveInventory("MenuPosX", 1);
					else {
						// skip as far ahead as we are able
						do {
							++curposx;
						} while(ResearchInfo[curr_res_page][curposx].res_id != -1 && curposx < condx_max && CheckResearchStatus(ResearchInfo[curr_res_page][curposx].res_id) == RES_DONE);
						// no change if we can't move further
						if(ResearchInfo[curr_res_page][curposx].res_id != -1)
							SetInventory("MenuPosX", curposx);
					}
					LocalAmbientSound("RPG/MenuMove", 127);
					GiveInventory("Menu_LRCooldown", 1);
				}
				SetInventory("MenuInput", DND_MENUINPUT_NEXTBUTTON);
				
				// reset these in page transitions
				if(PlayerCursorData.itemDragged != -1)
					ResetCursorDragData();
				if(PlayerCursorData.itemHovered != -1)
					ResetCursorHoverData();
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

void ResetCursorDragData() {
	PlayerCursorData.itemDragged = -1;
	PlayerCursorData.itemDragInfo.topboxid = -1;
	PlayerCursorData.itemDragInfo.source = -1;
	PlayerCursorData.itemDragInfo.click_box = -1;
}

void ResetCursorHoverProc() {
	PlayerCursorData.itemHovered = -1;
	PlayerCursorData.itemHoveredType = 0;
	PlayerCursorData.itemHoveredSource = 0;
	PlayerCursorData.itemHoveredDim.x = 0;
	PlayerCursorData.itemHoveredDim.y = 0;
	PlayerCursorData.itemHoveredOffset = 0;
	PlayerCursorData.owner_pnum = -1;
	PlayerCursorData.hoverNeedsReset = false;
}

void ResetCursorHoverData() {
	// cleanup anything that is potentially shown by cursor hovering here
	CleanInventoryInfo();
	ResetCursorHoverProc();
}

void UpdateCursorHoverData(int itemid, int source, int itemtype, int owner_p, int offset, int dimx = 0, int dimy = 0) {
	if(PlayerCursorData.itemHovered != itemid)
		CleanInventoryInfo();
	
	PlayerCursorData.itemHovered = itemid;
	PlayerCursorData.itemHoveredSource = source;
	PlayerCursorData.itemHoveredType = itemtype;
	PlayerCursorData.itemHoveredDim.x = dimx;
	PlayerCursorData.itemHoveredDim.y = dimy;
	PlayerCursorData.itemHoveredOffset = offset;
	PlayerCursorData.owner_pnum = owner_p;
}

void resetCursorClickProc() {
	PlayerCursorData.itemClicked = -1;
	PlayerCursorData.itemClickedPos.x = -1;
	PlayerCursorData.itemClickedPos.y = -1;
	PlayerCursorData.clickNeedsReset = false;
}

void ResetCursorClickData() {
	CleanInventoryInfo(RPGMENUCLICKEDID);
	
	TakeInventory("DnD_CursorDataClearRequest", 1);

	resetCursorClickProc();
}

void UpdateCursorClickData(int itemid, int source, int itemtype, int clickx = -1, int clicky = -1) {
	if(PlayerCursorData.itemClicked != itemid) {
		CleanInventoryInfo(RPGMENUCLICKEDID);

		PlayerCursorData.itemClickedPos.x = clickx;
		PlayerCursorData.itemClickedPos.y = clicky;
		PlayerCursorData.itemClicked = itemid;
		PlayerCursorData.itemClickedSource = source;
		PlayerCursorData.itemClickedType = itemtype;
	}
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
		return ArtifactDrawInfo[itemid - SHOP_FIRSTARTI1_INDEX].flags;
		case TYPE_ABILITY:
		return AbilityDrawInfo[itemid - SHOP_ABILITY1_BEGIN].flags;
		case TYPE_ACCOUNT:
		return OBJ_RESEARCH;
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

void ShowNeedResearchPopup() {
	ShowPopup(POPUP_NEEDRESEARCH, false, 0);
}

str GetPerkText(int id) {
	return StrParam(s:"DND_MENU_PERKTEXT", d:id + 1);
}

// yeah this is ugly, but it is what it is
void DrawPerkText(int boxid) {
	if(boxid != MAINBOX_NONE) {
		// order follows perk definition order at the enum in DND_CommonStat.h
		int perk = boxid + DND_PERK_BEGIN - 1;
		str toShow = "";
		switch(perk) {
			case STAT_SHRP:
				toShow = StrParam(s:"\cd* \ci+", d:DND_PERK_SHARPSHOOTER_INC, s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_BRUT:
				toShow = StrParam(s:"\cd* \ci+", d:DND_PERK_BRUTALITY_DAMAGEINC, s:"%\c- ", l:GetPerkText(boxid - 1), l:"DND_MENU_AND", s:"\ci+", d:DND_PERK_BRUTALITY_RANGEINC, s:"%\c- ", l:"DND_MENU_PERKTEXT2_CONT");
			break;
			case STAT_RISK:
				toShow = StrParam(s:"\cd* \ci+", d:RISK_AVERSION_VALUE, s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_WIS:
				toShow = StrParam(s:"\cd* \ci+", d:BASE_WISDOM_GAIN, s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_GRE:
				toShow = StrParam(s:"\cd* \ci+", d:BASE_GREED_GAIN, s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_MED:
				toShow = StrParam(s:"\cd* \ci+", d:PERK_MEDICBONUS, s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_MUN:
				toShow = StrParam(s:"\cd* \ci+", d:DND_MUNITION_GAIN, s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_DED:
				toShow = StrParam(s:"\cd* \ci+", s:GetFixedRepresentation(PERK_DEADLINESS_BONUS, true), s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_SAV:
				toShow = StrParam(s:"\cd* \ci+", d:DND_SAVAGERY_BONUS, s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
			case STAT_LUCK:
				toShow = StrParam(s:"\cd* \ci+", s:GetFixedRepresentation(DND_LUCK_GAIN, true), s:"%\c- ", l:GetPerkText(boxid - 1));
			break;
		}
		if(GetPerk(perk) == DND_PERK_MAX)
			toShow = StrParam(s:toShow, s:"\n\c[Y5]", l:"DND_MENU_MASTERY", s:": \cd", l:StrParam(s:"DND_MENU_PERKMASTERY", d:boxid));
		else
			toShow = StrParam(s:toShow, s:"\n\c[Y5]", l:"DND_MENU_MASTERY", s:" (", l:"DND_MENU_MASTERY_COND", s:"): \cu", l:StrParam(s:"DND_MENU_PERKMASTERY", d:boxid));
		SetHudClipRect(184, 232, 256, 48, 256);
		HudMessage(s:toShow; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 184.1, 232.1 + 2.0 * ScrollPos.x, 0.0, 0.0);
		SetHudClipRect(0, 0, 0, 0, 0);
	}
}

void DrawDamageTypes(int req_id, int constraint, int flags) {
	DeleteTextRange(RPGMENUDAMAGETYPEID, RPGMENUDAMAGETYPEID + MAX_DAMAGE_TYPES - 1);
	if(req_id == -1 || CheckItemRequirements(req_id, constraint, flags)) {
		int j = 0;
		for(int i = 0; i < MAX_DAMAGE_TYPES; ++i) {
			if(IsSet(WeaponDamageTypes[req_id], i)) {
				SetFont(GetDamageTypeIcon(i));
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUDAMAGETYPEID + j, CR_CYAN, 62.1 + j * 30.0 - 60.0 * (j > 1), 84.1 - 32.0 * (j > 1), 0.0, 0.0);
				++j;
			}
		}
		SetFont("NMENUFNT");
	}
}

/*bool HasCritDamageBonus(int pnum, int wep) {
	return Player_Weapon_Infos[pnum][wep].wep_mods[WEP_MOD_CRITDMG].val;
}

int GetCritDamageDisplay(int pnum, int wep) {
	int base = 100.0 + 100 * (Player_Weapon_Infos[pnum][wep].wep_mods[WEP_MOD_CRITDMG].val);
	return base;
}*/

int GetDamageTypeBonus(int pnum, int dtype, int flags) {
	return MapDamageCategoryToPercentBonus(pnum, dtype, flags);
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
		case MENU_SHOP_WEAPON3_3:
		return SHOP_WEAPON33_BEGIN;
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
	return SHOP_WEAPON1_BEGIN;
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
		case MENU_SHOP_WEAPON3_3:
		return SHOP_WEAPON_SLOT33END;
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
	return SHOP_WEAPON_SLOT1END;
}

str GetWeaponToTake(int wepid) {
	int real_id = ShopTableIdToWeaponTableId(wepid);
	int slot = GetGameSlotOfWeapon(real_id);
	if(slot != 8 && !IsLuxuryWeapon(real_id)) {
		if(wepid >= SHOP_WEAPON1CSAW_BEGIN && wepid <= SHOP_WEAPON1CSAW_END)
			return Weapons_Data[DND_WEAPON_CHAINSAW].name;
		else if(wepid >= SHOP_WEAPON2PISTOL_BEGIN && wepid <= SHOP_WEAPON2PISTOL_END)
			return Weapons_Data[DND_WEAPON_PISTOL].name;
		else if(wepid >= SHOP_WEAPON3SG_BEGIN && wepid <= SHOP_WEAPON3SG_END) {
			// little hack here to fix special ammos after refunding weapon
			SetInventory("SpecialAmmoMode_3", AMMO_BASICSHELL);
			return Weapons_Data[DND_WEAPON_SHOTGUN].name;
		}
		else if(wepid >= SHOP_WEAPON3SSG_BEGIN && wepid <= SHOP_WEAPON3SSG_END)
			return Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name;
		else if(wepid >= SHOP_WEAPON4CG_BEGIN && wepid <= SHOP_WEAPON4CG_END) {
			SetInventory("SpecialAmmoMode_4", AMMO_BULLET);
			return Weapons_Data[DND_WEAPON_MACHINEGUN].name;
		}
		else if(wepid >= SHOP_WEAPON5RL_BEGIN && wepid <= SHOP_WEAPON5RL_END)
			return Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].name;
		else if(wepid >= SHOP_WEAPON6PL_BEGIN && wepid <= SHOP_WEAPON6PL_END)
			return Weapons_Data[DND_WEAPON_PLASMARIFLE].name;
		else if(wepid >= SHOP_WEAPON7BFG_BEGIN && wepid <= SHOP_WEAPON7BFG_END)
			return Weapons_Data[DND_WEAPON_BFG6000].name;
	}
	return "";
}

void HandleWeaponPropertyImages(int curopt, int boxid, int ypos) {
	int wid = GetWeaponBeginIndexFromOption(curopt);
	int imgcount = 0;
	wid += boxid - 1;
	int real_wid = ShopTableIdToWeaponTableId(wid);
	
	DeleteTextRange(RPGMENUWEAPONPANELID - MAX_WEAPON_PROPERTIES, RPGMENUWEAPONPANELID - 1);
	if(CheckItemRequirements(wid, RES_KNOWN, WeaponDrawInfo[wid - SHOP_WEAPON_BEGIN].flags)) {
		SetHudSize(HUDMAX_X * 2, HUDMAX_Y * 2, 1);
		for(int i = 0; i < MAX_WEAPON_PROPERTIES; ++i) {
			if(IsSet(Weapons_Data[real_wid].properties, i)) {
				SetFont(WeaponPropertyImages[i]);
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUWEAPONPANELID - i - 1, -1, -64.0 + (imgcount & 1) * 64.0, -160.0 + ypos * 9.0 + (imgcount / 2) * 40.0, 0.0, 0.0);
				++imgcount;
			}
		}
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	}
}

bool HandlePageListening(int curopt, int boxid) {
	bool redraw = false;
	int temp, i;
	switch(curopt) {
		case MENU_MAIN:
		case MENU_HELP_RESEARCHES:
		case MENU_HELP_AILMENTS:
			redraw = ListenScroll(-32, 0);
		break;
		case MENU_PERK:
			redraw = ListenScroll(-16, 0);
		break;

		case MENU_SHOP_AMMO_SPECIAL1:
			redraw = ListenScroll(-24, 0);
		break;

		case MENU_STAT2_DEFENSE:
			redraw = ListenScroll(ScrollPos.y - 8, 0);
		break;
		case MENU_STAT2_OFFENSE1:
		case MENU_STAT2_OFFENSE2:
		case MENU_STAT2_UTILITY:
			redraw = ListenScroll(ScrollPos.y, 0);
		break;
		case MENU_HELP_MMODS_IMMUNITY:
			redraw = ListenScroll(-144, 0);
		break;
		case MENU_HELP_CHARACTER:
		case MENU_HELP_MMODS_RESIST:
			redraw = ListenScroll(-96, 0);
		break;
		case MENU_HELP_DAMAGETYPES:
			redraw = ListenScroll(-128, 0);
		break;
		case MENU_HELP_MMODS_UTILITY:
			redraw = ListenScroll(-208, 0);
		break;
		case MENU_HELP_LEGMONS:
			temp = 0;
			for(i = 0; i < MAX_LEGMONS_TEXT; ++i) {
				if(HasKilledLegendary(i))
					++temp;
			}
			redraw = ListenScroll(-64 * (temp / 2), 0);
		break;
		case MENU_HELP_MMODS_WEAKNESS:
			redraw = ListenScroll(-48, 0);
		break;
		case MENU_HELP_WEAPONPROPS:
			redraw = ListenScroll(-256, 0);
		break;
		case MENU_HELP_MMODS_AGGRESSIVE:
			redraw = ListenScroll(-176, 0);
		break;
		case MENU_HELP_ORBS:
			redraw = ListenScroll(-320, 0);
		break;
		case MENU_HELP_MMODS_DEFENSIVE:
			redraw = ListenScroll(-272, 0);
		break;
		
		// weapon pages -- this part is ugly I know, at this time I couldnt find a better solution :P
		case MENU_SHOP_WEAPON1:
		case MENU_SHOP_WEAPON3_1:
		case MENU_SHOP_WEAPON3_2:
		case MENU_SHOP_WEAPON4_2:
			if(boxid != -1 && (WeaponDrawInfo[GetWeaponBeginIndexFromOption(curopt) + boxid - 1].flags & OBJ_USESCROLL))
				redraw = ListenScroll(-36, 0);
		break;
		case MENU_SHOP_WEAPON5_1:
			if(boxid != -1 && (WeaponDrawInfo[GetWeaponBeginIndexFromOption(curopt) + boxid - 1].flags & OBJ_USESCROLL))
				redraw = ListenScroll(-40, 0);
		break;
		case MENU_SHOP_WEAPON2:
		case MENU_SHOP_WEAPON6_2:
		case MENU_SHOP_WEAPON7:
			if(boxid != -1 && (WeaponDrawInfo[GetWeaponBeginIndexFromOption(curopt) + boxid - 1].flags & OBJ_USESCROLL))
				redraw = ListenScroll(-16, 0);
		break;
		case MENU_SHOP_WEAPON4_1:
		case MENU_SHOP_WEAPON5_2:
			if(boxid != -1 && (WeaponDrawInfo[GetWeaponBeginIndexFromOption(curopt) + boxid - 1].flags & OBJ_USESCROLL))
				redraw = ListenScroll(-24, 0);
		break;
		
		case MENU_SHOP_WEAPON6_1:
			if(boxid != -1 && (WeaponDrawInfo[GetWeaponBeginIndexFromOption(curopt) + boxid - 1].flags & OBJ_USESCROLL))
				redraw = ListenScroll(-40, 0);
		break;
		case MENU_RESEARCH_BODY:
		case MENU_RESEARCH_AMMO:
		case MENU_RESEARCH_SLOTGUNS:
		case MENU_RESEARCH_LUXURYGUNS:
		case MENU_RESEARCH_UTILITY:
			if(boxid == MBOX_2)
				redraw = ListenScroll(-16, 0);
		break;
		#ifdef ISAPRILFIRST
		case MENU_SHOP_NFT:
			redraw = ListenScroll(-48, 0);
		break;
		#endif
	}
	return redraw;
}

void DrawAmmoIconCorner(int slot, int boxid, int ammoindex, bool isSpecial) {
	str toshow = "";
	
	if(boxid != MAINBOX_NONE) {
		if(isSpecial) {
			ammoindex = MAXSHOPNORMALAMMOS + boxid - 1;
			toshow = GetTextWithResearch(SpecialAmmoInfo_Str[boxid - 1][AMMOINFO_ICON], "", AmmoDrawInfo[ammoindex].res_id, RES_KNOWN, AmmoDrawInfo[ammoindex].flags);
		}
		else {
			int shopindex = MenuAmmoIndexMap[slot][ammoindex] - SHOP_FIRSTAMMO_INDEX;
			toshow = GetTextWithResearch(AmmoInfo[slot][ammoindex].icon, "", AmmoDrawInfo[shopindex].res_id, RES_KNOWN, AmmoDrawInfo[shopindex].flags);
		}
	}

	if(StrCmp(toshow, "")) {
		SetFont(toshow);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUHELPCORNERID, CR_CYAN, 108.4, 84.0, 0.0, 0.0);
		SetFont("NMENUFNT");
	}
	else
		DeleteText(RPGMENUHELPCORNERID);
}

// the itemid as is works here because the first item list on menu is weapons in enum at dnd_menuconstants.h
void DrawWeaponIconCorner(int itemid) {
	str toshow = GetTextWithResearch(GetWeaponShopIcon(itemid), "", WeaponDrawInfo[itemid].res_id, RES_KNOWN, WeaponDrawInfo[itemid].flags);

	if(StrCmp(toshow, "")) {
		SetFont(toshow);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUHELPCORNERID, CR_CYAN, 96.4, 48.0, 0.0, 0.0);
		SetFont("NMENUFNT");
		
		// draw damage types as well
		DrawDamageTypes(itemid, RES_KNOWN, WeaponDrawInfo[itemid].flags);
	}
}

void DrawArtifactIconCorner(int boxid) {
	str toshow = GetTextWithResearch(ArtifactInfo[boxid][ARTI_ICON], "", ArtifactDrawInfo[boxid].res_id, RES_KNOWN, ArtifactDrawInfo[boxid].flags);

	if(StrCmp(toshow, "")) {
		SetFont(toshow);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUHELPCORNERID, CR_CYAN, 92.1, 56.1, 0.0, 0.0);
		SetFont("NMENUFNT");
	}
	else
		DeleteText(RPGMENUHELPCORNERID);
}

// curopt is the pageid
void HandleItemInfoPanel(int curopt, int boxid, bool redraw) {
	static int ypos = 0;
	bool isWepPage = IsWeaponPage(curopt);

	// pull down = 1
	if(isWepPage) {
		if(ypos < 22)
			++ypos;
	}
	else if(ypos > 0)
		--ypos;

	SetFont("DND_PANL");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUWEAPONPANELID, -1, -12.0, -64.0 + ypos * 5.0, 0.0, 0.0);
	
	if(curopt >= MENU_SHOP && curopt <= SHOP_LASTWEAPON_PAGE) {
		if(isWepPage && boxid != MAINBOX_NONE && boxid <= MAX_WEPS_PER_PAGE)
			HandleWeaponPropertyImages(curopt, boxid, ypos);
		else if(redraw)
			DeleteTextRange(RPGMENUWEAPONPANELID - MAX_WEAPON_PROPERTIES, RPGMENUWEAPONPANELID - 1);
	}
	
	SetFont("NMENUFNT");
}

void ClearInfoPanel() {
	DeleteText(RPGMENUHELPCORNERID);
	DeleteTextRange(RPGMENUDAMAGETYPEID, RPGMENUDAMAGETYPEID + MAX_DAMAGE_TYPES - 1);
}

void ShowWeaponIcon(int wep, int i, int k) {
	SetHudSize(640, 480, 1);
	SetFont(Weapons_Data[wep].icon);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - DND_MENU_LOADOUTWEPITEMS * i - 5, CR_WHITE, 280.1, 126.1 + 72.0 * i + 24.0 * k + 6.0 * ScrollPos.x, 0.0, 0.0);
	SetFont("NMENUFNT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void ShowDamageTypeIcon(int dmg) {
	SetHudSize(640, 480, 1);
	SetFont(GetDamageTypeIcon(dmg));
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_DAMAGE_TYPES - 1 - dmg, CR_WHITE, 384.1, 88.1 + 120.0 * dmg + 6.0 * ScrollPos.x, 0.0, 0.0);
	SetFont("NMENUFNT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void ShowWeaponPropertyIcon(int id) {
	int offset = 0.0;
	SetFont(WeaponPropertyImages[id]);
	
	if(id == 2)
		offset = -24.0;
	else if(id == 3 || id == 7)
		offset = -16.0;
	else if(id == 6 || id == 9)
		offset = -8.0;
	else if(id == 11)
		offset = -24.0;
	else if(id == 14 || id == 16)
		offset = -20.0;
	else if(id == 15)
		offset = -36.0;
	else if(id > 11)
		offset = -16.0;
	
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_WEAPON_PROPERTIES - id - 1, CR_WHITE, 444.1, 76.1 + offset + 104.0 * id + 6.0 * ScrollPos.x, 0.0, 0.0);
}

void ShowOrbIcon(int id, int offset) {
	SetFont(GetItemImage(id + ITEM_IMAGE_ORB_BEGIN));
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_ORBS - id - 2, CR_WHITE, 237.4, 172.1 + 8.0 * ScrollPos.x + offset, 0.0, 0.0);
	SetFont("NMENUFNT");
}

void ShowLegendaryMonsterIcon(int id, int j) {
	switch(id) {
		case 0:
			SetFont("LEG_DRMR");
		break;
		case 1:
			SetFont("LEG_TORR");
		break;
		case 2:
			SetFont("LEG_MORD");
		break;
		case 3:
			SetFont("LEG_GODS");
		break;
		case 4:
			SetFont("LEG_GOLG");
		break;
	}
	
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * id - 1, CR_WHITE, 256.4, 56.1 + 144.0 * j + 4.0 * ScrollPos.x, 0.0, 0.0);
	SetFont("NMENUFNT");
}

void ShowBobby() {
	SetHudSize(1280, 864, 1);
	SetFont("BBYSNC");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 848.4, 140.1, 0.0, 0.0);
	SetFont("NMENUFNT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

str GetWeaponShopIcon(int id) {
	id = ShopTableIdToWeaponTableId(id);
	return Weapons_Data[id].icon;
}

str GetWeaponExplanation(int id) {
	int real_id = ShopTableIdToWeaponTableId(id);
	return GetWeaponTipText(real_id);
}

void ShowAccessoryIcon(int acc, int i) {
	SetHudSize(640, 480, 1);
	SetFont(AccessoryInfo[acc][ACCESSORY_ICON]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 3 * i - 2, CR_WHITE, 421.4, 75.1 + 48.0 * i, 0.0, 0.0);
	SetFont("NMENUFNT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

int CurrentAccessoryIndex () {
	return CheckInventory("Accessory_Index");
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
		case TYPE_ARTI:
		return amount * Clamp_Between(GetCVar("dnd_shop_artifact_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_ACCOUNT:
		return amount * Clamp_Between(GetCVar("dnd_shop_account_scale"), 1, SHOP_SCALE_MAX);
	}
	return 1;
}

int GetShopPrice (int id, int priceflag = 0) {
	int res = 0, chr = 0, shop_scale = Clamp_Between(GetCVar("dnd_shop_scale"), 1, SHOP_SCALE_MAX);
	if(id < MAXSHOPITEMS)
		res = ShopInfo[id][SHOPINFO_PRICE] * shop_scale;
	res = ShopScale(res, id);
	
	if(priceflag & PRICE_INCREASE_STOCK_LOW) {
	
	}
	
	if((GetItemFlags(id) & OBJ_SHOTGUN) && HasClassPerk_Fast("Hobo", 1)) {
		res -= res / 5;
		if(res < 0)
			res = 0;
	}
	
	return res;
}

// returns 0 for buy being possible
bool CanResearch(int respage, int posx) {
	int status = CheckResearchStatus(ResearchInfo[respage][posx].res_id);
	bool finish_check = status != RES_DONE;
	bool found_check = status == RES_KNOWN;
	bool budget_check = CheckInventory("Budget") >= (ResearchInfo[respage][posx].res_cost * (IsShotgunResearch(ResearchInfo[respage][posx].res_id) && HasClassPerk_Fast("Hobo", 1) ? 4 : 5)) / 5;
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
int CanTrade (int pnum, int id, int tradeflag, int price) {
	int credit = GetPlayerCredit(pnum), res = 0, type = 0;
	// enough credits - maxed on the item - I own this thing already - out of stock
	bool cond1 = 0, cond2 = 0, cond3 = 1, cond4 = 1;
	
	if(!price)
		price = GetShopPrice(id);
		
	cond1 = credit >= price;
		
	str item = "";
	str wepcheck = "";
	
	type = GetItemType(id);
	
	if(type == TYPE_ARTI)
		item = ArtifactInfo[id - SHOP_FIRSTARTI1_INDEX][ARTI_NAME]; // put it in the artifact info range
	else {
		item = GetItemName(id);
		wepcheck = GetWeaponCondition(id);
	}
	
	if(tradeflag & TRADE_BUY) {
		if(type == TYPE_AMMO) { // ammo
			cond2 = (CheckInventory(item) < GetAmmoCapacity(item)) && !GetPlayerAttributeValue(pnum, INV_EX_CANNOTPICKAMMO);
			cond4 = ShopStockRemaining[PlayerNumber()][id] > 0;
		}
		else if(type != TYPE_WEAPON && type != TYPE_ABILITY) { // item
			if(id != SHOP_ARTI_BACKPACK) {
				cond2 = (CheckInventory(item) < ShopInfo[id][SHOPINFO_MAX]);
				cond4 = ShopStockRemaining[PlayerNumber()][id] > 0;
			}
			else
				cond2 = !IsBackpackLimitReached();
		}
		else { // weapon or ability
			if(type == TYPE_WEAPON && HasClassPerk_Fast("Berserker", 1)) {
				if(id <= SHOP_WEAPON_SLOT1END) {
					cond3 = !CheckInventory(item);
					cond2 = cond3;
				}
				else if(id >= SHOP_WEP_BFG)
					return POPUP_YOUARENTALLOWED;
				else {
					cond3 = !CheckInventory(item);
					cond2 = cond3 && !CheckInventory(wepcheck);
				}
			}
			else {
				cond3 = !CheckInventory(item);
				cond2 = cond3 && !CheckInventory(wepcheck);
			}
			cond4 = ShopStockRemaining[PlayerNumber()][id] > 0;
		}
		
		if(!cond4)
			res = POPUP_OUTOFSTOCK;
		else if(cond1) { // I have credits
			if(!cond3)
				res = POPUP_ALREADYOWN;
			else if(!cond2) { // got credit but maxed
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

void DrawToggledLabel(str label, bool language_lookup, int afterlabel, int boxid, int boxcheck, int drawid, str color, str activecolor, int hudx, int hudy) {
	if(boxid == boxcheck)
		color = activecolor;
	if(afterlabel != -1) {
		if(language_lookup)
			HudMessage(s:color, l:label, s:": \c-", d:afterlabel; HUDMSG_PLAIN, drawid, CR_WHITE, hudx, hudy, 0.0, 0.0);
		else
			HudMessage(s:color, s:label, s:": \c-", d:afterlabel; HUDMSG_PLAIN, drawid, CR_WHITE, hudx, hudy, 0.0, 0.0);
	}
	else {
		if(language_lookup)
			HudMessage(s:color, l:label, s:": "; HUDMSG_PLAIN, drawid, CR_WHITE, hudx, hudy, 0.0, 0.0);
		else
			HudMessage(s:color, s:label, s:": "; HUDMSG_PLAIN, drawid, CR_WHITE, hudx, hudy, 0.0, 0.0);
	}
}

void DrawCredits(int pnum, int y_off = 0) {
	HudMessage(s:"\c[Y5]", l:"DND_MENU_CREDITS", s:": \c-$", d:GetPlayerCredit(pnum); HUDMSG_PLAIN, RPGMENUITEMSUBID, CR_WHITE, 264.1, 64.0 + y_off, 0.0, 0.0);
}

void DrawBudget() {
	int budget = CheckInventory("Budget");
	if(budget)
		HudMessage(s:"\c[Y5]", l:"DND_MENU_BUDGET", s:": \c-$", d:budget, s:"k"; HUDMSG_PLAIN, RPGMENUITEMSUBID, CR_WHITE, 264.1, 64.0, 0.0, 0.0);
	else
		HudMessage(s:"\c[Y5]", l:"DND_MENU_BUDGET", s:": \c-$", d:0; HUDMSG_PLAIN, RPGMENUITEMSUBID, CR_WHITE, 264.1, 64.0, 0.0, 0.0);
}

int GetAmmoToGive(int index) {
	return AmmoCounts[index - SHOP_FIRSTAMMO_INDEX][AMMOID_COUNT] * GetAmmoGainFactor() / 100;
}

str GetShopWeaponTag(int id) {
	str tag = StrParam(l:GetWeaponTag(ShopTableIdToWeaponTableId(id)));
	if(StrLen(tag) > MAX_TAGCHARS_SHOWN) {
		// we need to cut the longer text short
		if(GetChar(tag, MAX_TAGCHARS_SHOWN) == ' ')
			tag = StrLeft(tag, MAX_TAGCHARS_SHOWN);
		else if(GetChar(tag, MAX_TAGCHARS_SHOWN - 1) == ' ' || GetChar(tag, MAX_TAGCHARS_SHOWN - 2) == ' ')
			tag = StrParam(s:StrLeft(tag, MAX_TAGCHARS_SHOWN), s:".");
		else
			tag = StrParam(s:StrLeft(tag, MAX_TAGCHARS_SHOWN - 1), s:".");
	}
	return tag;
}

void DrawShopItemTag(str weptype, str toshow, int id, int objflag, int onposy) {
	str tag = "";
	bool use_str = objflag & OBJ_WEP;
	if(objflag & OBJ_WEP)
		tag = GetShopWeaponTag(id);
	else if(objflag & OBJ_AMMO)
		tag = GetAmmoTag(id);
	else if(objflag & OBJ_ABILITY)
		tag = GetAbilityTag(id - SHOP_ABILITY1_BEGIN);
	else if(objflag & OBJ_ARTI)
		tag = GetArtifactTag(id - SHOP_FIRSTARTI1_INDEX);
	else if(objflag & OBJ_ACCOUNT)
		tag = GetAccountPurchaseTag(id - SHOP_ACCOUNT_BEGIN);
	
	if(!use_str)
		HudMessage(s:weptype, s:toshow, l:tag; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
	else
		HudMessage(s:weptype, s:toshow, s:tag; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
}

// Draws a toggled image that changes color depending on given scenarios
// By default, if insufficient credits occur, it will be gray.
// of it's kind. For example, there are 2 weapons that replace the shotgun. You can have only one, so you set this flag, and set choicename to P_Slot3Replaced. One of the two will be
// red if the other is bought. The item that is bought will be green.
void DrawToggledImage(int pnum, int itemid, int boxid, int onposy, int objectflag, int offcolor, int oncolor, str choicename, int choicecount, int choicecolor, bool redraw = false) {
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
		int color = offcolor;
		str toshow = "\c[Y5]", colorprefix = "\cj", weptype = ""; // textcolor colors don't work for some reason
		
		int price = 0;
		
		if(!(objectflag & OBJ_AMMO))
			price = GetShopPrice(itemid);
		else
			price = GetShopPrice(itemid, PRICE_INCREASE_STOCK_LOW);
		
		bool sellstate = false;
		bool price_vs_credit = price > GetPlayerCredit(pnum);
		bool nostock = ShopStockRemaining[PlayerNumber()][itemid] <= 0;
		
		if(objectflag & OBJ_RESEARCH && !res_state) {
			toshow = "\c[W3]";
			colorprefix = "\c[W3]";
		}
		else {
			// if not ammo, talent or armor
			if(!(objectflag & (OBJ_AMMO | OBJ_ARMOR))) {
				// if not artifact and owning it (basically has weapon)
				if(!(objectflag & (OBJ_ARTI | OBJ_ACCOUNT)) && CheckInventory(GetItemName(itemid))) {
					color = oncolor;
					colorprefix = "\c[M3]";
					toshow = "\c[M3]";
					sellstate = true;
				}
				else { // no?
					if(price_vs_credit || nostock) {
						color = CR_BLACK;
						colorprefix = "\c[G8]";
						toshow = "\c[G8]";
					} // if I have options color others
					else if( (!(HasClassPerk_Fast("Berserker", 1) && itemid <= SHOP_WEAPON_SLOT1END) && CheckInventory(choicename) == choicecount) || 
							 (itemid == SHOP_ARTI_BACKPACK && IsBackpackLimitReached()) ||
							 (objectflag & OBJ_ARTI && IsSet(CheckInventory("DnD_Artifact_MapBits"), itemid - SHOP_FIRSTARTI1_INDEX))
						   ) 
					{
						// if has choice and count met or this is a backpack and limit reached on it (backpack limit has to be checked dynamically due to classic backpack cvar)
						color = choicecolor;
						colorprefix = "\c[Q2]";
						toshow = "\c[Q2]";
					}
				}
			}
			else { // ammo or armor
				if(price_vs_credit || nostock) {
					color = CR_BLACK;
					colorprefix = "\c[G8]";
					toshow = "\c[G8]";
				}
				else if(!(objectflag & (OBJ_AMMO | OBJ_ARMOR)) && CheckInventory(choicename) >= choicecount) {
					// we only wish armor price to be fit, otherwise we don't care -- replacing is default behavior on shop
					// this part only affects talents really and should be removed!
					// REMOVE IN THE FUTURE
					color = choicecolor;
					colorprefix = "\c[Q2]";
					toshow = "\c[Q2]";
				}
			}
		}
		
		if(objectflag & OBJ_WEP) {
			if(!IsLuxuryWeapon(ShopTableIdToWeaponTableId(itemid)))
				weptype = "\c[J7][\c[S7]R\c[J7]] ";
			else
				weptype = "\c[J7][\c[E3]L\c[J7]] ";

			// we aren't on anything, clear it
			if(boxid == MAINBOX_NONE && redraw)
				ClearInfoPanel();
		}
		else if(boxid == MAINBOX_NONE)
			DeleteText(RPGMENUHELPCORNERID);
		
		if(curposy == onposy) {
			// this part could be grouped into just deciding on a string to use, but I want to keep variable amount low here (already a lot)
			if(objectflag & OBJ_WEP) {
				if(sellstate)
					HudMessage(s:"\c[M1]--> ", l:"DND_MENU_SELLSFOR", s:":\c- $", d:GetShopPrice(itemid) / 2; HUDMSG_PLAIN, RPGMENUITEMID - 41, CR_WHITE, 192.1, 200.1, 0.0, 0.0);
				// stock
				HudMessage(s:toshow, l:"DND_MENU_STOCK", s:":\c- ", s:colorprefix, d:ShopStockRemaining[PlayerNumber()][itemid]; HUDMSG_PLAIN, RPGMENUITEMID - 42, CR_WHITE, 440.2, 200.1, 0.0, 0.0);
				SetHudClipRect(184, 216, 256, 64, 256);
				HudMessage
				(
					s:"\cd*\c- ", s:GetWeaponExplanation(itemid);
					HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 184.1, 216.1 + !!(objectflag & OBJ_USESCROLL) * (1.0 * ScrollPos.x), 0.0, 0.0
				);
				SetHudClipRect(0, 0, 0, 0, 0);
				
				// update corner panel info
				DrawWeaponIconCorner(itemid);
			}
			else if(objectflag & OBJ_AMMO) {
				SetHudClipRect(184, 244, 256, 36, 256);
				DrawAmmoExplanation(itemid);
				SetHudClipRect(0, 0, 0, 0, 0);
				// stock
				HudMessage(s:toshow, l:"DND_MENU_STOCK", s:":\c- ", s:colorprefix, d:ShopStockRemaining[PlayerNumber()][itemid]; HUDMSG_PLAIN, RPGMENUITEMID - 42, CR_WHITE, 440.2, 232.1, 0.0, 0.0);
				// bulk price -- res_state isn't used from below here
				res_state = GetBulkPriceForAmmo(itemid);
				if(res_state)
					HudMessage(s:toshow, l:"DND_MENU_INBULK", s:":\c- ", s:colorprefix, d:res_state; HUDMSG_PLAIN, RPGMENUITEMID - 43, CR_WHITE, 192.1, 232.1, 0.0, 0.0);
			}
			else if(objectflag & OBJ_ARTI) {
				SetHudClipRect(192, 224, 256, 64, 256);
				HudMessage(s:"\cd*\c- ", l:GetArtifactText(itemid - SHOP_FIRSTARTI1_INDEX); HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 232.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
				
				// update corner panel info
				DrawArtifactIconCorner(itemid - SHOP_FIRSTARTI1_INDEX);
			}
			else if(objectflag & OBJ_ABILITY) {
				SetHudClipRect(192, 208, 256, 64, 256);
				if(objectflag & OBJ_USESCROLL)
					HudMessage(s:"\cd*\c- ", l:GetAbilityHelpText(itemid - SHOP_ABILITY1_BEGIN); HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1 * 1.0 + ScrollPos.x, 0.0, 0.0);
				else
					HudMessage(s:"\cd*\c- ", l:GetAbilityHelpText(itemid - SHOP_ABILITY1_BEGIN); HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			else if(objectflag & OBJ_ACCOUNT) {
				SetHudClipRect(192, 208, 256, 64, 256);
				if(objectflag & OBJ_USESCROLL)
					HudMessage(s:"\cd*\c- ", l:GetAccountPurchaseText(itemid - SHOP_ACCOUNT_BEGIN); HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1 * 1.0 + ScrollPos.x, 0.0, 0.0);
				else
					HudMessage(s:"\cd*\c- ", l:GetAccountPurchaseText(itemid - SHOP_ACCOUNT_BEGIN); HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 216.1, 0.0, 0.0);
				SetHudClipRect(0, 0, 0, 0, 0);
			}
			DrawShopItemTag(weptype, "\c[B1]", itemid, objectflag, onposy);
			HudMessage(s:colorprefix, s:"--> $", d:price; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, color, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
		else {
			DrawShopItemTag(weptype, toshow, itemid, objectflag, onposy);
			HudMessage(s:colorprefix, s:"$", d:price; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, color, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
	}
}

void DrawAccessory(int id, int boxid, int page, menu_pane_T module& CurrentPane) {
	int pos = id - ACCESSORY_PER_PAGE * (page - MENU_FIRST_ACCESSORY_PAGE);
	if(CheckInventory(AccessoryInfo[id][ACCESSORY_NAME])) {
		if(boxid == pos + 1) {
			HudMessage(s:"\c[B1]", l:GetAccessoryText(id, ACCESSORY_TAG); HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_GREEN, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
			
			SetHudClipRect(192, 32, 256, 256, 256);
			HudMessage(s:StrParam(s:"+ ", l:GetAccessoryText(id, ACCESSORY_PRO), s:"\n\c[D4]- ", l:GetAccessoryText(id, ACCESSORY_CON)); HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos - 1, CR_GREEN, 192.1, 212.1, 0.0, 0.0);
			SetHudClipRect(0, 0, 0, 0, 0);
		}
		else if(IsAccessoryEquipped(0, 1 << id))
			HudMessage(s:"\c[C5]", l:GetAccessoryText(id, ACCESSORY_TAG); HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_GREEN, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
		else
			HudMessage(s:"\c[Y5]", l:GetAccessoryText(id, ACCESSORY_TAG); HUDMSG_PLAIN, RPGMENUITEMID - 3 * pos, CR_WHITE, 316.4, 74.1 + 32.0 * pos, 0.0, 0.0);
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
	Player_Weapon_Infos[pnum][wepid].quality = 0;
	
	for(int i = 0; i < MAX_WEP_MODS; ++i) {
		Player_Weapon_Infos[pnum][wepid].wep_mods[i][WMOD_WEP].val = 0;
		Player_Weapon_Infos[pnum][wepid].wep_mods[i][WMOD_WEP].tier = 0;
	}
	SyncClientsideVariable_WeaponMods(pnum, wepid);
}

int GetBulkPriceForAmmo(int itemid) {
	int id = GetAmmoSlotAndIndexFromShop(itemid);
	int temp = 0, count = 0;
	str ammo = AmmoInfo[id & 0xFFFF][id >> 16].name;
	if(CheckInventory(ammo) < GetAmmoCapacity(ammo)) {
		int price = GetShopPrice(itemid, PRICE_INCREASE_STOCK_LOW);
		temp = GetAmmoToGive(itemid);
		count = (GetAmmoCapacity(ammo) - CheckInventory(ammo)) / temp;
		id = price * count;
		// if there is remainder ammo
		count = GetAmmoCapacity(ammo) - (CheckInventory(ammo) + temp * count);
		if(count)
			id += Max(price * count / temp, 1);
		return id;
	}
	return 0;
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
				// loop 
				do {
					loopnumber++;
					
					// now consider money and other things as factors
					if(!(tradeflag & TRADE_AMMO))
						price = GetShopPrice(itemid);
					else
						price = GetShopPrice(itemid, PRICE_INCREASE_STOCK_LOW);
						
					buystatus = CanTrade(pnum, itemid, tradeflag, price);
					if(!buystatus) {
						// consider researches before handing out
						TakeCredit(price);
						GiveInventory(GetItemName(itemid), 1);
						if(tradeflag & TRADE_WEAPON) {
							totake = GetWeaponToTake(itemid);
							if(StrCmp(totake, ""))
								TakeInventory(totake, 1);
							GiveInventory(GetWeaponCondition(itemid), 1);
							SetWeapon(GetItemName(itemid));
							// fix special ammo cursor
							int fix = IsSpecialFixWeapon(itemid);
							if(fix != -1) {
								SetInventory(StrParam(s:"SpecialAmmoMode", s:GetSpecialAmmoSuffix(SpecialAmmoFixWeapons[fix][1])), SpecialAmmoRanges[SpecialAmmoFixWeapons[fix][2]][0]);
							}
						}
						if(tradeflag & TRADE_ARTIFACT)
							SetInventory("DnD_Artifact_MapBits", SetBit(CheckInventory("DnD_Artifact_MapBits"), itemid - SHOP_FIRSTARTI1_INDEX));
						--ShopStockRemaining[pnum][itemid];
					}
				} while (givefull && !buystatus);
				ACS_NamedExecuteAlways("DnD Sync Shop Stock", 0, pnum, itemid, ShopStockRemaining[pnum][itemid]);
				//sound (mostly)
				if (buystatus && loopnumber == 1)
					ShowPopup(buystatus, false, 0);
				else {
					if(tradeflag & TRADE_WEAPON)
						LocalAmbientSound("weapons/pickup", 127);
					else if(tradeflag & TRADE_AMMO)
						LocalAmbientSound("items/ammo", 127);
					else if(tradeflag & (TRADE_ABILITY | TRADE_ARTIFACT | TRADE_ACCOUNT)) {
						LocalAmbientSound("Bonus/Received", 127);
						if(itemid == SHOP_ACCOUNT_STASHTAB)
							++PlayerActivities[pnum].stash_pages;
					}
				}
			}
		}
	}
	else if(tradeflag & TRADE_SELL) {
		itemid = low + posy;
		if(itemid <= high) {
			if(!CheckInventory("DnD_SellConfirm")) { // confirmation screen did not pop
				buystatus = CanTrade(pnum, itemid, TRADE_SELL, 0);
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
				price = GetShopPrice(itemid) / 2;
				TakeInventory("DnD_SellConfirm", 1);
				TakeInventory("DnD_PopupSell", 1);
				TakeInventory("DnD_ShowSellPopup", 1);
				TakeInventory("DnD_MoneySpentQuest", price);
				SetInventory("DnD_PopupID", 0);
				SetInventory("ActivePopupBox", 0);

				// get slot of this weapon mapped to player discard
				buystatus = GetDiscardSlotFromShopWeapon(itemid);
				totake = GetWeaponToTake(itemid);
				if(StrCmp(totake, "") && !HasPlayerDiscardedSlot(pnum, buystatus))
					GiveInventory(totake, 1);

				// if you sell a weapon that has lock-on, it lingers because deselect state doesnt trigger apparently
				if(IsHomingHudWeapon(GetCurrentWeaponID())) {
					GiveInventory("DnD_RemoveLockOn", 1);
					ACS_NamedExecuteAlways("DnD Clear Homing Lock-on", 0);
				}

				// this works on weapon table ids, so map it there to use
				ResetWeaponStats(ShopTableIdToWeaponTableId(itemid));
				TakeInventory(GetWeaponCondition(itemid), 1);
				TakeInventory(GetItemName(itemid), 1);
				
				// reset buffs of weapon
				GiveCredit(price);
				ACS_NamedExecuteAlways("DnD Menu Sell Popup Clear", 0);
			}
		}
	}
}

void DrawHighLightBar (int posy, int framecounter) {
	int drawstate = 0;
	SetFont("NMENUFNT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	if(
		CheckInventory("AttributePoint") && 
		!(framecounter % 2)
	)
	{
		drawstate = posy == MAINBOX_STATS;
		HudMessage(s:"\c[B3]", l:"DND_MENU_SIDE_STATS"; HUDMSG_PLAIN, RPGMENULISTID, -1, 96.0, 168.0, 0.0);
	}
	else if(posy == MAINBOX_STATS)
		HudMessage(s:"\c[B1]", l:"DND_MENU_SIDE_STATS"; HUDMSG_PLAIN, RPGMENULISTID, -1, 96.0, 168.0, 0.0);
	else
		HudMessage(s:"\c[Y5]", l:"DND_MENU_SIDE_STATS"; HUDMSG_PLAIN, RPGMENULISTID, -1, 96.0, 168.0, 0.0);

	drawstate |= 2;
	if(
		CheckInventory("PerkPoint") && 
		!(framecounter % 2) &&
	    !(
			(CheckInventory("Perk_Sharpshooting") == DND_PERK_MAX) &&
			(CheckInventory("Perk_RiskAversion") == DND_PERK_MAX) &&
			(CheckInventory("Perk_Wisdom") == DND_PERK_MAX) &&
			(CheckInventory("Perk_Greed") == DND_PERK_MAX) &&
			(CheckInventory("Perk_Medic") == DND_PERK_MAX) &&
			(CheckInventory("Perk_Munitionist") == DND_PERK_MAX) &&
			(CheckInventory("Perk_Deadliness") == DND_PERK_MAX) &&
			(CheckInventory("Perk_Savagery") == DND_PERK_MAX) &&
			(CheckInventory("Perk_Luck") == DND_PERK_MAX)
		)
	)
	{
		drawstate |= (posy == MAINBOX_PERK) * 4;
		HudMessage(s:"\c[B3]", l:"DND_MENU_SIDE_PERKS"; HUDMSG_PLAIN, RPGMENULISTID - 1, -1, 96.0, 186.0, 0.0);
	}
	else if(posy == MAINBOX_PERK)
		HudMessage(s:"\c[B1]", l:"DND_MENU_SIDE_PERKS"; HUDMSG_PLAIN, RPGMENULISTID - 1, -1, 96.0, 186.0, 0.0);
	else
		HudMessage(s:"\c[Y5]", l:"DND_MENU_SIDE_PERKS"; HUDMSG_PLAIN, RPGMENULISTID - 1, -1, 96.0, 186.0, 0.0);


	if(posy == MAINBOX_NONE)
		HudMessage(s:""; HUDMSG_PLAIN, RPGMENUHIGHLIGHTID, -1, 47.1, 99.1, 0.0);
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
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUHIGHLIGHTID, -1, 47.1, 99.1 + 1.0 * yadd, 0.0);
	}
	else
		HudMessage(s:""; HUDMSG_PLAIN, RPGMENUHIGHLIGHTID, -1, 47.1, 99.1, 0.0);
	// restore default
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("NMENUFNT");
}

void DrawFrequentRedrawItems(int curopt, int pnum) {
	// generally used for drawing things right under cursor that are messages and such, and not dragged items
	// check if we have used an assimilation orb, if we did draw text under cursor
	if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
		int i = CheckInventory("DnD_UsedTwoItemRequirementMaterial") - 1;
		bool doDraw = false;
		str text = "";
		if(i != -1) {
			// player used an assimilation orb, figure out if they made a choice or not
			if(PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && PlayerInventoryList[pnum][i].item_subtype == DND_ORB_ASSIMILATION) {
				doDraw = true;
				if(!CheckInventory("DnD_SelectedInventoryBox"))
					text = StrParam(s:"\c[Q2]", l:"DND_MENU_ASSIMORB1");
				else
					text = StrParam(s:"\c[Q2]", l:"DND_MENU_ASSIMORB2");
			}
		}

		// draw right under crosshair
		if(doDraw) {
			int x = HUDMAX_XF - (PlayerCursorData.posx & MMASK) + 16.0;
			int y = HUDMAX_YF - (PlayerCursorData.posy & MMASK) + 24.0;
			SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
			SetFont("NMENUFNT");
			SetHudClipRect((x >> 16) - 8, (y >> 16) - 8, 96, 64, 96);
			HudMessage(s:text; HUDMSG_PLAIN, RPGMENUCURSORID + 1, -1, x + 0.1, y + 0.1, 0.0);
			SetHudClipRect(0, 0, 0, 0, 0);
		}
	}
}

void HandleAmmoPurchase(int pnum, int slot, int itemid, int shop_index, bool givefull, bool isSpecialAmmo) {
	if(!CheckItemRequirements(shop_index, RES_DONE, GetItemFlags(shop_index))) {
		// not done, so we can't give this
		ShowNeedResearchPopup();
	}
	else {
		int price = GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW);
		int buystatus = CanTrade(pnum, shop_index, TRADE_BUY, price);
		
		if(!buystatus) {
			int amt = GetAmmoToGive(shop_index), count = 1;
			
			// if we are maxing the ammo
			if(givefull) {
				// count was overcounting before
				--count;
				if(slot != -1)
					count += (GetAmmoCapacity(AmmoInfo[slot][itemid].name) - CheckInventory(AmmoInfo[slot][itemid].name)) / amt;
				else
					count += (GetAmmoCapacity(SpecialAmmoInfo_Str[itemid][AMMOINFO_NAME]) - CheckInventory(SpecialAmmoInfo_Str[itemid][AMMOINFO_NAME])) / amt;
				price = price * count;
				if(price > GetPlayerCredit(pnum)) {
					count = GetPlayerCredit(pnum) / GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW);
					price = count * GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW);
					amt *= count;
				}
				else {
					amt *= count;
					// we can indeed afford to buy a complete fill, check for remainder ammo from bulk purchase -- like 28 shells, need 32 but each buy gives 10 so 2 remains
					if(slot != -1)
						buystatus = GetAmmoCapacity(AmmoInfo[slot][itemid].name) - (CheckInventory(AmmoInfo[slot][itemid].name) + amt);
					else
						buystatus = GetAmmoCapacity(SpecialAmmoInfo_Str[itemid][AMMOINFO_NAME]) - (CheckInventory(SpecialAmmoInfo_Str[itemid][AMMOINFO_NAME]) + amt);
					if(buystatus) {
						amt += buystatus;
						price += Max(buystatus * GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW) / GetAmmoToGive(shop_index), 1);
					}
				}
			}
			if(amt > ShopStockRemaining[PlayerNumber()][shop_index]) {
				// rebalance the amount so we don't go in debt...
				amt = ShopStockRemaining[PlayerNumber()][shop_index];
				price = GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW) * amt / GetAmmoToGive(shop_index);
			}
			
			// we're OK now
			TakeCredit(price);
			if(slot != -1)
				GiveInventory(AmmoInfo[slot][itemid].name, amt);
			else
				GiveInventory(SpecialAmmoInfo_Str[itemid][AMMOINFO_NAME], amt);
			ShopStockRemaining[PlayerNumber()][shop_index] -= amt;
			ACS_NamedExecuteAlways("DnD Sync Shop Stock", 0, PlayerNumber(), shop_index, ShopStockRemaining[PlayerNumber()][shop_index]);
			LocalAmbientSound("items/ammo", 127);
		}
		else
			ShowPopup(buystatus, false, 0);
	}
}

bool point_in_inventory_box(rect_T module& box, int mx, int my) {
	return (mx <= box.topleft_x && mx >= box.botright_x && my <= box.topleft_y && my >= box.botright_y);
}

menu_inventory_T module& GetInventoryPane() {
	static menu_inventory_T pane;
	return pane;
}

void DisableBoxesInPane(menu_inventory_T module& p, int beg, int end) {
	for(int i = beg; i <= end; ++i)
		p.MenuRectangles[i].topleft_x = -1;
}

void DisableBoxInPane(menu_inventory_T module& p, int i) {
	p.MenuRectangles[i].topleft_x = -1;
}

void EnableBoxWithPoints(menu_inventory_T module& p, int box, int tx, int ty, int bx, int by) {
	//Log(d:box, s:" ", d:MAX_INVENTORY_BOXES);
	p.MenuRectangles[box].topleft_x = tx;
	p.MenuRectangles[box].topleft_y = ty;
	p.MenuRectangles[box].botright_x = bx;
	p.MenuRectangles[box].botright_y = by;
}

bool IsBoxEnabled(menu_inventory_T module& p, int box) {
	return p.MenuRectangles[box].topleft_x != -1;
}

// deepcopy to avoid accidental overriding
void AddBoxToPane(menu_pane_T module& p, rect_T module& box) {
	//Log(s:"add box ", d:p.cursize);
	//if(p.cursize < MAX_MENU_BOXES) {
		p.MenuRectangles[p.cursize].topleft_x = box.topleft_x;
		p.MenuRectangles[p.cursize].topleft_y = box.topleft_y;
		p.MenuRectangles[p.cursize].botright_x = box.botright_x;
		p.MenuRectangles[p.cursize].botright_y = box.botright_y;
		++p.cursize;
	/*}
	else
		Log(s:"Menu box limit exceeded.");*/
}

void AddBoxToInventory(menu_inventory_T module& p, rect_T module& box) {
	//Log(s:"add box inv ", d:p.cursize);
	//if(p.cursize < MAX_INVENTORY_BOXES) {
		p.MenuRectangles[p.cursize].topleft_x = box.topleft_x;
		p.MenuRectangles[p.cursize].topleft_y = box.topleft_y;
		p.MenuRectangles[p.cursize].botright_x = box.botright_x;
		p.MenuRectangles[p.cursize].botright_y = box.botright_y;
		++p.cursize;
	/*}
	else
		Log(s:"Menu box limit exceeded.");*/
}

void ResetInventoryPane(menu_inventory_T module& p) {
	p.cursize = 0;
}

void SetPaneSize(menu_pane_T module& p, int s) {
	p.cursize = s;
}

rect_T module& GetMainBox(int id) {
	static rect_T MainBoxList[MAX_MAIN_BOXES] = { 
		{ 419.0, 159.0, 348.0, 147.0 },
		{ 419.0, 141.0, 348.0, 129.0 },
		{ 419.0, 124.0, 348.0, 112.0 },
		{ 419.0, 107.0, 348.0, 95.0  },
		
		{ 419.0, 76.0,  348.0, 64.0  },
		{ 419.0, 59.0,  348.0, 47.0  },
		{ 419.0, 42.0,  348.0, 30.0  },
		
		// buttons
		{ 228.0, 35.0, 187.0, 16.0 },
		{ 187.0, 35.0, 146.0, 16.0 },
		{ 146.0, 35.0, 105.0, 16.0 }
	};
	return MainBoxList[id];
}

rect_T module& LoadRect(int menu_page, int id) {
	static rect_T bp[MAX_MENU_BOXPAGES][MAX_MENU_BOXES] = {
		// null
		{
			{ -1, -1, -1, -1 }
		},
		// stat 1
		{
			{ 288.0, 180.0, 108.0, 172.0 }, // str
			{ 288.0, 164.0, 108.0, 156.0 }, // dex
			{ 288.0, 148.0, 108.0, 140.0 }, // int
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// stat 2
		{
			{ 289.0, 229.0, 108.0, 222.0 }, // off
			{ 289.0, 213.0, 108.0, 206.0 }, // def
			{ 289.0, 197.0, 108.0, 190.0 }, // uti
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// stat off1
		{
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// stat off2
		{
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// stat def
		{
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// stat uti
		{
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// perk
		{
			{ 296.0, 245.0, 184.0, 237.0 }, // sharp
			{ 296.0, 229.0, 184.0, 221.0 }, // brutality
			{ 296.0, 213.0, 184.0, 205.0 }, // end
			{ 296.0, 197.0, 184.0, 189.0 }, // wis
			{ 296.0, 181.0, 184.0, 173.0 }, // greed
			{ 296.0, 165.0, 184.0, 157.0 }, // med
			{ 296.0, 149.0, 184.0, 141.0 }, // mun
			{ 296.0, 133.0, 184.0, 125.0 }, // ded
			{ 296.0, 117.0, 184.0, 109.0 }, // sav
			{ 296.0, 101.0, 184.0, 93.0  }, // luck
			{ -1, -1, -1, -1 }
		},
		// loadout
		{
			{ 289.0, 229.0, 248.0, 222.0 }, // items
			{ 289.0, 213.0, 215.0, 206.0 }, // inventory
			{ 289.0, 197.0, 243.0, 190.0 }, // stash
			{ 289.0, 181.0, 200.0, 174.0 }, // equipments
			{ 289.0, 165.0, 172.0, 158.0 }, // weapon discard
			{ 289.0, 149.0, 221.0, 142.0 }, // crafting
			{ 289.0, 133.0, 196.0, 126.0 }, // accessories
			{ 289.0, 117.0, 230.0, 110.0 }, // trading
			{ -1, -1, -1, -1 }
		},
		// loadout 1
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
		// loadout for equipments
		{
			// leftside -- charms
			{ 292.0, 132.0, 252.0, 94.0 },
			{ 236.0, 132.0, 196.0, 94.0 },
			{ 292.0, 84.0, 252.0, 46.0 },
			{ 236.0, 84.0, 196.0, 46.0 },
			{ 292.0, 194.0, 252.0, 140.0 },
			{ 236.0, 194.0, 196.0, 140.0 },
			{ 265.0, 282.0, 225.0, 200.0 },
			
			// rightside -- armors
			{ 112.0, 257.0, 62.0, 207.0 },
			{ 112.0, 193.0, 62.0, 143.0 },
			{ 161.0, 193.0, 111.0, 143.0 },
			{ 112.0, 129.0, 62.0, 79.0 },

			// inv explore icon
			{ 58.0, 66.0, 38.0, 48.0 },
			{ -1, -1, -1, -1 }
		},
		// loadout for weapon discarding
		{
			// weapon slots 1 to 9 discarding, no slot 8! They are only purchasable from the shop so 1 2 3 (+1 for ssg) 4 5 6 7 9
			{ 289.0, 245.0, 32.0, 238.0 },
			{ 289.0, 229.0, 32.0, 222.0 },
			{ 289.0, 213.0, 32.0, 206.0 },
			{ 289.0, 197.0, 32.0, 190.0 },
			{ 289.0, 181.0, 32.0, 174.0 },
			{ 289.0, 165.0, 32.0, 158.0 },
			{ 289.0, 149.0, 32.0, 142.0 },
			{ 289.0, 133.0, 32.0, 126.0 },
			{ 289.0, 117.0, 32.0, 110.0 },
			{ -1, -1, -1, -1 }
		},
		// loadout for crafting
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for crafting - weps
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for crafting - inv
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for crafting - transmute
		{
			{ -1, -1, -1, -1 }
		},
		// loadout for accessories below
		{
			{ 226.0, 248.0, 102.0, 238.0 },
			{ 224.0, 216.0, 107.0, 206.0 },
			{ 210.0, 184.0, 119.0, 174.0 },
			{ 240.0, 152.0, 87.0, 142.0 },
			{ 216.0, 120.0, 113.0, 110.0 },
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		{
			{ 240.0, 248.0, 90.0, 238.0 },
			{ 213.0, 216.0, 116.0, 206.0 },
			{ 208.0, 184.0, 120.0, 174.0 },
			{ 207.0, 152.0, 120.0, 142.0 },
			{ 226.0, 120.0, 102.0, 110.0 },
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		{
			{ 242.0, 248.0, 86.0, 238.0 },
			{ 220.0, 216.0, 108.0, 206.0 },
			{ 197.0, 184.0, 131.0, 174.0 },
			{ 232.0, 152.0, 97.0, 142.0 },
			{ 226.0, 120.0, 106.0, 110.0 },
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		{
			{ 244.0, 248.0, 86.0, 238.0 },
			{ 236.0, 216.0, 96.0, 206.0 },
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
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
			{ 289.0, 165.0, 183.0, 157.0 }, // account
			#ifdef ISAPRILFIRST
				{ 289.0, 149.0, 188.0, 141.0 }, // nft
			#endif
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
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
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
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE }, // right arr
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE }, // left arr
			{ -1, -1, -1, -1 }
		},
		// wep 3 - 3
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
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
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// wep 4 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
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
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// wep 5 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
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
			{ 289.0, 149.0, 120.0, 143.0 }, // w7
			{ 289.0, 133.0, 120.0, 127.0 }, // w8
			{ 45.0, 280.0, 45.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// wep 6 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
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
			{ 289.0, 245.0, 96.0, 237.0 }, // w1
			{ 289.0, 229.0, 96.0, 221.0 }, // w2
			{ 289.0, 213.0, 96.0, 205.0 }, // w3
			{ 289.0, 197.0, 96.0, 189.0 }, // w4
			{ 289.0, 181.0, 96.0, 173.0 }, // w5
			{ 289.0, 165.0, 96.0, 157.0 }, // w6
			{ 289.0, 149.0, 96.0, 141.0 }, // w7
			{ 289.0, 133.0, 96.0, 125.0 }, // w8
			{ 289.0, 117.0, 96.0, 109.0 }, // w9
			{ 289.0, 101.0, 96.0, 93.0 }, // w10
			{ -1, -1, -1, -1 }
		},
		// ammo 1 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ -1, -1, -1, -1 }
		},
		// ammo 2
		{
			{ 289.0, 245.0, 96.0, 237.0 }, // w1
			{ 289.0, 229.0, 96.0, 221.0 }, // w2
			{ 289.0, 213.0, 96.0, 205.0 }, // w3
			{ 289.0, 197.0, 96.0, 189.0 }, // w4
			{ 289.0, 181.0, 96.0, 173.0 }, // w5
			{ 289.0, 165.0, 96.0, 157.0 }, // w6
			{ 289.0, 149.0, 96.0, 141.0 }, // w7
			{ 289.0, 133.0, 96.0, 125.0 }, // w8
			{ 289.0, 117.0, 96.0, 109.0 }, // w9
			{ 289.0, 101.0, 96.0, 93.0 }, // w10
			{ -1, -1, -1, -1 }
		},
		// ammo 2 - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 96.0, 221.0 }, // w2
			{ -1, -1, -1, -1 }
		},
		// ammo 3
		{
			{ 289.0, 245.0, 96.0, 237.0 }, // w1
			{ 289.0, 229.0, 96.0, 221.0 }, // w2
			{ 289.0, 213.0, 96.0, 205.0 }, // w3
			{ 289.0, 197.0, 96.0, 189.0 }, // w4
			{ 289.0, 181.0, 96.0, 173.0 }, // w5
			{ 289.0, 165.0, 96.0, 157.0 }, // w6
			{ 289.0, 149.0, 96.0, 141.0 }, // w7
			{ 289.0, 133.0, 96.0, 125.0 }, // w8
			{ 289.0, 117.0, 96.0, 109.0 }, // w9
			{ -1, -1, -1, -1 }
		},
		// ammo 4 - 1
		{
			{ 289.0, 245.0, 96.0, 237.0 }, // w1
			{ 289.0, 229.0, 96.0, 221.0 }, // w2
			{ 289.0, 213.0, 96.0, 205.0 }, // w3
			{ 289.0, 197.0, 96.0, 189.0 }, // w4
			{ 289.0, 181.0, 96.0, 173.0 }, // w5
			{ 289.0, 165.0, 96.0, 157.0 }, // w6
			{ 289.0, 149.0, 96.0, 141.0 }, // w7
			{ 289.0, 133.0, 96.0, 125.0 }, // w8
			{ 289.0, 117.0, 96.0, 109.0 }, // w9
			{ 289.0, 101.0, 96.0, 93.0 }, // w10
			{ -1, -1, -1, -1 }
		},
		// ammo 4 - 2
		{
			{ 289.0, 245.0, 96.0, 237.0 }, // w1
			/*{ 289.0, 229.0, 96.0, 221.0 }, // w2
			{ 289.0, 213.0, 96.0, 205.0 }, // w3
			{ 289.0, 197.0, 96.0, 189.0 }, // w4
			{ 289.0, 181.0, 96.0, 173.0 }, // w5
			{ 289.0, 165.0, 96.0, 157.0 }, // w6
			{ 289.0, 149.0, 96.0, 141.0 }, // w7
			{ 289.0, 133.0, 96.0, 125.0 }, // w8
			{ 289.0, 117.0, 96.0, 109.0 }, // w9
			{ 289.0, 101.0, 96.0, 93.0 }, // w10*/
			{ -1, -1, -1, -1 }
		},
		// ammo special
		{
			{ 289.0, 245.0, 96.0, 237.0 }, // w1
			{ 289.0, 229.0, 96.0, 221.0 }, // w2
			{ 289.0, 213.0, 96.0, 205.0 }, // w3
			{ 289.0, 197.0, 96.0, 189.0 }, // w4
			{ 289.0, 181.0, 96.0, 173.0 }, // w5
			{ 289.0, 165.0, 96.0, 157.0 }, // w6
			{ 289.0, 149.0, 96.0, 141.0 }, // w7
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
			{ -1, -1, -1, -1 }
		},
		// artifact shop - 1
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
		// artifact shop - 2
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
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
			{ 289.0, 102.0, 80.0, 94.0 }, // donate
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
			{ 289.0, 92.0, 217.0, 84.0 }, // res
			{ 92.0, 92.0, 32.0, 84.0 }, // invest
			{ -1, -1, -1, -1 }
		},
		// research - ammo
		{
			{ 289.0, 92.0, 217.0, 84.0 }, // res
			{ 92.0, 92.0, 32.0, 84.0 }, // invest
			{ -1, -1, -1, -1 }
		},
		// research - slotguns
		{
			{ 289.0, 92.0, 217.0, 84.0 }, // res
			{ 92.0, 92.0, 32.0, 84.0 }, // invest
			{ -1, -1, -1, -1 }
		},
		// research - luxury guns
		{
			{ 289.0, 92.0, 217.0, 84.0 }, // res
			{ 92.0, 92.0, 32.0, 84.0 }, // invest
			{ -1, -1, -1, -1 }
		},
		// research - utility
		{
			{ 289.0, 92.0, 217.0, 84.0 }, // res
			{ 92.0, 92.0, 32.0, 84.0 }, // invest
			{ -1, -1, -1, -1 }
		},
		// main
		{
			{ -1, -1, -1, -1 }
		},
		// help
		{
			{ 289.0, 261.0, 179.0, 254.0 }, // char
			{ 289.0, 245.0, 179.0, 238.0 }, // class perks
			{ 289.0, 229.0, 162.0, 222.0 }, // weapon prop
			{ 289.0, 213.0, 178.0, 206.0 }, // res
			{ 289.0, 197.0, 178.0, 190.0 }, // dmg
			{ 289.0, 181.0, 178.0, 174.0 }, // ailments
			{ 289.0, 165.0, 178.0, 158.0 }, // orb
			{ 289.0, 149.0, 182.0, 142.0 }, // mods
			{ 289.0, 133.0, 179.0, 126.0 }, // leg
			{ 296.0, 97.0, 182.0, 89.0 }, // show info
			{ 296.0, 81.0, 182.0, 73.0 }, // show info
			{ 296.0, 65.0, 142.0, 57.0 }, // show mod tiers
			{ 296.0, 49.0, 142.0, 41.0 }, // show mod tiers
			{ -1, -1, -1, -1 }
		},
		// help char
		{
			{ -1, -1, -1, -1 }
		},
		// help class perks
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
		// help ailments
		{
			{ -1, -1, -1, -1 }
		},
		// help orbs
		{
			{ -1, -1, -1, -1 }
		},
		// help monster mods
		{
			{ 289.0, 229.0, 144.0, 222.0 }, // weakness
			{ 289.0, 213.0, 144.0, 206.0 }, // res
			{ 289.0, 197.0, 144.0, 190.0 }, // imm
			{ 289.0, 181.0, 144.0, 174.0 }, // agg
			{ 289.0, 165.0, 144.0, 158.0 }, // def
			{ 289.0, 149.0, 144.0, 142.0 }, // uti
			{ 289.0, 133.0, 144.0, 126.0 }, // special
			{ -1, -1, -1, -1 }
		},
		// help leg
		{
			{ -1, -1, -1, -1 }
		},
		// help mmod weakness
		{
			{ -1, -1, -1, -1 }
		},
		// help mmod resist
		{
			{ -1, -1, -1, -1 }
		},
		// help mmod immunity
		{
			{ -1, -1, -1, -1 }
		},
		// help mmod aggressive
		{
			{ -1, -1, -1, -1 }
		},
		// help mmod defensive
		{
			{ -1, -1, -1, -1 }
		},
		// help mmod utility
		{
			{ -1, -1, -1, -1 }
		},
		// help special
		{
			{ -1, -1, -1, -1 }
		},
		// ability
		{
			{ 289.0, 69.0, 104.0, 61.0 }, // dash
			{ -1, -1, -1, -1 }
		}
		#ifdef ISAPRILFIRST
		,
		{
			{ 289.0, 209.0, 120.0, 201.0 }, // w1
			{ 289.0, 193.0, 120.0, 185.0 }, // w2
			{ 289.0, 177.0, 120.0, 169.0 }, // w3
			{ 289.0, 161.0, 120.0, 153.0 }, // w4
			{ 289.0, 145.0, 120.0, 137.0 }, // w5
			{ 289.0, 129.0, 120.0, 121.0 }, // w6
			{ 289.0, 113.0, 120.0, 105.0 }, // w7
			{ 289.0, 97.0, 120.0, 91.0 }, // w8
			{ -1, -1, -1, -1 }
		}
		#endif
	};
	return bp[menu_page][id];
}

// specialized one for inventory view
rect_T module& LoadInventoryViewRect(int id) {
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

rect_T module& LoadStashViewRect(int id) {
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

rect_T module& LoadTradeViewRect(int id) {
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

rect_T module& LoadCraftingViewRect(int id) {
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
		bp[CRAFTING_WEAPON_BOXID].topleft_x = -1;
		bp[CRAFTING_WEAPON_BOXID].topleft_y = -1;
		bp[CRAFTING_WEAPON_BOXID].botright_x = -1;
		bp[CRAFTING_WEAPON_BOXID].botright_y = -1;
		// inventory crafting box
		bp[CRAFTING_INVENTORY_BOXID].topleft_x = -1;
		bp[CRAFTING_INVENTORY_BOXID].topleft_y = -1;
		bp[CRAFTING_INVENTORY_BOXID].botright_x = -1;
		bp[CRAFTING_INVENTORY_BOXID].botright_y = -1;
		// transmuting crafting box
		bp[CRAFTING_TRANSMUTING_BOXID].topleft_x = -1;
		bp[CRAFTING_TRANSMUTING_BOXID].topleft_y = -1;
		bp[CRAFTING_TRANSMUTING_BOXID].botright_x = -1;
		bp[CRAFTING_TRANSMUTING_BOXID].botright_y = -1;
		
		PaneSetup = SetBit(PaneSetup, CRAFTING_SETUP_BIT);
	}
	return bp[id];
}

void LoadTradeView(menu_inventory_T module& p) {
	p.cursize = 0;
	for(int i = 0; i < 3 * MAX_INVENTORY_BOXES + 2; ++i) {
		auto r = LoadTradeViewRect(i);
		if(r.topleft_x != -1) {
			// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
			AddBoxToInventory(p, r);
		}
		else
			break;
	}
}

void LoadInventoryView(menu_inventory_T module& p) {
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

void LoadStashView(menu_inventory_T module& p) {
	p.cursize = 0;
	for(int i = 0; i < 2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES; ++i) {
		auto r = LoadStashViewRect(i);
		if(r.topleft_x != -1) {
			// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
			AddBoxToInventory(p, r);
		}
		else
			break;
	}
}

void LoadCraftingView(menu_inventory_T module& p) {
	p.cursize = 0;
	for(int i = 0; i < MAX_CRAFTING_BOXES; ++i) {
		auto r = LoadCraftingViewRect(i);
		// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
		AddBoxToInventory(p, r);
	}
}

void LoadPane(menu_pane_T module& p, int menu_page) {
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

int GetTriggeredBoxOnInventoryPane(menu_inventory_T module& p, int mx, int my) {
	if(mx >= 400.0 || my <= 64.0 || mx <= 82.0 || my >= 260.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnCraftingPane(menu_inventory_T module& p, int mx, int my) {
	if(mx >= 470.0 || my <= 24.0 || mx <= 4.0 || my >= 293.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my)) {
			return i + 1;
		}
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnTradePane(menu_inventory_T module& p, int mx, int my) {
	if(mx >= 465.0 || mx <= 30.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnStashPane(menu_inventory_T module& p, int mx, int my) {
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

void DrawBoxText(str msg, bool language_lookup, int boxid, int thisboxid, int id, int x, int y, str hcolor, str lcolor) {
	str color = lcolor;
	if(boxid == thisboxid && !CheckInventory("DnD_HighlightBlocker"))
		color = hcolor;

	if(language_lookup)
		HudMessage(s:color, l:msg, s:"\c-"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
	else
		HudMessage(s:color, s:msg, s:"\c-"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
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

menu_stack_T module& GetMenuStack(int pnum) {
	static menu_stack_T stack[MAXPLAYERS];
	return stack[pnum];
}

void FlushStack(int pnum) {
	menu_stack_T module& stack = GetMenuStack(pnum);
	for(int i = 0; i < MAX_STACK_ELEMS; ++i)
		stack.stack_elems[i] = MENU_MAIN;
	stack.cursize = 1;
	stack.stackptr = 0;
}

void PushStack(int pnum, int val) {
	menu_stack_T module& stack = GetMenuStack(pnum);
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
	ACS_NamedExecuteWithResult("DnD Main Button Click Animation", boxid);

	// all other boxes now have some other behavior
	// flush stack if return is pressed
	menu_stack_T module& stack = GetMenuStack(PlayerNumber());
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

// opt is menu page, multipage is for when one option can lead to multiple weapon pages
void HandleWeaponPageDraw(int pnum, int opt, int multipage, int slotid, int boxid, int scrollamt, int total_boxes, bool redraw) {
	int begin = GetWeaponBeginIndexFromOption(opt);
	int end = GetWeaponEndIndexFromOption(opt);

	// if both pages exist, left arrow is always the last one
	int sub_for_arr = 0;
	
	if(multipage > 1 || multipage < 0) {
		DrawBoxText("<=", DND_NOLOOKUP, boxid, total_boxes, RPGMENUPAGEID - 1, 184.1, 44.0, "\c[B1]", "\c[Y5]");
		sub_for_arr = 1;
	}

	if(multipage > 0)
		DrawBoxText("=>", DND_NOLOOKUP, boxid, total_boxes - sub_for_arr, RPGMENUPAGEID, 436.1, 44.0, "\c[B1]", "\c[Y5]");

	// negative indicates last page
	if(multipage < 0)
		multipage = -multipage;

	if(multipage)
		HudMessage(s:"--- ", l:"DND_MENU_SLOT", s:" ", d:slotid, s:" (", d:multipage, s:") ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	else
		HudMessage(s:"--- ", l:"DND_MENU_SLOT", s:" ", d:slotid, s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	HudMessage(s:"\c[Y5]", l:"DND_MENU_CREDITS", s:": \c-$", d:GetPlayerCredit(pnum); HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 264.1, 64.0, 0.0, 0.0);
	
	for(int i = begin; i <= end; ++i)
		DrawToggledImage(pnum, i, boxid, i - begin, WeaponDrawInfo[i - SHOP_WEAPON_BEGIN].flags, CR_WHITE, CR_GREEN, GetWeaponCondition(i), 1, CR_RED, redraw);
}

// These except ammo indexes, true ammo indexes (dnd_ammo.h) not menu relative
void HandleAmmoPageDraw(int pnum, int boxid, int slot, int multipage, int start_index, int end_index, bool specialammo) {
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
		HudMessage(s:"--- ", l:"DND_MENU_AMMOCATEGORY", s:" ", d:slot + 1, s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	else
		HudMessage(s:"--- ", l:"DND_MENU_SPECIALAMMO", s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0, 0.0);
	
	HudMessage(s:"\c[Y5]", l:"DND_MENU_CREDITS", s:": \c-$", d:GetPlayerCredit(pnum); HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 264.1, 64.0, 0.0, 0.0);

	if(!specialammo) {
		for(i = start_index; i <= end_index; ++i) {
			shopindex = MenuAmmoIndexMap[slot][i];
			DrawToggledImage(pnum, shopindex, boxid, i - start_index, AmmoDrawInfo[shopindex - SHOP_FIRSTAMMO_INDEX].flags, CR_WHITE, CR_GREEN, "", 1, CR_RED);
		}
		DrawAmmoIconCorner(slot, boxid, boxid + start_index - 1, false);
	}
	else {
		for(i = 0; i < MAX_SPECIAL_AMMOS_FOR_SHOP; ++i) {
			shopindex = SHOP_FIRSTAMMOSPECIAL_INDEX + i;
			DrawToggledImage(pnum, shopindex, boxid, i, AmmoDrawInfo[shopindex - SHOP_FIRSTAMMO_INDEX].flags, CR_WHITE, CR_GREEN, "", 1, CR_RED);
		}
		DrawAmmoIconCorner(-1, boxid, -1, true);
	}
}

void HandleWeaponPageInput(int pnum, int boxid, int wbegin, int wend, int pageprev, int pagenext) {
	if(HasLeftClicked(pnum)) {
		if(boxid != MAINBOX_NONE) {
			// only one arrow available
			if(pageprev == -1 || pagenext == -1) {
				if(boxid - 1 == wend - wbegin + 1) {
					if(pageprev != -1)
						UpdateMenuPosition(pageprev);
					else
						UpdateMenuPosition(pagenext);
				}
				else
					ProcessTrade(pnum, boxid - 1, wbegin, wend, TRADE_BUY | TRADE_WEAPON, false);
			}
			else {
				// both arrows available
				if(boxid - 1 == wend - wbegin + 2)
					UpdateMenuPosition(pageprev);
				else if(boxid - 1 == wend - wbegin + 1)
					UpdateMenuPosition(pagenext);
				else
					ProcessTrade(pnum, boxid - 1, wbegin, wend, TRADE_BUY | TRADE_WEAPON, false);
			}
		}
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

void HandleAmmoPageInput(int pnum, int slot, int boxid, int begin_index, int pageprev, int pagenext, bool IsSpecialAmmo) {
	int id = 0;
	
	if(IsSpecialAmmo) {
		id = SHOP_FIRSTAMMOSPECIAL_INDEX + boxid - 1;
		slot = -1;
	}
	else
		id = MenuAmmoIndexMap[slot][boxid + begin_index - 1];
		
	// printbold(d:id, s: " ", d:boxid + begin_index - 1, s: " vs ", d:AMMO_SLAYER);
	
	if(HasLeftClicked(pnum)) {
		if(boxid != MAINBOX_NONE)
			HandleAmmoPurchase(pnum, slot, boxid + begin_index - 1, id, false, IsSpecialAmmo);
		ClearPlayerInput(pnum, true);
	} // ammos have alternate functionality for sell
	else if(HasRightClicked(pnum) && boxid != MAINBOX_NONE) {
		HandleAmmoPurchase(pnum, slot, boxid + begin_index - 1, id, true, IsSpecialAmmo);
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

void HandleResearchPageDraw(int pnum, int page, int boxid) {
	int posx = CheckInventory("MenuPosX");
	int status = CheckResearchStatus(ResearchInfo[page][posx].res_id);
	int budget = CheckInventory("Budget");

	HudMessage(s:"--- ", l:"DND_MENU_HEAD_RESPAN", s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, 316.4, 44.0, 0.0);
	
	DrawCredits(pnum, -8.0);

	if(posx)
		HudMessage(s:"\c[Y5]<="; HUDMSG_PLAIN, RPGMENUPAGEID - 1, CR_CYAN, 184.1, 44.0, 0.0);

	if(ResearchInfo[page][posx + 1].res_id != -1)
		HudMessage(s:"\c[Y5]=>"; HUDMSG_PLAIN, RPGMENUPAGEID, CR_CYAN, 436.1, 44.0, 0.0);

	if(budget)
		HudMessage(s:"\c[Y5]", l:"DND_MENU_BUDGET", s:": \c-$", d:budget, s:"\cjK"; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 264.1, 72.0, 0.0);
	else
		HudMessage(s:"\c[Y5]", l:"DND_MENU_BUDGET", s:": \c-$0"; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 264.1, 72.0, 0.0);
	
	// adjust x offset on 2nd hudmsg based on length of status for future language compat
	if(status == RES_NA) {
		HudMessage(s:"\c[Y5]", l:"DND_MENU_STATUS", s:": "; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 264.1, 116.1, 0.0);
		SetHudClipRect(324, 108, 128, 32, 128);
		HudMessage(l:"DND_MENU_RESEARCH_NA"; HUDMSG_PLAIN, RPGMENUITEMID - 2, CR_WHITE, 324.1, 116.1, 0.0);
		SetHudClipRect(0, 0, 0, 0, 0);
		SetFont("RESBLAK");
	}
	else if(status == RES_KNOWN) {
		HudMessage(s:"\c[Y5]", l:"DND_MENU_STATUS", s:": "; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 264.1, 116.1, 0.0);
		SetHudClipRect(324, 108, 128, 32, 128);
		HudMessage(l:"DND_MENU_RESEARCH_FOUND"; HUDMSG_PLAIN, RPGMENUITEMID - 2, CR_WHITE, 324.1, 116.1, 0.0);
		SetHudClipRect(0, 0, 0, 0, 0);
		SetFont("RESFOUND");
	}
	else {
		HudMessage(s:"\c[Y5]", l:"DND_MENU_STATUS", s:": "; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 264.1, 116.1, 0.0);
		SetHudClipRect(324, 108, 128, 32, 128);
		HudMessage(l:"DND_MENU_RESEARCH_DONE"; HUDMSG_PLAIN, RPGMENUITEMID - 2, CR_WHITE, 324.1, 116.1, 0.0);
		SetHudClipRect(0, 0, 0, 0, 0);
		SetFont("RESDONE");
	}
	
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_WHITE, 192.1, 96.0, 0.0);

	bool no_drop = ResearchFlags[ResearchInfo[page][posx].res_id].res_flags & RESF_NODROP;
	if(no_drop && status == RES_NA) {
		SetFont("RESNONE");
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_WHITE, 199.1, 96.0, 0.0);
		
		SetFont("NMENUFNT");
		HudMessage(s:"\c[Y5]", l:"DND_MENU_ENTRY", s:":\c- ", l:"DND_MENU_NA"; HUDMSG_PLAIN, RPGMENUITEMID - 5, CR_WHITE, 264.1, 88.0, 0.0);
		HudMessage(s:"\c[Y5]", l:"DND_MENU_BUDGET", s: " ", l:"DND_MENU_COST", s:":\c- ", l:"DND_MENU_NA"; HUDMSG_PLAIN, RPGMENUITEMID - 6, CR_WHITE, 264.1, 104.0, 0.0);
	}
	else {
		SetFont(GetResearchIcon(page, posx));
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_WHITE, 199.1, 96.0, 0.0);
		
		SetFont("NMENUFNT");
		
		HudMessage(s:"\c[Y5]", l:"DND_MENU_ENTRY", s:":\c- #", d:ResearchInfo[page][posx].res_number; HUDMSG_PLAIN, RPGMENUITEMID - 5, CR_WHITE, 264.1, 88.0, 0.0);
		HudMessage(s:"\c[Y5]", l:"DND_MENU_BUDGET", s: " ", l:"DND_MENU_COST", s:":\c- $", d:ResearchInfo[page][posx].res_cost, s:"k"; HUDMSG_PLAIN, RPGMENUITEMID - 6, CR_WHITE, 264.1, 104.0, 0.0);
		
		SetHudClipRect(192, 144, 256, 96, 256);
		HudMessage(l:GetResearchDescription(ResearchInfo[page][posx].res_id); HUDMSG_PLAIN, RPGMENUITEMID - 7, CR_WHITE, 192.1, 152.1, 0.0);
		SetHudClipRect(0, 0, 0, 0, 0);
	}

	DrawBoxText("DND_MENU_RESEARCH", DND_LANGUAGE_LOOKUP, boxid, MBOX_1, RPGMENUITEMID - 8, 192.1, 232.0, "\c[B1]", "\c[Y5]");

	// if hovering on invest option, show the investment cost to the user
	if(status == RES_NA) {
		// no drop ones can't be clicked on either way so color them black
		if(!no_drop) {
			DrawBoxText("DND_MENU_INVEST", DND_LANGUAGE_LOOKUP, boxid, MBOX_2, RPGMENUITEMID - 9, 392.1, 232.0, "\c[B1]", "\c[Y5]");
		
			// show the invested % chance to drop if it can drop
			HudMessage(s:"\c[Y5]", l:"DND_MENU_DISCOVERYCHANCE", s:": \cj", s:GetFixedRepresentation(GetResearchDropRate(pnum, ResearchInfo[page][posx].res_id, true), false), s:"%"; HUDMSG_PLAIN, RPGMENUITEMID - 10, CR_WHITE, 264.1, 136.1, 0.0, 0.0);
		}
		else
			DrawBoxText("DND_MENU_INVEST", DND_LANGUAGE_LOOKUP, boxid, MBOX_2, RPGMENUITEMID - 11, 392.1, 232.0, "\c[B1]", "\c[G8]");
			
		if(boxid == MBOX_2) {
			HudMessage(s:"\c[Y5]", l:"DND_MENU_COST", s:": \cj", d:GetInvestmentCost(pnum, page, posx, ResearchInfo[page][posx].res_id); HUDMSG_PLAIN, RPGMENUITEMID - 12, CR_WHITE, 440.2, 236.1, 0.0, 0.0);
			SetHudClipRect(184, 244, 256, 36, 256);
			HudMessage(s:"\cd*\c- ", l:"DND_INVESTMENT_EXPLANATION"; HUDMSG_PLAIN, RPGMENUITEMID - 13, CR_WHITE, 184.1, 248.1 + 2.0 * ScrollPos.x, 0.0, 0.0);
			SetHudClipRect(0, 0, 0, 0, 0);
		}
		//else
		//	DeleteTextRange(RPGMENUITEMID - 13, RPGMENUITEMID - 12);
	}
	else
		DrawBoxText("DND_MENU_INVEST", DND_LANGUAGE_LOOKUP, boxid, MBOX_2, RPGMENUITEMID - 14, 392.1, 232.0, "\c[B1]", "\c[G8]");
}

int GetInvestmentCost(int pnum, int page, int menu_res_id, int res_id) {
	// base budget cost's 15% + 5% is applied per investment on top of a base of 1k credits to the previous value
	// let (5 + x / 10) = y
	// c * (100 + k * (5 + x / 10)) / 100 -- replace c here with k - 1's value
	// c * (100 + (k - 1) * (5 + x / 10)) / 100 * (100 + k * (5 + x / 10)) / 100
	// c * (100 + (k - 1) * y) / 100 * (100 + k * y) / 100
	int k = ResearchInvestments[pnum][res_id];
	if(!k)
		return BASE_INVESTMENT_COST;
	else if(k == 1)
		return BASE_INVESTMENT_COST * (105 + 3 * ResearchInfo[page][menu_res_id].res_cost / 20) / 100;
		
	int y = 5 + 3 * ResearchInfo[page][menu_res_id].res_cost / 20;
	return (BASE_INVESTMENT_COST * (100 + (k - 1) * y) / 100) * (100 + k * y) / 100;
}

void HandleResearchPageInput(int pnum, int page, int boxid) {
	bool buystatus = 0;
	int curposx = MenuInputData[pnum][DND_MENUINPUT_LRPOS];
	if(HasLeftClicked(pnum)) {
		if(boxid == MBOX_1) {
			buystatus = CanResearch(page, curposx);
			if(!buystatus) {
				TakeBudget(ResearchInfo[page][curposx].res_cost);
				LocalAmbientSound("items/research", 127);
				CompleteResearch(ResearchInfo[page][curposx].res_id);
			}
			else
				ShowPopup(CheckInventory("DnD_PopupId"), false, 0);
		}
		else if(boxid == MBOX_2) {
			// investment option
			// check if it's not found already, no point investing into finding it if its there already
			if(CheckResearchStatus(ResearchInfo[page][curposx].res_id) != RES_NA) {
				// popup, you found this research already
				ShowPopup(POPUP_ALREADYFOUND, false, 0);
			}
			else if(ResearchFlags[ResearchInfo[page][curposx].res_id].res_flags & RESF_NODROP) {
				// popup, we dont know this one yet
				ShowPopup(POPUP_NARESEARCH, false, 0);
			}
			else {
				int resid = ResearchInfo[page][curposx].res_id;
				int credit = GetPlayerCredit(pnum);
				int cost = GetInvestmentCost(pnum, page, curposx, resid);
				if(credit < cost) {
					// not enough credits
					ShowPopup(POPUP_NOFUNDS, false, 0);
				}
				else if(ResearchInvestments[pnum][resid] >= DND_MAX_INVESTMENT) {
					// you already (somehow) invested to make this appear 100%, no need!
					ShowPopup(POPUP_MAXINVESTMENT, false, 0);
				}
				else {
					// investment can proceed
					++ResearchInvestments[pnum][resid];
					TakeCredit(cost);
					ACS_NamedExecuteWithResult("DnD Menu Investment Sync", pnum, resid, ResearchInvestments[pnum][resid]);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
			}
		}
		ClearPlayerInput(pnum, true);
	}
}

void ResetInventoryLitState(int beg, int end) {
	for(int i = beg; i < end; ++i)
		InventoryBoxLit[i] = BOXLIT_STATE_OFF;
}

void DrawCharmBox(int charm_type, int boxid, int thisboxid, int hudx, int hudy) {
	str charmborderpic = GetCharmBoxLabel(charm_type, boxid == thisboxid);
	int pnum = PlayerNumber();
	
	// fixes background being lit on first row boxes
	if(CheckInventory("DnD_InventoryView"))
		charmborderpic = GetCharmBoxLabel(charm_type, false);
	
	// if there is a charm here
	if(Items_Used[pnum][thisboxid - 1].item_type != DND_ITEM_NULL) {
		SetFont(GetItemImage(Items_Used[pnum][thisboxid - 1].item_image));
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid - 1, CR_WHITE, hudx, hudy, 0.0, 0.0);
		
		if(boxid == thisboxid && !CheckInventory("DnD_InventoryView"))
			UpdateCursorHoverData(thisboxid - 1, DND_SYNC_ITEMSOURCE_ITEMSUSED, 0, pnum, 0, HUDMAX_X, HUDMAX_Y);
	}
	SetFont(charmborderpic);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid, CR_WHITE, hudx, hudy, 0.0, 0.0);
	SetFont("NMENUFNT");
}

void DrawArmorBox(int boxid, int thisboxid, int hudx, int hudy, int armor_slot) {
	int pnum = PlayerNumber();

	if(Items_Used[pnum][thisboxid - 1].item_type != DND_ITEM_NULL) {
		SetFont(GetItemImage(Items_Used[pnum][thisboxid - 1].item_image));
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid - 1, CR_WHITE, hudx, hudy, 0.0, 0.0);
		
		if(boxid == thisboxid && !CheckInventory("DnD_InventoryView"))
			UpdateCursorHoverData(thisboxid - 1, DND_SYNC_ITEMSOURCE_ITEMSUSED, 0, pnum, 0, HUDMAX_X, HUDMAX_Y);
	}
	else {
		switch(armor_slot) {
			case DND_ITEM_BODYARMOR:
				SetFont("ARMBAK");
			break;
			case DND_ITEM_HELM:
				SetFont("HELMBAK");
			break;
			case DND_ITEM_BOOT:
				SetFont("BOOTBAK");
			break;

			// specialty items
			case DND_ITEM_SPECIALTY_CYBORG:
				if(!HasClassPerk("Cyborg", 2))
					SetFont("PCORBAKB");
				else
					SetFont("PCORBAK");
			break;
		}
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 30 - thisboxid, CR_WHITE, hudx, hudy, 0.0, 0.0);
	}

	if(boxid == thisboxid && !CheckInventory("DnD_InventoryView"))
		SetFont("ARMSELG");
	else
		SetFont("ARMSELB");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid, CR_WHITE, hudx, hudy, 0.0, 0.0);
	SetFont("NMENUFNT");
}

void DrawInventoryExploreIcon(int boxid, int thisboxid, int id, int x, int y) {
	SetFont("ICOBSML");
	if(boxid == thisboxid && !CheckInventory("DnD_InventoryView")) // don't highlight in inventory view
		SetFont("ICOGSML");
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_CYAN, x, y, 0.0, 0.0);
	SetFont("NMENUFNT");
}

void DrawInventoryBlock(int idx, int idy, int bid, bool hasItem, int basex, int basey, int skip, int idbase, int source, int pnum, int boff, int itemskipx, int itemskipy) {
	int temp;
	int img = GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, bid, -1, source);
	// inventory icon
	if(hasItem && (PlayerCursorData.itemDragInfo.topboxid != bid || PlayerCursorData.itemDragInfo.source != source)) {
		// shift down tall items height - 1 times
		temp = (GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, bid, -1, source) - 1) * ITEMOFFSETSCALE;
		SetFont(GetItemImage(img, true));
		HudMessage(
			s:"A"; 
			HUDMSG_PLAIN, idbase - bid - boff - MAX_INVENTORY_BOXES - 2, 
			CR_WHITE, 
			basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + (GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, bid, -1, source) - 1) * ITEMOFFSETSCALE, 
			basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + temp, 
			0.0, 
			0.0
		);
	
		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, bid, -1, source);
		if(temp) {
			SetFont("NMENUFNT");
			HudMessage(d:temp; HUDMSG_PLAIN, idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + 14.2, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + 8.0, 0.0, 0.0);
		}
		else
			DeleteText(idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2);
	}
	else { // refresh picture if swapped
		DeleteText(idbase - bid - boff - MAX_INVENTORY_BOXES - 2);
		DeleteText(idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2);
	}

	// gray inventory chunks
	if(IsValidBox(bid) && InventoryBoxLit[bid + boff] == BOXLIT_STATE_CURSORON)
		SetFont("LDTBOXS");
	else if(InventoryBoxLit[bid + boff] == BOXLIT_STATE_BAD)
		SetFont("LDTBOXN");
	else if(CheckActorInventory(pnum + P_TIDSTART, "DnD_Trade_Confirmed"))
		SetFont("LDTBOXCN");
	else if(IsValidBox(bid) && InventoryBoxLit[bid + boff] == BOXLIT_STATE_CLICK)
		SetFont("LDTBOXC");
	else if((temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source))) {
		// if lvl requirement is satisfied draw them normal -- topbox is +1 of actual index
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, temp - 1, -1, source) <= GetLevel())
			SetFont("LDTBOXO");
		else
			SetFont("LDTBOXR");
	}
	else
		SetFont("LDTBOX");
	HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - 1, CR_WHITE, basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * skip, basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * skip, 0.0);
}

void DrawInventoryInfo(int pnum) {
	int pn, mx, my, offset, stack = 0;
	
	if(CheckInventory("DnD_SelectedCharmBox"))
		DrawInventoryInfo_Field(pnum, CheckInventory("DnD_SelectedCharmBox") - 1, DND_SYNC_ITEMSOURCE_ITEMSUSED, 0, true);
		
	int itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, PlayerCursorData.itemHovered, -1, PlayerCursorData.itemHoveredSource);
	if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, PlayerCursorData.itemHovered, -1, PlayerCursorData.itemHoveredSource) != DND_ITEM_NULL) {
		DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 18 - ITEMINFOBG_MAXMIDS, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES);

		int isubt = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, PlayerCursorData.itemHovered, -1, PlayerCursorData.itemHoveredSource);
		
		mx = HUDMAX_XF - (PlayerCursorData.posx & MMASK) + 16.1;
		my = HUDMAX_YF - (PlayerCursorData.posy & MMASK) + 16.1;

		// to force them to appear in window
		if(PlayerCursorData.itemHoveredDim.x == HUDMAX_X) {
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
		
		int bg_x = mx;
		int bg_y = my;

		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		int attr_count = GetItemSyncValue(pnum, DND_SYNC_ITEMSATTRIBCOUNT, PlayerCursorData.itemHovered, -1, PlayerCursorData.itemHoveredSource);

		stack = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, PlayerCursorData.itemHovered, -1, PlayerCursorData.itemHoveredSource);
		if(stack) {
			SetFont("NMENUFNT");
			HudMessage(d:stack; HUDMSG_PLAIN, RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES - 14, CR_GREEN, GetIntegerBits(mx + HUD_ITEMBAK_XF - 9.0) + 0.2, my + 15.0, 0);
		}
		
		SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
		
		int prev_x = 3 * mx / 2;
		int prev_y = 3 * my / 2;
		
		mx = GetIntegerBits(3 * (mx + HUD_ITEMBAK_XF / 2) / 2) + 0.4;
		my = GetIntegerBits(3 * my / 2) + 20.1;
		
		SetHudClipRect(12 + (prev_x >> 16), 15 + (prev_y >> 16), HUD_ITEMBAK_WIDTH, 288, HUD_ITEMBAK_WIDTH);
		DrawInventoryText(
			PlayerCursorData.itemHovered,
			PlayerCursorData.itemHoveredSource, 
			pnum, mx, my, itype, isubt, RPGMENUINVENTORYID, HUD_DII_MULT, 
			HUDMAX_X, HUDMAX_Y, 
			bg_x, bg_y,
			attr_count
		);
		SetHudClipRect(0, 0, 0, 0, 0);
		SetHudSize(PlayerCursorData.itemHoveredDim.x, PlayerCursorData.itemHoveredDim.y, 1);
	}
}

// min is included max is excluded
void UpdateDraggedItemLitBoxes(int boxid, int min_box, int max_box, int pnum, int source) {
	// update the area being hovered wrt item being dragged
	int temp = PlayerCursorData.itemDragInfo.topboxid;
	--boxid;
	if(boxid == -1 || boxid >= max_box || boxid < min_box)
		return;

	if(temp != -1) {
		int idx;
		int s, p;

		int prev_boxid = boxid;
		if(!(prev_boxid % MAXINVENTORYBLOCKS_VERT) && temp - PlayerCursorData.itemDragInfo.click_box) {
			boxid = prev_boxid - (PlayerCursorData.itemDragInfo.size_y - 1) * MAXINVENTORYBLOCKS_VERT;
			if(boxid < min_box)
				boxid = min_box;
		}
		else {
			boxid += temp - PlayerCursorData.itemDragInfo.click_box;

			// adjust boxid so that we align the top left corner to be within our grid at all times
			// if we are at the beginning of a row, don't do it obviously, so we check mod for that
			// to detect beginning of a new line, we check if adding a size - 1 to it would make it go over nearest multiple of 9, if it would we stop it
			// this coincidentally also checks for upper bound, as the upper bound is always a multiple of 9 as well
			if(boxid < min_box) {
				boxid = min_box + (prev_boxid % MAXINVENTORYBLOCKS_VERT) - (PlayerCursorData.itemDragInfo.size_x - 1);
				if(boxid < min_box)
					boxid = min_box;
			}

			if(boxid % MAXINVENTORYBLOCKS_VERT) {			
				if(boxid + PlayerCursorData.itemDragInfo.size_x - 1 >= roundUp(boxid, MAXINVENTORYBLOCKS_VERT))
					boxid -= PlayerCursorData.itemDragInfo.size_x - 1;
			}
			
			if(boxid + (PlayerCursorData.itemDragInfo.size_y - 1) * MAXINVENTORYBLOCKS_VERT >= max_box) {
				temp = ((boxid + (PlayerCursorData.itemDragInfo.size_y - 1) * MAXINVENTORYBLOCKS_VERT) - max_box);
				if(!temp)
					temp = 1;
				boxid -= roundUp(temp, MAXINVENTORYBLOCKS_VERT);
			}
		}

		for(p = 0; p < PlayerCursorData.itemDragInfo.size_y; ++p) {
			for(s = 0; s < PlayerCursorData.itemDragInfo.size_x; ++s) {
				idx = boxid + s + p * MAXINVENTORYBLOCKS_VERT;
				
				// if this id isn't occupied by another item and its color scheme, color it
				if(GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, idx, -1, source) - 1 == -1) {
					InventoryBoxLit[idx] = BOXLIT_STATE_CURSORON;
				}
				else
					InventoryBoxLit[idx] = BOXLIT_STATE_BAD;
			}
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
		topboxid = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - offset, -1, source) - 1;
		
	bool hasItem = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source) - 1 == bid && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, bid, -1, source) != DND_ITEM_NULL;
	
	// click coloring
	if(prevclick != -1) {
		// we need offset taken here because data is in 0 - 44 cells
		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, prevclick - offset, -1, source) - 1;
		if(temp != -1) {
			// use topbox here
			ht = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, temp, -1, source);
			wt = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, temp, -1, source);
			// in the part below, use the real value of actual box clicked for highlighting
			for(p = 0; p < ht; ++p) {
				for(s = 0; s < wt; ++s) {
					//Log(s:"id ", d:temp + offset + s + p * MAXINVENTORYBLOCKS_VERT);
					InventoryBoxLit[temp + offset + s + p * MAXINVENTORYBLOCKS_VERT] = BOXLIT_STATE_CLICK;
				}
			}
			if(PlayerCursorData.itemDragInfo.click_box == -1) {
				PlayerCursorData.itemDragged = GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, temp, -1, source);
				PlayerCursorData.itemDragInfo.size_x = wt;
				PlayerCursorData.itemDragInfo.size_y = ht;
				PlayerCursorData.itemDragInfo.topboxid = temp;
				PlayerCursorData.itemDragInfo.source = source;
				PlayerCursorData.itemDragInfo.click_box = boxid - 1 - offset;
				//Log(s:"click box ", d:boxid - 1 - offset, s: " tpbxid ", d:temp);
			}
		}
		else if(((source & 0xFFFF) == DND_SYNC_ITEMSOURCE_STASH && CheckInventory("DnD_PlayerPreviousPage") == CheckInventory("DnD_PlayerCurrentPage")) || (source & 0xFFFF) != DND_SYNC_ITEMSOURCE_STASH)
			InventoryBoxLit[prevclick] = BOXLIT_STATE_CLICK;
	}
	else
		ResetCursorDragData();
	
	// highlight checking
	if(boxid - 1 == bid + offset) {
		// highlight the current box we hovered on
		InventoryBoxLit[boxid - 1] = BOXLIT_STATE_CURSORON;
		if(topboxid != -1)
			UpdateCursorHoverData(topboxid, source, 0, pnum, offset, dimx, dimy);
		else {
			if(PlayerCursorData.itemHovered != -1)
				PlayerCursorData.hoverNeedsReset = true;
			PlayerCursorData.itemHovered = -1;
		}
	}
	
	if(topboxid != -1) {
		ht = GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, topboxid, -1, source);
		wt = GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, topboxid, -1, source);
		// all boxes in range of this should be highlighted
		for(p = 0; p < ht; ++p) {
			for(s = 0; s < wt; ++s) {
				//printbold(s:"hl ", d:topboxid + s + p * MAXINVENTORYBLOCKS_VERT + offset);
				InventoryBoxLit[topboxid + s + p * MAXINVENTORYBLOCKS_VERT + offset] = BOXLIT_STATE_CURSORON;
			}
		}
	}
	
	// trade view or not, little hack to check dimensions
	DrawInventoryBlock(bh, bw, bid, hasItem, basex, basey, skip, idbase, source, pnum, offset, 32.0, 32.0);
}

void HandleInventoryView(int boxid) {
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	SetFont("LDTSCRN");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID, CR_WHITE, HUDMAX_XF / 2, HUDMAX_YF / 2, 0.0, 0.0);
	
	PlayerCursorData.itemDraggedStashSize = false;
	
	ResetInventoryLitState(0, MAX_INVENTORY_BOXES);
	
	//CleanInventoryInfo();
	int pnum = PlayerNumber();
	UpdateDraggedItemLitBoxes(boxid, 0, MAX_INVENTORY_BOXES, pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX, INVENTORYBOX_BASEY, 32.0, RPGMENUINVENTORYID, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, pnum, HUDMAX_X, HUDMAX_Y);
		}
	}
	
	SetFont("NMENUFNT");
}

// This is mainly used for stack editing
void HandleM2Inputs(int pnum, int boxid, int source, int seloffset, int prevsource) {
	int ipos, epos, bid;
	if(CheckInventory("DnD_SelectedInventoryBox")) {
		ipos = CheckInventory("DnD_SelectedInventoryBox") - 1 - seloffset;
		source = prevsource;
	}
	else
		ipos = boxid - 1;
	int istack = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos, -1, source);
	if(istack > 1) {
		// check for item space left for a new possible stack, if there isn't abort and popup
		//printbold(s:"slot for ", d:ipos, s: ": ", d:GetFreeSpotForItem(ipos, pnum, source, source));
		if(GetFreeSpotForItem(ipos, pnum, source, source) == -1) {
			ShowPopup(POPUP_NOTENOUGHSPACE, false, 0);
			return;
		}
		
		// sound of something happening
		LocalAmbientSound("RPG/MenuChoose", 127);
	
		// no previously selected box, just right click => halve a stack
		if(CheckInventory("DnD_SelectedInventoryBox")) {
			// always leave the odd one out on the new stack
			bid = istack / 2;
			epos = CloneItem(pnum, ipos, source, true);
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, epos, -1, bid, source);
			if(istack > 2 && (istack & 1))
				++bid;
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos, -1, bid, source);
			// create a new item of this type, halve the stack on it
			SyncItemData(pnum, epos, source, -1, -1);
		}
		else {
			// we need to have previously clicked on the item for this, then we take only one out of it
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos, -1, istack - 1, source);
			epos = CloneItem(pnum, ipos, source, true);
			SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, epos, -1, 1, source);
			SyncItemData(pnum, epos, source, -1, -1);
		}
		
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos, -1, source)) {
			// sync just stack as there are more left here
			SyncItemStack(pnum, ipos, source);
		}
		else {
			// sync the whole item after freeing
			FreeItem(pnum, ipos, source, false);
		}
	}
	SetInventory("DnD_SelectedInventoryBox", 0);
}

void HandleInventoryViewClicks(int pnum, int boxid, int choice) {
	int bid;
	int epos, ipos, temp;
	if(choice == DND_MENUINPUT_LCLICK) {
		if(boxid != MAINBOX_NONE) {
			// m1
			if(!CheckInventory("DnD_SelectedInventoryBox")) {
				SetInventory("DnD_SelectedInventoryBox", boxid);
			}
			else if(boxid != CheckInventory("DnD_SelectedInventoryBox")) {
				// if neither are empty spots
				bool boxidon = PlayerInventoryList[pnum][boxid - 1].item_type != DND_ITEM_NULL;
				bool prevselecton = PlayerInventoryList[pnum][CheckInventory("DnD_SelectedInventoryBox") - 1].item_type != DND_ITEM_NULL;
				if(boxidon && prevselecton) {
					// if both of them point to the same pointer, we need to move this item instead
					if(PlayerInventoryList[pnum][boxid - 1].topleftboxid == PlayerInventoryList[pnum][CheckInventory("DnD_SelectedInventoryBox") - 1].topleftboxid) {
						ipos = CheckInventory("DnD_SelectedInventoryBox") - 1;
						epos = boxid - 1;
						if(IsFreeSpot(pnum, ipos, epos))
							MoveItem(pnum, ipos, epos);
					}
					else
						SwapItems(pnum, boxid - 1, CheckInventory("DnD_SelectedInventoryBox") - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
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
					if((boxidon || prevselecton) && IsFreeSpot(pnum, ipos, epos))
						MoveItem(pnum, ipos, epos);
				}
				SetInventory("DnD_SelectedInventoryBox", 0);
			}
			else
				SetInventory("DnD_SelectedInventoryBox", 0);
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		else if(CheckInventory("DnD_SelectedInventoryBox") && (temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)) != DND_ITEM_NULL) {
			// drop selected item
			DropItemToField(pnum, CheckInventory("DnD_SelectedInventoryBox") - 1, true, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			ACS_NamedExecuteAlways("DnD Save Player Item Data", 0, pnum | (CheckInventory("DnD_CharacterID") << 16), CheckInventory("DnD_SelectedInventoryBox") - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			SetInventory("DnD_SelectedInventoryBox", 0);
			PlayItemDropSound(temp, true);
		}
	}
	else if(choice == DND_MENUINPUT_RCLICK)
		HandleM2Inputs(pnum, boxid, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void PlayItemDropSound(int type, bool use_activator_sound) {
	type &= 0xFFFF;

	str snd = "Items/Drop";
	if(type == DND_ITEM_BODYARMOR || type == DND_ITEM_BOOT || type == DND_ITEM_HELM)
		snd = "Items/ArmorEquip";
	else if(type == DND_ITEM_SPECIALTY_CYBORG)
		snd = "Items/PowercoreDrop";

	if(!use_activator_sound) {
		// this is the "unequip item" case, we should use this one as charms dont have their own "throw" sound
		if(type == DND_ITEM_CHARM)
			snd = "Items/CharmDrop";
		LocalAmbientSound(snd, 127);
	}
	else
		ActivatorSound(snd, 127);
}

// charm page controls
void HandleItemPageInputs(int pnum, int boxid) {
	int item_sel, item_type, item_subt = 0, topboxid;
	int temp;
	if(HasLeftClicked(pnum)) {
		if(boxid != MAINBOX_NONE) {
			// we pressed an item box
			if(!CheckInventory("DnD_InventoryView")) {
				// don't let user even click this if they aren't allowed
				if(boxid - 1 == POWERCORE_INDEX && !HasClassPerk("Cyborg", 2)) {
					ShowPopup(POPUP_CANTUSEPOWERCORE, false, 0);
					return;
				}

				GiveInventory("DnD_InventoryView", 1);
				LocalAmbientSound("RPG/MenuChoose", 127);
				if(boxid != DND_INV_ICON_BOXID)
					SetInventory("DnD_SelectedCharmBox", boxid);
			}
			else {
				// now track our choices made in the inventory view
				item_sel = CheckInventory("DnD_SelectedCharmBox");
				if(item_sel) {
					item_type = DND_ITEM_NULL;
					topboxid = PlayerInventoryList[pnum][boxid - 1].topleftboxid - 1;

					--item_sel;
					switch(item_sel) {
						case SMALLCHARM_INDEX1:
						case SMALLCHARM_INDEX2:
						case SMALLCHARM_INDEX3:
						case SMALLCHARM_INDEX4:
							item_type = DND_ITEM_CHARM;
							item_subt = DND_CHARM_SMALL;
						break;
						case MEDIUMCHARM_INDEX1:
						case MEDIUMCHARM_INDEX2:
							item_type = DND_ITEM_CHARM;
							item_subt = DND_CHARM_MEDIUM;
						break;
						case LARGECHARM_INDEX:
							item_type = DND_ITEM_CHARM;
							item_subt = DND_CHARM_LARGE;
						break;
						case HELM_INDEX:
							item_type = DND_ITEM_HELM;
						break;
						case BODY_ARMOR_INDEX:
							item_type = DND_ITEM_BODYARMOR;
						break;
						case POWERCORE_INDEX:
							item_type = DND_ITEM_SPECIALTY_CYBORG;
						break;
						case BOOT_INDEX:
							item_type = DND_ITEM_BOOT;
						break;
					}
					
					if(topboxid != -1 && (PlayerInventoryList[pnum][topboxid].item_type & 0xFFFF) == item_type) {
						// this returns popupid to show, and -1 if it was ok
						temp = MakeItemUsed(pnum, item_sel, topboxid, item_type, item_subt);
						if(temp == -1) {
							if(item_type == DND_ITEM_CHARM)
								LocalAmbientSound("Items/CharmDrop", 127);
							else if(item_type != DND_ITEM_SPECIALTY_CYBORG)
								LocalAmbientSound("Items/ArmorEquip", 127);
							else
								LocalAmbientSound("Items/PowercoreEquip", 127);
							TakeInventory("DnD_InventoryView", 1);
							SetInventory("DnD_SelectedCharmBox", 0);
							GiveInventory("DnD_CleanInventoryRequest", 1);
						}
						else
							ShowPopup(temp, false, 0);
					}
					else if(topboxid == -1)
						ShowPopup(POPUP_NOITEMTHERE, false, 0);
					else
						ShowPopup(POPUP_ITEMTYPEMISMATCH, false, 0);
				}
				else {
					// normal clicking functionality on inventory view
					// 1 is for m1
					HandleInventoryViewClicks(pnum, boxid, DND_MENUINPUT_LCLICK);
				}
			}
		}
		else {
			// we clicked outside bounds
			// clear previous selected charm box -- fixes issue of drawing the same charm on top left corner for comparison
			SetInventory("DnD_SelectedCharmBox", 0);
			if(CheckInventory("DnD_InventoryView")) {
				// ok we are in inventory view
				item_sel = CheckInventory("DnD_SelectedInventoryBox");
				if(item_sel) {
					// we have selected a box previously, if this has an item drop it otherwise clear it
					if((temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_sel - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)) != DND_ITEM_NULL) {
						// drop selected item
						DropItemToField(pnum, item_sel - 1, true, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
						ACS_NamedExecuteAlways("DnD Save Player Item Data", 0, pnum | (CheckInventory("DnD_CharacterID") << 16), item_sel - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
						PlayItemDropSound(temp, true);
					}
					// clear selection
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
				else { 
					// just exit if nothing was selected
					GiveInventory("DnD_CleanInventoryRequest", 1);
					TakeInventory("DnD_InventoryView", 1);
					LocalAmbientSound("RPG/MenuClose", 127);
				}
			}
		}
		ClearPlayerInput(pnum, true);
	}
	else if(HasRightClicked(pnum) && boxid != DND_INV_ICON_BOXID) {
		// mbox 8 is the view inventory button
		if(!CheckInventory("DnD_InventoryView") && boxid != MAINBOX_NONE && Items_Used[pnum][boxid - 1].item_type != DND_ITEM_NULL) {
			// try to drop item
			item_sel = GetFreeSpotForItem(boxid - 1, pnum, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			if(item_sel != -1) {
				PlayItemDropSound(Items_Used[pnum][boxid - 1].item_type, false);
				ApplyItemFeatures(pnum, boxid - 1, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE);
				MoveItemTrade(pnum, boxid - 1, item_sel, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				// force a damage cache recalc
				ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, pnum);
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
	
	// draw other player offering up top
	ResetInventoryLitState(0, 3 * MAX_INVENTORY_BOXES);
	UpdateDraggedItemLitBoxes(boxid, 0, 3 * MAX_INVENTORY_BOXES, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW);
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX_TRADEUP, INVENTORYBOX_BASEY_TRADEUP, 32.0, RPGMENUINVENTORYID - 4, 0, DND_SYNC_ITEMSOURCE_TRADEVIEW, GetTradee(), dimx, dimy);
		}
	}
	
	// draw our offering below
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX_TRADEDOWN, INVENTORYBOX_BASEY_TRADEDOWN, 32.0, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 6, MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_TRADEVIEW, pnum, dimx, dimy);
		}
	}
	// draw our inventory to the side
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, INVENTORYBOX_BASEX_TRADEMID, INVENTORYBOX_BASEY_TRADEMID, 32.0, RPGMENUINVENTORYID - 6 * MAX_INVENTORY_BOXES - 12, 2 * MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, pnum, dimx, dimy);
		}
	}
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("TRADEBAR");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 7, CR_WHITE, 110.4, 160.0, 0.0, 0.0);
	
	SetFont("NMENUFNT");
	--boxid;
	if(CheckInventory("DnD_Trade_ConfirmButtonPress"))
		HudMessage(s:"\cd", l:"DND_MENU_CONFIRM"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 10, CR_WHITE, 52.4, 160.0, 0.0, 0.0);
	else if(boxid == TRADECONFIRM_BOXID)
		HudMessage(s:"\c[M3]", l:"DND_MENU_CONFIRM"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 10, CR_WHITE, 52.4, 160.0, 0.0, 0.0);
	else
		HudMessage(l:"DND_MENU_CONFIRM"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 10, CR_WHITE, 52.4, 160.0, 0.0, 0.0);
	
	if(CheckInventory("DnD_Trade_CancelButtonPress"))
		HudMessage(s:"\cd", l:"DND_MENU_CANCEL"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 11, CR_WHITE, 168.4, 160.0, 0.0, 0.0);
	else if(boxid == TRADECANCEL_BOXID)
		HudMessage(s:"\c[M3]", l:"DND_MENU_CANCEL"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 11, CR_WHITE, 168.4, 160.0, 0.0, 0.0);
	else
		HudMessage(l:"DND_MENU_CANCEL"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 11, CR_WHITE, 168.4, 160.0, 0.0, 0.0);
	
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
	SetHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 1, CR_WHITE, 0.1, 120.0, 0.0, 0.0);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 2, CR_WHITE, 0.1, 360.0, 0.0, 0.0);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 3, CR_WHITE, 364.1, 260.0, 0.0, 0.0);
	SetFont("NMENUFNT");
	HudMessage(n:GetTradee() + 1, s:"\c[W3]", l:"DND_MENU_THEIROFFER"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 4, CR_WHITE, 160.4, 30.0, 0.0, 0.0);
	HudMessage(s:"\c[W3]", l:"DND_MENU_YOUROFFERING"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 5, CR_WHITE, 160.4, 270.0, 0.0, 0.0);
	HudMessage(s:"\c[W3]", l:"DND_MENU_YOURINV"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 6, CR_WHITE, 528.4, 170.0, 0.0, 0.0);
	
	HandleTradeBoxDraw(boxid, HUDMAX_X_STASH, HUDMAX_Y_STASH);
	
	PlayerCursorData.itemDraggedStashSize = true;
	
	SetFont("NMENUFNT");
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

// Just takes care of backend trade details
void CancelTrade(int pnum) {
	int tid = pnum + P_TIDSTART;
	ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
	GiveActorInventory(tid, "DnD_CleanTradeviewRequest", 1);
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
	for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			if(TradeViewList[pnum][bid].item_type != DND_ITEM_NULL) {
				int pos = GetFreeSpotForItem(bid, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				if(pos != -1)
					MoveItemTrade(pnum, bid, pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				else {
					// drop this item, just in case
					// for now, since this isn't very likely to happen, just log an error message
					Log(s:"Attempted to return item from trade view but no fitting slot found! Drop item.");
				}
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

// serverside
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
					// we pressed confirm button, make us confirm it and refresh other player's view
					GiveInventory("DnD_Trade_Confirmed", 1);
					GiveInventory("DnD_Trade_ConfirmButtonPress", 1);
					ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
					ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
						
					// if other side's confirmation is set, check for item space
					if(CheckActorInventory(bid + P_TIDSTART, "DnD_Trade_Confirmed")) {
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
					
						if(ok_from_us && ok_from_tradee) {
							// start countdown, when it's ready, proceed
							HandleTradeCountdown(bid, PlayerNumber());
						}
						else {
							// unconfirm the trade
							if(!ok_from_tradee) {
								SetActorInventory(bid + P_TIDSTART, "DnD_Trade_Confirmed", 0);
								ShowActorPopup(bid, POPUP_NOSPACEFORTRADE, false, 0);
								ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
								ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
							}
							else {
								SetInventory("DnD_Trade_Confirmed", 0);
								// show popup for not enough space
								ShowPopup(POPUP_NOSPACEFORTRADE, false, 0);
								ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
								ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
							}
						}
					}
				}
				else {
					// cancel confirmation
					TakeInventory("DnD_Trade_Confirmed", 1);
					TakeInventory("DnD_TradeSpaceFit", 1);
					TakeActorInventory(bid + P_TIDSTART, "DnD_Trade_Confirmed", 1);
					TakeActorInventory(bid + P_TIDSTART, "DnD_TradeSpaceFit", 1);

					ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
					ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
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
					if(!CheckInventory("DnD_SelectedInventoryBox")) {
						// jump click is to auto-dump hovered item to stash or vice versa
						if((GetPlayerInput(-1, INPUT_BUTTONS) & BT_JUMP) && !CheckInventory("DnD_AutoDumpCooldown")) {
							if(boxid > 2 * MAX_INVENTORY_BOXES) {
								isource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
								ioffset = 2 * MAX_INVENTORY_BOXES;

								ssource = DND_SYNC_ITEMSOURCE_TRADEVIEW;
								soffset = MAX_INVENTORY_BOXES;
							}
							else {
								isource = DND_SYNC_ITEMSOURCE_TRADEVIEW;
								ioffset = MAX_INVENTORY_BOXES;

								ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
								soffset = 2 * MAX_INVENTORY_BOXES;
							}

							// auto move code -- returns success if it could move
							if
							(
								GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL &&
								AutoMoveItem(pnum, boxid - ioffset - 1, isource, ssource)
							)
							{
								GiveInventory("DnD_AutoDumpCooldown", 1);
							}
						}
						else
							SetInventory("DnD_SelectedInventoryBox", boxid);
					}
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
						bool boxidon = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL;
						bool prevselecton = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource) != DND_ITEM_NULL;
						if(boxidon && prevselecton) {
							// if both of them point to the same pointer, we need to move this item instead
							if(isource == ssource && GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - ioffset, -1, isource) == GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource)) {
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
								if(IsFreeSpot(pnum, ipos - ioffset, epos - soffset, isource, ssource)) {
									MoveItemTrade(pnum, ipos - ioffset, epos - soffset, isource, ssource);
									ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
								}
							}
							else {
								SwapItems(pnum, boxid - 1 - ioffset, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, isource, ssource, false);
								ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
							}
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
							if((boxidon || prevselecton) && IsFreeSpot(pnum, ipos - ioffset, epos - soffset, isource, ssource)) {
								MoveItemTrade(pnum, ipos - ioffset, epos - soffset, isource, ssource);
								ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
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
					
					if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource) != DND_ITEM_NULL) {
						// drop selected item
						DropItemToField(pnum, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, true, ssource);
						
						// check if commenting this causes any issues, we already save player inventories when they spectate
						// ACS_NamedExecuteAlways("DnD Save Player Item Data", 0, pnum | (CheckInventory("DnD_CharacterID") << 16), CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, ssource);
						
						SetInventory("DnD_SelectedInventoryBox", 0);
						ActivatorSound("Items/Drop", 127);
					}
				}

				// make sure changes are reflected dynamically
				ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
				ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
			}
		}
	}
	else if(HasRightClicked(pnum) && boxid <= 3 * MAX_INVENTORY_BOXES && boxid != MAINBOX_NONE && !CheckInventory("DnD_Trade_Confirmed")) {
		// dont let player clutter up by spamming m2 while countdown is going
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
		HandleM2Inputs(pnum, boxid - ioffset, isource, soffset, ssource);

		// make sure changes are reflected dynamically
		ACS_NamedExecuteAlways("DnD Refresh Request", 0, pnum, 1);
		ACS_NamedExecuteAlways("DnD Refresh Request", 0, bid, 1);
	}
}

void DrawDraggedItem(int pnum) {
	SetFont(GetItemImage(PlayerCursorData.itemDragged, true));

	int scale = 1;
	int sx = PlayerCursorData.itemDragInfo.size_x;
	int itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, PlayerCursorData.itemDragInfo.topboxid, -1, PlayerCursorData.itemDragInfo.source);

	// adjust to be in center of cursor
	if(itype == DND_ITEM_BODYARMOR)
		scale = 0;
	else if(itype == DND_ITEM_BOOT)
		sx = 1;
	
	if(PlayerCursorData.itemDraggedStashSize) {
		SetHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH, 1);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCURSORID + 1, CR_WHITE, HUDMAX_XF_STASH - ((PlayerCursorData.posx * 3 / 2) & MMASK) - 16.0 * scale * (sx - 1) + 0.4, HUDMAX_YF_STASH - ((PlayerCursorData.posy * 3 / 2) & MMASK), 0.0, 0.0);
	}
	else
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCURSORID + 1, CR_WHITE, HUDMAX_XF - (PlayerCursorData.posx & MMASK) - 16.0 * scale * (sx - 1) + 0.4, HUDMAX_YF - (PlayerCursorData.posy & MMASK), 0.0, 0.0);
}

void HandleStashView(int boxid) {
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int pnum = PlayerNumber(), i, j;
	int curpages = CheckInventory("DnD_PlayerCurrentPage") - 1;
	int color;
	SetFont("LDTSCRN");
	SetHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 1, CR_WHITE, 452.4, 120.0, 0.0, 0.0);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 2, CR_WHITE, 452.4, 360.0, 0.0, 0.0);
	
	PlayerCursorData.itemDraggedStashSize = true;
	
	ResetInventoryLitState(0, 2 * MAX_INVENTORY_BOXES);
	UpdateDraggedItemLitBoxes(boxid, 0, 2 * MAX_INVENTORY_BOXES, pnum, DND_SYNC_ITEMSOURCE_STASH);
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
		SetFont("NMENUFNT");
		HudMessage(d:i + 1; HUDMSG_PLAIN, RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES - 1 - 2 * i, color, 388.4 + 15.0 * i, 210.0, 0.0, 0.0);
	}
	
	SetFont("NMENUFNT");
	HudMessage(s:"\c[W3]", l:"DND_MENU_HEAD_STASH"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 4, CR_WHITE, 452.4, 30.0, 0.0, 0.0);
	HudMessage(s:"\c[W3]", l:"DND_MENU_INVENTORY"; HUDMSG_PLAIN, RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 5, CR_WHITE, 452.4, 270.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void HandleStashViewClicks(int pnum, int boxid, int choice) {
	int temp = 0;
	int cpage, ppage, ssource, isource;
	int ioffset = 0;
	int soffset = 0;
	if(choice == DND_MENUINPUT_LCLICK) {
		int sel_box = CheckInventory("DnD_SelectedInventoryBox");
		//printbold(d:sel_box, s: " ", d:boxid);
		if(sel_box && boxid == MAINBOX_NONE) {
			// item dropping
			ppage = CheckInventory("DnD_PlayerPreviousPage") - 1;
			//printbold(s:"page ", d:ppage);
			ssource = DND_SYNC_ITEMSOURCE_STASH | (ppage << 16);
			soffset = 0;
			
			if(sel_box > MAX_INVENTORY_BOXES) {
				ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
				soffset = MAX_INVENTORY_BOXES;
			}
			
			if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, sel_box - 1 - soffset, -1, ssource) != DND_ITEM_NULL) {
				// drop selected item
				DropItemToField(pnum, sel_box - 1 - soffset, true, ssource);
				ACS_NamedExecuteAlways("DnD Save Player Item Data", 0, pnum | (CheckInventory("DnD_CharacterID") << 16), sel_box - 1 - soffset, ssource);
				SetInventory("DnD_SelectedInventoryBox", 0);
				ActivatorSound("Items/Drop", 127);
			}
		}
		else if(!sel_box && boxid < STASHBUTTON_BOXID_START) {
			//printbold(s:"set selected box to ", d:boxid);
			LocalAmbientSound("RPG/MenuChoose", 127);
			SetInventory("DnD_PlayerPreviousPage", CheckInventory("DnD_PlayerCurrentPage"));

			// strafe click is to auto-dump hovered item to stash or vice versa
			if(GetPlayerInput(-1, INPUT_BUTTONS) & BT_JUMP) {
				if(boxid > MAX_INVENTORY_BOXES) {
					isource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
					ioffset = MAX_INVENTORY_BOXES;

					ssource = DND_SYNC_ITEMSOURCE_STASH | ((CheckInventory("DnD_PlayerCurrentPage") - 1) << 16);
				}
				else {
					isource = DND_SYNC_ITEMSOURCE_STASH | ((CheckInventory("DnD_PlayerCurrentPage") - 1) << 16);

					ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
					soffset = MAX_INVENTORY_BOXES;
				}

				// auto move code -- returns success if it could move
				if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL)
					AutoMoveItem(pnum, boxid - ioffset - 1, isource, ssource);
			}
			else
				SetInventory("DnD_SelectedInventoryBox", boxid);
		}
		else if(boxid != sel_box) {
			if(boxid >= STASHBUTTON_BOXID_START) {
				// because fucking ACS is stupid somehow this is 2 when evaluated normally...
				temp = -(STASHBUTTON_BOXID_START - boxid);
				if(temp < CheckInventory("DnD_PlayerInventoryPages")) {
					GiveInventory(StrParam(s:"DnD_ButtonPress_", d:temp + 1), 1);
					//printbold(s:"set current page to ", d:temp + 1);
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
				
				if(sel_box > MAX_INVENTORY_BOXES) {
					ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
					soffset = MAX_INVENTORY_BOXES;
				}
				
				// if neither are empty spots
				int epos, ipos;
				bool boxidon = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL;
				bool prevselecton = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, sel_box - 1 - soffset, -1, ssource) != DND_ITEM_NULL;
				if(boxidon && prevselecton) {
					// if both of them point to the same pointer, we need to move this item instead
					if(isource == ssource && GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - ioffset, -1, isource) == GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource)) {
						ipos = sel_box - 1;
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
						if(IsFreeSpot(pnum, ipos - ioffset, epos - soffset, isource, ssource))
							MoveItemTrade(pnum, ipos - ioffset, epos - soffset, isource, ssource);
					}
					else {
						//printbold(s:"swapping");
						SwapItems(pnum, boxid - 1 - ioffset, sel_box - 1 - soffset, isource, ssource, false);
					}
				}
				else {
					// find which one has an item, and move it
					if(!boxidon) {
						epos = boxid - 1;
						ipos = sel_box - 1;
						// swap because the ipos place is changed
						temp = ssource;
						ssource = isource;
						isource = temp;
						temp = soffset;
						soffset = ioffset;
						ioffset = temp;
					}
					else {
						epos = sel_box - 1;
						ipos = boxid - 1;
					}
					// epos holds the empty position now
					// make sure we aren't both empty slots
					if((boxidon || prevselecton) && IsFreeSpot(pnum, ipos - ioffset, epos - soffset, isource, ssource))
						MoveItemTrade(pnum, ipos - ioffset, epos - soffset, isource, ssource);
				}
				//printbold(s:"reset at mainbox != none");
				SetInventory("DnD_SelectedInventoryBox", 0);
				LocalAmbientSound("RPG/MenuChoose", 127);
			}
		}
		else {
			//printbold(s:"reset selected box");
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
		
		HandleM2Inputs(pnum, boxid - ioffset, isource, soffset, ssource);
	}
}

void HandleMaterialDraw(menu_inventory_T module& p, int boxid, int curopt, int k) {
	int mcount = CountCraftingMaterials(IsCraftingPageForTokens(curopt));
	int i;
	int tx, ty, bx, by;
	int page = CheckInventory("DnD_Crafting_MaterialPage");
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int pnum = PlayerNumber();

	if(mcount) {
		if(!k || CheckInventory("DnD_RefreshPane")) {
			// add predefined size boxes for use
			for(i = 0; i < MAX_CRAFTING_MATERIALBOXES && i < mcount - MAX_CRAFTING_MATERIALBOXES * page; ++i) {
				tx = CRAFTING_MATERIALBOX_X - (32.0 + CRAFTING_MATERIALBOX_SKIPX) * (i % 2);
				ty = CRAFTING_MATERIALBOX_Y - (32.0 + CRAFTING_MATERIALBOX_SKIPY) * (i / 2);
				bx = tx - 32.0;
				by = ty - 32.0;
				EnableBoxWithPoints(p, i + MATERIALBOX_OFFSET_BOXID, tx, ty, bx, by);
			}

			// clear the previously displayed material boxes potentially
			for(i = 0; i < MAX_CRAFTING_MATERIALBOXES; ++i) {
				HudMessage(s:""; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i, CR_CYAN, 404.0, 72.0, 0.0);
				HudMessage(s:""; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i - 1, CR_CYAN, 404.0, 72.0, 0.0);
				HudMessage(s:""; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i - 2, CR_CYAN, 404.0, 72.0, 0.0);
			}
		}
		i = 0;
		//for(ty = 0; ty < MAX_CRAFTITEMTYPES; ++ty) {
			// by holds currently visited unique item's order, it's unique to each item type so we reset it, we don't reset i
			for(by = 0; i < MAX_CRAFTING_MATERIALBOXES && i < mcount - MAX_CRAFTING_MATERIALBOXES * page; ++i, ++by) {
				tx = GetNextUniqueCraftableMaterial(by + MAX_CRAFTING_MATERIALBOXES * page, IsCraftingPageForTokens(curopt));
				if(tx != -1) {
					if(boxid - 1 == MATERIALBOX_OFFSET_BOXID + i) {
						//Log(s:"update item boxlit material ", d:tx);
						UpdateCursorHoverData(tx, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][tx].item_type, pnum, 0);

						MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK1;
						MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] |= tx << DND_MENU_ITEMSAVEBITS1;
						//SetInventory("DnD_PlayerItemIndex", tx);
						SetFont("CRFBX_H2");
					}
					else if(prevclick == MATERIALBOX_OFFSET_BOXID + i) {
						//Log(s:"update prev item in material ", d:tx);
						//SetInventory("DnD_PlayerPrevItemIndex", tx);
						MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK2;
						MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] |= tx << DND_MENU_ITEMSAVEBITS2;
						SetFont("CRFBX_H2");
					}
					else
						SetFont("CRFBX_N2");
					HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i, CR_CYAN, 404.0 + 44.0 * (i % 2), 72.0 + 36.0 * (i / 2), 0.0);
					SetFont(GetItemImage(PlayerInventoryList[pnum][tx].item_image));
					HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i - 1, CR_CYAN, 404.0 + 44.0 * (i % 2), 72.0 + 36.0 * (i / 2), 0.0);
					bx = GetTotalStackOfMaterial(tx);
					if(bx > 0) {
						SetFont("NMENUFNT");
						HudMessage(d:bx; HUDMSG_PLAIN, RPGMENUID - MATERIALBOX_OFFSET - 3 * i - 2, CR_WHITE, 412.0 + 44.0 * (i % 2), 80.0 + 36.0 * (i / 2), 0.0);
					}
				}
				else
					break;
			}
		//}
		SetFont("NMENUFNT");
		HudMessage(s:"P", d:page + 1; HUDMSG_PLAIN, RPGMENUID - MATERIALARROW_HUDID - 2, CR_WHITE, 430.4, 284.0, 0.0);
		// draw next page button and enable it for use
		if(mcount > MAX_CRAFTING_MATERIALBOXES) {
			if(page) {
				// remember to send -1 of the actual box here
				EnableBoxWithPoints(p, MATERIALARROW_ID - 1, CRAFTING_LARR_X, CRAFTING_LARR_Y, CRAFTING_LARR_X - CRAFTING_ARROW_X_SKIP, CRAFTING_LARR_Y - CRAFTING_ARROW_Y_SKIP);
				DrawBoxText("<=", DND_NOLOOKUP, boxid, MATERIALARROW_ID, RPGMENUID - MATERIALARROW_HUDID - 1, 388.0, 284.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, MATERIALARROW_ID - 1);
			if(mcount - MAX_CRAFTING_MATERIALBOXES * (page + 1) > 0) {
				EnableBoxWithPoints(p, MATERIALARROW_ID, CRAFTING_RARR_X, CRAFTING_LARR_Y, CRAFTING_RARR_X - CRAFTING_ARROW_X_SKIP, CRAFTING_LARR_Y - CRAFTING_ARROW_Y_SKIP);
				DrawBoxText("=>", DND_NOLOOKUP, boxid, MATERIALARROW_ID + 1, RPGMENUID - MATERIALARROW_HUDID, 464.0, 284.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, MATERIALARROW_ID);
		}
	}
	SetFont("NMENUFNT");
}

void HandleCraftingWeaponDraw(int pnum, menu_inventory_T module& p, int boxid, int k) {
	int i, j = 0, mcount = GetWeaponCount();
	int tx, ty, bx, by;
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int page = CheckInventory("DnD_Crafting_ItemPage");
	
	if(mcount) {
		if(!k || CheckInventory("DnD_RefreshPane")) {
			// add predefined size boxes for use
			for(i = 0; i < MAX_CRAFTING_ITEMBOXES/* * (page + 1)*/ && i < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
				tx = CRAFTING_ITEMBOX_X - (64.0 + CRAFTING_ITEMBOX_SKIPX) * (i % 4);
				ty = CRAFTING_ITEMBOX_Y - (64.0 + CRAFTING_ITEMBOX_SKIPY) * (i / 4);
				bx = tx - 64.0;
				by = ty - 64.0;
				EnableBoxWithPoints(p, i, tx, ty, bx, by);
			}
		}
		// first count over the weapons we must skip
		for(i = 0; i < MAXWEPS && j < MAX_CRAFTING_ITEMBOXES * page; ++i)
			if(CheckInventory(Weapons_Data[i].name))
				++j;
		//if(j)
		//	++i;
		j = 0;
		// i will count onwards from here
		for(; i < MAXWEPS && j < MAX_CRAFTING_ITEMBOXES && j < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
			if(CheckInventory(Weapons_Data[i].name)) {
				if(boxid - 1 == j) {
					//Log(s:"update item boxlit ", d:i);
					UpdateCursorHoverData(i, 0, DND_ITEM_WEAPON, 0, pnum, 0);
					
					MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK1;
					MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] |= i << DND_MENU_ITEMSAVEBITS1;
					SetFont("CRFBX_H");
				}
				else if(prevclick == j) {
					//Log(s:"update prev item ", d:i);
					MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK2;
					MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] |= i << DND_MENU_ITEMSAVEBITS2;
					SetFont("CRFBX_H");
				}
				else
					SetFont("CRFBX_N");
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 5 - 3 * j, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (j % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (j / 4), 0.0, 0.0);
				SetFont(Weapons_Data[i].icon);
				HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 6 - 3 * j, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (j % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (j / 4), 0.0, 0.0);
				++j;
			}
		}
		SetFont("NMENUFNT");
		// draw next page button and enable it for use
		if(mcount > MAX_CRAFTING_ITEMBOXES) {
			if(mcount - MAX_CRAFTING_ITEMBOXES * (page + 1) > 0) {
				EnableBoxWithPoints(p, CRAFTING_PAGEARROW_ID, CRAFTING_PAGEARROWR_X, CRAFTING_PAGEARROWL_Y, CRAFTING_PAGEARROWR_X - CRAFTING_PAGEARROW_XSIZE, CRAFTING_PAGEARROWL_Y - CRAFTING_PAGEARROW_YSIZE);
				DrawBoxText("=>", DND_NOLOOKUP, boxid, CRAFTING_PAGEARROW_ID + 1, RPGMENUID - 4, 340.1, 288.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, CRAFTING_PAGEARROW_ID + 1);
		}
	}
	SetFont("NMENUFNT");
}

void HandleCraftingInventoryDraw(int pnum, menu_inventory_T module& p, int boxid, int k) {
	int i, mcount = 0;
	int tx, ty;
	
	// construct fast lookup array
	static int craftable_materials[MAX_INVENTORY_BOXES];
	for(i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		// if this a craftable item, include in list
		if(IsCraftableItem(PlayerInventoryList[pnum][i].item_type) && PlayerInventoryList[pnum][i].height) {
			craftable_materials[mcount] = i;
			++mcount;
		}
	}

	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int page = CheckInventory("DnD_Crafting_ItemPage");
	if(mcount) {
		if(!k || CheckInventory("DnD_RefreshPane")) {
			// add predefined size boxes for use
			for(i = 0; i < MAX_CRAFTING_ITEMBOXES/* * (page + 1)*/ && i < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
				tx = CRAFTING_ITEMBOX_X - (64.0 + CRAFTING_ITEMBOX_SKIPX) * (i % 4);
				ty = CRAFTING_ITEMBOX_Y - (64.0 + CRAFTING_ITEMBOX_SKIPY) * (i / 4);
				EnableBoxWithPoints(p, i, tx, ty, tx - 64.0, ty - 64.0);
			}
		}
		
		// j holds how many craftable materials there are, and the value stored in that pos of array is itemid
		bool click_reset = true;
		for(i = 0; i < MAX_CRAFTING_ITEMBOXES && i < mcount - MAX_CRAFTING_ITEMBOXES * page; ++i) {
			// store id into this for ease -- move forward page x box times to skip
			tx = craftable_materials[i + MAX_CRAFTING_ITEMBOXES * page];
			// give priority to clicking over hovering
			if(prevclick == i) {
				UpdateCursorClickData(tx, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][tx].item_type, PlayerCursorData.posx, PlayerCursorData.posy);
				click_reset = false;
			
				//Log(s:"update prev item inv ", d:tx);
				//SetInventory("DnD_PlayerPrevItemIndex", tx);
				MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK2;
				MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] |= tx << DND_MENU_ITEMSAVEBITS2;
				SetFont("CRFBX_H");
			}
			else if(boxid - 1 == i) {
				UpdateCursorHoverData(tx, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, PlayerInventoryList[pnum][tx].item_type, pnum, 0);
				
				//Log(s:"update cur item inv ", d:tx);
				MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] &= DND_MENU_ITEMCLEARMASK1;
				MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] |= tx << DND_MENU_ITEMSAVEBITS1;
				//SetInventory("DnD_PlayerItemIndex", tx);
				SetFont("CRFBX_H");
			}
			else
				SetFont("CRFBX_N");
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 5 - 3 * i, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (i % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (i / 4), 0.0, 0.0);
			SetFont(GetItemImage(PlayerInventoryList[pnum][tx].item_image));
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 6 - 3 * i, CR_CYAN, CRAFTING_WEAPONBOXDRAW_X + 76.0 * (i % 4), CRAFTING_WEAPONBOXDRAW_Y + 68.0 * (i / 4), 0.0, 0.0);
		}
		
		PlayerCursorData.clickNeedsReset = click_reset;
		
		SetFont("NMENUFNT");
		// draw next page button and enable it for use
		if(mcount > MAX_CRAFTING_ITEMBOXES) {
			if(mcount - MAX_CRAFTING_ITEMBOXES * (page + 1) > 0) {
				EnableBoxWithPoints(p, CRAFTING_PAGEARROW_ID, CRAFTING_PAGEARROWR_X, CRAFTING_PAGEARROWL_Y, CRAFTING_PAGEARROWR_X - CRAFTING_PAGEARROW_XSIZE, CRAFTING_PAGEARROWL_Y - CRAFTING_PAGEARROW_YSIZE);
				DrawBoxText("=>", DND_NOLOOKUP, boxid, CRAFTING_PAGEARROW_ID + 1, RPGMENUID - 1, 340.1, 288.0, "\c[B1]", "\c[Y5]");
			}
			else
				DisableBoxInPane(p, CRAFTING_PAGEARROW_ID);
		}
	}
	SetFont("NMENUFNT");
}

void ResetTransmutingData(int pnum) {
	for(int i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		TransmuteOrbs[pnum][i] = -1;
		ACS_NamedExecuteWithResult("DnD Transmute Orb Sync", pnum, i, -1);
	}
}

int IsValidTransmuteRecipe(int pnum) {
	// check the orbs now for valid recipes
	static int orb_counts[MAX_ORBS];
	int res = -1, i, j;
	int overall_count = 0;

	for(i = 0; i < MAX_ORBS; ++i)
		orb_counts[i] = 0;

	// get count of all orbs in the transmute chamber
	for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		j = TransmuteOrbs[pnum][i];
		if(j != -1) {
			++orb_counts[j];
			++overall_count;
		}
	}

	// recipe verification... could honestly be a lot better if stored in an array, but maybe wasteful in resources too, idk... mods kinda tight with array usage
	// besides, I feel like I'd have to use a tree to really handle this properly... I think? I haven't thought too hard on making it efficiently, hopefully wont have to
	for(i = 0; i < DND_MAX_ORB_KINDS; ++i) {
		auto recipe = GetOrbRecipe(i);
		int running_count = 0;
		bool goodCount = false;
		for(j = 0; j < MAX_TRANSMUTE_BOXES; ++j) {
			if(recipe.components[j].id != -1) {
				goodCount = orb_counts[recipe.components[j].id] == recipe.components[j].count;
				if(goodCount)
					running_count += recipe.components[j].count;
				else // don't look further, we won't match, quit this loop
					break;
			}
		}

		if(goodCount && overall_count && running_count == overall_count) {
			res = i;
			break;
		}
	}

	if(res == -1) {
		// check special recipes, like the 420 one for TURMOIL + tagged orb => another random tagged orb
		if
		(
			orb_counts[DND_ORB_TURMOIL] == 1 && overall_count == 2 &&
			(
				orb_counts[DND_ORB_PRISMATIC] || orb_counts[DND_ORB_DESTRUCTION] || orb_counts[DND_ORB_VIOLENCE] ||
				orb_counts[DND_ORB_FORTITUDE] || orb_counts[DND_ORB_TREMORS] || orb_counts[DND_ORB_TINKERER] ||
				orb_counts[DND_ORB_HEXES] || orb_counts[DND_ORB_GROWTH] || orb_counts[DND_ORB_CRACKLING] ||
				orb_counts[DND_ORB_BRUTE] || orb_counts[DND_ORB_JAGGED] || orb_counts[DND_ORB_SAVAGERY] || orb_counts[DND_ORB_PROSPERITY]
			)
		) 
		{
			res = DND_RANDOM_TAGGED_ORB_TRANSMUTE; // random tagged orb
		}
		else if // use XOR to see if an odd amount of orbs are there
		(
			overall_count == 3 && (
				(orb_counts[DND_ORB_WINTER] == 1) + (orb_counts[DND_ORB_VOLTAIC] == 1) + (orb_counts[DND_ORB_VILE] == 1) + (orb_counts[DND_ORB_EMBERS] == 1) == overall_count
			)
		)
		{
			res = DND_ELEMENTAL_TO_PRISMATIC;
		}
	}

	return res;
}

void HandleTransmutingDraw(int pnum, menu_inventory_T module& p, int boxid, int k) {
	// first three are the 3 slots for orbs, 4th is transmute button
	EnableBoxWithPoints(p, 0, 392.0, 148.0, 352.0, 108.0);
	EnableBoxWithPoints(p, 1, 312.0, 276.0, 272.0, 236.0);
	EnableBoxWithPoints(p, 2, 232.0, 148.0, 192.0, 108.0);
	EnableBoxWithPoints(p, 3, 334.0, 68.0, 250.0, 60.0);

	// we have three total boxes
	int i;
	int id_offset = 7;
	for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		str borderpic = "SCHNOR";
		int thisboxid = MBOX_1 + i;
		bool isHovered = boxid == thisboxid && !CheckInventory("DnD_InventoryView");
		if(isHovered)
			borderpic = "SCHSEL";

		int hudx = 108.4 + i * 80.0;
		int hudy = 128.0 + 64.0 * (1 - 2 * (i % 2));
			
		// if there is an orb here, draw it
		int o_id = TransmuteOrbs[pnum][i];
		if(o_id != -1) {
			SetFont(GetItemImage(o_id + ITEM_IMAGE_ORB_BEGIN));
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 2 * thisboxid - id_offset, CR_WHITE, hudx, hudy, 0.0, 0.0);
			
			if(isHovered)
				UpdateCursorHoverData(thisboxid - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0, pnum, 0, HUDMAX_X, HUDMAX_Y);
		}
		else {
			SetFont("ORBBAK");
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 2 * thisboxid - id_offset, CR_WHITE, hudx, hudy, 0.0, 0.0);
		}
		SetFont(borderpic);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 2 * thisboxid - id_offset + 1, CR_WHITE, hudx, hudy, 0.0, 0.0);
	}

	// middle orb -- hudx + offset, hudy - offset / 2
	int recipe_out = IsValidTransmuteRecipe(pnum);
	if(recipe_out == -1) {
		SetFont("ORBMBK");
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 6, CR_WHITE, 188.4, 140.0, 0.0, 0.0);

		SetFont("NMENUFNT");
		DrawBoxText("DND_TRANSMUTE", DND_LANGUAGE_LOOKUP, boxid, MBOX_4, RPGMENUID - 7, 188.4, 256.0, "\c[B1]", "\c[G8]");
	}
	else {
		SetFont("ORBMBKG");
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 6, CR_WHITE, 188.4, 140.0, 0.0, 0.0);

		if(recipe_out != DND_RANDOM_TAGGED_ORB_TRANSMUTE && recipe_out != DND_ELEMENTAL_TO_PRISMATIC) {
			SetFont(
				GetItemImage(
					GetOrbItemImage(recipe_out)
				)
			);
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 7, CR_WHITE, 188.4, 140.0, 0.0, 0.0);

			SetFont("NMENUFNT");
			recipe_out = ORBS_BEGIN + recipe_out;
			HudMessage(s:"\c[Y5]", l:GetInventoryTag(recipe_out); HUDMSG_PLAIN, RPGMENUID - 16, CR_WHITE, 188.4, 180.0, 0.0, 0.0);
		}
		else if(recipe_out == DND_ELEMENTAL_TO_PRISMATIC) {
			SetFont(GetItemImage(ITEM_IMAGE_ORB_BEGIN + DND_ORB_PRISMATIC));
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 7, CR_WHITE, 188.4, 140.0, 0.0, 0.0);

			SetFont("NMENUFNT");
			recipe_out = ORBS_BEGIN + DND_ORB_PRISMATIC;
			HudMessage(s:"\c[Y5]", l:GetInventoryTag(recipe_out); HUDMSG_PLAIN, RPGMENUID - 16, CR_WHITE, 188.4, 180.0, 0.0, 0.0);
		}
		else {
			// random tagged orb
			SetFont("ORBBAKQ");
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID - 7, CR_WHITE, 188.4, 140.0, 0.0, 0.0);

			SetFont("NMENUFNT");
			HudMessage(s:"\c[Y5]", l:"DND_RANDOM_TAGGED_ORB"; HUDMSG_PLAIN, RPGMENUID - 16, CR_WHITE, 188.4, 180.0, 0.0, 0.0);
		}

		DrawBoxText("DND_TRANSMUTE", DND_LANGUAGE_LOOKUP, boxid, MBOX_4, RPGMENUID - 5, 188.4, 256.0, "\c[B1]", "\c[Y5]");
	}

	SetFont("NMENUFNT");
}

void DrawCraftingInventoryInfo(int pn, int id_begin, int x, int y, int item_id, int item_type, int item_source) {
	int mx, my;
	
	mx = HUDMAX_XF - (x & MMASK) + 16.1;
	my = HUDMAX_YF - (y & MMASK) + 16.1;

	// to force them to appear in window
	if(mx > INVENTORYINFO_NORMALVIEW_WRAPX)
		mx = INVENTORYINFO_NORMALVIEW_WRAPX + 0.1;
	if(my > INVENTORYINFO_NORMALVIEW_WRAPY)
		my = INVENTORYINFO_NORMALVIEW_WRAPY + 0.1;

	DeleteTextRange(id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 18, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES);

	int bg_x = mx;
	int bg_y = my;
		
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	int attr_count = GetItemSyncValue(pn, DND_SYNC_ITEMSATTRIBCOUNT, item_id, -1, item_source);
	
	if(GetStackValue(item_type) && item_source) {
		SetFont("NMENUFNT");
		HudMessage(d:GetTotalStackOfMaterial(item_id); HUDMSG_PLAIN, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 14, CR_GREEN, mx + 200.1, my + 16.0, 0.0);
	}
	
	int prev_x = GetIntegerBits(mx + HUD_ITEMBAK_XF / 2) + 0.4;
	int prev_y;
	
	// show item details -- dont draw charm icons (saves space)
	/*if(item_type == DND_ITEM_WEAPON) {
		SetFont(Weapons_Data[item_id].icon);	
		HudMessage(s:"A"; HUDMSG_PLAIN, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 1, CR_WHITE, prev_x, my, 0.0);
	}
	else if(PlayerCursorData.itemHoveredType != DND_ITEM_CHARM && PlayerCursorData.itemHoveredType < UNIQUE_BEGIN) {
		// dont draw charm images here
		SetFont(Item_Images[PlayerInventoryList[pn][PlayerCursorData.itemHovered].item_image]);
		HudMessage(s:"A"; HUDMSG_PLAIN, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 1, CR_WHITE, prev_x, my + 16.0, 0.0);
	}*/
	
	SetHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2, 1);
	
	prev_x = 3 * mx / 2;
	prev_y = 3 * my / 2;
	
	mx = GetIntegerBits(3 * (mx + HUD_ITEMBAK_XF / 2) / 2) + 0.4;
	my = GetIntegerBits(3 * my / 2) + 20.1;
	
	SetHudClipRect(12 + (prev_x >> 16), 15 + (prev_y >> 16), HUD_ITEMBAK_WIDTH, 288, HUD_ITEMBAK_WIDTH);
	DrawCraftingInventoryText(item_type, item_id, item_source, pn, id_begin, mx, my, HUDMAX_X, HUDMAX_Y, bg_x, bg_y, attr_count);
	SetHudClipRect(0, 0, 0, 0, 0);
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

// extra1 is topboxid for items, extra2 works as source for inventory related things (used charms vs player inventory etc.)
void DrawCraftingInventoryText(int itype, int extra1, int extra2, int pnum, int id_begin, int mx, int my, int hx, int hy, int bg_x, int bg_y, int attr_count) {
	if(itype == DND_ITEM_WEAPON) {
		int j, temp;
		j = PlayerNumber();
		my += 24.0;
		
		SetFont("NMENUFNT");
		HudMessage(s:"\c[R5]", l:GetWeaponTag(extra1), s:":\c- \c[Y5]", l:"DND_MENU_SLOT", s:" - ", d:GetGameSlotOfWeapon(extra1); HUDMSG_PLAIN, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3 - ITEMINFOBG_MAXMIDS, CR_WHITE, mx, my - 24.0, 0.0, 0.0);
		
		str modText = "";
		temp = GetPlayerWeaponQuality(j, extra1);
		// make sure quality text isn't on the weapon name
		if(temp) {
			if (temp == MAXWEPQUALITY) // Add the "MAX" indicator.
				modText = StrParam(s:"\c[Y5]* ", l:"WEPMOD_TEXT1", s:": \c[Q9]+", d:temp, s:"% ", l:"DND_MENU_MAX", s:"\n");
			else
				modText = StrParam(s:"\c[Y5]* ", l:"WEPMOD_TEXT1", s:": \c[Q9]+", d:temp, s:"% \n");
		}
		temp = GetCritChance(j, -1, extra1);
		if(temp)
			modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_CRIT), s:"\n");
		else {
			// this is only triggered if we have % crit chance increases and the weapon itself shows no base crit chance
			temp = GetPercentCritChanceIncrease(j, extra1);
			if(temp)
				modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_CRITPERCENT), s:"\n");
		}

		// itype isnt used from here on
		temp = GetBaseCritModifier(j, extra1);
		if(temp != DND_BASE_CRITMODIFIER)
			modText = StrParam(s:modText, s:GetWeaponModText(temp - DND_BASE_CRITMODIFIER, WEP_MOD_CRITDMG), s:"\n");
			
		temp = GetWeaponModValue(j, extra1, WEP_MOD_DMG);
		if(temp)
			modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_DMG), s:"\n");
		
		// print weapon mods -- generalize later idc at this time about how neat it is
		temp = GetWeaponModValue(j, extra1, WEP_MOD_PERCENTDAMAGE);
		if(temp)
			modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_PERCENTDAMAGE), s:"\n");
		
		temp = GetWeaponModValue(j, extra1, WEP_MOD_POISONFORPERCENTDAMAGE);
		if(temp)
			modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_POISONFORPERCENTDAMAGE), s:"\n");
		
		temp = GetWeaponModValue(j, extra1, WEP_MOD_FORCEPAINCHANCE);
		if(temp)
			modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_FORCEPAINCHANCE), s:"\n");
		
		for(temp = 0; temp < MAX_WEP_MOD_POWERSET1; ++temp) {
			if(HasWeaponPower(j, extra1, temp))
				modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_POWERSET1, temp), s:"\n");
		}
		
		HudMessage(s:modText; HUDMSG_PLAIN, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 - ITEMINFOBG_MAXMIDS, CR_WHITE, mx, my - 8.0, 0.0, 0.0);

		DrawItemInfoBackground(id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES, hx, hy, bg_x, bg_y, CountNewLinesInText(modText, NEXT_LINE_LEN_ATTR + 8));
	}
	else // draw using our established drawing routine
		DrawInventoryText(extra1, extra2, pnum, mx, my, itype, GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, extra1, -1, extra2), id_begin, HUD_DII_MULT, hx, hy, bg_x, bg_y, attr_count);
}

void HandleCraftingView(int pnum, menu_inventory_T module& p, int boxid, int curopt, int k) {
	int i;
	HudMessage(s:"\c[Y5]", l:"DND_MENU_MATERIALS"; HUDMSG_PLAIN, RPGMENUID - 2, CR_CYAN, 424.0, 32.0, 0.0, 0.0);
	
	// load rectangles
	if(!k || CheckInventory("DnD_RefreshPane")) {
		ResetInventoryPane(p);

		LoadCraftingView(p);

		EnableBoxWithPoints(p, CRAFTING_PAGEARROW_ID - 1, CRAFTING_PAGEARROWL_X, CRAFTING_PAGEARROWL_Y, CRAFTING_PAGEARROWL_X - CRAFTING_PAGEARROW_XSIZE, CRAFTING_PAGEARROWL_Y - CRAFTING_PAGEARROW_YSIZE);
	
		// these were lingering
		if(curopt != MENU_LOAD_CRAFTING)
			DeleteTextRange(RPGMENUID - 5, RPGMENUID - 3);
		else {
			// enable these two only for crafting view
			// weapon crafting box
			EnableBoxWithPoints(p, CRAFTING_WEAPON_BOXID, 455.0, 292.0, 321.0, 285.0);
			// inventory crafting box
			EnableBoxWithPoints(p, CRAFTING_INVENTORY_BOXID, 455.0, 276.0, 310.0, 269.0);
			// transmuting box
			EnableBoxWithPoints(p, CRAFTING_TRANSMUTING_BOXID, 455.0, 260.0, 310.0, 253.0);
		}

		HandleMaterialDraw(p, boxid, curopt, k);
	}
	
	// CleanInventoryInfo();
	if(curopt == MENU_LOAD_CRAFTING) {
		DrawBoxText("<=", DND_NOLOOKUP, boxid, CRAFTING_PAGEARROW_ID, RPGMENUID - 6, 16.1, 288.0, "\c[B1]", "\c[Y5]");
		DrawBoxText("DND_MENU_WEPCRAFT", DND_LANGUAGE_LOOKUP, boxid, CRAFTING_WEAPON_BOXID + 1, RPGMENUID - 3, 24.1, 32.0, "\c[B1]", "\c[Y5]");
		DrawBoxText("DND_MENU_INVCRAFT", DND_LANGUAGE_LOOKUP, boxid, CRAFTING_INVENTORY_BOXID + 1, RPGMENUID - 4, 24.1, 48.0, "\c[B1]", "\c[Y5]");

		if(CheckResearchStatus(RES_TRANSMUTING) != RES_DONE)
			DrawBoxText("???", DND_NOLOOKUP, boxid, CRAFTING_TRANSMUTING_BOXID + 1, RPGMENUID - 5, 24.1, 64.0, "\c[B1]", "\c[Y5]");
		else
			DrawBoxText("DND_MENU_TRANSMUTE", DND_LANGUAGE_LOOKUP, boxid, CRAFTING_TRANSMUTING_BOXID + 1, RPGMENUID - 5, 24.1, 64.0, "\c[B1]", "\c[Y5]");
	}
	else if(curopt == MENU_LOAD_CRAFTING_WEAPON) {
		DrawBoxText("<=", DND_NOLOOKUP, boxid, CRAFTING_PAGEARROW_ID, RPGMENUID - 3, 16.1, 288.0, "\c[B1]", "\c[Y5]");
		HandleCraftingWeaponDraw(pnum, p, boxid, k);
	}
	else if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
		DrawBoxText("<=", DND_NOLOOKUP, boxid, CRAFTING_PAGEARROW_ID, RPGMENUID - 3, 16.1, 288.0, "\c[B1]", "\c[Y5]");
		HandleCraftingInventoryDraw(pnum, p, boxid, k);

		// draw player credits on top right corner
		SetFont("NMENUFNT");
		HudMessage(s:"\c[Y5]", l:"DND_MENU_CREDITS", s:": \c-$", d:GetPlayerCredit(pnum); HUDMSG_PLAIN, RPGMENUID - 4, CR_WHITE, 372.1, 10.0, 0.0, 0.0);
	}
	else if(curopt == MENU_LOAD_CRAFTING_TRANSMUTING) {
		DrawBoxText("<=", DND_NOLOOKUP, boxid, CRAFTING_PAGEARROW_ID, RPGMENUID - 3, 16.1, 288.0, "\c[B1]", "\c[Y5]");
		HandleTransmutingDraw(pnum, p, boxid, k);
		
		// draw player credits on top right corner
		SetFont("NMENUFNT");
		HudMessage(s:"\c[Y5]", l:"DND_MENU_CREDITS", s:": \c-$", d:GetPlayerCredit(pnum); HUDMSG_PLAIN, RPGMENUID - 4, CR_WHITE, 372.1, 10.0, 0.0, 0.0);
	}
	SetFont("NMENUFNT");
}

// serverside -- input function
void HandleCraftingInputs(int boxid, int curopt) {
	int pnum = PlayerNumber();
	int prevselect;
	if(HasPlayerClicked(pnum)) {
		int curitemeindex = ((boxid >> DND_MENU_ITEMSAVEBITS1) & DND_MENU_ITEMSAVEBITS1_MASK);
		int previtemindex = (boxid >> DND_MENU_ITEMSAVEBITS2);
		//Log(d:curitemeindex, s: " ", d:previtemindex);
		//printbold(d:previtemindex, s: " ", d:boxid);
		boxid = (boxid & DND_MENU_ITEMSAVEBITS1_MASK);
		if(boxid != MAINBOX_NONE && (CheckInventory("DnD_SellConfirm") || boxid != CheckInventory("DnD_SelectedInventoryBox"))) {
			if(HasLeftClicked(pnum)) {
				// arrows in material part, left and right respectively
				if(boxid == MATERIALARROW_ID) {
					TakeInventory("DnD_Crafting_MaterialPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					GiveInventory("DnD_CleanCraftingRequest", 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(boxid == MATERIALARROW_ID + 1) {
					GiveInventory("DnD_Crafting_MaterialPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					GiveInventory("DnD_CleanCraftingRequest", 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(boxid > MATERIALBOX_OFFSET_BOXID && boxid <= MATERIALBOX_OFFSET_BOXID + MAX_CRAFTING_MATERIALBOXES) {
					// marking a material in case they need something to be used on
					SetInventory("DnD_SelectedInventoryBox", boxid);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(boxid == CRAFTING_PAGEARROW_ID) {
					// bottom arrows for crafting
					if(!CheckInventory("DnD_Crafting_ItemPage")) {
						if(curopt == MENU_LOAD_CRAFTING_WEAPON || curopt == MENU_LOAD_CRAFTING_INVENTORY)
							UpdateMenuPosition(MENU_LOAD_CRAFTING);
						else if(curopt == MENU_LOAD_CRAFTING)
							UpdateMenuPosition(MENU_LOAD);
					}
					else {
						TakeInventory("DnD_Crafting_ItemPage", 1);
						GiveInventory("DnD_RefreshPane", 1);
						LocalAmbientSound("RPG/MenuChoose", 127);
					}
					GiveInventory("DnD_CleanCraftingRequest", 1);
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
				else if(boxid == CRAFTING_PAGEARROW_ID + 1) {
					GiveInventory("DnD_Crafting_ItemPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					SetInventory("DnD_SelectedInventoryBox", 0);
					LocalAmbientSound("RPG/MenuChoose", 127);
					GiveInventory("DnD_CleanCraftingRequest", 1);
				}
				else if(curopt == MENU_LOAD_CRAFTING) {
					if(boxid == CRAFTING_WEAPON_BOXID + 1)
						UpdateMenuPosition(MENU_LOAD_CRAFTING_WEAPON);
					else if(boxid == CRAFTING_INVENTORY_BOXID + 1)
						UpdateMenuPosition(MENU_LOAD_CRAFTING_INVENTORY);
					else if(boxid == CRAFTING_TRANSMUTING_BOXID + 1) {
						if(CheckResearchStatus(RES_TRANSMUTING) != RES_DONE)
							ShowPopup(POPUP_NEEDRESEARCH, false, 0);
						else {
							ResetTransmutingData(pnum);
							UpdateMenuPosition(MENU_LOAD_CRAFTING_TRANSMUTING);
						}
					}
				}
				else if(curopt == MENU_LOAD_CRAFTING_WEAPON) {
					if(boxid > 0 && boxid <= MAX_CRAFTING_ITEMBOXES) {
						SetInventory("DnD_SelectedInventoryBox", boxid);
						LocalAmbientSound("RPG/MenuChoose", 127);
					}
				}
				else if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
					// inventory clicking
					if(boxid > 0 && boxid <= MAX_CRAFTING_ITEMBOXES) {
						if(CheckInventory("DnD_UsedTwoItemRequirementMaterial") && CheckInventory("DnD_SelectedInventoryBox")) {
							if(!HandleTwoRequirementMaterialUse(pnum, previtemindex, curitemeindex))
								ShowPopup(POPUP_MATERIALCANTUSE, false, 0);
							else // we managed to use it
								UsePlayerItem(pnum, CheckInventory("DnD_UsedTwoItemRequirementMaterial") - 1, false);
							SetInventory("DnD_SelectedInventoryBox", 0);
							SetInventory("DnD_UsedTwoItemRequirementMaterial", 0);
						}
						else {
							// printbold(s:"first click ", d:curitemeindex, s: " ", d:previtemindex, s:" ", d:boxid);
							SetInventory("DnD_SelectedInventoryBox", boxid);
						}
						LocalAmbientSound("RPG/MenuChoose", 127);
					}
				}
			}
			else if(HasRightClicked(pnum)) {
				// using an orb in material part
				if(boxid > MATERIALBOX_OFFSET_BOXID && boxid <= MATERIALBOX_OFFSET_BOXID + MAX_CRAFTING_MATERIALBOXES) {
					//Log(s:"item type ", d:PlayerInventoryList[pnum][curitemeindex].item_type, s: " ", d:PlayerInventoryList[pnum][curitemeindex].item_subtype);
					if(IsSelfUsableItem(PlayerInventoryList[pnum][curitemeindex].item_type, PlayerInventoryList[pnum][curitemeindex].item_subtype)) {
						if(IsTwoSelectionItem(pnum, curitemeindex)) {
							// start the display saying player needs to make two selections
							// printbold(s:"right click on assim ", d:curitemeindex);
							SetInventory("DnD_UsedTwoItemRequirementMaterial", curitemeindex + 1);
						}
						else if(HandleMaterialUse(pnum, curitemeindex, -1))
							UsePlayerItem(pnum, curitemeindex, IsCraftingPageForTokens(curopt));
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
								if(HandleMaterialUse(pnum, curitemeindex, previtemindex, true))
									UsePlayerItem(pnum, curitemeindex, true);
								else
									ShowPopup(POPUP_MATERIALCANTUSE, false, 0);
							}
							else if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
								//printbold(d:previtemindex, s: " ", d:PlayerInventoryList[pnum][curitemeindex].item_type, s: " vs ", d:DND_ITEM_CHARM);
								// is the index shown here not being the same as the one on inventory list causing the problem here?
								if(HandleMaterialUse(pnum, curitemeindex, previtemindex))
									UsePlayerItem(pnum, curitemeindex, false);
								else // this part can handle rest of the item types to come later on
									ShowPopup(POPUP_MATERIALCANTUSE, false, 0);
							}
						}
					}
				}
				else if(curopt == MENU_LOAD_CRAFTING_INVENTORY) {
					if(boxid > 0 && boxid <= MAX_CRAFTING_ITEMBOXES) {
						if(!CheckInventory("DnD_SellConfirm")) {
							if(CheckInventory("DnD_CantSalvage"))
								return;
							// scavenge, ask user in the form of a popup to confirm
							SetInventory("DnD_ItemCursorMsg", GetDissassembleChance(pnum, curitemeindex));
							SetInventory("DnD_ItemPriceTemp", DisassembleItem_Price(pnum, curitemeindex));
							SetInventory("DnD_ItemSelectTemp", curitemeindex);
							ShowNotif(POPUP_SCAVENGECONFIRM, 0, CheckInventory("DnD_ItemPriceTemp"));
							LocalAmbientSound("RPG/MenuSellConfirm", 127);
							SetInventory("DnD_SelectedInventoryBox", boxid);
							GiveInventory("DnD_SellConfirm", 1);
						}
						else {
							if(CheckInventory("DnD_CantSalvage"))
								return;

							int price = CheckInventory("DnD_ItemPriceTemp"); 
							if(GetPlayerCredit(pnum) >= price) {
								DisassembleItem(pnum, CheckInventory("DnD_ItemSelectTemp"), price, CheckInventory("DnD_ItemCursorMsg"));
								TakeInventory("DnD_SellConfirm", 1);
								ClearTempItemInventory();
								SetInventory("DnD_SelectedInventoryBox", 0);
								ACS_NamedExecuteAlways("DnD Menu Sell Popup Clear", 0);
							}
							else {
								TakeInventory("DnD_SellConfirm", 1);
								ClearTempItemInventory();
								SetInventory("DnD_SelectedInventoryBox", 0);
								ShowPopup(POPUP_NOFUNDS, false, 0);
							}
						}
					}
					else if(CheckInventory("DnD_SellConfirm")) {
						TakeInventory("DnD_SellConfirm", 1);
						ClearTempItemInventory();
						ACS_NamedExecuteAlways("DnD Menu Sell Popup Clear", 0);
					}
				}
			}
		}
		else {
			SetInventory("DnD_SelectedInventoryBox", 0);
			
			// if player clicks nothing we reset the two item engagement
			SetInventory("DnD_UsedTwoItemRequirementMaterial", 0);
		}
		ClearPlayerInput(pnum, true);
	}
	else if(HasPressedLeft(pnum)) {
		if(CheckInventory("DnD_Crafting_ItemPage")) {
			TakeInventory("DnD_Crafting_ItemPage", 1);
			GiveInventory("DnD_RefreshPane", 1);
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		else if(curopt != MENU_LOAD_CRAFTING) {
			UpdateMenuPosition(MENU_LOAD_CRAFTING);
			GiveInventory("DnD_CleanCraftingRequest", 1);
			SetInventory("DnD_Crafting_MaterialPage", 0);
		}
		else {
			UpdateMenuPosition(MENU_LOAD);
			GiveInventory("DnD_CleanCraftingRequest", 1);
			SetInventory("DnD_Crafting_MaterialPage", 0);
		}
			
		SetInventory("DnD_SelectedInventoryBox", 0);
	}
	else if(curopt != MENU_LOAD_CRAFTING && HasPressedRight(pnum)) {
		// we dont want right page input to work on crafting selection
		int mcount = curopt == MENU_LOAD_CRAFTING_WEAPON ? GetWeaponCount() : GetCraftableItemCount();
		if(mcount - MAX_CRAFTING_ITEMBOXES * (CheckInventory("DnD_Crafting_ItemPage") + 1) > 0) {
			GiveInventory("DnD_Crafting_ItemPage", 1);
			GiveInventory("DnD_RefreshPane", 1);
			GiveInventory("DnD_CleanCraftingRequest", 1);
			SetInventory("DnD_SelectedInventoryBox", 0);
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
	}
}

void HandleTransmutingInputs(int pnum, int boxid) {
	if(HasPlayerClicked(pnum)) {
		int curitemeindex = ((boxid >> DND_MENU_ITEMSAVEBITS1) & DND_MENU_ITEMSAVEBITS1_MASK);
		int previtemindex = (boxid >> DND_MENU_ITEMSAVEBITS2);
		int recipe = 0, i, temp;
		//Log(d:curitemeindex, s: " ", d:previtemindex);
		//printbold(d:previtemindex, s: " ", d:boxid);
		boxid = (boxid & DND_MENU_ITEMSAVEBITS1_MASK);
		if(boxid != MAINBOX_NONE) {
			if(HasLeftClicked(pnum)) {
				// arrows in material part, left and right respectively
				if(boxid == MATERIALARROW_ID) {
					TakeInventory("DnD_Crafting_MaterialPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					GiveInventory("DnD_CleanCraftingRequest", 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(boxid == MATERIALARROW_ID + 1) {
					GiveInventory("DnD_Crafting_MaterialPage", 1);
					GiveInventory("DnD_RefreshPane", 1);
					GiveInventory("DnD_CleanCraftingRequest", 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(boxid > MATERIALBOX_OFFSET_BOXID && boxid <= MATERIALBOX_OFFSET_BOXID + MAX_CRAFTING_MATERIALBOXES) {
					// marking a material in case they need something to be used on
					SetInventory("DnD_SelectedInventoryBox", boxid);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(boxid == CRAFTING_PAGEARROW_ID) {
					// bottom arrows for crafting
					UpdateMenuPosition(MENU_LOAD_CRAFTING);
					GiveInventory("DnD_CleanCraftingRequest", 1);
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
				else if(boxid == MBOX_4 && (recipe = IsValidTransmuteRecipe(pnum)) != -1) {
					// check credit
					if(GetPlayerCredit(pnum) < DND_TRANSMUTE_COST) {
						ShowPopup(POPUP_NOFUNDS, false, 0);
						return;
					}

					// add them only if they aren't the same -- really shitty code but I really didn't wanna use an array to group them together
					int oid_1 = TransmuteOrbs[pnum][0];
					int oid_2 = TransmuteOrbs[pnum][1];
					int oid_3 = TransmuteOrbs[pnum][2];
					int oamt_1 = 1;
					int oamt_2 = 1;
					int oamt_3 = 1;
					int ocount_1 = 0;
					int ocount_2 = 0;
					int ocount_3 = 0;
					if(oid_1 == oid_2) {
						++oamt_1;
						oid_2 = -1;
					}
					if(oid_1 == oid_3) {
						++oamt_1;
						oid_3 = -1;
					}
					if(oid_2 == oid_3) {
						++oamt_2;
						oid_3 = -1;
					}

					if
					(
						(oid_1 == -1 || (ocount_1 = CountOrbsOfTypeInventory(pnum, oid_1)) >= oamt_1) &&
						(oid_2 == -1 || (ocount_2 = CountOrbsOfTypeInventory(pnum, oid_2)) >= oamt_2) &&
						(oid_3 == -1 || (ocount_3 = CountOrbsOfTypeInventory(pnum, oid_3)) >= oamt_3)
					)
					{
						if(oid_1 != -1)
							TakeOrbFromPlayer_NoStash(pnum, oid_1, oamt_1);
						if(oid_2 != -1)
							TakeOrbFromPlayer_NoStash(pnum, oid_2, oamt_2);
						if(oid_3 != -1)
							TakeOrbFromPlayer_NoStash(pnum, oid_3, oamt_3);

						if(recipe != DND_RANDOM_TAGGED_ORB_TRANSMUTE && recipe != DND_ELEMENTAL_TO_PRISMATIC) {
							ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, recipe, 1);
							MarkOrbRecipeComplete(recipe);
						}
						else if(recipe == DND_ELEMENTAL_TO_PRISMATIC) {
							ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, DND_ORB_PRISMATIC, 1);
							MarkOrbRecipeComplete(DND_ORB_PRISMATIC);
						}
						else {
							recipe = PickRandomTaggedOrb();
							ACS_NamedExecuteAlways("DnD Give Orb Delayed", 0, recipe, 1);
						}

						TakeCredit(DND_TRANSMUTE_COST);
						GiveInventory("DnD_CleanCraftingRequest", 1);
						LocalAmbientSound("Items/SuccessDisassemble", 108);

						if((oid_1 != -1 && ocount_1 - oamt_1 < oamt_1) || (oid_2 != -1 && ocount_2 - oamt_2 < oamt_2) || (oid_3 != -1 && ocount_3 - oamt_3 < oamt_3)) {
							// reset the transmute array
							ResetTransmutingData(pnum);
						}
					}
					else
						ShowPopup(POPUP_NOTENOUGHMATERIALS, false, 0);
				}
				else if(boxid >= MBOX_1 && boxid <= MBOX_3 && CheckInventory("DnD_SelectedInventoryBox")) {
					// previtemindex holds (topboxid of item) - 1 in inventory
					temp = PlayerInventoryList[pnum][previtemindex].item_subtype;
					TransmuteOrbs[pnum][boxid - 1] = temp;
					ACS_NamedExecuteAlways("DnD Transmute Orb Sync", 0, pnum, boxid - 1, temp);
					LocalAmbientSound("RPG/MenuChoose", 127);
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
			}
			else if(HasRightClicked(pnum) && boxid >= MBOX_1 && boxid <= MBOX_3) {
				// take orb off transmute list
				TransmuteOrbs[pnum][boxid - 1] = -1;
				ACS_NamedExecuteAlways("DnD Transmute Orb Sync", 0, pnum, boxid - 1, -1);
				LocalAmbientSound("RPG/MenuChoose", 127);
				SetInventory("DnD_SelectedInventoryBox", 0);
			}
		}
		else {
			SetInventory("DnD_SelectedInventoryBox", 0);
			
			// if player clicks nothing we reset the two item engagement
			SetInventory("DnD_UsedTwoItemRequirementMaterial", 0);
		}
		ClearPlayerInput(pnum, true);
	}
	else if(HasPressedLeft(pnum)) {
		UpdateMenuPosition(MENU_LOAD_CRAFTING);
		GiveInventory("DnD_CleanCraftingRequest", 1);
		SetInventory("DnD_SelectedInventoryBox", 0);
	}
}

// returns success of use of item
// item is craft material, target is the item we intend to use it on
bool HandleMaterialUse(int pnum, int itemindex, int targetindex, bool isWeapon = false) {
	bool res = false;
	int itype = PlayerInventoryList[pnum][itemindex].item_type;
	int isubtype = PlayerInventoryList[pnum][itemindex].item_subtype;
	
	int targettype = DND_ITEM_NULL;
	if(targetindex != -1) {
		if(!isWeapon)
			targettype = PlayerInventoryList[pnum][targetindex].item_type;
		else
			targettype = DND_ITEM_WEAPON;
	}

	// printbold(s:"type: ", d:itype, s: " ", d:isubtype);
	// self usable materials usually go here as they have targettype NULL
	if(!IsCraftableItem(targettype)) {
		if(itype == DND_ITEM_ORB) {
			res = CanUseOrb(isubtype, 0, 0);
			if(res) {
				ACS_NamedExecuteAlways("DND Orb Use", 0, isubtype, 0);
			}
		}
	}
	else if(itype == DND_ITEM_ORB) {
		// targettype is a craftable item
		res = CanUseOrb(isubtype, targetindex, targettype);
		if(res) {
			ACS_NamedExecuteAlways("DND Orb Use", 0, isubtype, targetindex);
		}
	}
	else if(itype == DND_ITEM_TOKEN) {
		res = CanUseToken(isubtype, targetindex, targettype);
		if(res) {
			ACS_NamedExecuteAlways("DND Token Use", 0, isubtype, targetindex);
		}
	}
	
	return res;
}

bool HandleTwoRequirementMaterialUse(int pnum, int item_index1, int item_index2) {
	// preliminary check to see if these are items that can indeed be crafted on
	if(!IsCraftableItem(PlayerInventoryList[pnum][item_index1].item_type) || !IsCraftableItem(PlayerInventoryList[pnum][item_index2].item_type))
		return false;
	int mat_index = CheckInventory("DnD_UsedTwoItemRequirementMaterial") - 1;
	int type = PlayerInventoryList[pnum][mat_index].item_type;
	int subtype = PlayerInventoryList[pnum][mat_index].item_subtype;
	
	// We check to specific types now and determine what needs to be done
	if(type == DND_ITEM_ORB) {
		if(CanUseOrb(subtype, item_index1, item_index2)) {
			ACS_NamedExecuteAlways("DND Orb Use", 0, subtype, item_index1, item_index2);
			return true;
		}
	}
	return false;
}

void GetInputOnMenuPage(int opt) {
	if(opt == MENU_MAIN)
		ListenInput(0, 0, 0);
	else if(opt == MENU_RESEARCH || opt == MENU_RESEARCH_GUNS)
		ListenInput(LISTEN_LEFT | LISTEN_RIGHT, 0, 0);
	else if(opt >= SHOP_RESPAGE_BEGIN && opt <= SHOP_RESPAGE_END)
		ListenInput(LISTEN_LEFT | LISTEN_RIGHT | LISTEN_FASTLR | LISTEN_SKIPKNOWNRES, 0, MENU_MAXRES_PERPAGE, opt - SHOP_RESPAGE_BEGIN);
	else
		ListenInput(LISTEN_LEFT | LISTEN_RIGHT, 0, 0);
}

int GetAmmoSlotAndIndexFromShop(int index) {
	if(index < SHOP_FIRSTAMMOSPECIAL_INDEX) {
		for(int i = 0; i < MAX_SLOTS; ++i)
			int temp = GetAmmoSlotMaxIndex(i);
			for(int j = 0; j < temp; ++j) {
				if(MenuAmmoIndexMap[i][j] == index)
					return i | (j << 16);
			}
	}
	else
		return index - SHOP_FIRSTAMMOSPECIAL_INDEX;
	// keep this return structure as is, if the top loop finds nothing silently return 0
	return 0;
}

int GetResistDisplayVal(int pnum, int res, int cap, int reduce) {
	int val = GetPlayerAttributeValue(pnum, res) + GetPlayerAttributeValue(pnum, INV_DMGREDUCE_ALL) + reduce;
	switch(res) {
		case INV_DMGREDUCE_FIRE:
		case INV_DMGREDUCE_ICE:
		case INV_DMGREDUCE_POISON:
		case INV_DMGREDUCE_LIGHTNING:
			val += GetPlayerAttributeValue(pnum, INV_DMGREDUCE_ELEM);
		break;
	}
	return val;
	//return ApplyResistCap(pnum, val, cap);
}

// not sure how to group these for other places, their calculations arent exactly done in straightforward fashion so calculating as they come makes sense
void DrawPlayerStats(int pnum, int category) {
	int val;
	int k = 0;
	int tid = pnum + P_TIDSTART;
	int i;
	int temp;
	
	// sum of y and height should = 248
	SetHudClipRect(192, 52, 256, 196, 256);
	
	// yea this is some terrible piece of code but idc, should be cleaned up with an array to lookup attribs and a switch-case for exceptions? idk
	if(PlayerStatText == "") {
		if(category == DRAW_STAT_OFFENSE1) {
			// melee dmg
			val = GetPlayerAttributeValue(pnum, INV_MELEEDAMAGE);
			if(val) {
				PlayerStatText = StrParam(s:GetItemAttributeText(INV_MELEEDAMAGE, 0, 0, val), s:"\n");
				++k;
			}
			
			// melee range
			val = GetPlayerMeleeRange(pnum + P_TIDSTART, 100.0);
			if(val != 100.0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", s:GetFixedRepresentation(val, false), s:"%\c- ", l:"DND_MELEERANGE", s:"\n");
				++k;
			}

			// accuracy
			val = GetActorProperty(0, APROP_ACCURACY);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_ACCURACY_INCREASE, 0, 0, val), s:"\n");
				++k;
			}

			// crit block begins
			val = GetCritChance_Display(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_GLOBALCRIT", s:"\n");
				++k;
			}
			
			val = GetIndependentCritModifier(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_GLOBALCRITMULT", s:"\n");
				++k;
			}
			// crit block ends
			
			
			// exp repeat chance
			val = GetExplosiveRepeatChance(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_CHANCETOEXPREPEAT", s:"\n");
				++k;
			}
			
			// exp rad
			val = GetPlayerAttributeValue(pnum, INV_EXPLOSION_RADIUS);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"IATTR_T31", s:"\n");
				++k;
			}
			
			// more damage
			val = GetPlayerAttributeValue(pnum, INV_DAMAGEPERCENT_MORE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_DAMAGEPERCENT_MORE, 0, 0, val), s:"\n");
				++k;
			}
			
			
			// pen block begins
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_BULLET);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_PEN_PHYSICAL, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_ENERGY);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_PEN_ENERGY, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_OCCULT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_PEN_OCCULT, 0, 0, val), s:"\n");
				++k;
			}
			
			// get the pens of 4 elements here
			i = GetPlayerAttributeValue(pnum, INV_EX_ELEPENHARMONY) ? GetHighestElePen(pnum) : 0;
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_FIRE);
			if(val) {
				if(i != DND_DAMAGECATEGORY_FIRE)
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_FIRE", s: " ", l:"DND_MENU_PENETRATION", s:"\n");
				else
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_FIRE", s: " ", l:"DND_MENU_PENETRATION", s:" \cf(H)\c-\n");
				++k;
			}
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_ICE);
			if(val) {
				if(i != DND_DAMAGECATEGORY_ICE)
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_ICE", s: " ", l:"DND_MENU_PENETRATION", s:"\n");
				else
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_ICE", s: " ", l:"DND_MENU_PENETRATION", s:" \cf(H)\c-\n");
				++k;
			}
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_LIGHTNING);
			if(val) {
				if(i != DND_DAMAGECATEGORY_LIGHTNING)
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_LGHT", s: " ", l:"DND_MENU_PENETRATION", s:"\n");
				else
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_LGHT", s: " ", l:"DND_MENU_PENETRATION", s:" \cf(H)\c-\n");
				++k;
			}
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_POISON);
			if(val) {
				if(i != DND_DAMAGECATEGORY_POISON)
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_POIS", s: " ", l:"DND_MENU_PENETRATION", s:"\n");
				else
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_POIS", s: " ", l:"DND_MENU_PENETRATION", s:" \cf(H)\c-\n");
				++k;
			}
			// pen block ends
			
			
			// flat and % dmg block
			for(i = DND_DAMAGECATEGORY_BEGIN; i < DND_DAMAGECATEGORY_END; ++i) {
				val = GetDamageTypeBonus(pnum, i, 0);
				if(val) {
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:GetTalentTag(i), s:" ", l:"DND_DAMAGEBONUS", s:"\n");
					++k;
				}
				
				val = MapDamageCategoryToFlatBonus(pnum, i, 0);
				if(val) {
					PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"\c- to ", l:GetTalentTag(i), s:" ", l:"DND_DAMAGE", s:"\n");
					++k;
				}
			}
			// end block
			
			// blocking mons take % more
			val = GetPlayerAttributeValue(pnum, INV_BLOCKERS_MOREDMG);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_BLOCKERS_MOREDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			// pellet count -- 1.0 is base
			val = GetPelletIncrease(pnum) - 1.0;
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"IATTR_T30", s:"\n");
				++k;
			}
		}
		else if(category == DRAW_STAT_OFFENSE2) {
			// wep type specific bonuses flat and %
			val = GetPlayerAttributeValue(pnum, INV_HANDGUN_PERCENT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_HANDGUN_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_FLAT_HANDGUN);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_HANDGUN, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_SHOTGUN_PERCENT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_SHOTGUN_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_FLAT_SHOTGUN);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_SHOTGUN, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_AUTOMATIC_PERCENT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AUTOMATIC_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_FLAT_AUTOMATIC);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_AUTOMATIC, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_ARTILLERY_PERCENT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_ARTILLERY_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_FLAT_ARTILLERY);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_ARTILLERY, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_PRECISION_PERCENT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_PRECISION_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_FLAT_PRECISION);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_PRECISION, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_TECH_PERCENT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_TECH_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_FLAT_TECH);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_TECH, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_MAGIC_PERCENT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_MAGIC_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_FLAT_MAGIC);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_MAGIC, 0, 0, val), s:"\n");
				++k;
			}

			// generic things like dot multi, dot %
			val = GetPlayerAttributeValue(pnum, INV_DOTMULTI);
			if(val) {
				PlayerStatText = StrParam(s:GetItemAttributeText(INV_DOTMULTI, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_INCREASEDDOT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_INCREASEDDOT, 0, 0, val), s:"\n");
				++k;
			}
			
			// fire things
			val = GetFireDOTDamage(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:"\c- ", l:"DND_IGNITEDAMAGE", s:"\n");
				++k;
			}
			
			// +flat fire damage
			val = GetPlayerAttributeValue(pnum, INV_FLAT_FIREDMG);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_FIREDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetIgniteChance(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:"%\c- ", l:"DND_IGNITECHANCE", s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_IGNITEDURATION);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_IGNITEDURATION, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetIgniteProlifChance(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:"%\c- ", l:"DND_IGNITEPROLIFCHANCE", s:"\n");
				++k;
			}
			
			val = GetIgniteProlifRange(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", f:val, s:"\c- ", l:"DND_IGNITEPROLIFRANGE", s:"\n");
				++k;
			}
			
			val = GetIgniteProlifCount(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:"\c- ", l:"DND_IGNITEPROLIFCOUNT", s:"\n");
				++k;
			}
			
			// ice things
			val = GetPlayerAttributeValue(pnum, INV_FLAT_ICEDMG);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_ICEDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetFreezeChance(pnum, 1);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s: "%\c- ", l:"DND_FREEZECHANCE", s:"\n");
				++k;
			}
			
			val = GetChillEffect(pnum, 1);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, l:"DND_CHILLSLOWS", s: " \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\n");
				++k;
			}
			
			val = GetChillThreshold(pnum, 1);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s: "%\c- ", l:"DND_CHILLTHRESHOLD", s:"\n");
				++k;
			}
			
			// lightning things
			val = GetPlayerAttributeValue(pnum, INV_FLAT_LIGHTNINGDMG);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_LIGHTNINGDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_OVERLOADCHANCE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_OVERLOADCHANCE, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_OVERLOAD_ZAPCOUNT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_OVERLOAD_ZAPCOUNT, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_OVERLOAD_DMGINCREASE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_OVERLOAD_DMGINCREASE, 0, 0, val), s:"\n");
				++k;
			}
			
			// poison things
			val = GetPlayerAttributeValue(pnum, INV_FLAT_POISONDMG);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_FLAT_POISONDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPoisonTicrate(pnum);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, l:"DND_POISONTICRATE", s:"\c[Q9]", d:(FixedDiv(val, DND_POISON_CHECKRATE) * DND_POISON_TICCHECK) >> 16, s:"\c- ", l:"DND_TICS", s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_POISON_DURATION);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_POISON_DURATION, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_POISON_TICDMG);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_POISON_TICDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			// ailment ignore chance
			val = GetPlayerAttributeValue(pnum, INV_CHANCE_AILMENTIGNORE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_CHANCE_AILMENTIGNORE, 0, 0, val), s:"\n");
				++k;
			}
		}
		else if(category == DRAW_STAT_DEFENSE) {
			// hp cap
			val = GetSpawnHealth();
			PlayerStatText = StrParam(s:"\c[Q9]", d:val, s: " \c-", l:"DND_HEALTHCAPIS", s:"\n");
			++k;
			
			// armor rating
			val = GetPlayerArmor(pnum);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:" \c-", l:"DND_ARMORCAPIS", s:"\n");
				++k;

				// estimated armor protection
				val = GetPlayerEstimatedArmorProtect(pnum, val);
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:"% \c-", l:"DND_ESTREDUCTION", s:"\n");
				++k;
			}

			val = GetMitigationChance(pnum);
			if(val > 0) {
				// clamp it on display, the math stuff behind the scenes doesn't need an extra if statement
				if(val > 100.0)
					val = 100.0;
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", s:GetFixedRepresentation(val, false), s:"% \c-", l:"IATTR_T102", s:"\n");
				++k;

				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", s:GetFixedRepresentation(GetMitigationEffect(pnum), false), s:"% \c-", l:"IATTR_T103", s:"\n");
				++k;
			}

			val = GetPlayerEnergyShieldCap(pnum);
			i = val;
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:" \c-", l:"DND_ESHIELDIS", s:"\n");
				++k;

				val = GetPlayerEnergyShieldRechargeDelay(pnum);
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", s:GetFixedRepresentation((val << 16) / TICRATE, false), s:"\c-", l:"DND_ESHIELDDELAY", s:"\n");
				++k;

				val = GetPlayerEnergyShieldRecoveryRate(pnum, i);
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:" \c-", l:"DND_ESHIELDRECOVERY", s:"\n");
				++k;

				val = GetEShieldMagicAbsorbValue(pnum);
				if(val > 100)
					val = 100;
				PlayerStatText = StrParam(s:PlayerStatText, s:"\c[Q9]", d:val, s:"% \c-", l:"DND_ESHIELDABSORB", s:"\n");
				++k;
			}
			
			// inc damage taken
			val = GetPlayerAttributeValue(pnum, INV_EX_DMGINCREASE_TAKEN);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_EX_DMGINCREASE_TAKEN, 0, 0, val), s:"\n");
				++k;
			}
			
			// regen cap
			val = GetPlayerAttributeValue(pnum, INV_REGENCAP_INCREASE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_REGENCAP_INCREASE, 0, 0, val), s:"\n");
				++k;
			}
			
			// knockback
			if(CheckUniquePropertyOnPlayer(pnum, PUP_KNOCKBACKIMMUNE)) {
				PlayerStatText = StrParam(s:PlayerStatText, l:"DND_KNOCKBACKIMMUNE", s:"\n");
				++k;
			}
			else {
				val = GetPlayerAttributeValue(pnum, INV_KNOCKBACK_RESIST);
				if(val) {
					PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_KNOCKBACK_RESIST, 0, 0, val), s:"\n");
					++k;
				}
			}
			
			// dmg reduction block begins -- shown with their respective caps applied
			val = 1.0 - GetSelfExplosiveResist(pnum);
			if(val != 0.0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_SELFEXPDMG", s:"\n");
				++k;
			}
			
			i = GetMaxResistCap(pnum);
			temp = DND_PLAYER_RESIST_REDUCE * (GetLevel() / DND_PLAYER_WEAKEN_LEVELS) + 1.0 * GetPlayerAttributeValue(pnum, INV_EX_UNITY_RES_BONUS) * GetUnity() / DND_UNITY_DIVISOR;
			val = GetPlayerAttributeValue(pnum, INV_EX_RESPERESHIELD);
			if(val)
				temp += DND_RES_PER_PRISMGUARD * (CheckInventory("EShieldAmount") / val);
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_PHYS, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_PHYS", s:"\n");
			++k;
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_ENERGY, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_ENRG", s:"\n");
			++k;
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_MAGIC, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_MAGC", s:"\n");
			++k;
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_FIRE, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_FIRE", s:"\n");
			++k;
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_ICE, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_ICE", s:"\n");
			++k;
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_LIGHTNING, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_LGHT", s:"\n");
			++k;
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_POISON, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_POIS", s:"\n");
			++k;
			
			val = GetResistDisplayVal(pnum, INV_DMGREDUCE_REFL, i, temp);
			PlayerStatText = StrParam(s:PlayerStatText, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val, false), s:" / \c[Q9]", s:GetFixedRepresentation(i, false), s:" \c-", l:"DND_MENU_RES_REFL", s:"\n");
			++k;
			// dmg reduction block ends

			// reduced curse things
			val = GetPlayerAttributeValue(pnum, INV_REDUCEDCURSEDURATION);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_REDUCEDCURSEDURATION, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_REDUCEDCURSEEFFECT);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_REDUCEDCURSEEFFECT, 0, 0, val), s:"\n");
				++k;
			}

			// ailment avoidance
			val = GetPlayerElementalAvoidance(pnum, INV_AVOID_IGNITE);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AVOID_IGNITE, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerElementalAvoidance(pnum, INV_AVOID_CHILLFREEZE);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AVOID_CHILLFREEZE, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerElementalAvoidance(pnum, INV_AVOID_POISON);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AVOID_POISON, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerElementalAvoidance(pnum, INV_AVOID_OVERLOAD);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AVOID_OVERLOAD, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerNonElementalAvoidance(pnum, INV_AVOID_BLEED);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AVOID_BLEED, 0, 0, val), s:"\n");
				++k;
			}
		}
		else {
			// utility
			// drop chance
			val = (GetDropChance(pnum) - 1.0);
			if(val > 0.0) {
				PlayerStatText = StrParam(s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_DROPCHANCE", s:"\n");
				++k;
			}

			val = (GetPlayerItemRarity(pnum) - 1.0);
			if(val > 0.0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_RARITY", s:"\n");
				++k;
			}

			// ripper block
			val = GetPlayerAttributeValue(pnum, INV_RIPCOUNT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_RIPCOUNT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_RIPDAMAGE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_RIPDAMAGE, 0, 0, val), s:"\n");
				++k;
			}

			// lock-on block
			val = GetPlayerAttributeValue(pnum, INV_LOCKONAREA);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_LOCKONAREA, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerAttributeValue(pnum, INV_LOCKONRANGE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_LOCKONRANGE, 0, 0, val), s:"\n");
				++k;
			}
			
			// lifesteal block begins
			val = GetPlayerAttributeValue(pnum, INV_LIFESTEAL);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_LIFESTEAL, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_LIFESTEAL_CAP);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_LIFESTEAL_CAP, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_LIFESTEAL_RATE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_LIFESTEAL_RATE, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPlayerAttributeValue(pnum, INV_LIFESTEAL_RECOVERY);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_LIFESTEAL_RECOVERY, 0, 0, val), s:"\n");
				++k;
			}
			// lifesteal block ends

			// killing spree
			val = GetPlayerAttributeValue(pnum, INV_INCKILLINGSPREE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_INCKILLINGSPREE, 0, 0, val), s:"\n");
				++k;
			}
			
			// ammo gain chance
			val = GetPlayerAttributeValue(pnum, INV_AMMOGAIN_CHANCE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AMMOGAIN_CHANCE, 0, 0, val), s:"\n");
				++k;
			}
			
			// magsize
			val = GetPlayerAttributeValue(pnum, INV_MAGAZINE_INCREASE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"IATTR_T10", s:"\n");
				++k;
			}
			
			// ammo cap inc
			val = GetAmmoCapIncrease(pnum) - 100;
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_AMMOCAPS", s:"\n");
				++k;
			}
			
			// ammo pickup inc
			val = GetPlayerAttributeValue(pnum, INV_AMMOGAIN_INCREASE);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_AMMOGAIN_INCREASE, 0, 0, val), s:"\n");
				++k;
			}

			// reduced overheat
			val = GetPlayerAttributeValue(pnum, INV_REDUCED_OVERHEAT);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_REDUCED_OVERHEAT, 0, 0, val), s:"\n");
				++k;
			}

			// exp bonus
			val = GetPlayerWisdomBonus(pnum, tid);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_EXPERIENCEGAIN", s:"\n");
				++k;
			}
			
			// credit bonus
			val = GetPlayerGreedBonus(pnum, tid);
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_CREDITGAIN", s:"\n");
				++k;
			}
		
			// movespeed
			val = GetBonusPlayerSpeed(pnum);
			if(val > 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"IATTR_T9", s:"\n");
				++k;
			}
			else if(val < 0) {
				PlayerStatText = StrParam(s:PlayerStatText, s:"- \c[Q2]", s:GetFixedRepresentation(-val, true), s:"%\c- ", l:"IATTR_T9", s:"\n");
				++k;
			}
			
			// shop stock
			val = (GetAmmoCapIncrease(pnum) - 100) * (100 + GetPlayerAttributeValue(pnum, INV_SHOPSTOCK_INCREASE)) / 100;
			if(val) {
				PlayerStatText = StrParam(s:PlayerStatText, s:GetItemAttributeText(INV_SHOPSTOCK_INCREASE, 0, 0, val), s:"\n");
				++k;
			}
		}
	}

	HudMessage(s:PlayerStatText; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 192.1, 64.1 + 6.0 * ScrollPos.x, 0.0, 0.0);
	// --------------------------------------
	
	SetHudClipRect(0, 0, 0, 0, 0);
	if(GetCVar("survival")) {
		HudMessage(s:"\c[Y5]", l:"DND_MENU_LIVESLEFT", s:": \c-", d:GetPlayerLivesLeft(PlayerNumber()); HUDMSG_PLAIN, RPGMENUITEMID - 5, CR_WHITE, 190.1, 252.1, 0.0, 0.0);
		HudMessage(s:"\c[Y5]", l:"DND_MENU_MAPDIFF", s:": \c-", l:GetMapDifficultyLabel(CheckInventory("MapDifficultyClientside")); HUDMSG_PLAIN, RPGMENUITEMID - 6, CR_WHITE, 190.1, 260.1, 0.0, 0.0);
		HudMessage(s:"\c[Y5]", l:"DND_MENU_TOTALKILLS", s:": \c-", s:GetPlayerLifetimeKills(); HUDMSG_PLAIN, RPGMENUITEMID - 7, CR_WHITE, 190.1, 268.1, 0.0, 0.0);
	}
	else {
		HudMessage(s:"\c[Y5]", l:"DND_MENU_MAPDIFF", s:": \c-", l:GetMapDifficultyLabel(CheckInventory("MapDifficultyClientside")); HUDMSG_PLAIN, RPGMENUITEMID - 8, CR_WHITE, 190.1, 252.1, 0.0, 0.0);
		HudMessage(s:"\c[Y5]", l:"DND_MENU_TOTALKILLS", s:": \c-", s:GetPlayerLifetimeKills(); HUDMSG_PLAIN, RPGMENUITEMID - 9, CR_WHITE, 190.1, 260.1, 0.0, 0.0);
	}
	
	if(k > 12)
		ScrollPos.y = -(k - 12) * 4;
}

str GetPlayerLifetimeKills() {
	int temp = CheckInventory("DnD_LifeTimeKills_Billions");
	str result = "";
	bool have_commas = false;
	
	if(temp) {
		result = StrParam(s:result, d:temp, s:", ");
		have_commas = true;
	}
	
	temp = CheckInventory("DnD_LifeTimeKills_Millions");
	if(temp) {
		if(have_commas) {
			if(temp < 10)
				result = StrParam(s:result, d:0, d:0, d:temp, s:", ");
			else if(temp < 100)
				result = StrParam(s:result, d:0, d:temp, s:", ");
			else
				result = StrParam(s:result, d:temp, s:", ");
		}
		else
			result = StrParam(s:result, d:temp, s:", ");
		have_commas = true;
	}
	else if(have_commas)
		result = StrParam(s:result, s:"000, ");

	temp = CheckInventory("DnD_LifeTimeKills") / 1000;
	if(temp) {
		if(have_commas) {
			if(temp < 10)
				result = StrParam(s:result, d:0, d:0, d:temp, s:", ");
			else if(temp < 100)
				result = StrParam(s:result, d:0, d:temp, s:", ");
			else
				result = StrParam(s:result, d:temp, s:", ");
		}
		else
			result = StrParam(s:result, d:temp, s: ", ");
		have_commas = true;
	}
	else if(have_commas)
		result = StrParam(s:result, s:"000, ");
	
	temp = CheckInventory("DnD_LifeTimeKills") % 1000;
	if(have_commas) {
		if(temp < 10)
			result = StrParam(s:result, d:0, d:0, d:temp);
		else if(temp < 100)
			result = StrParam(s:result, d:0, d:temp);
		else
			result = StrParam(s:result, d:temp);
	}
	else
		result = StrParam(s:result, d:temp);
	
	return result;
}

void DrawMonsterModCategory(int category) {
	int i;
	SetHudClipRect(192, 52, 256, 228, 256);
	switch(category) {
		case DND_TRAITCODE_WEAKNESS:
			for(i = 0; i < MAX_WEAKNESS_MODS; ++i)
				HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[i]), s:"\n", l:StrParam(s:"DND_MENU_MMOD_WEAK", d:i + 1); HUDMSG_PLAIN, RPGMENUITEMID - i - 1, CR_WHITE, 192.1, 80.1 + 64.0 * i + 4.0 * ScrollPos.x, 0.0, 0.0);
		break;
		case DND_TRAITCODE_RESIST:
			for(i = 0; i < MAX_RESIST_MODS; ++i)
				HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[i + RESMODOFFSET]), s:"\n", l:StrParam(s:"DND_MENU_MMOD_RES", d:i + 1); HUDMSG_PLAIN, RPGMENUITEMID - i - 1, CR_WHITE, 192.1, 80.1 + 64.0 * i + 4.0 * ScrollPos.x, 0.0, 0.0);
		break;
		case DND_TRAITCODE_IMMUNITY:
			for(i = 0; i < MAX_IMMUNITY_MODS; ++i)
				HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[i + IMMMODOFFSET]), s:"\n", l:StrParam(s:"DND_MENU_MMOD_IMM", d:i + 1); HUDMSG_PLAIN, RPGMENUITEMID - i - 1, CR_WHITE, 192.1, 80.1 + 64.0 * i + 4.0 * ScrollPos.x, 0.0, 0.0);
		break;
		case DND_TRAITCODE_AGGRESSIVE:
			for(i = 0; i < MAX_AGGRESSIVE_MODS; ++i)
				HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[i + AGGMODOFFSET]), s:"\n", l:StrParam(s:"DND_MENU_MMOD_AGG", d:i + 1); HUDMSG_PLAIN, RPGMENUITEMID - i - 1, CR_WHITE, 192.1, 80.1 + 64.0 * i + 4.0 * ScrollPos.x, 0.0, 0.0);
		break;
		case DND_TRAITCODE_DEFENSIVE:
			for(i = 0; i < MAX_DEFENSE_MODS; ++i)
				HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[i + DEFMODOFFSET]), s:"\n", l:StrParam(s:"DND_MENU_MMOD_DEF", d:i + 1); HUDMSG_PLAIN, RPGMENUITEMID - i - 1, CR_WHITE, 192.1, 80.1 + 64.0 * i + 4.0 * ScrollPos.x, 0.0, 0.0);
		break;
		case DND_TRAITCODE_UTILITY:
			for(i = 0; i < MAX_UTILITY_MODS; ++i)
				HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[i + UTIMODOFFSET]), s:"\n", l:StrParam(s:"DND_MENU_MMOD_UTI", d:i + 1); HUDMSG_PLAIN, RPGMENUITEMID - i - 1, CR_WHITE, 192.1, 80.1 + 64.0 * i + 4.0 * ScrollPos.x, 0.0, 0.0);
		break;
		case -1:
			// since these are very few we can put these here for now
			HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[MAX_MONSTER_MODS - 2]), s:"\n", l:"DND_MENU_MMOD_CHAOS"; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 192.1, 80.1 + 4.0 * ScrollPos.x, 0.0, 0.0);
			HudMessage(l:GetMonsterTraitLabel(MonsterModGroupMapper[MAX_MONSTER_MODS - 1]), s:"\n", l:"DND_MENU_MMOD_LEG"; HUDMSG_PLAIN, RPGMENUITEMID - 2, CR_WHITE, 192.1, 132.1 + 4.0 * ScrollPos.x, 0.0, 0.0);
		break;
	}
	SetHudClipRect(0, 0, 0, 0);
}

bool IsPageHoverDrawer(int curopt) {
	switch(curopt) {
		case MENU_LOAD_INVENTORY:
		case MENU_LOAD_STASH:
		case MENU_LOAD_CHARM:
		case MENU_LOAD_CRAFTING_WEAPON:
		case MENU_LOAD_CRAFTING_INVENTORY:
		return true;
	}
	return false;
}

bool IsBoxChangeException(int curopt, int boxid) {
	// accessory page has some exceptions (dont randomly click "nothing")
	if(curopt >= MENU_FIRST_ACCESSORY_PAGE && curopt <= MENU_LAST_ACCESSORY_PAGE) {
		if(curopt != MENU_LAST_ACCESSORY_PAGE)
			return CheckResearchStatus(RES_ACCESSORY) != RES_DONE && boxid < MBOX_6;
		return CheckResearchStatus(RES_ACCESSORY) != RES_DONE && boxid < MBOX_3;
	}
	return false;
}

#ifdef ISAPRILFIRST
void HandleNFTDrawing(int boxid) {
	for(int i = 0; i < MAX_NFTS; ++i) {
		// draw nft label, cost, and image next to it
		if(!IsSet(CheckInventory("NFT_Token"), i))
			DrawBoxText(StrParam(s:"DND_NFT_TAG", d:i), DND_LANGUAGE_LOOKUP, boxid, MBOX_1 + i, RPGMENUITEMID - 1 - i, 192.1, 112.1 + 16.0 * i, "\c[B1]", "\c[Y5]");
		else
			DrawBoxText(StrParam(s:"DND_NFT_TAG", d:i), DND_LANGUAGE_LOOKUP, boxid, MBOX_1 + i, RPGMENUITEMID - 1 - i, 192.1, 112.1 + 16.0 * i, "\c[B1]", "\c[G8]");
		if(boxid == MBOX_1 + i) {
			SetHudClipRect(192, 248, 256, 32, 256);
			HudMessage(l:StrParam(s:"DND_NFT_EXP", d:i); HUDMSG_PLAIN, RPGMENUITEMID - MAX_NFTS - 1 - i, CR_WHITE, 192.1, 248.1 + 1.0 * ScrollPos.x, 0.0, 0.0);
			SetHudClipRect(0, 0, 0, 0, 0);
			
			// draw the nft image in left corner
			SetFont(StrParam(s:"NFT", d:i));
			SetHudSize(2576, 1960, 1);
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 68, CR_WHITE, 532.0, 444.0, 0.0, 0.0);
			
			SetFont("NMENUFNT");
			SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		}
	}
}
#endif
