#ifndef DND_MENUCHROME_IN
#define DND_MENUCHROME_IN

#include "DnD_MenuTables.h"

// ---------------------------------------------------------------- menu helpers, panes, views and page input

Script "DnD Refresh Request" (int pnum, int state) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum)
		Terminate;

	Delay(const:1);

	int tid = pnum + P_TIDSTART;

	if(state == 2)
		GiveActorInventory(tid, "DnD_CleanTradeviewRequest", 1);

	GiveActorInventory(tid, "DnD_RefreshPane", 1);
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
void ListenInput(int listenflag, int condx_min, int condx_max, int curr_res_page = -1, int jumpmod = DND_JUMPMOD_NONE) {
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
							res_info_T module& resData = GetResearchInfo(curr_res_page, curposx);
						} while(curposx > 0 && resData.res_id != -1 && CheckResearchStatus(resData.res_id) == RES_DONE);
						
						if(CheckResearchStatus(resData.res_id) != RES_DONE)
							SetInventory("MenuPosX", curposx);
					}
					LocalAmbientSound("RPG/MenuMove", 127);
					GiveInventory("Menu_LRCooldown", 1);
				}
				SetInventory("MenuInput", DND_MENUINPUT_PREVBUTTON);
				
				// reset these in page transitions
				if(PlayerCursorData.itemDragged != -1)
					ResetCursorDragData();
				// the server's word has to go with it, otherwise the very next draw repopulates the
				// drag straight back out of it and the item stays stuck to the cursor. The server
				// will not correct us for a whole round trip, and by the time it does the view has
				// stopped drawing, so nothing would ever clear it
				ResetCursorDragConfirm();
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
				if(curposx + 1 < condx_max && GetResearchInfo(curr_res_page, curposx + 1).res_id != -1) {
					if(!(listenflag & LISTEN_SKIPKNOWNRES))
						GiveInventory("MenuPosX", 1);
					else {
						// skip as far ahead as we are able
						do {
							++curposx;
							resData = GetResearchInfo(curr_res_page, curposx);
						} while(resData.res_id != -1 && curposx < condx_max && CheckResearchStatus(resData.res_id) == RES_DONE);
						// no change if we can't move further
						if(resData.res_id != -1)
							SetInventory("MenuPosX", curposx);
					}
					LocalAmbientSound("RPG/MenuMove", 127);
					GiveInventory("Menu_LRCooldown", 1);
				}
				SetInventory("MenuInput", DND_MENUINPUT_NEXTBUTTON);
				
				// reset these in page transitions
				if(PlayerCursorData.itemDragged != -1)
					ResetCursorDragData();
				// the server's word has to go with it, otherwise the very next draw repopulates the
				// drag straight back out of it and the item stays stuck to the cursor. The server
				// will not correct us for a whole round trip, and by the time it does the view has
				// stopped drawing, so nothing would ever clear it
				ResetCursorDragConfirm();
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

			// Jump held hands the click to whatever the page uses the gesture for, and is an
			// ordinary click on every page that asked for nothing.
			if(jumpmod != DND_JUMPMOD_NONE && IsButtonHeld(bpress, BT_JUMP))
				SetInventory("MenuInput", DND_MENUINPUT_JUMPCLICK);
			else
				SetInventory("MenuInput", DND_MENUINPUT_LCLICK);
		}
		else if(IsButtonPressed(bpress, obpress, BT_ALTATTACK)) {
			GiveInventory("DnD_ClickTicker", 1);
			// Only a refund page takes the right button too, and it has to: right click already
			// means "all of them" there, so jump+right would otherwise spend the lot by accident.
			if((jumpmod == DND_JUMPMOD_REFUND || jumpmod == DND_JUMPMOD_PINREFUND) && IsButtonHeld(bpress, BT_JUMP))
				SetInventory("MenuInput", DND_MENUINPUT_JUMPRCLICK);
			else
				SetInventory("MenuInput", DND_MENUINPUT_RCLICK);
		}
	}
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
		return GetWeaponDrawInfo(itemid - SHOP_WEAPON_BEGIN).flags;
		case TYPE_AMMO:
		return GetAmmoDrawInfo(itemid - SHOP_FIRSTAMMO_INDEX).flags;
		case TYPE_ACCOUNT:
		return OBJ_RESEARCH;
	}
	return 0;
}

int CheckItemRequirements (int req_id, int constraint, int flags) {
	int res = !(flags & OBJ_RESEARCH_ATLEASTONE); // if it has at least one flag set, the logic is inverted here
	for(int i = 0; i < MAX_RESEARCH_REQUIREMENTS; ++i) {
		int req = GetShopItemInfo(req_id, i + DND_SHOPINFO_REQUIREMENT1);
		if(req == -1)
			continue;
		if(CheckResearchStatus(req) < constraint) {
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

int GetShopWeaponDamageTypes(int id) {
	static int WeaponDamageTypes[MAXSHOPWEAPONS] = {
		// 1
		DTYPE_MELEE,
		DTYPE_MELEE,
		DTYPE_MELEE | DTYPE_OCCULT,
		DTYPE_MELEE | DTYPE_FIRE,
		DTYPE_MELEE | DTYPE_OCCULT,
		DTYPE_MELEE | DTYPE_OCCULT,
		
		// 2
		DTYPE_PHYSICAL,
		DTYPE_PHYSICAL,
		DTYPE_ENERGY,
		DTYPE_FIRE,
		DTYPE_PHYSICAL,
		DTYPE_OCCULT,
		DTYPE_PHYSICAL,
		
		// 3 - 1
		DTYPE_PHYSICAL,
		DTYPE_PHYSICAL,
		DTYPE_POISON,
		DTYPE_PHYSICAL,
		DTYPE_ICE,
		DTYPE_OCCULT | DTYPE_MELEE,
		DTYPE_ICE | DTYPE_FIRE | DTYPE_POISON,
		DTYPE_PHYSICAL,
		
		// 3 - 2
		DTYPE_PHYSICAL,
		DTYPE_OCCULT,
		DTYPE_MELEE | DTYPE_LIGHTNING,
		DTYPE_ENERGY,
		DTYPE_LIGHTNING | DTYPE_FIRE,
		DTYPE_PHYSICAL | DTYPE_FIRE,
		DTYPE_PHYSICAL | DTYPE_OCCULT,
		DTYPE_PHYSICAL,

		// 3 - 3
		DTYPE_OCCULT,
		
		// 4 - 1
		DTYPE_PHYSICAL,
		DTYPE_ENERGY,
		DTYPE_OCCULT,
		DTYPE_PHYSICAL,
		DTYPE_PHYSICAL,
		DTYPE_PHYSICAL | DTYPE_POISON,
		DTYPE_ENERGY,
		DTYPE_FIRE,
		
		// 4 - 2
		DTYPE_POISON,
		DTYPE_PHYSICAL,
		DTYPE_OCCULT,
		DTYPE_ENERGY,
		
		// 5 - 1
		DTYPE_PHYSICAL,
		DTYPE_PHYSICAL,
		DTYPE_PHYSICAL,
		DTYPE_FIRE | DTYPE_MELEE,
		DTYPE_OCCULT,
		DTYPE_PHYSICAL,
		DTYPE_ICE,
		DTYPE_PHYSICAL,
		
		// 5 - 2
		DTYPE_ENERGY,
		DTYPE_ICE,
		DTYPE_PHYSICAL,
		DTYPE_PHYSICAL,
		DTYPE_ENERGY | DTYPE_POISON,
		
		// 6 - 1
		DTYPE_ENERGY,
		DTYPE_ENERGY,
		DTYPE_ICE,
		DTYPE_PHYSICAL,
		DTYPE_FIRE,
		DTYPE_LIGHTNING,
		DTYPE_ENERGY,
		DTYPE_OCCULT,
		
		// 6 - 2
		DTYPE_PHYSICAL,
		DTYPE_OCCULT,
		DTYPE_OCCULT,
		
		// 7
		DTYPE_ENERGY,
		DTYPE_PHYSICAL,
		DTYPE_ENERGY,
		DTYPE_ENERGY,
		DTYPE_LIGHTNING,
		DTYPE_ENERGY,
		DTYPE_ENERGY,
		DTYPE_ENERGY,
		
		// 8
		DTYPE_OCCULT,
		DTYPE_OCCULT,
		DTYPE_OCCULT,
		DTYPE_OCCULT
	};
	return WeaponDamageTypes[id];
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

bool HandlePageListening(int curopt, int boxid) {
	bool redraw = false;
	int temp, i;
	// Pages that scroll set this again through ListenScroll below; the ones that don't leave it
	// clear, which is how the bar knows to stand down. Cleared here rather than by the caller so
	// it cannot be forgotten at a call site that only wanted the key handling.
	// Every bar stands down unless the page below asks for it back. A page that mentions only bar 0
	// leaves the second one un-listened, which is what keeps it off every page but the perk tree.
	for(int b = 0; b < DND_SCROLLBAR_COUNT; ++b)
		GetScrollBar(b).listened = false;
	// the redesigned pages scroll their own way
	if(IsChromePage(curopt))
		return HandleChromeListening(curopt, boxid);
	switch(curopt) {
		case MENU_SPELL_FIRE:
		case MENU_SPELL_ICE:
		case MENU_SPELL_LIGHTNING:
		case MENU_SPELL_EARTH:
		case MENU_SPELL_BLACK:
		case MENU_SPELL_ARCANE:
		case MENU_SPELL_CHAOS:
		case MENU_SPELL_COMBAT:
			redraw = HandleSpellPageScroll(curopt - MENU_SPELLTREE_FIRST, boxid);
		break;
		case MENU_SPELL_HOTBAR:
		case MENU_SPELL_GEMS:
			redraw = HandleSpellPageScroll(0, boxid);
		break;
		#ifdef ISAPRILFIRST
		case MENU_SHOP_NFT:
			redraw = ListenScroll(-48, 0, 1, 32);
		break;
		#endif
	}
	return redraw;
}

void ClearInfoPanel() {
	DeleteText(RPGMENUHELPCORNERID);
	DeleteTextRange(RPGMENUDAMAGETYPEID, RPGMENUDAMAGETYPEID + MAX_DAMAGE_TYPES_MENU - 1);
}

str GetWeaponShopIcon(int id) {
	id = ShopTableIdToWeaponTableId(id);
	return Weapons_Data[id].icon;
}

str GetWeaponExplanation(int id) {
	int real_id = ShopTableIdToWeaponTableId(id);
	return GetWeaponTipText(real_id);
}

int ShopScale(int amount, int id) {
	int type = GetItemType(id);
	switch(type) {
		case TYPE_WEAPON:
		return amount * Clamp_Between(GetCVar("dnd_shop_wep_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_AMMO:
		return amount * Clamp_Between(GetCVar("dnd_shop_ammo_scale"), 1, SHOP_SCALE_MAX);
		case TYPE_ACCOUNT:
		return amount * Clamp_Between(GetCVar("dnd_shop_account_scale"), 1, SHOP_SCALE_MAX);
	}
	return 1;
}

int GetShopPrice (int id, int priceflag = 0) {
	int res = 0, chr = 0, shop_scale = Clamp_Between(GetCVar("dnd_shop_scale"), 1, SHOP_SCALE_MAX);
	if(id < MAXSHOPITEMS)
		res = GetShopItemInfo(id, DND_SHOPINFO_PRICE) * shop_scale;
	res = ShopScale(res, id);
	
	if(priceflag & PRICE_INCREASE_STOCK_LOW) {
	
	}
	
	if((GetItemFlags(id) & OBJ_SHOTGUN) && HasClassPerk_Fast(DND_PLAYER_HOBO, 1)) {
		res -= res / 5;
		if(res < 0)
			res = 0;
	}
	
	return res;
}

// returns 0 for buy being possible
bool CanResearch(int respage, int posx) {
	res_info_T module& resData = GetResearchInfo(respage, posx);
	int status = CheckResearchStatus(resData.res_id);
	bool finish_check = status != RES_DONE;
	bool found_check = status == RES_KNOWN;
	bool budget_check = CheckInventory("Budget") >= (resData.res_cost * (IsShotgunResearch(resData.res_id) && HasClassPerk_Fast(DND_PLAYER_HOBO, 1) ? 4 : 5)) / 5;
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
	
	item = GetItemName(id);
	wepcheck = GetWeaponCondition(id);
	
	if(tradeflag & TRADE_BUY) {
		if(type == TYPE_AMMO) { // ammo
			cond2 = (CheckInventory(item) < GetAmmoCapacity(item)) && !PlayerModData[pnum].vals[PSTAT_EX_CANNOTPICKAMMO];
			cond4 = GlobalData.ShopStockRemaining[PlayerNumber()][id] > 0;
		}
		else { // weapon or account
			if(type == TYPE_WEAPON && HasClassPerk_Fast(DND_PLAYER_BERSERKER, 1)) {
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
			cond4 = GlobalData.ShopStockRemaining[PlayerNumber()][id] > 0;
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

void DrawCredits(int pnum, int y_off = 0) {
	HudMessage(s:"\c[Y5]", l:"DND_MENU_CREDITS", s:": \c-$", d:GetPlayerCredit(pnum); HUDMSG_PLAIN, RPGMENUITEMSUBID, CR_WHITE, MenuX(264.1), MenuY(64.0 + y_off), 0.0, 0.0);
}

int GetAmmoToGive(int index) {
	return GetMenuAmmoCount(index - SHOP_FIRSTAMMO_INDEX, AMMOID_COUNT) * GetAmmoGainFactor() / 100;
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

	str ammo = "";

	// special correction for special ammo
	if(itemid >= SHOP_FIRSTAMMOSPECIAL_INDEX) {
		if(id >= SSAM_EXSHELL)
			id = id + 1;
		ammo = GetSpecialAmmoStr(id, AMMOINFO_NAME);
	}
	else
		ammo = GetAmmoInfo(id & 0xFFFF, id >> 16).name;

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
							// Buying a shotgun shifts CountShotgunWeaponsOwned(), which is
							// baked into every OTHER boomstick's cached flat bonus. Only the
							// boomsticks care, so only they get invalidated -- the bought
							// weapon itself rebuilds on its first raise anyway.
							if(IsBoomstick(ShopTableIdToWeaponTableId(itemid)))
								ForceShotgunDamageCaching(pnum);

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
						--GlobalData.ShopStockRemaining[pnum][itemid];
					}
				} while (givefull && !buystatus);
				SendOwnerScript("DnD Sync Shop Stock", pnum, pnum, itemid, GlobalData.ShopStockRemaining[pnum][itemid]);
				//sound (mostly)
				if (buystatus && loopnumber == 1)
					ShowPopup(buystatus, false, 0);
				else {
					if(tradeflag & TRADE_WEAPON)
						LocalAmbientSound("weapons/pickup", 127);
					else if(tradeflag & TRADE_AMMO)
						LocalAmbientSound("items/ammo", 127);
					else if(tradeflag & TRADE_ACCOUNT) {
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
					SendOwnerScript("DnD Clear Homing Lock-on", PlayerNumber());
				}

				// this works on weapon table ids, so map it there to use
				ResetWeaponStats(ShopTableIdToWeaponTableId(itemid));
				TakeInventory(GetWeaponCondition(itemid), 1);
				TakeInventory(GetItemName(itemid), 1);

				// selling a shotgun shifts CountShotgunWeaponsOwned(), which is baked into
				// every OTHER boomstick's cached flat bonus -- and only theirs
				if(IsBoomstick(ShopTableIdToWeaponTableId(itemid)))
					ForceShotgunDamageCaching(pnum);
				
				// reset buffs of weapon
				GiveCredit(price);
				SendOwnerScript("DnD Menu Sell Popup Clear", PlayerNumber());
			}
		}
	}
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
			if(GlobalItemStorage.PlayerInventoryList[pnum][i].item_type == DND_ITEM_ORB && GlobalItemStorage.PlayerInventoryList[pnum][i].item_subtype == DND_ORB_ASSIMILATION) {
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
			MenuHudSize(HUDMAX_X, HUDMAX_Y);
			SetFont("NMENUFNT");
			MenuClipRect((x >> 16) - 8, (y >> 16) - 8, 96, 64, 96);
			HudMessage(s:text; HUDMSG_PLAIN, RPGMENUCURSORID + 1, -1, MenuX(x + 0.1), MenuY(y + 0.1), 0.0);
			MenuClipRect(0, 0, 0, 0, 0);
		}
	}
}

void HandleAmmoPurchase(int pnum, int slot, int itemid, int shop_index, bool givefull, bool isSpecialAmmo) {
	if(!CheckItemRequirements(shop_index, RES_DONE, GetItemFlags(shop_index))) {
		// not done, so we can't give this
		ShowNeedResearchPopup();
	}
	else {
		if(shop_index > SHOP_AMMO_SLUGSHELL && isSpecialAmmo)
			++itemid;

		int price = GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW);
		int buystatus = CanTrade(pnum, shop_index, TRADE_BUY, price);
		auto a_info = GetAmmoInfo(slot, itemid);

		if(!buystatus) {
			int amt = GetAmmoToGive(shop_index), count = 1;
			
			// if we are maxing the ammo
			if(givefull) {
				// count was overcounting before
				--count;
				if(slot != -1)
					count += (GetAmmoCapacity(a_info.name) - CheckInventory(a_info.name)) / amt;
				else
					count += (GetAmmoCapacity(GetSpecialAmmoStr(itemid, AMMOINFO_NAME)) - CheckInventory(GetSpecialAmmoStr(itemid, AMMOINFO_NAME))) / amt;
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
						buystatus = GetAmmoCapacity(a_info.name) - (CheckInventory(a_info.name) + amt);
					else
						buystatus = GetAmmoCapacity(GetSpecialAmmoStr(itemid, AMMOINFO_NAME)) - (CheckInventory(GetSpecialAmmoStr(itemid, AMMOINFO_NAME)) + amt);
					if(buystatus) {
						// the remainder is only added if it is affordable too
						count = Max(buystatus * GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW) / GetAmmoToGive(shop_index), 1);
						if(price + count <= GetPlayerCredit(pnum)) {
							amt += buystatus;
							price += count;
						}
					}
				}
			}
			if(amt > GlobalData.ShopStockRemaining[pnum][shop_index]) {
				// rebalance the amount so we don't go in debt...
				amt = GlobalData.ShopStockRemaining[pnum][shop_index];
				price = GetShopPrice(shop_index, PRICE_INCREASE_STOCK_LOW) * amt / GetAmmoToGive(shop_index);
			}
			
			// we're OK now
			TakeCredit(price);
			if(slot != -1)
				GiveInventory(a_info.name, amt);
			else
				GiveInventory(GetSpecialAmmoStr(itemid, AMMOINFO_NAME), amt);
			GlobalData.ShopStockRemaining[pnum][shop_index] -= amt;
			SendOwnerScript("DnD Sync Shop Stock", pnum, pnum, shop_index, GlobalData.ShopStockRemaining[pnum][shop_index]);
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

// deepcopy to avoid accidental overriding
//
// The bound is MAX_PANE_BOXES, not MAX_MENU_BOXES: this writes into the runtime pane, while
// MAX_MENU_BOXES belongs to the bp[][] table LoadPane reads FROM. The guard is live rather than
// commented because overflowing here runs off the end of MenuRectangles and straight over cursize.
void AddBoxToPane(menu_pane_T module& p, rect_T module& box) {
	if(p.cursize < MAX_PANE_BOXES) {
		p.MenuRectangles[p.cursize].topleft_x = box.topleft_x;
		p.MenuRectangles[p.cursize].topleft_y = box.topleft_y;
		p.MenuRectangles[p.cursize].botright_x = box.botright_x;
		p.MenuRectangles[p.cursize].botright_y = box.botright_y;
		++p.cursize;
	}
	else
		Log(s:"AddBoxToPane: pane is full at ", d:MAX_PANE_BOXES, s:" boxes, dropping one.");
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
			{ 289.0, 181.0, 108.0, 174.0 }, // exotic
			{ 289.0, 165.0, 108.0, 158.0 }, // misc
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
		// stat exotic
		{
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// stat misc
		{
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// perk -- one row per archetype, in PERK_ARCH_* order, DND_PERKINDEX_ROWGAP apart
		{
			{ 296.0, 238.0, 80.0, 226.0 }, // acrobacy
			{ 296.0, 220.0, 80.0, 208.0 }, // assassination
			{ 296.0, 202.0, 80.0, 190.0 }, // cunning
			{ 296.0, 184.0, 80.0, 172.0 }, // endurance
			{ 296.0, 166.0, 80.0, 154.0 }, // martialist
			{ 296.0, 148.0, 80.0, 136.0 }, // perception
			{ 296.0, 130.0, 80.0, 118.0 }, // tormentor
			{ 296.0, 112.0, 80.0, 100.0 }, // magi
			{ 296.0, 96.0, 80.0, 84.0 },   // dash toggle, below the rule
			{ -1, -1, -1, -1 }
		},
		// perk tree: acrobacy
		{
			{ -1, -1, -1, -1 }
		},
		// perk tree: assassination
		{
			{ -1, -1, -1, -1 }
		},
		// perk tree: cunning
		{
			{ -1, -1, -1, -1 }
		},
		// perk tree: endurance
		{
			{ -1, -1, -1, -1 }
		},
		// perk tree: martialist
		{
			{ -1, -1, -1, -1 }
		},
		// perk tree: perception
		{
			{ -1, -1, -1, -1 }
		},
		// perk tree: tormentor
		{
			{ -1, -1, -1, -1 }
		},
		// perk tree: magi
		{
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
			{ 289.0, 101.0, 224.0, 94.0 }, // dungeons
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
			{ 167.0, 193.0, 117.0, 143.0 },
			{ 112.0, 129.0, 62.0, 79.0 },

			// flasks -- 2x2, mirrored like every rect here (480 - screen x, 320 - screen y), so the
			// LARGER number is the top left. 32px boxes centred on the grid constants.
			{ 184.0, 125.0, 152.0, 93.0 },
			{ 148.0, 125.0, 116.0, 93.0 },
			{ 184.0, 85.0, 152.0, 53.0 },
			{ 148.0, 85.0, 116.0, 53.0 },

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
		// dungeons
		{
			{ 184.0, 260.0, 144.0, 220.0 },
			{ 198.0, 212.0, 130.0, 204.0 },
			{ 296.0, 280.0, 296.0 - CRAFTING_PAGEARROW_XSIZE, 278.0 - CRAFTING_PAGEARROW_YSIZE },
			{ -1, -1, -1, -1 }
		},
		// shop -- abilities and artifacts no longer have a counter, so the rows below them moved up
		{
			{ 289.0, 229.0, 179.0, 221.0 }, // wep
			{ 289.0, 213.0, 162.0, 205.0 }, // ammo
			{ 289.0, 197.0, 183.0, 189.0 }, // account
			#ifdef ISAPRILFIRST
				{ 289.0, 181.0, 188.0, 173.0 }, // nft
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
		// spells -- one row per tree, in DND_SKILLTREE_* order
		{
			{ 296.0, 246.0, 80.0, 234.0 }, // Fire
			{ 296.0, 230.0, 80.0, 218.0 }, // Cold
			{ 296.0, 214.0, 80.0, 202.0 }, // Lightning
			{ 296.0, 198.0, 80.0, 186.0 }, // Earth
			{ 296.0, 182.0, 80.0, 170.0 }, // Black
			{ 296.0, 166.0, 80.0, 154.0 }, // Arcane
			{ 296.0, 150.0, 80.0, 138.0 }, // Chaos
			{ 296.0, 134.0, 80.0, 122.0 }, // Combat
			{ 296.0, 110.0, 80.0, 98.0 }, // Hotbar Assignments
			{ 296.0, 94.0, 80.0, 82.0 }, // Gem Sockets
			{ -1, -1, -1, -1 }
		},
		// The tree pages build their own pane in HandleSpellTreeDraw, so they own no authored rect.
		// spell tree: fire
		{
			{ -1, -1, -1, -1 }
		},
		// spell tree: cold
		{
			{ -1, -1, -1, -1 }
		},
		// spell tree: lightning
		{
			{ -1, -1, -1, -1 }
		},
		// spell tree: earth
		{
			{ -1, -1, -1, -1 }
		},
		// spell tree: black
		{
			{ -1, -1, -1, -1 }
		},
		// spell tree: arcane
		{
			{ -1, -1, -1, -1 }
		},
		// spell tree: chaos
		{
			{ -1, -1, -1, -1 }
		},
		// spell tree: combat
		{
			{ -1, -1, -1, -1 }
		},
		// hotbar assignments -- builds its own pane, like the trees
		{
			{ -1, -1, -1, -1 }
		},
		// gem sockets -- the same
		{
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
	// this is the top left box, ie 0, 0 -- one past the grid is the sort button
	static rect_T bp[MAX_INVENTORY_BOXES + 1];

	// construct for first time, these can be constructed easily from base coordinates
	if(!IsSet(PaneSetup, INVENTORY_SETUP_BIT)) {
		for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i)
			for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_x = INVENTORYBOX_BASEX_RECT - 32.0 * j;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].topleft_y = INVENTORYBOX_BASEY_RECT - 32.0 * i;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_x = INVENTORYBOX_BASEX_RECT - 32.0 * j - 32.0;
				bp[i * MAXINVENTORYBLOCKS_VERT + j].botright_y = INVENTORYBOX_BASEY_RECT - 32.0 * i - 32.0;
			}

		bp[MAX_INVENTORY_BOXES].topleft_x = SORTBUTTON_INVVIEW_RECT_TX;
		bp[MAX_INVENTORY_BOXES].topleft_y = SORTBUTTON_INVVIEW_RECT_TY;
		bp[MAX_INVENTORY_BOXES].botright_x = SORTBUTTON_INVVIEW_RECT_BX;
		bp[MAX_INVENTORY_BOXES].botright_y = SORTBUTTON_INVVIEW_RECT_BY;

		PaneSetup = SetBit(PaneSetup, INVENTORY_SETUP_BIT);
	}
	return bp[id];
}

rect_T module& LoadStashViewRect(int id) {
	// stash cells, then inventory cells, the page tabs, the orb tab and the two sort buttons, each where the art draws it
	static rect_T bp[2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES + 3];
	if(!IsSet(PaneSetup, STASH_SETUP_BIT)) {
		for(int i = 0; i < 2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES + 3; ++i) {
			bp[i].topleft_x = MchStashRectEdge(i, 0);
			bp[i].topleft_y = MchStashRectEdge(i, 1);
			bp[i].botright_x = MchStashRectEdge(i, 2);
			bp[i].botright_y = MchStashRectEdge(i, 3);
		}
		PaneSetup = SetBit(PaneSetup, STASH_SETUP_BIT);
	}
	return bp[id];
}

rect_T module& LoadTradeViewRect(int id) {
	// their offer, our offer, our inventory, then confirm and cancel, each where the art draws it
	static rect_T bp[3 * MAX_INVENTORY_BOXES + 2];
	if(!IsSet(PaneSetup, TRADEVIEW_SETUP_BIT)) {
		for(int i = 0; i < 3 * MAX_INVENTORY_BOXES + 2; ++i) {
			bp[i].topleft_x = MchTradeRectEdge(i, 0);
			bp[i].topleft_y = MchTradeRectEdge(i, 1);
			bp[i].botright_x = MchTradeRectEdge(i, 2);
			bp[i].botright_y = MchTradeRectEdge(i, 3);
		}
		PaneSetup = SetBit(PaneSetup, TRADEVIEW_SETUP_BIT);
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
	// + 1 for the sort button past the grid
	for(int i = 0; i < MAX_INVENTORY_BOXES + 1; ++i) {
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
	// + 2 for the two sort buttons past the tabs
	for(int i = 0; i < 2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES + 3; ++i) {
		auto r = LoadStashViewRect(i);
		if(r.topleft_x != -1) {
			// Log(s:"Adding box: ", f:bp[menu_page][i].topleft_x, s: " ", f:bp[menu_page][i].topleft_y, s: " ", f:bp[menu_page][i].botright_x, s: " ", f:bp[menu_page][i].botright_y);
			AddBoxToInventory(p, r);
		}
		else
			break;
	}
}

// Bounded by MAX_MENU_BOXES because it is walking a bp[] ROW, not filling the pane to capacity.
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
	// a redesigned page keeps this row's box count and order, only where the boxes sit changes
	if(IsChromePage(menu_page))
		LoadChromePane(p, menu_page);
}

// The early out is a cheap reject before the rect scan, so its bounds have to CONTAIN every rect the
// pane owns -- including the ones outside the grid. A widget outside these bounds is not merely
// mispositioned, it is dead: the scan never runs, so its rect is never consulted, and it looks like
// the click is being swallowed. The sort button hangs below the grid, so the floor is tied to the
// button's own bottom edge rather than to a literal that stops matching the moment the button moves.
int GetTriggeredBoxOnInventoryPane(menu_inventory_T module& p, int mx, int my) {
	if(mx >= 400.0 || my <= SORTBUTTON_INVVIEW_RECT_BY - 1.0 || mx <= 82.0 || my >= 260.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

int GetTriggeredBoxOnTradePane(menu_inventory_T module& p, int mx, int my) {
	// the three grids span the frame's full width, past both edges of the 4:3 cursor space
	if(mx >= 540.0 || mx <= -60.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

// Same rule as GetTriggeredBoxOnInventoryPane. The floor used to be 27, one under the inventory
// grid's bottom row -- correct while the grid was the lowest thing in the pane, and wrong the moment
// the inventory sort button moved onto the panel border below it. All but the button's topmost line
// was rejected here before the rect scan could see it.
int GetTriggeredBoxOnStashPane(menu_inventory_T module& p, int mx, int my) {
	// the two panels side by side, headings and sort buttons to the page tabs below the stash
	if(mx >= 454.0 || my <= 92.0 || mx <= 18.0 || my >= 239.0)
		return MAINBOX_NONE;
	for(int i = 0; i < p.cursize; ++i) {
		if(point_in_inventory_box(p.MenuRectangles[i], mx, my))
			return i + 1;
	}
	return MAINBOX_NONE;
}

void DrawBoxText(str msg, bool language_lookup, int boxid, int thisboxid, int id, int x, int y, str hcolor, str lcolor) {
	str color = lcolor;
	if(boxid == thisboxid && !CheckInventory("DnD_HighlightBlocker"))
		color = hcolor;

	if(language_lookup)
		HudMessage(s:color, l:msg, s:"\c-"; HUDMSG_PLAIN, id, CR_WHITE, MenuX(x), MenuY(y), 0.0, 0.0);
	else
		HudMessage(s:color, s:msg, s:"\c-"; HUDMSG_PLAIN, id, CR_WHITE, MenuX(x), MenuY(y), 0.0, 0.0);
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
		id = GetMenuAmmoIndexMap(slot, boxid + begin_index - 1);
		
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

// ---------------------------------------------------------------- the perk tree
// Two pages. MENU_PERK indexes the archetypes; one MENU_PERK_* page per archetype draws its tree.
//
// A tree page lays itself out from the perk table rather than from a bp[] row, so it builds its own
// boxes here on every redraw. The BOX ORDER is the contract with the click handler in
// "DND Server Box Receive": box n is entry n of GetPerkArchList(arch), then the back arrow last.
// Rows scrolled out of view still take their box -- parked where the cursor cannot reach it -- so
// the numbering can never shift under the server, which has no idea where the client scrolled to.

// Jump + click pins the panel to the row under the cursor; any plain click releases it.
//
// Called from the menu loop DIRECTLY after the input is produced, and that placement is the whole
// point. MenuInput is cleared twice per iteration -- once before the input is read, and again the
// moment the server's DND_ACK comes back. On a listen server the ack lands within the same
// iteration, so anything reading MenuInput on a LATER frame sees zero. The first version of this
// lived in HandlePerkPageScroll, which runs before the input exists, and so did nothing at all.
bool HandlePerkPin(int arch, int boxid, int input) {
	perk_scroll_T module& ps = GetPerkScroll();

	if(input == DND_MENUINPUT_JUMPCLICK) {
		EnsurePerkArchLists();

		perk_arch_list_T module& plist = GetPerkArchList(arch);
		if(boxid >= MBOX_1 && boxid < MBOX_1 + plist.count) {
			int pk = plist.perk[boxid - MBOX_1];

			// Rewind only for a DIFFERENT perk. Pinning the one already being read is the whole
			// gesture -- scroll partway down a long description, then pin so the cursor cannot move
			// it -- and rewinding there would undo the reading the pin exists to protect.
			if(ps.shown_perk != pk)
				SetScrollBarPos(DND_PERKBAR_PANEL, 0);

			ps.shown_perk = pk;
			ps.pinned = true;
			LocalAmbientSound("RPG/MenuChoose", 127);
			return true;
		}
	}
	else if(input == DND_MENUINPUT_LCLICK && ps.pinned) {
		ps.pinned = false;
		return true;
	}

	return false;
}

int GetInvestmentCost(int pnum, int page, int menu_res_id, int res_id) {
	// base budget cost's 15% + 5% is applied per investment on top of a base of 1k credits to the previous value
	// let (5 + x / 10) = y
	// c * (100 + k * (5 + x / 10)) / 100 -- replace c here with k - 1's value
	// c * (100 + (k - 1) * (5 + x / 10)) / 100 * (100 + k * (5 + x / 10)) / 100
	// c * (100 + (k - 1) * y) / 100 * (100 + k * y) / 100
	res_info_T module& resData = GetResearchInfo(page, menu_res_id);
	int k = ResearchInvestments[pnum][res_id];
	if(!k)
		return BASE_INVESTMENT_COST;
	else if(k == 1)
		return BASE_INVESTMENT_COST * (105 + 3 * resData.res_cost / 20) / 100;
		
	int y = 5 + 3 * resData.res_cost / 20;
	return (BASE_INVESTMENT_COST * (100 + (k - 1) * y) / 100) * (100 + k * y) / 100;
}

void HandleResearchPageInput(int pnum, int page, int boxid) {
	bool buystatus = 0;
	int curposx = MenuInputData[pnum][DND_MENUINPUT_LRPOS];
	res_info_T module& resData = GetResearchInfo(page, curposx);
	if(HasLeftClicked(pnum)) {
		if(boxid == MBOX_1) {
			buystatus = CanResearch(page, curposx);
			if(!buystatus) {
				TakeBudget(resData.res_cost);
				LocalAmbientSound("items/research", 127);
				CompleteResearch(resData.res_id);
			}
			else
				ShowPopup(CheckInventory("DnD_PopupId"), false, 0);
		}
		else if(boxid == MBOX_2) {
			// investment option
			// check if it's not found already, no point investing into finding it if its there already
			if(CheckResearchStatus(resData.res_id) != RES_NA) {
				// popup, you found this research already
				ShowPopup(POPUP_ALREADYFOUND, false, 0);
			}
			else if(ResearchFlags[resData.res_id].res_flags & RESF_NODROP) {
				// popup, we dont know this one yet
				ShowPopup(POPUP_NARESEARCH, false, 0);
			}
			else {
				int resid = resData.res_id;
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

// the shadowed picture an empty armor slot shows, so the player can tell the slots apart
str GetArmorSlotSilhouette(int armor_slot) {
	switch(armor_slot) {
		case DND_ITEM_BODYARMOR:
		return "ARMBAK";
		case DND_ITEM_HELM:
		return "HELMBAK";
		case DND_ITEM_BOOT:
		return "BOOTBAK";
		case DND_ITEM_SPECIALTY_DOOMGUY:
		return "SCARDBAK";
		case DND_ITEM_SPECIALTY_MARINE:
		return "DOGTGBAK";
		case DND_ITEM_SPECIALTY_HOBO:
		return "SUNGLBAK";
		case DND_ITEM_SPECIALTY_PUNISHER:
		return "CIGRBAK";
		case DND_ITEM_SPECIALTY_WANDERER:
		return "PRINGBAK";
		case DND_ITEM_SPECIALTY_CYBORG:
		return "PCORBAK";
		case DND_ITEM_SPECIALTY_BERSERKER:
		return "BELTBAK";
	}
	return "CLAWBAK";
}

// the cell picture for a grid box: its highlight first, else the item on it (usable or not), else empty
str GetInventoryCellPic(int pnum, int bid, int boff, int source) {
	int temp, filter, itype;
	if(IsValidBox(bid) && InventoryBoxLit[bid + boff] == BOXLIT_STATE_CURSORON)
		return "LDTBOXS";
	if(InventoryBoxLit[bid + boff] == BOXLIT_STATE_BAD)
		return "LDTBOXN";
	if(CheckActorInventory(pnum + P_TIDSTART, "DnD_Trade_Confirmed"))
		return "LDTBOXCN";
	if(IsValidBox(bid) && InventoryBoxLit[bid + boff] == BOXLIT_STATE_CLICK)
		return "LDTBOXC";
	if((temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source))) {
		// if lvl requirement is satisfied draw them normal -- topbox is +1 of actual index
		filter = CheckInventory("DnD_Inventory_Filter");
		itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, temp - 1, -1, source);
		if(filter && itype != GetFilterItemType(filter))
			return "LDTBOXR";
		// dont subject keys to level cap, if user wants to go there, let em
		if(GetItemSyncValue(pnum, DND_SYNC_ITEMLEVEL, temp - 1, -1, source) <= GetLevel() || itype == DND_ITEM_DUNGEONKEY)
			return "LDTBOXO";
		return "LDTBOXR";
	}
	return "LDTBOX";
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
			MenuX(basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + (GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, bid, -1, source) - 1) * ITEMOFFSETSCALE), 
			MenuY(basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + temp), 
			0.0, 
			0.0
		);
	
		temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, bid, -1, source);
		if(temp) {
			SetFont("NMENUFNT");
			HudMessage(d:temp; HUDMSG_PLAIN, idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2, CR_WHITE, MenuX(basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * itemskipx + 14.2), MenuY(basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * itemskipy + 8.0), 0.0, 0.0);
		}
		else
			DeleteText(idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2);
	}
	else { // refresh picture if swapped
		DeleteText(idbase - bid - boff - MAX_INVENTORY_BOXES - 2);
		DeleteText(idbase - bid - boff - 2 * MAX_INVENTORY_BOXES - 2);
	}

	// gray inventory chunks
	str cellpic = GetInventoryCellPic(pnum, bid, boff, source);
	// the inventory and stash grids on a redesigned page draw the frame's own cells in the same places
	if(basex == INVENTORYBOX_BASEX) {
		MchInventoryCell(pnum, cellpic, idbase - bid - boff - 1, idx, idy, boff, source, 0);
		return;
	}
	if(basex == STASHVIEW_STASH_BASEX || basex == STASHVIEW_INV_BASEX) {
		MchInventoryCell(pnum, cellpic, idbase - bid - boff - 1, idx, idy, boff, source, (basex == STASHVIEW_STASH_BASEX) ? 1 : 2);
		return;
	}
	if(basex == TRADEVIEW_THEIRS_BASEX || basex == TRADEVIEW_OURS_BASEX || basex == TRADEVIEW_INV_BASEX) {
		MchInventoryCell(pnum, cellpic, idbase - bid - boff - 1, idx, idy, boff, source, (basex == TRADEVIEW_THEIRS_BASEX) ? 3 : (basex == TRADEVIEW_OURS_BASEX) ? 4 : 5);
		return;
	}
	SetFont(cellpic);
	HudMessage(s:"A"; HUDMSG_PLAIN, idbase - bid - boff - 1, CR_WHITE, MenuX(basex - (MAXINVENTORYBLOCKS_VERT - idy - 1) * skip), MenuY(basey - (MAXINVENTORYBLOCKS_HORIZ - idx - 1) * skip), 0.0);
}

// Which stacked layout the caller is drawing. A box index handed to UpdateDraggedItemLitBoxes spans
// every grid in the view, so the occupancy test has to be told which grid each index lands in. One
// blanket source used to be passed for the whole view, which meant the stash view asked the stash
// (page 0 at that, the page was never packed in) about indices belonging to the player inventory
// grid -- those always answered "empty", so every box lit green no matter what was under it.
enum {
	DND_LITVIEW_INVENTORY,
	DND_LITVIEW_STASH,
	DND_LITVIEW_TRADE
};

// grid order per view mirrors the DoInventoryBoxDraw calls in each Handle*View function
int GetLitBoxSource(int view, int grid) {
	if(view == DND_LITVIEW_STASH)
		return grid ? DND_SYNC_ITEMSOURCE_PLAYERINVENTORY : (DND_SYNC_ITEMSOURCE_STASH | ((CheckInventory("DnD_PlayerCurrentPage") - 1) << 16));
	if(view == DND_LITVIEW_TRADE)
		return grid == 2 ? DND_SYNC_ITEMSOURCE_PLAYERINVENTORY : DND_SYNC_ITEMSOURCE_TRADEVIEW;
	return DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
}

int GetLitBoxOwner(int view, int grid, int pnum) {
	// trade draws the other player's offering as its top grid
	if(view == DND_LITVIEW_TRADE && !grid)
		return GetTradee();
	return pnum;
}

// min is included max is excluded
// one edge of a grid box in the cursor space: 0 top left x, 1 top left y, 2 bottom right x, 3 bottom right y
int GetDragRectEdge(int view, int b, int edge) {
	if(view == DND_LITVIEW_STASH) {
		rect_T module& rs = LoadStashViewRect(b);
		return (edge == 0) ? rs.topleft_x : (edge == 1) ? rs.topleft_y : (edge == 2) ? rs.botright_x : rs.botright_y;
	}
	if(view == DND_LITVIEW_TRADE) {
		rect_T module& rt = LoadTradeViewRect(b);
		return (edge == 0) ? rt.topleft_x : (edge == 1) ? rt.topleft_y : (edge == 2) ? rt.botright_x : rt.botright_y;
	}
	rect_T module& ri = LoadInventoryViewRect(b);
	return (edge == 0) ? ri.topleft_x : (edge == 1) ? ri.topleft_y : (edge == 2) ? ri.botright_x : ri.botright_y;
}

// A held item sits centred on the cursor, snapped to the nearest cells inside the hovered grid (Diablo II style);
// returns the box its grabbed cell lands on, which is what the server already places by.
int GetDragAnchorBox(int boxid, int view) {
	int src = PlayerCursorData.itemDragInfo.topboxid, grab = PlayerCursorData.itemDragInfo.click_box;
	int grids = (view == DND_LITVIEW_STASH) ? 2 : (view == DND_LITVIEW_TRADE) ? 3 : 1;
	if(src == -1 || grab == -1 || boxid == MAINBOX_NONE || boxid > grids * MAX_INVENTORY_BOXES)
		return boxid;
	int b = boxid - 1, cell = b % MAX_INVENTORY_BOXES;
	int w = PlayerCursorData.itemDragInfo.size_x, h = PlayerCursorData.itemDragInfo.size_y;
	// where in the hovered box the cursor is, 0 to 1 -- the cursor space runs mirrored
	int tlx = GetDragRectEdge(view, b, 0), tly = GetDragRectEdge(view, b, 1);
	int fx = FixedDiv(tlx - (PlayerCursorData.posx & MMASK), tlx - GetDragRectEdge(view, b, 2));
	int fy = FixedDiv(tly - (PlayerCursorData.posy & MMASK), tly - GetDragRectEdge(view, b, 3));
	// the item's top left corner sits half its size up and left of the cursor, rounded to the nearest cell
	int left = (((cell % MAXINVENTORYBLOCKS_VERT) << 16) + fx - (w << 15) + 0.5) >> 16;
	int top = (((cell / MAXINVENTORYBLOCKS_VERT) << 16) + fy - (h << 15) + 0.5) >> 16;
	left = Clamp_Between(left, 0, MAXINVENTORYBLOCKS_VERT - w);
	top = Clamp_Between(top, 0, MAXINVENTORYBLOCKS_HORIZ - h);
	return b - cell + top * MAXINVENTORYBLOCKS_VERT + left + grab - src + 1;
}

void UpdateDraggedItemLitBoxes(int boxid, int min_box, int max_box, int pnum, int view) {
	// update the area being hovered wrt item being dragged
	int temp = PlayerCursorData.itemDragInfo.topboxid;
	--boxid;
	if(boxid == -1 || boxid >= max_box || boxid < min_box)
		return;

	if(temp != -1) {
		int idx;
		int s, p;

		// The hovered box decides which grid we are in, and a dragged item must stay inside it --
		// clamping to the whole view let a tall or wide item spill out of the stash grid into the
		// player inventory grid stacked below it, lighting boxes it could never actually occupy.
		int hov_grid = boxid / MAX_INVENTORY_BOXES;
		int gmin = hov_grid * MAX_INVENTORY_BOXES;
		int gmax = gmin + MAX_INVENTORY_BOXES;
		if(gmin < min_box)
			gmin = min_box;
		if(gmax > max_box)
			gmax = max_box;

		// boxid is already where the grabbed cell lands (GetDragAnchorBox), so the footprint starts the grab offset back
		boxid += temp - PlayerCursorData.itemDragInfo.click_box;

		for(p = 0; p < PlayerCursorData.itemDragInfo.size_y; ++p) {
			for(s = 0; s < PlayerCursorData.itemDragInfo.size_x; ++s) {
				idx = boxid + s + p * MAXINVENTORYBLOCKS_VERT;

				// never light a box outside the hovered grid, or past the end of InventoryBoxLit
				if(idx < gmin || idx >= gmax)
					continue;

				// ask the grid this index actually belongs to, not the view as a whole
				// if this id isn't occupied by another item and its color scheme, color it
				if(GetItemSyncValue(GetLitBoxOwner(view, hov_grid, pnum), DND_SYNC_ITEMTOPLEFTBOX, idx - hov_grid * MAX_INVENTORY_BOXES, -1, GetLitBoxSource(view, hov_grid)) - 1 == -1) {
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
			// click_box is where inside the item the player grabbed it: UpdateDraggedItemLitBoxes
			// lights up (hovered box + topboxid - click_box), so getting it wrong shifts the whole
			// lit region somewhere unrelated. It has to come from prevclick, the box the click
			// actually landed on, NOT from boxid, which is wherever the cursor happens to be when
			// this first runs. Those are the same box only if the cursor has not moved since the
			// click, and the selection does not arrive until a round trip later -- so on a quick
			// click-and-drag at high ping boxid was simply the wrong box.
			if(PlayerCursorData.itemDragInfo.click_box == -1) {
				if(PlayerCursorData.itemDragInfo.confirmed.valid) {
					// the server vouches for this view, so draw only what it says we hold. Our own
					// arrays can still be mid-sync here and must not get a say -- inferring from
					// them is what used to leave a phantom item on the cursor.
					// Matched on offset, which names the grid within a stacked view. Not on source:
					// a held stash item keeps the page it was picked up on packed into its source,
					// so that stops matching the moment the player flips pages while holding it.
					//
					// "Holding something" is size_x, NOT image. Image id 0 is a real image -- it is
					// IIMG_SC_1, the first small charm -- so testing the image here meant the server
					// could vouch for a held item and be read as vouching for an empty hand. One
					// small charm in three simply refused to come off the grid: no item on the
					// cursor, nothing to drop. Width is 0 only in the payload the server sends for
					// an empty hand, and every real item is at least one box wide.
					if(PlayerCursorData.itemDragInfo.confirmed.offset == offset && PlayerCursorData.itemDragInfo.confirmed.size_x) {
						PlayerCursorData.itemDragged = PlayerCursorData.itemDragInfo.confirmed.image;
						PlayerCursorData.itemDragInfo.size_x = PlayerCursorData.itemDragInfo.confirmed.size_x;
						PlayerCursorData.itemDragInfo.size_y = PlayerCursorData.itemDragInfo.confirmed.size_y;
						PlayerCursorData.itemDragInfo.topboxid = PlayerCursorData.itemDragInfo.confirmed.topboxid;
						PlayerCursorData.itemDragInfo.source = PlayerCursorData.itemDragInfo.confirmed.source;
						PlayerCursorData.itemDragInfo.click_box = prevclick - offset;
					}
				}
				else {
					// any view the server does not vouch for keeps deriving the cursor locally
					PlayerCursorData.itemDragged = GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, temp, -1, source);
					PlayerCursorData.itemDragInfo.size_x = wt;
					PlayerCursorData.itemDragInfo.size_y = ht;
					PlayerCursorData.itemDragInfo.topboxid = temp;
					PlayerCursorData.itemDragInfo.source = source;
					PlayerCursorData.itemDragInfo.click_box = prevclick - offset;
					//Log(s:"click box ", d:prevclick - offset, s: " tpbxid ", d:temp);
				}
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

int GetMenuViewFlags() {
	return CheckInventory("DnD_InventoryView") | (CheckInventory("DnD_StashView") << 1) | (CheckInventory("InTradeView") << 2);
}

void HandleInventoryView(int boxid) {
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	MchInventoryPanel(RPGMENUINVENTORYID);
	
	PlayerCursorData.itemDraggedStashSize = false;
	
	ResetInventoryLitState(0, MAX_INVENTORY_BOXES);
	
	//CleanInventoryInfo();
	int pnum = PlayerNumber();
	UpdateDraggedItemLitBoxes(boxid, 0, MAX_INVENTORY_BOXES, pnum, DND_LITVIEW_INVENTORY);

	// The sort button's id is one past the grid, and the box drawer reads boxid - 1 straight into
	// the item array -- handing it that would index off the end. It is not a grid box, so as far as
	// the grid is concerned nothing is hovered.
	int gridboxid = boxid > MAX_INVENTORY_BOXES ? MAINBOX_NONE : boxid;

	for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(gridboxid, prevclick, i, j, INVENTORYBOX_BASEX, INVENTORYBOX_BASEY, 32.0, RPGMENUINVENTORYID, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, pnum, HUDMAX_X, HUDMAX_Y);
		}
	}

	MchInventorySort(boxid == INVSORT_BOXID, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 20);
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

void HandleMenuItemDrop(int pnum, int boxid, int source) {
	int itype = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1, -1, source);
	if(itype == DND_ITEM_NULL)
		return;

	int epos = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, boxid - 1, -1, source);
	DropItemToField(pnum, boxid - 1, true, source);
	ACS_NamedExecuteAlways("DnD Save Player Item Data", 0, pnum | (CheckInventory("DnD_CharacterID") << 16), boxid - 1, source);
	PlayItemDropSound(itype, epos, true);
}

void HandleInventoryViewClicks(int pnum, int boxid, int choice) {
	int bid;
	int epos, ipos, temp;
	// The sort button's id is one past the grid, so it must never reach anything that reads boxid
	// as an item slot -- m2 included, since HandleM2Inputs indexes straight off it.
	if(boxid == INVSORT_BOXID) {
		if(choice == DND_MENUINPUT_LCLICK && !CheckInventory("DnD_SortCooldown")) {
			// a held box id means nothing once the grid is repacked under it
			SetInventory("DnD_SelectedInventoryBox", 0);
			LocalAmbientSound("RPG/MenuChoose", 127);
			ACS_NamedExecuteAlways("DnD Sort Inventory", 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0);
		}
		return;
	}

	if(choice == DND_MENUINPUT_LCLICK) {
		if(boxid != MAINBOX_NONE) {
			// m1
			if(!CheckInventory("DnD_SelectedInventoryBox")) {
				if(GetPlayerInput(-1, INPUT_BUTTONS) & BT_CROUCH)
					HandleMenuItemDrop(pnum, boxid, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				// An empty box has nothing to pick up, so picking it up is a no-op that lights a
				// box and then eats the next click outside. Move into a gap the other way round:
				// take the item first, then click where it should go.
				else if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) == DND_ITEM_NULL)
					return;
				else
					SetInventory("DnD_SelectedInventoryBox", boxid);
			}
			else if(boxid != CheckInventory("DnD_SelectedInventoryBox")) {
				// if neither are empty spots
				bool boxidon = GlobalItemStorage.PlayerInventoryList[pnum][boxid - 1].item_type != DND_ITEM_NULL;
				bool prevselecton = GlobalItemStorage.PlayerInventoryList[pnum][CheckInventory("DnD_SelectedInventoryBox") - 1].item_type != DND_ITEM_NULL;
				if(boxidon && prevselecton) {
					// if both of them point to the same pointer, we need to move this item instead
					if(GlobalItemStorage.PlayerInventoryList[pnum][boxid - 1].topleftboxid == GlobalItemStorage.PlayerInventoryList[pnum][CheckInventory("DnD_SelectedInventoryBox") - 1].topleftboxid) {
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
		else {
			boxid = CheckInventory("DnD_SelectedInventoryBox");
			if(boxid && (temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)) != DND_ITEM_NULL) {
				// drop selected item
				HandleMenuItemDrop(pnum, boxid, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				SetInventory("DnD_SelectedInventoryBox", 0);
			}
		}
	}
	else if(choice == DND_MENUINPUT_RCLICK)
		HandleM2Inputs(pnum, boxid, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
}

void PlayItemDropSound(int type, int sub_type, bool use_activator_sound) {
	type &= 0xFFFF;

	str snd = "Items/Drop";
	if(type == DND_ITEM_BODYARMOR || type == DND_ITEM_BOOT || type == DND_ITEM_HELM)
		snd = "Items/ArmorEquip";
	else if(type == DND_ITEM_SPECIALTY_CYBORG)
		snd = "Items/PowercoreDrop";
	else if(type == DND_ITEM_FLASK)
		snd = "Items/FlaskDrop";
	else if(type == DND_ITEM_DUNGEONKEY)
		snd = "Items/DungeonKeyDrop";
	else if(type == DND_ITEM_GEM)
		snd = "Items/GemDrop";

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
				GiveInventory("DnD_InventoryView", 1);
				LocalAmbientSound("RPG/MenuChoose", 127);
				if(boxid - 1 != INV_ICON_INDEX)
					SetInventory("DnD_SelectedCharmBox", boxid);
			}
			else {
				// now track our choices made in the inventory view
				item_sel = CheckInventory("DnD_SelectedCharmBox");
				if(item_sel) {
					item_type = DND_ITEM_NULL;
					topboxid = GlobalItemStorage.PlayerInventoryList[pnum][boxid - 1].topleftboxid - 1;

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
							item_type = DND_ITEM_SPECIALTY_DOOMGUY + GetPlayerClass();
						break;
						case BOOT_INDEX:
							item_type = DND_ITEM_BOOT;
						break;
						case FLASK1_INDEX:
						case FLASK2_INDEX:
						case FLASK3_INDEX:
						case FLASK4_INDEX:
							item_type = DND_ITEM_FLASK;
						break;
					}
					
					if(topboxid != -1 && (GlobalItemStorage.PlayerInventoryList[pnum][topboxid].item_type & 0xFFFF) == item_type) {
						// this returns popupid to show, and -1 if it was ok
						temp = MakeItemUsed(pnum, item_sel, topboxid, item_type, item_subt);
						if(temp == -1) {
							if(item_type == DND_ITEM_CHARM)
								LocalAmbientSound("Items/CharmDrop", 127);
							else if(item_type == DND_ITEM_FLASK) {
								//printbold(s:"slot ", d:item_sel - FLASK1_INDEX);
								UpdatePlayerFlaskData(pnum, item_sel - FLASK1_INDEX, false);
								LocalAmbientSound("Items/FlaskDrop", 127);
							}
							else if(item_type != DND_ITEM_SPECIALTY_CYBORG)
								LocalAmbientSound("Items/ArmorEquip", 127);
							else
								LocalAmbientSound("Items/PowercoreEquip", 127);
							TakeInventory("DnD_InventoryView", 1);
							SetInventory("DnD_SelectedCharmBox", 0);
							GiveInventory("DnD_CleanInventoryRequest", 1);
						}
						else if((GlobalItemStorage.PlayerInventoryList[pnum][topboxid].item_type & 0xFFFF) != item_type && item_type >= FIRST_SPECIALTY_ITEM_TYPE && item_type <= LAST_SPECIALTY_ITEM_TYPE)
							ShowPopup(POPUP_CANTUSEPOWERCORE, false, 0); 
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
				// A selection only earns the click if it is actually holding something to throw.
				// One left over on an empty box used to swallow it silently, which is why closing
				// with the mouse needed a second click while the left key always worked.
				if(item_sel && (temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, item_sel - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)) != DND_ITEM_NULL) {
					// drop selected item
					HandleMenuItemDrop(pnum, item_sel, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
				else {
					// nothing held, so exit
					SetInventory("DnD_SelectedInventoryBox", 0);
					GiveInventory("DnD_CleanInventoryRequest", 1);
					TakeInventory("DnD_InventoryView", 1);
					LocalAmbientSound("RPG/MenuClose", 127);
				}
			}
		}
		ClearPlayerInput(pnum, true);
	}
	else if(HasRightClicked(pnum) && boxid - 1 != INV_ICON_INDEX) {
		// With the inventory grid up, boxid is a grid slot and m2 splits a stack -- the same route
		// the standalone inventory page takes. Without this the grid had no m2 at all, and the
		// GetUsedItem below was reading a grid id into the equipped list, which only holds
		// MAX_ITEMS_EQUIPPABLE entries.
		if(CheckInventory("DnD_InventoryView")) {
			if(boxid != MAINBOX_NONE)
				HandleInventoryViewClicks(pnum, boxid, DND_MENUINPUT_RCLICK);

			ClearPlayerInput(pnum, true);
			return;
		}

		// mbox 8 is the view inventory button
		auto item = GetUsedItem(pnum, boxid - 1);
		if(boxid != MAINBOX_NONE && item.item_type != DND_ITEM_NULL) {
			// try to drop item
			temp = GetFreeSpotForItem(boxid - 1, pnum, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
			if(temp != -1) {
				// if flask, update the data for the player
				item_subt = item.item_type == DND_ITEM_FLASK;

				PlayItemDropSound(item.item_type, item.item_subtype, false);
				ApplyItemFeatures(pnum, boxid - 1, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_ITEMMOD_REMOVE);
				MoveItemTrade(pnum, boxid - 1, temp, DND_SYNC_ITEMSOURCE_ITEMSUSED, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				// force a damage cache recalc
				ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, pnum);

				if(item_subt)
					UpdatePlayerFlaskData(pnum, boxid - 1 - FLASK1_INDEX, false);
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
	UpdateDraggedItemLitBoxes(boxid, 0, 3 * MAX_INVENTORY_BOXES, pnum, DND_LITVIEW_TRADE);
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, TRADEVIEW_THEIRS_BASEX, TRADEVIEW_BASEY, 32.0, RPGMENUINVENTORYID - 4, 0, DND_SYNC_ITEMSOURCE_TRADEVIEW, GetTradee(), dimx, dimy);
		}
	}
	
	// draw our offering below
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, TRADEVIEW_OURS_BASEX, TRADEVIEW_BASEY, 32.0, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 6, MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_TRADEVIEW, pnum, dimx, dimy);
		}
	}
	// draw our inventory to the side
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, TRADEVIEW_INV_BASEX, TRADEVIEW_BASEY, 32.0, RPGMENUINVENTORYID - 6 * MAX_INVENTORY_BOXES - 12, 2 * MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, pnum, dimx, dimy);
		}
	}
	MchTradeControls(pnum, boxid - 1);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

void HandleInventoryViewTrade(int boxid) {
	// on the redesigned frame the grids sit on the page itself; the headings and buttons come with the grids
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
	HandleTradeBoxDraw(boxid, HUDMAX_X_STASH, HUDMAX_Y_STASH);
	PlayerCursorData.itemDraggedStashSize = true;
	SetFont("NMENUFNT");
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// Just takes care of backend trade details
void CancelTrade(int pnum) {
	int tid = pnum + P_TIDSTART;
	SendOwnerScript("DnD Refresh Request", pnum, pnum, 2);
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
	// from trade view of this player, move items back to the boxes they were staked from
	int bid;
	for(int i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(int j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			bid = j + i * MAXINVENTORYBLOCKS_VERT;
			if(GlobalItemStorage.TradeViewList[pnum][bid].item_type != DND_ITEM_NULL) {
				if(!UnstakeTradeItem(pnum, bid)) {
					// drop this item, just in case
					// for now, since this isn't very likely to happen, just log an error message
					Log(s:"Attempted to return item from trade view but no fitting slot found! Drop item.");
				}
			}
		}
	}
	ClearTradeItemOrigins(pnum);
}

// Hand a staked item back to the box it was staked from. The trade rules keep that box free for as
// long as the item is offered -- see TradeItemOrigin -- so this normally cannot fail. The free spot
// search is only a backstop for an item that reached the grid without a note.
bool UnstakeTradeItem(int pnum, int itempos) {
	int tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, DND_SYNC_ITEMSOURCE_TRADEVIEW) - 1;
	if(tb < 0)
		return false;

	int pos = GetTradeItemOrigin(pnum, tb);
	if(pos == -1 || !IsFreeSpot(pnum, tb, pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY))
		pos = GetFreeSpotForItem(tb, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);

	if(pos == -1)
		return false;

	MoveItemTrade(pnum, tb, pos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
	return true;
}

// Offer an item from this player's inventory. Prefers the box they aimed at so dragging still lands
// where they expect, and falls back to the first free spot rather than swapping with whatever is
// already there -- a swap would send the displaced offer to a box that isn't its own.
bool StakeTradeItem(int pnum, int itempos, int destpos) {
	int tb = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, itempos, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) - 1;
	if(tb < 0)
		return false;

	if(destpos != -1 && IsFreeSpot(pnum, itempos, destpos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_TRADEVIEW)) {
		MoveItemTrade(pnum, itempos, destpos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_TRADEVIEW);
		return true;
	}

	int pos = GetFreeSpotForItem(tb, pnum, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_TRADEVIEW);
	if(pos == -1)
		return false;

	MoveItemTrade(pnum, tb, pos, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, DND_SYNC_ITEMSOURCE_TRADEVIEW);
	return true;
}

void HandleTradeCountdown(int p1, int p2) {
	SetActivator(p1 + P_TIDSTART);
	SendOwnerScript("DnD Trade Countdown", p1, p1, p2);
	SetActivator(p2 + P_TIDSTART);
	SendOwnerScript("DnD Trade Countdown", p2, p2, p1);
	ACS_NamedExecuteAlways("DnD Trade Counter", 0, p1, p2);
}

// serverside
void HandleTradeViewButtonClicks(int pnum, int boxid) {
	int bid = GetTradee();
	int ioffset = 0, isource, soffset = 0, ssource;
	// clicking an empty box does nothing, so it should not ring the choose sound either
	bool acted = true;

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
					SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
					SendOwnerScript("DnD Refresh Request", bid, bid, 1);
						
					// if other side's confirmation is set, check for item space
					if(CheckActorInventory(bid + P_TIDSTART, "DnD_Trade_Confirmed")) {
						// check if we have space for all the offerings in our inventory
						bool ok_from_us = true, ok_from_tradee = true;
						// only a passed check is cached; the countdown rechecks both before anything moves
						if(!CheckInventory("DnD_TradeSpaceFit")) {
							ok_from_us = ConfirmSpaceForOfferings(bid, PlayerNumber());
							if(ok_from_us)
								GiveInventory("DnD_TradeSpaceFit", 1);
						}

						if(!CheckActorInventory(bid + P_TIDSTART, "DnD_TradeSpaceFit")) {
							ok_from_tradee = ConfirmSpaceForOfferings(PlayerNumber(), bid);
							if(ok_from_tradee)
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
								SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
								SendOwnerScript("DnD Refresh Request", bid, bid, 1);
							}
							else {
								SetInventory("DnD_Trade_Confirmed", 0);
								// show popup for not enough space
								ShowPopup(POPUP_NOSPACEFORTRADE, false, 0);
								SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
								SendOwnerScript("DnD Refresh Request", bid, bid, 1);
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

					SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
					SendOwnerScript("DnD Refresh Request", bid, bid, 1);
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
						// offset deciphering
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

						// jump click is to auto-dump hovered item to stash or vice versa
						if(GetPlayerInput(-1, INPUT_BUTTONS) & BT_CROUCH) {
							if(isource == DND_SYNC_ITEMSOURCE_TRADEVIEW)
								SetTradeItemOrigin(pnum, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - ioffset, -1, isource) - 1, -1);
							HandleMenuItemDrop(pnum, boxid - ioffset, isource);
						}
						else if((GetPlayerInput(-1, INPUT_BUTTONS) & BT_JUMP) && !CheckInventory("DnD_AutoDumpCooldown")) {
							// auto move code -- returns success if it could move
							// deliberately not AutoMoveItem: that one merges and splits stacks, and a
							// split leaves half the stack sitting in the box the other half has to
							// come home to. stakes stay whole for as long as a trade is open.
							if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL) {
								bool moved;
								if(isource == DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)
									moved = StakeTradeItem(pnum, boxid - ioffset - 1, -1);
								else
									moved = UnstakeTradeItem(pnum, boxid - ioffset - 1);

								if(moved)
									GiveInventory("DnD_AutoDumpCooldown", 1);
							}
						}
						// an empty box has nothing to pick up -- same rule as HandleInventoryViewClicks
						else if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL)
							SetInventory("DnD_SelectedInventoryBox", boxid);
						else
							acted = false;
					}
					else if(boxid != CheckInventory("DnD_SelectedInventoryBox")) {
						int sel = CheckInventory("DnD_SelectedInventoryBox");

						// past 2 * MAX is the player's own inventory grid, below it is their half of
						// the offer. i is for the current click, s for the previous selection.
						bool box_inv = boxid > 2 * MAX_INVENTORY_BOXES;
						bool sel_inv = sel > 2 * MAX_INVENTORY_BOXES;
						isource = box_inv ? DND_SYNC_ITEMSOURCE_PLAYERINVENTORY : DND_SYNC_ITEMSOURCE_TRADEVIEW;
						ssource = sel_inv ? DND_SYNC_ITEMSOURCE_PLAYERINVENTORY : DND_SYNC_ITEMSOURCE_TRADEVIEW;
						ioffset = box_inv ? 2 * MAX_INVENTORY_BOXES : MAX_INVENTORY_BOXES;
						soffset = sel_inv ? 2 * MAX_INVENTORY_BOXES : MAX_INVENTORY_BOXES;

						int ipos = boxid - 1 - ioffset, spos = sel - 1 - soffset;
						bool boxidon = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, ipos, -1, isource) != DND_ITEM_NULL;
						bool prevselecton = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, spos, -1, ssource) != DND_ITEM_NULL;

						// "carry the item at frompos onto topos". A selection is always holding
						// something now, so the carried item is always the selected one -- the swap
						// below only survives as a guard in case that ever stops being true.
						int frompos = spos, topos = ipos;
						bool from_inv = sel_inv, to_inv = box_inv;
						if(!prevselecton && boxidon) {
							frompos = ipos;
							topos = spos;
							from_inv = box_inv;
							to_inv = sel_inv;
						}

						if(boxidon || prevselecton) {
							if(from_inv && to_inv) {
								// No shuffling your own grid while a trade is open. Every staked item
								// has to find its own box still free and still the right shape if the
								// trade falls through -- see TradeItemOrigin. Letting the player fill
								// the hole an offered item left is exactly what breaks that.
							}
							else if(from_inv)
								StakeTradeItem(pnum, frompos, topos);
							else if(to_inv) {
								// the aimed at box is ignored: an offer only ever comes back home
								UnstakeTradeItem(pnum, frompos);
							}
							else if(boxidon && prevselecton) {
								// both inside the offer, so this is just rearranging what's on show
								if(GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, ipos, -1, isource) == GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, spos, -1, ssource)) {
									if(IsFreeSpot(pnum, spos, ipos, ssource, isource))
										MoveItemTrade(pnum, spos, ipos, ssource, isource);
								}
								else
									SwapItems(pnum, ipos, spos, isource, ssource, false);
							}
							else if(IsFreeSpot(pnum, frompos, topos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_TRADEVIEW))
								MoveItemTrade(pnum, frompos, topos, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_TRADEVIEW);
						}
						SetInventory("DnD_SelectedInventoryBox", 0);
					}
					else
						SetInventory("DnD_SelectedInventoryBox", 0);

					if(acted)
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
						// drop selected item -- an offer thrown on the floor has no home to keep
						if(ssource == DND_SYNC_ITEMSOURCE_TRADEVIEW)
							SetTradeItemOrigin(pnum, GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, -1, ssource) - 1, -1);
						HandleMenuItemDrop(pnum, CheckInventory("DnD_SelectedInventoryBox") - soffset, ssource);
						//DropItemToField(pnum, CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, true, ssource);
						
						// check if commenting this causes any issues, we already save player inventories when they spectate
						// ACS_NamedExecuteAlways("DnD Save Player Item Data", 0, pnum | (CheckInventory("DnD_CharacterID") << 16), CheckInventory("DnD_SelectedInventoryBox") - 1 - soffset, ssource);
						
						SetInventory("DnD_SelectedInventoryBox", 0);
						ActivatorSound("Items/Drop", 127);
					}
				}

				// make sure changes are reflected dynamically
				SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
				SendOwnerScript("DnD Refresh Request", bid, bid, 1);
			}
		}
	}
	// m2 is deliberately dead while a trade is open. HandleM2Inputs is nothing but stack splitting,
	// and it CloneItems into the same grid it split from -- in the inventory that eats a box a
	// staked item has to come home to, and in the offer it leaves a piece with no note attached.
	// See TradeItemOrigin.
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
		MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCURSORID + 1, CR_WHITE, MenuX(HUDMAX_XF_STASH - ((PlayerCursorData.posx * 3 / 2) & MMASK) - 16.0 * scale * (sx - 1) + 0.4), MenuY(HUDMAX_YF_STASH - ((PlayerCursorData.posy * 3 / 2) & MMASK)), 0.0, 0.0);
	}
	else
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUCURSORID + 1, CR_WHITE, MenuX(HUDMAX_XF - (PlayerCursorData.posx & MMASK) - 16.0 * scale * (sx - 1) + 0.4), MenuY(HUDMAX_YF - (PlayerCursorData.posy & MMASK)), 0.0, 0.0);
}

void HandleStashView(int boxid) {
	int prevclick = CheckInventory("DnD_SelectedInventoryBox") - 1;
	int pnum = PlayerNumber(), i, j;
	int curpages = CheckInventory("DnD_PlayerCurrentPage") - 1;
	int color;
	MchStashPanels(RPGMENUINVENTORYID - 1);
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
	
	PlayerCursorData.itemDraggedStashSize = true;
	
	ResetInventoryLitState(0, 2 * MAX_INVENTORY_BOXES);
	UpdateDraggedItemLitBoxes(boxid, 0, 2 * MAX_INVENTORY_BOXES, pnum, DND_LITVIEW_STASH);
	// draw stash at top
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, STASHVIEW_STASH_BASEX, STASHVIEW_BASEY, 32.0, RPGMENUINVENTORYID - 4, 0, DND_SYNC_ITEMSOURCE_STASH | (curpages << 16), pnum, TRADE_RES_X, TRADE_RES_Y);
		}
	}
	
	// draw our inventory below
	for(i = 0; i < MAXINVENTORYBLOCKS_HORIZ; ++i) {
		for(j = 0; j < MAXINVENTORYBLOCKS_VERT; ++j) {
			DoInventoryBoxDraw(boxid, prevclick, i, j, STASHVIEW_INV_BASEX, STASHVIEW_BASEY, 32.0, RPGMENUINVENTORYID - 3 * MAX_INVENTORY_BOXES - 6, MAX_INVENTORY_BOXES, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, pnum, TRADE_RES_X, TRADE_RES_Y);
		}
	}
	
	MchStashControls(pnum, boxid, curpages);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
	SetFont("NMENUFNT");
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
				HandleMenuItemDrop(pnum, sel_box - soffset, ssource);
			}
		}
		// sort buttons sit past every grid and tab id, so they get looked at before anything tries
		// to read boxid as an item slot
		else if(boxid == STASHSORT_BOXID_STASH || boxid == STASHSORT_BOXID_INV) {
			if(!CheckInventory("DnD_SortCooldown")) {
				// a held box id means nothing once the grid is repacked under it
				SetInventory("DnD_SelectedInventoryBox", 0);
				LocalAmbientSound("RPG/MenuChoose", 127);
				if(boxid == STASHSORT_BOXID_STASH)
					ACS_NamedExecuteAlways("DnD Sort Inventory", 0, DND_SYNC_ITEMSOURCE_STASH, CheckInventory("DnD_PlayerCurrentPage") - 1);
				else
					ACS_NamedExecuteAlways("DnD Sort Inventory", 0, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0);
			}
		}
		// MAINBOX_NONE is 0, which passes the button range check on its own -- without the guard a
		// click outside the grid took the pick up path with boxid - 1 as an index and rang the
		// choose sound for nothing
		else if(!sel_box && boxid != MAINBOX_NONE && boxid < STASHBUTTON_BOXID_START) {
			//printbold(s:"set selected box to ", d:boxid);
			cpage = CheckInventory("DnD_PlayerCurrentPage");
			SetInventory("DnD_PlayerPreviousPage", cpage);

			if(boxid > MAX_INVENTORY_BOXES) {
				isource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
				ioffset = MAX_INVENTORY_BOXES;

				ssource = DND_SYNC_ITEMSOURCE_STASH | ((cpage - 1) << 16);
			}
			else {
				isource = DND_SYNC_ITEMSOURCE_STASH | ((cpage - 1) << 16);

				ssource = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
				soffset = MAX_INVENTORY_BOXES;
			}

			// jump click is to auto-dump hovered item to stash or vice versa
			if(GetPlayerInput(-1, INPUT_BUTTONS) & BT_JUMP) {
				// auto move code -- returns success if it could move
				ppage = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource);
				if(ppage != DND_ITEM_NULL) {
					if(cpage - 1 != PAGEID_STASHTAB_ORBS || ppage == DND_ITEM_ORB) {
						LocalAmbientSound("RPG/MenuChoose", 127);
						AutoMoveItem(pnum, boxid - ioffset - 1, isource, ssource);
					}
					else
						ShowPopup(POPUP_ORBSONLY, false, 0);
				}
			}
			else if(GetPlayerInput(-1, INPUT_BUTTONS) & BT_CROUCH) {
				LocalAmbientSound("RPG/MenuChoose", 127);
				HandleMenuItemDrop(pnum, boxid - ioffset, isource);
			}
			// an empty box has nothing to pick up -- same rule as HandleInventoryViewClicks
			else if(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_NULL) {
				LocalAmbientSound("RPG/MenuChoose", 127);
				SetInventory("DnD_SelectedInventoryBox", boxid);
			}
		}
		// A stash box id repeats on every page, so "the box I am already holding" has to mean same
		// box AND same page. Without the page half, picking up page 1 box N and then clicking box N
		// on page 2 looked like clicking the held box itself and just dropped the selection instead
		// of moving. Only the stash grid is paged -- the player inventory grid stacked below it
		// (boxid > MAX_INVENTORY_BOXES) has no pages, and the page buttons sit past it too, so both
		// are left on the plain box comparison. The move code below already handles the cross page
		// case correctly: isource takes the current page, ssource the page the item was picked up on.
		else if(boxid != sel_box || (boxid <= MAX_INVENTORY_BOXES && CheckInventory("DnD_PlayerCurrentPage") != CheckInventory("DnD_PlayerPreviousPage"))) {
			if(boxid >= STASHBUTTON_BOXID_START) {
				// because fucking ACS is stupid somehow this is 2 when evaluated normally...
				temp = -(STASHBUTTON_BOXID_START - boxid);
				if(temp < CheckInventory("DnD_PlayerInventoryPages")) {
					GiveInventory(StrParam(s:"DnD_ButtonPress_", d:temp + 1), 1);
					//printbold(s:"set current page to ", d:temp + 1);
					SetInventory("DnD_PlayerCurrentPage", temp + 1);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(temp == PAGEID_STASHTAB_ORBS) {
					GiveInventory("DnD_ButtonPress_OrbPage", 1);
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
						// if we want to move something from inv to stash check if its orbs only page
						if
						(
							((ssource >> 16) == PAGEID_STASHTAB_ORBS && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1 - ioffset, -1, isource) != DND_ITEM_ORB) ||
							((isource >> 16) == PAGEID_STASHTAB_ORBS && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, sel_box - 1 - soffset, -1, ssource) != DND_ITEM_ORB)
						)
						{
							ShowPopup(POPUP_ORBSONLY, false, 0);
						}
						else {
							//printbold(s:"swapping ", d:boxid - 1 - ioffset, s: " and ", d:sel_box - 1 - soffset);3
							SwapItems(pnum, boxid - 1 - ioffset, sel_box - 1 - soffset, isource, ssource, false);
						}
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
					if((boxidon || prevselecton) && IsFreeSpot(pnum, ipos - ioffset, epos - soffset, isource, ssource)) {
						if((ssource >> 16) == PAGEID_STASHTAB_ORBS && GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, ipos - ioffset, -1, isource) != DND_ITEM_ORB)
							ShowPopup(POPUP_ORBSONLY, false, 0);
						else if
						(
							(isource >> 16) == PAGEID_STASHTAB_ORBS && 
							(ppage = GetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos - ioffset, -1, isource)) > (cpage = GetStackValue(DND_ITEM_ORB, ssource))
						)
						{
							// if we are not allowed to pull more here, adjust stacks
							SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, ipos - ioffset, -1, ppage - cpage, isource);
							SyncItemStack(pnum, ipos - ioffset, isource);

							CloneItemToSpot(pnum, ipos - ioffset, isource, epos - soffset, true, ssource);
							SetItemSyncValue(pnum, DND_SYNC_ITEMSTACK, epos - soffset, -1, cpage, ssource);
							SyncItemData(pnum, epos - soffset, ssource, -1, -1);
						}
						else
							MoveItemTrade(pnum, ipos - ioffset, epos - soffset, isource, ssource);
					}
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

void ResetDungeonSelectData(int pnum) {
	auto d = GetSelectedDungeonData(pnum);
	d.id = -1;
	SendOwnerSync("DnD Selected Dungeon Sync", pnum, pnum, -1, 0, 0);
}

void ResetTransmutingData(int pnum) {
	auto d = GetTransmuteOrbData(pnum);
	for(int i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		d.val[i] = -1;
		SendOwnerSync("DnD Transmute Orb Sync", pnum, pnum, i, -1, 0);
	}
}

bool IsTransmuteGemSlot(int v) {
	return v != -1 && (v & DND_TRANSMUTE_GEMSLOT);
}

// The box a gem slot points at, or -1 once that box no longer holds a gem (socketed, sold, sorted away).
int GetTransmuteSlotGem(int pnum, int v) {
	int box = v & DND_TRANSMUTE_GEMBOXMASK;
	if(GetPlayerInventoryItem(pnum, box).item_type != DND_ITEM_GEM || GetPlayerInventoryItem(pnum, box).topleftboxid != box + 1)
		return -1;
	return box;
}

// -1 with no gem in the chamber, DND_GEM_TRANSMUTE for three distinct gems of one tier (any colours), -2 for anything else,
// gems without the Gemcrafting research included.
int GetTransmuteGemRecipe(int pnum) {
	auto d = GetTransmuteOrbData(pnum);
	int i, k, box, tier = -1, gems = 0;
	for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i)
		gems += IsTransmuteGemSlot(d.val[i]);
	if(!gems)
		return -1;
	if(gems < MAX_TRANSMUTE_BOXES || CheckResearchStatus(RES_GEMCRAFTING) != RES_DONE)
		return -2;

	for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		box = GetTransmuteSlotGem(pnum, d.val[i]);
		if(box == -1)
			return -2;
		for(k = 0; k < i; ++k)
			if((d.val[k] & DND_TRANSMUTE_GEMBOXMASK) == box)
				return -2;

		k = IsGreaterGem(GetPlayerInventoryItem(pnum, box).item_subtype);
		if(tier != -1 && k != tier)
			return -2;
		tier = k;
	}
	return DND_GEM_TRANSMUTE;
}

// Spends the chamber's three gems for one of their tier, colour and role random. GetTransmuteGemRecipe vetted them.
void TransmuteChamberGems(int pnum) {
	auto d = GetTransmuteOrbData(pnum);
	bool greater = IsGreaterGem(GetPlayerInventoryItem(pnum, d.val[0] & DND_TRANSMUTE_GEMBOXMASK).item_subtype);
	for(int i = 0; i < MAX_TRANSMUTE_BOXES; ++i)
		FreeItem(pnum, d.val[i] & DND_TRANSMUTE_GEMBOXMASK, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, false);
	RollGemIntoInventory(pnum, greater);
}

int IsValidTransmuteRecipe(int pnum) {
	// check the orbs now for valid recipes
	static int orb_counts[MAX_ORBS];
	int res = -1, i, j;
	int overall_count = 0;

	// Gems first: their slots hold boxes, which the orb counts below would read as orb types.
	j = GetTransmuteGemRecipe(pnum);
	if(j != -1)
		return j == DND_GEM_TRANSMUTE ? j : -1;

	for(i = 0; i < MAX_ORBS; ++i)
		orb_counts[i] = 0;

	// get count of all orbs in the transmute chamber
	auto transmute_data = GetTransmuteOrbData(pnum);
	for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		j = transmute_data.val[i];
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
				orb_counts[DND_ORB_BRUTE] || orb_counts[DND_ORB_JAGGED] || orb_counts[DND_ORB_SAVAGERY] || orb_counts[DND_ORB_PROSPERITY] ||
				orb_counts[DND_ORB_ARCANA]
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

void DrawCraftingInventoryInfo(int pn, int id_begin, int x, int y, int item_id, int item_type, int item_source) {
	int mx, my;
	
	mx = HUDMAX_XF - (x & MMASK) + 16.1;
	my = HUDMAX_YF - (y & MMASK) + 16.1;

	// to force them to appear in window
	if(mx > INVENTORYINFO_NORMALVIEW_WRAPX)
		mx = INVENTORYINFO_NORMALVIEW_WRAPX + 0.1;
	if(my > INVENTORYINFO_NORMALVIEW_WRAPY)
		my = INVENTORYINFO_NORMALVIEW_WRAPY + 0.1;

	// the panel, the weapon lines and the stack count: everything this view draws above the text ids
	DeleteTextRange(id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 5 - ITEMINFOBG_MAXMIDS, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES);

	int bg_x = mx;
	int bg_y = my;

	MenuHudSize(HUDMAX_X, HUDMAX_Y);
	int attr_count = GetItemSyncValue(pn, DND_SYNC_ITEMSATTRIBCOUNT, item_id, -1, item_source);

	// measured, then lifted by any overflow past the screen bottom and drawn again, as DrawInventoryInfo does
	int prev_x, prev_y, over;
	for(int pass = 0; pass < 2; ++pass) {
		MenuHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2);

		prev_x = 3 * bg_x / 2;
		prev_y = 3 * bg_y / 2;

		mx = GetIntegerBits(3 * (bg_x + HUD_ITEMBAK_XF / 2) / 2) + 0.4;
		my = GetIntegerBits(3 * bg_y / 2 + ITEMINFO_TEXTTOP) + 0.1;

		MenuClipRect(ITEMINFO_CLIPX + (prev_x >> 16), 15 + (prev_y >> 16), HUD_ITEMBAK_WIDTH, ITEMINFO_CLIPHEIGHT, HUD_ITEMBAK_WIDTH);
		over = DrawCraftingInventoryText(item_type, item_id, item_source, pn, id_begin, mx, my, HUDMAX_X, HUDMAX_Y, bg_x, bg_y, attr_count);
		MenuClipRect(0, 0, 0, 0, 0);

		over = (bg_y >> 16) + ItemInfoPanelHeight(over) - HUDMAX_Y;
		if(over <= 0)
			break;
		bg_y -= over << 16;
	}
	MenuHudSize(HUDMAX_X, HUDMAX_Y);

	if(GetStackValue(item_type) && item_source) {
		SetFont("NMENUFNT");
		HudMessage(
			d:GetTotalStackOfMaterial(item_id); HUDMSG_PLAIN | HUDMSG_FADEOUT,
			id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 5 - ITEMINFOBG_MAXMIDS, CR_GREEN, MenuX(bg_x + 200.1), MenuY(bg_y + 16.0),
			INVENTORY_HOLDTIME_CRAFT,
			INVENTORY_FADETIME,
			INVENTORY_INFO_ALPHA
		);
	}
}

// extra1 is topboxid for items, extra2 works as source for inventory related things (used charms vs player inventory etc.)
// Returns the panel's mid count, as DrawInventoryText does.
int DrawCraftingInventoryText(int itype, int extra1, int extra2, int pnum, int id_begin, int mx, int my, int hx, int hy, int bg_x, int bg_y, int attr_count) {
	if(itype == DND_ITEM_WEAPON) {
		int j, temp;
		j = PlayerNumber();
		my += 24.0;
		
		SetFont("NMENUFNT");
		HudMessage(s:"\c[R5]", l:GetWeaponTag(extra1), s:":\c- \c[Y5]", l:"DND_MENU_SLOT", s:" - ", d:GetGameSlotOfWeapon(extra1); HUDMSG_PLAIN, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 3 - ITEMINFOBG_MAXMIDS, CR_WHITE, MenuX(mx), MenuY(my - 24.0), 0.0, 0.0);
		
		str modText = "";
		wep_dps_view_T module& dps = GetWeaponDPSView(j, extra1);
		if(dps.dps[0])
			modText = StrParam(s:"\c[Y5]* \c[Q9]", d:dps.hit[0], s:"\c- ", l:"DND_MENU_WEPHIT", s:", \c[Q9]", d:dps.dps[0], s:"\c- ", l:"DND_MENU_WEPDPS", s:"\n");
		if(dps.dps[1])
			modText = StrParam(s:modText, s:"\c[Y5]* ", l:dps.alt_mode ? "DND_MENU_WEPALTMODE" : "DND_MENU_WEPALT", s:": \c[Q9]", d:dps.hit[1], s:"\c- ", l:"DND_MENU_WEPHIT", s:", \c[Q9]", d:dps.dps[1], s:"\c- ", l:"DND_MENU_WEPDPS", s:"\n");
		temp = GetPlayerWeaponQuality(j, extra1);
		// make sure quality text isn't on the weapon name
		if(temp) {
			if (temp == MAXWEPQUALITY) // Add the "MAX" indicator.
				modText = StrParam(s:modText, s:"\c[Y5]* ", l:"WEPMOD_TEXT1", s:": \c[Q9]+", d:temp, s:"% ", l:"DND_MENU_MAX", s:"\n");
			else
				modText = StrParam(s:modText, s:"\c[Y5]* ", l:"WEPMOD_TEXT1", s:": \c[Q9]+", d:temp, s:"% \n");
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

		temp = GetWeaponModValue(j, extra1, WEP_MOD_EXTRAPROJ);
		if(temp)
			modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_EXTRAPROJ), s:"\n");
		
		for(temp = 0; temp < MAX_WEP_MOD_POWERSET1; ++temp) {
			if(HasWeaponPower(j, extra1, temp))
				modText = StrParam(s:modText, s:GetWeaponModText(temp, WEP_MOD_POWERSET1, temp), s:"\n");
		}
		
		HudMessage(s:modText; HUDMSG_PLAIN, id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES - 4 - ITEMINFOBG_MAXMIDS, CR_WHITE, MenuX(mx), MenuY(my - 8.0), 0.0, 0.0);

		temp = CountNewLinesInText(modText, HUD_ITEMBAK_WIDTH);
		DrawItemInfoBackground(id_begin - HUD_DII_MULT * MAX_INVENTORY_BOXES, hx, hy, bg_x, bg_y, temp);
		return Min(temp, ITEMINFOBG_MAXMIDS);
	}

	// draw using our established drawing routine
	return DrawInventoryText(
		extra1, extra2, pnum, mx, my, itype,
		GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, extra1, -1, extra2),
		id_begin, HUD_DII_MULT, hx, hy, bg_x, bg_y, attr_count, false,
		MchCraftPreview(PlayerCursorData.itemHovered), INVENTORY_HOLDTIME_CRAFT
	);
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
					if(IsSelfUsableItem(GlobalItemStorage.PlayerInventoryList[pnum][curitemeindex].item_type, GlobalItemStorage.PlayerInventoryList[pnum][curitemeindex].item_subtype)) {
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
								SendOwnerScript("DnD Menu Sell Popup Clear", PlayerNumber());
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
						SendOwnerScript("DnD Menu Sell Popup Clear", PlayerNumber());
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
		UpdateMenuPosition(MENU_LOAD);
		SetInventory("DnD_SelectedInventoryBox", 0);
		SetInventory("DnD_UsedTwoItemRequirementMaterial", 0);
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
		auto transmute_data = GetTransmuteOrbData(pnum);
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
				else if(boxid == MBOX_4 && (recipe = IsValidTransmuteRecipe(pnum)) == DND_GEM_TRANSMUTE) {
					if(GetPlayerCredit(pnum) < DND_TRANSMUTE_COST) {
						ShowPopup(POPUP_NOFUNDS, false, 0);
						return;
					}

					TransmuteChamberGems(pnum);
					TakeCredit(DND_TRANSMUTE_COST);
					GiveInventory("DnD_CleanCraftingRequest", 1);
					LocalAmbientSound("Items/SuccessDisassemble", 108);
					ResetTransmutingData(pnum);
				}
				else if(boxid == MBOX_4 && recipe != -1) {
					// check credit
					if(GetPlayerCredit(pnum) < DND_TRANSMUTE_COST) {
						ShowPopup(POPUP_NOFUNDS, false, 0);
						return;
					}

					// add them only if they aren't the same -- really shitty code but I really didn't wanna use an array to group them together
					auto data = GetTransmuteOrbData(pnum);
					int oid_1 = data.val[0];
					int oid_2 = data.val[1];
					int oid_3 = data.val[2];
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
					// previtemindex holds (topboxid of item) - 1 in inventory; a gem goes in by its box, an orb by its type
					temp = GlobalItemStorage.PlayerInventoryList[pnum][previtemindex].item_subtype;
					if(GlobalItemStorage.PlayerInventoryList[pnum][previtemindex].item_type == DND_ITEM_GEM) {
						// gems take the Gemcrafting research; the material pane does not list them without it either
						if(CheckResearchStatus(RES_GEMCRAFTING) != RES_DONE)
							return;
						temp = DND_TRANSMUTE_GEMSLOT | previtemindex;
					}
					transmute_data.val[boxid - 1] = temp;
					SendOwnerScript("DnD Transmute Orb Sync", pnum, pnum, boxid - 1, temp);
					LocalAmbientSound("RPG/MenuChoose", 127);
					SetInventory("DnD_SelectedInventoryBox", 0);
				}
			}
			else if(HasRightClicked(pnum) && boxid >= MBOX_1 && boxid <= MBOX_3) {
				// take orb off transmute list
				transmute_data.val[boxid - 1] = -1;
				SendOwnerScript("DnD Transmute Orb Sync", pnum, pnum, boxid - 1, -1);
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
		UpdateMenuPosition(MENU_LOAD);
		GiveInventory("DnD_CleanCraftingRequest", 1);
		SetInventory("DnD_SelectedInventoryBox", 0);
	}
}

// returns success of use of item
// item is craft material, target is the item we intend to use it on
bool HandleMaterialUse(int pnum, int itemindex, int targetindex, bool isWeapon = false) {
	bool res = false;
	int itype = GlobalItemStorage.PlayerInventoryList[pnum][itemindex].item_type;
	int isubtype = GlobalItemStorage.PlayerInventoryList[pnum][itemindex].item_subtype;
	
	int targettype = DND_ITEM_NULL;
	if(targetindex != -1) {
		if(!isWeapon)
			targettype = GlobalItemStorage.PlayerInventoryList[pnum][targetindex].item_type;
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
	if(!IsCraftableItem(GlobalItemStorage.PlayerInventoryList[pnum][item_index1].item_type) || !IsCraftableItem(GlobalItemStorage.PlayerInventoryList[pnum][item_index2].item_type))
		return false;
	int mat_index = CheckInventory("DnD_UsedTwoItemRequirementMaterial") - 1;
	int type = GlobalItemStorage.PlayerInventoryList[pnum][mat_index].item_type;
	int subtype = GlobalItemStorage.PlayerInventoryList[pnum][mat_index].item_subtype;
	
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
	else {
		// Two pages read the jump modifier. The perk trees pin a description with it: their rows ARE
		// the hover targets, so reading a long one means holding the cursor still on the very row you
		// are scrolling away from. The stat page turns a spend into a refund, both buttons being
		// spent already.
		int jumpmod = DND_JUMPMOD_NONE;
		if(IsSpellTreePage(opt))
			jumpmod = DND_JUMPMOD_PINREFUND;
		else if(IsPerkTreePage(opt) || IsSpellPage(opt))
			jumpmod = DND_JUMPMOD_PIN;
		else if(opt == MENU_STAT1)
			jumpmod = DND_JUMPMOD_REFUND;

		ListenInput(LISTEN_LEFT | LISTEN_RIGHT, 0, 0, -1, jumpmod);
	}
}

int GetAmmoSlotAndIndexFromShop(int index) {
	if(index < SHOP_FIRSTAMMOSPECIAL_INDEX) {
		for(int i = 0; i < MAX_SLOTS; ++i)
			int temp = GetAmmoSlotMaxIndex(i);
			for(int j = 0; j < temp; ++j) {
				if(GetMenuAmmoIndexMap(i, j) == index)
					return i | (j << 16);
			}
	}
	else
		return index - SHOP_FIRSTAMMOSPECIAL_INDEX;
	// keep this return structure as is, if the top loop finds nothing silently return 0
	return 0;
}

// Display mirror of ApplyPlayerDamageResist -- same terms, no cap and no damage applied. Takes a
// DND_PRESIST_* index for the same reason that one does, and renamed alongside it so that a stale
// attribute id cannot slip through on this side either.
int GetResistDisplayValue(int pnum, int res_id, int reduce) {
	int val = PlayerModData[pnum].vals[PSTAT_RESIST_BASE + res_id] + PlayerModData[pnum].vals[PSTAT_RESIST_ALL] + reduce;

	switch(res_id) {
		case DND_PRESIST_ELEM:
			val += pbuffs[pnum].buff_net_values[BUFF_ELEMENTALRESIST].additive;
		break;
		case DND_PRESIST_ENERGY:
			val += pbuffs[pnum].buff_net_values[BUFF_ENERGYRESIST].additive;
		break;
		case DND_PRESIST_PHYS:
			val += pbuffs[pnum].buff_net_values[BUFF_PHYSRESIST].additive;
		break;
		case DND_PRESIST_MAGIC:
			val += pbuffs[pnum].buff_net_values[BUFF_MAGICRESIST].additive;
		break;
	}	

	return val;
}

int GetPlayerMoreDamageDisplay(int pnum) {
	int base = PlayerModData[pnum].vals[PSTAT_DAMAGEPERCENT_MORE];
	base = FixedMul(1.0 + base, pbuffs[pnum].buff_net_values[BUFF_FRENZYCHARGE].multiplicative);
	if(base > 0)
		return base - 1.0;
	return 0;
}

int GetPlayerLessDamageDisplay(int pnum) {
	int mult = 1.0;
	mult = FixedMul(mult, pbuffs[pnum].buff_net_values[BUFF_DAMAGETAKEN].multiplicative);
	mult = FixedMul(mult, pbuffs[pnum].buff_net_values[BUFF_ENDURANCECHARGE].multiplicative);

	int temp = PlayerModData[pnum].vals[PSTAT_EX_LESSDMGTAKENMAXOVERHEAT];
	if(temp && HasRunningOverheatCooldown(pnum + P_TIDSTART))
		mult = CombineFactors(mult, -((temp << 16)) / 100);

	temp = PlayerModData[pnum].vals[PSTAT_EX_DMGINCREASE_TAKEN];
	if(temp)
		mult = CombineFactors(mult, (temp << 16) / 100);

	return mult - 1.0;
}

// not sure how to group these for other places, their calculations arent exactly done in straightforward fashion so calculating as they come makes sense
// Builds the cached stat list text of a category; returns its line count when it built, 0 when it was cached.
int BuildPlayerStatsText(int pnum, int category) {
	int val;
	int k = 0;
	int tid = pnum + P_TIDSTART;
	int i;
	int temp;
	pstat_text_T? pstat_text = GetPlayerStatText();
	
	// yea this is some terrible piece of code but idc, should be cleaned up with an array to lookup attribs and a switch-case for exceptions? idk
	if(pstat_text.text == "") {
		if(category == DRAW_STAT_OFFENSE1) {
			// melee dmg
			val = PlayerModData[pnum].vals[PSTAT_MELEEDAMAGE];
			if(val) {
				pstat_text.text = StrParam(s:GetItemAttributeText(INV_MELEEDAMAGE, 0, 0, val), s:"\n");
				++k;
			}
			
			// melee range
			val = GetPlayerMeleeRange(pnum, 100.0);
			if(val != 100.0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", s:GetFixedRepresentation(val, false), s:"%\c- ", l:"DND_MELEERANGE", s:"\n");
				++k;
			}

			// melee specific skill stuff like deep cuts etc.
			i = GetCVar("dnd_detailedmods");
			val = PlayerModData[pnum].vals[PSTAT_CRUSHINGBLOW];
			if(val) {
				// detailed mods view
				if(i) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_CRUSHINGBLOW, 0, 0, val), s:" ", s:"\cu", l:"DND_CRUSHINGBLOW_EXPL", s:"\n");
					k += 3;
				}
				else {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_CRUSHINGBLOW, 0, 0, val), s:"\n");
					++k;
				}
			}

			val = PlayerModData[pnum].vals[PSTAT_DEEPCUTS];
			if(val) {
				if(i) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DEEPCUTS, 0, 0, val), s:" ", s:"\cu", l:"DND_DEEPCUTS_EXPL", s:"\n");
					k += 3;
				}
				else {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DEEPCUTS, 0, 0, val), s:"\n");
					++k;
				}
			}

			val = PlayerModData[pnum].vals[PSTAT_OPENWOUNDS];
			if(val) {
				if(i) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_OPENWOUNDS, 0, 0, val), s:" ", s:"\cu", l:"DND_OPENWOUNDS_EXPL", s:"\n");
					k += 3;
				}
				else {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_OPENWOUNDS, 0, 0, val), s:"\n");
					++k;
				}
			}

			val = PlayerModData[pnum].vals[PSTAT_DEADLYSTRIKE];
			if(val) {
				if(i) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DEADLYSTRIKE, 0, 0, val), s:" ", s:"\cu", l:"DND_DEADLYSTRIKE_EXPL", s:"\n");
					k += 3;
				}
				else {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DEADLYSTRIKE, 0, 0, val), s:"\n");
					++k;
				}
			}

			val = PlayerModData[pnum].vals[PSTAT_REAPINGCLEAVE];
			if(val) {
				if(i) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_REAPINGCLEAVE, 0, 0, val), s:" ", s:"\cu", l:"DND_REAPINGCLEAVE_EXPL", s:"\n");
					k += 3;
				}
				else {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_REAPINGCLEAVE, 0, 0, val), s:"\n");
					++k;
				}
			}

			// accuracy
			val = GetActorProperty(0, APROP_ACCURACY);
			if(HasPlayerFlag(pnum, PFLAG_ACCURACY_REVERSED))
				val *= -1;
			
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ACCURACY_INCREASE, 0, 0, val), s:"\n");
				++k;
			}

			// crit block begins
			val = GetCritChance_Display(pnum);
			if(val) {
				if(val <= 1.0)
					pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_GLOBALCRIT", s:"\n");
				else {
					// over the cap special display
					val -= 1.0;
					if(val > 10.0) // some impossible cap nobody will probably reach????
						val = 10.0;
					pstat_text.text = StrParam(
						s:pstat_text.text, s:"\c[Q9]100.0", 
						s:" (\cd", s:GetFixedRepresentation(val, true), s:"\c[Q9])%\c- ", l:"DND_MENU_GLOBALCRIT", s:"\n"
					);
				}
				++k;
			}
			
			val = GetIndependentCritModifier(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_GLOBALCRITMULT", s:"\n");
				++k;
			}

			// The spell axis is separate from the attack one above -- different mods feed each, and a
			// build can have one without the other. Shown only when there is something to show, so a
			// pure gunner does not carry two dead lines.
			val = GetCritChance_Display(pnum, true);
			if(val) {
				if(val <= 1.0)
					pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_SPELLCRIT", s:"\n");
				else {
					val -= 1.0;
					if(val > 10.0)
						val = 10.0;
					pstat_text.text = StrParam(
						s:pstat_text.text, s:"\c[Q9]100.0",
						s:" (\cd", s:GetFixedRepresentation(val, true), s:"\c[Q9])%\c- ", l:"DND_MENU_SPELLCRIT", s:"\n"
					);
				}
				++k;
			}

			// Only the delta against the attack multiplier would be misleading, so this is the real
			// figure a spell crit uses -- the global pile plus the spell one.
			val = GetIndependentCritModifier(pnum, true, true);
			if(val && val != GetIndependentCritModifier(pnum)) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_SPELLCRITMULT", s:"\n");
				++k;
			}
			// crit block ends
			
			// exp repeat chance
			val = GetExplosiveRepeatChance(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_CHANCETOEXPREPEAT", s:"\n");
				++k;
			}
			
			// exp rad. Two lines rather than one sum, for the same reason artillery is listed apart:
			// they answer for different explosions, so one number could only be right for one of them.
			int aoe_all = GetPlayerAoEIncrease(pnum, DND_AOESRC_NONWEAPON);
			if(aoe_all) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:aoe_all, s:"%\c- ", l:"IATTR_T161", s:"\n");
				++k;
			}

			val = GetPlayerAoEIncrease(pnum, DND_AOESRC_WEAPON) - aoe_all;
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"IATTR_T31", s:"\n");
				++k;
			}

			// The artillery-only half, listed separately rather than summed into the line above --
			// they apply to different explosions, so one number could only be wrong for one of them.
			// Read off the slot rather than GetPlayerArtilleryAoEIncrease, which answers for the
			// weapon in hand; this is a character sheet, not a live readout.
			val = PlayerModData[pnum].vals[PSTAT_ARTILLERY_RADIUS];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ",
					l:"DND_ARTILLERYRADIUS", s:"\n");
				++k;
			}
			
			// more damage
			val = GetPlayerMoreDamageDisplay(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DAMAGEPERCENT_MORE, 0, 0, val), s:"\n");
				++k;
			}
			
			// pen block begins
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_BULLET);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_PEN_PHYSICAL, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_ENERGY);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_PEN_ENERGY, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_OCCULT);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_PEN_OCCULT, 0, 0, val), s:"\n");
				++k;
			}
			
			// get the pens of 4 elements here
			i = HasPlayerFlag(pnum, PFLAG_ELEPENHARMONY) ? GetHighestElePen(pnum) : 0;
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_FIRE);
			if(val) {
				if(i != DND_DAMAGECATEGORY_FIRE)
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_FIRE_PEN", s:"\n");
				else
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_FIRE_PEN", s:" \cf(H)\c-\n");
				++k;
			}
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_ICE);
			if(val) {
				if(i != DND_DAMAGECATEGORY_ICE)
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_ICE_PEN", s:"\n");
				else
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_ICE_PEN", s:" \cf(H)\c-\n");
				++k;
			}
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_LIGHTNING);
			if(val) {
				if(i != DND_DAMAGECATEGORY_LIGHTNING)
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_LGHT_PEN", s:"\n");
				else
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_LGHT_PEN", s:" \cf(H)\c-\n");
				++k;
			}
			val = GetResistPenetration(pnum, DND_DAMAGECATEGORY_POISON);
			if(val) {
				if(i != DND_DAMAGECATEGORY_POISON)
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_POIS_PEN", s:"\n");
				else
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_RES_POIS_PEN", s:" \cf(H)\c-\n");
				++k;
			}
			// pen block ends
			
			// flat and % dmg block
			for(i = DND_DAMAGECATEGORY_BEGIN; i < DND_DAMAGECATEGORY_END; ++i) {
				val = GetDamageTypeBonus(pnum, i, 0);
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:GetTalentTag(i), s:" ", l:"DND_DAMAGEBONUS", s:"\n");
					++k;
				}
				
				val = MapDamageCategoryToFlatBonus(pnum, i, 0);
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"\c- to ", l:GetTalentTag(i), s:" ", l:"DND_DAMAGE", s:"\n");
					++k;
				}
			}

			// no category of its own: an out of range one returns just the radius term, attack worded and generic
			val = MapDamageCategoryToFlatBonus(pnum, -1, DND_DAMAGEFLAG_ISRADIUSDMG);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"\c- to ", l:"DND_RADIUSTAG", s:" ", l:"DND_DAMAGE", s:"\n");
				++k;
			}
			// end block
			
			// blocking mons take % more
			val = PlayerModData[pnum].vals[PSTAT_BLOCKERS_MOREDMG];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_BLOCKERS_MOREDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			// pellet count -- 1.0 is base
			val = GetPelletIncrease(pnum) - 1.0;
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"IATTR_T30", s:"\n");
				++k;
			}

			// Added damage of a second type. Listed one per element rather than summed: each lands as
			// its own typed instance against its own resist, so a single total would be a fiction.
			for(i = INV_ADDED_PHYSDMG; i <= INV_ADDED_POISONDMG; ++i) {
				val = ReadPlayerModValue(pnum, i);
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(i, 0, 0, val), s:"\n");
					++k;
				}
			}

			// INV_CRITPERCENT_FORWEPTYPE is deliberately absent from this page. It has no player-scope
			// storage at all -- ProcessAttribute fans it into
			// Player_Weapon_Infos[pnum][w].wep_mods[WEP_MOD_CRITPERCENT] for every weapon of the type
			// it names. GetPercentCritChanceIncrease already sums that side, GetCritChance folds that
			// in, and the Loadout page prints the result per weapon, which is where a per-weapon-type
			// mod belongs. A player-wide line here could only ever have shown zero.

			// projectile speed
			val = PlayerModData[pnum].vals[PSTAT_PROJSPEED];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_PROJSPEED, 0, 0, val), s:"\n");
				++k;
			}

			// radius immunity ignore
			val = PlayerModData[pnum].vals[PSTAT_CHANCE_IGNORERADIUSIMMUNITY];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_CHANCE_IGNORERADIUSIMMUNITY, 0, 0, val), s:"\n");
				++k;
			}

			// melee attack cooldown and splash
			val = PlayerModData[pnum].vals[PSTAT_MELEE_ATKCDR];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_MELEE_ATKCDR, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_MELEESPLASH_NOTONLOWSTAMINA];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_MELEESPLASH_NOTONLOWSTAMINA, 0, 0, val), s:"\n");
				++k;
			}

			// more damage to bosses -- an implicit, but it applies to everything the player fires
			val = PlayerModData[pnum].vals[PSTAT_IMP_MOREDAMAGETOBOSSES];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_MOREDAMAGETOBOSSES, 0, 0, val), s:"\n");
				++k;
			}

			// INV_INC_PLUSPROJ and INV_INC_PLUSTWOPROJ are deliberately absent, same reason as
			// INV_CRITPERCENT_FORWEPTYPE above: they have no player-scope storage. ProcessAttribute folds
			// them into Player_Weapon_Infos[pnum][w].wep_mods[WEP_MOD_EXTRAPROJ] and [WEP_MOD_DMG] for the
			// one weapon their extra field names, and the Loadout page already prints both -- the
			// projectile count directly, the damage penalty through the damage line. A player-wide line
			// here could only ever have shown zero.

			val = PlayerModData[pnum].vals[PSTAT_INC_PROJREVERSE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_PROJREVERSE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_ENEMYRIPCHANCE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_ENEMYRIPCHANCE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_RIPPERSEXPLODE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_RIPPERSEXPLODE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_INVERTRESISTANCES];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_INVERTRESISTANCES, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_BLOCKPREVENTION];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_BLOCKPREVENTION, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_WEAPONDMG_PERINT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_WEAPONDMG_PERINT, 0, 0, val), s:"\n");
				++k;
			}

			// essences that act on the attack itself
			val = PlayerModData[pnum].vals[PSTAT_ESS_VAAJ];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_VAAJ, 0, 0, val, ReadPlayerModExtra(pnum, INV_ESS_VAAJ)), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ESS_SSRATH];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_SSRATH, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ESS_HARKIMONDE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_HARKIMONDE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ESS_ZRAVOG];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_ZRAVOG, 0, 0, val), s:"\n");
				++k;
			}

			// accuracy essences sit next to the accuracy line above
			val = PlayerModData[pnum].vals[PSTAT_ESS_OMNISIGHT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_OMNISIGHT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ESS_OMNISIGHT2];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_OMNISIGHT2, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ESS_THORAX];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_THORAX, 0, 0, val), s:"\n");
				++k;
			}
		}
		else if(category == DRAW_STAT_OFFENSE2) {
			// wep type specific bonuses flat and %
			val = PlayerModData[pnum].vals[PSTAT_HANDGUN_PERCENT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_HANDGUN_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLAT_HANDGUN];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_HANDGUN, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_SHOTGUN_PERCENT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_SHOTGUN_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLAT_SHOTGUN];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_SHOTGUN, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_AUTOMATIC_PERCENT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AUTOMATIC_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLAT_AUTOMATIC];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_AUTOMATIC, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ARTILLERY_PERCENT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ARTILLERY_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLAT_ARTILLERY];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_ARTILLERY, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_PRECISION_PERCENT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_PRECISION_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLAT_PRECISION];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_PRECISION, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_TECH_PERCENT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_TECH_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLAT_TECH];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_TECH, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_MAGIC_PERCENT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_MAGIC_PERCENT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLAT_MAGIC];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_MAGIC, 0, 0, val), s:"\n");
				++k;
			}

			// generic things like dot multi, dot %
			val = GetPlayerDOTMulti(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DOTMULTI, 0, 0, val), s:"\n");
				++k;
			}
			
			val = PlayerModData[pnum].vals[PSTAT_DOT_INCREASED];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INCREASEDDOT, 0, 0, val), s:"\n");
				++k;
			}

			// bleed things
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetPlayerBleedTimeDisplay(pnum), s:"\c- ", l:"DND_BLEED_TIME", s:"\n");
			++k;

			val = PlayerModData[pnum].vals[PSTAT_BLEED_CHANCE];
			pstat_text.text = StrParam(s:pstat_text.text, s:GetBleedChanceDisplay(pnum), s:"\n");
			++k;

			val = PlayerModData[pnum].vals[PSTAT_BLEED_DMG_PCT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_PERCENTDMG_BLEED, 0, 0, val), s:"\n");
				++k;
			}
			
			// fire things
			val = GetFireDOTDamage(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"\c- ", l:"DND_IGNITEDAMAGE", s:"\n");
				++k;
			}
			
			// +flat fire damage
			val = PlayerModData[pnum].vals[PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_FIRE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_FIREDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetIgniteChance(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"%\c- ", l:"DND_IGNITECHANCE", s:"\n");
				++k;
			}
			
			// The resolved duration, not the percentage: the number a player can act on. GetIgniteDuration
			// returns a LOOP COUNT, so GetPlayerIgniteTimeDisplay converts it through DND_IGNITE_TICKRATE.
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetPlayerIgniteTimeDisplay(pnum), s:"\c- ", l:"DND_IGNITE_TIME", s:"\n");
			++k;
			
			val = GetIgniteProlifChance(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"%\c- ", l:"DND_IGNITEPROLIFCHANCE", s:"\n");
				++k;
			}
			
			val = GetIgniteProlifRange(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", f:val, s:"\c- ", l:"DND_IGNITEPROLIFRANGE", s:"\n");
				++k;
			}
			
			val = GetIgniteProlifCount(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"\c- ", l:"DND_IGNITEPROLIFCOUNT", s:"\n");
				++k;
			}
			
			// ice things
			val = PlayerModData[pnum].vals[PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_ICE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_ICEDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetFreezeChance(pnum, 1);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s: "%\c- ", l:"DND_FREEZECHANCE", s:"\n");
				++k;
			}
			
			val = GetChillEffect(pnum, 1);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, l:"DND_CHILLSLOWS", s: " \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\n");
				++k;
			}
			
			val = GetChillThreshold(pnum, 1);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s: "%\c- ", l:"DND_CHILLTHRESHOLD", s:"\n");
				++k;
			}
			
			// lightning things			
			val = PlayerModData[pnum].vals[PSTAT_OVERLOADCHANCE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_OVERLOADCHANCE, 0, 0, val), s:"\n");
				++k;
			}
			
			val = PlayerModData[pnum].vals[PSTAT_OVERLOAD_ZAPCOUNT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_OVERLOAD_ZAPCOUNT, 0, 0, val), s:"\n");
				++k;
			}
			
			val = PlayerModData[pnum].vals[PSTAT_OVERLOAD_DMGINCREASE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_OVERLOAD_DMGINCREASE, 0, 0, val), s:"\n");
				++k;
			}
			
			// poison things
			val = GetPlayerPoisonStacks(pnum);
			if(val != DND_BASE_POISON_STACKS) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_MAXPOISONSTACK, 0, 0, val - DND_BASE_POISON_STACKS), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLATDMG_BASE + DND_DAMAGECATEGORY_POISON];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLAT_POISONDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			val = GetPoisonTicrate(pnum);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, l:"DND_POISONTICRATE", s:"\c[Q9]", d:(FixedDiv(val, DND_POISON_CHECKRATE) * DND_POISON_TICCHECK) >> 16, s:"\c- ", l:"DND_TICS", s:"\n");
				++k;
			}
			
			// Resolved duration, same as ignite and bleed. GetPoisonDuration is already fixed point seconds.
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetPlayerPoisonTimeDisplay(pnum), s:"\c- ", l:"DND_POISON_TIME", s:"\n");
			++k;
			
			val = PlayerModData[pnum].vals[PSTAT_POIS_TICDMG];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_POISON_TICDMG, 0, 0, val), s:"\n");
				++k;
			}
			
			// ailment ignore chance
			val = PlayerModData[pnum].vals[PSTAT_AILMENT_IGNORECHANCE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_CHANCE_AILMENTIGNORE, 0, 0, val), s:"\n");
				++k;
			}

			// Per ailment DoT multipliers. The generic one (INV_DOTMULTI) is already above; these three
			// only apply to their own ailment and do not show up in it.
			val = PlayerModData[pnum].vals[PSTAT_DOTMULTI_FIRE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DOTMULTI_FIRE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_DOTMULTI_POISON];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DOTMULTI_POISON, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_DOTMULTI_BLEED];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_DOTMULTI_BLEED, 0, 0, val), s:"\n");
				++k;
			}

			// overload duration -- the chance and the damage bonus are already listed, this was not
			val = PlayerModData[pnum].vals[PSTAT_OVERLOAD_DURATION];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_OVERLOAD_DURATION, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_ALLOVERLOAD];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_ALLOVERLOAD, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_POISONSPREAD];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_POISONSPREAD, 0, 0, val), s:"\n");
				++k;
			}

			// ailment essences
			val = PlayerModData[pnum].vals[PSTAT_ESS_CHEGOVAX];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_CHEGOVAX, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ESS_ERYXIA];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ESS_ERYXIA, 0, 0, val), s:"\n");
				++k;
			}
		}
		else if(category == DRAW_STAT_DEFENSE) {
			// hp cap
			val = GetSpawnHealth();
			pstat_text.text = StrParam(s:"\c[Q9]", d:val, s: " \c-", l:"DND_HEALTHCAPIS", s:"\n");
			++k;

			// reduced damage taken
			// inc damage taken
			val = GetPlayerLessDamageDisplay(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[D4]", s:GetFixedRepresentation(val, true), s:"% \c-", l:"DND_MOREDMGTAKEN", s:"\n");
				++k;
			}
			else if(val < 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation(val, true), s:"% \c-", l:"DND_LESSDMGTAKEN", s:"\n");
				++k;
			}
			
			// armor rating
			val = GetPlayerArmor(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:" \c-", l:"DND_ARMORCAPIS", s:"\n");
				++k;

				// estimated armor protection
				val = GetPlayerEstimatedArmorProtect(pnum, val);
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"% \c-", l:"DND_ESTREDUCTION", s:"\n");
				++k;
			}

			val = GetMitigationChance(pnum);
			if(val > 0) {
				// clamp it on display, the math stuff behind the scenes doesn't need an extra if statement
				if(val > 100.0)
					val = 100.0;
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation(val, false), s:"% \c-", l:"IATTR_T102", s:"\n");
				++k;

				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation(GetMitigationEffect(pnum), false), s:"% \c-", l:"IATTR_T103", s:"\n");
				++k;
			}
			else if((val = GetDodgeChance(pnum))) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation(val, false), s:"% \c-", l:"DND_MENU_DODGECHANCE", s:"\n");
				++k;
			}

			val = GetPlayerEnergyShieldCap(pnum);
			i = val;
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:" \c-", l:"DND_ESHIELDIS", s:"\n");
				++k;

				val = GetPlayerEnergyShieldRechargeDelay(pnum);
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation((val << 16) / TICRATE, false), s:"\c-", l:"DND_ESHIELDDELAY", s:"\n");
				++k;

				val = GetPlayerEnergyShieldRecoveryRate(pnum, i);
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:" \c-", l:"DND_ESHIELDRECOVERY", s:"\n");
				++k;

				val = GetEShieldMagicAbsorbValue(pnum);
				if(val > 100)
					val = 100;
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"% \c-", l:"DND_ESHIELDABSORB", s:"\n");
				++k;
			}
			
			// regen cap
			val = GetRegenCap(pnum);
			i = CheckInventory("PlayerHealthCap");
			if(val > i)
				val = i;
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:" \c-", l:"DND_MENU_REGENCAP", s:"\n");
			++k;
			
			// knockback
			if(CheckUniquePropertyOnPlayer(pnum, PUP_KNOCKBACKIMMUNE)) {
				pstat_text.text = StrParam(s:pstat_text.text, l:"DND_KNOCKBACKIMMUNE", s:"\n");
				++k;
			}
			else {
				val = PlayerModData[pnum].vals[PSTAT_KNOCKBACK_RESIST];
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_KNOCKBACK_RESIST, 0, 0, val), s:"\n");
					++k;
				}
			}
			
			// dmg reduction block begins -- shown with their respective caps applied
			val = 1.0 - GetPlayerSelfDamageReduction_Display(pnum);
			if(val != 0.0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_SELFEXPDMG", s:"\n");
				++k;
			}
			
			i = GetMaxResistCap(pnum);
			temp = DND_PLAYER_RESIST_REDUCE * (GetLevel() / DND_PLAYER_WEAKEN_LEVELS) + 1.0 * PlayerModData[pnum].vals[PSTAT_EX_UNITY_RES_BONUS] * GetUnity() / DND_UNITY_DIVISOR;
			val = PlayerModData[pnum].vals[PSTAT_EX_RESPERESHIELD];
			if(val)
				temp += DND_RES_PER_PRISMGUARD * (CheckInventory("EShieldAmount") / val);
			
			val = GetResistDisplayValue(pnum, DND_PRESIST_PHYS, temp);
			pstat_text.text = StrParam(s:pstat_text.text, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val > i ? i : val, false), s:" \c-(\c[Q9]", s:GetFixedRepresentation(val, false), s:"\c-) ", l:"DND_MENU_RES_PHYS", s:"\n");
			++k;
			
			val = GetResistDisplayValue(pnum, DND_PRESIST_ENERGY, temp);
			pstat_text.text = StrParam(s:pstat_text.text, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val > i ? i : val, false), s:" \c-(\c[Q9]", s:GetFixedRepresentation(val, false), s:"\c-) ", l:"DND_MENU_RES_ENRG", s:"\n");
			++k;
			
			val = GetResistDisplayValue(pnum, DND_PRESIST_MAGIC, temp);
			pstat_text.text = StrParam(s:pstat_text.text, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val > i ? i : val, false), s:" \c-(\c[Q9]", s:GetFixedRepresentation(val, false), s:"\c-) ", l:"DND_MENU_RES_MAGC", s:"\n");
			++k;
			
			val = GetResistDisplayValue(pnum, DND_PRESIST_ELEM, temp);
			pstat_text.text = StrParam(s:pstat_text.text, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val > i ? i : val, false), s:" \c-(\c[Q9]", s:GetFixedRepresentation(val, false), s:"\c-) ", l:"DND_MENU_RES_ELE", s:"\n");
			++k;
			
			val = GetResistDisplayValue(pnum, DND_PRESIST_REFL, temp);
			pstat_text.text = StrParam(s:pstat_text.text, s:val >= 0 ? "\c[Q9]" : "\cg", s:GetFixedRepresentation(val > i ? i : val, false), s:" \c-(\c[Q9]", s:GetFixedRepresentation(val, false), s:"\c-) ", l:"DND_MENU_RES_REFL", s:"\n");
			++k;
			// dmg reduction block ends

			// reduced curse things
			val = PlayerModData[pnum].vals[PSTAT_REDUCEDCURSEDURATION];
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_REDUCEDCURSEDURATION, 0, 0, val), s:"\n");
				++k;
			}

			val = GetReducedCurseEffect(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_REDUCEDCURSEEFFECT, 0, 0, val), s:"\n");
				++k;
			}

			val = GetHomingAvoidChance(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_HOMINGAVOID", s:"\n");
				++k;
			}

			// ailment avoidance
			val = GetPlayerElementalAvoidChance(pnum, DND_PAVOID_IGNITE);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AVOID_IGNITE, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerElementalAvoidChance(pnum, DND_PAVOID_CHILLFREEZE);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AVOID_CHILLFREEZE, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerElementalAvoidChance(pnum, DND_PAVOID_POISON);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AVOID_POISON, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerElementalAvoidChance(pnum, DND_PAVOID_OVERLOAD);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AVOID_OVERLOAD, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerBleedAvoidChance(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AVOID_BLEED, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerNonElementalAvoidance(pnum, INV_IMP_LESSFIRETAKEN);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_LESSFIRETAKEN, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerNonElementalAvoidance(pnum, INV_IMP_LESSPOISONTAKEN);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_LESSPOISONTAKEN, 0, 0, val), s:"\n");
				++k;
			}

			val = GetPlayerNonElementalAvoidance(pnum, INV_IMP_LESSLIGHTNINGTAKEN);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_LESSLIGHTNINGTAKEN, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_REGEN_FLAT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"\c- ", l:"DND_MENU_FLATREGEN", s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_REGEN_PCT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", s:GetFixedRepresentation(val, false), s:"%\c- ", l:"DND_MENU_PCTREGEN", s:"\n");
				++k;
			}

			// regen rate -- the CAP is listed above, the rate it refills at was not
			val = PlayerModData[pnum].vals[PSTAT_REGENRATE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_REGENRATE, 0, 0, val), s:"\n");
				++k;
			}

			// percent knockback resist -- the flat one is already listed
			val = PlayerModData[pnum].vals[PSTAT_PERCENT_KNOCKBACKRESIST];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_PERCENT_KNOCKBACKRESIST, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ARMOR_DOUBLEDEF];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_TWICEARMORDEFENSE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_HPREGENINTERRUPT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_HPREGENINTERRUPT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_INC_PASSIVEREGEN];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_PASSIVEREGEN, 0, 0, val, ReadPlayerModExtra(pnum, INV_INC_PASSIVEREGEN)), s:"\n");
				k += 2; // this one is worded over two lines
			}

			// Defensive implicits. These pass their second number in the EXTRA slot rather than the
			// val2 one -- that is the convention the implicit branch of the tooltip uses, and these
			// cases read it back out of the same place.
			val = PlayerModData[pnum].vals[PSTAT_IMP_ABSORBLIGHTNING];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_ABSORBLIGHTNING, 0, 0, val, 0, 0, false, ReadPlayerModExtra(pnum, INV_IMP_ABSORBLIGHTNING)), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_KNIGHTARMOR];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_KNIGHTARMOR, 0, 0, val, 0, 0, false, ReadPlayerModExtra(pnum, INV_IMP_KNIGHTARMOR)), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_RAVAGER];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_RAVAGER, 0, 0, val, 0, 0, false, ReadPlayerModExtra(pnum, INV_IMP_RAVAGER)), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_NECROARMOR];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_NECROARMOR, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_RECOVERESONUNDEADKILL];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_RECOVERESONUNDEADKILL, 0, 0, val), s:"\n");
				++k;
			}
		}
		else if(category == DRAW_STAT_UTILITY) {
			// utility
			// drop chance
			val = (GetDropChance(pnum) - 1.0);
			if(val != 0.0) {
				if(val > 0)
					pstat_text.text = StrParam(s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_DROPCHANCE", s:"\n");
				else
					pstat_text.text = StrParam(s:"+ \cg", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_DROPCHANCE", s:"\n");
				++k;
			}

			// What a kill pays on this map against a mid-sized one; a 0 is a client not synced yet.
			val = MapData[DND_MAPDATA_LOOTPAYOUT] ? MapData[DND_MAPDATA_LOOTPAYOUT] * 100 / DND_LOOTPAYOUT_REF - 100 : 0;
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_MAPLOOT_MORE", s:"\n");
				++k;
			}
			else if(val < 0 && val > -100) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"- \cg", d:-val, s:"%\c- ", l:"DND_MENU_MAPLOOT_LESS", s:"\n");
				++k;
			}

			val = (GetPlayerItemRarity(pnum) - 1.0);
			if(val > 0.0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"DND_MENU_RARITY", s:"\n");
				++k;
			}

			// stamina things
			val = CheckInventory("P_StaminaCap");
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"\c- ", l:"DND_MENU_STAMINACAP", s:"\n");
			++k;

			val = GetPlayerStaminaGain(pnum);
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"\c- ", l:"DND_MENU_STAMINAGAINEDPER", s:" \c[Q9]", d:GetPlayerStaminaRecoveryRate(pnum), s:"\c- ", l:"DND_MENU_TICS", s:"\n");
			++k;

			// mana things. Both read the networked copies -- the real figures need INT and the level off
			// a player TID, which does not exist on the client.
			val = CheckInventory("P_ManaCap");
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:val, s:"\c- ", l:"DND_MENU_MANACAP", s:"\n");
			++k;

			// P_ManaRegen is hundredths of mana a second; the shift puts it in the fixed point the helper wants.
			val = CheckInventory("P_ManaRegen");
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", s:GetFixedRepresentation((val << 16) / 100, false), s:"\c- ", l:"DND_MENU_MANAREGEN", s:"\n");
			++k;

			// The rest of the spell kit. Each is skipped at zero -- a gunner never spends a line on them,
			// and a caster sees only what they actually carry.
			val = PlayerModData[pnum].vals[PSTAT_SPELL_DAMAGE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_SPELLDMG", s:"\n");
				++k;
			}

			for(i = INV_INC_SPELLDMG_PERSTR; i <= INV_INC_SPELLDMG_PERDEX; ++i) {
				val = ReadPlayerModValue(pnum, i);
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(i, 0, 0, val), s:"\n");
					++k;
				}
			}

			// Flat spell damage per type, spell worded and generic summed, as the attack lines sum theirs; melee is physical's twin.
			for(i = DND_DAMAGECATEGORY_BEGIN; i < DND_DAMAGECATEGORY_END; ++i) {
				val = (i != DND_DAMAGECATEGORY_MELEE) ? GetSpellFlatDamage(pnum, i, 0) : 0;
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"\c- to ", l:GetTalentTag(i), s:" ", l:"DND_SPELLDAMAGE", s:"\n");
					++k;
				}
			}
			val = GetSpellFlatDamage(pnum, -1, DND_DAMAGEFLAG_ISRADIUSDMG);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"\c- to ", l:"DND_RADIUSTAG", s:" ", l:"DND_SPELLDAMAGE", s:"\n");
				++k;
			}

			// Tenths, with INT's and DEX's shares in.
			val = PlayerModData[pnum].vals[PSTAT_SPELL_CDR] * 10 + GetIntellectCDRTenths(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val / 10, s:".", d:val % 10, s:"%\c- ", l:"DND_MENU_SPELLCDR", s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_SPELL_HASTE] * 10 + GetDexterityHasteTenths(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val / 10, s:".", d:val % 10, s:"%\c- ", l:"DND_MENU_SPELLHASTE", s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_SPELL_AOE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_SPELLAOE", s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_SPELL_DURATION];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_SPELLDURATION", s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_MANARESERVE_EFF];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_MANARESERVE", s:"\n");
				++k;
			}

			// minion block: the eight minion mods, printed as their tooltips print them
			for(i = INV_MINION_DAMAGE; i <= INV_MINION_RES_MAGIC; ++i) {
				val = ReadPlayerModValue(pnum, i);
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(i, 0, 0, val), s:"\n");
					++k;
				}
			}

			// minion pierce: the three gear mods, then the every-type stat Sharpened Claws feeds
			for(i = INV_MINION_PEN_PHYSICAL; i <= INV_MINION_PEN_MAGIC; ++i) {
				val = ReadPlayerModValue(pnum, i);
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(i, 0, 0, val), s:"\n");
					++k;
				}
			}
			val = PlayerModData[pnum].vals[PSTAT_MINION_PEN];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"\c- ", l:"DND_MENU_MINIONPEN", s:"\n");
				++k;
			}

			// ripper block
			val = PlayerModData[pnum].vals[PSTAT_RIPCOUNT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_RIPCOUNT, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_RIPDAMAGE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_RIPDAMAGE, 0, 0, val), s:"\n");
				++k;
			}

			// lock-on block
			val = PlayerModData[pnum].vals[PSTAT_LOCKONAREA];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_LOCKONAREA, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_LOCKONRANGE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_LOCKONRANGE, 0, 0, val), s:"\n");
				++k;
			}
			
			// lifesteal block begins
			val = PlayerModData[pnum].vals[PSTAT_LIFESTEAL];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_LIFESTEAL, 0, 0, val), s:"\n");
				++k;
			}
			
			val = PlayerModData[pnum].vals[PSTAT_LIFESTEAL_CAP];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_LIFESTEAL_CAP, 0, 0, val), s:"\n");
				++k;
			}
			
			val = PlayerModData[pnum].vals[PSTAT_LIFESTEAL_RATE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_LIFESTEAL_RATE, 0, 0, val), s:"\n");
				++k;
			}
			
			val = PlayerModData[pnum].vals[PSTAT_LIFESTEAL_RECOVERY];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_LIFESTEAL_RECOVERY, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_LIFESTEAL_DAMAGE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"IATTR_T90", s:"\n");
				++k;
			}
			// lifesteal block ends

			// killing spree
			val = PlayerModData[pnum].vals[PSTAT_INCKILLINGSPREE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INCKILLINGSPREE, 0, 0, val), s:"\n");
				++k;
			}
			
			// ammo gain chance
			val = PlayerModData[pnum].vals[PSTAT_AMMOGAIN_CHANCE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AMMOGAIN_CHANCE, 0, 0, val), s:"\n");
				++k;
			}
			
			// magsize
			val = PlayerModData[pnum].vals[PSTAT_MAGAZINE_INCREASE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"IATTR_T10", s:"\n");
				++k;
			}
			
			// ammo cap inc
			val = GetAmmoCapIncrease(pnum) - 100;
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_MENU_AMMOCAPS", s:"\n");
				++k;
			}
			
			// ammo pickup inc
			val = PlayerModData[pnum].vals[PSTAT_AMMOGAIN_INCREASE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_AMMOGAIN_INCREASE, 0, 0, val), s:"\n");
				++k;
			}

			// reduced overheat
			val = PlayerModData[pnum].vals[PSTAT_REDUCED_OVERHEAT];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_REDUCED_OVERHEAT, 0, 0, val), s:"\n");
				++k;
			}

			// exp bonus
			val = GetPlayerWisdomBonus(pnum, tid);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_EXPERIENCEGAIN", s:"\n");
				++k;
			}
			
			// credit bonus
			val = GetPlayerGreedBonus(pnum, tid);
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", d:val, s:"%\c- ", l:"DND_CREDITGAIN", s:"\n");
				++k;
			}
		
			// movespeed
			val = GetBonusPlayerSpeed(pnum);
			if(val > 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"+ \c[Q9]", s:GetFixedRepresentation(val, true), s:"%\c- ", l:"IATTR_T9", s:"\n");
				++k;
			}
			else if(val < 0) {
				pstat_text.text = StrParam(s:pstat_text.text, s:"- \c[Q2]", s:GetFixedRepresentation(-val, true), s:"%\c- ", l:"IATTR_T9", s:"\n");
				++k;
			}
			
			// shop stock
			val = (GetAmmoCapIncrease(pnum) - 100) * (100 + PlayerModData[pnum].vals[PSTAT_SHOPSTOCK_INCREASE]) / 100;
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_SHOPSTOCK_INCREASE, 0, 0, val), s:"\n");
				++k;
			}

			// flask gains -- both of these are player wide, the flask's own mods are on the flask
			val = PlayerModData[pnum].vals[PSTAT_INCFLASKCHARGEGAINED];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INCFLASKCHARGEGAINED, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FLASKLIFERECOVERYRATE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FLASKLIFERECOVERYRATE, 0, 0, val), s:"\n");
				++k;
			}

			// stamina pool -- the gain and recovery rate are already listed above
			val = PlayerModData[pnum].vals[PSTAT_INC_STAMINA];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_STAMINA, 0, 0, val), s:"\n");
				++k;
			}

			// instant share of lifesteal, next to the lifesteal block above
			val = PlayerModData[pnum].vals[PSTAT_INC_INSTANTLIFESTEAL];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_INC_INSTANTLIFESTEAL, 0, 0, val, ReadPlayerModExtra(pnum, INV_INC_INSTANTLIFESTEAL)), s:"\n");
				++k;
			}

			// utility implicits
			val = PlayerModData[pnum].vals[PSTAT_IMP_BONUSPETCAP];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_BONUSPETCAP, 0, 0, val), s:" ", l:"IATTR_PETCAPMAX", s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_REDUCEDVISIONIMPAIR];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_REDUCEDVISIONIMPAIR, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_FASTEROVERHEATDISS];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_FASTEROVERHEATDISS, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_AMMOGAIN_SHOTGUNS];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_AMMOGAIN_SHOTGUNS, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_REDUCEDSLOWSHOTGUNS];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_REDUCEDSLOWSHOTGUNS, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_PHASINGTIME];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_PHASINGTIME, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_STAMINAONKILL];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_STAMINAONKILL, 0, 0, val, 0, 0, false, ReadPlayerModExtra(pnum, INV_IMP_STAMINAONKILL)), s:"\n");
				++k;
			}
		}
		else if(category == DRAW_STAT_EXOTIC) {
			// One pass over the whole unique range instead of a hand written block per mod. Every id in
			// it is an effect granted by a unique item, GetItemAttributeText already knows how to word
			// each one, and walking the range means this page cannot fall behind the next unique added.
			//
			// Nothing here is filtered on "is it shown elsewhere": the point of the page is to answer
			// "what unique effects am I carrying", so a mod that also feeds a derived number on another
			// page still belongs on this one.
			for(i = UNIQUE_ATTRIB_BEGIN; i <= UNIQUE_ATTRIB_END; ++i) {
				val = ReadPlayerModValue(pnum, i);
				if(val) {
					pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(i, 0, 0, val, ReadPlayerModExtra(pnum, i)), s:"\n");
					++k;
				}
			}

			// The powerset bits are granted by implicits that store no value of their own -- they live
			// in the player flag words and are otherwise invisible on every page. Each is worded by
			// the implicit that grants it, which is where the text for it already lives.
			if(HasPlayerFlag(pnum, PFLAG_CYBER)) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_DOUBLEESHIELDRECOVERY, 0, 0, 1), s:"\n");
				++k;
			}

			if(HasPlayerFlag(pnum, PFLAG_ESHIELDBLOCKALL)) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_ESHIELDBLOCKSALL, 0, 0, 1), s:"\n");
				++k;
			}

			if(HasPlayerFlag(pnum, PFLAG_MELEEIGNORESHIELD)) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_MELEEIGNORESSHIELDS, 0, 0, 1), s:"\n");
				++k;
			}

			if(HasPlayerFlag(pnum, PFLAG_LOWERREFLECT)) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_HIGHREFLECTREDUCE, 0, 0, 1), s:"\n");
				++k;
			}
		}
		else {
			// misc stats

			// charge duration and the on-kill charge gains, which belong with the counts below
			val = PlayerModData[pnum].vals[PSTAT_CHARGEDURATION];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_CHARGEDURATION, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_FRENZYCHARGE_ONSHATTER];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_FRENZYCHARGE_ONSHATTER, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_ENDURANCECHARGE_ONMELEE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_ENDURANCECHARGE_ONMELEE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_POWERCHARGE_ONOVERLOAD];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_POWERCHARGE_ONOVERLOAD, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_ONKILL_FRENZY];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_ONKILL_FRENZY, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_ONKILL_ENDURANCE];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_ONKILL_ENDURANCE, 0, 0, val), s:"\n");
				++k;
			}

			val = PlayerModData[pnum].vals[PSTAT_IMP_ONKILL_POWER];
			if(val) {
				pstat_text.text = StrParam(s:pstat_text.text, s:GetItemAttributeText(INV_IMP_ONKILL_POWER, 0, 0, val), s:"\n");
				++k;
			}

			// charges
			val = 0;
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:GetPlayerFrenzyCharges(tid, pnum), s: " \c-/ \c[Q9]", d:GetPlayerMaxFrenzyCharges(pnum), s:"\c- ", l:"LCHARGE_NOPRE_0", s:"\n");
			++k;

			val = 0;
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:GetPlayerEnduranceCharges(tid, pnum), s: " \c-/ \c[Q9]", d:GetPlayerMaxEnduranceCharges(pnum), s:"\c- ", l:"LCHARGE_NOPRE_1", s:"\n");
			++k;

			val = 0;
			pstat_text.text = StrParam(s:pstat_text.text, s:"\c[Q9]", d:GetPlayerPowerCharges(tid, pnum), s: " \c-/ \c[Q9]", d:GetPlayerMaxPowerCharges(pnum), s:"\c- ", l:"LCHARGE_NOPRE_2", s:"\n");
			++k;
		}
	}

	return k;
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

// input is menu based index, returns the trait it lists
int GetMonsterModMenuTrait(int i) {
	static int MonsterModGroupMapper[MAX_MONSTER_MODS] = {
		// weaknesses
		DND_ENERGY_WEAKNESS,
		DND_SILVER_WEAKNESS,
		DND_FIRE_WEAKNESS,
		DND_ICE_WEAKNESS,
		DND_MAGIC_WEAKNESS,
		DND_ELEMENTAL_WEAKNESS,
		DND_GUARDBROKEN,
		
		// resists
		DND_BULLET_RESIST,
		DND_ENERGY_RESIST,
		DND_MAGIC_RESIST,
		DND_ELEMENTAL_RESIST,
		DND_ICECREATURE,
		DND_FIRECREATURE,
		DND_STONECREATURE,
		DND_EARTHCREATURE,
		
		// immune
		DND_EXPLOSIVE_IMMUNE,
		DND_BULLET_IMMUNE,
		DND_ENERGY_IMMUNE,
		DND_MAGIC_IMMUNE,
		DND_ELEMENTAL_IMMUNE,
		DND_TOXICBLOOD,
		DND_FROSTBLOOD,
		DND_MOLTENBLOOD,
		DND_INSULATED,
		DND_ETHEREAL,
		DND_FIRE_IMMUNE,
		DND_ICE_IMMUNE,
		DND_POISON_IMMUNE,
		DND_LIGHTNING_IMMUNE,
		DND_CURSEIMMUNE,

		// aggression
		DND_RAGE,
		DND_PIERCE,
		DND_AGGRESSIVE,
		DND_HASTE,
		DND_FASTREACTION,
		DND_EXTRASTRONG,
		DND_HOMING,
		DND_ARMORPEN,
		DND_VIOLENTRETALIATION,
		DND_HATRED,
		DND_VAMPIRISM,
		DND_RUINATION,
		DND_OTHERWORLDGRIP,
		DND_THUNDERSTRUCK,
		DND_PENETRATOR,
		DND_FASTPROJ,
		DND_BORROWEDTIME,
		DND_EXHAUSTING,
		DND_EXTRASPEED,
		DND_EMBERTOUCH,
		DND_RIMETOUCH,
		DND_STORMTOUCH,
		DND_VILETOUCH,
		
		// defense
		DND_GHOST,
		DND_HARDENED_SKIN,
		DND_REFLECTIVE,
		DND_MINIONS,
		DND_HEAL,
		DND_BLOCK,
		DND_SPLIT,
		DND_DEATH,
		DND_VITAL,
		DND_ISBLOCKING,
		DND_BLOODLESS,
		DND_REBIRTH,
		DND_REJUVENATING,
		DND_FORTIFIED,
		DND_SUBORDINATE,
		DND_REPEL,
		DND_PHANTASM,
		DND_NUCLEAR,
		DND_OSMIUM,
		DND_PHASING,
		DND_TEMPORALBUBBLE,
		DND_CHTHONBLESSED,
		DND_AURA_WARD,

		// utility
		DND_CURSE,
		DND_RAISE,
		DND_TELEPORT,
		DND_RESURRECT,
		DND_MOBILITY,
		DND_POISON,
		DND_NOPAIN,
		DND_PET,
		DND_SUMMONED,
		DND_REVIVED,
		DND_THIEF,
		DND_HEXFUSION,
		DND_SHOCKER,
		DND_CRIPPLE,
		DND_SILENT,
		DND_BLACKOUT,
		DND_ENSHROUDED,
		DND_VENOMANCER,
		DND_FRIGID,
		DND_SCORCHED,
		DND_VOLTAIC,
		DND_ENERGYLEECH,
		DND_SILENCER,
		DND_MANABURNER,

		DND_MARKOFCHAOS,
		DND_MARKOFASMODEUS,
		
		DND_LEGENDARY
	};
	return MonsterModGroupMapper[i];
}

// how many traits a monster mod category lists; -1 is the special ones
int GetMonsterModMenuCount(int category) {
	switch(category) {
		case DND_TRAITCODE_WEAKNESS:
		return MAX_WEAKNESS_MODS;
		case DND_TRAITCODE_RESIST:
		return MAX_RESIST_MODS;
		case DND_TRAITCODE_IMMUNITY:
		return MAX_IMMUNITY_MODS;
		case DND_TRAITCODE_AGGRESSIVE:
		return MAX_AGGRESSIVE_MODS;
		case DND_TRAITCODE_DEFENSIVE:
		return MAX_DEFENSE_MODS;
		case DND_TRAITCODE_UTILITY:
		return MAX_UTILITY_MODS;
	}
	return 3;
}

// a trait's label and description, as the codex lists them
str GetMonsterModMenuEntry(int category, int i) {
	str key;
	int off = 0;
	switch(category) {
		case DND_TRAITCODE_WEAKNESS:
			key = StrParam(s:"DND_MENU_MMOD_WEAK", d:i + 1);
		break;
		case DND_TRAITCODE_RESIST:
			key = StrParam(s:"DND_MENU_MMOD_RES", d:i + 1);
			off = RESMODOFFSET;
		break;
		case DND_TRAITCODE_IMMUNITY:
			key = StrParam(s:"DND_MENU_MMOD_IMM", d:i + 1);
			off = IMMMODOFFSET;
		break;
		case DND_TRAITCODE_AGGRESSIVE:
			key = StrParam(s:"DND_MENU_MMOD_AGG", d:i + 1);
			off = AGGMODOFFSET;
		break;
		case DND_TRAITCODE_DEFENSIVE:
			key = StrParam(s:"DND_MENU_MMOD_DEF", d:i + 1);
			off = DEFMODOFFSET;
		break;
		case DND_TRAITCODE_UTILITY:
			key = StrParam(s:"DND_MENU_MMOD_UTI", d:i + 1);
			off = UTIMODOFFSET;
		break;
		default:
			off = MAX_MONSTER_MODS - 3;
			if(!i)
				key = "DND_MENU_MMOD_CHAOS";
			else if(i == 1)
				key = "DND_MENU_MMOD_ASMODEUS";
			else
				key = "DND_MENU_MMOD_LEG";
		break;
	}
	return StrParam(l:GetMonsterTraitLabel(GetMonsterModMenuTrait(i + off)), s:"\n", l:key);
}

bool IsBoxChangeException(int curopt, int boxid) {
	// With an item view up the box ids are that view's grid, not this page's slots -- inventory boxes
	// 13 and 14 were being refused as locked flask slots.
	if(CheckInventory("DnD_InventoryView") || CheckInventory("DnD_StashView") || CheckInventory("InTradeView"))
		return false;

	// accessory page has some exceptions (dont randomly click "nothing")
	if(curopt >= MENU_FIRST_ACCESSORY_PAGE && curopt <= MENU_LAST_ACCESSORY_PAGE) {
		if(curopt != MENU_LAST_ACCESSORY_PAGE)
			return CheckResearchStatus(RES_ACCESSORY) != RES_DONE && boxid < MBOX_6;
		return CheckResearchStatus(RES_ACCESSORY) != RES_DONE && boxid < MBOX_3;
	}

	// Flask slots the player has not unlocked. The rects live in a static table and the unlock
	// does not, so the box is refused here rather than removed -- which is also what keeps the
	// hover sound and the tooltip off it, since this is checked exactly where the box changes.
	if(curopt == MENU_LOAD_CHARM && boxid <= FLASK4_INDEX + 1 &&
		boxid > FLASK1_INDEX + GetPlayerFlaskSlots(PlayerNumber()))
		return true;

	return false;
}

void ResetSelectedDungeonInformation(int pnum) {
	SetInventory("DnD_SelectedDungeonBox", 0);
	ResetDungeonSelectData(pnum);
}

void CloseDungeonKeyView() {
	SetInventory("DnD_Inventory_Filter", 0);
	TakeInventory("DnD_InventoryView", 1);
	GiveInventory("DnD_CleanInventoryRequest", 1);
}

void HandleDungeonKeySelection(int pnum, int boxid, int choice) {
	int temp;
	int tpbid = -1;
	// a click off the grid puts the inventory away
	if(choice == DND_MENUINPUT_LCLICK && boxid == MAINBOX_NONE) {
		CloseDungeonKeyView();
		return;
	}
	if(choice == DND_MENUINPUT_LCLICK) {
		if(boxid != MAINBOX_NONE && (temp = GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, boxid - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY)) != DND_ITEM_NULL) {
			tpbid = GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) - 1;
			if(temp != DND_ITEM_DUNGEONKEY) {
				// failed, popup
				ShowPopup(POPUP_ITEMTYPEMISMATCH, false, 0);
			}
			else {
				// success, place it for activation
				temp = GetItemSyncValue(pnum, DND_SYNC_ITEMSUBTYPE, tpbid, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY);
				GetSelectedDungeonData(pnum).id = temp;
				SendOwnerSync("DnD Selected Dungeon Sync", pnum, pnum, temp, 0, 0);

				// save the topleftbox for later removal in case player activates this
				SetInventory("DnD_SelectedDungeonBox", tpbid + 1);

				CloseDungeonKeyView();
				LocalAmbientSound("RPG/MenuChoose", 127);
			}
		}
	}
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
			MenuClipRect(192, 248, 256, 32, 256);
			HudMessage(l:StrParam(s:"DND_NFT_EXP", d:i); HUDMSG_PLAIN, RPGMENUITEMID - MAX_NFTS - 1 - i, CR_WHITE, MenuX(192.1), MenuY(248.1 + 1.0 * ScrollPos.x), 0.0, 0.0);
			MenuClipRect(0, 0, 0, 0, 0);
			
			// draw the nft image in left corner
			SetFont(StrParam(s:"NFT", d:i));
			MenuHudSize(2576, 1960);
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 68, CR_WHITE, MenuX(532.0), MenuY(444.0), 0.0, 0.0);
			
			SetFont("NMENUFNT");
			MenuHudSize(HUDMAX_X, HUDMAX_Y);
		}
	}
}
#endif

// ---------------------------------------------------------------- the frame and its pages

// The redesigned menu canvas. The engine maps a hud's width onto the 4:3 area of the screen, so a 4:3 canvas is the
// only one drawn 1:1; the 480x320 design space it stands in for is 3.0 wide and 3.375 tall per unit at 1080p.
#define MCH_W 1440
#define MCH_H 1080
#define MCH_SCALE_X 3.0
#define MCH_SCALE_Y 3.375

#define MCH_FX -220
#define MCH_FY 10
#define MCH_FW 1880
// how far past the design space the cursor may go on these pages: the 16:9 width
#define MCH_CURSOR_OVER 80

// Lower ids draw in front; ClearMenuDisplay already sweeps this band when the menu closes.
#define MCH_ID_ROWTXT		1760
#define MCH_ID_PANETXT		1796
#define MCH_ID_PAGETXT		1820
#define MCH_ID_CHIPTXT		1832
#define MCH_ID_TABTXT		1842
#define MCH_ID_STATTXT		1859
// sub tabs: labels from MCH_ID_SUBTXT, plates from MCH_ID_SUBIMG
#define MCH_MAX_SUBS		11
#define MCH_ID_PANEIMG		1862
#define MCH_ID_ROWIMG		1892
#define MCH_ID_CHIPIMG		1916
#define MCH_ID_PAGEIMG		1926
#define MCH_ID_BADGEIMG		1934
#define MCH_ID_TABIMG		1938
#define MCH_ID_SUBTXT		1960
#define MCH_ID_SUBIMG		1972

// Chrome clicks travel as mainboxid MCH_NAV_BASE + element code; the server decodes the code against its own page.
#define MCH_NAV_BASE		256
#define MCH_EL_TAB			0
#define MCH_EL_SUB			32
#define MCH_EL_CHIP			64
#define MCH_EL_PAGER		96
#define MCH_EL_CLOSE		128
// the confirm popup's buttons never reach the server as nav: they turn into the old confirm / cancel clicks
#define MCH_EL_POPYES		160
#define MCH_EL_POPNO		161
// research list rows: picked on the client, never sent
#define MCH_EL_ROW			192
// crafting: a material into the chamber (+ its inventory box), a recipe into it (+ the orb), emptying it
#define MCH_EL_CRFILL		512
#define MCH_EL_CRRECIPE		576
#define MCH_EL_CREMPTY		704
#define MCH_RES_BTNX		634
#define MCH_RES_BTNY		852
#define MCH_RES_BTNSTEP		220
#define MCH_CODEX_TOPICS	9
// codex landing page: the options rows sit in the pane, MMODS' category rows too
#define MCH_CODEX_OPTY		626
#define MCH_CODEX_CATY		420

// the confirm popup draws in front of the whole menu, just behind the cursor; the menu's close sweep covers it
#define MCH_ID_POPTXT		108
#define MCH_ID_POPIMG		113
#define MCH_ID_POPFIRST		108
#define MCH_ID_POPLAST		116
// the terminal confirm: a 720x300 card, its art carrying a 48 px margin of glow and shadow
#define MCH_POP_X			360
#define MCH_POP_Y			390
#define MCH_POP_W			720
#define MCH_POP_BTNY		614
#define MCH_POP_YESX		640
#define MCH_POP_NOX			856
#define MCH_TERM_M			48
// the terminal error card, 720x210, on the ids the old error box had (101 is the dragged item, 106 the trade timer)
#define MCH_ID_TERMTXT		102
#define MCH_ID_TERMCARD		105
#define MCH_ID_TERMDIM		107

// layout, canvas pixels
#define MCH_TAB_X			-184
#define MCH_TAB_Y			42
#define MCH_TAB_W			200
#define MCH_TAB_H			52
#define MCH_TAB_STEP		210
#define MCH_KEY_X			1358
#define MCH_KEY_W			50
#define MCH_KEY_STEP		56
#define MCH_CLOSE_X			1528
#define MCH_CLOSE_W			96
#define MCH_SUB_X			-184
#define MCH_SUB_Y			116
#define MCH_SUB_W			150
#define MCH_SUB_H			34
#define MCH_CHIP_X			210
#define MCH_CHIP_Y			200
#define MCH_CHIP_W			48
#define MCH_CHIP_H			42
#define MCH_ROW_X			-146
#define MCH_ROW_Y			278
#define MCH_ROW_W			720
#define MCH_ROW_H			54
#define MCH_ROW_STEP		60
#define MCH_PAGER_Y			892
#define MCH_PANE_X			610
#define MCH_PANE_Y			278
#define MCH_STATUS_Y		1020
#define MCH_PANE_IX 634
#define MCH_PANE_TX 920
#define MCH_PANE_TEXTW 928
#define MCH_PANE_FOOT 868

enum {
	MCH_SEC_CHARACTER,
	MCH_SEC_SKILLS,
	MCH_SEC_GEAR,
	MCH_SEC_SHOP,
	MCH_SEC_RESEARCH,
	MCH_SEC_WORLD,
	MCH_SEC_CODEX,
	MCH_SEC_COUNT
};
#define MCH_SEC_NONE -1

enum {
	MCH_KIND_NONE,
	MCH_KIND_SHOPINDEX,
	MCH_KIND_SLOTLIST,
	MCH_KIND_WEAPONS,
	MCH_KIND_AMMOSELECT,
	MCH_KIND_AMMO,
	MCH_KIND_ACCOUNT,
	MCH_KIND_OVERVIEW,
	MCH_KIND_STATINDEX,
	MCH_KIND_STATS,
	MCH_KIND_PERKINDEX,
	MCH_KIND_PERKTREE,
	MCH_KIND_RESINDEX,
	MCH_KIND_RESGUNS,
	MCH_KIND_RESLIST,
	MCH_KIND_CODEXINDEX,
	MCH_KIND_CODEX,
	MCH_KIND_SKILLINDEX,
	MCH_KIND_GEARINDEX,
	MCH_KIND_GEARDISCARD,
	MCH_KIND_GEARACC,
	MCH_KIND_GEAREQUIP,
	MCH_KIND_GEARINV,
	MCH_KIND_GEARSTASH,
	MCH_KIND_GEARTRADE,
	MCH_KIND_DUNGEONS,
	MCH_KIND_HOME,
	MCH_KIND_CRAFT
};

bool IsChromePage(int page) {
	return (page >= MENU_SHOP && page <= MENU_SHOP_ACCOUNT) || (page >= MENU_STAT1 && page <= MENU_PERKTREE_LAST) || (page >= MENU_RESEARCH && page <= SHOP_RESPAGE_END) || (page >= MENU_HELP && page <= MENU_MMOD_HELP_END) || page == MENU_SPELL || page == MENU_LOAD || page == MENU_LOAD_INVENTORY || page == MENU_LOAD_STASH || page == MENU_LOAD_CHARM || page == MENU_LOAD_WEAPONDISCARD
		|| (page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE) || page == MENU_LOAD_DUNGEONS || page == MENU_MAIN
		|| (page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE) || IsChromeCraftPage(page);
}

int GetChromeKind(int page) {
	if(page == MENU_SHOP)
		return MCH_KIND_SHOPINDEX;
	if(page == MENU_SHOP_WEAPON)
		return MCH_KIND_SLOTLIST;
	if(page >= MENU_SHOP_WEAPON1 && page <= MENU_SHOP_WEAPON8)
		return MCH_KIND_WEAPONS;
	if(page == MENU_SHOP_AMMOSELECT)
		return MCH_KIND_AMMOSELECT;
	if(page >= MENU_SHOP_AMMO_1 && page <= MENU_SHOP_AMMO_SPECIAL1)
		return MCH_KIND_AMMO;
	if(page == MENU_SHOP_ACCOUNT)
		return MCH_KIND_ACCOUNT;
	if(page == MENU_STAT1)
		return MCH_KIND_OVERVIEW;
	if(page == MENU_STAT2)
		return MCH_KIND_STATINDEX;
	if(page >= MENU_STAT2_OFFENSE1 && page <= MENU_STAT2_MISC)
		return MCH_KIND_STATS;
	if(page == MENU_PERK)
		return MCH_KIND_PERKINDEX;
	if(IsPerkTreePage(page))
		return MCH_KIND_PERKTREE;
	if(page == MENU_RESEARCH)
		return MCH_KIND_RESINDEX;
	if(page == MENU_RESEARCH_GUNS)
		return MCH_KIND_RESGUNS;
	if(page >= SHOP_RESPAGE_BEGIN && page <= SHOP_RESPAGE_END)
		return MCH_KIND_RESLIST;
	if(page == MENU_HELP)
		return MCH_KIND_CODEXINDEX;
	if(page > MENU_HELP && page <= MENU_MMOD_HELP_END)
		return MCH_KIND_CODEX;
	if(page == MENU_SPELL)
		return MCH_KIND_SKILLINDEX;
	if(page == MENU_LOAD)
		return MCH_KIND_GEARINDEX;
	if(page == MENU_LOAD_WEAPONDISCARD)
		return MCH_KIND_GEARDISCARD;
	if(page == MENU_LOAD_CHARM)
		return MCH_KIND_GEAREQUIP;
	if(page == MENU_LOAD_INVENTORY)
		return MCH_KIND_GEARINV;
	if(page == MENU_LOAD_STASH)
		return MCH_KIND_GEARSTASH;
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return MCH_KIND_GEARTRADE;
	if(page == MENU_LOAD_DUNGEONS)
		return MCH_KIND_DUNGEONS;
	if(page == MENU_MAIN)
		return MCH_KIND_HOME;
	if(IsChromeCraftPage(page))
		return MCH_KIND_CRAFT;
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return MCH_KIND_GEARACC;
	return MCH_KIND_NONE;
}

int GetChromeSection(int page) {
	if(page == MENU_LOAD_DUNGEONS)
		return MCH_SEC_WORLD;
	if(page >= MENU_SHOP && page <= MENU_SHOP_ACCOUNT)
		return MCH_SEC_SHOP;
	if(page >= MENU_STAT1 && page <= MENU_PERKTREE_LAST)
		return MCH_SEC_CHARACTER;
	if(page >= MENU_RESEARCH && page <= SHOP_RESPAGE_END)
		return MCH_SEC_RESEARCH;
	if(page >= MENU_HELP && page <= MENU_MMOD_HELP_END)
		return MCH_SEC_CODEX;
	if(page == MENU_SPELL || IsSpellPage(page))
		return MCH_SEC_SKILLS;
	if((page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE) || page == MENU_LOAD || page == MENU_LOAD_INVENTORY || page == MENU_LOAD_STASH || page == MENU_LOAD_CHARM || page == MENU_LOAD_WEAPONDISCARD || (page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE) || IsChromeCraftPage(page))
		return MCH_SEC_GEAR;
	return MCH_SEC_NONE;
}

str GetChromeTabLabel(int sec) {
	switch(sec) {
		case MCH_SEC_CHARACTER:
		return "DND_MCH_TAB_CHARACTER";
		case MCH_SEC_SKILLS:
		return "DND_MCH_TAB_SKILLS";
		case MCH_SEC_GEAR:
		return "DND_MCH_TAB_GEAR";
		case MCH_SEC_SHOP:
		return "DND_MCH_TAB_SHOP";
		case MCH_SEC_RESEARCH:
		return "DND_MCH_TAB_RESEARCH";
		case MCH_SEC_WORLD:
		return "DND_MCH_TAB_WORLD";
	}
	return "DND_MCH_TAB_CODEX";
}

// Where each tab lands until its section is redesigned.
int GetChromeTabTarget(int sec) {
	switch(sec) {
		case MCH_SEC_CHARACTER:
		return MENU_STAT1;
		case MCH_SEC_SKILLS:
		return MENU_SPELL;
		case MCH_SEC_GEAR:
		return MENU_LOAD;
		case MCH_SEC_SHOP:
		return MENU_SHOP_WEAPON1;
		case MCH_SEC_RESEARCH:
		return MENU_RESEARCH;
		case MCH_SEC_WORLD:
		return MENU_LOAD_DUNGEONS;
		case MCH_SEC_CODEX:
		return MENU_HELP;
	}
	return -1;
}

int GetChromeSubCount(int sec) {
	if(sec == MCH_SEC_SHOP || sec == MCH_SEC_CHARACTER)
		return 3;
	if(sec == MCH_SEC_RESEARCH)
		return 5;
	if(sec == MCH_SEC_SKILLS)
		return 3 + MAX_SKILL_TREES;
	if(sec == MCH_SEC_GEAR)
		return 8;
	return 0;
}

// the skill section's eleven sub tabs are narrower
int GetChromeSubW(int sec) {
	if(sec == MCH_SEC_SKILLS || sec == MCH_SEC_GEAR)
		return 128;
	return MCH_SUB_W;
}

str GetChromeSubLabel(int sec, int i) {
	if(sec == MCH_SEC_GEAR)
		return i ? StrParam(s:"DND_MCH_GSUB", d:i + 1) : "DND_MCH_SUB_OVERVIEW";
	if(sec == MCH_SEC_SKILLS) {
		if(!i)
			return "DND_MCH_SUB_OVERVIEW";
		if(i == 1)
			return "DND_MCH_SUB_HOTBAR";
		if(i == 2)
			return "DND_MCH_SUB_GEMS";
		return StrParam(s:"DND_SPELLTREE", d:i - 3);
	}
	if(sec == MCH_SEC_RESEARCH) {
		switch(i) {
			case 0:
			return "DND_MCH_SUB_OVERVIEW";
			case 1:
			return "DND_MCH_RES_BODY";
			case 2:
			return "DND_MCH_SUB_AMMO";
			case 3:
			return "DND_MCH_SUB_WEAPONS";
		}
		return "DND_MCH_STAT_UTIL";
	}
	if(sec == MCH_SEC_CHARACTER) {
		if(!i)
			return "DND_MCH_SUB_OVERVIEW";
		if(i == 1)
			return "DND_MCH_SUB_STATSHEET";
		return "DND_MCH_SUB_PERKS";
	}
	if(!i)
		return "DND_MCH_SUB_WEAPONS";
	if(i == 1)
		return "DND_MCH_SUB_AMMO";
	return "DND_MCH_SUB_ACCOUNT";
}

int GetChromeSubTarget(int sec, int i) {
	if(sec == MCH_SEC_GEAR) {
		if(i < 0 || i > 7)
			return -1;
		return i ? MchGearRowPage(i) : MENU_LOAD;
	}
	if(sec == MCH_SEC_SKILLS) {
		if(i < 0 || i >= 3 + MAX_SKILL_TREES)
			return -1;
		if(!i)
			return MENU_SPELL;
		if(i == 1)
			return MENU_SPELL_HOTBAR;
		if(i == 2)
			return MENU_SPELL_GEMS;
		return MENU_SPELLTREE_FIRST + i - 3;
	}
	if(sec == MCH_SEC_RESEARCH) {
		switch(i) {
			case 0:
			return MENU_RESEARCH;
			case 1:
			return MENU_RESEARCH_BODY;
			case 2:
			return MENU_RESEARCH_AMMO;
			case 3:
			return MENU_RESEARCH_SLOTGUNS;
			case 4:
			return MENU_RESEARCH_UTILITY;
		}
		return -1;
	}
	if(i < 0 || i > 2)
		return -1;
	if(sec == MCH_SEC_CHARACTER) {
		if(!i)
			return MENU_STAT1;
		if(i == 1)
			return MENU_STAT2_OFFENSE1;
		return MENU_PERK;
	}
	if(sec != MCH_SEC_SHOP)
		return -1;
	if(!i)
		return MENU_SHOP_WEAPON1;
	if(i == 1)
		return MENU_SHOP_AMMO_1;
	return MENU_SHOP_ACCOUNT;
}

int GetChromeSubActive(int page) {
	if(page == MENU_LOAD)
		return 0;
	if(page == MENU_LOAD_INVENTORY)
		return 1;
	if(page == MENU_LOAD_STASH)
		return 2;
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return 7;
	if(page == MENU_LOAD_CHARM)
		return 3;
	if(page == MENU_LOAD_WEAPONDISCARD)
		return 4;
	if(IsChromeCraftPage(page))
		return 5;
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return 6;
	if(page == MENU_SPELL)
		return 0;
	if(page == MENU_SPELL_HOTBAR)
		return 1;
	if(page == MENU_SPELL_GEMS)
		return 2;
	if(IsSpellTreePage(page))
		return 3 + page - MENU_SPELLTREE_FIRST;
	int k = GetChromeKind(page);
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		return 0;
	if(k == MCH_KIND_AMMOSELECT || k == MCH_KIND_AMMO)
		return 1;
	if(k == MCH_KIND_ACCOUNT)
		return 2;
	if(k == MCH_KIND_OVERVIEW)
		return 0;
	if(k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS)
		return 1;
	if(k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE)
		return 2;
	switch(page) {
		case MENU_RESEARCH:
		return 0;
		case MENU_RESEARCH_BODY:
		return 1;
		case MENU_RESEARCH_AMMO:
		return 2;
		case MENU_RESEARCH_GUNS:
		case MENU_RESEARCH_SLOTGUNS:
		case MENU_RESEARCH_LUXURYGUNS:
		return 3;
		case MENU_RESEARCH_UTILITY:
		return 4;
	}
	return -1;
}

int GetChromeChipCount(int page) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS || k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE)
		return 8;
	if(k == MCH_KIND_AMMOSELECT || k == MCH_KIND_AMMO || k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS)
		return 5;
	if(page == MENU_RESEARCH_GUNS || page == MENU_RESEARCH_SLOTGUNS || page == MENU_RESEARCH_LUXURYGUNS)
		return 2;
	if(MchIsMonsterModPage(page))
		return 7;
	if(k == MCH_KIND_CRAFT)
		return 3;
	return 0;
}

bool MchIsMonsterModPage(int page) {
	return page == MENU_HELP_MMODS || (page >= MENU_MMOD_HELP_BEGIN && page <= MENU_MMOD_HELP_END);
}

// stat category and perk archetype chips carry words, the rest digits
bool ChromeChipsAreWords(int page) {
	int k = GetChromeKind(page);
	return k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS || k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE || k == MCH_KIND_RESGUNS || k == MCH_KIND_RESLIST || MchIsMonsterModPage(page) || k == MCH_KIND_CRAFT;
}

int GetChromeChipW(int page) {
	if(ChromeChipsAreWords(page))
		return 130;
	return MCH_CHIP_W;
}

str GetChromeWordChipLabel(int page, int i) {
	if(MchIsMonsterModPage(page))
		return StrParam(s:"DND_MCH_MMOD", d:i + 1);
	if(GetChromeKind(page) == MCH_KIND_CRAFT)
		return StrParam(s:"DND_MCH_CR_CHIP", d:i);
	if(GetChromeKind(page) >= MCH_KIND_RESINDEX)
		return i ? "DND_MCH_RES_LUX" : "DND_MCH_RES_SLOT";
	if(GetChromeKind(page) >= MCH_KIND_PERKINDEX)
		return GetPerkArchLangKey(i);
	return GetChromeStatChipLabel(i);
}

str GetChromeStatChipLabel(int i) {
	switch(i) {
		case 0:
		return "DND_MCH_STAT_OFF";
		case 1:
		return "DND_MCH_STAT_DEF";
		case 2:
		return "DND_MCH_STAT_UTIL";
		case 3:
		return "DND_MCH_STAT_EXO";
	}
	return "DND_MCH_STAT_MISC";
}

int GetChromeChipTarget(int page, int i) {
	int k = GetChromeKind(page);
	if(i < 0 || i >= GetChromeChipCount(page))
		return -1;
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		return GetWeaponPage(MBOX_1 + i);
	if(k == MCH_KIND_STATINDEX || k == MCH_KIND_STATS) {
		if(!i)
			return MENU_STAT2_OFFENSE1;
		return MENU_STAT2_DEFENSE + i - 1;
	}
	if(k == MCH_KIND_RESGUNS || k == MCH_KIND_RESLIST)
		return i ? MENU_RESEARCH_LUXURYGUNS : MENU_RESEARCH_SLOTGUNS;
	if(MchIsMonsterModPage(page))
		return MENU_MMOD_HELP_BEGIN + i;
	if(k == MCH_KIND_CRAFT)
		return MENU_LOAD_CRAFTING_FIRST + i;
	// an archetype with nothing authored has no page to open
	if(k == MCH_KIND_PERKINDEX || k == MCH_KIND_PERKTREE) {
		EnsurePerkArchLists();
		if(!GetPerkArchList(i).count)
			return -1;
		return MENU_PERKTREE_FIRST + i;
	}
	switch(i) {
		case 0:
		return MENU_SHOP_AMMO_1;
		case 1:
		return MENU_SHOP_AMMO_2;
		case 2:
		return MENU_SHOP_AMMO_3;
		case 3:
		return MENU_SHOP_AMMO_4_1;
	}
	return MENU_SHOP_AMMO_SPECIAL1;
}

int GetChromeChipActive(int page) {
	if(IsPerkTreePage(page))
		return page - MENU_PERKTREE_FIRST;
	if(page == MENU_RESEARCH_SLOTGUNS)
		return 0;
	if(page == MENU_RESEARCH_LUXURYGUNS)
		return 1;
	if(page >= MENU_MMOD_HELP_BEGIN && page <= MENU_MMOD_HELP_END)
		return page - MENU_MMOD_HELP_BEGIN;
	if(IsChromeCraftPage(page))
		return page - MENU_LOAD_CRAFTING_FIRST;
	switch(page) {
		case MENU_SHOP_WEAPON1:
		case MENU_SHOP_AMMO_1:
		case MENU_SHOP_AMMO_1_2:
		case MENU_STAT2_OFFENSE1:
		case MENU_STAT2_OFFENSE2:
		return 0;
		case MENU_STAT2_DEFENSE:
		return 1;
		case MENU_STAT2_UTILITY:
		return 2;
		case MENU_STAT2_EXOTIC:
		return 3;
		case MENU_STAT2_MISC:
		return 4;
		case MENU_SHOP_WEAPON2:
		case MENU_SHOP_AMMO_2:
		case MENU_SHOP_AMMO_2_2:
		return 1;
		case MENU_SHOP_WEAPON3_1:
		case MENU_SHOP_WEAPON3_2:
		case MENU_SHOP_WEAPON3_3:
		case MENU_SHOP_AMMO_3:
		return 2;
		case MENU_SHOP_WEAPON4_1:
		case MENU_SHOP_WEAPON4_2:
		case MENU_SHOP_AMMO_4_1:
		case MENU_SHOP_AMMO_4_2:
		return 3;
		case MENU_SHOP_WEAPON5_1:
		case MENU_SHOP_WEAPON5_2:
		case MENU_SHOP_AMMO_SPECIAL1:
		return 4;
		case MENU_SHOP_WEAPON6_1:
		case MENU_SHOP_WEAPON6_2:
		return 5;
		case MENU_SHOP_WEAPON7:
		return 6;
		case MENU_SHOP_WEAPON8:
		return 7;
	}
	return -1;
}

// page number (1 based) and page count of a multi page weapon slot; 1 of 1 otherwise
int GetChromePageNumber(int page) {
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return page - MENU_FIRST_TRADE + 1;
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return page - MENU_FIRST_ACCESSORY_PAGE + 1;
	if(page == MENU_STAT2_OFFENSE2)
		return 2;
	return 1;
}

int GetChromePageCount(int page) {
	if(page >= MENU_FIRST_TRADE && page <= MENU_LAST_TRADE)
		return Clamp_Between((PlayerCount() - 2) / MENU_TRADE_PLAYERCOUNT + 1, 1, MENU_TRADE_PAGECOUNT);
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return MENU_LAST_ACCESSORY_PAGE - MENU_FIRST_ACCESSORY_PAGE + 1;
	if(page == MENU_STAT2_OFFENSE1 || page == MENU_STAT2_OFFENSE2)
		return 2;
	return 1;
}

// Weapon pages page with boxes the server already numbers (next, then previous); ammo pages page through chrome nav.
bool ChromePagerIsBoxes(int page) {
	return GetChromeKind(page) == MCH_KIND_WEAPONS || GetChromeKind(page) == MCH_KIND_GEARACC;
}

int GetChromePagerTarget(int page, int dir) {
	if(GetChromePageCount(page) < 2)
		return -1;
	int n = GetChromePageNumber(page);
	if(dir < 0 && n > 1)
		return page - 1;
	if(dir > 0 && n < GetChromePageCount(page))
		return page + 1;
	return -1;
}

int GetChromePagerBoxes(int page) {
	if(!ChromePagerIsBoxes(page))
		return 0;
	return (GetChromePagerTarget(page, 1) != -1) + (GetChromePagerTarget(page, -1) != -1);
}

// Decodes a chrome click on the server against the page it is on.
int GetChromeElementTarget(int page, int code) {
	if(code >= MCH_EL_TAB && code < MCH_EL_TAB + MCH_SEC_COUNT)
		return GetChromeTabTarget(code - MCH_EL_TAB);
	if(code >= MCH_EL_SUB && code < MCH_EL_CHIP)
		return GetChromeSubTarget(GetChromeSection(page), code - MCH_EL_SUB);
	if(code >= MCH_EL_CHIP && code < MCH_EL_PAGER)
		return GetChromeChipTarget(page, code - MCH_EL_CHIP);
	if(code >= MCH_EL_ROW && code < MCH_EL_ROW + MCH_CODEX_TOPICS && GetChromeKind(page) == MCH_KIND_CODEX)
		return MENU_HELP_BEGIN + code - MCH_EL_ROW;
	if(code == MCH_EL_PAGER && !ChromePagerIsBoxes(page))
		return GetChromePagerTarget(page, -1);
	if(code == MCH_EL_PAGER + 1 && !ChromePagerIsBoxes(page))
		return GetChromePagerTarget(page, 1);
	return -1;
}

void HandleChromeNav(int pnum, int page, int code) {
	if(code == MCH_EL_CLOSE) {
		ACS_ExecuteWithResult(900, 0, 0);
		return;
	}
	if(page == MENU_LOAD_CRAFTING_TRANSMUTING) {
		if(code >= MCH_EL_CRFILL && code < MCH_EL_CRFILL + MAX_INVENTORY_BOXES) {
			MchTransmuteFill(pnum, code - MCH_EL_CRFILL);
			return;
		}
		if(code >= MCH_EL_CRRECIPE && code < MCH_EL_CRRECIPE + DND_MAX_ORB_KINDS) {
			MchTransmuteRecipe(pnum, code - MCH_EL_CRRECIPE);
			return;
		}
		if(code == MCH_EL_CREMPTY) {
			ResetTransmutingData(pnum);
			LocalAmbientSound("RPG/MenuChoose", 127);
			return;
		}
	}
	int target = GetChromeElementTarget(page, code);
	if(target <= 0)
		return;
	if(MchPageLocked(target)) {
		ShowPopup(POPUP_NEEDRESEARCH, false, 0);
		return;
	}
	// the same side effects the gear index rows have, so a tab lands on the page in the state its row leaves it
	SetInventory("DnD_SelectedInventoryBox", 0);
	SetInventory("DnD_UsedTwoItemRequirementMaterial", 0);
	if(target == MENU_LOAD_CRAFTING_TRANSMUTING)
		ResetTransmutingData(pnum);
	if(target == MENU_LOAD_INVENTORY)
		GiveInventory("DnD_InventoryView", 1);
	else if(target == MENU_LOAD_STASH)
		GiveInventory("DnD_StashView", 1);
	else if(target == MENU_LOAD_DUNGEONS)
		ResetDungeonSelectData(pnum);
	UpdateMenuPosition(target);
}

// ---------------------------------------------------------------- geometry

// The old equipment page's layout (480x320 units) at 2.4x, the charm and armor groups centred as a pair.
#define MCH_EQ_CHARMX 327
#define MCH_EQ_ARMORX 872
#define MCH_EQ_Y 322
#define MCH_EQ_BTNX 1370
#define MCH_EQ_BTNY 868
// the dungeon key socket, centred in the list column, Activate under it
#define MCH_DUN_X 154
#define MCH_DUN_Y 380

int MchEqX(int x) {
	if(x < 290)
		return MCH_EQ_CHARMX + (x - 188) * 12 / 5;
	return MCH_EQ_ARMORX + (x - 296) * 12 / 5;
}

int MchEqY(int y) {
	return MCH_EQ_Y + (y - 38) * 12 / 5;
}

// where the old page centres equipped slot i, in its own units
int MchEqSlotX(int i) {
	switch(i) {
		case SMALLCHARM_INDEX1:
		case SMALLCHARM_INDEX3:
		case MEDIUMCHARM_INDEX1:
		return 208;
		case SMALLCHARM_INDEX2:
		case SMALLCHARM_INDEX4:
		case MEDIUMCHARM_INDEX2:
		return 264;
		case LARGECHARM_INDEX:
		return 236;
		case POWERCORE_INDEX:
		return 338;
		case HELM_INDEX:
		case BODY_ARMOR_INDEX:
		case BOOT_INDEX:
		return 394;
	}
	return ((i - FLASK1_INDEX) & 1) ? 348 : 312;
}

int MchEqSlotY(int i) {
	switch(i) {
		case SMALLCHARM_INDEX1:
		case SMALLCHARM_INDEX2:
		return 208;
		case SMALLCHARM_INDEX3:
		case SMALLCHARM_INDEX4:
		return 256;
		case MEDIUMCHARM_INDEX1:
		case MEDIUMCHARM_INDEX2:
		return 154;
		case POWERCORE_INDEX:
		case BODY_ARMOR_INDEX:
		return 152;
		case LARGECHARM_INDEX:
		return 80;
		case HELM_INDEX:
		return 88;
		case BOOT_INDEX:
		return 216;
	}
	return ((i - FLASK1_INDEX) & 2) ? 247 : 207;
}

str MchEqWell(int i) {
	if(i <= SMALLCHARM_INDEX4)
		return "MCEQS";
	if(i <= MEDIUMCHARM_INDEX2)
		return "MCEQM";
	if(i == LARGECHARM_INDEX)
		return "MCEQG";
	if(i <= BOOT_INDEX)
		return "MCEQA";
	return "MCEQF";
}

int MchEqWellW(int i) {
	if(i >= FLASK1_INDEX)
		return 76;
	return (i >= HELM_INDEX) ? 120 : 96;
}

int MchEqWellH(int i) {
	if(i == LARGECHARM_INDEX)
		return 202;
	if(i == MEDIUMCHARM_INDEX1 || i == MEDIUMCHARM_INDEX2)
		return 134;
	return MchEqWellW(i);
}

// each slot of a kind has its own worn well (variant 0 is the plain name), so neighbours never repeat
str MchWellPic(str base, int slot, bool hover) {
	int v = slot - SMALLCHARM_INDEX1;
	if(slot >= FLASK1_INDEX)
		v = slot - FLASK1_INDEX;
	else if(slot >= HELM_INDEX)
		v = slot - HELM_INDEX;
	else if(slot == LARGECHARM_INDEX)
		v = 0;
	else if(slot >= MEDIUMCHARM_INDEX1)
		v = slot - MEDIUMCHARM_INDEX1;
	if(v)
		return StrParam(s:base, d:v, s:hover ? "H" : "");
	return StrParam(s:base, s:hover ? "H" : "");
}

int MchCursorX() {
	return FixedMul(HUDMAX_XF - (PlayerCursorData.posx & MMASK), MCH_SCALE_X) >> 16;
}

int MchCursorY() {
	return FixedMul(HUDMAX_YF - (PlayerCursorData.posy & MMASK), MCH_SCALE_Y) >> 16;
}

bool MchIn(int cx, int cy, int x, int y, int w, int h) {
	return cx >= x && cx < x + w && cy >= y && cy < y + h;
}

// a canvas rect as a pane box: the cursor runs mirrored in the 480x320 design space
void MchAddBox(menu_pane_T module& p, int x, int y, int w, int h) {
	AddBoxToPane_Points(
		p,
		HUDMAX_XF - FixedDiv(x << 16, MCH_SCALE_X), HUDMAX_YF - FixedDiv(y << 16, MCH_SCALE_Y),
		HUDMAX_XF - FixedDiv((x + w) << 16, MCH_SCALE_X), HUDMAX_YF - FixedDiv((y + h) << 16, MCH_SCALE_Y)
	);
}

int MchPagerX(int dir) {
	if(dir < 0)
		return MCH_ROW_X;
	return MCH_ROW_X + MCH_ROW_W - MCH_KEY_W;
}

// The old row's boxes this page shows as rows; the rest are navigation the chrome now offers itself.
int GetChromeRowCount(int page, int count) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_OVERVIEW)
		return 3;
	if(k == MCH_KIND_STATINDEX)
		return 5;
	if(k == MCH_KIND_STATS || k == MCH_KIND_PERKTREE || IsChromeShopList(page))
		return 0;
	return count - GetChromePagerBoxes(page);
}

// the dash toggle sits apart from the archetype rows above it
#define MCH_PERK_TOGGLEGAP 24

int GetChromeRowY(int page, int i) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_OVERVIEW)
		return 560 + MCH_ROW_STEP * i;
	if(k == MCH_KIND_PERKINDEX && i >= PERK_ARCH_COUNT)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_GEARDISCARD && i >= 8)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_RESINDEX && i >= 4)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_SKILLINDEX && i >= MAX_SKILL_TREES)
		return MCH_ROW_Y + MCH_ROW_STEP * i + MCH_PERK_TOGGLEGAP;
	if(k == MCH_KIND_CODEXINDEX && i >= MCH_CODEX_TOPICS)
		return MCH_CODEX_OPTY + MCH_ROW_STEP * (i - MCH_CODEX_TOPICS);
	if(page == MENU_HELP_MMODS)
		return MCH_CODEX_CATY + MCH_ROW_STEP * i;
	return MCH_ROW_Y + MCH_ROW_STEP * i;
}

int GetChromeRowX(int page, int i) {
	if((GetChromeKind(page) == MCH_KIND_CODEXINDEX && i >= MCH_CODEX_TOPICS) || page == MENU_HELP_MMODS)
		return MCH_PANE_IX;
	return MCH_ROW_X;
}

// Same box count and order as the old row of this page: the rows, then next, then previous, then the ones
// parked where the cursor cannot reach, so the numbering the server knows never shifts.
void LoadChromePane(menu_pane_T module& p, int page) {
	int count = p.cursize, rows = GetChromeRowCount(page, count), i;
	p.cursize = 0;
	// a research entry's two boxes are the pane's Research and Invest buttons
	if(GetChromeKind(page) == MCH_KIND_RESLIST) {
		MchAddBox(p, MCH_RES_BTNX, MCH_RES_BTNY, MCH_TAB_W, MCH_TAB_H);
		MchAddBox(p, MCH_RES_BTNX + MCH_RES_BTNSTEP, MCH_RES_BTNY, MCH_TAB_W, MCH_TAB_H);
		return;
	}
	if(GetChromeKind(page) == MCH_KIND_GEAREQUIP) {
		for(i = 0; i < INV_ICON_INDEX; ++i)
			MchAddBox(p, MchEqX(MchEqSlotX(i)) - MchEqWellW(i) / 2, MchEqY(MchEqSlotY(i)) - MchEqWellH(i) / 2, MchEqWellW(i), MchEqWellH(i));
		MchAddBox(p, MCH_EQ_BTNX, MCH_EQ_BTNY, MCH_TAB_W, MCH_TAB_H);
		rows = 0;
	}
	// the inventory page's boxes are its grid, which the view pane owns
	// the key socket, then Activate; the old back arrow stays parked, the frame's keys go back
	if(GetChromeKind(page) == MCH_KIND_DUNGEONS) {
		MchAddBox(p, MCH_DUN_X, MCH_DUN_Y, 120, 120);
		MchAddBox(p, MCH_DUN_X + 60 - MCH_TAB_W / 2, MCH_DUN_Y + 150, MCH_TAB_W, MCH_TAB_H);
		rows = 0;
	}
	// these pages' boxes are their grids, which the view pane owns, or rows built as they draw
	if(GetChromeKind(page) == MCH_KIND_GEARINV || GetChromeKind(page) == MCH_KIND_GEARSTASH || GetChromeKind(page) == MCH_KIND_GEARTRADE || GetChromeKind(page) == MCH_KIND_CRAFT)
		rows = 0;
	if(GetChromeKind(page) == MCH_KIND_GEARINDEX) {
		MchAddBox(p, -5000, -5000, 1, 1);
		for(i = 1; i < rows; ++i)
			MchAddBox(p, MCH_ROW_X, GetChromeRowY(page, i - 1), MCH_ROW_W, MCH_ROW_H);
		rows = 0;
	}
	for(i = 0; i < rows; ++i)
		MchAddBox(p, GetChromeRowX(page, i), GetChromeRowY(page, i), MCH_ROW_W, MCH_ROW_H);
	if(ChromePagerIsBoxes(page)) {
		if(GetChromePagerTarget(page, 1) != -1)
			MchAddBox(p, MchPagerX(1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H);
		if(GetChromePagerTarget(page, -1) != -1)
			MchAddBox(p, MchPagerX(-1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H);
	}
	for(i = p.cursize; i < count; ++i)
		MchAddBox(p, -5000, -5000, 1, 1);
}

// What the cursor is over among the chrome's own controls, as a mainboxid.
int GetTriggeredChromeBox(int page) {
	int cx = MchCursorX(), cy = MchCursorY(), i;
	if(cy >= MCH_TAB_Y && cy < MCH_TAB_Y + MCH_TAB_H) {
		for(i = 0; i < MCH_SEC_COUNT; ++i)
			if(MchIn(cx, cy, MCH_TAB_X + MCH_TAB_STEP * i, MCH_TAB_Y, MCH_TAB_W, MCH_TAB_H))
				return MCH_NAV_BASE + MCH_EL_TAB + i;
		for(i = 0; i < 3; ++i)
			if(MchIn(cx, cy, MCH_KEY_X + MCH_KEY_STEP * i, MCH_TAB_Y, MCH_KEY_W, MCH_TAB_H))
				return MAINBOX_LARR + i;
		if(MchIn(cx, cy, MCH_CLOSE_X, MCH_TAB_Y, MCH_CLOSE_W, MCH_TAB_H))
			return MCH_NAV_BASE + MCH_EL_CLOSE;
		return MAINBOX_NONE;
	}
	int n = GetChromeSubCount(GetChromeSection(page)), sw = GetChromeSubW(GetChromeSection(page));
	for(i = 0; i < n; ++i)
		if(MchIn(cx, cy, MCH_SUB_X + (sw + 10) * i, MCH_SUB_Y, sw, MCH_SUB_H))
			return MCH_NAV_BASE + MCH_EL_SUB + i;
	n = GetChromeChipCount(page);
	int cw = GetChromeChipW(page);
	for(i = 0; i < n; ++i)
		if(MchIn(cx, cy, MCH_CHIP_X + (cw + 10) * i, MCH_CHIP_Y, cw, MCH_CHIP_H))
			return MCH_NAV_BASE + MCH_EL_CHIP + i;
	if(GetChromeKind(page) == MCH_KIND_RESLIST) {
		i = MchResRowAt(page, cx, cy);
		if(i != -1)
			return MCH_NAV_BASE + MCH_EL_ROW + i;
	}
	if(GetChromeKind(page) == MCH_KIND_CODEX)
		for(i = 0; i < MCH_CODEX_TOPICS; ++i)
			if(MchIn(cx, cy, MCH_ROW_X, MCH_ROW_Y + MCH_ROW_STEP * i, MCH_ROW_W, MCH_ROW_H))
				return MCH_NAV_BASE + MCH_EL_ROW + i;
	if(!ChromePagerIsBoxes(page) && cy >= MCH_PAGER_Y && cy < MCH_PAGER_Y + MCH_TAB_H) {
		if(GetChromePagerTarget(page, -1) != -1 && MchIn(cx, cy, MchPagerX(-1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H))
			return MCH_NAV_BASE + MCH_EL_PAGER;
		if(GetChromePagerTarget(page, 1) != -1 && MchIn(cx, cy, MchPagerX(1), MCH_PAGER_Y, MCH_KEY_W, MCH_TAB_H))
			return MCH_NAV_BASE + MCH_EL_PAGER + 1;
	}
	return MAINBOX_NONE;
}

// ---------------------------------------------------------------- drawing primitives

void MchPic(str pic, int id, int x, int y) {
	SetFont(pic);
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED, MenuX((x << 16) + 0.1), MenuY((y << 16) + 0.1), 0.0);
}

// A picture made for a smaller canvas, drawn k times larger and centred on (x, y).
void MchPicScaled(str pic, int id, int x, int y, int k) {
	MenuHudSize(FixedDiv(MCH_W << 16, k) >> 16, FixedDiv(MCH_H << 16, k) >> 16);
	SetFont(pic);
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED, MenuX(FixedDiv(x << 16, k) & MMASK), MenuY(FixedDiv(y << 16, k) & MMASK), 0.0);
	MenuHudSize(MCH_W, MCH_H);
}

// GENERATED icon offsets: an ammo icon's sprite offsets (y when asked); HudMessage shifts the picture by them
int MchIconOffset(str pic, bool y) {
	if(!StrICmp(pic, "AHRLA0"))
		return y ? 28 : 8;
	if(!StrICmp(pic, "AMM3A0"))
		return y ? 14 : 8;
	if(!StrICmp(pic, "AXEAMM"))
		return y ? 14 : 9;
	if(!StrICmp(pic, "BASIA01"))
		return y ? 19 : 4;
	if(!StrICmp(pic, "CBWAA0"))
		return y ? 23 : 6;
	if(!StrICmp(pic, "CELLA0"))
		return y ? 12 : 8;
	if(!StrICmp(pic, "CHARAMMO"))
		return y ? 17 : 12;
	if(!StrICmp(pic, "CLIPA0"))
		return y ? 11 : 2;
	if(!StrICmp(pic, "C_H2A1"))
		return y ? 36 : 14;
	if(!StrICmp(pic, "D95AA0"))
		return y ? 16 : 7;
	if(!StrICmp(pic, "D97A1"))
		return y ? 20 : 6;
	if(!StrICmp(pic, "D97AA0"))
		return y ? 28 : 11;
	if(!StrICmp(pic, "D98AB1"))
		return y ? 30 : 17;
	if(!StrICmp(pic, "DBLUD1"))
		return y ? 25 : 5;
	if(!StrICmp(pic, "DEMSICO"))
		return y ? 12 : 8;
	if(!StrICmp(pic, "DES4Y0"))
		return y ? 22 : 19;
	if(!StrICmp(pic, "DEVIAM0"))
		return y ? 26 : 22;
	if(!StrICmp(pic, "EBOPA0"))
		return y ? 18 : 11;
	if(!StrICmp(pic, "EBOPE0"))
		return y ? 18 : 11;
	if(!StrICmp(pic, "EICEAM1"))
		return y ? 24 : 20;
	if(!StrICmp(pic, "EMEAA0"))
		return y ? 8 : 10;
	if(!StrICmp(pic, "ESHAA0"))
		return y ? 7 : 8;
	if(!StrICmp(pic, "ESHAB0"))
		return y ? 12 : 15;
	if(!StrICmp(pic, "FBATA0"))
		return y ? 11 : 11;
	if(!StrICmp(pic, "FSHLA0"))
		return y ? 27 : 19;
	if(!StrICmp(pic, "FUAMA0"))
		return y ? 32 : 12;
	if(!StrICmp(pic, "GAM1A0"))
		return y ? 16 : 15;
	if(!StrICmp(pic, "GAM2A0"))
		return y ? 25 : 19;
	if(!StrICmp(pic, "GAUSICO"))
		return y ? 21 : 15;
	if(!StrICmp(pic, "GBUNA0"))
		return y ? 11 : 9;
	if(!StrICmp(pic, "GRAVD0"))
		return y ? 23 : 12;
	if(!StrICmp(pic, "HEGRA0"))
		return y ? 24 : 14;
	if(!StrICmp(pic, "HMAWA1"))
		return y ? 21 : 4;
	if(!StrICmp(pic, "INCRAMMO"))
		return y ? 21 : 15;
	if(!StrICmp(pic, "IONAMM1"))
		return y ? 12 : 10;
	if(!StrICmp(pic, "LAAM1"))
		return y ? 23 : 13;
	if(!StrICmp(pic, "NPAKA0"))
		return y ? 16 : 3;
	if(!StrICmp(pic, "PCNIC1"))
		return y ? 17 : 17;
	if(!StrICmp(pic, "RBAMA0"))
		return y ? 6 : 4;
	if(!StrICmp(pic, "RIOTJ0"))
		return y ? 13 : 14;
	if(!StrICmp(pic, "ROCKA0"))
		return y ? 27 : 6;
	if(!StrICmp(pic, "SAM1A0"))
		return y ? 7 : 8;
	if(!StrICmp(pic, "SAM2A0"))
		return y ? 7 : 8;
	if(!StrICmp(pic, "SAM3A0"))
		return y ? 7 : 8;
	if(!StrICmp(pic, "SAM4A0"))
		return y ? 7 : 8;
	if(!StrICmp(pic, "SAM5A0"))
		return y ? 7 : 8;
	if(!StrICmp(pic, "SHELA0"))
		return y ? 7 : 5;
	if(!StrICmp(pic, "SLAYAM01"))
		return y ? 24 : 13;
	if(!StrICmp(pic, "STRGY0"))
		return y ? 12 : 19;
	return 0;
}
// GENERATED END

// a sprite-anchored icon centred on (x, y): its offsets are added back
void MchIconScaled(str pic, int id, int x, int y, int k) {
	MchPicScaled(pic, id, x + (FixedMul(MchIconOffset(pic, false) << 16, k) >> 16), y + (FixedMul(MchIconOffset(pic, true) << 16, k) >> 16), k);
}

// align: 0 centred, 1 left middle, 2 right middle, 3 left top
void MchText(str font, str txt, int id, int x, int y, int align) {
	int fx = x << 16, fy = y << 16;
	if(align == 1)
		fx += 0.1;
	else if(align == 2)
		fx += 0.2;
	else if(align == 3) {
		fx += 0.1;
		fy += 0.1;
	}
	SetFont(font);
	HudMessage(s:txt; HUDMSG_PLAIN, id, CR_WHITE, MenuX(fx), MenuY(fy), 0.0);
}

// ---------------------------------------------------------------- scroll bars

// A bar the mouse can drag: the draw records where it is, MchScrollDrag reads presses against that.
#define MCH_SBARS 4
#define MCH_SB_W 8
#define MCH_SB_THUMB 80
#define MCH_SB_PAD 12
#define MCH_SB_POCKET 3

typedef struct {
	int x;
	int y;
	int h;
	int max;
	int pos;
	int grab;
	bool grabbed;
	bool lit;
} mch_sbar_T;

mch_sbar_T module& GetMchSbar(int i) {
	static mch_sbar_T b[MCH_SBARS];
	return b[i];
}

// a page change: no bar is where it was
void MchSbarReset() {
	for(int i = 0; i < MCH_SBARS; ++i) {
		GetMchSbar(i).max = 0;
		GetMchSbar(i).grabbed = false;
		GetMchSbar(i).lit = false;
	}
}

// a chrome page's redraw: its bars register again as they draw
void MchSbarClear() {
	for(int i = 0; i < MCH_SB_POCKET; ++i)
		GetMchSbar(i).max = 0;
}

bool MchSbarGrabbed() {
	for(int i = 0; i < MCH_SBARS; ++i)
		if(GetMchSbar(i).grabbed)
			return true;
	return false;
}

int MchSbarThumbY(int slot) {
	mch_sbar_T module& b = GetMchSbar(slot);
	return b.y + (b.h - MCH_SB_THUMB) * b.pos / b.max;
}

bool MchOnSbar(int slot, int cx, int cy) {
	mch_sbar_T module& b = GetMchSbar(slot);
	return b.max > 0 && cx >= b.x - MCH_SB_PAD && cx < b.x + MCH_SB_W + MCH_SB_PAD && cy >= b.y && cy < b.y + b.h;
}

// pos of max on a track h tall at (x, y); id is the thumb, id + 1 the track
void MchScrollBar(int slot, int id, int x, int y, int h, int pos, int max) {
	mch_sbar_T module& b = GetMchSbar(slot);
	b.x = x;
	b.y = y;
	b.h = h;
	b.max = max;
	if(max <= 0) {
		b.grabbed = false;
		DeleteTextRange(id, id + 1);
		return;
	}
	b.pos = Clamp_Between(pos, 0, max);
	MenuClipRect(x, y, MCH_SB_W, h);
	MchPic("MCSBTRK", id + 1, x, y);
	MchPic(b.lit ? "MCSBTHH" : "MCSBTHM", id, x, MchSbarThumbY(slot));
	MenuClipRect(0, 0, 0, 0, 0);
}

// The bar's position after this tic's mouse: a press on the thumb drags it, a press on the track jumps there.
int MchScrollDrag(int slot, int pos) {
	mch_sbar_T module& b = GetMchSbar(slot);
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS), cy = MchCursorY();
	if(b.max <= 0 || CheckInventory("DnD_SellConfirm") || !IsButtonHeld(bpress, BT_ATTACK)) {
		b.grabbed = false;
		return pos;
	}
	if(!b.grabbed) {
		if(!IsButtonPressed(bpress, GetPlayerInput(-1, INPUT_OLDBUTTONS), BT_ATTACK) || !MchOnSbar(slot, MchCursorX(), cy))
			return pos;
		b.pos = Clamp_Between(pos, 0, b.max);
		int ty = MchSbarThumbY(slot);
		b.grab = (cy >= ty && cy < ty + MCH_SB_THUMB) ? cy - ty : MCH_SB_THUMB / 2;
		b.grabbed = true;
		LocalAmbientSound("RPG/MenuMove", 127);
	}
	int travel = b.h - MCH_SB_THUMB;
	return Clamp_Between(((cy - b.grab - b.y) * b.max + travel / 2) / travel, 0, b.max);
}

// lit while grabbed or under the cursor; true when any bar changed
bool MchSbarHover() {
	bool r = false;
	int cx = MchCursorX(), cy = MchCursorY();
	for(int i = 0; i < MCH_SBARS; ++i) {
		bool lit = GetMchSbar(i).max > 0 && (GetMchSbar(i).grabbed || MchOnSbar(i, cx, cy));
		if(lit != GetMchSbar(i).lit) {
			GetMchSbar(i).lit = lit;
			r = true;
		}
	}
	return r;
}

// a skill page's instructions, on the board's bottom edge
void DrawChromeSpellHint(str lump) {
	MenuHudSize(MCH_W, MCH_H);
	MchText("MENUSUB", StrParam(s:"\c[UIGrey]", l:lump), SPELLTREE_HEAD_ID + 1, 476, MCH_PANE_FOOT + 54, 0);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
	SetFont("NMENUFNT");
}

// the skill pages' info pocket, drawn from their 480x320 hud
void MchSpellPocketBar(int pos, int max) {
	MenuHudSize(MCH_W, MCH_H);
	MchScrollBar(MCH_SB_POCKET, SPELLTREE_BAR_ID, 1588, 307, 648, pos, max);
	DeleteTextRange(SPELLTREE_BAR_ID + 2, SPELLTREE_BAR_ID + 3);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// the pocket's drag onto the shared bar's position, which counts down from 0
bool MchSpellPocketDrag() {
	bool r = MchSbarHover();
	int p = -GetScrollBarPos(DND_SPELLBAR_PANEL), v = MchScrollDrag(MCH_SB_POCKET, p);
	if(v != p) {
		SetScrollBarPos(DND_SPELLBAR_PANEL, -v);
		r = true;
	}
	return r;
}

// A stack count short enough for a well corner: 999, 1.2k, 12k.
str MchStackText(int v) {
	if(v < 1000)
		return StrParam(d:v);
	if(v < 10000 && (v / 100) % 10)
		return StrParam(d:v / 1000, s:".", d:(v / 100) % 10, s:"k");
	return StrParam(d:v / 1000, s:"k");
}

str MchMoney(int v) {
	str out = "";
	bool neg = v < 0;
	if(neg)
		v = -v;
	do {
		int part = v % 1000;
		v /= 1000;
		if(v)
			out = StrParam(s:",", s:(part < 100) ? "0" : "", s:(part < 10) ? "0" : "", d:part, s:out);
		else
			out = StrParam(d:part, s:out);
	} while(v);
	return StrParam(s:neg ? "-$" : "$", s:out);
}

// ---------------------------------------------------------------- the frame, tabs and status strip

void DrawChromeFrame(int pnum, int page, int mainboxid) {
	int i, x, n, sec = GetChromeSection(page);
	str st, col;
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCFRAME", RPGMENUBACKGROUNDID, MCH_FX - 360, MCH_FY - 10);

	for(i = 0; i < MCH_SEC_COUNT; ++i) {
		x = MCH_TAB_X + MCH_TAB_STEP * i;
		st = "O";
		col = "\c[UIGrey]";
		if(i == sec) {
			st = "A";
			col = "\c[UIGold]";
		}
		else if(mainboxid == MCH_NAV_BASE + MCH_EL_TAB + i) {
			st = "H";
			col = "\c[UIWhite]";
		}
		MchPic(StrParam(s:"MCT", s:st, d:i % 4), MCH_ID_TABIMG + i, x, MCH_TAB_Y);
		MchText("MENUTAB", StrParam(s:col, l:GetChromeTabLabel(i)), MCH_ID_TABTXT + i, x + MCH_TAB_W / 2, MCH_TAB_Y + 26, 0);
	}

	// unspent points light up their tab
	for(i = 0; i < 2; ++i) {
		if(!i)
			n = CheckInventory("AttributePoint") + CheckInventory("PerkPoint");
		else
			n = CheckInventory("SpellPoint");
		x = MCH_TAB_X + MCH_TAB_STEP * (i ? MCH_SEC_SKILLS : MCH_SEC_CHARACTER) + MCH_TAB_W - 26;
		if(n) {
			MchPic("MCBADGE", MCH_ID_BADGEIMG + i, x, MCH_TAB_Y - 8);
			MchText("MENUSUB", StrParam(s:"\c[UIInk]", d:n), MCH_ID_TABTXT + 7 + i, x + 17, MCH_TAB_Y + 4, 0);
		}
		else {
			DeleteText(MCH_ID_BADGEIMG + i);
			DeleteText(MCH_ID_TABTXT + 7 + i);
		}
	}

	// history keys share the old buttons' ids, so the server and their click animation are unchanged
	for(i = 0; i < 3; ++i) {
		x = MCH_KEY_X + MCH_KEY_STEP * i;
		st = "O";
		if(CheckInventory(ButtonTimers[i]))
			st = "P";
		else if(mainboxid == MAINBOX_LARR + i)
			st = "H";
		MchPic(StrParam(s:"MCK", s:st, s:"0"), MCH_ID_TABIMG + 10 + i, x, MCH_TAB_Y);
		if(!i)
			col = "MCIBK";
		else if(i == 1)
			col = "MCIHM";
		else
			col = "MCIFW";
		MchPic(StrParam(s:col, d:mainboxid == MAINBOX_LARR + i), MCH_ID_TABIMG + 7 + i, x + 13, MCH_TAB_Y + 14);
	}
	bool hot = mainboxid == MCH_NAV_BASE + MCH_EL_CLOSE;
	MchPic(hot ? "MCWH0" : "MCWO0", MCH_ID_TABIMG + 13, MCH_CLOSE_X, MCH_TAB_Y);
	MchText("MENUSUB", StrParam(s:hot ? "\c[UIWhite]" : "\c[UIGrey]", l:"DND_MCH_CLOSE"), MCH_ID_TABTXT + 9, MCH_CLOSE_X + MCH_CLOSE_W / 2, MCH_TAB_Y + 26, 0);

	// sub tabs of this section
	DeleteTextRange(MCH_ID_SUBTXT, MCH_ID_SUBTXT + MCH_MAX_SUBS - 1);
	DeleteTextRange(MCH_ID_SUBIMG, MCH_ID_SUBIMG + MCH_MAX_SUBS - 1);
	n = GetChromeSubCount(sec);
	int act = GetChromeSubActive(page);
	int sw = GetChromeSubW(sec);
	for(i = 0; i < n; ++i) {
		x = MCH_SUB_X + (sw + 10) * i;
		st = "O";
		col = "\c[UIGrey]";
		if(i == act) {
			st = "A";
			col = "\c[UIGold]";
		}
		else if(mainboxid == MCH_NAV_BASE + MCH_EL_SUB + i) {
			st = "H";
			col = "\c[UIWhite]";
		}
		MchPic(StrParam(s:(sw == MCH_SUB_W) ? "MCS" : "MCN", s:st, d:(i + 1) % 4), MCH_ID_SUBIMG + i, x, MCH_SUB_Y);
		if(sec == MCH_SEC_GEAR && MchPageLocked(GetChromeSubTarget(sec, i)))
			MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_LOCKEDTAB"), MCH_ID_SUBTXT + i, x + sw / 2, MCH_SUB_Y + 16, 0);
		else
			MchText("MENUSUB", StrParam(s:col, l:GetChromeSubLabel(sec, i)), MCH_ID_SUBTXT + i, x + sw / 2, MCH_SUB_Y + 16, 0);
	}

	DrawChromeStatus(pnum, page);
}

void DrawChromeStatus(int pnum, int page) {
	int lvl = CheckInventory("Level");
	str extra = "";
	if(GetChromeSection(page) == MCH_SEC_SKILLS)
		extra = StrParam(s:"\c[UIGrey]", l:"DND_MCH_SKILLPTS", s:"  ", s:GetSpellPoints(pnum) ? "\c[UIGold]" : "\c[UIWhite]", d:GetSpellPoints(pnum), s:"      ");
	MchText("MENUTAB", StrParam(s:"\c[UIWhite]", l:"DND_MCH_LV", s:" ", d:lvl, s:"    \c[UIGreen]",
		l:GetClassLabel(StrParam(s:"CLASS", d:CheckInventory("DnD_Character") - 1), DND_CLASS_LABEL_NAME)), MCH_ID_STATTXT, MCH_FX + 54, MCH_STATUS_Y, 1);

	int pct = 0;
	if(lvl > 0 && lvl <= MAXLEVELS)
		pct = Clamp_Between(GetPlayerExp(pnum) / Max(LevelCurve[lvl - 1] / 100, 1), 0, 100);
	MchPic("MCXPBG", MCH_ID_PAGEIMG + 7, MCH_FX + 330, MCH_STATUS_Y - 8);
	if(pct) {
		MenuClipRect(MCH_FX + 333, MCH_STATUS_Y - 4, 414 * pct / 100, 8);
		MchPic("MCXPFL", MCH_ID_PAGEIMG + 6, MCH_FX + 333, MCH_STATUS_Y - 4);
		MenuClipRect(0, 0, 0, 0, 0);
	}
	else
		DeleteText(MCH_ID_PAGEIMG + 6);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", d:pct, s:"%"), MCH_ID_STATTXT + 1, MCH_FX + 766, MCH_STATUS_Y, 1);

	MchText("MENUTAB", StrParam(s:extra, s:"\c[UIGrey]", l:"DND_MENU_CREDITS", s:"  \c[UIGold]", s:MchMoney(GetPlayerCredit(pnum)),
		s:"      \c[UIGrey]", l:"DND_MENU_BUDGET", s:"  \c[UIGold]$", d:CheckInventory("Budget"), s:"K"), MCH_ID_STATTXT + 2, MCH_FX + MCH_FW - 36, MCH_STATUS_Y, 2);
}

// ---------------------------------------------------------------- shop rows

typedef struct {
	bool known;
	bool owned;
	int price;
	int stock;
	str col;
	str pcol;
} mch_shoprow_T;

mch_shoprow_T module& GetMchRow() {
	static mch_shoprow_T r;
	return r;
}

// The same states and precedence as DrawToggledImage, as colours of the new palette.
void MchFillShopRow(int pnum, int itemid, int flags, str choicename, int choicecount) {
	mch_shoprow_T module& r = GetMchRow();
	r.known = !(flags & OBJ_RESEARCH) || CheckItemRequirements(itemid, RES_KNOWN, flags);
	r.owned = false;
	r.price = 0;
	r.stock = GlobalData.ShopStockRemaining[pnum][itemid];
	r.col = "\c[UIDim]";
	r.pcol = "\c[UIDim]";
	if(!r.known)
		return;

	bool done = CheckItemRequirements(itemid, RES_DONE, flags);
	if(!(flags & OBJ_AMMO))
		r.price = GetShopPrice(itemid);
	else
		r.price = GetShopPrice(itemid, PRICE_INCREASE_STOCK_LOW);
	bool cant = r.price > GetPlayerCredit(pnum) || r.stock <= 0;

	r.col = "\c[UIWhite]";
	r.pcol = "\c[UIGold]";
	if(flags & OBJ_RESEARCH && !done) {
		r.col = "\c[UISlate]";
		r.pcol = "\c[UISlate]";
	}
	else if(!(flags & (OBJ_AMMO | OBJ_ARMOR))) {
		if(!(flags & OBJ_ACCOUNT) && CheckInventory(GetItemName(itemid))) {
			r.owned = true;
			r.col = "\c[UIGreen]";
			r.pcol = "\c[UIGreen]";
		}
		else if(cant) {
			r.col = "\c[UIGrey]";
			r.pcol = "\c[UIGrey]";
		}
		else if(!(HasClassPerk_Fast(DND_PLAYER_BERSERKER, 1) && itemid <= SHOP_WEAPON_SLOT1END) && CheckInventory(choicename) == choicecount) {
			r.col = "\c[UIRed]";
			r.pcol = "\c[UIRed]";
		}
	}
	else if(cant) {
		r.col = "\c[UIGrey]";
		r.pcol = "\c[UIGrey]";
	}
}

typedef struct {
	int slot;
	int first;
	int last;
	bool spec;
} mch_ammopage_T;

mch_ammopage_T module& GetMchAmmoPage(int page) {
	static mch_ammopage_T a;
	a.spec = false;
	switch(page) {
		case MENU_SHOP_AMMO_1:
		case MENU_SHOP_AMMO_1_2:
			a.slot = DND_AMMOSLOT_CLIP;
			a.first = AMMOSLOT_CLIP_BEGIN;
			a.last = AMMOSLOT_CLIP2_END;
		break;
		case MENU_SHOP_AMMO_2:
		case MENU_SHOP_AMMO_2_2:
			a.slot = DND_AMMOSLOT_SHELL;
			a.first = AMMOSLOT_SHELL_BEGIN;
			a.last = AMMOSLOT_SHELL2_END;
		break;
		case MENU_SHOP_AMMO_3:
			a.slot = DND_AMMOSLOT_ROCKET;
			a.first = AMMOSLOT_ROCKET_BEGIN;
			a.last = AMMOSLOT_ROCKET_END;
		break;
		case MENU_SHOP_AMMO_4_1:
		case MENU_SHOP_AMMO_4_2:
			a.slot = DND_AMMOSLOT_CELL;
			a.first = AMMOSLOT_CELL_BEGIN;
			a.last = AMMOSLOT_CELL2_END;
		break;
		default:
			a.slot = -1;
			a.first = 0;
			a.last = MAX_SPECIAL_AMMOS_FOR_SHOP - 1;
			a.spec = true;
		break;
	}
	return a;
}

// ---------------------------------------------------------------- the shop's slot lists

// A weapon or ammo slot is one list on its first page, scrolled rather than paged.
#define MCH_LIST_ROWS 10

bool IsChromeShopList(int page) {
	int k = GetChromeKind(page);
	return k == MCH_KIND_WEAPONS || k == MCH_KIND_AMMO;
}

int MchWeaponSlotFirst(int page) {
	if(page >= MENU_SHOP_WEAPON3_1 && page <= MENU_SHOP_WEAPON3_3)
		return SHOP_WEAPON31_BEGIN;
	if(page == MENU_SHOP_WEAPON4_1 || page == MENU_SHOP_WEAPON4_2)
		return SHOP_WEAPON41_BEGIN;
	if(page == MENU_SHOP_WEAPON5_1 || page == MENU_SHOP_WEAPON5_2)
		return SHOP_WEAPON51_BEGIN;
	if(page == MENU_SHOP_WEAPON6_1 || page == MENU_SHOP_WEAPON6_2)
		return SHOP_WEAPON61_BEGIN;
	return GetWeaponBeginIndexFromOption(page);
}

int MchWeaponSlotLast(int page) {
	if(page >= MENU_SHOP_WEAPON3_1 && page <= MENU_SHOP_WEAPON3_3)
		return SHOP_WEAPON_SLOT3END;
	if(page == MENU_SHOP_WEAPON4_1 || page == MENU_SHOP_WEAPON4_2)
		return SHOP_WEAPON_SLOT42END;
	if(page == MENU_SHOP_WEAPON5_1 || page == MENU_SHOP_WEAPON5_2)
		return SHOP_WEAPON_SLOT52END;
	if(page == MENU_SHOP_WEAPON6_1 || page == MENU_SHOP_WEAPON6_2)
		return SHOP_WEAPON_SLOT62END;
	return GetWeaponEndIndexFromOption(page);
}

int MchShopListCount(int page) {
	if(GetChromeKind(page) == MCH_KIND_WEAPONS)
		return MchWeaponSlotLast(page) - MchWeaponSlotFirst(page) + 1;
	mch_ammopage_T module& a = GetMchAmmoPage(page);
	return a.last - a.first + 1;
}

// the row under the cursor as the box the server knows: the list index + 1
int MchShopListHit(int page) {
	mch_state_T module& s = GetMchState();
	int cx = MchCursorX(), cy = MchCursorY() - MCH_ROW_Y;
	if(s.page != page || cx < MCH_ROW_X || cx >= MCH_ROW_X + MCH_ROW_W || cy < 0 || cy % MCH_ROW_STEP >= MCH_ROW_H)
		return MAINBOX_NONE;
	int i = cy / MCH_ROW_STEP;
	if(i >= MCH_LIST_ROWS || s.scroll + i >= MchShopListCount(page))
		return MAINBOX_NONE;
	return s.scroll + i + 1;
}

// The shop item of row r on a page, -1 for rows that are navigation.
int GetChromeRowItem(int page, int r) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_WEAPONS) {
		if(MchWeaponSlotFirst(page) + r <= MchWeaponSlotLast(page))
			return MchWeaponSlotFirst(page) + r;
	}
	else if(k == MCH_KIND_AMMO) {
		mch_ammopage_T module& a = GetMchAmmoPage(page);
		if(a.spec)
			return SHOP_FIRSTAMMOSPECIAL_INDEX + r;
		return GetMenuAmmoIndexMap(a.slot, a.first + r);
	}
	else if(k == MCH_KIND_ACCOUNT)
		return SHOP_ACCOUNT_BEGIN + r;
	return -1;
}

int GetChromeRowFlags(int page, int item, int r) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_WEAPONS)
		return GetWeaponDrawInfo(item - SHOP_WEAPON_BEGIN).flags;
	if(k == MCH_KIND_AMMO) {
		if(GetMchAmmoPage(page).spec)
			return GetAmmoDrawInfo(r + MAXSHOPNORMALAMMOS).flags;
		return GetAmmoDrawInfo(item - SHOP_FIRSTAMMO_INDEX).flags;
	}
	return OBJ_RESEARCH | OBJ_ACCOUNT;
}

void MchFillRowFor(int pnum, int page, int item, int r) {
	int k = GetChromeKind(page), flags = GetChromeRowFlags(page, item, r);
	if(k == MCH_KIND_WEAPONS)
		MchFillShopRow(pnum, item, flags, GetWeaponCondition(item), 1);
	else if(k == MCH_KIND_AMMO)
		MchFillShopRow(pnum, item, flags, "", 1);
	else
		MchFillShopRow(pnum, item, flags, GetItemName(item), GetShopItemInfo(item, DND_SHOPINFO_MAX));
}

str GetChromeRowName(int page, int item, int r) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_WEAPONS)
		return StrParam(l:GetWeaponTag(ShopTableIdToWeaponTableId(item)));
	if(k == MCH_KIND_AMMO)
		return StrParam(l:GetAmmoTag(item));
	if(k == MCH_KIND_ACCOUNT)
		return StrParam(l:GetAccountPurchaseTag(item - SHOP_ACCOUNT_BEGIN));
	if(k == MCH_KIND_SHOPINDEX) {
		if(!r)
			return StrParam(l:"DND_MCH_SUB_WEAPONS");
		if(r == 1)
			return StrParam(l:"DND_MCH_SUB_AMMO");
		if(r == 2)
			return StrParam(l:"DND_MCH_SUB_ACCOUNT");
		return StrParam(l:"DND_MENU_NFT");
	}
	if(k == MCH_KIND_SLOTLIST)
		return StrParam(l:"DND_MENU_SLOT", s:" ", d:r + 1);
	if(r < MAX_SLOTS - 1)
		return StrParam(l:"DND_MENU_AMMOCATEGORY", s:" ", d:r + 1);
	return StrParam(l:"DND_MENU_SPECIALAMMO");
}

str GetChromeTitle(int page) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_SHOPINDEX)
		return "DND_MENU_HEAD_SHOP";
	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		return "DND_MENU_HEAD_WEAPONS";
	if(k == MCH_KIND_ACCOUNT)
		return "DND_MENU_HEAD_ACCOUNT";
	if(k == MCH_KIND_OVERVIEW)
		return "DND_MENU_HEAD_STATS";
	if(k == MCH_KIND_PERKINDEX)
		return "DND_MENU_HEAD_PERKS";
	if(k == MCH_KIND_PERKTREE)
		return GetPerkArchLangKey(page - MENU_PERKTREE_FIRST);
	if(k == MCH_KIND_RESINDEX)
		return "DND_MENU_RESEARCH";
	if(k == MCH_KIND_RESGUNS)
		return "DND_MCH_SUB_WEAPONS";
	if(k == MCH_KIND_RESLIST)
		return GetChromeSubLabel(MCH_SEC_RESEARCH, GetChromeSubActive(page));
	if(k == MCH_KIND_CODEXINDEX)
		return "DND_MCH_TAB_CODEX";
	if(k == MCH_KIND_SKILLINDEX)
		return "DND_MCH_TAB_SKILLS";
	if(k == MCH_KIND_GEARINDEX)
		return "DND_MCH_TAB_GEAR";
	if(k == MCH_KIND_GEARDISCARD)
		return "DND_MENU_WEAPONDISCARD";
	if(k == MCH_KIND_GEARACC)
		return "DND_MENU_ACCESSORIES";
	if(k == MCH_KIND_GEAREQUIP)
		return "DND_MENU_EQUIPMENTS";
	if(k == MCH_KIND_GEARINV)
		return "DND_MENU_INVENTORY";
	if(k == MCH_KIND_GEARSTASH)
		return "DND_MENU_STASH";
	if(k == MCH_KIND_GEARTRADE)
		return "DND_MENU_TRADING";
	if(k == MCH_KIND_DUNGEONS)
		return "DND_MENU_DUNGEONS";
	if(k == MCH_KIND_HOME)
		return "DND_MCH_HOME_TITLE";
	if(k == MCH_KIND_CRAFT)
		return "DND_MENU_CRAFTING";
	if(k == MCH_KIND_CODEX)
		return StrParam(s:"DND_MCH_CODEX", d:MchCodexTopic(page) + 1);
	if(k == MCH_KIND_STATINDEX)
		return "DND_MENU_HEAD_GAINS";
	if(k == MCH_KIND_STATS) {
		if(page == MENU_STAT2_OFFENSE1 || page == MENU_STAT2_OFFENSE2)
			return "DND_MENU_GAIN_OFFENSE";
		return StrParam(s:"DND_MENU_GAIN_", s:(page == MENU_STAT2_DEFENSE) ? "DEFENSE" : (page == MENU_STAT2_UTILITY) ? "UTILITY" : (page == MENU_STAT2_EXOTIC) ? "EXOTIC" : "MISC");
	}
	return "DND_MENU_HEAD_AMMO";
}

typedef struct {
	int page;
	int row;
	int scroll;
	int scroll_max;
	int scroll_tic;
	int pscroll;
	int pscroll_max;
	int lastpos;
	bool popanswered;
} mch_state_T;

mch_state_T module& GetMchState() {
	static mch_state_T s;
	return s;
}

// ---------------------------------------------------------------- the page

void DrawChromePage(int pnum, int page, int boxid, int mainboxid, menu_pane_T module& p) {
	int i, x, item, k = GetChromeKind(page), count = p.cursize;
	str st;
	MenuHudSize(MCH_W, MCH_H);
	MchSbarClear();
	// the page's own bands only: the frame's labels and the status strip sit between them
	DeleteTextRange(MCH_ID_ROWTXT, MCH_ID_TABTXT - 1);
	DeleteTextRange(MCH_ID_PANEIMG, MCH_ID_PAGEIMG + 5);

	MchText("MENUHEAD", StrParam(s:"\c[UICyan]", l:GetChromeTitle(page)), MCH_ID_PAGETXT, MCH_ROW_X, 196, 3);

	int n = GetChromeChipCount(page), act = GetChromeChipActive(page), cw = GetChromeChipW(page);
	for(i = 0; i < n; ++i) {
		x = MCH_CHIP_X + (cw + 10) * i;
		st = "N";
		if(i == act)
			st = "S";
		else if(mainboxid == MCH_NAV_BASE + MCH_EL_CHIP + i)
			st = "H";
		if(ChromeChipsAreWords(page)) {
			MchPic(StrParam(s:"MCCW", s:st), MCH_ID_CHIPIMG + i, x - 6, MCH_CHIP_Y - 6);
			if(MchPageLocked(GetChromeChipTarget(page, i)))
				MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_LOCKEDTAB"), MCH_ID_CHIPTXT + i, x + cw / 2, MCH_CHIP_Y + 20, 0);
			else
				MchText("MENUSUB", StrParam(s:(i == act) ? "\c[UIGold]" : (GetChromeChipTarget(page, i) == -1) ? "\c[UIDim]" : "\c[UIGrey]", l:GetChromeWordChipLabel(page, i)),
					MCH_ID_CHIPTXT + i, x + cw / 2, MCH_CHIP_Y + 20, 0);
			continue;
		}
		MchPic(StrParam(s:"MCCHIP", s:st), MCH_ID_CHIPIMG + i, x - 6, MCH_CHIP_Y - 6);
		if(k == MCH_KIND_AMMO || k == MCH_KIND_AMMOSELECT) {
			if(i == 4)
				MchText("MENUSUB", StrParam(s:(i == act) ? "\c[UIGold]" : "\c[UIGrey]", l:"DND_MCH_SPECIALCHIP"), MCH_ID_CHIPTXT + i, x + MCH_CHIP_W / 2, MCH_CHIP_Y + 20, 0);
			else
				MchText("MENUTAB", StrParam(s:(i == act) ? "\c[UIGold]" : "\c[UIGrey]", d:i + 1), MCH_ID_CHIPTXT + i, x + MCH_CHIP_W / 2, MCH_CHIP_Y + 20, 0);
		}
		else
			MchText("MENUTAB", StrParam(s:(i == act) ? "\c[UIGold]" : "\c[UIGrey]", d:i + 1), MCH_ID_CHIPTXT + i, x + MCH_CHIP_W / 2, MCH_CHIP_Y + 20, 0);
	}

	if(k == MCH_KIND_SLOTLIST || k == MCH_KIND_WEAPONS)
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]R  \c[UIGrey]", l:"DND_MENU_REPLACESWEP", s:"      \c[UIDim]L  \c[UIGrey]", l:"DND_MENU_LUXURYWEP"),
			MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	MchPic("MCDIV", MCH_ID_PAGEIMG, MCH_ROW_X, 258);
	if(k == MCH_KIND_SKILLINDEX) {
		DrawChromeSkillIndex(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARINDEX) {
		DrawChromeGearIndex(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARDISCARD) {
		DrawChromeGearDiscard(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARACC) {
		DrawChromeGearAcc(pnum, page, boxid, mainboxid, count);
		return;
	}
	if(k == MCH_KIND_GEAREQUIP) {
		DrawChromeGearEquip(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_GEARINV) {
		DrawChromeGearInv(pnum);
		return;
	}
	if(k == MCH_KIND_GEARTRADE) {
		DrawChromeGearTrade(pnum, page, boxid, mainboxid, p);
		return;
	}
	if(k == MCH_KIND_DUNGEONS) {
		DrawChromeDungeons(pnum, boxid);
		return;
	}
	if(k == MCH_KIND_HOME) {
		DrawChromeHome();
		return;
	}
	if(k == MCH_KIND_CRAFT) {
		DrawChromeCraft(pnum, page, boxid);
		return;
	}
	if(k == MCH_KIND_GEARSTASH) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_INV"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
		return;
	}
	if(k >= MCH_KIND_CODEXINDEX) {
		DrawChromeCodex(pnum, page, boxid, mainboxid);
		return;
	}
	if(k >= MCH_KIND_RESINDEX) {
		DrawChromeResearch(pnum, page, boxid, mainboxid);
		return;
	}
	if(k >= MCH_KIND_PERKINDEX) {
		DrawChromePerks(pnum, page, boxid, p);
		return;
	}
	if(k >= MCH_KIND_OVERVIEW) {
		DrawChromeCharacter(pnum, page, boxid, mainboxid);
		return;
	}

	// the rows, and which one the pane describes: the hovered one, else the last one hovered
	int rows = count - GetChromePagerBoxes(page), first = 0, total = rows;
	bool list = IsChromeShopList(page);
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
	}
	if(list) {
		total = MchShopListCount(page);
		s.scroll_max = Max(total - MCH_LIST_ROWS, 0);
		s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);
		first = s.scroll;
		rows = Min(total - first, MCH_LIST_ROWS);
	}
	if(boxid != MAINBOX_NONE && boxid <= total)
		s.row = boxid - 1;

	for(int ri = 0; ri < rows; ++ri) {
		int y = MCH_ROW_Y + MCH_ROW_STEP * ri;
		i = first + ri;
		item = GetChromeRowItem(page, i);
		str name, price = "", col = "\c[UIWhite]", pcol = "\c[UIGold]";
		bool locked = false, tagged = false, lux = false;
		if(item != -1) {
			MchFillRowFor(pnum, page, item, i);
			mch_shoprow_T module& r = GetMchRow();
			if(!r.known) {
				locked = true;
				name = StrParam(l:"DND_MCH_NA");
				price = "$ ???";
			}
			else {
				name = GetChromeRowName(page, item, i);
				price = MchMoney(r.price);
			}
			col = r.col;
			pcol = r.pcol;
			if(k == MCH_KIND_WEAPONS && r.known) {
				tagged = true;
				lux = IsLuxuryWeapon(ShopTableIdToWeaponTableId(item));
			}
		}
		else
			name = GetChromeRowName(page, -1, i);

		if(boxid == i + 1)
			st = "S";
		else if(locked)
			st = "L";
		else
			st = "N";
		MchPic(StrParam(s:"MCROW", s:st), MCH_ID_ROWIMG + 12 + ri, MCH_ROW_X - 8, y - 8);
		x = MCH_ROW_X + 30;
		if(tagged) {
			MchPic("MCTAGBOX", MCH_ID_ROWIMG + ri, MCH_ROW_X + 24, y + 13);
			MchText("MENUSUB", lux ? "\c[UICyan]L" : "\c[UIWhite]R", MCH_ID_ROWTXT + 3 * ri + 2, MCH_ROW_X + 38, y + 27, 0);
			x = MCH_ROW_X + 68;
		}
		MchText("MENUBODY", StrParam(s:col, s:name), MCH_ID_ROWTXT + 3 * ri, x, y + 27, 1);
		if(StrLen(price))
			MchText("MENUBODY", StrParam(s:pcol, s:price), MCH_ID_ROWTXT + 3 * ri + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}

	if(list) {
		MchScrollBar(0, MCH_ID_PANEIMG + 26, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y, MCH_ROW_STEP * MCH_LIST_ROWS - 6, s.scroll, s.scroll_max);
		if(s.scroll_max)
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_SHOWING", s:" ", d:first + 1, s:"-", d:first + rows, s:" / ", d:total, s:"    \c[UIDim]", l:"DND_MCH_SCROLLHINT"),
				MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
	}
	else
		DrawChromePager(page, boxid, mainboxid, rows);
	DrawChromePane(pnum, page, Clamp_Between(s.row, 0, Max(total - 1, 0)));
}

void DrawChromePager(int page, int boxid, int mainboxid, int rows) {
	int pages = GetChromePageCount(page);
	if(pages < 2)
		return;
	for(int d = -1; d <= 1; d += 2) {
		if(GetChromePagerTarget(page, d) == -1)
			continue;
		bool hot;
		if(ChromePagerIsBoxes(page))
			hot = boxid == rows + 1 + (d < 0 && GetChromePagerTarget(page, 1) != -1);
		else
			hot = mainboxid == MCH_NAV_BASE + MCH_EL_PAGER + (d > 0);
		int x = MchPagerX(d);
		MchPic(StrParam(s:"MCK", s:hot ? "H" : "O", s:"0"), MCH_ID_PAGEIMG + 3 + (d > 0), x, MCH_PAGER_Y);
		MchPic(StrParam(s:(d < 0) ? "MCIBK" : "MCIFW", d:hot), MCH_ID_PAGEIMG + 1 + (d > 0), x + 13, MCH_PAGER_Y + 14);
	}
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_PAGE", s:" ", d:GetChromePageNumber(page), s:" / ", d:pages),
		MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
}

// ---------------------------------------------------------------- the detail pane

void DrawChromePane(int pnum, int page, int row) {
	int k = GetChromeKind(page), item = GetChromeRowItem(page, row), y;
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);

	if(item == -1) {
		str head = GetChromeRowName(page, -1, row), body = "";
		if(k == MCH_KIND_SHOPINDEX && row < 3)
			body = StrParam(l:StrParam(s:"DND_MCH_SHOPDESC", d:row + 1));
		else if(k == MCH_KIND_SLOTLIST)
			body = StrParam(l:"DND_MCH_PICKSLOT", s:"\n\n\c[UIGrey]R  ", l:"DND_MENU_REPLACESWEP", s:".\n\c[UICyan]L  \c[UIGrey]", l:"DND_MENU_LUXURYWEP", s:". ", l:"DND_MENU_CARRYLIMIT", s:".");
		else if(k == MCH_KIND_AMMOSELECT)
			body = StrParam(l:"DND_MCH_PICKCAT");
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", s:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 500, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", s:body), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}

	MchFillRowFor(pnum, page, item, row);
	mch_shoprow_T module& r = GetMchRow();
	MchPic("MCWELL", MCH_ID_PANEIMG + 29, MCH_PANE_IX, MCH_PANE_Y + 24);
	if(!r.known) {
		MchText("MENUHEAD", StrParam(s:"\c[UIDim]", l:"DND_MCH_NA"), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
		MchText("MENUHEAD", "\c[UIDim]$ ???", MCH_ID_PANETXT + 5, MCH_PANE_IX, MCH_PANE_FOOT, 3);
		return;
	}

	MchText("MENUHEAD", StrParam(s:r.col, s:GetChromeRowName(page, item, row)), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
	int desc_y = MCH_PANE_Y + 212;
	str desc = "", hint = "DND_MCH_HINT_ACCOUNT";
	if(k == MCH_KIND_WEAPONS) {
		int real = ShopTableIdToWeaponTableId(item);
		draw_info_T module& info = GetWeaponDrawInfo(item);
		str icon = GetTextWithResearch(GetWeaponShopIcon(item), "", info.res_id, RES_KNOWN, info.flags);
		if(StrLen(icon))
			MchPicScaled(icon, MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 3.0);
		if(IsLuxuryWeapon(real))
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_SLOT", s:" ", d:GetChromeChipActive(page) + 1, s:"  -  \c[UICyan]", l:"DND_MENU_LUXURYWEP",
				s:"\c[UIGrey]. ", l:"DND_MENU_CARRYLIMIT"), MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
		else
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_SLOT", s:" ", d:GetChromeChipActive(page) + 1, s:"  -  ", l:"DND_MENU_REPLACESWEP"),
				MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);

		int i, j = 0;
		if(CheckItemRequirements(item, RES_KNOWN, info.flags)) {
			int dt = GetShopWeaponDamageTypes(item);
			for(i = 0; i < MAX_DAMAGE_TYPES_MENU; ++i)
				if(IsSet(dt, i)) {
					MchPicScaled(GetDamageTypeIcon(i), MCH_ID_PANEIMG + 1 + j, MCH_PANE_TX + 32 + 70 * j, MCH_PANE_Y + 146, 2.0);
					++j;
				}
			j = 0;
			for(i = 0; i < MAX_WEAPON_PROPERTIES; ++i)
				if(IsSet(Weapons_Data[real].properties, i)) {
					MchPicScaled(WeaponPropertyImages[i], MCH_ID_PANEIMG + 9 + j, MCH_PANE_IX + 52 + 108 * (j % 8), MCH_PANE_Y + 254 + 84 * (j / 8), 1.5);
					++j;
				}
		}
		desc_y = MCH_PANE_Y + 216 + 84 * ((j + 7) / 8);
		desc = GetWeaponExplanation(item);
		hint = "DND_MCH_HINT_WEAPON";
	}
	else if(k == MCH_KIND_AMMO) {
		mch_ammopage_T module& a = GetMchAmmoPage(page);
		str aicon;
		if(a.spec) {
			int sidx = row;
			if(sidx >= SSAM_EXSHELL)
				++sidx;
			aicon = GetTextWithResearch(GetSpecialAmmoStr(sidx, AMMOINFO_ICON), "", GetAmmoDrawInfo(MAXSHOPNORMALAMMOS + row).res_id, RES_KNOWN, GetAmmoDrawInfo(MAXSHOPNORMALAMMOS + row).flags);
		}
		else
			aicon = GetTextWithResearch(GetAmmoInfo(a.slot, a.first + row).icon, "", GetAmmoDrawInfo(item - SHOP_FIRSTAMMO_INDEX).res_id, RES_KNOWN, GetAmmoDrawInfo(item - SHOP_FIRSTAMMO_INDEX).flags);
		if(StrLen(aicon))
			MchIconScaled(aicon, MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 3.0);
		desc = GetChromeAmmoText(item);
		desc_y = MCH_PANE_Y + 212;
		int bulk = GetBulkPriceForAmmo(item);
		if(bulk)
			MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_INBULK", s:": \c[UIGold]", s:MchMoney(bulk)), MCH_ID_PANETXT + 6, MCH_PANE_IX + 560, MCH_PANE_FOOT + 18, 1);
		hint = "DND_MCH_HINT_AMMO";
	}
	else
		desc = StrParam(l:GetAccountPurchaseText(item - SHOP_ACCOUNT_BEGIN));

	MenuClipRect(MCH_PANE_IX, desc_y, MCH_PANE_TEXTW, MCH_PANE_FOOT - 12 - desc_y, MCH_PANE_TEXTW);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", s:desc), MCH_ID_PANETXT + 2, MCH_PANE_IX, desc_y, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	MchText("MENUHEAD", StrParam(s:r.pcol, s:MchMoney(r.price)), MCH_ID_PANETXT + 5, MCH_PANE_IX, MCH_PANE_FOOT, 3);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_STOCK", s:": \c[UIWhite]", d:r.stock), MCH_ID_PANETXT + 3, MCH_PANE_IX + 300, MCH_PANE_FOOT + 18, 1);
	if(r.owned)
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_SELLSFOR", s:": \c[UIGreen]", s:MchMoney(GetShopPrice(item) / 2)), MCH_ID_PANETXT + 4, MCH_PANE_IX + 560, MCH_PANE_FOOT + 18, 1);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PANETXT + 7, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// DrawAmmoExplanation's text, without its drawing.
str GetChromeAmmoText(int itemid) {
	int ammo_id = itemid - SHOP_FIRSTAMMO_INDEX;
	int ammo_ref = GetMenuAmmoCount(ammo_id, AMMOID_WEPREF);
	str base = StrParam(l:"DND_MENU_GIVES", s:" \c[UIGold]", d:GetAmmoToGive(itemid), s:"\c[UIWhite] ", l:StrParam(s:"DND_MENU_AMMOTEXT", d:ammo_id + 1));
	if(ammo_ref < 65536) {
		if(itemid <= SHOP_AMMO_CELL || itemid >= SHOP_AMMO_FLECHETTE)
			return base;
		return StrParam(s:base, s:" ", l:"DND_MENU_FOR", s:"\c[UICyan]", l:GetWeaponTag(ammo_ref), s:"\c[UIWhite].");
	}
	return StrParam(s:base, s:" ", l:"DND_MENU_FOR", s:"\c[UICyan]", l:GetWeaponTag(ammo_ref & 0xFFFF), s:"\c[UIWhite]", l:"DND_MENU_AND",
		s:"\c[UICyan]", l:GetWeaponTag(ammo_ref >> 16), s:"\c[UIWhite].");
}

// ---------------------------------------------------------------- the character pages

#define MCH_STAT_LINEH 27
#define MCH_STAT_TOP (MCH_PANE_Y + 22)
#define MCH_STAT_VIEW 600

// lines a text takes when wrapped at cols characters, colour codes not counted
int MchCountLines(str t, int cols) {
	int lines = 0, col = 0, n = StrLen(t);
	for(int i = 0; i < n; ++i) {
		int c = GetChar(t, i);
		if(c == 28) {
			if(i + 1 < n && GetChar(t, i + 1) == '[') {
				while(i < n && GetChar(t, i) != ']')
					++i;
			}
			else
				++i;
			continue;
		}
		if(c == '\n') {
			++lines;
			col = 0;
			continue;
		}
		if(++col >= cols) {
			++lines;
			col = 0;
		}
	}
	return lines + (col > 0);
}

int GetChromeStatCategory(int page) {
	switch(page) {
		case MENU_STAT2_OFFENSE1:
		return DRAW_STAT_OFFENSE1;
		case MENU_STAT2_OFFENSE2:
		return DRAW_STAT_OFFENSE2;
		case MENU_STAT2_DEFENSE:
		return DRAW_STAT_DEFENSE;
		case MENU_STAT2_UTILITY:
		return DRAW_STAT_UTILITY;
		case MENU_STAT2_EXOTIC:
		return DRAW_STAT_EXOTIC;
	}
	return DRAW_STAT_MISC;
}

str GetChromeStatName(int i) {
	if(!i)
		return "DND_MENU_GAIN_OFFENSE";
	if(i == 1)
		return "DND_MENU_GAIN_DEFENSE";
	if(i == 2)
		return "DND_MENU_GAIN_UTILITY";
	if(i == 3)
		return "DND_MENU_GAIN_EXOTIC";
	return "DND_MENU_GAIN_MISC";
}

// map difficulty, kills and lives, as the old gains pages showed them
str GetChromeRunSummary() {
	str out = StrParam(s:"\c[UIGrey]", l:"DND_MENU_MAPDIFF", s:": \c[UIWhite]", l:MapDifficultyLabel(MapData[DND_MAPDATA_DIFFICULTY]),
		s:"\n\c[UIGrey]", l:"DND_MENU_TOTALKILLS", s:": \c[UIWhite]", s:GetPlayerLifetimeKills());
	if(GetCVar("survival"))
		out = StrParam(s:out, s:"\n\c[UIGrey]", l:"DND_MENU_LIVESLEFT", s:": \c[UIWhite]", d:GetPlayerLivesLeft(PlayerNumber()));
	return out;
}

void DrawChromeCharacter(int pnum, int page, int boxid, int mainboxid) {
	int k = GetChromeKind(page), i, y;
	str st;
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
		s.scroll_max = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);

	if(k == MCH_KIND_OVERVIEW) {
		int lvl = CheckInventory("Level"), pct = 0;
		bool unity = PlayerModData[pnum].vals[PSTAT_EX_UNITY];
		MchText("MENUHEAD", StrParam(s:"\c[UIWhite]", l:"DND_STAT18", s:" ", d:lvl, s:"   \c[UIGreen]",
			l:GetClassLabel(StrParam(s:"CLASS", d:CheckInventory("DnD_Character") - 1), DND_CLASS_LABEL_NAME)), MCH_ID_ROWTXT + 30, MCH_ROW_X, 282, 3);
		if(lvl > 0 && lvl <= MAXLEVELS)
			pct = Clamp_Between(GetPlayerExp(pnum) / Max(LevelCurve[lvl - 1] / 100, 1), 0, 100);
		MchPic("MCXPBG", MCH_ID_PANEIMG + 21, MCH_ROW_X, 338);
		if(pct) {
			MenuClipRect(MCH_ROW_X + 3, 342, 414 * pct / 100, 8);
			MchPic("MCXPFL", MCH_ID_PANEIMG + 20, MCH_ROW_X + 3, 342);
			MenuClipRect(0, 0, 0, 0, 0);
		}
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_STAT16", s:": \c[UIWhite]", d:GetPlayerExp(pnum), s:" / ", d:(lvl > 0 && lvl <= MAXLEVELS) ? LevelCurve[lvl - 1] : 0),
			MCH_ID_ROWTXT + 31, MCH_ROW_X + 440, 346, 1);
		MchText("MENUBODY", StrParam(s:"\c[UIGrey]", l:"DND_STAT19", s:": \c[UIGold]", s:MchMoney(GetPlayerCredit(pnum)),
			s:"\n\c[UIGrey]", l:"DND_MENU_BUDGET", s:": \c[UIGold]$", d:CheckInventory("Budget"), s:"K",
			s:"\n\c[UIGrey]", l:"DND_ATTRIBUTEPTS", s:": ", s:CheckInventory("AttributePoint") ? "\c[UIGold]" : "\c[UIWhite]", d:CheckInventory("AttributePoint"),
			s:"\n\c[UIGrey]", l:"DND_MCH_LOOTPENALTY", s:": \c[UIWhite]", d:PlayerActivities[pnum].vote_skips),
			MCH_ID_ROWTXT + 32, MCH_ROW_X, 380, 3);

		if(boxid != MAINBOX_NONE && boxid <= 3)
			s.row = boxid - 1;
		for(i = 0; i < 3; ++i) {
			y = 560 + MCH_ROW_STEP * i;
			MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
			int base = 0, bonus = 0;
			if(!unity) {
				if(!i) {
					base = STAT_BASE + CheckInventory("PSTAT_Strength");
					bonus = GetStrength(pnum);
				}
				else if(i == 1) {
					base = STAT_BASE + CheckInventory("PSTAT_Dexterity");
					bonus = GetDexterity(pnum);
				}
				else {
					base = STAT_BASE + CheckInventory("PSTAT_Intellect");
					bonus = GetIntellect(pnum);
				}
			}
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_ATTR", d:i + 1)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", d:base, s:"  \c[UIGrey](\c[UIGreen]", d:bonus, s:"\c[UIGrey])"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		}
		if(unity)
			MchText("MENUBODY", StrParam(s:"\c[UIGold]", l:"UNITY", s:": \c[UIWhite]", d:GetUnity()), MCH_ID_ROWTXT + 33, MCH_ROW_X + 30, 560 + MCH_ROW_STEP * 3 + 27, 1);

		i = Clamp_Between(s.row, 0, 2);
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MCH_ATTR", d:i + 1)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetAttributeText(i), s:"\n\n\c[UIGrey]", l:"DND_MENU_ATTRKNEE"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_ATTR"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 22, 1);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MENU_STAT_REFUND1", s:" \c[UIGrey]", k:"+jump", s:"\c[UIDim] ", l:"DND_MENU_STAT_REFUND2",
			s:"\c[UIGold]$", d:GetAttributeRefundCost(pnum)), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}

	if(k == MCH_KIND_STATINDEX) {
		if(boxid != MAINBOX_NONE && boxid <= 5)
			s.row = boxid - 1;
		for(i = 0; i < 5; ++i) {
			y = MCH_ROW_Y + MCH_ROW_STEP * i;
			MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetChromeStatName(i)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
		}
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:GetChromeStatName(Clamp_Between(s.row, 0, 4))), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MchText("MENUBODY", GetChromeRunSummary(), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}

	// a stat category: the run summary on the left, the stat list in the pane
	MchText("MENUBODY", GetChromeRunSummary(), MCH_ID_ROWTXT, MCH_ROW_X, MCH_ROW_Y + 4, 3);
	DrawChromePager(page, boxid, mainboxid, 0);

	pstat_text_T? pstat_text = GetPlayerStatText();
	BuildPlayerStatsText(pnum, GetChromeStatCategory(page));
	str txt = pstat_text.text;
	if(!StrLen(txt)) {
		MenuClipRect(MCH_PANE_IX, MCH_STAT_TOP, MCH_PANE_TEXTW, MCH_STAT_VIEW, MCH_PANE_TEXTW);
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]", l:"DND_MCH_NOSTATS"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_STAT_TOP, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		s.scroll_max = 0;
		return;
	}
	s.scroll_max = Max(MchCountLines(txt, 74) - MCH_STAT_VIEW / MCH_STAT_LINEH, 0);
	s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);
	MenuClipRect(MCH_PANE_IX, MCH_STAT_TOP, MCH_PANE_TEXTW - 24, MCH_STAT_VIEW, MCH_PANE_TEXTW - 24);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", s:txt), MCH_ID_PANETXT, MCH_PANE_IX, MCH_STAT_TOP - MCH_STAT_LINEH * s.scroll, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchScrollBar(0, MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_STAT_TOP, 620, s.scroll, s.scroll_max);
	if(s.scroll_max) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_SCROLLHINT"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
	}
}

// ---------------------------------------------------------------- the research pages

#define MCH_RES_ROWS 10

typedef struct {
	int total;
	int done;
	int found;
} mch_rescount_T;

// how far a research page (RESPAGE_*) has come
mch_rescount_T module& MchCountResearch(int respage) {
	static mch_rescount_T c;
	c.total = 0;
	c.done = 0;
	c.found = 0;
	for(int i = 0; i < MENU_MAXRES_PERPAGE; ++i) {
		int id = GetResearchInfo(respage, i).res_id;
		if(id == -1)
			break;
		++c.total;
		int st = CheckResearchStatus(id);
		if(st == RES_DONE)
			++c.done;
		else if(st == RES_KNOWN)
			++c.found;
	}
	return c;
}

int MchResEntryCount(int respage) {
	return MchCountResearch(respage).total;
}

// the entry under the cursor on a research list, -1 if none
int MchResRowAt(int page, int cx, int cy) {
	mch_state_T module& s = GetMchState();
	if(s.page != page || cx < MCH_ROW_X || cx >= MCH_ROW_X + MCH_ROW_W || cy < MCH_ROW_Y)
		return -1;
	int r = (cy - MCH_ROW_Y) / MCH_ROW_STEP;
	if(r >= MCH_RES_ROWS || (cy - MCH_ROW_Y) % MCH_ROW_STEP >= MCH_ROW_H)
		return -1;
	r += s.scroll;
	if(r >= MchResEntryCount(page - SHOP_RESPAGE_BEGIN))
		return -1;
	return r;
}

str MchResName(int respage, int i) {
	int id = GetResearchInfo(respage, i).res_id;
	if(CheckResearchStatus(id) == RES_NA && (ResearchFlags[id].res_flags & RESF_NODROP))
		return StrParam(l:"DND_MCH_RES_UNKNOWN");
	return StrParam(l:StrParam(s:"RES_LABEL", d:id));
}

str MchResProgress(int respage) {
	mch_rescount_T module& c = MchCountResearch(respage);
	return StrParam(s:"\c[UIGreen]", d:c.done, s:"\c[UIGrey] / ", d:c.total);
}

void DrawChromeResearch(int pnum, int page, int boxid, int mainboxid) {
	int k = GetChromeKind(page), i, y;
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
		s.lastpos = -1;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(k == MCH_KIND_RESLIST) {
		DrawChromeResearchList(pnum, page, boxid, mainboxid);
		return;
	}

	// the categories, the donation below a gap; the weapons page splits into slot weapons and bundles
	int rows = (k == MCH_KIND_RESINDEX) ? 5 : 2;
	if(boxid != MAINBOX_NONE && boxid <= rows)
		s.row = boxid - 1;
	for(i = 0; i < rows; ++i) {
		y = GetChromeRowY(page, i);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		str name, right = "";
		if(k == MCH_KIND_RESGUNS) {
			name = StrParam(l:i ? "DND_MCH_RES_LUX" : "DND_MCH_RES_SLOT");
			right = MchResProgress(RESPAGE_SLOTGUNS + i);
		}
		else if(i == 4) {
			name = StrParam(l:"DND_MENU_DONATE");
			right = StrParam(s:"\c[UIGold]", s:MchMoney(DND_BUDGET_CONVERSION_CREDIT), s:"\c[UIGrey]  =  \c[UIGold]1K");
		}
		else {
			name = StrParam(l:GetChromeSubLabel(MCH_SEC_RESEARCH, i + 1));
			if(i == 2) {
				mch_rescount_T module& a = MchCountResearch(RESPAGE_SLOTGUNS);
				int d = a.done, t = a.total;
				mch_rescount_T module& b = MchCountResearch(RESPAGE_LUXURYGUNS);
				right = StrParam(s:"\c[UIGreen]", d:d + b.done, s:"\c[UIGrey] / ", d:t + b.total);
			}
			else
				right = MchResProgress((i == 3) ? RESPAGE_UTILITY : i);
		}
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", s:name), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
		MchText("MENUBODY", right, MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}

	i = Clamp_Between(s.row, 0, rows - 1);
	str head, body, hint = "DND_MCH_HINT_OPEN";
	if(k == MCH_KIND_RESGUNS) {
		head = i ? "DND_MCH_RES_LUX" : "DND_MCH_RES_SLOT";
		body = StrParam(l:StrParam(s:"DND_MCH_RESDESC", d:5 + i));
	}
	else if(i == 4) {
		head = "DND_MENU_DONATE";
		body = StrParam(l:"DND_MCH_RESDONATE", s:"\n\n\c[UIGrey]", l:"DND_MENU_CONVRATE", s:": \c[UIGold]", s:MchMoney(DND_BUDGET_CONVERSION_CREDIT), s:"\c[UIGrey] = \c[UIGold]1K");
		hint = "DND_MCH_HINT_DONATE";
	}
	else {
		head = GetChromeSubLabel(MCH_SEC_RESEARCH, i + 1);
		body = StrParam(l:StrParam(s:"DND_MCH_RESDESC", d:i + 1));
	}
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", s:body), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// One page of entries, picked by a click or stepped with the keys (MenuPosX, which every click sends along).
void DrawChromeResearchList(int pnum, int page, int boxid, int mainboxid) {
	int respage = page - SHOP_RESPAGE_BEGIN, n = MchResEntryCount(respage), i, y;
	mch_state_T module& s = GetMchState();
	int pos = Clamp_Between(CheckInventory("MenuPosX"), 0, Max(n - 1, 0));
	s.scroll_max = Max(n - MCH_RES_ROWS, 0);
	if(pos != s.lastpos) {
		s.lastpos = pos;
		if(pos < s.scroll)
			s.scroll = pos;
		else if(pos >= s.scroll + MCH_RES_ROWS)
			s.scroll = pos - MCH_RES_ROWS + 1;
	}
	s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);

	for(int r = 0; r < MCH_RES_ROWS && s.scroll + r < n; ++r) {
		i = s.scroll + r;
		y = MCH_ROW_Y + MCH_ROW_STEP * r;
		res_info_T module& rd = GetResearchInfo(respage, i);
		int st = CheckResearchStatus(rd.res_id);
		bool unknown = st == RES_NA && (ResearchFlags[rd.res_id].res_flags & RESF_NODROP);
		bool hot = mainboxid == MCH_NAV_BASE + MCH_EL_ROW + i;
		str plate = "N", col = "\c[UIWhite]", right;
		if(i == pos)
			plate = "S";
		else if(hot)
			plate = "H";
		else if(unknown)
			plate = "L";
		MchPic(StrParam(s:"MCROW", s:plate), MCH_ID_ROWIMG + 12 + r, MCH_ROW_X - 8, y - 8);
		MchPicScaled(unknown ? "RESNONE" : GetResearchIcon(respage, i), MCH_ID_ROWIMG + r, MCH_ROW_X + 36, y + 27, 0.8);
		if(st == RES_DONE) {
			col = "\c[UIGreen]";
			right = StrParam(s:"\c[UIGreen]", l:"DND_MCH_RES_DONESHORT");
		}
		else if(st == RES_KNOWN)
			right = StrParam(s:"\c[UIGold]", d:rd.res_cost, s:"K");
		else if(unknown) {
			col = "\c[UIDim]";
			right = "\c[UIDim]-";
		}
		else {
			col = "\c[UIGrey]";
			right = StrParam(s:"\c[UIGrey]", l:"DND_MCH_RES_NOTFOUND");
		}
		// dim text would vanish on the highlight
		if(i == pos && st != RES_DONE)
			col = "\c[UIWhite]";
		MchText("MENUBODY", StrParam(s:col, s:MchResName(respage, i)), MCH_ID_ROWTXT + 3 * r, MCH_ROW_X + 70, y + 27, 1);
		MchText("MENUBODY", right, MCH_ID_ROWTXT + 3 * r + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}
	MchScrollBar(0, MCH_ID_PANEIMG + 24, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y, 620, s.scroll, s.scroll_max);
	if(s.scroll_max) {
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_SHOWING", s:" ", d:s.scroll + 1, s:"-", d:Min(s.scroll + MCH_RES_ROWS, n), s:" / ", d:n, s:"    \c[UIDim]", l:"DND_MCH_SCROLLHINT"),
			MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
	}
	if(!n)
		return;

	// the picked entry
	res_info_T module& e = GetResearchInfo(respage, pos);
	int status = CheckResearchStatus(e.res_id);
	bool nodrop = ResearchFlags[e.res_id].res_flags & RESF_NODROP;
	bool hidden = status == RES_NA && nodrop;
	MchPic("MCWELL", MCH_ID_PANEIMG + 20, MCH_PANE_IX, MCH_PANE_Y + 24);
	MchPicScaled(hidden ? "RESNONE" : GetResearchIcon(respage, pos), MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 2.6);
	str hcol = "\c[UIWhite]", sline = StrParam(s:"\c[UIRed]", l:"DND_MCH_RES_NA");
	if(status == RES_DONE) {
		hcol = "\c[UIGreen]";
		sline = StrParam(s:"\c[UIGreen]", l:"DND_MCH_RES_DONE");
	}
	else if(status == RES_KNOWN) {
		hcol = "\c[UIGold]";
		sline = StrParam(s:"\c[UIGold]", l:"DND_MCH_RES_KNOWN");
	}
	else if(hidden)
		hcol = "\c[UIDim]";
	MchText("MENUHEAD", StrParam(s:hcol, s:MchResName(respage, pos)), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
	if(hidden)
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_ENTRY", s:" \c[UIWhite]", l:"DND_MENU_NA", s:"      \c[UIGrey]", l:"DND_MENU_BUDGET", s:" ", l:"DND_MENU_COST", s:" \c[UIWhite]", l:"DND_MENU_NA"),
			MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
	else
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_ENTRY", s:" \c[UIWhite]#", d:e.res_number, s:"      \c[UIGrey]", l:"DND_MENU_BUDGET", s:" ", l:"DND_MENU_COST", s:" \c[UIGold]", d:e.res_cost, s:"K"),
			MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
	MchText("MENUBDYS", sline, MCH_ID_PANETXT + 2, MCH_PANE_TX, MCH_PANE_Y + 112, 3);
	if(status == RES_NA && !nodrop)
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_DISCOVERYCHANCE", s:" \c[UIWhite]", s:GetFixedRepresentation(GetResearchDropRate(pnum, e.res_id, true), false), s:"%",
			s:"      \c[UIGrey]", l:"DND_MCH_RES_INVESTED", s:" \c[UIWhite]", d:ResearchInvestments[pnum][e.res_id]), MCH_ID_PANETXT + 3, MCH_PANE_TX, MCH_PANE_Y + 146, 3);

	str desc = "";
	if(!hidden)
		desc = StrParam(s:"\c[UIWhite]", l:GetResearchDescription(e.res_id));
	if(status == RES_NA && !nodrop)
		desc = StrParam(s:desc, s:"\n\n\c[UIGrey]", l:"DND_INVESTMENT_EXPLANATION");
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 212, MCH_PANE_TEXTW, MCH_RES_BTNY - 16 - MCH_PANE_Y - 212, MCH_PANE_TEXTW);
	MchText("MENUBDYS", desc, MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_Y + 212, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	// the two actions; a button that cannot act now is drawn dim but still answers, as the old boxes did
	for(i = 0; i < 2; ++i) {
		bool live = i ? (status == RES_NA && !nodrop) : status == RES_KNOWN;
		bool over = boxid == MBOX_1 + i;
		int bx = MCH_RES_BTNX + MCH_RES_BTNSTEP * i;
		MchPic(StrParam(s:over ? "MCTH" : "MCTO", d:i), MCH_ID_PANEIMG + 21 + i, bx, MCH_RES_BTNY);
		str lab;
		if(i)
			lab = StrParam(l:"DND_MENU_INVEST", s:"  ", s:MchMoney(GetInvestmentCost(pnum, respage, pos, e.res_id)));
		else
			lab = StrParam(l:"DND_MENU_RESEARCH", s:"  ", d:e.res_cost, s:"K");
		MchText("MENUTAB", StrParam(s:over ? "\c[UIWhite]" : live ? "\c[UIGold]" : "\c[UIDim]", s:lab), MCH_ID_PANETXT + 5 + i, bx + MCH_TAB_W / 2, MCH_RES_BTNY + 26, 0);
	}
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_RES"), MCH_ID_PANETXT + 7, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- the codex

#define MCH_CODEX_TOP (MCH_PANE_Y + 24)
#define MCH_CODEX_BOT (MCH_PANE_FOOT + 32)
#define MCH_CODEX_SLOTS 14

// the topic a codex page belongs to; the monster mod categories are under monster mods
int MchCodexTopic(int page) {
	if(page >= MENU_MMOD_HELP_BEGIN)
		return MENU_HELP_MMODS - MENU_HELP_BEGIN;
	return page - MENU_HELP_BEGIN;
}

typedef struct {
	str icon;
	int k;
	str text;
} mch_codexblock_T;

mch_codexblock_T module& GetMchCodexBlock() {
	static mch_codexblock_T b;
	return b;
}

int MchLegendaryCount() {
	int n = 0;
	for(int i = 0; i < MAX_LEGMONS_TEXT; ++i)
		n += HasKilledLegendary(i);
	return n;
}

// legendary monsters show only once one has been killed
int MchLegendaryAt(int b) {
	for(int i = 0; i < MAX_LEGMONS_TEXT; ++i)
		if(HasKilledLegendary(i) && !b--)
			return i;
	return -1;
}

int MchCodexBlockCount(int page) {
	switch(page) {
		case MENU_HELP_WEAPONPROPS:
		return MAX_WEAPON_PROPERTIES;
		case MENU_HELP_DAMAGETYPES:
		return MAX_DAMAGE_TYPES_MENU;
		case MENU_HELP_AILMENTS:
		return DND_MENU_MAX_AILMENTDESCRIPTIONS;
		case MENU_HELP_ORBS:
		return MAX_ORBS + 1;
		case MENU_HELP_LEGMONS:
		return Max(MchLegendaryCount(), 1);
		case MENU_HELP_MMODS:
		return 1;
	}
	if(page >= MENU_MMOD_HELP_BEGIN)
		return GetMonsterModMenuCount((page == MENU_HELP_MMODS_SPECIAL) ? -1 : page - MENU_MMOD_HELP_BEGIN);
	return 1;
}

str MchCodexDamageText(int i) {
	switch(i) {
		case 0:
		return "DND_MENU_BULLETTEXT";
		case 1:
		return "DND_MENU_MELEETEXT";
		case 2:
		return "DND_MENU_OCCULTTEXT";
		case 3:
		return "DND_MENU_ENERGYTEXT";
		case 4:
		return "DND_MENU_FIRETEXT";
		case 5:
		return "DND_MENU_ICETEXT";
		case 6:
		return "DND_MENU_POISONTEXT";
	}
	return "DND_MENU_LIGHTNINGTEXT";
}

str MchLegendaryIcon(int i) {
	switch(i) {
		case 0:
		return "LEG_DRMR";
		case 1:
		return "LEG_TORR";
		case 2:
		return "LEG_MORD";
		case 3:
		return "LEG_GODS";
	}
	return "LEG_GOLG";
}

// The same words the old pages showed, one block per entry: an icon (or none) and its text.
mch_codexblock_T module& MchCodexBlock(int page, int b) {
	mch_codexblock_T module& c = GetMchCodexBlock();
	int i;
	str cls;
	c.icon = "";
	c.k = 2.0;
	switch(page) {
		case MENU_HELP_CHARACTER:
			c.text = "";
			for(i = 1; i <= 9; ++i)
				c.text = StrParam(s:c.text, s:"\c[UIGold]*  \c[UIWhite]", l:StrParam(s:"DND_MENU_CHARHELPTEXT", d:i), s:(i < 9) ? "\n\n" : "");
		break;
		case MENU_HELP_RESEARCHES:
			c.text = "";
			for(i = 1; i <= 5; ++i)
				c.text = StrParam(s:c.text, s:"\c[UIGold]*  \c[UIWhite]", l:StrParam(s:"DND_MENU_RESHELPTEXT", d:i), s:(i < 5) ? "\n\n" : "");
		break;
		case MENU_HELP_CLASSPERKS:
			cls = StrParam(s:"CLASS", d:CheckInventory("DnD_Character") - 1);
			c.text = StrParam(s:"\c[UIGrey]", l:"DND_CLASS", s:": \c[UIGreen]", l:GetClassLabel(cls, DND_CLASS_LABEL_NAME), s:"\n\n\c-", s:GetClassPerkDisplay(cls, true));
		break;
		case MENU_HELP_WEAPONPROPS:
			c.icon = WeaponPropertyImages[b];
			c.k = 1.5;
			c.text = StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_WEPPROP_", d:b + 1), s:"\n\c[UIWhite]", l:StrParam(s:"DND_MENU_WEPHELPTEXT", d:b + 1));
		break;
		case MENU_HELP_DAMAGETYPES:
			c.icon = GetDamageTypeIcon(b);
			c.text = StrParam(l:MchCodexDamageText(b));
		break;
		case MENU_HELP_AILMENTS:
			c.text = StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MENU_AILMENT", d:b + 1), s:"\n\c[UIWhite]", l:StrParam(s:"DND_MENU_AILMENT", d:b + 1, s:"_EXP"));
		break;
		case MENU_HELP_ORBS:
			if(!b) {
				c.text = StrParam(s:"\c[UIWhite]", l:"DND_MENU_ORBHELPTEXT");
				break;
			}
			i = b - 1;
			c.icon = GetItemImage(i + ITEM_IMAGE_ORB_BEGIN);
			c.text = StrParam(s:"\c[UIGold]", l:GetInventoryTag(i + ORBS_BEGIN), s:"\n\c[UIWhite]", l:GetInventoryText(i + ORBS_BEGIN));
			if(CheckOrbRecipe(i))
				c.text = StrParam(s:c.text, s:"\n", s:GetOrbRecipeText(i));
		break;
		case MENU_HELP_LEGMONS:
			i = MchLegendaryAt(b);
			if(i == -1) {
				c.text = StrParam(s:"\c[UIDim]", l:"DND_MCH_NOLEGENDARY");
				break;
			}
			c.icon = MchLegendaryIcon(i);
			c.k = 1.5;
			c.text = StrParam(l:StrParam(s:"DND_MENU_LEGHELPTEXT", d:i + 1));
		break;
		case MENU_HELP_MMODS:
			c.text = StrParam(s:"\c[UIWhite]", l:"DND_MCH_MMODINTRO");
		break;
		default:
			c.text = StrParam(s:"\c[UIWhite]", s:GetMonsterModMenuEntry((page == MENU_HELP_MMODS_SPECIAL) ? -1 : page - MENU_MMOD_HELP_BEGIN, b));
		break;
	}
	return c;
}

// the topics down the left; on the landing page they are boxes, elsewhere chrome nav the server decodes
void DrawChromeCodexTopics(int page, int boxid, int mainboxid) {
	int cur = (page == MENU_HELP) ? -1 : MchCodexTopic(page);
	for(int i = 0; i < MCH_CODEX_TOPICS; ++i) {
		int y = MCH_ROW_Y + MCH_ROW_STEP * i;
		bool hot = (page == MENU_HELP) ? boxid == i + 1 : mainboxid == MCH_NAV_BASE + MCH_EL_ROW + i;
		str plate = "N";
		if(i == cur || (hot && page == MENU_HELP))
			plate = "S";
		else if(hot)
			plate = "H";
		MchPic(StrParam(s:"MCROW", s:plate), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_CODEX", d:i + 1)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
	}
}

void DrawChromeCodex(int pnum, int page, int boxid, int mainboxid) {
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.pscroll = 0;
		s.pscroll_max = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	DrawChromeCodexTopics(page, boxid, mainboxid);
	if(page == MENU_HELP) {
		DrawChromeCodexIndex(pnum, boxid);
		return;
	}

	int n = MchCodexBlockCount(page), i, y = MCH_CODEX_TOP - MCH_STAT_LINEH * s.pscroll, slot = 0, total = 0;
	bool icons = page == MENU_HELP_WEAPONPROPS || page == MENU_HELP_DAMAGETYPES || page == MENU_HELP_ORBS || page == MENU_HELP_LEGMONS;
	int tx = MCH_PANE_IX + (icons ? 130 : 0), cols = icons ? 68 : 80;
	for(i = 0; i < n; ++i) {
		mch_codexblock_T module& c = MchCodexBlock(page, i);
		int h = MchCountLines(c.text, cols) * MCH_STAT_LINEH;
		if(StrLen(c.icon))
			h = Max(h, 96);
		// only what reaches the view is drawn; the rest only counts toward the scroll range
		if(y + total + h > MCH_CODEX_TOP && y + total < MCH_CODEX_BOT && slot < MCH_CODEX_SLOTS) {
			// a scaled icon changes the hud size under the clip, so it is drawn only when wholly in view
			if(StrLen(c.icon) && y + total >= MCH_CODEX_TOP && y + total + 96 <= MCH_CODEX_BOT)
				MchPicScaled(c.icon, MCH_ID_PANEIMG + slot, MCH_PANE_IX + 56, y + total + 44, c.k);
			MenuClipRect(MCH_PANE_IX, MCH_CODEX_TOP, MCH_PANE_TEXTW - 24, MCH_CODEX_BOT - MCH_CODEX_TOP, MCH_PANE_TEXTW - 24 - (tx - MCH_PANE_IX) * !!StrLen(c.icon));
			MchText("MENUBDYS", c.text, MCH_ID_PANETXT + slot, StrLen(c.icon) ? tx : MCH_PANE_IX, y + total, 3);
			MenuClipRect(0, 0, 0, 0, 0);
			++slot;
		}
		total += h + MCH_STAT_LINEH;
	}
	int view = (MCH_CODEX_BOT - MCH_CODEX_TOP) / MCH_STAT_LINEH;
	s.pscroll_max = Max((total + MCH_STAT_LINEH - 1) / MCH_STAT_LINEH - view, 0);
	if(s.pscroll > s.pscroll_max)
		s.pscroll = s.pscroll_max;
	MchScrollBar(1, MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_CODEX_TOP, MCH_CODEX_BOT - MCH_CODEX_TOP, s.pscroll, s.pscroll_max);
	if(s.pscroll_max) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_SCROLLHINT"), MCH_ID_PAGETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
	}

	// the monster mod categories are this page's own boxes, as rows in the pane
	if(page == MENU_HELP_MMODS)
		for(i = 0; i < 7; ++i) {
			y = MCH_CODEX_CATY + MCH_ROW_STEP * i;
			MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_PANEIMG + 14 + i, MCH_PANE_IX - 8, y - 8);
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_MMOD", d:i + 1)), MCH_ID_PANETXT + 14 + i, MCH_PANE_IX + 30, y + 27, 1);
		}
}

// The landing page: the hovered topic up top, the game options (boxes 10-13) below it.
void DrawChromeCodexIndex(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y;
	if(boxid != MAINBOX_NONE && boxid <= MCH_CODEX_TOPICS)
		s.row = boxid - 1;
	i = Clamp_Between(s.row, 0, MCH_CODEX_TOPICS - 1);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MCH_CODEX", d:i + 1)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 180, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_CODEXDESC", d:i + 1)), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	// the option toggles flip their cvar here, on the client, when the server hands back the pulse item
	if(CheckInventory("SetMapInfo")) {
		SetCVar("dnd_showmapinfo", !GetCVar("dnd_showmapinfo"));
		SetInventory("ShowMapInfo", !CheckInventory("ShowMapInfo"));
	}
	if(CheckInventory("SetModTierInfo"))
		SetCVar("dnd_detailedmods", !GetCVar("dnd_detailedmods"));
	if(CheckInventory("SetTempWepIgnore"))
		SetCVar("dnd_ignoretempweps", !GetCVar("dnd_ignoretempweps"));

	MenuClipRect(MCH_PANE_IX, MCH_CODEX_OPTY - 70, MCH_PANE_TEXTW, 2);
	MchPic("MCDIV", MCH_ID_PAGEIMG + 1, MCH_PANE_IX, MCH_CODEX_OPTY - 70);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_OPTIONS"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_CODEX_OPTY - 34, 1);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_AVGMONLEVEL", s:" \c[UIGold]", d:CheckInventory("DnD_PTotalLevel") / Max(PlayerCount(), 1)),
		MCH_ID_PANETXT + 3, MCH_PANE_IX + MCH_ROW_W, MCH_CODEX_OPTY - 34, 2);
	for(i = 0; i < 4; ++i) {
		y = MCH_CODEX_OPTY + MCH_ROW_STEP * i;
		MchPic(StrParam(s:"MCROW", s:(boxid == MCH_CODEX_TOPICS + 1 + i) ? "S" : "N"), MCH_ID_PANEIMG + 14 + i, MCH_PANE_IX - 8, y - 8);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_OPT", d:i + 1)), MCH_ID_PANETXT + 4 + 2 * i, MCH_PANE_IX + 30, y + 27, 1);
		str v;
		if(i == 3)
			v = StrParam(s:"\c[UIGold]", l:StrParam(s:"DND_MCH_FILTER_", d:GetCVar("dnd_pickup_filter")));
		else {
			bool on;
			if(!i)
				on = CheckInventory("ShowMapInfo");
			else if(i == 1)
				on = GetCVar("dnd_detailedmods");
			else
				on = GetCVar("dnd_ignoretempweps");
			v = StrParam(s:on ? "\c[UIGreen]" : "\c[UIGrey]", l:on ? "DND_MCH_ON" : "DND_MCH_OFF");
		}
		MchText("MENUBODY", v, MCH_ID_PANETXT + 5 + 2 * i, MCH_PANE_IX + MCH_ROW_W - 20, y + 27, 2);
	}
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_CODEX"), MCH_ID_PAGETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- skills

int MchTreeLearned(int pnum, int tree) {
	int n = 0, c = GetTreeSpellCount(tree);
	for(int j = 0; j < c; ++j)
		n += GetSpellAllocatedRank(pnum, GetTreeSpellByIndex(tree, j)) > 0;
	return n;
}

// The trees, the hotbar and the gems; each tree opens its own page, which keeps its board.
void DrawChromeSkillIndex(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y, rows = MAX_SKILL_TREES + 2;
	if(s.page != MENU_SPELL) {
		s.page = MENU_SPELL;
		s.row = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(boxid != MAINBOX_NONE && boxid <= rows)
		s.row = boxid - 1;
	for(i = 0; i < rows; ++i) {
		y = GetChromeRowY(MENU_SPELL, i);
		bool empty = i < MAX_SKILL_TREES && !GetTreeSpellCount(i);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : empty ? "L" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		str name;
		if(i < MAX_SKILL_TREES) {
			name = StrParam(l:StrParam(s:"DND_SPELLTREE", d:i));
			if(!empty) {
				int got = MchTreeLearned(pnum, i);
				MchText("MENUBODY", StrParam(s:got ? "\c[UIGold]" : "\c[UIGrey]", d:got, s:"\c[UIGrey] / ", d:GetTreeSpellCount(i)),
					MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
			}
		}
		else
			name = StrParam(l:(i == MAX_SKILL_TREES) ? "DND_MCH_SUB_HOTBAR" : "DND_MCH_SUB_GEMS");
		MchText("MENUBODY", StrParam(s:empty ? "\c[UIDim]" : "\c[UIWhite]", s:name), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
	}

	i = Clamp_Between(s.row, 0, rows - 1);
	str head, body;
	if(i < MAX_SKILL_TREES) {
		head = StrParam(s:"DND_SPELLTREETITLE", d:i);
		if(GetTreeSpellCount(i))
			body = StrParam(s:"\c[UIGrey]", l:"DND_MCH_LEARNED", s:" \c[UIWhite]", d:MchTreeLearned(pnum, i), s:" / ", d:GetTreeSpellCount(i),
				s:"\n\n\c[UIWhite]", l:"DND_SPELLINDEX_HELP1", s:"\n", l:"DND_SPELLINDEX_HELP2",
				s:"\n", l:"DND_MENU_PERK_PINHELP1", s:" \c[UIGold]", k:"+jump", s:"\c[UIWhite] ", l:"DND_MENU_PERK_PINHELP2",
				s:"\n", l:"DND_SPELLINDEX_REFUND1", s:" \c[UIGold]", k:"+jump", s:"\c[UIWhite] ", l:"DND_SPELLINDEX_REFUND2");
		else
			body = StrParam(s:"\c[UIDim]", l:"DND_MCH_TREEEMPTY");
	}
	else {
		head = (i == MAX_SKILL_TREES) ? "DND_SPELLTREE_HOTBAR" : "DND_SPELLTREE_GEMS";
		body = StrParam(s:"\c[UIWhite]", l:(i == MAX_SKILL_TREES) ? "DND_MCH_HOTBARDESC" : "DND_MCH_GEMSDESC");
	}
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 520, MCH_PANE_TEXTW);
	MchText("MENUBODY", body, MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// The spell pages' own backdrop gave way to the frame; their info panel keeps a pane behind it.
void DrawChromeSpellPocket() {
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCPOCKET", RPGMENUID, 1140, 194);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// ---------------------------------------------------------------- gear

// The equipment page's own layout (480x320 units) drawn 2x in the pane, so every slot sits where players know it;
// the charm and armor groups each keep their own shape but stand further apart than on that page.
#define MCH_GEAR_CHARMX (MCH_PANE_IX + 145)
#define MCH_GEAR_ARMORX (MCH_GEAR_CHARMX + 392)
#define MCH_GEAR_Y 440

int MchGearX(int x) {
	if(x < 290)
		return MCH_GEAR_CHARMX + (x - 188) * 2;
	return MCH_GEAR_ARMORX + (x - 296) * 2;
}

int MchGearY(int y) {
	return MCH_GEAR_Y + (y - 38) * 2;
}

// the page a gear row opens, as the server's own row handling does; -1 is the gear index
int MchGearRowPage(int i) {
	switch(i) {
		case -1:
		return MENU_LOAD;
		case 6:
		return MENU_LOAD_ACC1;
		case 7:
		return MENU_LOAD_TRADE1;
		case 8:
		return MENU_LOAD_DUNGEONS;
		case 5:
		return MENU_LOAD_CRAFTING_WEAPON;
	}
	return MENU_LOAD + i + 1;
}

// a gear page whose research is not done yet: its tab, chip and row read "Not available" and do not open it
bool MchPageLocked(int page) {
	if(page == MENU_LOAD_CRAFTING_TRANSMUTING)
		return CheckResearchStatus(RES_TRANSMUTING) != RES_DONE;
	if(page >= MENU_FIRST_ACCESSORY_PAGE && page <= MENU_LAST_ACCESSORY_PAGE)
		return CheckResearchStatus(RES_ACCESSORY) != RES_DONE;
	return false;
}

str MchGearRowName(int i) {
	switch(i) {
		case 1:
		return "DND_MENU_INVENTORY";
		case 2:
		return "DND_MENU_STASH";
		case 3:
		return "DND_MENU_EQUIPMENTS";
		case 4:
		return "DND_MENU_WEAPONDISCARD";
		case 5:
		return "DND_MENU_CRAFTING";
		case 6:
		return "DND_MENU_ACCESSORIES";
		case 7:
		return "DND_MENU_TRADING";
	}
	return "DND_MENU_DUNGEONS";
}

// One equipment slot centred where the equipment page has it: its well, and the equipped item's picture at the same 2x.
void MchGearSlot(int pnum, int slot, int n, int x, int y, str well, int w, int h, bool locked, str empty) {
	int cx = MchGearX(x), cy = MchGearY(y);
	MchPic(MchWellPic(locked ? "MCSLFL" : well, slot, false), (n < 12) ? MCH_ID_ROWIMG + n : MCH_ID_ROWIMG + 9 + n, cx - w / 2, cy - h / 2);
	if(locked)
		return;
	auto item = GetUsedItem(pnum, slot);
	if(item.item_type != DND_ITEM_NULL)
		MchPicScaled(GetItemImage(item.item_image), MCH_ID_PANEIMG + n, cx, cy, 2.0);
	// an empty slot shows the same shadowed picture as the equipment page, so each slot says what goes in it
	else if(StrLen(empty))
		MchPicScaled(empty, MCH_ID_PANEIMG + n, cx, cy, 2.0);
}

// The gear pages down the left; the pane shows the hovered one over what is equipped, all four flask slots included.
void DrawChromeGearIndex(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y, n = 0;
	if(s.page != MENU_LOAD) {
		s.page = MENU_LOAD;
		s.row = 1;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(boxid > MBOX_1 && boxid <= 9)
		s.row = boxid - 1;
	// row 0 was the artifacts page, which the menu no longer has
	for(i = 1; i < 9; ++i) {
		y = MCH_ROW_Y + MCH_ROW_STEP * (i - 1);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		if(MchPageLocked(MchGearRowPage(i)))
			MchText("MENUBODY", StrParam(s:"\c[UIDim]", l:"DND_MCH_LOCKEDTAB"), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
		else
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:MchGearRowName(i)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
	}

	i = Clamp_Between(s.row, 1, 8);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:MchGearRowName(i)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 90, MCH_PANE_TEXTW);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_GEARDESC", d:i + 1)), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	MenuClipRect(MCH_PANE_IX, MCH_GEAR_Y - 44, MCH_PANE_TEXTW, 2);
	MchPic("MCDIV", MCH_ID_PAGEIMG + 1, MCH_PANE_IX, MCH_GEAR_Y - 44);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW2"), MCH_ID_PANETXT + 2, MchGearX(236), MCH_GEAR_Y - 20, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW1"), MCH_ID_PANETXT + 3, MchGearX(394), MCH_GEAR_Y - 20, 0);

	// charms: the large one on top, the two medium under it, the four small in a square
	MchGearSlot(pnum, LARGECHARM_INDEX, n++, 236, 80, "MCSLG", 80, 168, false, "");
	MchGearSlot(pnum, MEDIUMCHARM_INDEX1, n++, 208, 154, "MCSLM", 80, 112, false, "");
	MchGearSlot(pnum, MEDIUMCHARM_INDEX2, n++, 264, 154, "MCSLM", 80, 112, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX1, n++, 208, 208, "MCSLS", 80, 80, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX2, n++, 264, 208, "MCSLS", 80, 80, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX3, n++, 208, 256, "MCSLS", 80, 80, false, "");
	MchGearSlot(pnum, SMALLCHARM_INDEX4, n++, 264, 256, "MCSLS", 80, 80, false, "");
	// armor: helm over body, the class specialty left of the body, boots under it
	MchGearSlot(pnum, HELM_INDEX, n++, 394, 88, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_HELM));
	MchGearSlot(pnum, BODY_ARMOR_INDEX, n++, 394, 152, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_BODYARMOR));
	MchGearSlot(pnum, POWERCORE_INDEX, n++, 338, 152, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_SPECIALTY_DOOMGUY + GetPlayerClass()));
	MchGearSlot(pnum, BOOT_INDEX, n++, 394, 216, "MCSLA", 100, 100, false, GetArmorSlotSilhouette(DND_ITEM_BOOT));
	// flasks: the 2x2 block left of the boots, all four shown, the ones not unlocked yet as locked
	int open = GetPlayerFlaskSlots(pnum);
	for(i = 0; i <= FLASK4_INDEX - FLASK1_INDEX; ++i)
		MchGearSlot(pnum, FLASK1_INDEX + i, n++, (i & 1) ? 348 : 312, (i & 2) ? 247 : 207, "MCSLF", 64, 64, i >= open, "FLASKBAK");

	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PAGETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: equipment

// the shadowed picture an empty armor slot shows
str MchEqSilhouette(int i) {
	if(i == POWERCORE_INDEX)
		return GetArmorSlotSilhouette(DND_ITEM_SPECIALTY_DOOMGUY + GetPlayerClass());
	if(i == HELM_INDEX)
		return GetArmorSlotSilhouette(DND_ITEM_HELM);
	if(i == BOOT_INDEX)
		return GetArmorSlotSilhouette(DND_ITEM_BOOT);
	return GetArmorSlotSilhouette(DND_ITEM_BODYARMOR);
}

// Every equipped slot where the old page had it, larger; a click opens the inventory over the page as before.
void DrawChromeGearEquip(int pnum, int boxid) {
	bool view = CheckInventory("DnD_InventoryView");
	int i, hov = (!view && boxid != MAINBOX_NONE) ? boxid - 1 : -1, open = GetPlayerFlaskSlots(pnum);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW2"), MCH_ID_PANETXT + 2, MchEqX(236), MCH_EQ_Y - 24, 0);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_GEARROW1"), MCH_ID_PANETXT + 3, MchEqX(394), MCH_EQ_Y - 24, 0);
	for(i = 0; i < INV_ICON_INDEX; ++i) {
		int cx = MchEqX(MchEqSlotX(i)), cy = MchEqY(MchEqSlotY(i)), w = MchEqWellW(i), h = MchEqWellH(i);
		if(i >= FLASK1_INDEX + open) {
			MchPic(MchWellPic("MCEQFL", i, false), MCH_ID_PANEIMG + 15 + i, cx - w / 2, cy - h / 2);
			continue;
		}
		MchPic(MchWellPic(MchEqWell(i), i, i == hov), MCH_ID_PANEIMG + 15 + i, cx - w / 2, cy - h / 2);
		// item pictures stay 2x so they sit crisp inside the wells
		auto item = GetUsedItem(pnum, i);
		if(item.item_type != DND_ITEM_NULL)
			MchPicScaled(GetItemImage(item.item_image), MCH_ID_PANEIMG + i, cx, cy, 2.0);
		else if(i >= FLASK1_INDEX)
			MchPicScaled("FLASKBAK", MCH_ID_PANEIMG + i, cx, cy, 2.0);
		else if(i >= HELM_INDEX)
			MchPicScaled(MchEqSilhouette(i), MCH_ID_PANEIMG + i, cx, cy, 2.0);
	}
	bool over = !view && boxid == INV_ICON_INDEX + 1;
	MchPic(over ? "MCTH2" : "MCTO2", MCH_ID_PAGEIMG + 2, MCH_EQ_BTNX, MCH_EQ_BTNY);
	MchText("MENUTAB", StrParam(s:over ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_MENU_INVENTORY"), MCH_ID_PANETXT + 4, MCH_EQ_BTNX + MCH_TAB_W / 2, MCH_EQ_BTNY + 26, 0);
	str hint = "DND_MCH_HINT_EQUIP";
	if(view)
		hint = CheckInventory("DnD_SelectedCharmBox") ? "DND_MCH_HINT_EQUIPPICK" : "DND_MCH_HINT_EQUIPINV";
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);

	// the hovered item's card as the old page showed it; an empty slot leaves none behind
	if(view)
		return;
	if(hov >= 0 && hov < INV_ICON_INDEX && GetUsedItem(pnum, hov).item_type != DND_ITEM_NULL)
		UpdateCursorHoverData(hov, DND_SYNC_ITEMSOURCE_ITEMSUSED, 0, pnum, 0, HUDMAX_X, HUDMAX_Y);
	else if(PlayerCursorData.itemHovered != -1)
		ResetCursorHoverData();
}

// ---------------------------------------------------------------- gear: inventory grid

// The plain inventory grid keeps its cells where the hit boxes are (32 menu units each); only the art is the frame's.
#define MCH_INV_X 288
#define MCH_INV_Y 270

// Stash and inventory side by side, each on its own panel: heading and sort on top, the grid, then the stash's page
// tabs or the inventory's space; the cells are 64 x 72 (32 units of the stash hud space).
#define MCH_STASH_X 84
#define MCH_STASH_IX 804
#define MCH_STASH_Y 342
#define MCH_STASH_TABY 722
#define MCH_STASH_SORTY 280
// The trade view: their offer, our offer and our inventory side by side across the frame, buttons under our offer.
#define MCH_TRADE_X0 -160
#define MCH_TRADE_X1 432
#define MCH_TRADE_X2 1024
#define MCH_TRADE_Y 333
#define MCH_TRADE_BTNY 714
#define MCH_TRADE_ROWSTEP 38

void MchInventoryPanel(int id) {
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCINVPN", id, MCH_INV_X - 36, MCH_INV_Y - 34);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// the plate state letter for the old cell picture, empty for a bare cell
str MchCellState(str old) {
	if(!StrCmp(old, "LDTBOXO") || !StrCmp(old, "LDTBOXCN"))
		return "O";
	if(!StrCmp(old, "LDTBOXR"))
		return "R";
	if(!StrCmp(old, "LDTBOXS"))
		return "S";
	if(!StrCmp(old, "LDTBOXC"))
		return "C";
	if(!StrCmp(old, "LDTBOXN"))
		return "N";
	return "";
}

// a neighbour joins this cell's plate when it shows the same highlight, or the same item
// a confirmed trade marks every cell of that side; on the frame an item there stays an item and an empty cell plain
str MchCellPicNorm(int pnum, str old, int bid, int source) {
	if(StrCmp(old, "LDTBOXCN"))
		return old;
	return GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source) ? "LDTBOXO" : "LDTBOX";
}

int MchGridX(int grid) {
	switch(grid) {
		case 1:
		return MCH_STASH_X;
		case 2:
		return MCH_STASH_IX;
		case 3:
		return MCH_TRADE_X0;
		case 4:
		return MCH_TRADE_X1;
		case 5:
		return MCH_TRADE_X2;
	}
	return MCH_INV_X;
}

bool MchCellJoins(int pnum, str old, int bid, int nb, int boff, int source) {
	if(StrCmp(MchCellPicNorm(pnum, GetInventoryCellPic(pnum, nb, boff, source), nb, source), old))
		return false;
	if(!StrCmp(old, "LDTBOXO") || !StrCmp(old, "LDTBOXR") || !StrCmp(old, "LDTBOXCN"))
		return GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, bid, -1, source) == GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, nb, -1, source);
	return true;
}

// The frame's plate for a grid cell: items and highlights merge across their cells into one plate (MCIP<state><mask>).
void MchInventoryCell(int pnum, str old, int id, int row, int col, int boff, int source, int grid) {
	int bid = row * MAXINVENTORYBLOCKS_VERT + col, mask = 0;
	old = MchCellPicNorm(pnum, old, bid, source);
	str st = MchCellState(old), pic = "MCIP";
	if(StrLen(st)) {
		if(col > 0 && MchCellJoins(pnum, old, bid, bid - 1, boff, source))
			mask |= 1;
		if(col < MAXINVENTORYBLOCKS_VERT - 1 && MchCellJoins(pnum, old, bid, bid + 1, boff, source))
			mask |= 2;
		if(row > 0 && MchCellJoins(pnum, old, bid, bid - MAXINVENTORYBLOCKS_VERT, boff, source))
			mask |= 4;
		if(row < MAXINVENTORYBLOCKS_HORIZ - 1 && MchCellJoins(pnum, old, bid, bid + MAXINVENTORYBLOCKS_VERT, boff, source))
			mask |= 8;
		pic = StrParam(s:grid ? "MCIQ" : "MCIP", s:st, x:mask);
	}
	else if(grid)
		pic = "MCIQ";
	MenuHudSize(MCH_W, MCH_H);
	if(grid) {
		MchPic(pic, id, MchGridX(grid) + 64 * col, ((grid < 3) ? MCH_STASH_Y : MCH_TRADE_Y) + 72 * row);
		MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
		return;
	}
	MchPic(pic, id, MCH_INV_X + 96 * col, MCH_INV_Y + 108 * row);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

// the sort plate over the old sort box, the label on the id after the plate's
void MchInventorySort(bool over, int id) {
	MenuHudSize(MCH_W, MCH_H);
	MchPic(over ? "MCSRTH" : "MCSRTO", id, 660, 818);
	MchText("MENUTAB", StrParam(s:CheckInventory("DnD_SortCooldown") ? "\c[UIDim]" : over ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_SORT"), id - 1, 720, 844, 0);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

int MchInventoryCellsUsed(int pnum) {
	int n = 0;
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i)
		n += GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, i, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) != DND_ITEM_NULL;
	return n;
}

// The inventory page: the grid draws itself over the page; the page adds how full it is and how to use it.
void DrawChromeGearInv(int pnum) {
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CELLSUSED", s:"  \c[UIGold]", d:MchInventoryCellsUsed(pnum), s:" / ", d:MAX_INVENTORY_BOXES),
		MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_INV"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: stash

// canvas rect of stash view box id: 0 x, 1 y, 2 w, 3 h
int MchStashBox(int id, int what) {
	int x, y, w, h;
	if(id < 2 * MAX_INVENTORY_BOXES) {
		x = ((id < MAX_INVENTORY_BOXES) ? MCH_STASH_X : MCH_STASH_IX) + 64 * (id % MAXINVENTORYBLOCKS_VERT);
		y = MCH_STASH_Y + 72 * ((id % MAX_INVENTORY_BOXES) / MAXINVENTORYBLOCKS_VERT);
		w = 64;
		h = 72;
	}
	else if(id < 2 * MAX_INVENTORY_BOXES + MAX_EXTRA_INVENTORY_PAGES) {
		x = MCH_STASH_X + 48 * (id - 2 * MAX_INVENTORY_BOXES);
		y = MCH_STASH_TABY;
		w = 44;
		h = 42;
	}
	else if(id == 2 * MAX_INVENTORY_BOXES + PAGEID_STASHTAB_ORBS) {
		x = MCH_STASH_X + 484;
		y = MCH_STASH_TABY;
		w = 92;
		h = 42;
	}
	else {
		x = ((id == STASHSORT_BOXID_STASH - 1) ? MCH_STASH_X : MCH_STASH_IX) + 456;
		y = MCH_STASH_SORTY;
		w = 120;
		h = 52;
	}
	if(what == 0)
		return x;
	if(what == 1)
		return y;
	return (what == 2) ? w : h;
}

// the hit box edges of stash view box id in the mirrored cursor space: 0 top left x, 1 top left y, 2 bottom right x, 3 bottom right y
int MchStashRectEdge(int id, int edge) {
	if(edge == 0)
		return HUDMAX_XF - FixedDiv(MchStashBox(id, 0) << 16, MCH_SCALE_X);
	if(edge == 1)
		return HUDMAX_YF - FixedDiv(MchStashBox(id, 1) << 16, MCH_SCALE_Y);
	if(edge == 2)
		return HUDMAX_XF - FixedDiv((MchStashBox(id, 0) + MchStashBox(id, 2)) << 16, MCH_SCALE_X);
	return HUDMAX_YF - FixedDiv((MchStashBox(id, 1) + MchStashBox(id, 3)) << 16, MCH_SCALE_Y);
}

// the two panels, on the ids the old backdrops used
void MchStashPanels(int id) {
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCSTPN", id, MCH_STASH_X - 24, MCH_STASH_SORTY - 14);
	MchPic("MCSTPN", id - 1, MCH_STASH_IX - 24, MCH_STASH_SORTY - 14);
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
}

// Headings, sort buttons, page tabs and the inventory's space, on the ids the old stash controls used.
void MchStashControls(int pnum, int boxid, int curpage) {
	int base = RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES, i, x;
	bool on, over;
	MenuHudSize(MCH_W, MCH_H);
	str page = (curpage == PAGEID_STASHTAB_ORBS) ? StrParam(l:"DND_MCH_ORBS") : StrParam(l:"DND_MCH_PAGE", s:" ", d:curpage + 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_STASH", s:"  \c[UIGrey]", s:page), RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 4, MCH_STASH_X, MCH_STASH_SORTY + 26, 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_INVENTORY"), RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 5, MCH_STASH_IX, MCH_STASH_SORTY + 26, 1);
	for(i = 0; i < 2; ++i) {
		over = boxid == (i ? STASHSORT_BOXID_INV : STASHSORT_BOXID_STASH);
		x = (i ? MCH_STASH_IX : MCH_STASH_X) + 456;
		MchPic(over ? "MCSRTH" : "MCSRTO", base - 2 * MAX_EXTRA_INVENTORY_PAGES - 2 - 2 * i, x, MCH_STASH_SORTY);
		MchText("MENUTAB", StrParam(s:CheckInventory("DnD_SortCooldown") ? "\c[UIDim]" : over ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_SORT"),
			base - 2 * MAX_EXTRA_INVENTORY_PAGES - 3 - 2 * i, x + 60, MCH_STASH_SORTY + 26, 0);
	}
	for(i = 0; i <= MAX_EXTRA_INVENTORY_PAGES; ++i) {
		int id = 2 * MAX_INVENTORY_BOXES + i;
		bool orbs = i == PAGEID_STASHTAB_ORBS;
		if(!orbs && i >= CheckInventory("DnD_PlayerInventoryPages"))
			continue;
		on = curpage == i || CheckInventory(orbs ? "DnD_ButtonPress_OrbPage" : StrParam(s:"DnD_ButtonPress_", d:i + 1));
		over = boxid == STASHBUTTON_BOXID_START + i;
		str st = on ? "S" : over ? "H" : "N";
		MchPic(StrParam(s:orbs ? "MCCO" : "MCCS", s:st), base - 2 * i, MchStashBox(id, 0) - 6, MCH_STASH_TABY - 6);
		MchText("MENUSUB", StrParam(s:on ? "\c[UIGold]" : over ? "\c[UIWhite]" : "\c[UIGrey]", s:orbs ? StrParam(l:"DND_MCH_ORBS") : StrParam(d:i + 1)),
			base - 1 - 2 * i, MchStashBox(id, 0) + MchStashBox(id, 2) / 2, MCH_STASH_TABY + 21, 0);
	}
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CELLSUSED", s:"  \c[UIGold]", d:MchInventoryCellsUsed(pnum), s:" / ", d:MAX_INVENTORY_BOXES),
		RPGMENUINVENTORYID - 10 * MAX_INVENTORY_BOXES - 6, MCH_STASH_IX, MCH_STASH_TABY + 21, 1);
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
}

// ---------------------------------------------------------------- gear: trading

// canvas rect of trade view box id: 0 x, 1 y, 2 w, 3 h
int MchTradeBox(int id, int what) {
	int x, y, w = 64, h = 72;
	if(id < 3 * MAX_INVENTORY_BOXES) {
		x = MchGridX(3 + id / MAX_INVENTORY_BOXES) + 64 * (id % MAXINVENTORYBLOCKS_VERT);
		y = MCH_TRADE_Y + 72 * ((id % MAX_INVENTORY_BOXES) / MAXINVENTORYBLOCKS_VERT);
	}
	else {
		x = MCH_TRADE_X1 + 76 + 224 * (id - 3 * MAX_INVENTORY_BOXES);
		y = MCH_TRADE_BTNY;
		w = MCH_TAB_W;
		h = MCH_TAB_H;
	}
	if(what == 0)
		return x;
	if(what == 1)
		return y;
	return (what == 2) ? w : h;
}

// the hit box edges in the mirrored cursor space: 0 top left x, 1 top left y, 2 bottom right x, 3 bottom right y
int MchTradeRectEdge(int id, int edge) {
	if(edge == 0)
		return HUDMAX_XF - FixedDiv(MchTradeBox(id, 0) << 16, MCH_SCALE_X);
	if(edge == 1)
		return HUDMAX_YF - FixedDiv(MchTradeBox(id, 1) << 16, MCH_SCALE_Y);
	if(edge == 2)
		return HUDMAX_XF - FixedDiv((MchTradeBox(id, 0) + MchTradeBox(id, 2)) << 16, MCH_SCALE_X);
	return HUDMAX_YF - FixedDiv((MchTradeBox(id, 1) + MchTradeBox(id, 3)) << 16, MCH_SCALE_Y);
}

// Headings with each side's confirmation, the confirm and cancel buttons, and our space, on the old trade ids.
void MchTradeControls(int pnum, int box) {
	int base = RPGMENUINVENTORYID - 8 * MAX_INVENTORY_BOXES, tradee = GetTradee();
	bool theirs = CheckActorInventory(tradee + P_TIDSTART, "DnD_Trade_Confirmed"), ours = CheckInventory("DnD_Trade_Confirmed");
	MenuHudSize(MCH_W, MCH_H);
	// each offer's heading carries whether that side has confirmed
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", n:tradee + 1, s:"\c[UICyan]", l:"DND_MENU_THEIROFFER", s:theirs ? "   \c[UIGreen]" : "   \c[UIDim]",
		l:theirs ? "DND_MCH_TR_CONFIRMED" : "DND_MCH_TR_NOTCONFIRMED"), base - 4, MCH_TRADE_X0, MCH_TRADE_Y - 26, 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_YOUROFFERING", s:ours ? "   \c[UIGreen]" : "   \c[UIDim]",
		l:ours ? "DND_MCH_TR_CONFIRMED" : "DND_MCH_TR_NOTCONFIRMED"), base - 5, MCH_TRADE_X1, MCH_TRADE_Y - 26, 1);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_YOURINV"), base - 6, MCH_TRADE_X2, MCH_TRADE_Y - 26, 1);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CELLSUSED", s:"  \c[UIGold]", d:MchInventoryCellsUsed(pnum), s:" / ", d:MAX_INVENTORY_BOXES),
		498, MCH_TRADE_X2, MCH_TRADE_BTNY + 26, 1);
	// confirm, then cancel: pressed, confirmed (confirm only), hovered, plain
	for(int i = 0; i < 2; ++i) {
		bool press = CheckInventory(i ? "DnD_Trade_CancelButtonPress" : "DnD_Trade_ConfirmButtonPress");
		bool on = !i && ours;
		bool over = box == (i ? TRADECANCEL_BOXID : TRADECONFIRM_BOXID);
		str st = press ? "P" : on ? "A" : over ? "H" : "O";
		int x = MchTradeBox(3 * MAX_INVENTORY_BOXES + i, 0);
		MchPic(StrParam(s:"MCT", s:st, d:i), base - 8 - i, x, MCH_TRADE_BTNY);
		MchText("MENUTAB", StrParam(s:(press || on) ? "\c[UIGreen]" : over ? "\c[UIWhite]" : "\c[UIGold]", l:i ? "DND_MENU_CANCEL" : "DND_MENU_CONFIRM"),
			base - 10 - i, x + MCH_TAB_W / 2, MCH_TRADE_BTNY + 26, 0);
	}
	MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
}

// the status words a player's row carries
str MchTradeBadge(int pnum, int i) {
	str b = "";
	if(IsTradingWith(pnum, i))
		b = StrParam(s:CheckInventory("DnD_TradeAcceptWindow") ? "\c[UIGold]" : "\c[UICyan]", l:CheckInventory("DnD_TradeAcceptWindow") ? "DND_MCH_TR_WANTS" : "DND_MCH_TR_SENT");
	else if(IsTrading(i))
		b = StrParam(s:"\c[UIRed]", l:"DND_MCH_TR_BUSY");
	if(TradeMuteStatus(pnum, i))
		b = StrParam(s:b, s:StrLen(b) ? "   " : "", s:"\c[UIDim]", l:"DND_MCH_TR_MUTED");
	if(TradeMuteStatus(i, pnum))
		b = StrParam(s:b, s:StrLen(b) ? "   " : "", s:"\c[UIDim]", l:"DND_MCH_TR_MUTEDYOU");
	return b;
}

// The players to trade with, sixteen a page in the order the server counts them (rows built as they draw); the
// pane describes the hovered one. While trading, the trade view draws itself over the page.
void DrawChromeGearTrade(int pnum, int page, int boxid, int mainboxid, menu_pane_T module& p) {
	mch_state_T module& s = GetMchState();
	if(CheckInventory("InTradeView") && IsTrading(pnum)) {
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_TRADEVIEW"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
		HandleInventoryViewTrade(boxid);
		return;
	}
	if(s.page != page) {
		s.page = page;
		s.row = -1;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	int skip = MENU_TRADE_PLAYERCOUNT * (page - MENU_FIRST_TRADE), j = 0, n = 0, i, y;
	p.cursize = 0;
	for(i = 0; i < MAXPLAYERS && j < skip + MENU_TRADE_PLAYERCOUNT; ++i) {
		if(!PlayerInGame(i) || i == pnum)
			continue;
		if(j++ < skip)
			continue;
		y = MCH_ROW_Y + MCH_TRADE_ROWSTEP * n;
		MchAddBox(p, MCH_ROW_X, y, MCH_ROW_W, 34);
		if(boxid == n + 1)
			s.row = i;
		MchPic((boxid == n + 1) ? "MCRWCS" : "MCRWCN", MCH_ID_ROWIMG + n, MCH_ROW_X - 8, y - 8);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", n:i + 1), MCH_ID_ROWTXT + 2 * n, MCH_ROW_X + 24, y + 17, 1);
		MchText("MENUBDYS", MchTradeBadge(pnum, i), MCH_ID_ROWTXT + 2 * n + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 17, 2);
		++n;
	}
	DrawChromePager(page, boxid, mainboxid, n);
	if(!n) {
		MchText("MENUBODY", StrParam(s:"\c[UIDim]", l:"DND_MCH_TR_NOONE"), MCH_ID_ROWTXT, MCH_ROW_X, MCH_ROW_Y + 17, 1);
		s.row = -1;
	}
	if(s.row >= 0 && PlayerInGame(s.row)) {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", n:s.row + 1), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MchText("MENUBDYS", MchTradeBadge(pnum, s.row), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);
	}
	else
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_HEAD_TRADE"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 130, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_TR_HOW"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 132, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_TRADE"), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- home

// Bobby in his frame on the left, his welcome and how the frame works in the pane.
void DrawChromeHome() {
	MchPic("MCPORT", MCH_ID_PAGEIMG + 2, MCH_ROW_X + 195, MCH_PANE_Y);
	MchPicScaled("BBYSNC", MCH_ID_PAGEIMG + 1, MCH_ROW_X + 360, MCH_PANE_Y + 202, 1.5);
	MchText("MENUBDYS", StrParam(s:"\c[UIGold]Bobby \c[UIGrey]", l:"DND_MCH_HOME_BOBBY"), MCH_ID_PAGETXT + 3, MCH_ROW_X + 360, MCH_PANE_Y + 432, 0);
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 24, MCH_PANE_TEXTW, MCH_PANE_FOOT - MCH_PANE_Y - 24, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UICyan]*\c[UIWhite] ", l:"DND_MENU_BOBBY1", s:"\n\n\c[UICyan]*\c[UIWhite] ", l:"DND_MENU_BOBBY2",
		s:"\n\n\c[UICyan]*\c[UIWhite] ", l:"DND_MENU_BOBBY3", s:"\n\n\c[UIGold]", l:"DND_MCH_HOME_HOW", s:"\n\c[UIGrey]", l:"DND_MCH_HOME_TIPS", s:"\n\c[UIGrey]", l:"DND_MCH_HOME_TIPS2"),
		MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(0, 0, 0, 0, 0);
}

// ---------------------------------------------------------------- world: dungeons

// The key socket and Activate on the left; the pane reads out the dungeon the key opens, as the old page listed it.
void DrawChromeDungeons(int pnum, int boxid) {
	bool view = CheckInventory("DnD_InventoryView");
	int k = GetSelectedDungeonData(pnum).id, j;
	bool hov = !view && boxid == MBOX_1;
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_DUN_KEY"), MCH_ID_PAGETXT + 3, MCH_DUN_X + 60, MCH_DUN_Y - 26, 0);
	MchPic(hov ? "MCEQAH" : "MCEQA", MCH_ID_PANEIMG + 20, MCH_DUN_X, MCH_DUN_Y);
	if(k != -1) {
		MchPicScaled(GetItemImage(GetDungeonKeyImageId(k)), MCH_ID_PANEIMG, MCH_DUN_X + 60, MCH_DUN_Y + 60, 2.0);
		if(hov)
			UpdateCursorHoverData(CheckInventory("DnD_SelectedDungeonBox") - 1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, 0, pnum, 0, HUDMAX_X, HUDMAX_Y);
	}
	else
		MchPicScaled("ORBBAKQ", MCH_ID_PANEIMG, MCH_DUN_X + 60, MCH_DUN_Y + 60, 2.0);
	if(!view && !hov && PlayerCursorData.itemHovered != -1)
		ResetCursorHoverData();
	bool over = !view && boxid == MBOX_2;
	int bx = MCH_DUN_X + 60 - MCH_TAB_W / 2;
	MchPic(over ? "MCTH3" : "MCTO3", MCH_ID_PANEIMG + 21, bx, MCH_DUN_Y + 150);
	MchText("MENUTAB", StrParam(s:over ? "\c[UIWhite]" : (k != -1) ? "\c[UIGold]" : "\c[UIDim]", l:"DND_ACTIVATE"), MCH_ID_PANETXT + 5, bx + MCH_TAB_W / 2, MCH_DUN_Y + 176, 0);

	// the dungeon in the socket, or the one already open
	if(DungeonInformation.attrib_count) {
		if(k == -1 && DungeonInformation.dungeon_id != -1)
			k = DungeonInformation.dungeon_id;
		str body = "";
		for(j = 0; j < DungeonInformation.attrib_count; ++j)
			body = StrParam(s:body, s:DungeonAttributeString(DungeonInformation.attributes[j].attrib_id, DungeonInformation.attributes[j].attrib_val, 0, false, -1, false, 0, -1));
		body = StrParam(s:body, s:"\n");
		for(j = 0; j < DUN_UPSIDE_MAX; ++j)
			if(DungeonInformation.upside_vals[j])
				body = StrParam(s:body, s:DungeonUpsideString(j, DungeonInformation.upside_vals[j]), s:"\n");
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_DUNGEONS"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		if(k != -1)
			MchText("MENUBDYS", GetDungeonMonsterTypeString(k), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 120, MCH_PANE_TEXTW, MCH_PANE_FOOT - MCH_PANE_Y - 120, MCH_PANE_TEXTW);
		MchText("MENUBDYS", body, MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 122, 3);
		MenuClipRect(0, 0, 0, 0, 0);
	}
	else {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_DUNGEONS"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:(k != -1) ? "DND_MCH_DUN_READY" : "DND_MCH_DUN_HOW"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
	}
	str hint = view ? "DND_MCH_HINT_DUNPICK" : "DND_MCH_HINT_DUN";
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: weapon discard

// Each starting slot weapon can be kept or discarded, and the temporary weapon to prefer cycles; the same nine boxes.
void DrawChromeGearDiscard(int pnum, int boxid) {
	mch_state_T module& s = GetMchState();
	int i, y;
	if(s.page != MENU_LOAD_WEAPONDISCARD) {
		s.page = MENU_LOAD_WEAPONDISCARD;
		s.row = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	if(boxid != MAINBOX_NONE && boxid <= 9)
		s.row = boxid - 1;
	for(i = 0; i < 9; ++i) {
		y = GetChromeRowY(MENU_LOAD_WEAPONDISCARD, i);
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		str right;
		if(i < 8) {
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:StrParam(s:"DND_MCH_DSLOT", d:i + 1)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			if(HasPlayerDiscardedSlot(pnum, i))
				right = StrParam(s:"\c[UIRed]", l:"DND_MCH_DISCARDED");
			else
				right = StrParam(s:"\c[UIGreen]", l:"DND_MCH_KEPT");
		}
		else {
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_TEMPWEP"), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			int pref = GetPlayerPreferredTempWeapon(pnum);
			if(pref >= 0)
				right = StrParam(s:"\c[UIGold]", l:GetWeaponTag(pref + FIRST_SLOT9_WEAPON));
			else
				right = StrParam(s:"\c[UIGold]", l:(pref == -2) ? "DND_MCH_ANY" : "DND_MCH_NONE");
		}
		MchText("MENUBODY", right, MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}

	i = Clamp_Between(s.row, 0, 8);
	str head = (i < 8) ? StrParam(s:"DND_MCH_DSLOT", d:i + 1) : "DND_MCH_TEMPWEP";
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:head), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
	MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:(i < 8) ? "DND_MENU_DISCARD_EXPLANATION" : "DND_MCH_TEMPWEPDESC"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:(i < 8) ? "DND_MCH_HINT_DISCARD" : "DND_MCH_HINT_TEMPWEP"), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: accessories

int MchEquippedAccessories() {
	int n = 0, cur = CheckInventory("Accessory_Index");
	for(; cur; cur >>= 1)
		n += cur & 1;
	return n;
}

// Five accessories a page and the page arrows, the same boxes as before; the pane describes the hovered one.
void DrawChromeGearAcc(int pnum, int page, int boxid, int mainboxid, int count) {
	mch_state_T module& s = GetMchState();
	int rows = count - GetChromePagerBoxes(page), first = ACCESSORY_PER_PAGE * (page - MENU_FIRST_ACCESSORY_PAGE), i, y;
	bool open = CheckResearchStatus(RES_ACCESSORY) == RES_DONE;
	if(s.page != page) {
		s.page = page;
		s.row = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_EQUIPPED", s:"  \c[UIGold]", d:MchEquippedAccessories(), s:" / ", d:DND_ACCESSORY_BASELIMIT),
		MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	if(boxid != MAINBOX_NONE && boxid <= rows)
		s.row = boxid - 1;
	for(i = 0; i < rows; ++i) {
		int rid = first + i;
		bool owned = open && CheckInventory(GetAccessoryName(rid));
		y = MCH_ROW_Y + MCH_ROW_STEP * i;
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : owned ? "N" : "L"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		if(!owned) {
			MchText("MENUBODY", StrParam(s:(boxid == i + 1) ? "\c[UIWhite]" : "\c[UIDim]", l:open ? "DND_MCH_NOTFOUNDACC" : "DND_MCH_NEEDSRESEARCH"), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			continue;
		}
		MchPicScaled(GetAccessoryIcon(rid), MCH_ID_ROWIMG + i, MCH_ROW_X + 40, y + 27, 1.0);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetAccessoryText(rid, ACCESSORY_TAG)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 80, y + 27, 1);
		if(IsAccessoryEquipped(0, 1 << rid))
			MchText("MENUBODY", StrParam(s:"\c[UIGreen]", l:"DND_MCH_EQUIPPEDONE"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
	}
	DrawChromePager(page, boxid, mainboxid, rows);

	i = Clamp_Between(s.row, 0, Max(rows - 1, 0));
	int id = first + i;
	if(!open) {
		MchText("MENUHEAD", StrParam(s:"\c[UIDim]", l:"DND_MENU_ACCESSORIES"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_ACCLOCKED"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		return;
	}
	if(!CheckInventory(GetAccessoryName(id))) {
		MchText("MENUHEAD", StrParam(s:"\c[UIDim]", l:"DND_MCH_NOTFOUNDACC"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_ACCFIND"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		return;
	}
	bool on = IsAccessoryEquipped(0, 1 << id);
	MchPic("MCWELL", MCH_ID_PANEIMG + 20, MCH_PANE_IX, MCH_PANE_Y + 24);
	MchPicScaled(GetAccessoryIcon(id), MCH_ID_PANEIMG, MCH_PANE_IX + 130, MCH_PANE_Y + 109, 2.5);
	MchText("MENUHEAD", StrParam(s:on ? "\c[UIGreen]" : "\c[UIGold]", l:GetAccessoryText(id, ACCESSORY_TAG)), MCH_ID_PANETXT, MCH_PANE_TX, MCH_PANE_Y + 26, 3);
	MchText("MENUBDYS", StrParam(s:on ? "\c[UIGreen]" : "\c[UIGrey]", l:on ? "DND_MCH_EQUIPPEDONE" : "DND_MCH_NOTEQUIPPED"), MCH_ID_PANETXT + 1, MCH_PANE_TX, MCH_PANE_Y + 78, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 212, MCH_PANE_TEXTW, MCH_PANE_FOOT - MCH_PANE_Y - 212, MCH_PANE_TEXTW);
	MchText("MENUBDYS", StrParam(s:"\c[UIGreen]+  \c[UIWhite]", l:GetAccessoryText(id, ACCESSORY_PRO), s:"\n\n\c[UIRed]-  \c[UIWhite]", l:GetAccessoryText(id, ACCESSORY_CON)),
		MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PANE_Y + 212, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_ACC"), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// ---------------------------------------------------------------- gear: crafting

// The crafting pages answer the cursor with their own codes (rows, wells, buttons) and turn a click into what the
// server's crafting input already reads: a box in its range plus the hovered and the selected item's indices.
#define MCH_CR_ITEM 100
#define MCH_CR_MAT 200
#define MCH_CR_RECIPE 300
#define MCH_CR_APPLY 400
#define MCH_CR_SALVAGE 401
#define MCH_CR_TRANSMUTE 402
#define MCH_CR_EMPTY 403
#define MCH_CR_SOCKET 410
#define MCH_CR_FILTER 420
#define MCH_CR_PANEL 499

#define MCH_CR_LX -146
#define MCH_CR_LW 470
#define MCH_CR_LY 278
#define MCH_CR_ROWS 10
#define MCH_CR_RROWS 8
#define MCH_CR_MX 364
#define MCH_CR_MCX 717
#define MCH_CR_IMCX 756
#define MCH_CR_BTNY 840
#define MCH_CR_PX 1090
#define MCH_CR_PW 496
#define MCH_CR_PY 278
#define MCH_CR_GX 1118
#define MCH_CR_GY 352
#define MCH_CR_GCOLS 4
#define MCH_CR_GROWS 3
#define MCH_CR_GSTEP 98
#define MCH_CR_DY 724
#define MCH_CR_DLINES 5
#define MCH_CR_LINEH 27
#define MCH_CR_SX (MCH_CR_MCX - 272)
#define MCH_CR_SY 380
#define MCH_CR_GRIDY 306

typedef struct {
	int page;
	int pick;
	int sel;
	int pend;
	int filter;
	int mscroll;
	int lscroll;
	int dscroll;
	int dscroll_max;
	int hov_mat;
	int send_box;
	int send_main;
	int scroll_tic;
} mch_craft_T;

mch_craft_T module& GetMchCraft() {
	static mch_craft_T c;
	return c;
}

bool IsChromeCraftPage(int page) {
	return page >= MENU_LOAD_CRAFTING_FIRST && page <= MENU_LOAD_CRAFTING_LAST;
}

// the page's own state starts over when the page changes
mch_craft_T module& MchCraft(int page) {
	mch_craft_T module& c = GetMchCraft();
	if(c.page != page) {
		c.page = page;
		c.pick = -1;
		c.sel = -1;
		c.pend = 0;
		c.filter = 0;
		c.mscroll = 0;
		c.lscroll = 0;
		c.dscroll = 0;
		c.dscroll_max = 0;
		c.hov_mat = -1;
		c.send_main = 0;
	}
	return c;
}

// the materials a page lists, filtered: 0 all, 1 orbs, 2 tokens (gems on transmuting)
typedef struct {
	int n;
	int idx[MAX_UNIQUE_CRAFTING_TYPES];
} mch_craftlist_T;

mch_craftlist_T module& MchCraftMats(int pnum, int page, int filter) {
	static mch_craftlist_T l;
	bool tokens = IsCraftingPageForTokens(page), gems = page == MENU_LOAD_CRAFTING_TRANSMUTING && CheckResearchStatus(RES_GEMCRAFTING) == RES_DONE;
	int i, t, ty, n;
	ResetUniqueCraftingItemList();
	n = CountCraftingMaterials(tokens, gems);
	l.n = 0;
	for(i = 0; i < n && l.n < MAX_UNIQUE_CRAFTING_TYPES; ++i) {
		t = GetNextUniqueCraftableMaterial(i, tokens, gems);
		if(t == -1)
			break;
		ty = GlobalItemStorage.PlayerInventoryList[pnum][t].item_type & 0xFFFF;
		if((filter == 1 && ty != DND_ITEM_ORB) || (filter == 2 && ty == DND_ITEM_ORB))
			continue;
		l.idx[l.n++] = t;
	}
	return l;
}

int MchCraftMatAt(int pnum, int page, int k) {
	mch_craftlist_T module& l = MchCraftMats(pnum, page, GetMchCraft().filter);
	if(k < 0 || k >= l.n)
		return -1;
	return l.idx[k];
}

// the k-th craftable weapon's id
int MchCraftWeaponAt(int k) {
	for(int i = 0; i < MAXWEPS; ++i)
		if(IsCraftingWeapon(i) && !k--)
			return i;
	return -1;
}

bool MchIsCraftableAt(int pnum, int box) {
	return box >= 0 && box < MAX_INVENTORY_BOXES && IsCraftableItem(GlobalItemStorage.PlayerInventoryList[pnum][box].item_type) && GlobalItemStorage.PlayerInventoryList[pnum][box].height;
}

// the orbs a transmute recipe makes, in order; the k-th one, or -1
int MchRecipeAt(int k) {
	for(int i = 0; i < DND_MAX_ORB_KINDS; ++i)
		if(GetOrbRecipe(i).components[0].id != -1 && !k--)
			return i;
	return -1;
}

int MchRecipeCount() {
	int n = 0;
	for(int i = 0; i < DND_MAX_ORB_KINDS; ++i)
		n += GetOrbRecipe(i).components[0].id != -1;
	return n;
}

str MchOrbName(int o) {
	return GetInventoryTag(ORBS_BEGIN + o);
}

// What the cursor is over on a crafting page, as one of the codes above.
int MchCraftHit(int pnum, int page) {
	mch_craft_T module& c = MchCraft(page);
	int cx = MchCursorX(), cy = MchCursorY(), i, n;
	if(c.pend)
		--c.pend;
	if(MchIn(cx, cy, MCH_CR_PX, MCH_CR_PY, MCH_CR_PW, 616)) {
		for(i = 0; i < 3; ++i)
			if(MchIn(cx, cy, MCH_CR_PX + MCH_CR_PW - 268 + 80 * i, MCH_CR_PY + 16, 80, 32))
				return MCH_CR_FILTER + i;
		if(MchIn(cx, cy, MCH_CR_GX, MCH_CR_GY, MCH_CR_GCOLS * MCH_CR_GSTEP, MCH_CR_GROWS * MCH_CR_GSTEP)
			&& (cx - MCH_CR_GX) % MCH_CR_GSTEP < 86 && (cy - MCH_CR_GY) % MCH_CR_GSTEP < 86) {
			i = ((cy - MCH_CR_GY) / MCH_CR_GSTEP + c.mscroll) * MCH_CR_GCOLS + (cx - MCH_CR_GX) / MCH_CR_GSTEP;
			if(MchCraftMatAt(pnum, page, i) != -1)
				return MCH_CR_MAT + i;
		}
		return MCH_CR_PANEL;
	}
	if(page == MENU_LOAD_CRAFTING_TRANSMUTING) {
		for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i)
			if(MchIn(cx, cy, MCH_CR_SX + 130 * i, MCH_CR_SY, 120, 120))
				return MCH_CR_SOCKET + i;
		if(MchIn(cx, cy, MCH_CR_MCX - 212, 700, MCH_TAB_W, MCH_TAB_H))
			return MCH_CR_TRANSMUTE;
		if(MchIn(cx, cy, MCH_CR_MCX + 12, 700, MCH_TAB_W, MCH_TAB_H))
			return MCH_CR_EMPTY;
		n = MchRecipeCount();
		for(i = 0; i < MCH_CR_RROWS && i + c.lscroll < n; ++i)
			if(MchIn(cx, cy, MCH_CR_LX, MCH_CR_LY + 52 + 70 * i, MCH_CR_LW, 62))
				return MCH_CR_RECIPE + i + c.lscroll;
		return MAINBOX_NONE;
	}
	if(page == MENU_LOAD_CRAFTING_WEAPON) {
		if(MchIn(cx, cy, MCH_CR_MCX - 100, MCH_CR_BTNY, MCH_TAB_W, MCH_TAB_H))
			return MCH_CR_APPLY;
		n = GetWeaponCount();
		for(i = 0; i < MCH_CR_ROWS && i + c.lscroll < n; ++i)
			if(MchIn(cx, cy, MCH_CR_LX, MCH_CR_LY + 62 * i, MCH_CR_LW, 54))
				return MCH_CR_ITEM + i + c.lscroll;
		return MAINBOX_NONE;
	}
	// inventory crafting: a craftable item answers by its top left box
	if(MchIn(cx, cy, MCH_CR_IMCX - 212, MCH_CR_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_CR_APPLY;
	if(MchIn(cx, cy, MCH_CR_IMCX + 12, MCH_CR_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_CR_SALVAGE;
	if(MchIn(cx, cy, MCH_CR_LX, MCH_CR_GRIDY, 64 * MAXINVENTORYBLOCKS_VERT, 72 * MAXINVENTORYBLOCKS_HORIZ)) {
		i = ((cy - MCH_CR_GRIDY) / 72) * MAXINVENTORYBLOCKS_VERT + (cx - MCH_CR_LX) / 64;
		n = GlobalItemStorage.PlayerInventoryList[pnum][i].topleftboxid - 1;
		if(MchIsCraftableAt(pnum, n))
			return MCH_CR_ITEM + n;
	}
	return MAINBOX_NONE;
}

int MchCraftApply(int pnum, int mat) {
	mch_craft_T module& c = GetMchCraft();
	c.send_box = MATERIALBOX_OFFSET_BOXID + 1;
	MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = (mat << DND_MENU_ITEMSAVEBITS1) | (Max(c.sel, 0) << DND_MENU_ITEMSAVEBITS2);
	return DND_MENUINPUT_RCLICK;
}

// A click on a crafting page becomes what the server reads, or is answered here and not sent (0).
int MchCraftInput(int pnum, int page, int input, int boxid) {
	mch_craft_T module& c = MchCraft(page);
	int selbox = CheckInventory("DnD_SelectedInventoryBox"), other = (selbox == 1) ? 2 : 1, it;
	bool l = input == DND_MENUINPUT_LCLICK;
	c.send_box = boxid;
	c.send_main = 0;
	MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
	if(input != DND_MENUINPUT_LCLICK && input != DND_MENUINPUT_RCLICK) {
		c.send_box = MAINBOX_NONE;
		return input;
	}
	// the salvage confirm: its buttons already became a right click (salvage) or a left click (cancel)
	if(CheckInventory("DnD_SellConfirm")) {
		c.send_box = 1;
		return input;
	}
	// a left click into nothing lets the server drop its selection, as it always did; on the panel it drops the pick
	if(boxid == MAINBOX_NONE)
		return input;
	if(boxid == MCH_CR_PANEL) {
		if(l && c.pick != -1) {
			c.pick = -1;
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		return 0;
	}
	if(boxid >= MCH_CR_FILTER && boxid < MCH_CR_FILTER + 3) {
		if(l) {
			c.filter = (boxid == MCH_CR_FILTER + 2) ? 0 : boxid - MCH_CR_FILTER + 1;
			c.mscroll = 0;
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		return 0;
	}
	if(page == MENU_LOAD_CRAFTING_TRANSMUTING) {
		c.send_box = MAINBOX_NONE;
		if(boxid >= MCH_CR_MAT && boxid < MCH_CR_RECIPE) {
			it = MchCraftMatAt(pnum, page, boxid - MCH_CR_MAT);
			if(!l || it == -1)
				return 0;
			c.send_main = MCH_NAV_BASE + MCH_EL_CRFILL + it;
			return input;
		}
		if(boxid >= MCH_CR_RECIPE && boxid < MCH_CR_APPLY) {
			it = MchRecipeAt(boxid - MCH_CR_RECIPE);
			if(!l || it == -1 || !CheckOrbRecipe(it))
				return 0;
			c.send_main = MCH_NAV_BASE + MCH_EL_CRRECIPE + it;
			return input;
		}
		// either click on a socket takes its material back out
		if(boxid >= MCH_CR_SOCKET && boxid < MCH_CR_SOCKET + MAX_TRANSMUTE_BOXES) {
			c.send_box = boxid - MCH_CR_SOCKET + 1;
			return DND_MENUINPUT_RCLICK;
		}
		if(boxid == MCH_CR_TRANSMUTE && l) {
			c.send_box = MBOX_4;
			return input;
		}
		if(boxid == MCH_CR_EMPTY && l) {
			c.send_main = MCH_NAV_BASE + MCH_EL_CREMPTY;
			return input;
		}
		return 0;
	}
	if(boxid >= MCH_CR_MAT && boxid < MCH_CR_RECIPE) {
		it = MchCraftMatAt(pnum, page, boxid - MCH_CR_MAT);
		if(it == -1)
			return 0;
		// left picks (or drops) the material, right applies it straight away as before
		if(l) {
			c.pick = (c.pick == it) ? -1 : it;
			LocalAmbientSound("RPG/MenuChoose", 127);
			return 0;
		}
		return MchCraftApply(pnum, it);
	}
	if(boxid == MCH_CR_APPLY) {
		if(!l || c.pick == -1)
			return 0;
		return MchCraftApply(pnum, c.pick);
	}
	if(boxid == MCH_CR_SALVAGE) {
		if(!l || c.sel == -1)
			return 0;
		c.send_box = other;
		MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = c.sel << DND_MENU_ITEMSAVEBITS1;
		return DND_MENUINPUT_RCLICK;
	}
	if(boxid >= MCH_CR_ITEM && boxid < MCH_CR_MAT) {
		it = (page == MENU_LOAD_CRAFTING_WEAPON) ? MchCraftWeaponAt(boxid - MCH_CR_ITEM) : boxid - MCH_CR_ITEM;
		if(it == -1 || (!l && page == MENU_LOAD_CRAFTING_WEAPON))
			return 0;
		// a box other than the selected one, which the server would ignore; a left click on the selection keeps it
		c.send_box = (l && it == c.sel && selbox) ? selbox : other;
		MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = (it << DND_MENU_ITEMSAVEBITS1) | (Max(c.sel, 0) << DND_MENU_ITEMSAVEBITS2);
		if(l) {
			c.sel = it;
			c.pend = 35;
		}
		return input;
	}
	return 0;
}

// The material an item card previews: the picked one, else the one under the cursor.
int MchCraftPreview(int hovered) {
	if(!IsChromeCraftPage(CheckInventory("MenuOption")))
		return hovered;
	mch_craft_T module& c = GetMchCraft();
	if(c.pick != -1)
		return c.pick;
	return c.hov_mat;
}

// the selection follows the server's: dropped once it dropped it (a click having had time to land), or the item is gone
void MchCraftSyncSelection(int pnum, int page) {
	mch_craft_T module& c = GetMchCraft();
	if(c.pick != -1 && GlobalItemStorage.PlayerInventoryList[pnum][c.pick].item_type == DND_ITEM_NULL)
		c.pick = -1;
	if(c.sel == -1 || c.pend)
		return;
	if(!CheckInventory("DnD_SelectedInventoryBox") && !CheckInventory("DnD_UsedTwoItemRequirementMaterial"))
		c.sel = -1;
	else if(page == MENU_LOAD_CRAFTING_INVENTORY && !MchIsCraftableAt(pnum, c.sel))
		c.sel = -1;
	else if(page == MENU_LOAD_CRAFTING_WEAPON && !IsCraftingWeapon(c.sel))
		c.sel = -1;
}

// a scroll bar of h pixels for a list showing shown of total from first
void MchCraftScrollBar(int slot, int id, int x, int y, int h, int first, int shown, int total) {
	MchScrollBar(slot, id, x, y, h, first, total - shown);
}

void MchCraftButton(int id, int x, int y, str label, bool over, bool live, int k) {
	MchPic(StrParam(s:over ? "MCTH" : "MCTO", d:k), id, x, y);
	MchText("MENUTAB", StrParam(s:(over && live) ? "\c[UIWhite]" : live ? "\c[UIGold]" : "\c[UIDim]", l:label), id - 1, x + MCH_TAB_W / 2, y + 26, 0);
}

// The materials panel: filters, a scrolling grid of wells with the counts etched into their corners, and the
// picked or hovered material named and explained below.
void MchCraftMaterials(int pnum, int page, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	mch_craftlist_T module& l = MchCraftMats(pnum, page, c.filter);
	int B = MCH_ID_CRAFT_FIRST, i, k, rows = (l.n + MCH_CR_GCOLS - 1) / MCH_CR_GCOLS;
	MchPic("MCCRPN", B + 199, MCH_CR_PX, MCH_CR_PY);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MENU_MATERIALS"), B + 170, MCH_CR_GX, MCH_CR_PY + 32, 1);
	for(i = 0; i < 3; ++i) {
		str label = "DND_MCH_CR_ALL";
		if(!i)
			label = "DND_MCH_ORBS";
		else if(i == 1)
			label = (page == MENU_LOAD_CRAFTING_TRANSMUTING) ? "DND_MCH_CR_GEMS" : "DND_MCH_CR_TOKENS";
		bool on = c.filter == ((i == 2) ? 0 : i + 1);
		MchText("MENUBDYS", StrParam(s:on ? "\c[UIGold]" : (boxid == MCH_CR_FILTER + i) ? "\c[UIWhite]" : "\c[UIGrey]", l:label), B + 171 + i, MCH_CR_PX + MCH_CR_PW - 228 + 80 * i, MCH_CR_PY + 32, 0);
	}
	c.mscroll = Clamp_Between(c.mscroll, 0, Max(rows - MCH_CR_GROWS, 0));
	c.hov_mat = -1;
	for(k = 0; k < MCH_CR_GCOLS * MCH_CR_GROWS; ++k) {
		i = k + c.mscroll * MCH_CR_GCOLS;
		if(i >= l.n)
			break;
		int idx = l.idx[i], x = MCH_CR_GX + (k % MCH_CR_GCOLS) * MCH_CR_GSTEP, y = MCH_CR_GY + (k / MCH_CR_GCOLS) * MCH_CR_GSTEP;
		bool hov = boxid == MCH_CR_MAT + i;
		if(hov)
			c.hov_mat = idx;
		str st = (idx == c.pick) ? "S" : hov ? "H" : "N";
		MchPic(StrParam(s:"MCMWB", s:st), B + 45 + k, x, y);
		MchPicScaled(GetItemImage(GlobalItemStorage.PlayerInventoryList[pnum][idx].item_image), B + 30 + k, x + 43, y + 43, 2.0);
		MchPic(StrParam(s:"MCMWF", s:st), B + 15 + k, x, y);
		MchText("MENUNUM", StrParam(s:"\c[UIWhite]", s:MchStackText(GetTotalStackOfMaterial(idx))), B + k, x + 79, y + 69, 2);
	}
	MchCraftScrollBar(1, B + 174, MCH_CR_GX + 410, MCH_CR_GY, MCH_CR_GROWS * MCH_CR_GSTEP - 12, c.mscroll, MCH_CR_GROWS, rows);
	MenuClipRect(MCH_CR_GX, 658, MCH_CR_PW - 56, 4);
	MchPic("MCDIV", B + 198, MCH_CR_GX, 660);
	MenuClipRect(0, 0, 0, 0, 0);
	if(!l.n)
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]", l:"DND_MCH_CR_NOMATS"), B + 176, MCH_CR_GX, MCH_CR_GY + 30, 1);

	// what the shown material is and does
	int show = (c.hov_mat != -1) ? c.hov_mat : c.pick;
	if(show == -1) {
		c.dscroll_max = 0;
		MchText("MENUBDYS", StrParam(s:"\c[UIDim]", l:(page == MENU_LOAD_CRAFTING_TRANSMUTING) ? "DND_MCH_CR_PICKT" : "DND_MCH_CR_PICKM"), B + 177, MCH_CR_GX, 690, 1);
		return;
	}
	int ty = GlobalItemStorage.PlayerInventoryList[pnum][show].item_type & 0xFFFF, sub = GlobalItemStorage.PlayerInventoryList[pnum][show].item_subtype;
	str name, body = "";
	if(ty == DND_ITEM_GEM) {
		name = GetGemFullName(sub, GlobalItemStorage.PlayerInventoryList[pnum][show].implicit[0].attrib_id);
		body = StrParam(l:"DND_MCH_CR_GEMBODY");
	}
	else {
		name = StrParam(l:GetInventoryTag(sub + GetInventoryInfoOffset(ty, sub)));
		body = StrParam(l:GetInventoryText(sub + GetInventoryInfoOffset(ty, sub)));
	}
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", s:name), B + 176, MCH_CR_GX, 690, 1);
	// a long description scrolls with the forward / back keys while the cursor is on the panel
	c.dscroll_max = Max(MchCountLines(body, 33) - MCH_CR_DLINES, 0);
	c.dscroll = Clamp_Between(c.dscroll, 0, c.dscroll_max);
	MenuClipRect(MCH_CR_GX, MCH_CR_DY, 400, MCH_CR_DLINES * MCH_CR_LINEH, 400);
	MchText("MENUBDYS", StrParam(s:"\c[UIWhite]", s:body), B + 178, MCH_CR_GX, MCH_CR_DY - MCH_CR_LINEH * c.dscroll, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchCraftScrollBar(2, B + 179, MCH_CR_GX + 410, MCH_CR_DY, MCH_CR_DLINES * MCH_CR_LINEH, c.dscroll, MCH_CR_DLINES, c.dscroll_max + MCH_CR_DLINES);
}

// The selected item's card, which the menu draws every tic at the card spot in the middle column.
void MchCraftCard(int pnum, int page) {
	mch_craft_T module& c = GetMchCraft();
	// the card's panel sits 16 units right of and below the cursor point it is given
	int cx = HUDMAX_XF - FixedDiv(((page == MENU_LOAD_CRAFTING_INVENTORY) ? MCH_CR_IMCX - 318 : MCH_CR_MCX - 318) << 16, MCH_SCALE_X) + 16.0, cy = HUDMAX_YF - FixedDiv(MCH_CR_LY << 16, MCH_SCALE_Y) + 16.0;
	if(c.sel == -1) {
		if(PlayerCursorData.itemClicked != -1)
			ResetCursorClickData();
		MchText("MENUBODY", StrParam(s:"\c[UIDim]", l:(page == MENU_LOAD_CRAFTING_WEAPON) ? "DND_MCH_CR_PICKW" : "DND_MCH_CR_PICKI"),
			MCH_ID_CRAFT_FIRST + 182, (page == MENU_LOAD_CRAFTING_INVENTORY) ? MCH_CR_IMCX : MCH_CR_MCX, 420, 0);
		return;
	}
	if(page == MENU_LOAD_CRAFTING_WEAPON)
		UpdateCursorClickData(c.sel, 0, DND_ITEM_WEAPON, cx, cy);
	else
		UpdateCursorClickData(c.sel, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, GlobalItemStorage.PlayerInventoryList[pnum][c.sel].item_type, cx, cy);
	MchCraftUseLine(pnum, page);
}

// Over the buttons: what the picked material will be used on, or that it cannot be; Enhancement's quality range.
void MchCraftUseLine(int pnum, int page) {
	mch_craft_T module& c = GetMchCraft();
	if(c.pick == -1)
		return;
	int ty = GlobalItemStorage.PlayerInventoryList[pnum][c.pick].item_type & 0xFFFF, sub = GlobalItemStorage.PlayerInventoryList[pnum][c.pick].item_subtype;
	int x = (page == MENU_LOAD_CRAFTING_INVENTORY) ? MCH_CR_IMCX : MCH_CR_MCX, itype = DND_ITEM_WEAPON, q;
	bool ok = true;
	if(page == MENU_LOAD_CRAFTING_INVENTORY)
		itype = GlobalItemStorage.PlayerInventoryList[pnum][c.sel].item_type;
	if(ty == DND_ITEM_ORB)
		ok = CanUseOrb(sub, c.sel, itype);
	else if(ty == DND_ITEM_TOKEN && page == MENU_LOAD_CRAFTING_INVENTORY)
		ok = CanUseToken(sub, c.sel, itype);
	str mat = StrParam(l:GetInventoryTag(sub + GetInventoryInfoOffset(ty, sub)));
	if(!ok) {
		MchText("MENUBDYS", StrParam(s:"\c[UIRed]", s:mat, s:" ", l:"DND_MCH_CR_CANT"), MCH_ID_CRAFT_FIRST + 181, x, MCH_CR_BTNY - 36, 0);
		return;
	}
	str on = (page == MENU_LOAD_CRAFTING_WEAPON) ? StrParam(l:GetWeaponTag(c.sel)) : StrParam(l:"DND_MCH_CR_THEITEM");
	MchText("MENUBDYS", StrParam(s:"\c[UICyan]", s:mat, s:" \c[UIGrey]", l:"DND_MCH_CR_ON", s:" \c[UIWhite]", s:on), MCH_ID_CRAFT_FIRST + 181, x, MCH_CR_BTNY - 36, 0);
	if(ty != DND_ITEM_ORB || sub != DND_ORB_ENHANCE || page != MENU_LOAD_CRAFTING_WEAPON)
		return;
	q = GetPlayerWeaponQuality(pnum, c.sel);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CR_QUALITY", s:" \c[UIWhite]+", d:q, s:"%  \c[UIGrey]>  \c[UIGold]+", d:Min(q + QUALITY_ITEM_ADD_MIN * GetAffluenceBonus(), ENHANCEORB_MAX),
		s:"% \c[UIGrey]", l:"DND_MCH_CR_TO", s:" \c[UIGold]+", d:Min(q + QUALITY_ITEM_ADD_MAX * GetAffluenceBonus(), ENHANCEORB_MAX), s:"%"), MCH_ID_CRAFT_FIRST + 180, x, MCH_CR_BTNY - 70, 0);
}

void MchCraftWeapons(int pnum, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int B = MCH_ID_CRAFT_FIRST, n = GetWeaponCount(), i, w, y, q;
	c.lscroll = Clamp_Between(c.lscroll, 0, Max(n - MCH_CR_ROWS, 0));
	for(i = 0; i < MCH_CR_ROWS && i + c.lscroll < n; ++i) {
		w = MchCraftWeaponAt(i + c.lscroll);
		y = MCH_CR_LY + 62 * i;
		bool hov = boxid == MCH_CR_ITEM + i + c.lscroll;
		MchPic((w == c.sel || hov) ? "MCCRRS" : "MCCRRN", B + 120 + i, MCH_CR_LX - 8, y - 8);
		MchPicScaled(Weapons_Data[w].icon, B + 100 + i, MCH_CR_LX + 48, y + 27, 1.5);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetWeaponTag(w)), B + 60 + i, MCH_CR_LX + 100, y + 27, 1);
		q = GetPlayerWeaponQuality(pnum, w);
		MchText("MENUBDYS", StrParam(s:(w == c.sel || hov) ? "\c[UIWhite]" : q ? "\c[UIGold]" : "\c[UIDim]", s:"+", d:q, s:"%"), B + 80 + i, MCH_CR_LX + MCH_CR_LW - 20, y + 27, 2);
		if(hov && w != c.sel)
			UpdateCursorHoverData(w, 0, DND_ITEM_WEAPON, pnum, 0);
	}
	MchCraftScrollBar(0, B + 186, MCH_CR_LX + MCH_CR_LW + 10, MCH_CR_LY, 62 * MCH_CR_ROWS - 8, c.lscroll, MCH_CR_ROWS, n);
	MchCraftButton(B + 161, MCH_CR_MCX - 100, MCH_CR_BTNY, "DND_MCH_CR_APPLY", boxid == MCH_CR_APPLY, c.pick != -1 && c.sel != -1, 0);
}

// a cell's plate: S the hovered item, C the selected one, O any other item, "" empty
str MchCraftCellState(int pnum, int box, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int top = GlobalItemStorage.PlayerInventoryList[pnum][box].topleftboxid - 1;
	if(top < 0)
		return "";
	if(top == c.sel)
		return "C";
	if(boxid == MCH_CR_ITEM + top)
		return "S";
	return "O";
}

void MchCraftInventory(int pnum, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int B = MCH_ID_CRAFT_FIRST, r, col, box, top, mask;
	for(box = 0; box < MAX_INVENTORY_BOXES; ++box) {
		r = box / MAXINVENTORYBLOCKS_VERT;
		col = box % MAXINVENTORYBLOCKS_VERT;
		str st = MchCraftCellState(pnum, box, boxid), pic = "MCIQ";
		top = GlobalItemStorage.PlayerInventoryList[pnum][box].topleftboxid;
		if(StrLen(st)) {
			mask = 0;
			if(col > 0 && GlobalItemStorage.PlayerInventoryList[pnum][box - 1].topleftboxid == top)
				mask |= 1;
			if(col < MAXINVENTORYBLOCKS_VERT - 1 && GlobalItemStorage.PlayerInventoryList[pnum][box + 1].topleftboxid == top)
				mask |= 2;
			if(r > 0 && GlobalItemStorage.PlayerInventoryList[pnum][box - MAXINVENTORYBLOCKS_VERT].topleftboxid == top)
				mask |= 4;
			if(r < MAXINVENTORYBLOCKS_HORIZ - 1 && GlobalItemStorage.PlayerInventoryList[pnum][box + MAXINVENTORYBLOCKS_VERT].topleftboxid == top)
				mask |= 8;
			pic = StrParam(s:"MCIQ", s:st, x:mask);
		}
		MchPic(pic, B + 105 + box, MCH_CR_LX + 64 * col, MCH_CR_GRIDY + 72 * r);
		if(top != box + 1)
			continue;
		// the picture once, as the stash grid places it; items crafting cannot touch are dimmed
		int w = GlobalItemStorage.PlayerInventoryList[pnum][box].width, h = GlobalItemStorage.PlayerInventoryList[pnum][box].height;
		bool craftable = MchIsCraftableAt(pnum, box);
		MenuHudSize(HUDMAX_X_STASH, HUDMAX_Y_STASH);
		SetFont(GetItemImage(GlobalItemStorage.PlayerInventoryList[pnum][box].item_image, true));
		HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, B + 60 + box, CR_WHITE,
			MenuX(((MCH_CR_LX + 64 * col) << 15) + 16.4 + (w - 1) * ITEMOFFSETSCALE),
			MenuY(FixedDiv((MCH_CR_GRIDY + 72 * r) << 16, 2.25) + 16.0 + (h - 1) * ITEMOFFSETSCALE), 0.0, craftable ? 1.0 : 0.3);
		MenuHudSize(MCH_W, MCH_H);
		if(craftable && boxid == MCH_CR_ITEM + box && box != c.sel)
			UpdateCursorHoverData(box, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, GlobalItemStorage.PlayerInventoryList[pnum][box].item_type, pnum, 0);
	}
	MchCraftButton(B + 161, MCH_CR_IMCX - 212, MCH_CR_BTNY, "DND_MCH_CR_APPLY", boxid == MCH_CR_APPLY, c.pick != -1 && c.sel != -1, 0);
	MchCraftButton(B + 163, MCH_CR_IMCX + 12, MCH_CR_BTNY, "DND_MCH_CR_SALVAGE", boxid == MCH_CR_SALVAGE, c.sel != -1, 2);
}

// a chamber socket's picture: an orb, a gem still in its box, or the empty mark once that gem is gone
str MchSocketImage(int pnum, int v) {
	if(IsTransmuteGemSlot(v)) {
		int g = GetTransmuteSlotGem(pnum, v);
		return (g != -1) ? GetItemImage(GetGemImageId(GetPlayerInventoryItem(pnum, g).item_subtype)) : "ORBBAK";
	}
	return GetItemImage(GetOrbItemImage(v));
}

void MchCraftTransmute(int pnum, int boxid) {
	mch_craft_T module& c = GetMchCraft();
	int B = MCH_ID_CRAFT_FIRST, n = MchRecipeCount(), i, j, o, y;
	auto d = GetTransmuteOrbData(pnum);
	// the recipe book: what each orb is made from once found; picking one fills the chamber
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_CR_RECIPES"), B + 183, MCH_CR_LX, MCH_CR_LY + 20, 1);
	c.lscroll = Clamp_Between(c.lscroll, 0, Max(n - MCH_CR_RROWS, 0));
	for(i = 0; i < MCH_CR_RROWS && i + c.lscroll < n; ++i) {
		o = MchRecipeAt(i + c.lscroll);
		y = MCH_CR_LY + 52 + 70 * i;
		bool known = CheckOrbRecipe(o);
		MchPic((known && boxid == MCH_CR_RECIPE + i + c.lscroll) ? "MCCRQS" : "MCCRQN", B + 120 + i, MCH_CR_LX - 8, y - 8);
		MchPicScaled(GetItemImage(GetOrbItemImage(o)), B + 100 + i, MCH_CR_LX + 36, y + 31, 1.5);
		MchText("MENUBODY", StrParam(s:known ? "\c[UIWhite]" : "\c[UIGrey]", l:MchOrbName(o)), B + 60 + i, MCH_CR_LX + 76, y + 19, 1);
		str parts = "";
		if(known) {
			for(j = 0; j < MAX_TRANSMUTE_BOXES; ++j) {
				int cid = GetOrbRecipe(o).components[j].id;
				if(cid == -1)
					break;
				parts = StrParam(s:parts, s:j ? "  +  " : "", d:GetOrbRecipe(o).components[j].count, s:" x ", l:MchOrbName(cid));
			}
		}
		else
			parts = StrParam(l:"DND_MCH_CR_UNKNOWN");
		MchText("MENUSUB", StrParam(s:"\c[UIGrey]", s:parts), B + 80 + i, MCH_CR_LX + 76, y + 44, 1);
	}
	MchCraftScrollBar(0, B + 186, MCH_CR_LX + MCH_CR_LW + 10, MCH_CR_LY + 52, 70 * MCH_CR_RROWS - 8, c.lscroll, MCH_CR_RROWS, n);

	// the chamber: three sockets, then what they make
	MchPic("MCCRPM", B + 197, MCH_CR_MCX - 330, MCH_CR_PY);
	MchText("MENUTAB", StrParam(s:"\c[UICyan]", l:"DND_MCH_CR_CHAMBER"), B + 184, MCH_CR_MCX, MCH_CR_SY - 40, 0);
	for(i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		int x = MCH_CR_SX + 130 * i;
		MchPic((boxid == MCH_CR_SOCKET + i) ? "MCEQAH" : "MCEQA", B + 154 + i, x, MCH_CR_SY);
		if(d.val[i] != -1)
			MchPicScaled(MchSocketImage(pnum, d.val[i]), B + 150 + i, x + 60, MCH_CR_SY + 60, 2.0);
	}
	o = IsValidTransmuteRecipe(pnum);
	MchText("MENUHEAD", StrParam(s:(o >= 0) ? "\c[UIGold]" : "\c[UIDim]", s:"="), B + 187, MCH_CR_SX + 412, MCH_CR_SY + 60, 0);
	MchPic((o >= 0) ? "MCEQAH" : "MCEQA", B + 157, MCH_CR_SX + 434, MCH_CR_SY);
	str out = "";
	j = MCH_CR_SX + 494;
	if(o == DND_GEM_TRANSMUTE) {
		MchPicScaled("ORBBAKQ", B + 153, j, MCH_CR_SY + 60, 2.0);
		out = IsGreaterGem(GetPlayerInventoryItem(pnum, d.val[0] & DND_TRANSMUTE_GEMBOXMASK).item_subtype) ? "DND_RANDOM_GREATERGEM" : "DND_RANDOM_GEM";
	}
	else if(o == DND_RANDOM_TAGGED_ORB_TRANSMUTE) {
		MchPicScaled("ORBBAKQ", B + 153, j, MCH_CR_SY + 60, 2.0);
		out = "DND_RANDOM_TAGGED_ORB";
	}
	else if(o >= 0) {
		o = (o == DND_ELEMENTAL_TO_PRISMATIC) ? DND_ORB_PRISMATIC : o;
		MchPicScaled(GetItemImage(GetOrbItemImage(o)), B + 153, j, MCH_CR_SY + 60, 2.0);
		out = MchOrbName(o);
	}
	if(StrLen(out))
		MchText("MENUBDYS", StrParam(s:"\c[UIGold]", l:out), B + 188, j, MCH_CR_SY + 150, 0);
	MchText("MENUBODY", StrParam(s:"\c[UIGrey]", l:"DND_MCH_CR_COST", s:"  \c[UIGold]", s:MchMoney(DND_TRANSMUTE_COST)), B + 189, MCH_CR_MCX, 640, 0);
	MchCraftButton(B + 161, MCH_CR_MCX - 212, 700, "DND_MCH_CR_TRANSMUTE", boxid == MCH_CR_TRANSMUTE, o >= 0, 0);
	MchCraftButton(B + 163, MCH_CR_MCX + 12, 700, "DND_MCH_CR_EMPTY", boxid == MCH_CR_EMPTY, d.val[0] != -1 || d.val[1] != -1 || d.val[2] != -1, 1);
}

// One crafting page: its list, grid or recipe book, the middle column and the materials panel.
void DrawChromeCraft(int pnum, int page, int boxid) {
	DeleteTextRange(MCH_ID_CRAFT_FIRST, MCH_ID_CRAFT_LAST);
	MchCraft(page);
	MchCraftSyncSelection(pnum, page);
	// a card under the cursor is only for items: the panel says what a material is
	if((boxid < MCH_CR_ITEM || boxid >= MCH_CR_MAT) && PlayerCursorData.itemHovered != -1)
		ResetCursorHoverData();
	MchCraftMaterials(pnum, page, boxid);
	if(page == MENU_LOAD_CRAFTING_WEAPON)
		MchCraftWeapons(pnum, boxid);
	else if(page == MENU_LOAD_CRAFTING_INVENTORY)
		MchCraftInventory(pnum, boxid);
	else
		MchCraftTransmute(pnum, boxid);
	if(page != MENU_LOAD_CRAFTING_TRANSMUTING)
		MchCraftCard(pnum, page);
	str hint = "DND_MCH_HINT_CRT";
	if(page == MENU_LOAD_CRAFTING_WEAPON)
		hint = "DND_MCH_HINT_CRW";
	else if(page == MENU_LOAD_CRAFTING_INVENTORY)
		hint = "DND_MCH_HINT_CRI";
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:hint), MCH_ID_PAGETXT + 2, MCH_ROW_X, MCH_PANE_FOOT + 54, 1);
}

// Forward / back scroll whichever part the cursor is on: the list, the materials grid or the description.
bool MchCraftListening(int page) {
	mch_craft_T module& c = MchCraft(page);
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS), d = 0, cx = MchCursorX(), cy = MchCursorY(), v;
	bool r = MchSbarHover();
	v = MchScrollDrag(0, c.lscroll);
	r |= v != c.lscroll;
	c.lscroll = v;
	v = MchScrollDrag(1, c.mscroll);
	r |= v != c.mscroll;
	c.mscroll = v;
	v = MchScrollDrag(2, c.dscroll);
	r |= v != c.dscroll;
	c.dscroll = v;
	if(MchSbarGrabbed())
		return r;
	if(IsButtonHeld(bpress, BT_FORWARD))
		d = -1;
	else if(IsButtonHeld(bpress, BT_BACK))
		d = 1;
	if(!d) {
		c.scroll_tic = 0;
		return r;
	}
	if(c.scroll_tic-- > 0)
		return r;
	c.scroll_tic = 3;
	if(cx >= MCH_CR_PX) {
		if(cy >= 660) {
			v = Clamp_Between(c.dscroll + d, 0, c.dscroll_max);
			if(v == c.dscroll)
				return r;
			c.dscroll = v;
			return true;
		}
		v = Max(c.mscroll + d, 0);
		if(v == c.mscroll)
			return r;
		c.mscroll = v;
		return true;
	}
	if(cx < MCH_CR_MX && page != MENU_LOAD_CRAFTING_INVENTORY) {
		v = Max(c.lscroll + d, 0);
		if(v == c.lscroll)
			return r;
		c.lscroll = v;
		return true;
	}
	return r;
}

// Server: a material into the first empty chamber socket, as picking it then a socket did.
void MchTransmuteFill(int pnum, int idx) {
	auto d = GetTransmuteOrbData(pnum);
	int s, temp, ty;
	if(idx < 0 || idx >= MAX_INVENTORY_BOXES)
		return;
	for(s = 0; s < MAX_TRANSMUTE_BOXES && d.val[s] != -1; ++s);
	if(s == MAX_TRANSMUTE_BOXES)
		return;
	ty = GlobalItemStorage.PlayerInventoryList[pnum][idx].item_type & 0xFFFF;
	temp = GlobalItemStorage.PlayerInventoryList[pnum][idx].item_subtype;
	if(ty == DND_ITEM_GEM) {
		if(CheckResearchStatus(RES_GEMCRAFTING) != RES_DONE)
			return;
		temp = DND_TRANSMUTE_GEMSLOT | idx;
	}
	else if(ty != DND_ITEM_ORB)
		return;
	d.val[s] = temp;
	SendOwnerScript("DnD Transmute Orb Sync", pnum, pnum, s, temp);
	LocalAmbientSound("RPG/MenuChoose", 127);
}

// Server: a found recipe's orbs into the chamber, in order; transmuting checks they are carried.
void MchTransmuteRecipe(int pnum, int orb) {
	if(orb < 0 || orb >= DND_MAX_ORB_KINDS || !CheckOrbRecipe(orb))
		return;
	auto d = GetTransmuteOrbData(pnum);
	int s = 0, j, k, cid, cnt;
	ResetTransmutingData(pnum);
	for(j = 0; j < MAX_TRANSMUTE_BOXES; ++j) {
		cid = GetOrbRecipe(orb).components[j].id;
		cnt = GetOrbRecipe(orb).components[j].count;
		if(cid == -1)
			break;
		for(k = 0; k < cnt && s < MAX_TRANSMUTE_BOXES; ++k) {
			d.val[s] = cid;
			SendOwnerScript("DnD Transmute Orb Sync", pnum, pnum, s, cid);
			++s;
		}
	}
	LocalAmbientSound("RPG/MenuChoose", 127);
}

// ---------------------------------------------------------------- class select and character load

// Two carousels on the menu frame: the class select (classes) and the character load (save slots). The middle one
// stands large, its neighbours dimmed either side; perk cards below widen to the focused one. Motion eases out.
#define CSEL_ID 1440
#define CLOAD_ID 1560
#define CSEL_CX 720
#define CSEL_FLOOR 470
#define CSEL_STEP 440
#define CSEL_CARDX -158
#define CSEL_CARDY 680
#define CSEL_CARDH 270
#define CSEL_NARROW 196
#define CSEL_GAP 14
#define CSEL_WIDE 916
#define CSEL_RIGHT 32
#define CSEL_SLIDE_TICS 12
#define CSEL_CARD_TICS 10
#define CSEL_SELECT_X 1318
#define CSEL_PERKS 5

typedef struct {
	int cur;
	int count;
	int view;
	int view_from;
	int view_tic;
	int focus;
	int card_tic;
	int content_tic;
	int tic;
	int hot;
	int last_cx;
	int last_cy;
	int cw[CSEL_PERKS];
	int cw_from[CSEL_PERKS];
} csel_T;

csel_T module& GetCsel() {
	static csel_T s;
	return s;
}

csel_T module& GetCload() {
	static csel_T s;
	return s;
}

// the load screen's message line (load timer, waiting players), written by "DnD Character Select Animated"
typedef struct {
	str msg;
} cload_msg_T;

cload_msg_T module& GetCloadMsg() {
	static cload_msg_T m;
	return m;
}

// 0 to 1.0 eased out (cubic) after n of span tics
int CselEase(int n, int span) {
	if(n >= span)
		return 1.0;
	int t = 1.0 - FixedDiv(n << 16, span << 16);
	return 1.0 - FixedMul(FixedMul(t, t), t);
}

void CarouselStart(csel_T module& s, int cur, int count) {
	s.cur = cur;
	s.count = count;
	s.view = cur << 16;
	s.view_from = s.view;
	s.view_tic = -100;
	s.focus = 0;
	s.card_tic = -100;
	s.content_tic = -100;
	s.tic = 0;
	s.hot = 0;
	for(int i = 0; i < CSEL_PERKS; ++i) {
		s.cw[i] = i ? CSEL_NARROW : CSEL_WIDE;
		s.cw_from[i] = s.cw[i];
	}
	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF - 40.0;
	UpdateMenuZoom(true);
}

// where the carousel stands now, in items (fixed)
int CarouselView(csel_T module& s) {
	return s.view_from + FixedMul(s.view - s.view_from, CselEase(s.tic - s.view_tic, CSEL_SLIDE_TICS));
}

void CarouselSlide(csel_T module& s, int dir) {
	s.view_from = CarouselView(s);
	s.view += dir << 16;
	s.view_tic = s.tic;
	s.content_tic = s.tic;
	s.cur = ((s.view >> 16) % s.count + s.count) % s.count;
	LocalAmbientSound("RPG/MenuMove", 127);
}

// slides the short way round to item i
void CarouselGoTo(csel_T module& s, int i) {
	int d = (i - s.cur + s.count) % s.count;
	if(d > s.count / 2)
		d -= s.count;
	if(d)
		CarouselSlide(s, d);
}

int CarouselCardW(csel_T module& s, int i) {
	return s.cw_from[i] + (FixedMul((s.cw[i] - s.cw_from[i]) << 16, CselEase(s.tic - s.card_tic, CSEL_CARD_TICS)) >> 16);
}

void CarouselFocus(csel_T module& s, int f) {
	f = (f + CSEL_PERKS) % CSEL_PERKS;
	if(f == s.focus)
		return;
	for(int i = 0; i < CSEL_PERKS; ++i) {
		s.cw_from[i] = CarouselCardW(s, i);
		s.cw[i] = (i == f) ? CSEL_WIDE : CSEL_NARROW;
	}
	s.focus = f;
	s.card_tic = s.tic;
	LocalAmbientSound("RPG/MenuMove", 96);
}

int CarouselCardX(csel_T module& s, int k) {
	int x = CSEL_CARDX;
	for(int i = 0; i < k; ++i)
		x += CarouselCardW(s, i) + CSEL_GAP;
	return x;
}

// item i's distance from the view, wrapped to the nearer side (fixed)
int CarouselDist(csel_T module& s, int view, int i) {
	int r = (i << 16) - view, half = s.count << 15;
	while(r > half)
		r -= s.count << 16;
	while(r <= -half)
		r += s.count << 16;
	return r;
}

// 3 left arrow or neighbour, 4 right, 5 the middle one, 10 + k a perk card, 0 elsewhere
int CarouselHit(csel_T module& s, bool cards) {
	int cx = MchCursorX(), cy = MchCursorY();
	// each side: its chevron and the neighbour beside it, with room to spare around the chevron
	if(MchIn(cx, cy, CSEL_CX - 2 * CSEL_STEP + 120, 180, CSEL_STEP + 40, 400))
		return 3;
	if(MchIn(cx, cy, CSEL_CX + CSEL_STEP / 2 + 100, 180, CSEL_STEP + 40, 400))
		return 4;
	if(MchIn(cx, cy, CSEL_CX - 190, 180, 380, 400))
		return 5;
	if(cards)
		for(int k = 0; k < CSEL_PERKS; ++k)
			if(MchIn(cx, cy, CarouselCardX(s, k), CSEL_CARDY, CarouselCardW(s, k), CSEL_CARDH))
				return 10 + k;
	return 0;
}

// the cursor and the keys every carousel shares: left / right slide, forward / back and a moving cursor move the card focus
void CarouselInput(csel_T module& s, int bpress, int obpress, bool cards) {
	++s.tic;
	PlayerCursorData.posx = GetCursorPos(GetPlayerInput(-1, INPUT_YAW), MOUSE_INPUT_X, HUDMAX_X + MCH_CURSOR_OVER, HUDMAX_Y, -MCH_CURSOR_OVER << 16);
	PlayerCursorData.posy = GetCursorPos(GetPlayerInput(-1, INPUT_PITCH), MOUSE_INPUT_Y);
	int cx = MchCursorX(), cy = MchCursorY();
	s.hot = CarouselHit(s, cards);
	// a resting cursor leaves the keys in charge of the focus
	if((cx != s.last_cx || cy != s.last_cy) && s.hot >= 10)
		CarouselFocus(s, s.hot - 10);
	s.last_cx = cx;
	s.last_cy = cy;
	if(IsButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_LEFT)))
		CarouselSlide(s, -1);
	if(IsButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_RIGHT)))
		CarouselSlide(s, 1);
	if(cards && IsButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_FORWARD)))
		CarouselFocus(s, s.focus - 1);
	if(cards && IsButtonPressed(bpress, obpress, GetMovementButton(MVMT_BT_BACK)))
		CarouselFocus(s, s.focus + 1);
}

void MchTextA(str font, str txt, int id, int x, int y, int align, int alpha) {
	int fx = x << 16, fy = y << 16;
	if(align == 1)
		fx += 0.1;
	else if(align == 3) {
		fx += 0.1;
		fy += 0.1;
	}
	else if(align == 4) {
		fx += 0.4;
		fy += 0.1;
	}
	SetFont(font);
	HudMessage(s:txt; HUDMSG_PLAIN | HUDMSG_ALPHA, id, CR_WHITE, MenuX(fx), MenuY(fy), 0.0, alpha);
}

// a sprite k times larger (fixed), centred on cx and standing on by
void MchSprite(str pic, int id, int cx, int by, int k, int alpha) {
	MenuHudSize(FixedDiv(MCH_W << 16, k) >> 16, FixedDiv(MCH_H << 16, k) >> 16);
	SetFont(pic);
	HudMessage(s:"A"; HUDMSG_PLAIN | HUDMSG_ALPHA, id, CR_UNTRANSLATED, MenuX(FixedDiv(cx << 16, k) & MMASK), MenuY((FixedDiv(by << 16, k) & MMASK) + 0.2), 0.0, alpha);
	MenuHudSize(MCH_W, MCH_H);
}

// One carousel item at its distance from the view: a sprite (or text when pic is empty), and a name under neighbours.
void DrawCarouselItem(csel_T module& s, int view, int i, int base, str pic, str label) {
	int r = CarouselDist(s, view, i), ar = abs(r);
	if(ar >= 1.6) {
		DeleteText(base + i);
		DeleteText(base + 10 + i);
		return;
	}
	int k = 5.0 - Min(ar, 1.0);
	int a = (ar <= 1.0) ? 1.0 - FixedMul(ar, 0.65) : Max(0.35 - FixedMul(ar - 1.0, 0.6), 0);
	int x = CSEL_CX + (FixedMul(r, CSEL_STEP << 16) >> 16);
	if(StrLen(pic))
		MchSprite(pic, base + i, x, CSEL_FLOOR + 15 - FixedMul(Min(ar, 1.0), 20.0) / 65536, k, a);
	else
		MchTextA("MENUHEAD", StrParam(s:"\c[UIDim]", s:label), base + i, x, CSEL_FLOOR - 150, 0, a);
	if(ar > 0.5 && StrLen(pic))
		MchTextA("MENUTAB", StrParam(s:"\c[UIDim]", s:label), base + 10 + i, x, CSEL_FLOOR + 16, 0, Min(FixedMul(ar - 0.5, 2.0), a + 0.4));
	else
		DeleteText(base + 10 + i);
}

void DrawCarouselArrows(csel_T module& s, int base) {
	MchPic((s.hot == 3) ? "MCCHVL1" : "MCCHVL0", base, CSEL_CX - 2 * CSEL_STEP + 194, CSEL_FLOOR - 176);
	MchPic((s.hot == 4) ? "MCCHVR1" : "MCCHVR0", base + 1, CSEL_CX + 2 * CSEL_STEP - 266, CSEL_FLOOR - 176);
}

str CselLevelColour(int k) {
	switch(k) {
		case 0:
		return "\c[UICyan]";
		case 1:
		return "\c[UIGreen]";
		case 2:
		return "\c[UIGold]";
		case 3:
		return "\c[UIOrange]";
	}
	return "\c[UIRed]";
}

// The class's five perks as cards; widths ease toward their targets and the focused one's text shows as far as its
// width allows. lvl is a saved character's level (perks above it drawn locked), -1 for none. Lower ids draw in front.
void DrawPerkCards(csel_T module& s, int base, int cls, int alpha, int lvl) {
	str cprefix = StrParam(s:"CLASS", d:cls);
	int x = CSEL_CARDX;
	for(int k = 0; k < CSEL_PERKS; ++k) {
		int w = CarouselCardW(s, k), b = base + 8 * k;
		bool locked = lvl >= 0 && lvl < k * DND_PERK_REGULARTHRESHOLD;
		str st = (k == s.focus) ? "F" : "N";
		MenuClipRect(x, CSEL_CARDY, w - CSEL_RIGHT, CSEL_CARDH);
		MchPic(StrParam(s:"MCPCL", d:k, s:st), b + 5, x, CSEL_CARDY);
		MenuClipRect(0, 0, 0, 0, 0);
		MchPic(StrParam(s:"MCPCR", d:k, s:st), b + 4, x + w - CSEL_RIGHT, CSEL_CARDY);
		MchPic(StrParam(s:"MCLVC", d:k), b + 3, x + 16, CSEL_CARDY + 16);
		str lv = k ? StrParam(l:"DND_CSEL_LEVEL", s:" ", d:k * DND_PERK_REGULARTHRESHOLD) : StrParam(l:"CLASS_INNATE");
		MchText("MENUSUB", StrParam(s:CselLevelColour(k), s:lv), b, x + 58, CSEL_CARDY + 31, 0);
		str nm = StrParam(s:locked ? "\c[UIDim]" : "\c[UIGold]", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1 + k));
		if(w > (CSEL_NARROW + CSEL_WIDE) / 2) {
			MchTextA("MENUBODY", nm, b + 1, x + 116, CSEL_CARDY + 31, 1, alpha);
			MenuClipRect(x + 16, CSEL_CARDY + 56, w - 36, CSEL_CARDH - 70, CSEL_WIDE - 40);
			MchTextA("MENUBDYS", StrParam(s:locked ? "\c[UIGrey]" : "\c[UIWhite]", l:GetClassLabel(cprefix, DND_CLASS_LABEL_PERK1_DESC + k)), b + 2, x + 18, CSEL_CARDY + 62, 3, alpha);
			MenuClipRect(0, 0, 0, 0, 0);
		}
		else {
			MenuClipRect(x + 14, CSEL_CARDY + 56, w - 28, CSEL_CARDH - 70, CSEL_NARROW - 32);
			MchTextA("MENUBDYS", nm, b + 1, x + 16, CSEL_CARDY + 62, 3, alpha);
			MenuClipRect(0, 0, 0, 0, 0);
			DeleteText(b + 2);
		}
		x += w + CSEL_GAP;
	}
}

// ---- the class select

void CselStart(int cls) {
	CarouselStart(GetCsel(), cls, MAXPLAYERCLASSES);
}

void DrawClassSelect() {
	csel_T module& s = GetCsel();
	int i, view = CarouselView(s), hot = s.hot;
	str cprefix;
	if(UpdateMenuZoom(true))
		DeleteText(RPGMENUBACKGROUNDID);
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCFRAME", RPGMENUBACKGROUNDID, MCH_FX - 360, MCH_FY - 10);

	// title, Select and Close; each label sits on an id below its plate
	MchText("MENUHEAD", StrParam(s:"\c[UICyan]", l:"CLASS_SELECT"), CSEL_ID, MCH_ROW_X - 18, MCH_TAB_Y + 26, 1);
	MchPic((hot == 1) ? "MCTH0" : "MCTA0", CSEL_ID + 3, CSEL_SELECT_X, MCH_TAB_Y);
	MchText("MENUTAB", StrParam(s:(hot == 1) ? "\c[UIWhite]" : "\c[UIGold]", l:"DND_CSEL_SELECT"), CSEL_ID + 1, CSEL_SELECT_X + MCH_TAB_W / 2, MCH_TAB_Y + 26, 0);
	MchPic((hot == 2) ? "MCWH0" : "MCWO0", CSEL_ID + 4, MCH_CLOSE_X, MCH_TAB_Y);
	MchText("MENUSUB", StrParam(s:(hot == 2) ? "\c[UIWhite]" : "\c[UIGrey]", l:"DND_MCH_CLOSE"), CSEL_ID + 2, MCH_CLOSE_X + MCH_CLOSE_W / 2, MCH_TAB_Y + 26, 0);

	cprefix = StrParam(s:"CLASS", d:s.cur);
	MchText("MENUSUB", StrParam(s:"\c[UIGrey]", l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME), s:"   ", d:s.cur + 1, s:" / ", d:MAXPLAYERCLASSES),
		CSEL_ID + 5, MCH_ROW_X - 18, MCH_SUB_Y + 17, 1);
	for(i = 0; i < MAXPLAYERCLASSES; ++i)
		MchPic((i == s.cur) ? "MCPIP1" : "MCPIP0", CSEL_ID + 6 + i, CSEL_CX - 18 * MAXPLAYERCLASSES + 36 * i + 10, MCH_SUB_Y + 10);

	// the class in the middle is a button: under the cursor its floor turns gold and says so
	MchPic((hot == 5) ? "MCFLOORH" : "MCFLOOR", CSEL_ID + 40, CSEL_CX - 170, CSEL_FLOOR - 12);
	if(hot == 5)
		MchText("MENUSUB", StrParam(s:"\c[UIGold]", l:"DND_CSEL_CLICKPICK"), CSEL_ID + 46, CSEL_CX, CSEL_FLOOR + 30, 0);
	else
		DeleteText(CSEL_ID + 46);
	for(i = 0; i < MAXPLAYERCLASSES; ++i) {
		cprefix = StrParam(s:"CLASS", d:i);
		DrawCarouselItem(s, view, i, CSEL_ID + 20, StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_RIGHTIMG)), StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME)));
	}
	DrawCarouselArrows(s, CSEL_ID + 41);

	// the middle class's name and background fade in as it arrives
	int ca = 0.25 + FixedMul(0.75, CselEase(s.tic - s.content_tic, CSEL_SLIDE_TICS));
	cprefix = StrParam(s:"CLASS", d:s.cur);
	MchTextA("MENUHEAD", StrParam(s:"\c[UIGold]", l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME)), CSEL_ID + 43, CSEL_CX, CSEL_FLOOR + 66, 0, ca);
	MenuClipRect(CSEL_CX - 660, CSEL_FLOOR + 90, 1320, 100, 1320);
	MchTextA("MENUBODY", StrParam(s:"\c[UIWhite]", l:GetClassLabel(cprefix, DND_CLASS_LABEL_TEXT)), CSEL_ID + 44, CSEL_CX, CSEL_FLOOR + 92, 4, ca);
	MenuClipRect(0, 0, 0, 0, 0);

	DrawPerkCards(s, CSEL_ID + 50, s.cur, ca, -1);

	MchText("MENUSUB", StrParam(s:"\c[UIGrey]", k:"+moveleft", s:" / ", k:"+moveright", s:"  ", l:"DND_CSEL_CHOOSE", s:"      ", k:"+forward", s:" / ", k:"+back", s:"  ", l:"DND_CSEL_READ",
		s:"      ", k:"+use", s:"  ", l:"DND_CSEL_PICK", s:"      ", k:"+altattack", s:"  ", l:"DND_CSEL_CLOSEKEY"), CSEL_ID + 45, MCH_ROW_X - 18, MCH_STATUS_Y, 1);

	MenuHudSize(HUDMAX_X, HUDMAX_Y);
	DrawCursor();
}

void ClearClassSelect() {
	DeleteTextRange(CSEL_ID, CSEL_ID + 100);
	DeleteText(RPGMENUBACKGROUNDID);
	DeleteText(RPGMENUCURSORID);
	UpdateMenuZoom(false);
}

// One tic of the class select: returns 1 to pick the class, -1 to close.
int ClassSelectTic(int bpress, int obpress) {
	csel_T module& s = GetCsel();
	CarouselInput(s, bpress, obpress, true);
	int cx = MchCursorX(), cy = MchCursorY();
	if(MchIn(cx, cy, CSEL_SELECT_X, MCH_TAB_Y, MCH_TAB_W, MCH_TAB_H))
		s.hot = 1;
	else if(MchIn(cx, cy, MCH_CLOSE_X, MCH_TAB_Y, MCH_CLOSE_W, MCH_TAB_H))
		s.hot = 2;
	if(IsButtonPressed(bpress, obpress, BT_ALTATTACK))
		return -1;
	if(IsButtonPressed(bpress, obpress, BT_USE))
		return 1;
	if(IsButtonPressed(bpress, obpress, BT_ATTACK)) {
		// only Select and the class in the middle pick; a click elsewhere does nothing
		if(s.hot == 2)
			return -1;
		if(s.hot == 3)
			CarouselSlide(s, -1);
		else if(s.hot == 4)
			CarouselSlide(s, 1);
		else if(s.hot == 1 || s.hot == 5)
			return 1;
	}
	return 0;
}

// ---- the character load (database servers)

bool CloadHasChar(int slot) {
	return LoadedPlayerData[slot].classid != 0;
}

void CloadStart() {
	CarouselStart(GetCload(), 0, DND_MAX_CHARS);
}

// the slot whose details the server sent: slide to it
void CloadShow(int slot) {
	CarouselGoTo(GetCload(), slot);
}

// a slot's state for the floor, pip and status line: 0 none, 1 picked to load, 2 picked to be replaced by a new character
int CloadPick(int slot) {
	if(CheckInventory("DnD_SelectedCharmBox") == slot + 2)
		return 1;
	if(CheckInventory("DnD_SelectedInventoryBox") == slot + 2)
		return CloadHasChar(slot) ? 2 : 1;
	return 0;
}

// One tic of the character load's own input: 1 the Load button, 3 / 4 arrows, 5 the middle slot, 10 + k cards, 20 + i a pip.
int CloadTic(int bpress, int obpress) {
	csel_T module& s = GetCload();
	bool logged = CheckInventory("PlayerIsLoggedIn");
	CarouselInput(s, bpress, obpress, logged && CloadHasChar(s.cur));
	int cx = MchCursorX(), cy = MchCursorY(), i;
	if(MchIn(cx, cy, CSEL_SELECT_X, MCH_TAB_Y, MCH_TAB_W, MCH_TAB_H))
		s.hot = 1;
	for(i = 0; i < DND_MAX_CHARS && logged; ++i)
		if(MchIn(cx, cy, CSEL_CX - 18 * DND_MAX_CHARS + 36 * i + 2, MCH_SUB_Y + 2, 32, 32))
			s.hot = 20 + i;
	// fire on an arrow or a pip only moves the view; the box sent with it is none, as a click on empty space always was
	if(IsButtonPressed(bpress, obpress, BT_ATTACK)) {
		if(s.hot == 3)
			CarouselSlide(s, -1);
		else if(s.hot == 4)
			CarouselSlide(s, 1);
		else if(s.hot >= 20)
			CarouselGoTo(s, s.hot - 20);
	}
	return s.hot;
}

// the box the server knows for what the cursor is on: 1 Load, 2 + slot the slot in the middle once logged in
int CloadBox(int hot) {
	if(hot == 1)
		return MBOX_1;
	if(hot == 5 && CheckInventory("PlayerIsLoggedIn"))
		return MBOX_1 + 1 + GetCload().cur;
	return MAINBOX_NONE;
}

void DrawCharLoad(int boxid) {
	csel_T module& s = GetCload();
	int i, view = CarouselView(s), slot = s.cur, pick;
	bool logged = CheckInventory("PlayerIsLoggedIn");
	str cprefix, t;
	if(UpdateMenuZoom(true))
		DeleteText(RPGMENUBACKGROUNDID);
	MenuHudSize(MCH_W, MCH_H);
	MchPic("MCFRAME", RPGMENUBACKGROUNDID, MCH_FX - 360, MCH_FY - 10);

	// title and Load: live once a slot is picked, lit under the cursor
	MchText("MENUHEAD", StrParam(s:"\c[UICyan]", l:"CLASS_LOAD"), CLOAD_ID, MCH_ROW_X - 18, MCH_TAB_Y + 26, 1);
	bool ready = CheckInventory("DnD_SelectedCharmBox") || CheckInventory("DnD_SelectedInventoryBox");
	MchPic((boxid == MBOX_1) ? "MCTH0" : ready ? "MCTA0" : "MCTO0", CLOAD_ID + 3, CSEL_SELECT_X, MCH_TAB_Y);
	MchText("MENUTAB", StrParam(s:(boxid == MBOX_1) ? "\c[UIWhite]" : ready ? "\c[UIGold]" : "\c[UIDim]", l:"DND_CLOAD_LOAD"), CLOAD_ID + 1, CSEL_SELECT_X + MCH_TAB_W / 2, MCH_TAB_Y + 26, 0);

	// slot n of five, a pip each: gold the middle one, green picked to load, red picked to be replaced
	if(logged) {
		MchText("MENUSUB", StrParam(s:"\c[UIGrey]", l:"DND_MENU_SLOT", s:" ", d:slot, s:"   ", d:slot + 1, s:" / ", d:DND_MAX_CHARS), CLOAD_ID + 5, MCH_ROW_X - 18, MCH_SUB_Y + 17, 1);
		for(i = 0; i < DND_MAX_CHARS; ++i) {
			pick = CloadPick(i);
			t = (pick == 1) ? "MCPIPG" : (pick == 2) ? "MCPIPR" : (i == slot) ? "MCPIP1" : "MCPIP0";
			MchPic(t, CLOAD_ID + 6 + i, CSEL_CX - 18 * DND_MAX_CHARS + 36 * i + 10, MCH_SUB_Y + 10);
		}
	}
	else
		DeleteTextRange(CLOAD_ID + 5, CLOAD_ID + 5 + DND_MAX_CHARS);

	pick = logged ? CloadPick(slot) : 0;
	MchPic((pick == 1) ? "MCFLOORG" : (pick == 2) ? "MCFLOORR" : (boxid == MBOX_1 + 1 + slot) ? "MCFLOORH" : "MCFLOOR", CLOAD_ID + 40, CSEL_CX - 170, CSEL_FLOOR - 12);
	int ca = 0.25 + FixedMul(0.75, CselEase(s.tic - s.content_tic, CSEL_SLIDE_TICS));
	if(logged) {
		for(i = 0; i < DND_MAX_CHARS; ++i) {
			if(CloadHasChar(i)) {
				cprefix = StrParam(s:"CLASS", d:LoadedPlayerData[i].classid - 1);
				DrawCarouselItem(s, view, i, CLOAD_ID + 20, StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_RIGHTIMG)), StrParam(l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME)));
			}
			else
				DrawCarouselItem(s, view, i, CLOAD_ID + 20, "", StrParam(l:"DND_CLOAD_EMPTY"));
		}
		DrawCarouselArrows(s, CLOAD_ID + 41);
	}
	else
		DeleteTextRange(CLOAD_ID + 20, CLOAD_ID + 42);

	// the middle slot: what is saved there, and what Load would do with it
	if(logged && CloadHasChar(slot)) {
		cprefix = StrParam(s:"CLASS", d:LoadedPlayerData[slot].classid - 1);
		MchTextA("MENUHEAD", StrParam(s:"\c[UIGold]", l:GetClassLabel(cprefix, DND_CLASS_LABEL_NAME), s:"  \c[UIGrey]", l:"DND_STAT18", s:" \c[UIWhite]", d:LoadedPlayerData[slot].level),
			CLOAD_ID + 43, CSEL_CX, CSEL_FLOOR + 56, 0, ca);
		t = StrParam(s:"\c[UIGrey]", l:"DND_STAT16", s:" \c[UIWhite]", d:LoadedPlayerData[slot].exp, s:" / ", d:GetExpLimit_Level(LoadedPlayerData[slot].level),
			s:"      \c[UIGrey]", l:"DND_HEALTH", s:" \c[UIWhite]", d:LoadedPlayerData[slot].hp,
			s:"      \c[UIGrey]", l:"DND_STAT19", s:" \c[UIGold]", s:MchMoney(LoadedPlayerData[slot].credit),
			s:"      \c[UIGrey]", l:"DND_MENU_BUDGET", s:" \c[UIGold]$", d:LoadedPlayerData[slot].budget);
		MchTextA("MENUBODY", t, CLOAD_ID + 44, CSEL_CX, CSEL_FLOOR + 92, 0, ca);
		DrawPerkCards(s, CLOAD_ID + 50, LoadedPlayerData[slot].classid - 1, ca, LoadedPlayerData[slot].level);
	}
	else {
		MchTextA("MENUHEAD", StrParam(s:"\c[UIDim]", l:logged ? "DND_CLOAD_EMPTY" : "DND_CLOAD_NOTYET"), CLOAD_ID + 43, CSEL_CX, CSEL_FLOOR + 56, 0, ca);
		MchTextA("MENUBODY", StrParam(s:"\c[UIGrey]", l:logged ? "DND_CLOAD_NOCHAR" : "DND_CLOAD_LOGINWHY"), CLOAD_ID + 44, CSEL_CX, CSEL_FLOOR + 92, 0, ca);
		DeleteTextRange(CLOAD_ID + 50, CLOAD_ID + 50 + 8 * CSEL_PERKS);
	}
	// an empty slot's text already says what a pick does; the line only shows a pick there
	if(logged && (pick || CloadHasChar(slot))) {
		if(pick == 1)
			t = StrParam(s:"\c[UIGreen]", l:(CloadHasChar(slot) && CheckInventory("DnD_SelectedCharmBox") == slot + 2) ? "DND_CLOAD_PICKLOAD" : "DND_CLOAD_PICKNEW");
		else if(pick == 2)
			t = StrParam(s:"\c[UIRed]", l:"DND_CLOAD_PICKREPLACE");
		else
			t = StrParam(s:(boxid == MBOX_1 + 1 + slot) ? "\c[UIWhite]" : "\c[UIGrey]", l:"DND_CLOAD_HOWTO");
		MchText("MENUBDYS", t, CLOAD_ID + 46, CSEL_CX, CSEL_FLOOR + 124, 0);
	}
	else
		DeleteText(CLOAD_ID + 46);

	// the load timer and waiting players from "DnD Character Select Animated", above the cards
	SetFont("MENUBDYS");
	HudMessage(s:GetCloadMsg().msg; HUDMSG_PLAIN, CLOAD_ID + 47, CR_RED, MenuX((CSEL_CX << 16) + 0.4), MenuY(((CSEL_FLOOR + 142) << 16) + 0.1), 0.0);

	MchText("MENUSUB", StrParam(s:"\c[UIGrey]", l:"DND_CLOAD_HINT1", s:"      ", k:"+moveleft", s:" / ", k:"+moveright", s:"  ", l:"DND_CLOAD_HINT2",
		s:"      ", k:"+forward", s:" / ", k:"+back", s:"  ", l:"DND_CSEL_READ"), CLOAD_ID + 45, MCH_ROW_X - 18, MCH_STATUS_Y, 1);

	MenuHudSize(HUDMAX_X, HUDMAX_Y);
	DrawCursor();
}

void ClearCharLoad() {
	GetCloadMsg().msg = "";
	DeleteTextRange(CLOAD_ID, CLOAD_ID + 100);
	DeleteText(RPGMENUBACKGROUNDID);
	DeleteText(RPGMENUCURSORID);
	UpdateMenuZoom(false);
}

// ---------------------------------------------------------------- the sell confirm popup

int GetChromePopupBox() {
	int cx = MchCursorX(), cy = MchCursorY();
	if(MchIn(cx, cy, MCH_POP_YESX, MCH_POP_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_NAV_BASE + MCH_EL_POPYES;
	if(MchIn(cx, cy, MCH_POP_NOX, MCH_POP_BTNY, MCH_TAB_W, MCH_TAB_H))
		return MCH_NAV_BASE + MCH_EL_POPNO;
	return MAINBOX_NONE;
}

// What the server is sent as mainboxid: the left / right keys never carry a button, so they reach the page wherever
// the cursor rests; chrome navigation only rides a left click; popup buttons are never navigation.
int GetChromeSendMain(int mainboxid, int input) {
	if(input == DND_MENUINPUT_PREVBUTTON || input == DND_MENUINPUT_NEXTBUTTON)
		return MAINBOX_NONE;
	if(mainboxid == MCH_NAV_BASE + MCH_EL_POPYES || mainboxid == MCH_NAV_BASE + MCH_EL_POPNO)
		return MAINBOX_NONE;
	if(mainboxid >= MCH_NAV_BASE && input != DND_MENUINPUT_LCLICK)
		return MAINBOX_NONE;
	return mainboxid;
}

// Clicks the client answers itself: a research row picks the entry the keys would step to, and is not sent.
int HandleChromeClientInput(int page, int input, int mainboxid) {
	if(CheckInventory("DnD_SellConfirm"))
		return HandleChromeSellPopupInput(input, mainboxid);
	if(GetChromeKind(page) == MCH_KIND_RESLIST && mainboxid >= MCH_NAV_BASE + MCH_EL_ROW && mainboxid < MCH_NAV_BASE + MCH_EL_ROW + MENU_MAXRES_PERPAGE) {
		if(input == DND_MENUINPUT_LCLICK) {
			SetInventory("MenuPosX", mainboxid - MCH_NAV_BASE - MCH_EL_ROW);
			LocalAmbientSound("RPG/MenuChoose", 127);
		}
		return 0;
	}
	return input;
}

// Yes becomes the right click that confirmed a sale before; any other left click cancels, as it always did.
int HandleChromeSellPopupInput(int input, int mainboxid) {
	if(input != DND_MENUINPUT_LCLICK)
		return 0;
	// hidden from the click on, so it does not flash back before the server's answer lands
	GetMchState().popanswered = true;
	if(mainboxid == MCH_NAV_BASE + MCH_EL_POPYES)
		return DND_MENUINPUT_RCLICK;
	return DND_MENUINPUT_LCLICK;
}

bool ChromeSellPopupShown() {
	mch_state_T module& s = GetMchState();
	if(!CheckInventory("DnD_SellConfirm"))
		s.popanswered = false;
	return CheckInventory("DnD_SellConfirm") && !s.popanswered
		&& (CheckInventory("DnD_ShowSellPopup") || CheckInventory("DnD_PopupID") == POPUP_SCAVENGECONFIRM);
}

// The terminal confirm: header strip with the request's code, the question as console lines, credits in green.
void DrawChromeSellPopup(int mainboxid) {
	int item = CheckInventory("DnD_PopupID");
	MenuHudSize(MCH_W, MCH_H);
	MchPicScaled("MCDIM", MCH_ID_POPIMG + 3, MCH_W / 2, MCH_H / 2, 120.0);
	MchPic("MCTERMC", MCH_ID_POPIMG + 2, MCH_POP_X - MCH_TERM_M, MCH_POP_Y - MCH_TERM_M);
	bool salvage = IsChromeCraftPage(CheckInventory("MenuOption")) || (!CheckInventory("DnD_ShowSellPopup") && item == POPUP_SCAVENGECONFIRM);
	str body;
	if(salvage)
		body = StrParam(s:"\c[UIGold]> ", l:"DND_MCH_CR_SALVASK", s:" \c[UIGreen]", s:MchMoney(CheckInventory("DnD_ItemPriceTemp")), s:"\c[UIGold]?\n\c[UIGold]> ",
			l:"DND_DISASS_CHANCE", s:" ", d:CheckInventory("DnD_ItemCursorMsg") >> 16, s:"%\n\c[UIGold]> _");
	else
		body = StrParam(s:"\c[UIGold]> ", l:"DND_MCH_SELLASK", s:" ", l:GetWeaponTag(ShopTableIdToWeaponTableId(item)), s:"?\n\c[UIGold]> ",
			l:"DND_MCH_SELLGET", s:" \c[UIGreen]", s:MchMoney(GetShopPrice(item) / 2), s:"\n\c[UIGold]> _");
	MchText("MENUSUB", StrParam(s:"\c[UITermInk]UAC // ", l:salvage ? "DND_MCH_CR_SALVTITLE" : "DND_MCH_SELLTITLE"), MCH_ID_POPTXT, MCH_POP_X + 18, MCH_POP_Y + 21, 1);
	MchText("MENUSUB", StrParam(s:"\c[UITermInk]Q-", d:item), MCH_ID_POPTXT + 1, MCH_POP_X + MCH_POP_W - 18, MCH_POP_Y + 21, 2);
	MenuClipRect(MCH_POP_X + 24, MCH_POP_Y + 50, MCH_POP_W - 48, 160, MCH_POP_W - 56);
	MchText("MENUBODY", body, MCH_ID_POPTXT + 2, MCH_POP_X + 28, MCH_POP_Y + 62, 3);
	MenuClipRect(0, 0, 0, 0, 0);

	bool yes = mainboxid == MCH_NAV_BASE + MCH_EL_POPYES, no = mainboxid == MCH_NAV_BASE + MCH_EL_POPNO;
	MchPic(yes ? "MCTBTN1" : "MCTBTN0", MCH_ID_POPIMG, MCH_POP_YESX, MCH_POP_BTNY);
	MchText("MENUTAB", StrParam(s:yes ? "\c[UIWhite]" : "\c[UIGold]", s:"[ ", l:salvage ? "DND_MCH_CR_SALVAGE" : "DND_MCH_SELLYES", s:" ]"), MCH_ID_POPTXT + 3, MCH_POP_YESX + MCH_TAB_W / 2, MCH_POP_BTNY + 26, 0);
	MchPic(no ? "MCTBTN1" : "MCTBTN0", MCH_ID_POPIMG + 1, MCH_POP_NOX, MCH_POP_BTNY);
	MchText("MENUTAB", StrParam(s:no ? "\c[UIWhite]" : "\c[UIOrange]", s:"[ ", l:"DND_MCH_CANCEL", s:" ]"), MCH_ID_POPTXT + 4, MCH_POP_NOX + MCH_TAB_W / 2, MCH_POP_BTNY + 26, 0);
}

// A terminal message card: an error fades out after a second as the old box did, a notice stays until cleared.
void MchTermMessage(str head, str code, str body, bool err) {
	int x = MCH_POP_X, y = MCH_POP_Y + 45, hold = err ? 1.0 : 0.0, fade = err ? 1.0 : 0.0, mode = err ? HUDMSG_FADEOUT : HUDMSG_PLAIN;
	MenuHudSize(FixedDiv(MCH_W << 16, 120.0) >> 16, FixedDiv(MCH_H << 16, 120.0) >> 16);
	SetFont("MCDIM");
	HudMessage(s:"A"; mode | HUDMSG_ALPHA, MCH_ID_TERMDIM, CR_UNTRANSLATED, MenuX(FixedDiv((MCH_W / 2) << 16, 120.0) & MMASK), MenuY(FixedDiv((MCH_H / 2) << 16, 120.0) & MMASK), hold, fade, err ? 0.6 : 1.0);
	MenuHudSize(MCH_W, MCH_H);
	SetFont(err ? "MCTERME" : "MCTERMC");
	HudMessage(s:"A"; mode, MCH_ID_TERMCARD, CR_UNTRANSLATED, MenuX(((x - MCH_TERM_M) << 16) + 0.1), MenuY(((y - MCH_TERM_M) << 16) + 0.1), hold, fade);
	SetFont("MENUSUB");
	HudMessage(s:"\c[UITermInk]UAC // ", s:head; mode, MCH_ID_TERMTXT, CR_WHITE, MenuX(((x + 18) << 16) + 0.1), MenuY((y + 21) << 16), hold, fade);
	HudMessage(s:"\c[UITermInk]", s:code; mode, MCH_ID_TERMTXT + 1, CR_WHITE, MenuX(((x + MCH_POP_W - 18) << 16) + 0.2), MenuY((y + 21) << 16), hold, fade);
	SetFont("MENUBODY");
	MenuClipRect(x + 24, y + 50, MCH_POP_W - 48, 150, MCH_POP_W - 56);
	HudMessage(s:err ? "\c[UITermRed]> " : "\c[UIGold]> ", s:body, s:err ? "\n\c[UITermRed]> _" : "\n\c[UIGold]> _"; mode, MCH_ID_TERMTXT + 2, CR_WHITE, MenuX(((x + 28) << 16) + 0.1), MenuY(((y + 62) << 16) + 0.1), hold, fade);
	MenuClipRect(0, 0, 0, 0, 0);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

void ClearTermMessage() {
	DeleteTextRange(MCH_ID_TERMTXT, MCH_ID_TERMCARD);
	DeleteText(MCH_ID_TERMDIM);
}

// ---------------------------------------------------------------- the perk pages

#define MCH_PERK_LISTBOT 880
#define MCH_PERK_CATGAP 16
#define MCH_PERK_ROWS 10
#define MCH_PERK_INDENT 56
#define MCH_PERK_BODYTOP (MCH_PANE_Y + 120)
#define MCH_PERK_COLS 80

// whether the list fits from entry first on, category gaps included
bool MchPerkListFits(perk_arch_list_T module& list, int first) {
	int y = MCH_ROW_Y, n = 0;
	for(int i = first; i < list.count; ++i) {
		if(list.newcat[i] && i > first)
			y += MCH_PERK_CATGAP;
		if(y + MCH_ROW_H > MCH_PERK_LISTBOT || ++n > MCH_PERK_ROWS)
			return false;
		y += MCH_ROW_STEP;
	}
	return true;
}

// whether another child at depth lv follows entry i under the same parent
bool MchPerkHasLaterSibling(perk_arch_list_T module& list, int i, int lv) {
	for(int j = i + 1; j < list.count; ++j) {
		if(list.newcat[j] || list.depth[j] < lv)
			return false;
		if(list.depth[j] == lv)
			return true;
	}
	return false;
}

int MchPerkScrollMax(perk_arch_list_T module& list) {
	int first = 0;
	while(first < list.count && !MchPerkListFits(list, first))
		++first;
	return first;
}

str MchPerkRankCol(int pts, int mx) {
	if(pts >= mx)
		return "\c[UIGreen]";
	if(pts)
		return "\c[UIGold]";
	return "\c[UIGrey]";
}

// the perks of an archetype that hold points, one per line
str MchPerkArchSummary(int pnum, int arch) {
	perk_arch_list_T module& list = GetPerkArchList(arch);
	str out = "";
	for(int i = 0; i < list.count; ++i) {
		int pk = list.perk[i], pts = GetPerkPoints(pnum, pk);
		if(pts)
			out = StrParam(s:out, s:"\c[UIWhite]", l:GetPerkLangKey(pk), s:"  ", s:MchPerkRankCol(pts, PerkTable[pk].max_points), d:pts, s:" / ", d:PerkTable[pk].max_points, s:"\n");
	}
	if(!StrLen(out))
		return StrParam(s:"\c[UIDim]", l:"DND_MCH_PERK_NONE");
	return out;
}

void MchPerkArchPane(int pnum, int arch) {
	perk_arch_list_T module& list = GetPerkArchList(arch);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:GetPerkArchLangKey(arch)), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MENU_PERKSSPENT", s:" \c[UIWhite]", d:GetPerkSpentIn(pnum, arch), s:" / ", d:list.maxpoints,
		s:"      \c[UIGrey]", l:"DND_MCH_PERK_AVAIL", s:" \c[UIGold]", d:PlayerModData[pnum].unspent), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);
	MenuClipRect(MCH_PANE_IX, MCH_PERK_BODYTOP, MCH_PANE_TEXTW, MCH_PANE_FOOT - 12 - MCH_PERK_BODYTOP, MCH_PANE_TEXTW);
	MchText("MENUBDYS", MchPerkArchSummary(pnum, arch), MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PERK_BODYTOP, 3);
	MenuClipRect(0, 0, 0, 0, 0);
}

void DrawChromePerks(int pnum, int page, int boxid, menu_pane_T module& p) {
	EnsurePerkArchLists();
	mch_state_T module& s = GetMchState();
	if(s.page != page) {
		s.page = page;
		s.row = 0;
		s.scroll = 0;
		s.scroll_max = 0;
		s.pscroll = 0;
		s.pscroll_max = 0;
	}
	MchPic("MCPANE", MCH_ID_PAGEIMG + 5, MCH_PANE_X, MCH_PANE_Y);
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_PERK_AVAIL", s:"  \c[UIGold]", d:PlayerModData[pnum].unspent), MCH_ID_PAGETXT + 1, MCH_PANE_X + 976, 221, 2);
	if(GetChromeKind(page) == MCH_KIND_PERKTREE) {
		DrawChromePerkTree(pnum, page - MENU_PERKTREE_FIRST, boxid, p);
		return;
	}

	// the archetypes, then the dash toggle below a gap
	int i, y;
	if(boxid != MAINBOX_NONE && boxid <= PERK_ARCH_COUNT + 1)
		s.row = boxid - 1;
	for(i = 0; i <= PERK_ARCH_COUNT; ++i) {
		y = GetChromeRowY(page, i);
		bool empty = i < PERK_ARCH_COUNT && !GetPerkArchList(i).count;
		MchPic(StrParam(s:"MCROW", s:(boxid == i + 1) ? "S" : empty ? "L" : "N"), MCH_ID_ROWIMG + 12 + i, MCH_ROW_X - 8, y - 8);
		if(i == PERK_ARCH_COUNT) {
			MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MENU_DASH"), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
			if(CheckInventory("DashDisabled"))
				MchText("MENUBODY", StrParam(s:"\c[UIRed]", l:"DND_MCH_OFF"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
			else
				MchText("MENUBODY", StrParam(s:"\c[UIGreen]", l:"DND_MCH_ON"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
			continue;
		}
		MchText("MENUBODY", StrParam(s:empty ? "\c[UIDim]" : "\c[UIWhite]", l:GetPerkArchLangKey(i)), MCH_ID_ROWTXT + 3 * i, MCH_ROW_X + 30, y + 27, 1);
		if(empty)
			MchText("MENUBODY", StrParam(s:"\c[UIDim]", l:"DND_MENU_PERK_EMPTY"), MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		else {
			int spent = GetPerkSpentIn(pnum, i);
			MchText("MENUBODY", StrParam(s:spent ? "\c[UIGold]" : "\c[UIGrey]", d:spent, s:" / ", d:GetPerkArchList(i).maxpoints),
				MCH_ID_ROWTXT + 3 * i + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		}
	}

	i = Clamp_Between(s.row, 0, PERK_ARCH_COUNT);
	if(i == PERK_ARCH_COUNT) {
		MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MENU_DASH"), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);
		MenuClipRect(MCH_PANE_IX, MCH_PANE_Y + 82, MCH_PANE_TEXTW, 480, MCH_PANE_TEXTW);
		MchText("MENUBODY", StrParam(s:"\c[UIWhite]", l:"DND_MCH_DASHDESC"), MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 84, 3);
		MenuClipRect(0, 0, 0, 0, 0);
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_TOGGLE"), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}
	MchPerkArchPane(pnum, i);
	MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_OPEN"), MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// Rebuilds the pane each redraw: entry n is box n (off the list it gets an empty rect), the back arrow last, parked.
void DrawChromePerkTree(int pnum, int arch, int boxid, menu_pane_T module& p) {
	perk_arch_list_T module& list = GetPerkArchList(arch);
	perk_scroll_T module& ps = GetPerkScroll();
	mch_state_T module& s = GetMchState();
	if(ps.shown_arch != arch) {
		ps.shown_arch = arch;
		ps.shown_perk = -1;
		ps.pinned = false;
	}
	s.scroll_max = MchPerkScrollMax(list);
	s.scroll = Clamp_Between(s.scroll, 0, s.scroll_max);

	bool blocked = CheckInventory("DnD_HighlightBlocker"), full = false;
	// the hovered plate's bottom edge: the next row's lines reach up into it and would show through its fill
	int plate_bot = -1;
	int i, y = MCH_ROW_Y, n = 0, hovered = -1, last = -1, conn = 0, cats = 0;
	ResetPane(p);
	for(i = 0; i < list.count; ++i) {
		if(i < s.scroll || full) {
			AddBoxToPane_Points(p, -1, -1, -1, -1);
			continue;
		}
		if(list.newcat[i] && i > s.scroll) {
			y += MCH_PERK_CATGAP;
			if(cats < 4 && y + MCH_ROW_H <= MCH_PERK_LISTBOT) {
				MenuClipRect(MCH_ROW_X, y - 14, MCH_ROW_W, 2);
				MchPic("MCDIV", MCH_ID_PAGEIMG + 1 + cats, MCH_ROW_X, y - 14);
				MenuClipRect(0, 0, 0, 0, 0);
				++cats;
			}
		}
		if(y + MCH_ROW_H > MCH_PERK_LISTBOT || n >= MCH_PERK_ROWS) {
			full = true;
			AddBoxToPane_Points(p, -1, -1, -1, -1);
			continue;
		}
		MchAddBox(p, MCH_ROW_X, y, MCH_ROW_W, MCH_ROW_H);

		int pk = list.perk[i], pts = GetPerkPoints(pnum, pk), mx = PerkTable[pk].max_points, blocker = GetPerkSpendBlocker(pnum, pk);
		bool locked = !pts && (blocker == PERKBLOCK_THRESHOLD || blocker == PERKBLOCK_REQUIRES);
		bool hot = boxid == i + 1 && !blocked;
		if(hot)
			hovered = i;
		// a plain list; the hovered row's plate starts at its name, so the arrow points into its notch
		int x = MCH_ROW_X + 36 + MCH_PERK_INDENT * list.depth[i];
		// the plate takes the lowest id so the tree lines of the rows around it pass behind it
		if(hot)
			MchPic(StrParam(s:"MCRWS", d:Min(list.depth[i], 5)), MCH_ID_PANEIMG, x - 30, y - 8);
		// tree lines: trunks of ancestors that still have children below, then this row's elbow or tee
		for(int lv = 1; lv <= list.depth[i]; ++lv) {
			bool more = MchPerkHasLaterSibling(list, i, lv);
			str pic = "MCTRV";
			if(lv == list.depth[i])
				pic = StrParam(s:more ? "MCTRT" : "MCTRL", d:pts || IsPerkRequirementMet(pnum, pk));
			else if(!more)
				continue;
			int lx = MCH_ROW_X + 34 + MCH_PERK_INDENT * (lv - 1);
			if(plate_bot > y - 18)
				MenuClipRect(lx, plate_bot, 38, y + 54 - plate_bot);
			if(conn < 21)
				MchPic(pic, MCH_ID_PANEIMG + 1 + conn, lx, y - 18);
			else if(conn < 43)
				MchPic(pic, MCH_ID_ROWIMG + conn - 21, lx, y - 18);
			MenuClipRect(0, 0, 0, 0, 0);
			++conn;
		}
		// dim text would vanish on the highlight, so a hovered locked row reads white
		bool dim = locked && !hot;
		MchText("MENUBODY", StrParam(s:(pts >= mx) ? "\c[UIGreen]" : dim ? "\c[UIDim]" : "\c[UIWhite]", l:GetPerkLangKey(pk)), MCH_ID_ROWTXT + 3 * n, x, y + 27, 1);
		MchText("MENUBODY", StrParam(s:dim ? "\c[UIDim]" : locked ? "\c[UIWhite]" : MchPerkRankCol(pts, mx), d:pts, s:" / ", d:mx), MCH_ID_ROWTXT + 3 * n + 1, MCH_ROW_X + MCH_ROW_W - 20, y + 27, 2);
		last = i;
		plate_bot = hot ? y + 62 : -1;
		y += MCH_ROW_STEP;
		++n;
	}
	MchAddBox(p, -5000, -5000, 1, 1);

	MchScrollBar(0, MCH_ID_PANEIMG + 24, MCH_ROW_X + MCH_ROW_W + 10, MCH_ROW_Y, 620, s.scroll, s.scroll_max);
	if(s.scroll_max) {
		MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_SHOWING", s:" ", d:s.scroll + 1, s:"-", d:last + 1, s:" / ", d:list.count, s:"    \c[UIDim]", l:"DND_MCH_SCROLLHINT"),
			MCH_ID_PAGETXT + 2, MCH_ROW_X + MCH_ROW_W / 2, MCH_PAGER_Y + 26, 0);
	}

	// the pane keeps the last perk hovered so the cursor can travel to it; a pin holds it
	if(!ps.pinned && hovered != -1 && ps.shown_perk != list.perk[hovered]) {
		ps.shown_perk = list.perk[hovered];
		s.pscroll = 0;
	}
	if(ps.shown_perk == -1) {
		MchPerkArchPane(pnum, arch);
		s.pscroll_max = 0;
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_PERK_PICK"), MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
		return;
	}
	DrawChromePerkPane(pnum, ps.shown_perk, ps.pinned);
}

void DrawChromePerkPane(int pnum, int pk, bool pinned) {
	mch_state_T module& s = GetMchState();
	int pts = GetPerkPoints(pnum, pk), mx = PerkTable[pk].max_points, blocker = GetPerkSpendBlocker(pnum, pk);
	str key = GetPerkLangKey(pk), col = "\c[UIWhite]", status = "";
	if(pts >= mx)
		col = "\c[UIGreen]";
	else if(!pts && (blocker == PERKBLOCK_THRESHOLD || blocker == PERKBLOCK_REQUIRES))
		col = "\c[UIDim]";
	MchText("MENUHEAD", StrParam(s:col, l:key), MCH_ID_PANETXT, MCH_PANE_IX, MCH_PANE_Y + 26, 3);

	if(blocker == PERKBLOCK_MAXED)
		status = StrParam(s:"\c[UIGreen]", l:"DND_MENU_PERK_MAXED");
	else if(blocker == PERKBLOCK_THRESHOLD)
		status = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_NEEDSPENT", s:": ", d:PerkTable[pk].threshold);
	else if(blocker == PERKBLOCK_REQUIRES)
		status = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_LOCKED");
	else if(blocker == PERKBLOCK_NOPOINTS)
		status = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_NOPOINTS");
	MchText("MENUBDYS", StrParam(s:"\c[UIGrey]", l:"DND_MCH_PERK_RANK", s:" ", s:MchPerkRankCol(pts, mx), d:pts, s:" / ", d:mx, s:"      ", s:status),
		MCH_ID_PANETXT + 1, MCH_PANE_IX, MCH_PANE_Y + 78, 3);

	str body = StrParam(s:"\c[UIWhite]", l:StrParam(s:key, s:"_DESC"));
	if(mx > 1)
		body = StrParam(s:body, s:"\n\n\c[UIGold]", l:"DND_MENU_PERK_PERPOINT", s:": \c[UIWhite]", l:StrParam(s:key, s:"_PER"));
	if(PerkTable[pk].req_kind != PERK_REQ_NONE)
		body = StrParam(s:body, s:"\n\n\c[UIGold]", l:"DND_MENU_PERK_REQUIRES", s:": \c[UIWhite]", l:StrParam(s:key, s:"_REQ"));
	int view = (MCH_PANE_FOOT - 12 - MCH_PERK_BODYTOP) / MCH_STAT_LINEH;
	s.pscroll_max = Max(MchCountLines(body, MCH_PERK_COLS) - view, 0);
	s.pscroll = Clamp_Between(s.pscroll, 0, s.pscroll_max);
	MenuClipRect(MCH_PANE_IX, MCH_PERK_BODYTOP, MCH_PANE_TEXTW - 24, view * MCH_STAT_LINEH, MCH_PANE_TEXTW - 24);
	MchText("MENUBDYS", body, MCH_ID_PANETXT + 2, MCH_PANE_IX, MCH_PERK_BODYTOP - MCH_STAT_LINEH * s.pscroll, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MchScrollBar(1, MCH_ID_PANEIMG + 22, MCH_PANE_X + 954, MCH_PERK_BODYTOP, view * MCH_STAT_LINEH, s.pscroll, s.pscroll_max);

	str refund = "";
	int rs = GetPerkRefundStatus(pnum, pk);
	if(rs == PERKREFUND_NONE)
		refund = StrParam(s:"\c[UIGrey]", l:"DND_MCH_PERK_REFUNDCOST", s:" \c[UIGold]", s:MchMoney(GetPerkRespecCost(pnum, pk)));
	else if(rs == PERKREFUND_LOCKS)
		refund = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_REFUNDLOCKS", s:" \c[UIWhite]", l:GetPerkLangKey(GetPerkRefundBlocker(pnum, pk)));
	else if(rs == PERKREFUND_POOR)
		refund = StrParam(s:"\c[UIRed]", l:"DND_MENU_PERK_REFUNDPOOR", s:" (", s:MchMoney(GetPerkRespecCost(pnum, pk)), s:")");
	MchText("MENUBDYS", refund, MCH_ID_PANETXT + 3, MCH_PANE_IX, MCH_PANE_FOOT + 18, 1);
	if(pinned)
		MchText("MENUSUB", StrParam(s:"\c[UICyan]", l:"DND_MENU_PERK_PINNED1", s:"  \c[UIDim]", l:"DND_MCH_PERK_UNPIN"), MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
	else
		MchText("MENUSUB", StrParam(s:"\c[UIDim]", l:"DND_MCH_HINT_PERK", s:"    ", l:"DND_MENU_PERK_PINHELP1", s:" \c[UIGrey]", k:"+jump", s:"\c[UIDim] ", l:"DND_MCH_PERK_PINHINT"),
			MCH_ID_PANETXT + 4, MCH_PANE_IX, MCH_PANE_FOOT + 54, 1);
}

// Forward / back scroll a redesigned page's long text, a line every few tics while held.
bool HandleChromeListening(int page, int boxid) {
	int k = GetChromeKind(page);
	if(k == MCH_KIND_CRAFT)
		return MchCraftListening(page);
	if(k != MCH_KIND_STATS && k != MCH_KIND_PERKTREE && k != MCH_KIND_RESLIST && k != MCH_KIND_CODEX && !IsChromeShopList(page))
		return false;
	mch_state_T module& s = GetMchState();
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS), d = 0;
	bool r = MchSbarHover();
	int v = MchScrollDrag(0, s.scroll);
	r |= v != s.scroll;
	s.scroll = v;
	v = MchScrollDrag(1, s.pscroll);
	r |= v != s.pscroll;
	s.pscroll = v;
	if(MchSbarGrabbed())
		return r;
	if(IsButtonHeld(bpress, BT_FORWARD))
		d = -1;
	else if(IsButtonHeld(bpress, BT_BACK))
		d = 1;
	if(!d) {
		s.scroll_tic = 0;
		return r;
	}
	if(s.scroll_tic-- > 0)
		return r;
	s.scroll_tic = 2;
	// a perk tree scrolls whichever side the cursor is on: the list, or the pane's description
	if(k == MCH_KIND_CODEX || (k == MCH_KIND_PERKTREE && MchCursorX() >= MCH_PANE_X)) {
		int m = Clamp_Between(s.pscroll + d, 0, s.pscroll_max);
		if(m == s.pscroll)
			return r;
		s.pscroll = m;
		return true;
	}
	int n = Clamp_Between(s.scroll + d, 0, s.scroll_max);
	if(n == s.scroll)
		return r;
	s.scroll = n;
	return true;
}

// ---------------------------------------------------------------- menu scripts

/*
- Main idea here is simple. There are two main scripts: One draws the menu CLIENTSIDE, the other receives button info and stuff from client and vital info from server.
- There are A TON of helper functions to formulate things. Getters are all over the place.
- Everything in the shop has an ID associated with it.
- Everything is flexible and can be customized very easily (!!!). The essential code for input listeners etc are provided, and can be changed at will depending on the menu pages to achieve
different behavior.

============= ADDING NEW WEAPONS TO SHOP =============

1) Add it to DnD_Weapons.h
2) Add constant to DnD_MenuConstants.h, modify begin/last macros if position of them has changed
3) Add price, shop menu labels, shop menu helper text, research requirements (if any) and WeaponDrawInfo to DnD_MenuTables.h
4) Add weapon tip to DnD.h
5) Increment box count of whichever slot page it belongs to in LoadPane
6) Done!
*/

// Shows the menu
Script 900 (int opt, int showInv) NET {
	// dont let player cheese the petrify with this!
	if(CheckInventory("IsBlinded"))
		Terminate;

	int pnum = PlayerNumber();
	if(CheckInventory("DnD_StatListOpen")) {
		ClearMenuLeftovers(pnum);
		// a click that closed the menu is still held: stay frozen until it lets go, or the weapon fires on it
		if(GetPlayerInput(-1, INPUT_BUTTONS) & (BT_ATTACK | BT_ALTATTACK)) {
			SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
			ACS_NamedExecuteAlways("DnD Menu Close Grace", 0);
		}
		ResetSelectedDungeonInformation(pnum);
		DecideAccessories();
		//SetInventory("MenuPosY", 5);
		LocalAmbientSound("RPG/MenuClose", 127);
		
		int tradee = GetTradee();
		// cancel out of a trade if there is one
		if(tradee != -1) {
			CancelTradeRoutine(tradee);
		}
	}
	else {
		// a little extra check for singleplayer case
		if(CheckInventory("DnD_IntermissionState") || !HasChosenClass())
			Terminate;

		if
		(
			GameType() == GAME_SINGLE_PLAYER ||
			(!PlayerIsSpectator(pnum) && GetGameModeState() == GAMESTATE_INPROGRESS && !CheckInventory("CanLoad") && !CheckInventory("ParsingData"))
		)
		{
			if(CheckInventory("ShowingWeaponWheel")) {
				TakeInventory("ShowingWeaponWheel", 1);
				ACS_NamedExecuteWithResult("DnD Create Weapon Wheel", pnum, 1);
			}
			CloseSkillWheel(pnum);

			// No full resync on open: it was 100+ packets a time and "DnD Sync Check" found nothing for it
			// to fix. It is the manual "DnD Resync" now.
			SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
			GiveInventory("P_Frozen", 1);
			GiveInventory("DnD_StatListOpen", 1);

			if(!opt)
				SetInventory("MenuOption", MENU_MAIN);
			else
				SetInventory("MenuOption", opt);

			if(showInv == 1)
				GiveInventory("DnD_InventoryView", 1);
			else if(showInv == 2)
				GiveInventory("DnD_StashView", 1);

			SetInventory("ShowingMenu", 1);
			TakeInventory("DnD_CantSalvage", 1);
			TakeInventory("ShowingPrompt", 1);
			LocalAmbientSound("RPG/MenuOpen", 127);
			ClearPlayerInput(pnum, true);
			FlushStack(pnum);

			ResetTransmutingData(pnum);
			ResetDungeonSelectData(pnum);
			
			ACS_NamedExecuteWithResult("DnD Sync Player Exp", pnum, GetPlayerExp(pnum));
			SendOwnerScript("DnD Menu Input Loop", PlayerNumber());
			ACS_NamedExecuteAlways("DND Menu Icon Spawner", 0);
		}
		else {
			SetHudSize(800, 600, 1);
			HudMessage(l:"DND_MENU_NOTINGAME"; HUDMSG_FADEINOUT, MENUUSEID, CR_GOLD, 400.4, 2.1, 4.0, 1.0, 1.0);
		}
	}
}

// lifts the freeze once fire and alt fire are released, a second at most
Script "DnD Menu Close Grace" (void) {
	int t = 0;
	do {
		Delay(const:1);
		++t;
	} while(t < TICRATE && (GetPlayerInput(-1, INPUT_BUTTONS) & (BT_ATTACK | BT_ALTATTACK)));
	if(!CheckInventory("ShowingMenu") && !CheckInventory("P_Frozen"))
		SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
}

Script "DnD Selected Dungeon Sync" (int pnum, int val) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum || GameType() == GAME_SINGLE_PLAYER)
		Terminate;

	auto d = GetSelectedDungeonData(pnum);
	d.id = val;
	SetResultValue(0);
}

Script "DnD Transmute Orb Sync" (int pnum, int id, int val) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum || GameType() == GAME_SINGLE_PLAYER)
		Terminate;

	auto d = GetTransmuteOrbData(pnum);
	d.val[id] = val;
	SetResultValue(0);
}

Script "DnD Menu Reset on Enter" ENTER {
	int pnum = PlayerNumber();
	int ch_factor = (100 + PlayerModData[pnum].vals[PSTAT_SHOPSTOCK_INCREASE]), temp, ammo_bonus = GetAmmoCapIncrease(pnum);
	for(int j = 0; j < MAXSHOPITEMS; ++j) {
		// ammo has different stock method -- half of initial capacity
		int stock = GetShopItemInfo(j, DND_SHOPINFO_STOCK);
		if(stock == -1) {
			temp = GetAmmoSlotAndIndexFromShop(j);
			if(j < SHOP_FIRSTAMMOSPECIAL_INDEX) {
				// overflow fix
				auto a_info = GetAmmoInfo(temp & 0xFFFF, temp >> 16);
				GlobalData.ShopStockRemaining[pnum][j] = (a_info.initial_capacity * (ch_factor / 2) * GetMenuAmmoCount(j - SHOP_FIRSTAMMO_INDEX, AMMOID_COUNT) / 100);
				int ovf_temp = bcs::INT_MAX / ammo_bonus;
				if(GlobalData.ShopStockRemaining[pnum][j] > ovf_temp)
					GlobalData.ShopStockRemaining[pnum][j] = ovf_temp;
				else
					GlobalData.ShopStockRemaining[pnum][j] = (GlobalData.ShopStockRemaining[pnum][j] * ammo_bonus) / 500;
			}
			else
				GlobalData.ShopStockRemaining[pnum][j] = ((SpecialAmmoInfo[temp].initial_capacity * ch_factor * GetMenuAmmoCount(j - SHOP_FIRSTAMMO_INDEX, AMMOID_COUNT)) / 100) * ammo_bonus / 500;
		}
		else
			GlobalData.ShopStockRemaining[pnum][j] = (stock * ch_factor) / 100;
		SendOwnerScript("DnD Sync Shop Stock", pnum, pnum, j, GlobalData.ShopStockRemaining[pnum][j]);
		
		// just to balance the load a bit and not overwhelm
		if(!(j % 10))
			Delay(const:1);
	}
}

// So database can run this
Script "DnD Menu Reset Forced" (void) {
	int pnum = PlayerNumber();
	int ch_factor = (100 + PlayerModData[pnum].vals[PSTAT_SHOPSTOCK_INCREASE]), temp, ammo_bonus = GetAmmoCapIncrease(pnum);
	for(int j = 0; j < MAXSHOPITEMS; ++j) {
		// ammo has different stock method -- half of initial capacity
		int stock = GetShopItemInfo(j, DND_SHOPINFO_STOCK);
		if(stock == -1) {
			temp = GetAmmoSlotAndIndexFromShop(j);
			if(j < SHOP_FIRSTAMMOSPECIAL_INDEX) {
				// overflow fix
				auto a_info = GetAmmoInfo(temp & 0xFFFF, temp >> 16);
				GlobalData.ShopStockRemaining[pnum][j] = (a_info.initial_capacity * (ch_factor / 2) * GetMenuAmmoCount(j - SHOP_FIRSTAMMO_INDEX, AMMOID_COUNT) / 100);
				int ovf_temp = bcs::INT_MAX / ammo_bonus;
				if(GlobalData.ShopStockRemaining[pnum][j] > ovf_temp)
					GlobalData.ShopStockRemaining[pnum][j] = ovf_temp;
				else
					GlobalData.ShopStockRemaining[pnum][j] = (GlobalData.ShopStockRemaining[pnum][j] * ammo_bonus) / 500;
			}
			else
				GlobalData.ShopStockRemaining[pnum][j] = ((SpecialAmmoInfo[temp].initial_capacity * ch_factor * GetMenuAmmoCount(j - SHOP_FIRSTAMMO_INDEX, AMMOID_COUNT)) / 100) * ammo_bonus / 500;
		}
		else
			GlobalData.ShopStockRemaining[pnum][j] = (stock * ch_factor) / 100;
		SendOwnerScript("DnD Sync Shop Stock", pnum, pnum, j, GlobalData.ShopStockRemaining[pnum][j]);

		if(!(j % 10))
			Delay(const:1);
	}
}

Script "DnD Sync Shop Stock" (int pnum, int itemid, int val) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum)
		Terminate;
	GlobalData.ShopStockRemaining[pnum][itemid] = val;
}

Script "DND On Disconnect Menu Cleanup" (int pnum) DISCONNECT CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum)
		Terminate;
		
	DeleteText(BLACKOUT_ID);
	DeleteTextRange(HUD_STAMINA_FOREGROUND, HUD_STAMINA_ICON);
		
	ACS_NamedExecuteAlways("DND Menu Cleanup", 0, pnum);
}

Script "DND Menu Icon Spawner" (void) CLIENTSIDE {
	// dont draw it for the player who is on the menu
	if(ConsolePlayerNumber() == PlayerNumber())
		Terminate;
		
	int tid = DND_MENUFLOATYICON_TID + PlayerNumber();
	SpawnForced("DnD_MenuPlayerIcon", GetActorX(0), GetActorY(0), GetActorZ(0) + 64.0, tid);
	while(CheckInventory("ShowingMenu")) {
		SetActorPosition(tid, GetActorX(0), GetActorY(0), GetActorZ(0) + 64.0, 0);
		Delay(const:1);
	}
	Thing_Remove(tid);
}

// this is needed in case user crashes or abruptly leaves
Script "DnD Menu Icon Force Remove" (int tid) CLIENTSIDE {
	Thing_Remove(tid);
}

// the loop for things that need to be redrawn constantly or input handling
Script "DnD Menu Input Loop" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	// mainboxid was MAINBOX_NONE + 1
	bool redraw = false;
	int animcounter = 0, framecounter = 0, pnum = PlayerNumber();
	int i = 0;
	// tics left before we may send another input, see the send site below
	int op_cooldown = 0;
	int curopt, curopt_prev = MENU_NULL;
	int boxid = MAINBOX_NONE + 1, boxid_prev = MAINBOX_NONE, mainboxid = MAINBOX_NONE, mainboxid_prev = MAINBOX_NONE;
	// the sort button's own greyed out state, see the check inside the loop
	bool sortcd_prev = false;
	// Page change leftovers. Taken down in the redraw pass that draws the next page, not on the
	// change itself: that runs a tic earlier, and the page blinked out for a frame in between.
	bool clear_spellpage = false;
	int clear_backdrop = 0; // 1 = RPGMENUID only, 2 = the whole crafting band
	// the item views open last tic, and whether the tic after one opened still owes a redraw
	int views_last = 0;
	bool view_redraw = false;
	// a view opened, closed or swapped: the last one's drawing is taken down before the next draws
	bool clear_views = false;
	int temp_zx, temp_zy;
	auto CurrentPane = GetPane();
	ResetPane(CurrentPane);
	auto InventoryPane = GetInventoryPane();
	pstat_text_T? pstat_text = GetPlayerStatText();
	scrollbar_T module& ScrollBar = GetScrollBar();
	
	// initialize cursor data
	ResetCursorHoverProc();
	
	ResetCursorDragData();

	ResetCursorClickProc();

	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF / 2;

	// fixes merchant showing up on menu use when talking to him
	DeleteText(RPGMENUID);

	// Here, not in script 900: run on the server this range was 750 hud messages sent over the network.
	DeleteTextRange(RPGMENUCURSORID, RPGMENUINVENTORYID);

	while(CheckInventory("ShowingMenu")) {
		redraw = false;
		// a new screen shape redraws the whole menu at its new zoom, as a fresh page
		if(UpdateMenuZoom(true)) {
			ClearMenuDisplay();
			DeleteTextRange(SPELLPAGE_ID_FIRST, SPELLPAGE_ID_LAST);
			DeleteText(RPGMENUID);
			curopt_prev = MENU_NULL;
		}
		// the frame spans the 16:9 width, so the cursor reaches both sides past the design space
		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_YAW), MOUSE_INPUT_X,
			HUDMAX_X + MCH_CURSOR_OVER, HUDMAX_Y, -MCH_CURSOR_OVER << 16);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_PITCH), MOUSE_INPUT_Y);
		curopt = CheckInventory("MenuOption");
		
		// page changed
		if(curopt != curopt_prev) {
			ResetPane(CurrentPane);
			LoadPane(CurrentPane, curopt);
			ScrollPos.x = 0;

			// The spell page draws below RPGMENUID so it lands in front of its own backdrop, which puts
			// it outside the RPGMENUITEMID band the redraw clears. Nothing else would ever take these
			// down, so leaving a tree page has to -- only leaving one: the band covers the crafting
			// materials, and sweeping it on every change blinked them.
			if(!IsSpellPage(curopt)) {
				if(IsSpellPage(curopt_prev))
					clear_spellpage = true;

				// The pinned spell belongs to the page it was pinned on. Moving between two trees is
				// caught by the shown_tree check in the draw; leaving the spell pages entirely is not.
				GetSpellScroll().shown_spell = -1;
				GetSpellScroll().pinned = false;
				GetSpellScroll().picking_slot = -1;
				GetSpellScroll().picking_page = 0;
				GetSpellScroll().gem_pick = 0;
				SetScrollBarPos(DND_SPELLBAR_PANEL, 0);

				// The backdrop lives in RPGMENUID, outside the band above, and nothing else restates it
				// -- without this it stands over whatever page comes next. Leaving crafting takes its
				// materials and item boxes with it, which the spell sweep used to do by accident.
				if(curopt < MENU_LOAD_CRAFTING || curopt > MENU_LOAD_CRAFTING_LAST)
					clear_backdrop = (curopt_prev >= MENU_LOAD_CRAFTING && curopt_prev <= MENU_LOAD_CRAFTING_LAST) ? 2 : 1;
			}
			// one skill page to another: each lays out the band its own way, so nothing of the last may stay
			else if(IsSpellPage(curopt_prev))
				clear_spellpage = true;

			// A pinned perk description belongs to the page it was pinned on. Cleared here rather
			// than only in that page's draw so it also lapses when the menu is closed and reopened.
			GetPerkScroll().pinned = false;
			// the new page has its own extent, so a grab carried across would be dragging against
			// a range that no longer exists
			for(int b = 0; b < DND_SCROLLBAR_COUNT; ++b)
				GetScrollBar(b).grabbed = false;
			MchSbarReset();
			SetInventory("MenuPosX", 0);
			SetInventory("DnD_PlayerItemIndex", 0);
			SetInventory("DnD_PlayerPrevItemIndex", 0);
			pstat_text.text = ""; // reset stat block text
			PlayerCursorData.hoverNeedsReset = true;
			redraw = true;

			if(IsWeaponPage(curopt_prev))
				ClearInfoPanel();
			else if(IsAmmoPage(curopt_prev))
				DeleteText(RPGMENUHELPCORNERID);
			else
				DeleteText(RPGMENUITEMSUBID);

			// leaving the crafting pages takes their band and the selected item's card with them
			if(IsChromeCraftPage(curopt_prev) && !IsChromeCraftPage(curopt)) {
				DeleteTextRange(MCH_ID_CRAFT_FIRST, MCH_ID_CRAFT_LAST);
				GetMchCraft().page = -1;
				ResetCursorClickData();
			}
			// a skill page keeps only the frame: a redesigned page's content would stand on its board
			if(!IsChromePage(curopt)) {
				DeleteTextRange(MCH_ID_ROWTXT, MCH_ID_TABTXT - 1);
				DeleteTextRange(MCH_ID_PANEIMG, MCH_ID_PAGEIMG + 5);
			}
		}

		curopt_prev = curopt;
		
		int views_seen = GetMenuViewFlags();
		// boxes won't be updated until you finish your sell
		if(CheckInventory("DnD_SellConfirm")) {
			if(CheckInventory("ActivePopupBox"))
				boxid = CheckInventory("ActivePopupBox");
			// the redesigned frame's confirm popup has its own buttons
			mainboxid = GetChromePopupBox();
		}
		else if(curopt < MENU_LOAD_CRAFTING || curopt > MENU_LOAD_CRAFTING_LAST) {
			if(!CheckInventory("DnD_InventoryView") && !CheckInventory("InTradeView") && !CheckInventory("DnD_StashView")) {
				// GetTriggeredBoxOnPane rejects anything past xlim, and xlim is a FAR EDGE coordinate --
				// the default 348 means "nothing left of screen x 132". Every other page starts its
				// content at 184 so it never noticed; this one starts at 26, and its first three columns
				// were simply unreachable.
				if(IsSpellPage(curopt))
					boxid = GetTriggeredBoxOnPane(CurrentPane, PlayerCursorData.posx, PlayerCursorData.posy, 470.0, 300.0);
				else if(IsChromeShopList(curopt))
					boxid = MchShopListHit(curopt);
				else
					boxid = GetTriggeredBoxOnPane(CurrentPane, PlayerCursorData.posx, PlayerCursorData.posy, HUDMAX_XF + ((MCH_CURSOR_OVER + 1) << 16), HUDMAX_YF + 1.0);
				mainboxid = GetTriggeredChromeBox(curopt);
			}
			else {
				// a view that just opened has no pane yet: the draw loads it, after this hit test
				if(views_seen != views_last) {
					if(CheckInventory("DnD_InventoryView"))
						LoadInventoryView(InventoryPane);
					else if(CheckInventory("InTradeView"))
						LoadTradeView(InventoryPane);
					else
						LoadStashView(InventoryPane);
				}
				if(CheckInventory("DnD_InventoryView"))
					boxid = GetTriggeredBoxOnInventoryPane(InventoryPane, PlayerCursorData.posx, PlayerCursorData.posy);
				else if(CheckInventory("InTradeView"))
					boxid = GetTriggeredBoxOnTradePane(InventoryPane, PlayerCursorData.posx, PlayerCursorData.posy);
				else if(CheckInventory("DnD_StashView")) // I used the trade pane variable to hold the stash content
					boxid = GetTriggeredBoxOnStashPane(InventoryPane, PlayerCursorData.posx, PlayerCursorData.posy);
				// a held item sits centred on the cursor and lands on the cells under it
				boxid = GetDragAnchorBox(boxid, CheckInventory("DnD_InventoryView") ? DND_LITVIEW_INVENTORY : CheckInventory("InTradeView") ? DND_LITVIEW_TRADE : DND_LITVIEW_STASH);
				// the frame's tabs stay clickable over a grid; the server drops the view before it navigates
				if(CheckInventory("DnD_InventoryView") || CheckInventory("DnD_StashView"))
					mainboxid = GetTriggeredChromeBox(curopt);
			}
		}
		else {
			boxid = MchCraftHit(pnum, curopt);
			mainboxid = GetTriggeredChromeBox(curopt);
		}
		
		// play sound if box changed
		if(boxid != boxid_prev) {
			if(boxid != MAINBOX_NONE && IsBoxChangeException(curopt, boxid)) {
				boxid = MAINBOX_NONE;
				boxid_prev = MAINBOX_NONE;
			}
			else {
				if(boxid != MAINBOX_NONE) {
					// Rewinding on hover suits a page whose body changes with the hovered box -- the
					// shop help text, the research entry. On the perk tree the rows ARE the boxes, so
					// running the cursor down the list would rewind the list out from under it and
					// the bar would be useless. That page keeps its own two offsets and restores them
					// itself. The move sound is not part of that and stays for every page.
					// The spell index's binds strip reads the same whatever is hovered, so it keeps its place too.
					if(!IsPerkTreePage(curopt) && curopt != MENU_SPELL)
						ScrollPos.x = 0;

					if(!CheckInventory("DnD_InventoryView") && !CheckInventory("InTradeView") && !CheckInventory("DnD_StashView"))
						LocalAmbientSound("RPG/MenuMove", 127);
				}

				if
				(
					IsChromeCraftPage(curopt) ||
					PlayerCursorData.itemHovered == -1 || 
					PlayerCursorData.itemHovered != GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, boxid - 1 - PlayerCursorData.itemHoveredOffset, -1, PlayerCursorData.itemHoveredSource) - 1
				)
				{
					TakeInventory("LimitedCraftDraw", 1);
					redraw = true;
				}
			}
		}
		
		if(mainboxid != mainboxid_prev) {
			if(mainboxid != MAINBOX_NONE)
				LocalAmbientSound("RPG/MenuMove", 127);
			redraw = true;
		}

		// The sort button greys itself out off DnD_SortCooldown, a powerup that lapses on its own with
		// nothing to announce it. Every other redraw in here is driven by an event, so when the timer
		// simply ran out the button stayed grey until the player happened to move the cursor onto
		// something and triggered a redraw for an unrelated reason.
		//
		// Watched here rather than waited out on the server: this is the same copy of the item the
		// draw reads, so the two cannot disagree about when it lapsed, and it costs one inventory
		// lookup on a loop that already does dozens.
		if(!!CheckInventory("DnD_SortCooldown") != sortcd_prev) {
			sortcd_prev = !sortcd_prev;
			redraw = true;
		}
		
		// Both of these have to run before the click below is read. HandlePageListening is where a
		// page states its scroll range, and nothing downstream -- the bar's hit test included --
		// means anything until it has. The drag then gets first refusal on the click, so a press
		// that lands on the bar never also reaches the page as a box press.
		redraw |= HandlePageListening(curopt, boxid);
		redraw |= HandleScrollBarDrag();

		// clean input buffer
		SetInventory("MenuInput", 0);
		// receives MenuInput -- held back while the bar is being dragged, see above
		// A grab on EITHER bar has to hold the click handling off, or dragging the panel's bar would
		// also count as clicking whatever perk row the cursor started over.
		if(!IsAnyScrollBarGrabbed())
			GetInputOnMenuPage(curopt);
		i = CheckInventory("MenuInput");
		if(i && IsChromePage(curopt)) {
			int sent_input = i;
			i = HandleChromeClientInput(curopt, i, mainboxid);
			// a crafting page names its own box and selection, or answers the click itself
			if(i && IsChromeCraftPage(curopt)) {
				if(mainboxid == MAINBOX_NONE || CheckInventory("DnD_SellConfirm"))
					i = MchCraftInput(pnum, curopt, i, boxid);
				else {
					GetMchCraft().send_box = MAINBOX_NONE;
					GetMchCraft().send_main = 0;
				}
			}
			SetInventory("MenuInput", i);
			redraw |= i != sent_input;
		}

		// Straight after the input is produced and before the ack below can clear it. See
		// HandlePerkPin: nothing may read MenuInput on a later iteration and expect to find it.
		if(IsPerkTreePage(curopt))
			redraw |= HandlePerkPin(curopt - MENU_PERKTREE_FIRST, boxid, i);
		else if(IsSpellTreePage(curopt))
			redraw |= HandleSpellPin(curopt - MENU_SPELLTREE_FIRST, boxid, i);
		else if(curopt == MENU_SPELL_HOTBAR)
			redraw |= HandleSpellHotbarPick(pnum, boxid, i);
		else if(curopt == MENU_SPELL_GEMS)
			redraw |= HandleSpellGemsPick(pnum, boxid, i);
				
		// The server discards anything arriving inside its own DND_MENU_INPUTDELAYTICS debounce --
		// no ack, no processing, no way for us to know. Clicking faster than that just means a
		// random subset survives. We already know the rule, so pace ourselves to it instead of
		// trying to detect the drops: sends spaced by DND_MENU_SENDINTERVAL arrive spaced the same
		// way, and the server takes every one. Costs one tic over the debounce we were already
		// bounded by, and does not depend on latency in either direction.
		if(op_cooldown)
			--op_cooldown;

		if(i && !op_cooldown) {
			// server gets a few extra info in boxid
			// Recomputed every send, deliberately. This used to latch on the first click and only
			// clear once the server's ack came back, so at high ping every further click inside
			// that round trip re-sent the FIRST box: the input did leave, it just named a box the
			// player had already moved on from, which looks exactly like the click being eaten.
			// Nothing here needs to survive a send -- each click names its own box, and the craft
			// selection is read as it stands right now rather than as it stood one round trip ago.
			int sendbox = IsChromeCraftPage(curopt) ? GetMchCraft().send_box : boxid;
			MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = (sendbox | MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK]);
			i <<= 16;
			i |= PlayerNumber();
			op_cooldown = DND_MENU_SENDINTERVAL;
			int sendmain = GetChromeSendMain(mainboxid, i >> 16);
			if(IsChromeCraftPage(curopt) && GetMchCraft().send_main)
				sendmain = GetMchCraft().send_main;
			GetMchCraft().send_main = 0;
			NamedRequestScriptPuke("DND Server Box Receive", i, MenuInputData[pnum][DND_MENUINPUT_PAYLOAD], sendmain | (CheckInventory("MenuPosX") << 16));
			redraw = true;
		}
		
		views_last = views_seen;

		// update prev with new ones
		boxid_prev = boxid;
		mainboxid_prev = mainboxid;
		
		// clickable main buttons
		HandleClickableButtonFrames();

		MenuHudSize(HUDMAX_X, HUDMAX_Y);
		DrawCursor();
		
		// hovered on or things that are otherwise redrawn over and over
		if(PlayerCursorData.itemClicked != -1 && CheckInventory("DnD_UsedTwoItemRequirementMaterial") - 1 != PlayerCursorData.itemClicked) {
			if(PlayerCursorData.clickNeedsReset || CheckInventory("DnD_CursorDataClearRequest"))
				ResetCursorClickData();
			else {
				// don't draw twice
				if(PlayerCursorData.itemHovered == PlayerCursorData.itemClicked)
					ResetCursorHoverData();
			
				// insta refresh
				CleanInventoryInfo(RPGMENUCLICKEDID);
				DrawCraftingInventoryInfo(
					pnum,
					RPGMENUCLICKEDID,
					PlayerCursorData.itemClickedPos.x,
					PlayerCursorData.itemClickedPos.y,
					PlayerCursorData.itemClicked,
					PlayerCursorData.itemClickedType,
					PlayerCursorData.itemClickedSource
				);
			}
		}
		
		if(PlayerCursorData.itemHovered != -1/* && (PlayerCursorData.itemClicked == -1 || CheckInventory("DnD_UsedTwoItemRequirementMaterial"))*/) {
			if(boxid == MAINBOX_NONE || PlayerCursorData.hoverNeedsReset)
				ResetCursorHoverData();
			else if(curopt != MENU_LOAD_CRAFTING_TRANSMUTING || boxid >= MATERIALBOX_OFFSET_BOXID) {
				if(PlayerCursorData.itemHoveredDim.x) {
					DrawInventoryInfo(PlayerCursorData.owner_pnum);
				}
				else {
					if(!CheckInventory("LimitedCraftDraw") || (PlayerCursorData.itemHoveredType != DND_ITEM_ORB && PlayerCursorData.itemHoveredType != DND_ITEM_TOKEN)) {
						// don't redraw if its in crafting material menu, just draw once
						DrawCraftingInventoryInfo(
							PlayerCursorData.owner_pnum,
							RPGMENUINVENTORYID,
							PlayerCursorData.posx,
							PlayerCursorData.posy,
							PlayerCursorData.itemHovered,
							PlayerCursorData.itemHoveredType,
							PlayerCursorData.itemHoveredSource
						);
					}

					if(curopt == MENU_LOAD_CRAFTING_WEAPON || curopt == MENU_LOAD_CRAFTING_INVENTORY)
						GiveInventory("LimitedCraftDraw", 1);
				}
			}
		}
		else if(PlayerCursorData.hoverNeedsReset)
			ResetCursorHoverData();
			
		if(PlayerCursorData.itemDragged != -1 && PlayerCursorData.itemDragInfo.topboxid != -1) {
			DrawDraggedItem(pnum);
			redraw = true;
		}
		
		// other menu scripts may set their own canvas meanwhile, keep this one's offsets
		temp_zx = GetMenuZoom().ox;
		temp_zy = GetMenuZoom().oy;
		Delay(const:1);
		GetMenuZoom().ox = temp_zx;
		GetMenuZoom().oy = temp_zy;

		// A view the server opened or closed during the delay: boxid names the old layout's box, so this draw
		// shows no hover and the next one has the view's own.
		if(GetMenuViewFlags() != views_seen) {
			boxid = MAINBOX_NONE;
			view_redraw = true;
			clear_views = true;
		}
		else if(view_redraw) {
			redraw = true;
			view_redraw = false;
		}

		// the frame's buttons animate a press
		if(CheckInventory(ButtonTimers[0]) || CheckInventory(ButtonTimers[1]) || CheckInventory(ButtonTimers[2]))
			redraw = true;
		
		// draws items that need frequent redraw
		DrawFrequentRedrawItems(curopt, pnum);
		
		// animation things
		animcounter++;
		if(animcounter == 17) {
			animcounter = 0;
			framecounter = (++framecounter) % 3;
		}
		
		if(CheckInventory("DND_ACK")) {
			SetInventory("MenuInput", 0);
			//Log(s:"reset input data");
			MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = 0;
			MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
			
			// this part is important because due to how the system is set up once the ACK is received, we may not update for next frames
			// this causes the problem of "spam clicking" multiple stack craft materials and the inputs being reset above, with a redraw we
			// force a recalculation basically so we obtain the values once again
			redraw = true;
		}

		if(CheckInventory("DnD_ResetDrawnStatText")) {
			pstat_text.text = "";
			TakeInventory("DnD_ResetDrawnStatText", 1);
		}

		//////////////////////////////////////////
		// MENU DRAWING ROUTINE FROM HERE BELOW //
		//////////////////////////////////////////
		if(redraw || CheckInventory("DnD_RefreshPane")) {
			//Log(s:"redraw");
			str toshow = "";
			int j = 0, k = 0, temp;
			
			DrawChromeFrame(pnum, curopt, mainboxid);
			
			// main drawing of texts, labels, icons etc.
			MenuHudSize(HUDMAX_X, HUDMAX_Y);
			DrawScrollBar();
			SetFont("NMENUFNT");
			DeleteTextRange(RPGMENUITEMID - 45, RPGMENUITEMID);
			DeleteTextRange(RPGMENUPAGEID - 1, RPGMENUPAGEID);

			// set by the page change above, see the declaration
			if(clear_spellpage)
				DeleteTextRange(SPELLPAGE_ID_FIRST, SPELLPAGE_ID_LAST);
			if(clear_backdrop == 2)
				CleanCraftingInfo();
			else if(clear_backdrop == 1)
				DeleteText(RPGMENUID);
			clear_spellpage = false;
			clear_backdrop = 0;

			if(PlayerCursorData.itemDragged != -1 || !CheckInventory("DnD_UsedTwoItemRequirementMaterial"))
				DeleteText(RPGMENUCURSORID + 1); // dragged item

			// Before the views, not after: the inventory range below is the one they draw into, so a
			// clean landing with its view still up erased it and nothing redrew until a hover.
			if(CheckInventory("DnD_CleanInventoryRequest") || clear_views) {
				clear_views = false;
				// NOTE: THE RANGE BELOW IS HUGE BUT ITS NEEDED!!!!
				CleanMaterialInfo(true);
				CleanInventoryInfo();
				DeleteTextRange(RPGMENUINVENTORYID - HUD_DII_MULT * MAX_INVENTORY_BOXES, RPGMENUINVENTORYID);
				TakeInventory("DnD_CleanInventoryRequest", 1);
			}
			else if(CheckInventory("DnD_CleanCraftingRequest")) {
				CleanCraftingInfo();
				CleanMaterialInfo(true);
				// dont uncomment this from here... for some reason
				//TakeInventory("DnD_CleanCraftingRequest", 1);
			}
			else if(CheckInventory("DnD_CleanTradeviewRequest")) {
				CleanTradeInfo();
				TakeInventory("DnD_CleanTradeviewRequest", 1);
			}

			if(CheckInventory("DnD_InventoryView")) {
				LoadInventoryView(InventoryPane);
				HandleInventoryView(boxid);
			}
			else if(CheckInventory("DnD_StashView")) {
				LoadStashView(InventoryPane);
				HandleStashView(boxid);
			}
			else if(CheckInventory("InTradeView")) {
				LoadTradeView(InventoryPane);
			}
			else if(PlayerCursorData.itemDragged != -1) {
				// No item view is being drawn, so nothing here will refresh or clear the drag --
				// DoInventoryBoxDraw is what normally does both. Drop it rather than leaving the
				// last drawn frame's item stuck to the cursor.
				ResetCursorDragData();
				ResetCursorDragConfirm();
			}

			SetFont("NMENUFNT");
			if(IsChromePage(curopt)) {
				DrawChromePage(pnum, curopt, boxid, mainboxid, CurrentPane);
				if(ChromeSellPopupShown())
					DrawChromeSellPopup(mainboxid);
				else
					DeleteTextRange(MCH_ID_POPFIRST, MCH_ID_POPLAST);
				MenuHudSize(HUDMAX_X, HUDMAX_Y);
				SetFont("NMENUFNT");
			}
			else if(curopt == MENU_SPELL)
				HandleSpellIndexDraw(pnum, boxid);
			else if(IsSpellTreePage(curopt))
				HandleSpellTreeDraw(pnum, curopt - MENU_SPELLTREE_FIRST, boxid, CurrentPane);
			else if(curopt == MENU_SPELL_HOTBAR)
				HandleSpellHotbarDraw(pnum, boxid, CurrentPane);
			else if(curopt == MENU_SPELL_GEMS)
				HandleSpellGemsDraw(pnum, boxid, CurrentPane);
			#ifdef ISAPRILFIRST
			else if(curopt == MENU_SHOP_NFT) {
				HudMessage(s:"--- ", l:"DND_MENU_HEAD_NFT", s:" ---"; HUDMSG_PLAIN, RPGMENUHELPID, CR_CYAN, MenuX(316.4), MenuY(44.1), 0.0, 0.0);
				DrawCredits(pnum);
				
				MenuClipRect(184, 56, 264, 224, 264);
				
				HudMessage(l:"DND_NFT_INTRO"; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, MenuX(316.4), MenuY(72.1), 0.0, 0.0);
				
				// draw nft labels, with images
				HandleNFTDrawing(boxid);
				
				MenuClipRect(0, 0, 0, 0, 0);
			}
			#endif
			
			// we've had redrawn, take these away now
			TakeInventory("DnD_RefreshPane", 1);
			//TakeInventory("DnD_RefreshRequest", 1);
			///////////////////////////////
			// MENU DRAWING ROUTINE ENDS //
			///////////////////////////////
		}
	}
	
	ScrollPos.x = 0;

	// The spell pages draw outside every band ClearMenuDisplay sweeps, and their backdrop is an id of
	// its own. A page CHANGE cleared them; closing the menu on one did not, so the whole tree stayed
	// painted over the game.
	DeleteTextRange(SPELLPAGE_ID_FIRST, SPELLPAGE_ID_LAST);
	DeleteText(RPGMENUID);

	ClearMenuDisplay();
	UpdateMenuZoom(false);
}

Script "DnD Main Button Click Animation" (int boxid) CLIENTSIDE {
	if(!CheckInventory(ButtonTimers[boxid - FIRST_CLICKABLE_BOXID]))
		SetInventory(ButtonTimers[boxid - FIRST_CLICKABLE_BOXID], 1); // kick starts the animation
	SetResultValue(0);
}

// Tell the client what it is actually holding rather than letting it infer that from its own copy
// of the item arrays -- see idragconfirm_T for why inferring produced a cursor item that was not
// there. Sent once per processed input, from the server, reflecting final state.
// Only the plain inventory view is vouched for so far: it is one grid with one source. Stash and
// trade stack several grids under a single DnD_SelectedInventoryBox, so they keep the old derived
// path (valid stays clear for them) until this has been proven under real latency.
void SyncDragPayload(int pnum) {
	// read through the tid, handlers above may have moved the activator to another player
	int tid = pnum + P_TIDSTART;
	int sel = CheckActorInventory(tid, "DnD_SelectedInventoryBox");	// 1 based, 0 = holding nothing
	int source = -1, grid = 0;

	// Mirrors the offset/source deciphering the click handlers do, view for view: the plain
	// inventory is one grid, stash stacks stash over player inventory, trade stacks the other
	// player's offering, ours, then our inventory. If those handlers ever change, this has to move
	// with them or the cursor will disagree with what a click actually does.
	if(sel > 0) {
		if(CheckActorInventory(tid, "DnD_InventoryView"))
			source = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
		else if(CheckActorInventory(tid, "DnD_StashView")) {
			if(sel > MAX_INVENTORY_BOXES) {
				source = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
				grid = 1;
			}
			else	// a held stash item keeps the page it was picked up on, not the page on screen
				source = DND_SYNC_ITEMSOURCE_STASH | ((CheckActorInventory(tid, "DnD_PlayerPreviousPage") - 1) << 16);
		}
		else if(CheckActorInventory(tid, "InTradeView")) {
			if(sel > 2 * MAX_INVENTORY_BOXES) {
				source = DND_SYNC_ITEMSOURCE_PLAYERINVENTORY;
				grid = 2;
			}
			else {
				source = DND_SYNC_ITEMSOURCE_TRADEVIEW;
				grid = 1;
			}
		}
	}

	// nothing selected, or a view we do not vouch for -> client falls back to deriving the cursor
	if(source == -1) {
		ACS_NamedExecuteWithResult("DnD Drag Payload Sync", pnum, 0, 0, 0);
		return;
	}

	int box = sel - 1 - grid * MAX_INVENTORY_BOXES;
	int top = box >= 0 ? GetItemSyncValue(pnum, DND_SYNC_ITEMTOPLEFTBOX, box, -1, source) - 1 : -1;

	// Vouched for, holding nothing. Grid does not matter here, no grid will draw a cursor item.
	// The zero DIMENSIONS are what say "empty hand" on the other side, not the zero image -- image
	// id 0 is IIMG_SC_1, an ordinary small charm, and cannot carry that meaning.
	if(top < 0 || GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, top, -1, source) == DND_ITEM_NULL) {
		ACS_NamedExecuteWithResult("DnD Drag Payload Sync", pnum, 0, source | DND_DRAGSYNC_VALID, 0);
		return;
	}

	// source keeps the stash page in its high bits, so the rest is packed into the last argument
	ACS_NamedExecuteWithResult(
		"DnD Drag Payload Sync",
		pnum,
		GetItemSyncValue(pnum, DND_SYNC_ITEMIMAGE, top, -1, source),
		source | DND_DRAGSYNC_VALID,
		GetItemSyncValue(pnum, DND_SYNC_ITEMWIDTH, top, -1, source) |
		(GetItemSyncValue(pnum, DND_SYNC_ITEMHEIGHT, top, -1, source) << 8) |
		(top << 16) | (grid << 24)
	);
}

Script "DnD Drag Payload Sync" (int pnum, int image, int packed, int dims) CLIENTSIDE {
	// unlike the item syncer this one IS per viewer -- the cursor is ours alone
	if(GameType() == GAME_SINGLE_PLAYER || ConsolePlayerNumber() != pnum)
		Terminate;

	PlayerCursorData.itemDragInfo.confirmed.valid = !!(packed & DND_DRAGSYNC_VALID);
	PlayerCursorData.itemDragInfo.confirmed.source = packed & ~DND_DRAGSYNC_VALID;
	PlayerCursorData.itemDragInfo.confirmed.image = image;
	PlayerCursorData.itemDragInfo.confirmed.size_x = dims & 0xFF;
	PlayerCursorData.itemDragInfo.confirmed.size_y = (dims >> 8) & 0xFF;
	PlayerCursorData.itemDragInfo.confirmed.topboxid = (dims >> 16) & 0xFF;
	PlayerCursorData.itemDragInfo.confirmed.offset = ((dims >> 24) & 0x3) * MAX_INVENTORY_BOXES;

	SetResultValue(0);
}

Script "DND Server Box Receive" (int pnum, int boxid, int mainboxid) NET {
	// don't let garbage data slip in
	if(!pnum)
		Terminate;
	int temp = pnum >> 16;
	int tpos = 0;
	int stat_id, stat_pt;
	
	//int dummy = pnum;
	pnum &= 0xFFFF;
	if(!IsRequestFromPlayer(pnum))
		Terminate;
	if(!MenuInputData[pnum][DND_MENUINPUT_DELAY]) {
		//printbold(s:"received ", d:dummy, s: " ", d:boxid, s: " ", d:mainboxid);
		int curopt, i, j;

		// extra Left - Right pos is kept in mainboxid
		if(mainboxid > 0xFFFF) {
			MenuInputData[pnum][DND_MENUINPUT_LRPOS] = mainboxid >> 16;
			mainboxid &= 0xFFFF;
		}
		
		SetActivator(pnum + P_TIDSTART);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = DND_MENU_INPUTDELAYTICS;
		MenuInputData[pnum][DND_MENUINPUT] = temp;
		
		curopt = CheckInventory("MenuOption");
		GiveInventory("DND_ACK", 1);

		// leftmost panel clicked
		if(mainboxid != MAINBOX_NONE) {
			if(HasLeftClicked(pnum)) {
				TakeInventory("DnD_InventoryView", 1);
				TakeInventory("DnD_StashView", 1);
				if(mainboxid >= FIRST_CLICKABLE_BOXID && mainboxid <= LAST_CLICKABLE_BOXID)
					HandleButtonClick(mainboxid);
				else if(mainboxid >= MCH_NAV_BASE)
					HandleChromeNav(pnum, curopt, mainboxid - MCH_NAV_BASE);
			}
		}
		else {
			if(CheckInventory("DnD_SellConfirm") && HasLeftClicked(pnum)) {
				TakeInventory("DnD_SellConfirm", 1);
				// clean the sell popup
				SetInventory("DnD_PopupID", 0);
				SetInventory("DnD_PopupSell", 0);
				SetInventory("DnD_ShowPopup", 0);
				SetInventory("DnD_ShowSellPopup", 0);
				SetInventory("DnD_PopupID", 0);
				SetInventory("ActivePopupBox", 0);
				boxid = MAINBOX_NONE;
			}
			
			if(curopt == MENU_STAT1) { // stat screen
				ACS_NamedExecuteWithResult("DnD Sync Player Exp", pnum, GetPlayerExp(pnum));
				if(HasPlayerClicked(pnum)) {
					if(boxid == MBOX_4)
						UpdateMenuPosition(MENU_STAT2);
					else if(CheckInventory("AttributePoint") && boxid != MAINBOX_NONE) { // tried to level stat, so you must have some attrib point
						// here we check if option was left click or right click, right click consumes whatever we can on the given selection
						if(HasLeftClicked(pnum))
							temp = 1;
						else
							temp = CheckInventory("AttributePoint");
							
						stat_id = boxid - 1 + DND_ATTRIB_BEGIN;
						stat_pt = GetStatPoints(stat_id);
					
						ConsumeAttributePointOn(pnum, stat_id, temp);
						LocalAmbientSound("RPG/MenuChoose", 127);
						ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, pnum);
					}
				}
				else if((HasJumpClicked(pnum) || HasJumpRightClicked(pnum)) && boxid != MAINBOX_NONE && boxid <= MBOX_3) {
					// Both buttons already spend, so holding jump flips them into the refund: one
					// point on left, as many as the credits cover on right.
					stat_id = boxid - 1 + DND_ATTRIB_BEGIN;
					stat_pt = GetStatPoints(stat_id);
					temp = HasJumpClicked(pnum) ? 1 : stat_pt;

					if(RefundAttributePoints(pnum, stat_id, temp)) {
						LocalAmbientSound("RPG/MenuChoose", 127);
						ACS_NamedExecuteAlways("DnD Force Damage Cache Recalculation", 0, pnum);
					}
					else if(stat_pt)
						ShowActorPopup(pnum, POPUP_NOFUNDS, false, 0);
					else
						// Refunding a stat nothing was ever spent on is not worth a popup, but it
						// still has to answer.
						LocalAmbientSound("RPG/MenuError", 127);
				}
				else if(HasPressedLeft(pnum))
					ReturnToMain();
				else if(HasPressedRight(pnum))
					UpdateMenuPosition(MENU_STAT2);
			}
			else if(curopt == MENU_STAT2) {
				if(HasPlayerClicked(pnum)) {
					if(boxid == MBOX_6)
						UpdateMenuPosition(MENU_STAT1);
					else if(boxid == MBOX_1)
						UpdateMenuPosition(MENU_STAT2_OFFENSE1);
					else if(boxid != MAINBOX_NONE) // skip offense page 2
						UpdateMenuPosition(boxid - MBOX_1 + MENU_STAT2 + 2);
				}
				else if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_STAT1);
			}
			else if(curopt == MENU_STAT2_OFFENSE1) {
				if(HasPlayerClicked(pnum)) {
					if(boxid == MBOX_1)
						UpdateMenuPosition(MENU_STAT2);
					else
						UpdateMenuPosition(MENU_STAT2_OFFENSE2);
				}
				else if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_STAT2);
				else if(HasPressedRight(pnum))
					UpdateMenuPosition(MENU_STAT2_OFFENSE2);
			}
			else if(curopt == MENU_STAT2_OFFENSE2) {
				if(HasPlayerClicked(pnum)) {
					if(boxid == MBOX_1)
						UpdateMenuPosition(MENU_STAT2_OFFENSE1);
				}
				else if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_STAT2_OFFENSE1);
			}
			else if(curopt == MENU_STAT2_DEFENSE || curopt == MENU_STAT2_UTILITY || curopt == MENU_STAT2_EXOTIC || curopt == MENU_STAT2_MISC) {
				if(HasPlayerClicked(pnum)) {
					if(boxid == MBOX_1)
						UpdateMenuPosition(MENU_STAT2);
				}
				else if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_STAT2);
			}
			else if(curopt == MENU_PERK) {
				EnsurePerkArchLists();
				if(HasPlayerClicked(pnum)) {
					// Every archetype has a row in the layout table, including the ones with nothing
					// authored -- so the click is refused here rather than opening an empty page.
					if(boxid >= MBOX_1 && boxid < MBOX_1 + PERK_ARCH_COUNT && GetPerkArchList(boxid - MBOX_1).count)
						UpdateMenuPosition(MENU_PERKTREE_FIRST + boxid - MBOX_1);
					else if(boxid == DND_PERKINDEX_TOGGLEBOX && HasLeftClicked(pnum)) {
						SetInventory("DashDisabled", !CheckInventory("DashDisabled"));
						ACS_NamedExecuteAlways("DnD Toggle User Dash", 0);
						LocalAmbientSound("RPG/MenuChoose", 127);

						// The toggle is drawn from the item this just flipped, and the flip happens a
						// round trip after the client drew it.
						GiveInventory("DnD_RefreshPane", 1);
					}
				}
				else if(HasPressedLeft(pnum))
					ReturnToMain();
			}
			else if(curopt == MENU_SPELL) {
				if(HasPlayerClicked(pnum)) {
					if(boxid >= MBOX_1 && boxid < MBOX_1 + MAX_SKILL_TREES)
						UpdateMenuPosition(MENU_SPELLTREE_FIRST + boxid - MBOX_1);
					else if(boxid == MBOX_1 + MAX_SKILL_TREES)
						UpdateMenuPosition(MENU_SPELL_HOTBAR);
					else if(boxid == MBOX_2 + MAX_SKILL_TREES)
						UpdateMenuPosition(MENU_SPELL_GEMS);
				}
				else if(HasPressedLeft(pnum))
					ReturnToMain();
			}
			else if(curopt == MENU_SPELL_HOTBAR || curopt == MENU_SPELL_GEMS) {
				// Only the page change is the server's business here. The picker is clientside state --
				// see HandleSpellHotbarPick -- and the bind arrives as its own puke to "DnD Bind Hotbar".
				if(HasPlayerClicked(pnum)) {
					if(boxid == MBOX_1)
						UpdateMenuPosition(MENU_SPELL);
				}
				else if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_SPELL);
			}
			else if(IsSpellTreePage(curopt)) {
				// LEFT and RIGHT are separate here, not HasPlayerClicked, which answers to both: left
				// spends a point, right switches an aura or passive off. Sharing one button meant a
				// ranked aura ate the click as a toggle and could never be levelled.
				if(HasLeftClicked(pnum)) {
					// The back arrow is added to the pane after every spell, so entry n stays box n however
					// many spells the tree grows.
					j = GetTreeSpellCount(curopt - MENU_SPELLTREE_FIRST);
					if(boxid == MBOX_1 + j)
						UpdateMenuPosition(MENU_SPELL);
					else if(HandleSpellTreeClick(pnum, curopt - MENU_SPELLTREE_FIRST, boxid)) {
						LocalAmbientSound("RPG/MenuChoose", 127);
						GiveInventory("DnD_RefreshPane", 1);
					}
					// A spell the click could not rank up (locked, maxed or no points) says so.
					else if(boxid >= MBOX_1 && GetTreeSpellByIndex(curopt - MENU_SPELLTREE_FIRST, boxid - MBOX_1) != -1)
						LocalAmbientSound("RPG/MenuError", 127);
				}
				else if(HasRightClicked(pnum)) {
					if(HandleSpellTreeToggle(pnum, curopt - MENU_SPELLTREE_FIRST, boxid)) {
						LocalAmbientSound("RPG/MenuChoose", 127);
						GiveInventory("DnD_RefreshPane", 1);
					}
				}
				else if(HasJumpRightClicked(pnum)) {
					// Jump + right click takes a rank back for credits; the panel says what it costs or why not.
					if(HandleSpellTreeRefund(pnum, curopt - MENU_SPELLTREE_FIRST, boxid)) {
						LocalAmbientSound("RPG/MenuChoose", 127);
						GiveInventory("DnD_RefreshPane", 1);
					}
					else if(boxid >= MBOX_1 && GetTreeSpellByIndex(curopt - MENU_SPELLTREE_FIRST, boxid - MBOX_1) != -1)
						LocalAmbientSound("RPG/MenuError", 127);
				}
				else if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_SPELL);
			}
			else if(IsPerkTreePage(curopt)) {
				EnsurePerkArchLists();

				// The timer reconciles this every 10 tics, which is not soon enough for a click that
				// lands in between -- and the click is the one moment the number has to be right.
				RefreshUnspentPerkPoints(pnum);
				if(HasPlayerClicked(pnum)) {
					perk_arch_list_T module& plist = GetPerkArchList(curopt - MENU_PERKTREE_FIRST);

					// The back arrow is added to the pane after every perk, so that entry n stays
					// box n no matter what the tree does.
					if(boxid == MBOX_1 + plist.count)
						UpdateMenuPosition(MENU_PERK);
					else if(boxid >= MBOX_1 && boxid < MBOX_1 + plist.count) {
						// Left spends a point, right buys one back. Both re-run the whole gate rather
						// than trusting this: the box number was computed on a client, against a page
						// that may already be stale by the time it lands here.
						j = plist.perk[boxid - MBOX_1];

						if(HasLeftClicked(pnum) ? SpendPerkPoint(pnum, j) : RespecPerkPoint(pnum, j)) {
							LocalAmbientSound("RPG/MenuChoose", 127);

							// The change lands here, a round trip after the client drew. Without this
							// it does not appear until something else happens to ask for a redraw.
							GiveInventory("DnD_RefreshPane", 1);
						}
						else if(HasLeftClicked(pnum)) {
							// A click that does nothing and says nothing reads as the menu being
							// broken rather than as the perk being locked, so every refusal names
							// itself. The popup carries its own error sound.
							i = GetPerkSpendBlocker(pnum, j);
							if(i == PERKBLOCK_NOPOINTS)
								ShowActorPopup(pnum, POPUP_NOPERKPOINT, false, 0);
							else if(i == PERKBLOCK_MAXED)
								ShowActorPopup(pnum, POPUP_PERKMAXED, false, 0);
							else
								ShowActorPopup(pnum, POPUP_PERKLOCKED, false, 0);
						}
						else {
							i = GetPerkRefundStatus(pnum, j);
							if(i == PERKREFUND_POOR)
								ShowActorPopup(pnum, POPUP_NOFUNDS, false, 0);
							else if(i == PERKREFUND_LOCKS)
								ShowActorPopup(pnum, POPUP_PERKREFUNDLOCKED, false, 0);
							else
								// Right clicking a perk with nothing on it is not a mistake worth a
								// popup, but it still has to answer.
								LocalAmbientSound("RPG/MenuError", 127);
						}
					}
				}
				else if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_PERK);
			}
			else if(curopt == MENU_LOAD) {
				// prevents having something clicked previously in crafting altering something else
				SetInventory("DnD_SelectedInventoryBox", 0);
				if(HasLeftClicked(pnum)) {
					if(boxid > MBOX_1) {
						if(boxid == MBOX_9) {
							ResetDungeonSelectData(pnum);
							UpdateMenuPosition(MENU_LOAD_DUNGEONS);
						}
						else if(boxid == MBOX_8)
							UpdateMenuPosition(MENU_LOAD_TRADE1);
						else if(boxid == MBOX_7) {
							if(MchPageLocked(MENU_LOAD_ACC1))
								ShowPopup(POPUP_NEEDRESEARCH, false, 0);
							else
								UpdateMenuPosition(MENU_LOAD_ACC1);
						}
						else {
							if(MENU_LOAD + boxid == MENU_LOAD_INVENTORY)
								GiveInventory("DnD_InventoryView", 1);
							else if(MENU_LOAD + boxid == MENU_LOAD_STASH)
								GiveInventory("DnD_StashView", 1);
							// crafting opens on weapon crafting; its pages are chips there
							if(MENU_LOAD + boxid == MENU_LOAD_CRAFTING)
								UpdateMenuPosition(MENU_LOAD_CRAFTING_WEAPON);
							else
								UpdateMenuPosition(MENU_LOAD + boxid);
						}
					}
				}
				if(HasPressedLeft(pnum))
					ReturnToMain();
			}
			else if(curopt == MENU_LOAD_INVENTORY) {
				if(HasPlayerClicked(pnum)) {
					// handles clicking outside
					// at the last part, we reset the prev box only if there's no item there, otherwise another piece of code handles it
					if(HasLeftClicked(pnum) && boxid == MAINBOX_NONE) {
						// A selection only earns the click if it is holding something to throw, and
						// HandleInventoryViewClicks below is what throws it. One left over on an
						// empty box used to swallow the click instead, which is why closing with
						// the mouse needed a second one while the left key always worked.
						if
						(
							!CheckInventory("DnD_SelectedInventoryBox") ||
							GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) == DND_ITEM_NULL
						)
						{
							SetInventory("DnD_SelectedInventoryBox", 0);
							TakeInventory("DnD_InventoryView", 1);
							SetInventory("DnD_SelectedCharmBox", 0);
							GiveInventory("DnD_CleanInventoryRequest", 1);
							if(CheckInventory("MenuOption") == MENU_LOAD_INVENTORY || CheckInventory("MenuOption") == MENU_LOAD_STASH)
								UpdateMenuPosition_NoSound(MENU_LOAD);
							else
								LocalAmbientSound("RPG/MenuClose", 127);
						}
					}
					HandleInventoryViewClicks(pnum, boxid, temp);
				}
				if(HasPressedLeft(pnum)) {
					TakeInventory("DnD_InventoryView", 1);
					GiveInventory("DnD_CleanInventoryRequest", 1);
					UpdateMenuPosition(MENU_LOAD);
				}
			}
			else if(curopt == MENU_LOAD_STASH) {
				if(HasPlayerClicked(pnum)) {
					temp = MenuInputData[pnum][DND_MENUINPUT];
					// handles clicking outside
					// at the last part, we reset the prev box only if there's no item there, otherwise another piece of code handles it
					if(HasLeftClicked(pnum) && boxid == MAINBOX_NONE) {
						// Same rule the inventory view uses: a selection only earns the click if it
						// is holding something to throw, and HandleStashViewClicks below is what
						// throws it. The two arms are the two grids stacked in this view -- the
						// player inventory past MAX_INVENTORY_BOXES, the stash below it on the page
						// the item was picked up from.
						if
						(
							!CheckInventory("DnD_SelectedInventoryBox") ||
							((CheckInventory("DnD_SelectedInventoryBox") > MAX_INVENTORY_BOXES) && (GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - MAX_INVENTORY_BOXES - 1, -1, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY) == DND_ITEM_NULL)) ||
							(
								(CheckInventory("DnD_SelectedInventoryBox") <= MAX_INVENTORY_BOXES) &&
								(GetItemSyncValue(pnum, DND_SYNC_ITEMTYPE, CheckInventory("DnD_SelectedInventoryBox") - 1, -1, DND_SYNC_ITEMSOURCE_STASH | ((CheckInventory("DnD_PlayerPreviousPage") - 1) << 16)) == DND_ITEM_NULL)
							)
						)
						{
							//printbold(s:"set to 0 before arrival on page ", d:CheckInventory("DnD_PlayerPreviousPage"));
							SetInventory("DnD_SelectedInventoryBox", 0);
							TakeInventory("DnD_StashView", 1);
							SetInventory("DnD_SelectedCharmBox", 0);
							GiveInventory("DnD_CleanInventoryRequest", 1);
							if(CheckInventory("MenuOption") == MENU_LOAD_INVENTORY || CheckInventory("MenuOption") == MENU_LOAD_STASH)
								UpdateMenuPosition_NoSound(MENU_LOAD);
							else
								LocalAmbientSound("RPG/MenuClose", 127);
						}
					}
					HandleStashViewClicks(pnum, boxid, temp);
				}
				if(HasPressedLeft(pnum)) {
					TakeInventory("DnD_StashView", 1);
					GiveInventory("DnD_CleanInventoryRequest", 1);
					UpdateMenuPosition(MENU_LOAD);
				}
			}
			else if(curopt == MENU_LOAD_CHARM) {
				HandleItemPageInputs(pnum, boxid);
				if(HasPressedLeft(pnum)) {
					// skip the view if there was, otherwise move page back
					if(CheckInventory("DnD_InventoryView")) {
						TakeInventory("DnD_InventoryView", 1);
						GiveInventory("DnD_CleanInventoryRequest", 1);
						SetInventory("DnD_SelectedInventoryBox", 0);
						SetInventory("DnD_SelectedCharmBox", 0);
					}
					else
						UpdateMenuPosition(MENU_LOAD);
				}
			}
			else if(curopt == MENU_LOAD_WEAPONDISCARD) {
				if(HasPlayerClicked(pnum) && boxid != MAINBOX_NONE) {
					temp = boxid;
					if(temp < 9) {
						if(HasPlayerDiscardedSlot(pnum, temp - 1))
							temp = -temp;
						UpdateActivity(pnum, DND_ACTIVITY_WEAPONDISCARD, temp, 0);
					}
					else {
						UpdateActivity(pnum, DND_ACTIVITY_WEAPONDISCARD, temp, 2);
					}

					if(temp > 0) {
						switch(temp) {
							case 1:
								TakeInventory(Weapons_Data[DND_WEAPON_CHAINSAW].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_CHAINSAW].name))
									TakeInventory("H_WeaponSlot1", 1);
							break;
							case 2:
								TakeInventory(Weapons_Data[DND_WEAPON_PISTOL].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_PISTOL].name))
									TakeInventory("H_WeaponSlot2", 1);
							break;
							case 3:
								TakeInventory(Weapons_Data[DND_WEAPON_SHOTGUN].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_SHOTGUN].name))
									TakeInventory("H_WeaponSlot3", 1);
							break;
							case 4:
								TakeInventory(Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_SUPERSHOTGUN].name))
									TakeInventory("H_WeaponSlot3X", 1);
							break;
							case 5:
								TakeInventory(Weapons_Data[DND_WEAPON_MACHINEGUN].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_MACHINEGUN].name))
									TakeInventory("H_WeaponSlot4", 1);
							break;
							case 6:
								TakeInventory(Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_ROCKETLAUNCHER].name))
									TakeInventory("H_WeaponSlot5", 1);
							break;
							case 7:
								TakeInventory(Weapons_Data[DND_WEAPON_PLASMARIFLE].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_PLASMARIFLE].name))
									TakeInventory("H_WeaponSlot6", 1);
							break;
							case 8:
								TakeInventory(Weapons_Data[DND_WEAPON_BFG6000].name, 1);
								if(CheckWeapon(Weapons_Data[DND_WEAPON_BFG6000].name))
									TakeInventory("H_WeaponSlot7", 1);
							break;
						}
					}

					LocalAmbientSound("RPG/MenuChoose", 127);
				}

				if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_LOAD);
			}
			else if(curopt >= MENU_LOAD_CRAFTING && curopt <= MENU_LOAD_CRAFTING_INVENTORY)
				HandleCraftingInputs(boxid, curopt);
			else if(curopt == MENU_LOAD_CRAFTING_TRANSMUTING)
				HandleTransmutingInputs(pnum, boxid);
			else if(curopt >= MENU_FIRST_TRADE && curopt <= MENU_LAST_TRADE) {
				if(HasPlayerClicked(pnum)) {
					if(boxid >= MBOX_1) {
						// we need to find which player this box corresponds to
						i = 0, temp = 0;
						// how many we skip
						j = MENU_TRADE_PLAYERCOUNT * (curopt - MENU_FIRST_TRADE);
						for(; i < MAXPLAYERS && temp < boxid + j; ++i)
							if(PlayerInGame(i) && i != pnum)
								++temp;
						// i - 1 is the player number
						--i;
						// left click
						if(HasLeftClicked(pnum)) {
							if(!CheckInventory("DnD_Trade_Cooldown")) {
								// we aren't in trade panel
								if(!CheckInventory("InTradeView")) {
									if(!IsTrading(pnum)) {
										// handle trade protocol, initiate the request
										if(!IsTrading(i)) {
											if(!TradeMuteStatus(i, pnum)) {
												EngageTrade(pnum, i);
												SetActivator(i + P_TIDSTART);
												ACS_NamedExecuteAlways("DND Announcer", 0, DND_ANNOUNCER_TRADEREQUEST, pnum, i + 1);
												SetActivator(pnum + P_TIDSTART);
												ACS_NamedExecuteAlways("DnD Trade Window", 0, pnum, false);
												ACS_NamedExecuteAlways("DnD Trade Window", 0, i, true);
												LocalAmbientSound("RPG/MenuChoose", 127);
											}
											else
												ShowActorPopup(pnum, POPUP_YOUAREMUTED, false, 0);
										}
										else
											ShowActorPopup(pnum, POPUP_TARGETISTRADING, false, 0);
									}
									else {
										// trade accept window is given to the other party, who engaged this
										if(IsTradingWith(pnum, i) && CheckInventory("DnD_TradeAcceptWindow")) {
											if(!TradeMuteStatus(i, pnum)) {
												// handle trade now, other party accepted
												GiveInventory("InTradeView", 1);
												GiveActorInventory(i + P_TIDSTART, "InTradeView", 1);
												// nothing is staked yet, so neither side should be
												// carrying notes left over from an earlier trade
												ClearTradeItemOrigins(pnum);
												ClearTradeItemOrigins(i);
												SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);
												SendOwnerScript("DnD Refresh Request", i, i, 1);
												LocalAmbientSound("RPG/MenuChoose", 127);
											}
											else
												ShowActorPopup(pnum, POPUP_YOUAREMUTED, false, 0);
										}
										else
											ShowActorPopup(pnum, POPUP_TARGETISTRADING, false, 0);
									}
								}
								else {
									// we are in trade panel now, handle boxes accordingly
									// give some time for the synchronization to settle in
									HandleTradeViewButtonClicks(pnum, boxid);
								}
							}
						}
						else {
							// right click mutes people
							if(!CheckInventory("InTradeView")) {
								ToggleMuteTrade(i);
								LocalAmbientSound("RPG/MenuChoose", 127);
							}
							else
								HandleTradeViewButtonClicks(pnum, boxid);
						}
					}
				}
				
				// only allow these if we arent currently trading with anyone
				if(!IsTrading(pnum)) {
					if(HasPressedLeft(pnum)) {
						if(curopt == MENU_FIRST_TRADE)
							UpdateMenuPosition(MENU_LOAD);
						else
							UpdateMenuPosition(curopt - 1);
					}
					else if(HasPressedRight(pnum)) {
						if(curopt != MENU_LAST_TRADE && PlayerCount() > MENU_TRADE_PLAYERCOUNT * (curopt - MENU_FIRST_TRADE + 1))
							UpdateMenuPosition(curopt + 1);
					}
				}
			}
			else if(curopt >= MENU_FIRST_ACCESSORY_PAGE && curopt <= MENU_LAST_ACCESSORY_PAGE) {
				if(HasLeftClicked(pnum)) {
					if(boxid != MAINBOX_NONE) {
						if(boxid == MBOX_6 && curopt != MENU_LAST_ACCESSORY_PAGE)
							UpdateMenuPosition(curopt + 1);
						else if(boxid == MBOX_7 || (curopt == MENU_LAST_ACCESSORY_PAGE && boxid == MBOX_3))
							UpdateMenuPosition(curopt - 1);
						else {
							// i is the accessory to toggle
							temp = boxid + ACCESSORY_PER_PAGE * (curopt - MENU_FIRST_ACCESSORY_PAGE) - 1;
							// I don't have this
							if(!CheckInventory(GetAccessoryName(temp)))
								ShowPopup(POPUP_ACCESSORYNOTFOUND, false, 0);
							else if(IsAccessoryEquipped(0, 1 << temp)) { // I have and already equipped it
								LocalAmbientSound("RPG/MenuChoose", 127);
								UnequipAccessory(1 << temp);
							}
							else if(!ReachedAccessoryLimit()) { // I don't have this, and didn't equip
								LocalAmbientSound("RPG/MenuChoose", 127);
								EquipAccessory(1 << temp);
							}
							else
								ShowPopup(POPUP_MAXACCESSORYEQUIPPED, false, 0);
						}
					}
				}
				if(HasPressedLeft(pnum)) {
					if(curopt != MENU_FIRST_ACCESSORY_PAGE)
						UpdateMenuPosition(curopt - 1);
					else
						UpdateMenuPosition(MENU_LOAD);
				}
				else if(HasPressedRight(pnum)) {
					if(curopt != MENU_LAST_ACCESSORY_PAGE)
						UpdateMenuPosition(curopt + 1);
				}
			}
			else if(curopt == MENU_LOAD_DUNGEONS) {
				if(HasLeftClicked(pnum)) {
					if(!CheckInventory("DnD_InventoryView")) {
						if(boxid == MBOX_1) {
							SetInventory("DnD_Inventory_Filter", DND_BOXFILTER_DUNGEONS);
							GiveInventory("DnD_InventoryView", 1);
							LocalAmbientSound("RPG/MenuChoose", 127);
						}
						else if(boxid == MBOX_2) {
							// allow to open portal to dungeon
							temp = CheckInventory("DnD_SelectedDungeonBox");
							if(!temp)
								ShowPopup(POPUP_NODUNGEONSELECTED, false, 0);
							else if(DungeonInformation.dungeon_id == -1) {
								ACS_NamedExecuteWithResult("DnD Dungeon Portal Create", pnum, temp - 1, GetSelectedDungeonData(pnum).id);
								ResetDungeonSelectData(pnum);
								GiveInventory("DnD_RefreshPane", 1);
								LocalAmbientSound("RPG/MenuChoose", 127);
							}
							else
								ShowPopup(POPUP_ALREADYDUNGEONACTIVE, false, 0);
						}
						else if(boxid == MBOX_3) {
							SetInventory("DnD_Inventory_Filter", 0);
							TakeInventory("DnD_InventoryView", 1);
							GiveInventory("DnD_CleanInventoryRequest", 1);
							UpdateMenuPosition(MENU_LOAD);
						}
					}
					else {
						// inventory view clicks will attempt to place a proper (dungeon key) item to the slot
						HandleDungeonKeySelection(pnum, boxid, temp);
					}
				}
				else if(HasRightClicked(pnum) && boxid == MBOX_1 && CheckInventory("DnD_SelectedDungeonBox")) {
					ResetSelectedDungeonInformation(pnum);
					LocalAmbientSound("RPG/MenuChoose", 127);
				}
				else if(HasPressedLeft(pnum)) {
					SetInventory("DnD_Inventory_Filter", 0);
					TakeInventory("DnD_InventoryView", 1);
					GiveInventory("DnD_CleanInventoryRequest", 1);
					UpdateMenuPosition(MENU_LOAD);
				}
			}
			else if(curopt == MENU_SHOP) {
				if(HasLeftClicked(pnum)) {
					if(boxid == MBOX_1)
						temp = MENU_SHOP_WEAPON;
					else if(boxid == MBOX_2)
						temp = MENU_SHOP_AMMOSELECT;
					else if(boxid == MBOX_3)
						temp = MENU_SHOP_ACCOUNT;
					#ifdef ISAPRILFIRST
					else if(boxid == MBOX_4)
						temp = MENU_SHOP_NFT;
					#endif
					else
						temp = -1;
					if(boxid != MAINBOX_NONE && temp != -1)
						UpdateMenuPosition(temp);
					
				}
				if(HasPressedLeft(pnum))
					ReturnToMain();
			}
			else if(curopt == MENU_SHOP_WEAPON) {
				if(HasLeftClicked(pnum)) {
					if(boxid != MAINBOX_NONE && boxid < MBOX_9)
						UpdateMenuPosition(GetWeaponPage(boxid));
				}
				if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_SHOP);
			}
			else if(curopt == MENU_SHOP_WEAPON1)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON1_BEGIN, SHOP_WEAPON_SLOT1END, -1, -1);
			else if(curopt == MENU_SHOP_WEAPON2)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON2_BEGIN, SHOP_WEAPON_SLOT2END, -1, -1);
			else if(curopt == MENU_SHOP_WEAPON3_1)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON31_BEGIN, SHOP_WEAPON_SLOT3END, -1, -1);
			else if(curopt == MENU_SHOP_WEAPON4_1)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON41_BEGIN, SHOP_WEAPON_SLOT42END, -1, -1);
			else if(curopt == MENU_SHOP_WEAPON5_1)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON51_BEGIN, SHOP_WEAPON_SLOT52END, -1, -1);
			else if(curopt == MENU_SHOP_WEAPON6_1)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON61_BEGIN, SHOP_WEAPON_SLOT62END, -1, -1);
			else if(curopt == MENU_SHOP_WEAPON7)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON7_BEGIN, SHOP_WEAPON_SLOT7END, -1, -1);
			else if(curopt == MENU_SHOP_WEAPON8)
				HandleWeaponPageInput(pnum, boxid, SHOP_WEAPON8_BEGIN, SHOP_WEAPON_SLOT8END, -1, -1);
			else if(curopt == MENU_SHOP_AMMOSELECT) {
				if(HasLeftClicked(pnum)) {
					switch(boxid) {
						case 1:
							UpdateMenuPosition(MENU_SHOP_AMMO_1);
						break;
						case 2:
							UpdateMenuPosition(MENU_SHOP_AMMO_2);
						break;
						case 3:
							UpdateMenuPosition(MENU_SHOP_AMMO_3);
						break;
						case 4:
							UpdateMenuPosition(MENU_SHOP_AMMO_4_1);
						break;
						case 5:
							UpdateMenuPosition(MENU_SHOP_AMMO_SPECIAL1);
						break;
					}
				}
				if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_SHOP);
			}
			else if(curopt == MENU_SHOP_AMMO_1)
				HandleAmmoPageInput(pnum, DND_AMMOSLOT_CLIP, boxid, AMMOSLOT_CLIP_BEGIN, -1, -1, false);
			else if(curopt == MENU_SHOP_AMMO_2)
				HandleAmmoPageInput(pnum, DND_AMMOSLOT_SHELL, boxid, AMMOSLOT_SHELL_BEGIN, -1, -1, false);
			else if(curopt == MENU_SHOP_AMMO_3)
				HandleAmmoPageInput(pnum, DND_AMMOSLOT_ROCKET, boxid, AMMOSLOT_ROCKET_BEGIN, -1, -1, false);
			else if(curopt == MENU_SHOP_AMMO_4_1)
				HandleAmmoPageInput(pnum, DND_AMMOSLOT_CELL, boxid, AMMOSLOT_CELL_BEGIN, -1, -1, false);
			else if(curopt == MENU_SHOP_AMMO_SPECIAL1)
				HandleAmmoPageInput(pnum, -1, boxid, 0, -1, -1, true);
			else if(curopt == MENU_SHOP_ACCOUNT) {
				if(HasLeftClicked(pnum)) {
					if(boxid != MAINBOX_NONE)
						ProcessTrade(pnum, boxid - 1, SHOP_ACCOUNT_BEGIN, SHOP_ACCOUNT_END, TRADE_BUY | TRADE_ACCOUNT, false);
				}
				if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_SHOP);
			}
			else if(curopt == MENU_RESEARCH) {
				if(HasLeftClicked(pnum)) {
					if(boxid == MBOX_1)
						UpdateMenuPosition(MENU_RESEARCH_BODY);
					else if(boxid == MBOX_2)
						UpdateMenuPosition(MENU_RESEARCH_AMMO);
					else if(boxid == MBOX_3)
						UpdateMenuPosition(MENU_RESEARCH_GUNS);
					else if(boxid == MBOX_4)
						UpdateMenuPosition(MENU_RESEARCH_UTILITY);
					else if(boxid == MBOX_5) {
						// donate budget option, basically credit to budget conversion
						// 10,000 credits => 1k budget
						if(GetPlayerCredit(pnum) >= DND_BUDGET_CONVERSION_CREDIT) {
							TakeCredit(DND_BUDGET_CONVERSION_CREDIT);
							GiveInventory("Budget", 1);
							LocalAmbientSound("RPG/MenuSell", 127);
						}
						else
							ShowPopup(POPUP_NOFUNDS, false, 0);
					}
				}
				else if(HasRightClicked(pnum) && boxid == MBOX_5) {
					// x10
					// donate budget option, basically credit to budget conversion
					// 10,000 credits => 1k budget
					if(GetPlayerCredit(pnum) >= DND_BUDGET_CONVERSION_CREDIT * 10) {
						TakeCredit(DND_BUDGET_CONVERSION_CREDIT * 10);
						GiveInventory("Budget", 10);
						LocalAmbientSound("RPG/MenuSell", 127);
					}
					else
						ShowPopup(POPUP_NOFUNDS, false, 0);
				}
				if(HasPressedLeft(pnum))
					ReturnToMain();
			}
			else if(curopt == MENU_RESEARCH_GUNS) {
				if(HasLeftClicked(pnum)) {
					if(boxid == MBOX_1)
						UpdateMenuPosition(MENU_RESEARCH_SLOTGUNS);
					if(boxid == MBOX_2)
						UpdateMenuPosition(MENU_RESEARCH_LUXURYGUNS);
				}
				if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_RESEARCH);
			}
			else if(curopt >= SHOP_RESPAGE_BEGIN && curopt <= SHOP_RESPAGE_END) {
				HandleResearchPageInput(pnum, curopt - SHOP_RESPAGE_BEGIN, boxid);
			}
			else if(curopt == MENU_HELP) {
				if(HasLeftClicked(pnum)) {
					if(boxid >= MBOX_1 && boxid <= MENU_HELP_END - MENU_HELP_BEGIN + 1)
						temp = MENU_HELP_BEGIN + boxid - 1;
					else if(boxid == MBOX_10) {
						GiveInventory("SetMapInfo", 1);
						LocalAmbientSound("RPG/MenuChoose", 127);
						temp = -1;
					}
					else if(boxid == MBOX_11) {
						GiveInventory("SetModTierInfo", 1);
						LocalAmbientSound("RPG/MenuChoose", 127);
						temp = -1;
					}
					else if(boxid == MBOX_12) {
						GiveInventory("SetTempWepIgnore", 1);
						SetInventory("DnD_NoTempPickup", !CheckInventory("DnD_NoTempPickup"));
						LocalAmbientSound("RPG/MenuChoose", 127);
						temp = -1;
					}
					else if(boxid == MBOX_13) {
						ACS_NamedExecuteWithResult("DnD Filter Click", pnum, 0, 0, 0);
						LocalAmbientSound("RPG/MenuChoose", 127);
						temp = -1;
					}
					if(boxid != MAINBOX_NONE && temp != -1)
						UpdateMenuPosition(temp);
				}
				if(HasPressedLeft(pnum))
					ReturnToMain();
			}
			else if(curopt >= MENU_HELP_BEGIN && curopt <= MENU_HELP_END) {
				if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_HELP);
				if(curopt == MENU_HELP_MMODS && HasLeftClicked(pnum) && boxid >= MBOX_1)
					UpdateMenuPosition(MENU_HELP_MMODS_WEAKNESS + boxid - 1);
			}
			else if(curopt >= MENU_MMOD_HELP_BEGIN && curopt <= MENU_MMOD_HELP_END) {
				if(HasPressedLeft(pnum))
					UpdateMenuPosition(MENU_HELP_MMODS);
			}
			#ifdef ISAPRILFIRST
			else if(curopt == MENU_SHOP_NFT) {
				if(HasLeftClicked(pnum) && boxid != MAINBOX_NONE) {
					// check if player has enough $$
					if(GetPlayerCredit(pnum) >= NFT_COST) {
						if(!IsSet(CheckInventory("NFT_Token"), boxid - 1)) {
							TakeCredit(NFT_COST);
							SetInventory("NFT_Token", SetBit(CheckInventory("NFT_Token"), boxid - 1));
							ACS_NamedExecuteAlways("DnD NFT Announce", 0, pnum, boxid - 1);
						}
						else
							ShowPopup(POPUP_OWNNFT, false, 0);
					}
					else
						ShowPopup(POPUP_NOFUNDS, false, 0);
				}
			}
			#endif
		}
		
		// after every handler above has run, so this reflects the settled selection
		SyncDragPayload(pnum);

		ClearPlayerInput(pnum, true);

		Delay(const:DND_MENU_INPUTDELAYTICS);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = 0;
	}
}

Script "DND Menu Cleanup" (int pnum) CLIENTSIDE {
	if(pnum != ConsolePlayerNumber())
		Terminate;
		
	ClearMenuDisplay();
}

Script "DnD Toggle User Dash" (void) CLIENTSIDE {
	SetCVar("dnd_dashstatus", !GetCVar("dnd_dashstatus"));
}

Script "DnD Trade Cancel Button" (int tradee) {
	Delay(4);
	CancelTradeRoutine(tradee);
}

Script "DnD Trade Countdown" (int p1, int p2) CLIENTSIDE {
	if(ConsolePlayerNumber() != p1)
		Terminate;
	// Handle timer display
	int lim = 7 * Clamp_Between(GetCVar("dnd_trade_timer"), 1, 10);
	int count = 0;
	
	while(count < lim && CheckActorInventory(p1 + P_TIDSTART, "DnD_Trade_Confirmed") && CheckActorInventory(p2 + P_TIDSTART, "DnD_Trade_Confirmed")) {
		SetFont("DBIGFONT");
		// a third of the frame canvas, so the big font reads at the size it had on the old page
		MenuHudSize(MCH_W / 3, MCH_H / 3);
		HudMessage(s:"\c[UIGold]", d:(lim - count + 6) / 7; HUDMSG_PLAIN, RPGMENUTRADECOUNTDOWNID, -1, MenuX(240.4), MenuY(276.0), 0.0);
		MenuHudSize(HUDMAX_X * 3 / 2, HUDMAX_Y * 3 / 2);
		Delay(const:5);
		++count;
	}
	
	DeleteText(RPGMENUTRADECOUNTDOWNID);
}

// p1 is us
Script "DnD Trade Counter" (int p1, int p2) {
	int lim = 7 * Clamp_Between(GetCVar("dnd_trade_timer"), 1, 10);
	int count = 0;
	while(count < lim && CheckActorInventory(p1 + P_TIDSTART, "DnD_Trade_Confirmed") && CheckActorInventory(p2 + P_TIDSTART, "DnD_Trade_Confirmed")) {
		Delay(5);
		++count;
	}
	if(count == lim) {
		// inventories may have changed during the countdown: plan both sides again before moving anything
		bool p2_fits = ConfirmSpaceForOfferings(p2, p1);
		bool p1_fits = ConfirmSpaceForOfferings(p1, p2);
		if(!p1_fits || !p2_fits) {
			TakeActorInventory(p1 + P_TIDSTART, "DnD_Trade_Confirmed", 1);
			TakeActorInventory(p2 + P_TIDSTART, "DnD_Trade_Confirmed", 1);
			TakeActorInventory(p1 + P_TIDSTART, "DnD_TradeSpaceFit", 1);
			TakeActorInventory(p2 + P_TIDSTART, "DnD_TradeSpaceFit", 1);
			if(!p1_fits)
				ShowActorPopup(p1, POPUP_NOSPACEFORTRADE, false, 0);
			if(!p2_fits)
				ShowActorPopup(p2, POPUP_NOSPACEFORTRADE, false, 0);
			SetActivator(p1 + P_TIDSTART);
			SendOwnerScript("DnD Refresh Request", p1, p1, 1);
			SetActivator(p2 + P_TIDSTART);
			SendOwnerScript("DnD Refresh Request", p2, p2, 1);
			Terminate;
		}
		TransferTradeItems(p1, p2);
		TransferTradeItems(p2, p1);
		
		// check if commenting this causes any issues, we already save player inventories when they spectate
		// ACS_NamedExecuteAlways("DnD Save Player Inventory", 0, p1 | (CheckActorInventory(p1 + P_TIDSTART, "DnD_CharacterID") << 16), p2 | (CheckActorInventory(p2 + P_TIDSTART, "DnD_CharacterID") << 16));
		
		// take away confirmation stuff
		TakeActorInventory(p1 + P_TIDSTART, "DnD_Trade_Confirmed", 1);
		TakeActorInventory(p2 + P_TIDSTART, "DnD_Trade_Confirmed", 1);
		// finish trade view related stuff
		GiveActorInventory(p1 + P_TIDSTART, "DnD_CleanTradeviewRequest", 1);
		GiveActorInventory(p2 + P_TIDSTART, "DnD_CleanTradeviewRequest", 1);
		GiveActorInventory(p1 + P_TIDSTART, "DnD_Trade_Cooldown", 1);
		GiveActorInventory(p2 + P_TIDSTART, "DnD_Trade_Cooldown", 1);
		CancelTrade(p1);
		CancelTrade(p2);
	}
}

Script "DnD Trade Window" (int pnum, int giveWindow) {
	SetActivator(pnum + P_TIDSTART);
	
	if(giveWindow)
		GiveInventory("DnD_TradeAcceptWindow", 1);
	
	int lim = 5 * Clamp_Between(GetCVar("dnd_trade_window"), 5, 20);
	for(int i = 0; i < lim && !CheckInventory("InTradeView"); ++i)
		Delay(const:7);
	
	if(!CheckInventory("InTradeView")) {
		BreakTradesBetween(pnum);
		TakeInventory("DnD_TradeAcceptWindow", 1);
	}
}

Script "DnD Menu Popup" (int pnum, int isSell, int activebox) {
	SetActivator((pnum & 0xFFFF) + P_TIDSTART);
	if(isSell) {
		GiveInventory("DnD_SellConfirm", 1);
		TakeInventory("DnD_ShowPopup", 1);
		TakeInventory("DnD_PopupError", 1);
		GiveInventory("DnD_PopupSell", 1);
		GiveInventory("DnD_ShowSellPopup", 1);
		SetInventory("DnD_PopupID", activebox);
		SetInventory("ActivePopupBox", pnum >> 16);
	}
	else {
		GiveInventory("DnD_ShowPopup", 1);
		GiveInventory("DnD_PopupError", 1);
		TakeInventory("DnD_PopupSell", 1);
		TakeInventory("DnD_ShowSellPopup", 1);
		SetInventory("ActivePopupBox", 0);
		SetInventory("DnD_PopupID", activebox);
	}
	
	SendOwnerScript("DnD Menu Popup Show", pnum & 0xFFFF, pnum >> 16, pnum & 0xFFFF, isSell);
}

Script "DnD Menu Popup Show" (int id, int pnum, int isSell) CLIENTSIDE {
	// 0 for buy being possible, 2 if credits was not enough, 1 if credits was enough but you are maxed on the item and 3 if neither were satisfied 4 if item can't be sold
	SetActivator(pnum + P_TIDSTART);
	
	if(ConsolePlayerNumber() != pnum)
		Terminate;
	
	// a sale asks with the frame's confirm popup, with buttons
	if(isSell) {
		GetMchState().popanswered = false;
		DrawChromeSellPopup(MAINBOX_NONE);
		MenuHudSize(HUDMAX_X, HUDMAX_Y);
		Terminate;
	}

	// id is minimum 1; the card carries it as its code
	LocalAmbientSound("RPG/MenuError", 127);
	MchTermMessage(StrParam(l:"DND_MENU_ERROR"), StrParam(s:(id < 10) ? "E-0" : "E-", d:id), StrParam(l:GetPopupText(id)), true);
}

Script "DnD Menu Notif" (int pnum, int extra, int activebox) {
	int popupid = pnum >> 16;
	pnum &= 0xFFFF;

	SetActivator(pnum + P_TIDSTART);

	GiveInventory("DnD_ShowPopup", 1);
	SetInventory("ActivePopupBox", activebox);
	SetInventory("DnD_PopupID", popupid);
	
	SendOwnerScript("DnD Menu Notif Show", pnum, pnum, popupid, extra);
}

Script "DnD Menu Notif Show" (int pnum, int popupid, int extra) CLIENTSIDE {
	// 0 for buy being possible, 2 if credits was not enough, 1 if credits was enough but you are maxed on the item and 3 if neither were satisfied 4 if item can't be sold
	SetActivator(pnum + P_TIDSTART);
	
	if(ConsolePlayerNumber() != pnum)
		Terminate;

	// a scavenge request is the salvage confirm, with its buttons: the menu loop draws it once the request lands
	if(popupid == POPUP_SCAVENGECONFIRM)
		Terminate;
	MchTermMessage(StrParam(l:"DND_MENU_CONFIRM"), StrParam(s:(popupid < 10) ? "Q-0" : "Q-", d:popupid), StrParam(l:GetPopupText(popupid)), false);
}

Script "DnD Menu Sell Popup Clear" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	ClearTermMessage();
	DeleteTextRange(MCH_ID_POPFIRST, MCH_ID_POPLAST);
}

Script "DnD Menu Investment Sync" (int pnum, int resid, int val) CLIENTSIDE {
	ResearchInvestments[pnum][resid] = val;
	SetResultValue(0);
}

Script "DnD Open Inventory" (void) NET {
	ACS_ExecuteAlways(900, 0, MENU_LOAD_CHARM, 1);
}

Script "DnD Open Stash" (void) NET {
	ACS_ExecuteAlways(900, 0, MENU_LOAD_STASH, 2);
}

Script "DnD Dump to Stash" (int stackableOnly) NET {
	if(!CheckInventory("DnD_DumpCooldown") && CheckInventory("DnD_StashView")) {
		AutoDumpItems(PlayerNumber(), stackableOnly);
		GiveInventory("DnD_DumpCooldown", 1);
		SendOwnerScript("DnD Refresh Request", PlayerNumber(), PlayerNumber(), 1);
	}
}

// Auto sort one grid. source is the bare source id, page only matters for the stash -- each page
// sorts on its own, so switching tabs and pressing again sorts that tab.
Script "DnD Sort Inventory" (int source, int page) NET {
	int pnum = PlayerNumber();

	if(CheckInventory("DnD_SortCooldown"))
		Terminate;

	auto sort_state = GetSortState();

	// Wait our turn instead of queueing: the engine's own scheduling is the queue, so there is no
	// pending list to invalidate when a player leaves or changes page. With the cooldown above,
	// the window two players can collide in is a tic or two.
	int waited = 0;
	while(sort_state.busy && waited < DND_SORT_MAXWAIT) {
		Delay(const:1);
		++waited;
	}

	sort_state.busy = 1;
	GiveInventory("DnD_SortCooldown", 1);

	if(source == DND_SYNC_ITEMSOURCE_STASH)
		source |= page << 16;

	int count = SortInventoryGrid(pnum, source);

	Delay(const:1);

	// A repack moves nearly everything, so the whole grid is resynced. Deliberately in one tic and
	// not spread out: a box updates on the client the moment its sync lands, so yielding here shows
	// the grid half repacked for as many tics as the loop takes. The client sees the old layout
	// until this tic ends and the new one after it, with nothing in between.
	// Every box gets a definite state, including the third case this used to fall through: a cell in
	// the middle of a multi-cell item. Its topleft and type come from the owner's sync, but its own
	// fields do not, and a repack moves items across boxes that used to hold something else -- so a
	// middle cell kept the previous occupant's width, height, image and subtype. The crafting view
	// lists any box whose type is craftable and whose height is non-zero, so those cells showed up as
	// phantom entries: the old item's icon under the new owner's name, with an out of range subtype.
	for(int i = 0; i < MAX_INVENTORY_BOXES; ++i) {
		auto item = AcquireItemFromSource(pnum, i, source);
		if(item.topleftboxid == i + 1)
			SyncItemData(pnum, i, source, -1, -1);
		else if(item.item_type == DND_ITEM_NULL)
			SyncItemData_Null(pnum, i, source, 1, 1);
		else
			SyncItemData_ClearFields(pnum, i, source);
	}

	// A repack moves almost every box, and the per box syncs leave the icons of items that were
	// drawn where they no longer are. Redraw the pane outright rather than trying to chase them.
	SendOwnerScript("DnD Refresh Request", pnum, pnum, 1);

	sort_state.busy = 0;

#ifdef ISDEBUGBUILD
	if(count == -1)
		Log(s:"Auto sort could not repack source ", d:source, s:" -- grid restored.");
#endif

	SetResultValue(0);
}

Script "DnD Filter Click" (int pnum) CLIENTSIDE {
	if(ConsolePlayerNumber() != pnum)
		Terminate;
	int f = GetCVar("dnd_pickup_filter");
	++f;
	if(f > 7)
		f = 0;
	SetCVar("dnd_pickup_filter", f);
	NamedRequestScriptPuke("DND Set Pickup Filter", pnum, f);
	SetResultValue(0);
}

Script "DND Set Pickup Filter" (int pnum, int val) NET {
	if(!IsRequestFromPlayer(pnum))
		Terminate;
	SetActorInventory(pnum + P_TIDSTART, "PlayerPickupFilter", val);
}

Script "DnD Setup Menu Vars" (void) {
	GetShopItemInfo(0, 0);
	SetupComplete(SETUP_STATE1, SETUP_ITEMTABLES);
}

Script "DnD Setup Menu Vars - CS" (void) CLIENTSIDE {
	GetShopItemInfo(0, 0);
	SetupComplete(SETUP_STATE1, SETUP_ITEMTABLES);
	// Engines that wipe a client's globals on a map change rebuild here; the storage exists now, so ask for the data back.
	if(GameType() != GAME_SINGLE_PLAYER && ConsolePlayerNumber() >= 0)
		NamedRequestScriptPuke("DnD Map Resync");
}

#ifdef ISAPRILFIRST
Script "DnD NFT Announce" (int pnum, int id) CLIENTSIDE {
	AmbientSound("CoolNFT", 127);
	
	int cnt = 10 * TICRATE + 17;
	int u = 0;
	int x = 640.0;
	int y = 480.0;
	
	int r = 200;
	
	// only log first instance
	int f = HUDMSG_LOG | HUDMSG_PLAIN | HUDMSG_ALPHA;
	
	while(cnt--) {
		x = 640.0 - r * sin(u);
		y = 480.0 - r * sin(u) * 4 / 3;
		SetHudSize(x >> 16, y >> 16, 1);
		SetFont("DBIGFONT");
		HudmessageBold(
			n:pnum, s: " \cfHAS JUST BOUGHT THE \cv", l:StrParam(s:"DND_NFT_TAG", d:id), s: "\cf NFT!!!!";
			f, 1, CR_GOLD, GetIntegerBits(x / 2) + 0.4, 0.1 + GetIntegerBits(y / 30), 0.1, 0.25 + Clamp_Between(FixedMul(0.75, sin(u)), 0, 0.75)
		);
		u = (u + 0.0075) % 1.0;
		Delay(1);
		f = HUDMSG_PLAIN | HUDMSG_ALPHA;
	}
}
#endif

#endif
