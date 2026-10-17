#ifndef DND_GEMS_IN
#define DND_GEMS_IN

// Gems socket into spells. The colour is
// the subtype's low byte, DND_GEM_GREATER marks a greater gem, and the role sits in implicit 0 (-1 for none).
enum {
	DND_GEM_RUBY,
	DND_GEM_SAPPHIRE,
	DND_GEM_DIAMOND,
	DND_GEM_EMERALD,
	DND_GEM_TOPAZ,
	DND_GEM_AMETHYST,
	DND_GEM_OBSIDIAN,
	DND_GEM_BISMUTH,
	DND_GEM_COLOURS
};
#define DND_GEM_GREATER 0x100
#define MAX_GEM_KINDS (DND_GEM_COLOURS * 2)

// Saved by id in the role's implicit, so append only.
enum {
	DND_GEMROLE_MARKSMAN,
	DND_GEMROLE_WIZARD,
	DND_GEMROLE_CONQUEROR,
	DND_GEMROLE_BLADEDANCER,
	DND_GEMROLE_FIGHTER,
	DND_GEMROLE_CONJURER,
	DND_GEMROLE_LEADER,
	DND_GEMROLE_CONSERVATIONIST,
	DND_GEMROLE_MAGUS,
	DND_GEMROLE_SWIFTCASTER,
	DND_GEMROLE_SUMMONER,
	DND_GEMROLE_PRIEST,
	DND_GEMROLE_WITCH,
	DND_GEMROLE_CRUEL,
	DND_GEMROLE_PYROMANCER,
	DND_GEMROLE_CRYOMANCER,
	DND_GEMROLE_VENOMANCER,
	DND_GEMROLE_ELECTROMANCER,
	DND_GEMROLE_VAMPIRE,
	DND_GEMROLE_INFLUENCER,
	DND_GEMROLE_COMMANDER,
	DND_GEMROLE_QUICKTONGUE,
	DND_GEMROLE_ARCHMAGE,
	DND_GEMROLE_ASSASSIN,
	DND_GEMROLE_NECROMANCER,
	DND_GEMROLE_SANGUINOMANCER,
	DND_GEMROLE_DESTROYER,
	DND_GEMROLE_RESOLUTE,
	DND_GEMROLE_RAVAGER,
	DND_GEMROLE_POWERFUL,
	DND_GEMROLE_MERCILESS,
	DND_GEMROLE_EXECUTIONER,
	DND_GEMROLE_HAEMOCRAFTER,
	DND_GEM_ROLES
};
#define DND_GEMROLE_NONE -1

// ---- drop tuning ------------------------------------------------------------------------------
#define DND_GEM_MINLEVEL 33					// monster level for a kill, the player's own for a chest; never below
#define DND_GEM_GREATERCHANCE 4				// percent of gems that come out greater
#define DND_GEM_GREATERCHANCE_BOSS 12
#define DND_LOOTBOX_GEMCHANCE 0.03			// the loot chest monsters drop
#define DND_GEM_CHESTCHANCE_BRONZE 0.04
#define DND_GEM_CHESTCHANCE_SILVER 0.08
#define DND_GEM_CHESTCHANCE_GOLD 0.15

#define DND_GEMROLE_WEIGHT_NONE 600			// a gem with no role
#define DND_GEMROLE_WEIGHT_COMMON 100
#define DND_GEMROLE_WEIGHT_UNCOMMON 60
#define DND_GEMROLE_WEIGHT_RARE 25
#define DND_GEMROLE_WEIGHT_VERYRARE 8

// Colour rarity.
int GetGemColourWeight(int colour) {
	switch(colour) {
		case DND_GEM_RUBY:
		case DND_GEM_SAPPHIRE:
		return 100;
		case DND_GEM_EMERALD:
		case DND_GEM_TOPAZ:
		return 90;
		case DND_GEM_DIAMOND:
		case DND_GEM_AMETHYST:
		return 50;
		case DND_GEM_OBSIDIAN:
		return 35;
	}
	// Bismuth, the three in one.
	return 20;
}

// Role rarity: move a role between tiers to retune it.
int GetGemRoleWeight(int role) {
	switch(role) {
		case DND_GEMROLE_SWIFTCASTER:
		case DND_GEMROLE_CONSERVATIONIST:
		case DND_GEMROLE_MAGUS:
		case DND_GEMROLE_FIGHTER:
		case DND_GEMROLE_BLADEDANCER:
		case DND_GEMROLE_LEADER:
		case DND_GEMROLE_PRIEST:
		case DND_GEMROLE_INFLUENCER:
		return DND_GEMROLE_WEIGHT_COMMON;

		case DND_GEMROLE_WIZARD:
		case DND_GEMROLE_CRUEL:
		case DND_GEMROLE_CONQUEROR:
		case DND_GEMROLE_PYROMANCER:
		case DND_GEMROLE_CRYOMANCER:
		case DND_GEMROLE_VENOMANCER:
		case DND_GEMROLE_ELECTROMANCER:
		case DND_GEMROLE_SANGUINOMANCER:
		case DND_GEMROLE_SUMMONER:
		case DND_GEMROLE_COMMANDER:
		case DND_GEMROLE_WITCH:
		return DND_GEMROLE_WEIGHT_UNCOMMON;

		case DND_GEMROLE_MARKSMAN:
		case DND_GEMROLE_MERCILESS:
		case DND_GEMROLE_DESTROYER:
		case DND_GEMROLE_VAMPIRE:
		case DND_GEMROLE_ASSASSIN:
		case DND_GEMROLE_RAVAGER:
		case DND_GEMROLE_POWERFUL:
		case DND_GEMROLE_RESOLUTE:
		case DND_GEMROLE_EXECUTIONER:
		case DND_GEMROLE_NECROMANCER:
		case DND_GEMROLE_HAEMOCRAFTER:
		return DND_GEMROLE_WEIGHT_RARE;
	}
	return DND_GEMROLE_WEIGHT_VERYRARE;
}

// Roles that drop only from bosses; add or remove cases to retag them.
bool IsGemRoleBossOnly(int role) {
	switch(role) {
		case DND_GEMROLE_CONJURER:
		case DND_GEMROLE_QUICKTONGUE:
		case DND_GEMROLE_ARCHMAGE:
		case DND_GEMROLE_EXECUTIONER:
		case DND_GEMROLE_HAEMOCRAFTER:
		case DND_GEMROLE_NECROMANCER:
		return true;
	}
	return false;
}

// A dungeon at boss tier (its level at the regular item level cap or past it): any gem found there may roll a boss role.
bool IsBossTierDungeon() {
	return InformationInLevel[LEVELINFO_ISDUNGEON] && DungeonInformation.level >= MAX_REGULAR_ILVL;
}

// A boss for gem drops: the boss list, a legendary, or a dungeon's final boss.
bool IsGemBossSource(int m_id) {
	int id = MonsterProperties[m_id].id;
	return IsMonsterIdBoss(id) || isLegendaryMonster(id) || (MonsterProperties[m_id].flags & DND_MONFLAG_ISDUNGEONBOSS);
}

// Three rows: colours, roles off a boss, roles off anything else. The role rows put "no role" after the last role.
void SetupGemDropWeights() {
	INIT_ITEM_WEIGHTS;
	for(int i = 0; i < DND_GEM_COLOURS; ++i) {
		SET_ITEM_WEIGHT(DND_DROPPEDITEM_GEM, i, GetGemColourWeight(i));
	}

	RESET_ITEM_WEIGHTS;
	for(i = 0; i < DND_GEM_ROLES; ++i) {
		SET_ITEM_WEIGHT(DND_DROPPEDITEM_GEMROLE_BOSS, i, GetGemRoleWeight(i));
	}
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_GEMROLE_BOSS, DND_GEM_ROLES, DND_GEMROLE_WEIGHT_NONE);

	// The weight goes in as one value: inside the macro, "weight + cond ? 0 : w" parses as "(weight + cond) ? 0 : w".
	RESET_ITEM_WEIGHTS;
	int w;
	for(i = 0; i < DND_GEM_ROLES; ++i) {
		w = IsGemRoleBossOnly(i) ? 0 : GetGemRoleWeight(i);
		SET_ITEM_WEIGHT(DND_DROPPEDITEM_GEMROLE, i, w);
	}
	SET_ITEM_WEIGHT(DND_DROPPEDITEM_GEMROLE, DND_GEM_ROLES, DND_GEMROLE_WEIGHT_NONE);
}

// Index into a cumulative weight row; a zero weight entry can never be picked.
int PickGemWeighted(int row, int last) {
	int i, r = random(1, ItemDropWeights[row][last]);
	for(i = 0; i < last; ++i)
		if(r <= ItemDropWeights[row][i])
			break;
	return i;
}

// ---- subtype helpers --------------------------------------------------------------------------
int GetGemColour(int subt) {
	return subt & 0xFF;
}

bool IsGreaterGem(int subt) {
	return !!(subt & DND_GEM_GREATER);
}

// Ruby, Greater Ruby, Sapphire, ...: what names, images and drop actors index by.
int GetGemIndex(int subt) {
	return GetGemColour(subt) * 2 + IsGreaterGem(subt);
}

str GetGemDropClass(int subt) {
	return StrParam(s:"GemDrop_", d:GetGemIndex(subt));
}

// ---- colour terms -----------------------------------------------------------------------------
// What a colour grants, as GEMSTAT terms. Values are whole percents; a "less" is negative.
int GetGemColourTermCount(int colour) {
	return colour == DND_GEM_BISMUTH ? 3 : 1;
}

int GetGemColourTermStat(int colour, int k) {
	switch(colour) {
		case DND_GEM_RUBY:
		return GEMSTAT_MORE_DAMAGE;
		case DND_GEM_SAPPHIRE:
		return GEMSTAT_MORE_COST;
		case DND_GEM_DIAMOND:
		return GEMSTAT_CRITCHANCE;
		case DND_GEM_EMERALD:
		return GEMSTAT_CDR;
		case DND_GEM_TOPAZ:
		return GEMSTAT_MORE_HASTE;
		case DND_GEM_AMETHYST:
		return GEMSTAT_CRITMULT;
		case DND_GEM_OBSIDIAN:
		return GEMSTAT_MORE_DURATION;
	}

	// Bismuth: damage, cost, haste.
	if(!k)
		return GEMSTAT_MORE_DAMAGE;
	return k == 1 ? GEMSTAT_MORE_COST : GEMSTAT_MORE_HASTE;
}

int GetGemColourTermValue(int colour, int k, bool greater) {
	switch(colour) {
		case DND_GEM_SAPPHIRE:
		return greater ? -25 : -15;
		case DND_GEM_DIAMOND:
		return greater ? 10 : 5;
		case DND_GEM_AMETHYST:
		return greater ? 60 : 30;
		case DND_GEM_BISMUTH:
		if(k == 1)
			return greater ? -10 : -5;
		return greater ? 10 : 5;
	}
	return greater ? 25 : 15;
}

// ---- text -------------------------------------------------------------------------------------
str GetGemRoleNameLump(int role) {
	return StrParam(s:"DND_GEMROLE", d:role + 1);
}

str GetGemRoleTextLump(int role, bool greater) {
	return StrParam(s:"DND_GEMROLETXT", s:greater ? "G" : "", d:role + 1);
}

// One term as a tooltip line: "15% more damage", "15% less mana cost", "+5% spell critical strike chance".
str GetGemStatText(int stat, int v) {
	if(IsGemStatMore(stat))
		return StrParam(d:abs(v), s:"% ", l:v < 0 ? "DND_GEM_LESS" : "DND_GEM_MORE", s:" ", l:StrParam(s:"DND_GEMSTAT", d:stat));
	return StrParam(s:"+", d:v, s:stat == GEMSTAT_EXTRAPROJ ? " " : "% ", l:StrParam(s:"DND_GEMSTAT", d:stat));
}

// The colour's lines, then the role's.
str GetGemEffectText(int subt, int role) {
	int c = GetGemColour(subt), k;
	bool g = IsGreaterGem(subt);
	str t = "";
	for(k = 0; k < GetGemColourTermCount(c); ++k)
		t = StrParam(s:t, s:k ? "\n" : "", s:GetGemStatText(GetGemColourTermStat(c, k), GetGemColourTermValue(c, k, g)));

	if(role != DND_GEMROLE_NONE)
		t = StrParam(s:t, s:"\n\c[Y5]", l:GetGemRoleNameLump(role), s:": \c-", l:GetGemRoleTextLump(role, g));
	return t;
}

// "Ruby" or "Greater Ruby", then "of the <Role>" when it has one.
str GetGemFullName(int subt, int role) {
	if(role == DND_GEMROLE_NONE)
		return StrParam(l:GetItemTagName(DND_ITEM_GEM, subt));
	return StrParam(l:GetItemTagName(DND_ITEM_GEM, subt), s:" ", l:"DND_GEM_OFTHE", s:" ", l:GetGemRoleNameLump(role));
}

// ---- rolling and spawning ---------------------------------------------------------------------
// colour, greater and role of -1 roll their own; boss widens the role pool and the greater chance, boss_roles the pool alone.
void RollGemInfo(int item_pos, int colour = -1, int greater = -1, int role = -1, bool boss = false, bool boss_roles = false) {
	if(colour == -1)
		colour = PickGemWeighted(DND_DROPPEDITEM_GEM, DND_GEM_COLOURS - 1);
	if(greater == -1)
		greater = random(1, 100) <= (boss ? DND_GEM_GREATERCHANCE_BOSS : DND_GEM_GREATERCHANCE);
	if(role == -1) {
		role = PickGemWeighted(boss || boss_roles ? DND_DROPPEDITEM_GEMROLE_BOSS : DND_DROPPEDITEM_GEMROLE, DND_GEM_ROLES);
		if(role == DND_GEM_ROLES)
			role = DND_GEMROLE_NONE;
	}
	else if(role >= DND_GEM_ROLES)
		role = DND_GEMROLE_NONE;

	auto item = GetFieldItem(item_pos);
	item.item_level = 1;
	item.item_stack = 0;
	item.item_type = DND_ITEM_GEM;
	item.item_subtype = colour | (!!greater * DND_GEM_GREATER);
	item.width = 1;
	item.height = 1;
	item.item_image = GetGemImageId(item.item_subtype);

	if(role != DND_GEMROLE_NONE)
		GiveImplicitToField(item_pos, role, 0);
}

// level is the monster's for a kill, the player's for a chest; extra is 1 for a boss. Matches SpawnItemForAll's table.
void SpawnGem(int pnum, int rarity_boost, int level, bool noRandomVelXY = false, int extra = -1) {
	if(level < DND_GEM_MINLEVEL)
		return;

	int c = CreateItemSpot();
	if(c == -1)
		return;

	RollGemInfo(c, -1, -1, -1, extra == 1, IsBossTierDungeon());
	SpawnDrop(GetGemDropClass(GetFieldItem(c).item_subtype), 16.0, 16, pnum + 1, c, noRandomVelXY);
	SyncItemData(pnum, c, DND_SYNC_ITEMSOURCE_FIELD, -1, -1);
	SendOwnerScript("DnD Play Local Item Drop Sound", pnum, pnum, DND_ITEM_GEM);
}

// A chest's gem: one each, gated on each player's own level.
void SpawnGemForAll() {
	for(int i = 0; i < MAXPLAYERS; ++i)
		if(PlayerInGame(i) && !PlayerIsSpectator(i) && !PlayerActivities[i].loot_penalty)
			SpawnGem(i, 0, GetActorLevel(i + P_TIDSTART));
}

int GetGemChestChance(int ctype) {
	if(ctype == DND_CHESTTYPE_GOLD)
		return DND_GEM_CHESTCHANCE_GOLD;
	if(ctype == DND_CHESTTYPE_SILVER)
		return DND_GEM_CHESTCHANCE_SILVER;
	return DND_GEM_CHESTCHANCE_BRONZE;
}

// A rolled gem straight into the activator's inventory (Transmutia's output); a scratch field slot carries the roll.
// Returns the box it landed in, -1 when there was no slot to roll on.
int RollGemIntoInventory(int pnum, int greater) {
	int c = CreateItemSpot();
	if(c == -1)
		return -1;

	RollGemInfo(c, -1, greater, -1);
	auto item = GetFieldItem(c);
	SendOwnerScript("DnD Gem Message", pnum, item.item_subtype, item.implicit[0].attrib_id);
	return HandleInventoryPickup(c);
}

// ---- pickup -----------------------------------------------------------------------------------
Script "DnD Gem Message" (int subt, int role) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
	Log(s:StrParam(s:"\cc", l:"DND_PICKUP_GEM", s:": \c[Y5]", s:GetGemFullName(subt, role), s:"!\c-"));
}

Script "DnD Gem Item Pickup" (int sp) {
	if((sp & 0xFFFF) == 255)
		SetActivatorToTarget(0);
	else
		SetActivator((sp & 0xFFFF) + P_TIDSTART);

	auto item = GetFieldItem(sp >> 16);
	SendOwnerScript("DnD Gem Message", PlayerNumber(), item.item_subtype, item.implicit[0].attrib_id);
	GiveInventory("GemSoundPlayer", 1);

	HandleInventoryPickup(sp >> 16);
}

#endif
