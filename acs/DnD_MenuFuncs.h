#include "DnD_MenuTables.h"

void UpdateMenuPosition(int x, int option) {
	if(x >= 0) {
		// push unique pages
		if(CheckInventory("MenuOption") != option) {
			menu_stack_T? stack = GetMenuStack(PlayerNumber());
			PushStack(PlayerNumber(), option);
		}
	}
	else
		x = 0;
	SetInventory("MenuPosX", x);
	SetInventory("MenuLR", 0);
	SetInventory("MenuUD", 0);
	SetInventory("MenuOption", option);
	LocalAmbientSound("RPG/MenuChoose", 127);
}

int CalculateWisdomBonus(int pnum) {
	return Player_Bonuses[PlayerNumber()].wisdom_percent_bonus + GetDataFromOrbBonus(pnum, OBI_WISDOMPERCENT, -1) + CheckInventory("Perk_Wisdom") * BASE_WISDOM_GAIN;
}

int CalculateGreedBonus(int pnum) {
	return Player_Bonuses[PlayerNumber()].greed_percent_bonus + GetDataFromOrbBonus(pnum, OBI_GREEDPERCENT, -1) + CheckInventory("Perk_Greed") * BASE_WISDOM_GAIN;
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

void DeleteText(int textid) {
	HudMessage(s:""; HUDMSG_PLAIN, textid, -1, 160.0, 100.0, 0.0, 0.0);
}

void DeleteTextRange(int r1, int r2) {
	for(int i = 0; i < r2 - r1 + 1; i++)
		HudMessage(s:""; HUDMSG_PLAIN, r1 + i, -1, 160.0, 100.0, 0.0, 0.0);
}

void ShowNeedResearchPopup() {
	LocalAmbientSound("RPG/MenuError", 127);
	GiveInventory("DnD_ShowPopup", 1);
	GiveInventory("DnD_PopupError", 1);
	SetInventory("DnD_PopupId", POPUP_NEEDRESEARCH); // popup for research
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
	return Player_Bonuses[pnum].damage_type_bonus[dtype] + GetDataFromOrbBonus(pnum, OBI_DAMAGETYPE, dtype);
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
		case MENU_SHOP_WEAPON4:
		return SHOP_WEAPON4_BEGIN;
		case MENU_SHOP_WEAPON5:
		return SHOP_WEAPON5_BEGIN;
		case MENU_SHOP_WEAPON6:
		return SHOP_WEAPON6_BEGIN;
		case MENU_SHOP_WEAPON7:
		return SHOP_WEAPON7_BEGIN;
		case MENU_SHOP_WEAPON8:
		return SHOP_WEAPON8_BEGIN;
	}
	return 0;
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

void ShowOrbIcon(int id) {
	SetFont(OrbIcons[id]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_HELPTEXT_ORBS - id - 2, CR_WHITE, 237.4, 60.1 + 96.0 * id + 8.0 * ScrollPos, 0.0, 0.0);
	SetFont("SMALLFONT");
}

void ShowLegendaryMonsterIcon(int id, int j) {
	SetFont(LegendaryMonsterIcons[id]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - MAX_HELPTEXT_ORBS - id - 2, CR_WHITE, 256.4, 56.1 + 144.0 * j + 4.0 * ScrollPos, 0.0, 0.0);
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
	if(id >= SHOP_WEAPON4_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON5_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON6_BEGIN)
		skip += 1;
	if(id >= SHOP_WEAPON7_BEGIN)
		skip += 1;
	return Weapons[id + skip][WEAPON_ICON];
}

int GetItemBeginIndex(int opt) {
	if(opt >= MENU_SHOP_WEAPON1 && opt <= MENU_SHOP_WEAPON8)
		return WeaponBeginIndexes[opt - MENU_SHOP_WEAPON1];
	if(opt >= SHOP_FIRSTAMMO_PAGE && opt <= SHOP_LASTAMMO_PAGE)
		return AmmoBeginIndexes[opt - SHOP_FIRSTAMMO_PAGE];
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
		if(opt >= MENU_SHOP_AMMO1 && opt <= MENU_SHOP_AMMO_SPECIAL1) {
			beginindex = GetItemBeginIndex(opt) - SHOP_FIRSTAMMO_INDEX;
			toshow = GetTextWithResearch(AmmoInfo[boxid + beginindex][AMMO_ICON], "", AmmoDrawInfo[boxid + beginindex].res_id, RES_KNOWN, AmmoDrawInfo[boxid + beginindex].flags);
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
	SetFont(AccessoryImages[acc]);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - DND_MENU_ACCESSORYITEMS * i - 2, CR_WHITE, 421.4, 96.1 + 144.0 * i + 6.0 * ScrollPos, 0.0, 0.0);
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
	}
	return 1;
}

int GetShopPrice (int id, int priceflag) {
	int res = 0, chr = 0, shop_scale = Clamp_Between(GetCVar("dnd_shop_scale"), 1, SHOP_SCALE_MAX);
	if(id < MAXSHOPITEMS)
		res = ShopInfo[id][SHOPINFO_PRICE] * shop_scale;
	res = ShopScale(res, id);
	if(GetItemType(id) == TYPE_TALENT) {
		res += TALENT_COST_INCREASE * CheckInventory(TalentNames[id - SHOP_TALENT_BEGIN]) * shop_scale;
		if(CheckInventory(TalentNames[id - SHOP_TALENT_BEGIN]) >= TALENT_SCALE_DOUBLER_MARK)
			res *= 2 * CheckInventory(TalentNames[id - SHOP_TALENT_BEGIN]) / TALENT_SCALE_DOUBLER_MARK;
	}
	if(priceflag & PRICE_CHARISMAREDUCE) {
		chr = CheckInventory("PSTAT_Charisma");
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
int CanResearch(int res_id) {
	int finish_check = !CheckInventory(StrParam(s:"Done_", s:Research_List[res_id]));
	int found_check = CheckInventory(StrParam(s:"Research_", s:Research_List[res_id]));
	int budget_check = CheckInventory("Budget") >= ResearchCosts[res_id];
	if(finish_check && found_check && budget_check)
		return 0;
	GiveInventory("DnD_PopupError", 1);
	if(!found_check)
		SetInventory("DnD_PopupId", POPUP_NEEDDISCOVER);
	else if(!finish_check)
		SetInventory("DnD_PopupId", POPUP_ALREADYRESEARCHED);
	else
		SetInventory("DnD_PopupId", POPUP_NOBUDGET);
	return 1;
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
	HudMessage(s:color, s:label, s:": \c-", d:afterlabel; HUDMSG_PLAIN, drawid, CR_WHITE, hudx, hudy, 0.0, 0.0);
}

// Draws a toggled image that changes color depending on given scenarios
// By default, if insufficient credits occur, it will be gray. If credits are sufficient but objectflag has OBJ_HASCHOICE, that means you have to choose between one of the options
// of it's kind. For example, there are 2 weapons that replace the shotgun. You can have only one, so you set this flag, and set choicename to P_Slot3Replaced. One of the two will be
// red if the other is bought. The item that is bought will be green.
void DrawToggledImage(int itemid, int onposy, int objectflag, int offcolor, int oncolor, str choicename, int choicecount, int choicecolor) {
	// first of all check research (assuming the player can't own this item without having it researched)
	int res_state = 1;
	if(objectflag & OBJ_RESEARCH) {
		res_state = CheckItemRequirements(itemid, RES_KNOWN, objectflag); // if not known, draw nothing
	}
	int curposy = CheckInventory("ActiveBox") - 1;
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
		int price = GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
		int sellstate = 0;
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
				if(!(objectflag & OBJ_ARTI) && CheckInventory(itemname)) {
					color = oncolor;
					colorprefix = "\c[M3]";
					toshow = "\c[M3]";
					sellstate = 1;
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
				SetHudClipRect(184, 208, 256, 64, 256, 1);
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
				HudMessage(s:"* ", s:"Increases damage of ", s:TalentTypeNames[onposy], s:" weapons by \cf", f:GetMenuTalentBonus(onposy), s:"%\c-."; HUDMSG_PLAIN, RPGMENUITEMID - 40, CR_WHITE, 192.1, 232.1, 0.0, 0.0);
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
			HudMessage(s:weptype, s:"\c[B1]", s:itemtag; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
			HudMessage(s:colorprefix, s:"--> $", d:price; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, color, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
		else {
			HudMessage(s:weptype, s:toshow, s:itemtag; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 1, CR_WHITE, 192.1, 80.0 + 16.0 * onposy, 0.0, 0.0);
			HudMessage(s:colorprefix, s:"$", d:price; HUDMSG_PLAIN, RPGMENUITEMID - 2 * onposy - 2, color, 440.2, 80.0 + 16.0 * onposy, 0.0, 0.0);
		}
	}
}

// will process item selections depending on given valid range
// support for selling other stuff is here, it's just a few extra lines in the serverside script to handle the process
void ProcessTrade (int posy, int low, int high, int tradeflag) {
	int itemid, price, buystatus;
	if(tradeflag & TRADE_BUY) {
		itemid = low + posy;
		if(itemid <= high && CheckInventory("MadeChoice") == 1) {
			// consider the research if this item has any
			if(!CheckItemRequirements(itemid, RES_DONE, GetItemFlags(itemid))) {
				ShowNeedResearchPopup();
			}
			else {
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
						LocalAmbientSound("weapons/pickup", 127);
						TakeInventory(ShopWeaponTake[itemid], 1);	
						GiveInventory(ShopItemNames[itemid][SHOPNAME_CONDITION], 1);
						SetWeapon(ShopItemNames[itemid][SHOPNAME_ITEM]);
                        // fix special ammo cursor
                        int fix = IsSpecialFixWeapon(itemid);
                        if(fix != -1) {
                            int weptype = SpecialAmmoFixWeapons[fix][1];
                            SetInventory(StrParam(s:"SpecialAmmoMode", s:GetSpecialAmmoSuffix(weptype)), SpecialAmmoBase[SpecialAmmoFixWeapons[fix][2]]);
                        }
                    }
					else if(tradeflag & TRADE_AMMO)
						LocalAmbientSound("items/ammo", 127);
					else if(tradeflag & (TRADE_ABILITY | TRADE_ARTIFACT | TRADE_TALENT)) {
						if(tradeflag & TRADE_ARTIFACT)
							SetInventory("DnD_Artifact_MapBits", SetBit(CheckInventory("DnD_Artifact_MapBits"), itemid - SHOP_FIRSTARTI_INDEX));
						if(tradeflag & TRADE_TALENT)
							TakeInventory("TalentPoint", 1);
						LocalAmbientSound("Bonus/Received", 127);
					}
					else if(tradeflag & TRADE_ARMOR)
						LocalAmbientSound("items/armor", 127);
				}
				else {
					LocalAmbientSound("RPG/MenuError", 127);
					GiveInventory("DnD_ShowPopup", 1);
				}
			}
		}
		SetInventory("MadeChoice", 0);
	}
	else if(tradeflag & TRADE_SELL) {
		itemid = low + posy;
		if(itemid <= high && CheckInventory("MadeChoice") == 2) {
			if(!CheckInventory("DnD_SellConfirm")) { // confirmation screen did not pop
				buystatus = CanTrade(itemid, TRADE_SELL, 0);
				if(buystatus) {
					LocalAmbientSound("RPG/MenuSellConfirm", 127);
					GiveInventory("DnD_SellConfirm", 1);
					SetInventory("ActivePopupBox", itemid);
					TakeInventory("DnD_ShowPopup", 1);
					TakeInventory("DnD_PopupError", 1);
					GiveInventory("DnD_PopupSell", 1);
					GiveInventory("DnD_ShowSellPopup", 1);
					SetInventory("DnD_PopupID", itemid);
				}
				else {
					GiveInventory("DnD_ShowPopup", 1);
					GiveInventory("DnD_PopupError", 1);
					SetInventory("ActivePopupBox", 0);
					SetInventory("DnD_PopupId", POPUP_DONTOWN);
					LocalAmbientSound("RPG/MenuError", 127);
				}
			}
			else {
				itemid = CheckInventory("ActivePopupBox");
				price = GetShopPrice(itemid, 0) / 2;
				TakeInventory("DnD_SellConfirm", 1);
				TakeInventory("DnD_PopupSell", 1);
				TakeInventory("DnD_ShowSellPopup", 1);
				SetInventory("DnD_PopupID", 0);
				SetInventory("ActivePopupBox", 0);
				str totake = ShopWeaponTake[itemid];
				if(StrCmp(totake, " "))
					GiveInventory(totake, 1);
				TakeInventory(ShopItemNames[itemid][SHOPNAME_CONDITION], 1);
				TakeInventory(ShopItemNames[itemid][SHOPNAME_ITEM], 1);
				GiveInventory("Credit", price);
			}
		}
		SetInventory("MadeChoice", 0);
	}
}

// 0 for buy being possible, 2 if credits was not enough, 1 if credits was enough but you are maxed on the item and 3 if neither were satisfied 4 if item can't be sold
void HandlePopups(int id) {
	if(!CheckInventory("DnD_ShowPopup") && !CheckInventory("DnD_ShowSellPopup"))
		return;
	int popuptype = POPUP_ERROR;
	if(CheckInventory("DnD_PopupSell"))
		popuptype = POPUP_SELL;

	SetHudSize(480, 320, 1);
	SetFont("DND_ERR");
	HudMessage(s:"A"; HUDMSG_FADEOUT, RPGMENUITEMID - 45, -1, 216.1, 128.1, 1.0, 1.0);
	SetFont("SMALLFONT");
	str toshow = "", title = " ";
	if(popuptype == POPUP_ERROR) {
		title = "\c[Q2]ERROR";
		// id is minimum 1
		toshow = PopupText[id];
	}
	else if(popuptype == POPUP_SELL) {
		title = "\c[Q2]CONFIRM";
		toshow = StrParam(s:"Are you sure you\nwant to sell your\n\cd", s:ShopItemNames[id][SHOPNAME_TAG], s:"\c- ?");
	}

	HudMessage(s:title; HUDMSG_FADEOUT, RPGMENUITEMID - 46, -1, 316.4, 136.1, 1.0, 1.0);
	HudMessage(s:toshow; HUDMSG_FADEOUT, RPGMENUITEMID - 47, CR_GOLD, 224.1, 160.1, 1.0, 1.0);
}

void DrawHighLightBar (int posy, int drawlit) {
	if(posy == MAINBOX_NONE)
		HudMessage(s:""; HUDMSG_PLAIN, RPGMENULISTID - 7, -1, 47.1, 99.1, 0.0, 0.0);
	else {
		--posy;
		int yadd = 0;
		if(posy > 0)
			yadd = 11 * posy;
		if(posy > 3)
			yadd += 8;
		SetHudSize(384, 200, 1);
		if(drawlit)
			SetFont("BARHIGHL");
		else
			SetFont("BARHIGH");
		HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENULISTID - 7, -1, 47.1, 99.1 + 1.0 * yadd, 0.0, 0.0);
	}
}

void ReturnToMain() {
	SetInventory("MenuOption", MENU_MAIN);
	SetInventory("MenuPosX", 0);
	LocalAmbientSound("RPG/MenuChoose", 127);
}

// includes left right shortcuts
void ListenInput(int listenflag, int curposx, int condx_min, int condx_max) {
	int bpress = GetPlayerInput(-1, INPUT_BUTTONS);
	int obpress = GetPlayerInput(-1, INPUT_OLDBUTTONS);
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
				SetInventory("MenuLR", 1);
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
				SetInventory("MenuLR", 2);
			}
		}
		// implement server - client synced scroll here using inventory sometime
	}
	
	if(!CheckInventory("DnD_ClickTicker")) {
		if(IsButtonPressed(bpress, obpress, BT_USE) || IsButtonPressed(bpress, obpress, BT_ATTACK)) {
			GiveInventory("DnD_ClickTicker", 1);
			if(CheckInventory("DnD_SellConfirm")) {
				SetInventory("DnD_SellConfirm", 0);
				SetInventory("DnD_PopupID", 0);
				SetInventory("DnD_PopupSell", 0);
				SetInventory("DnD_ShowPopup", 0);
				SetInventory("DnD_ShowSellPopup", 0);
				SetInventory("MadeChoice", 0);
			}
			else
				SetInventory("MadeChoice", 1);
		}
		else if(IsButtonPressed(bpress, obpress, BT_ALTATTACK)) {
			GiveInventory("DnD_ClickTicker", 1);
			SetInventory("MadeChoice", 2);
		}
		else
			SetInventory("MadeChoice", 0);
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

void HandleAmmoPurchase(int boxid, int index_beg, int arr_index, bool givefull) {
	int itemid = index_beg + boxid - 1;
	
	if(!CheckItemRequirements(itemid, RES_DONE, GetItemFlags(itemid))) {
		// not done, so we can't give this
		ShowNeedResearchPopup();
	}
	else {
		int price = GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
		int buystatus = CanTrade(itemid, TRADE_BUY, price);
		
		if(!buystatus) {
			int amt = AmmoCounts[arr_index], count = 1;
			amt += ACS_ExecuteWithResult(918, 0, 1, amt);
			
			// if we are maxing the ammo
			if(givefull) {
				count += (GetAmmoCapacity(AmmoInfo[arr_index][AMMO_NAME]) - CheckInventory(AmmoInfo[arr_index][AMMO_NAME])) / amt;
				price = price * count;
				if(price > CheckInventory("Credit")) {
					count = CheckInventory("Credit") / GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
					price = count * GetShopPrice(itemid, PRICE_CHARISMAREDUCE);
				}
			}
			// check how much of this we can really afford
			
			TakeInventory("Credit", price);
			LocalAmbientSound("items/ammo", 127);
			GiveInventory(AmmoInfo[arr_index][AMMO_NAME], amt * count);
			
			GiveInventory("DnD_MoneySpentQuest", price);
		}
		else {
			LocalAmbientSound("RPG/MenuError", 127);
			GiveInventory("DnD_ShowPopup", 1);
		}
	}
}

str GetResearchImage(int pos) {
    return Research_Images[pos];
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

menu_pane_T& GetPane() {
	static menu_pane_T pane;
	return pane;
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
			{ 288.0, 164.0, 216.0, 157.0 }, // str
			{ 149.0, 164.0, 72.0, 157.0 }, // dex
			{ 288.0, 148.0, 213.0, 141.0 }, // bul
			{ 149.0, 148.0, 78.0, 141.0 }, // chr
			{ 288.0, 132.0, 216.0, 125.0 }, // vit
			{ 149.0, 132.0, 78.0, 125.0 }, // int
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
			{ 289.0, 229.0, 179.0, 222.0 }, // loadout 1
			{ 289.0, 211.0, 162.0, 206.0 }, // loadout 2
			{ 289.0, 195.0, 178.0, 190.0 }, // loadout 3
			{ 289.0, 181.0, 169.0, 174.0 }, // loadout 4
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
		// loadout 4 -- all the clickables in the respective order (95 diff on y, scrollpos adds (1 per movement) x 4 in loadout speed)
		// we really only generate the y positions for the boxes, and what the boxes are is arbitrary
		// we save x pairs here and load
		{
			{ 226.0, 268.0, 102.0, 260.0 },
			{ 224.0, 268.0, 107.0, 260.0 },
			{ 210.0, 268.0, 119.0, 260.0 },
			{ 240.0, 268.0, 87.0, 260.0 },
			{ 216.0, 268.0, 113.0, 260.0 },
			{ 240.0, 268.0, 90.0, 260.0 },
			{ 213.0, 268.0, 116.0, 260.0 },
			{ 208.0, 268.0, 120.0, 260.0 },
			{ 207.0, 268.0, 120.0, 260.0 },
			{ 226.0, 268.0, 102.0, 260.0 },
			{ 242.0, 268.0, 86.0, 260.0 },
			{ 220.0, 268.0, 108.0, 260.0 },
			{ 197.0, 268.0, 131.0, 260.0 },
			{ 232.0, 268.0, 97.0, 260.0 },
			{ 226.0, 268.0, 106.0, 260.0 },
			{ 244.0, 268.0, 86.0, 260.0 },
			{ 236.0, 268.0, 96.0, 260.0 },
			{ -1, -1, -1, -1 }
		},
		// shop
		{
			{ 289.0, 229.0, 179.0, 222.0 }, // wep
			{ 289.0, 211.0, 162.0, 206.0 }, // ammo
			{ 289.0, 195.0, 178.0, 190.0 }, // ability
			{ 289.0, 181.0, 169.0, 174.0 }, // arti
			{ 289.0, 164.0, 183.0, 158.0 }, // talent
			{ 289.0, 148.0, 188.0, 142.0 }, // armor
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
			{ -1, -1, -1, -1 }
		},
		// wep 4
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
		// wep 5
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
		// wep 6
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
		// wep 7
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
		// wep 8
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
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
			{ 289.0, 101.0, 104.0, 95.0 }, // w10
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
		// ammo special
		{
			{ 289.0, 245.0, 120.0, 239.0 }, // w1
			{ 289.0, 229.0, 120.0, 223.0 }, // w2
			{ 289.0, 213.0, 120.0, 207.0 }, // w3
			{ 289.0, 197.0, 120.0, 191.0 }, // w4
			{ 289.0, 181.0, 120.0, 175.0 }, // w5
			{ 289.0, 165.0, 120.0, 159.0 }, // w6
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
		// research
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
			{ 289.0, 213.0, 162.0, 206.0 }, // res
			{ 289.0, 197.0, 178.0, 190.0 }, // dmg
			{ 289.0, 181.0, 178.0, 174.0 }, // orb
			{ 289.0, 165.0, 178.0, 158.0 }, // leg
			{ 289.0, 149.0, 179.0, 142.0 }, // mods
			{ 296.0, 81.0, 182.0, 73.0 }, // show info
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
		case MENU_LOAD4:
			base = 3.9625;
		break;
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
			UpdateMenuPosition(-1, stack.stack_elems[stack.stackptr]);
		}
		else if(boxid == MAINBOX_RARR) {
			stack.stackptr = stack.stackptr < stack.cursize - 1 ? stack.stackptr + 1 : stack.cursize - 1;
			UpdateMenuPosition(-1, stack.stack_elems[stack.stackptr]);
		}
	}
}