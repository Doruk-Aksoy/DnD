#ifndef DND_PROMPT_IN
#define DND_PROMPT_IN

#include "../DnD_Common.h"
#include "DnD_Incursion.h"
#include "DnD_MerchantGUI.h"
// BEFORE the include: DnD_Ultimatum.h picks a line from these ranges, and a macro does not
// forward reference in BCS. The enum members they name are declared below and resolve at the
// expansion site, which is all a textual define needs.
//
// Extending an ASK run renumbers every lump under it. The INTRO run is last in the enum, so
// growing that one shifts nothing.
#define DW_ULTIMATUM_ASK_BEGIN DW_ULTIMATUM_ASK1
#define DW_ULTIMATUM_ASK_END DW_ULTIMATUM_ASK4
#define DW_ULTIMATUM_INTRO_BEGIN DW_ULTIMATUM_INTRO1
#define DW_ULTIMATUM_INTRO_END DW_ULTIMATUM_INTRO3

#include "DnD_Ultimatum.h"

#define DND_ARTIFACT_BASETIME 60
#define DND_ARTIFACT_TIME_PER 30

#define DND_SUPERDEMON_HPBUFF 4 // 4 times hp
#define DND_SUPERDEMON_NONBOSSBUFF 2
#define DND_SUPERDEMON_DIFFBONUS 20 // 20%

#define DND_ASMODEUS_RESISTBUFF 33 // 33%

enum {
	DW_GREET_FIRST_TIME1,
	DW_GREET_FIRST_TIME2,
	DW_GREET_FIRST_TIME3,
	DW_GREET1,
	DW_GREET2,
	DW_GREET3,
	DW_CHALLENGE1,
	DW_CHALLENGE2,
	DW_CHALLENGE3,

	// The between-wave offer lines.
	DW_ULTIMATUM_ASK1,
	DW_ULTIMATUM_ASK2,
	DW_ULTIMATUM_ASK3,
	DW_ULTIMATUM_ASK4,

	// match order of ultimatum challenges
	DW_ULTIMATUM_EXTRAELEDMG,
	DW_ULTIMATUM_SAWBLADES,
    DW_ULTIMATUM_MIASMA,
    DW_ULTIMATUM_CRACKLINGPAIN,
    DW_ULTIMATUM_STORMCALL,
    DW_ULTIMATUM_FIRESKULLS,

	MRCH_GREET,

	// The OPENING ultimatum offer, armed before any wave has run -- the ordinary set greets a
	// wave just survived, which does not fit the first time he appears.
	//
	// Appended deliberately. GetPromptText builds DND_PROMPT<id + 1>, so inserting these any
	// higher up would silently renumber every lump below them.
	DW_ULTIMATUM_INTRO1,
	DW_ULTIMATUM_INTRO2,
	DW_ULTIMATUM_INTRO3,

	// Printed UNDER whichever of the three above was picked, not instead of one -- the three
	// are greetings and this is the explanation they all share.
	DW_ULTIMATUM_INTRO_COMMON,

	MAX_PROMPTED_LINES
};
#define DW_CHALLENGE_BEGIN DW_CHALLENGE1
#define DW_ULTIMATUM_CHALLENGE_BEGIN DW_ULTIMATUM_EXTRAELEDMG

enum {
	NPC_OFFER_NA,
	NPC_OFFER_SLAYCHAOSMARK,
	NPC_OFFER_COLLECTARTIFACT,
	NPC_OFFER_SUPERDEMON,
	NPC_OFFER_DUNGEON
};

#define WANDERER_MARK_MIN 5
#define WANDERER_MARK_MAX 50

enum {
	DND_NPC_DARKWANDERER,
	DND_NPC_MERCHANT,
};
#define MAX_DND_NPCS (DND_NPC_DARKWANDERER + 1)
#define NPC_APPEAR_CHANCE 0.15 // 15%

enum {
	NPC_STATE_IDLE,
	NPC_STATE_VOTE_ONGOING,
	NPC_STATE_VOTE_DECLINE,
	NPC_STATE_VOTE_ACCEPT
};

typedef struct npc_info {
	bool voting;
	int vote_count;
	int offer;
	int dialog;
	int offer_progress;
	int time;
	int n_state;
	int aux_data;
	int interaction_count;
	int voters[MAXPLAYERS];		// index is player_id, value is what vote they cast, 1 for accept -1 for decline

	// ultimatum: both option buttons are an ACCEPT, so this settles WHICH one won. -1 is none
	int voter_choice[MAXPLAYERS];
	int chosen_option;
} npc_info_T;

npc_info_T NPC_States[MAX_DND_NPCS] = { { false, 0, 0, 0, 0, 0, 0, 0 } };

#define DND_VOTE_TIME 30 // seconds

// button row for the ultimatum offer -- lower than the plain one, the body is six lines longer
// The challenge list grows with the offer, so the vote row is placed under whatever the text came
// out as rather than at a fixed height. TEXTTOP is where the body is drawn, LINEH is SMALLFONT in
// this hud, and WRAPCHARS is the clip rect's wrap expressed in characters -- the same approximation
// of a variable width font that the button widths use.
#define DND_PROMPT_TEXTTOP 128.0
#define DND_PROMPT_LINEH 9.0
#define DND_PROMPT_WRAPW 512
#define DND_ULTIMATUM_PROMPT_BTNPAD 4.0

// a floor, and a ceiling that keeps the row off the reward cell under it -- that cell is drawn in
// the 480x320 hud at DND_ULTREWARD_OFFERY, so its top edge is 456 in this one
#define DND_ULTIMATUM_PROMPT_YMIN 256.0
#define DND_ULTIMATUM_PROMPT_YMAX 440.0

void MarkNPCMet(int npc) {
	SetInventory("DnD_NPC_Meet", CheckInventory("DnD_NPC_Meet") | (1 << (npc + 1)));
}

bool HasMetNPC(int npc) {
	return CheckInventory("DnD_NPC_Meet") & (1 << (npc + 1));
}

bool CanDarkWandererOfferSuperMonster() {
	return 	MapData[DND_MAPDATA_BARONCOUNT] || MapData[DND_MAPDATA_FATSOCOUNT] || MapData[DND_MAPDATA_ARACHNOCOUNT] ||
			MapData[DND_MAPDATA_ARCHVILECOUNT] || MapData[DND_MAPDATA_SPIDERMASTERMINDCOUNT] || MapData[DND_MAPDATA_CYBERDEMONCOUNT];
}

// The artifact offer needs somewhere to PUT the artifacts: each one is anchored beside an existing
// pickup or shared item. Demand the worst case the quest can roll -- 2 + 2 * difficulty -- so it can
// never promise more artifacts than the map has anchors for.
//
// This reads the tid counters, which are only populated once the per-item setup scripts have run,
// and those wait on SETUP_CLEANINGMONSTERTIDS. NPC_Setup's caller therefore has to wait for that
// stage before choosing an offer, or this always sees zero and the offer is never picked at all.
bool CanDarkWandererOfferArtifacts() {
	return InformationInLevel[LEVELINFO_TID_PICKUPS] + InformationInLevel[LEVELINFO_TID_SHAREDITEMS] >= 2 + 2 * MapData[DND_MAPDATA_DIFFICULTY];
}

// Is this offer viable on this map at all? Anything not listed is always placeable.
bool CanDarkWandererOffer(int offer) {
	switch(offer) {
		case NPC_OFFER_SUPERDEMON:
		return CanDarkWandererOfferSuperMonster();
		case NPC_OFFER_COLLECTARTIFACT:
		return CanDarkWandererOfferArtifacts();
	}
	return true;
}

void NPC_Setup() {
	// dont show npc in lobby maps or unpopulated maps
	if(IsLobbyMap(StrParam(n:PRINTNAME_LEVEL)) || !MapData[DND_MAPDATA_MONSTERTOTAL] || InformationInLevel[LEVELINFO_ISDUNGEON])
		return;

	int pcount = InformationInLevel[LEVELINFO_PLAYERCOUNTATSTART];
	bool NPCOK = random(0, 1.0) <= NPC_APPEAR_CHANCE;

	// check averages -- later on if there are more random npcs added, consider checking which npc to place instead of assuming its dark wanderer
#ifdef ISDEBUGBUILD
	if(1) {
#else
	if(InformationInLevel[LEVELINFO_PLAYERLEVEL] / pcount >= GetCVar("dnd_npc_appear_level") && NPCOK && NPC_States[DND_NPC_DARKWANDERER].offer == NPC_OFFER_NA) {
#endif
		// check if offers can be OK for this particular map
		NPC_States[DND_NPC_DARKWANDERER].aux_data = 0;

		// Reroll until the offer is one this map can actually host. Bounded, so a map that somehow
		// supports none of them leaves with whatever came up last rather than hanging the script --
		// SLAYCHAOSMARK is always viable here, since NPC_Setup already returned on a monsterless map.
		int offer_tries = 0;
		do {
			NPC_States[DND_NPC_DARKWANDERER].offer = random(NPC_OFFER_SLAYCHAOSMARK, NPC_OFFER_SUPERDEMON);
			++offer_tries;
		} while(!CanDarkWandererOffer(NPC_States[DND_NPC_DARKWANDERER].offer) && offer_tries < 32);

#ifdef ISDEBUGBUILD
		NPC_States[DND_NPC_DARKWANDERER].offer = NPC_OFFER_COLLECTARTIFACT;
#endif

		NPC_States[DND_NPC_DARKWANDERER].dialog = random(DW_GREET1, DW_GREET3);
		// find a player and spawn this near them at start
		int tid = 0;
		int i;
		for(i = 0; i < MAXPLAYERS; ++i) {
			tid = i + P_TIDSTART;

			// try a radius around this player first -- 4 degrees inc with 180 degree offset
			if(PlayerInGame(i) && isActorAlive(tid)) {
				// this part assumes only a single npc will be available per map
				ACS_NamedExecuteWithResult("DnD Try Spawn Area", tid, "DarkWanderer", DND_NPC_TID, 96 | (4 << 16) | (22 << 24));
				break;
			}
		}
		
		for(i = 0; i < MAXPLAYERS; ++i)
			NPC_States[DND_NPC_DARKWANDERER].voters[i] = 0;
		ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1, -1, DND_NPC_DARKWANDERER);
	}
}

str GetPromptText(int id) {
	return StrParam(s:"DND_PROMPT", d:id + 1);
}

str GetUltimatumPromptText(int id) {
	return StrParam(s:"DND_ULTIMATUM", d:id + 1);
}

// short button label. the lump above is the long description
str GetUltimatumOptionName(int id) {
	return StrParam(s:"DND_ULTIMATUM_NAME", d:id + 1);
}

// what THIS tier adds, header and all -- keep in step with the apply hook. returns the whole
// segment including its separator so a single tier option can drop out without leaving one
str GetUltimatumTierText(int id, int tier) {
	// one tier means the description above already says the whole thing
	if(GetUltimatumOptionMaxTier(id) <= 1)
		return "";

	str lump = StrParam(s:"DND_ULTIMATUM", d:id + 1, s:"_T", d:tier);
	return StrParam(s:"\n     \ck", l:"DND_TIER", s:" ", d:tier, s:"\n", l:lump, s:"\c-");
}

// which of the offered options the group picked. Ties go to the EARLIER slot, so a split never
// stalls -- strictly greater is what keeps slot 1 winning a draw.
int ResolveUltimatumVoteChoice(int npc_id) {
	int tally[ULTIMATUM_OFFER_SLOTS];
	int i;

	for(i = 0; i < ULTIMATUM_OFFER_SLOTS; ++i)
		tally[i] = 0;

	for(i = 0; i < MAXPLAYERS; ++i) {
		if(NPC_States[npc_id].voters[i] != 1)
			continue;

		for(int s = 0; s < ULTIMATUM_OFFER_SLOTS; ++s) {
			int opt = GetUltimatumOfferOption(NPC_States[npc_id].offer, s);
			if(opt != -1 && NPC_States[npc_id].voter_choice[i] == opt) {
				++tally[s];
				break;
			}
		}
	}

	int best = 0;
	for(i = 1; i < ULTIMATUM_OFFER_SLOTS; ++i) {
		if(GetUltimatumOfferOption(NPC_States[npc_id].offer, i) != -1 && tally[i] > tally[best])
			best = i;
	}

	return GetUltimatumOfferOption(NPC_States[npc_id].offer, best);
}

// TRADBTN is a 77x16 graphic with no tileable middle, so a long label is fitted by STRETCHING it.
// FITCHARS is what one plate holds; capped so neighbouring buttons cannot overlap. Both are dials.
#define TRADBTN_W 77
#define TRADBTN_FITCHARS 8

// At 152 the longest option names ran well past their plate -- "Overwhelming Monsters" wants 202
// and was drawn on a 152 one. 202 is that longest label, and capping there is what bounds the row:
// three of them plus decline plus the gaps is 731, inside the 760 the background is wide.
#define TRADBTN_MAXW 202

// Gap between neighbouring offer plates, and the point the row is centred on.
#define DND_ULTBTN_GAP 16
#define DND_ULTBTN_CENTRE 480

// the offered options, plus decline on the end
#define DND_ULTBTN_COUNT (ULTIMATUM_OFFER_SLOTS + 1)

// A button needs two hud ids, and they have to come from DISJOINT blocks. Interleaved as -3-2k
// and -1-2k they overlap at a stride of 2: button k's plate landed on button k+1's label and wiped
// the text off the whole row. Plates first to match the accept/decline pair. 10 upward is clear of
// the close hint at -5 and the bank button at -8/-9, and stops short of the voters at -20.
#define DND_ULTBTN_PLATEID(k) (RPGMENUITEMID - 10 - (k))
#define DND_ULTBTN_LABELID(k) (RPGMENUITEMID - 10 - DND_ULTBTN_COUNT - (k))

// A box id is its POSITION in the pane, so the two that follow the button row move whenever the
// row grows. Named off the count rather than written out, which is what went wrong last time.
#define DND_ULTBOX_CELL (MBOX_1 + DND_ULTBTN_COUNT)
#define DND_ULTBOX_BANKBTN (DND_ULTBOX_CELL + 1)

// Banked reward cell k. Derived for the same reason the two above are: the cells follow the bank
// button, which follows the button row, so a written-out MBOX_ here goes stale the moment the row
// changes size. It did -- the row is DND_ULTBTN_COUNT (4) wide and the hover test still said
// MBOX_5, which drew the highlight frame one cell along from the item the tooltip was describing
// and let the bank button itself light up cell 0.
#define DND_ULTBOX_REWARDCELL(k) (DND_ULTBOX_BANKBTN + 1 + (k))

int GetPromptButtonWidth(str lump) {
	// StrParam resolves the lump, so this measures what is DRAWN, not the key
	int chars = StrLen(StrParam(l:lump));
	if(chars <= TRADBTN_FITCHARS)
		return TRADBTN_W;

	return Min(TRADBTN_W * chars / TRADBTN_FITCHARS, TRADBTN_MAXW);
}

// Plate width for one button of THIS offer -- the options in slot order, then decline last.
// An unused slot measures 0 so it takes no room in the row.
int GetUltimatumOfferButtonWidth(int offer_id, int slot) {
	if(slot >= ULTIMATUM_OFFER_SLOTS)
		return GetPromptButtonWidth("DND_DECLINE");

	int o = GetUltimatumOfferOption(offer_id, slot);
	return o == -1 ? 0 : GetPromptButtonWidth(GetUltimatumOptionName(o));
}

// Centre x for one of the offer buttons.
//
// Each button takes its OWN width with an even gap between, and the packed row is centred. A
// uniform pitch sized to the widest plate was 880 across at four buttons, which hung off both
// ends of the 760 wide background.
//
// The plate, the hitbox and the voter column under each button all call this, which is what stops
// them drifting apart. The .4 is the alignment fraction -- see DrawPromptButtonPlate.
int GetUltimatumOfferButtonX(int offer_id, int slot) {
	int i, t, total = 0, x = 0, mine = 0;

	for(i = 0; i < DND_ULTBTN_COUNT; ++i) {
		t = GetUltimatumOfferButtonWidth(offer_id, i);
		if(!t)
			continue;

		if(i < slot)
			x += t + DND_ULTBTN_GAP;
		else if(i == slot)
			mine = t;

		total += t + DND_ULTBTN_GAP;
	}

	// one gap too many was added above, then step from the row's left edge to this plate's centre
	if(total)
		total -= DND_ULTBTN_GAP;

	return ((x + DND_ULTBTN_CENTRE - total / 2 + mine / 2) << 16) + 0.4;
}

// SMALLFONT glyph widths, straight out of the STCFN lumps in Font.wad, indexed by character minus
// 32. The font carries its own lowercase, so nothing is folded. verify_promptrow.py regenerates
// this from the wad and fails if the two drift apart.
#define DND_SMALLFONT_FIRSTCHAR 32
#define DND_SMALLFONT_LASTCHAR 122

int GetSmallFontCharWidth(int c) {
	static int w[DND_SMALLFONT_LASTCHAR - DND_SMALLFONT_FIRSTCHAR + 1] = {
		4, 5, 7, 7, 7, 10, 8, 4, 6, 6, 7, 7,// sp..+
		4, 6, 4, 8, 10, 10, 10, 10, 10, 10, 10, 10,// ,..7
		10, 10, 4, 4, 5, 5, 5, 9, 9, 11, 10, 9,// 8..C
		11, 9, 9, 11, 11, 5, 9, 10, 9, 12, 11, 11,// D..O
		9, 11, 9, 9, 9, 11, 11, 12, 9, 11, 9, 5,// P..[
		8, 5, 7, 8, 4, 9, 9, 9, 9, 9, 9, 9,// \..g
		9, 4, 9, 9, 9, 10, 9, 9, 9, 9, 9, 8,// h..s
		8, 9, 8, 10, 9, 9, 8 // t..z
	};

	if(c < DND_SMALLFONT_FIRSTCHAR || c > DND_SMALLFONT_LASTCHAR)
		return w[0];

	return w[c - DND_SMALLFONT_FIRSTCHAR];
}

// How many lines the body renders as. Greedy word wrap on the real glyph widths -- counting
// characters instead was four lines short on a full offer, which is what put the vote row back
// on top of the text. Colour escapes carry no width, so they are stepped over: 28 is
// TEXTCOLOR_ESCAPE, written as \cX or \c[Name].
int CountPromptBodyLines(str body) {
	int len = StrLen(body), n = 1, w = 0, wordw = 0, i, c, cw;

	for(i = 0; i < len; ++i) {
		c = GetChar(body, i);

		if(c == 10) {
			++n;
			w = 0;
			wordw = 0;
			continue;
		}

		if(c == 28) {
			if(GetChar(body, ++i) == '[')
				while(i < len && GetChar(body, i) != ']')
					++i;
			continue;
		}

		cw = GetSmallFontCharWidth(c);
		w += cw;

		// a space is where the next break can happen, so the word restarts there
		if(c == 32) {
			wordw = 0;
			continue;
		}

		wordw += cw;

		if(w > DND_PROMPT_WRAPW) {
			++n;

			// the word in hand moves down with the break, unless it IS the whole line
			if(wordw < w)
				w = wordw;
			else {
				w = cw;
				wordw = cw;
			}
		}
	}

	return n;
}

// the vote row sits under the text, clamped so a long offer cannot push it onto the reward cell
int GetUltimatumPromptButtonY(str body) {
	return Clamp_Between(DND_PROMPT_TEXTTOP + CountPromptBodyLines(body) * DND_PROMPT_LINEH +
		DND_ULTIMATUM_PROMPT_BTNPAD, DND_ULTIMATUM_PROMPT_YMIN, DND_ULTIMATUM_PROMPT_YMAX);
}

// cursor space is the draw space mirrored and halved: c = max - draw / 2. holds for all three
// vanilla boxes, so a button's hitbox is derived from where it is drawn instead of hand placed.
void AddPromptButtonBox(menu_pane_T module& p, int dx, int dy, int w) {
	int cx = 480.0 - dx / 2, cy = 320.0 - dy / 2, hw = (w << 16) / 4;
	AddBoxToPane_Points(p, cx + hw, cy + 4.0, cx - hw, cy - 4.0);
}

// the plate is stretched by drawing it in a NARROWER virtual hud, so its fixed 77 units cover more
// of the screen. x moves into that space with it; height and everything after are untouched.
void DrawPromptButtonPlate(int plate_id, int x, int y, int w) {
	if(w <= TRADBTN_W) {
		HudMessage(s:"A"; HUDMSG_PLAIN, plate_id, CR_WHITE, x, y, 0.0, 0.0);
		return;
	}

	// the FRACTION of a hud coordinate picks the alignment, so only the integer part scales --
	// multiplying the whole thing turns .4 into .098 and the plate aligns differently to its label
	int nx = ((FixedMul(x & 0xFFFF0000, FixedDiv(TRADBTN_W, w)) + 0.5) & 0xFFFF0000) | (x & 0xFFFF);

	SetHudSize(TRADBTN_W * HUDMAX_X_PROMPT / w, HUDMAX_Y_PROMPT, 1);
	HudMessage(s:"A"; HUDMSG_PLAIN, plate_id, CR_WHITE, nx, y, 0.0, 0.0);
	SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
}

// one prompt button, same shape the accept/decline pair uses. state: 0 idle, 1 hover, 2 picked
void DrawPromptButton(str lump, int state, int label_id, int plate_id, int x, int y) {
	SetFont("SMALLFONT");
	if(state == 2) {
		HudMessage(l:lump; HUDMSG_PLAIN, label_id, CR_GREEN, x, y, 0.0, 0.0);
		SetFont("TRADBTNC");
	}
	else if(state == 1) {
		HudMessage(s:"\c[M3]", l:lump; HUDMSG_PLAIN, label_id, CR_WHITE, x, y, 0.0, 0.0);
		SetFont("TRADBTNH");
	}
	else {
		HudMessage(s:"\c[Y5]", l:lump; HUDMSG_PLAIN, label_id, CR_WHITE, x, y, 0.0, 0.0);
		SetFont("TRADBTN");
	}
	DrawPromptButtonPlate(plate_id, x, y, GetPromptButtonWidth(lump));
}

// ---- reward pool view --------------------------------------------------------------------------
// Cells are drawn in the 480x320 hud with x their LEFT edge and y their CENTRE -- the geometry the
// merchant grid uses, so a hitbox is that space mirrored.
#define DND_ULTREWARD_CELL 40.0
#define DND_ULTREWARD_PERROW 5
#define DND_ULTREWARD_STEPX 44.0
#define DND_ULTREWARD_STEPY 48.0
#define DND_ULTREWARD_GRIDX 120.1
#define DND_ULTREWARD_GRIDY 90.0

// this round's reward and the button that opens the bank, on their own row under the vote buttons
#define DND_ULTREWARD_OFFERX 198.1
#define DND_ULTREWARD_OFFERY 248.0
#define DND_ULTREWARD_BTNX 526.4
#define DND_ULTREWARD_BTNY 496.0

// clear of the voter list, which runs down to RPGMENUITEMID - 20 - MAXPLAYERS
#define DND_ULTREWARD_HUDID (RPGMENUITEMID - 24 - MAXPLAYERS)

void AddUltimatumRewardCellBox(menu_pane_T module& p, int x, int y) {
	int l = 480.0 - GetIntegerBits(x);
	AddBoxToPane_Points(p, l, 320.0 - y + DND_ULTREWARD_CELL / 2, l - DND_ULTREWARD_CELL, 320.0 - y - DND_ULTREWARD_CELL / 2);
}

// frame behind, icon in front -- lower id draws on top
void DrawUltimatumRewardCell(int slot, int x, int y, bool hovered, int id) {
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);

	if(GetUltimatumRewardItemType(slot) != DND_ITEM_NULL) {
		auto item = GetUltimatumRewardItem(slot);
		SetFont(GetItemImage(item.item_image));
		HudMessage(s:"A"; HUDMSG_PLAIN, id - 1, CR_WHITE, GetIntegerBits(x) + DND_ULTREWARD_CELL / 2 + 0.4, y, 0.0, 0.0);
	}
	else
		DeleteText(id - 1);

	SetFont(GetCharmBoxLabel(DND_CHARM_SMALL, hovered));
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_WHITE, x, y, 0.0, 0.0);
	SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
}

// The label on box 5. The bank is a modal LOOK -- the way out is the only thing in it you can click.
str GetUltimatumBankButtonLump(bool showbank) {
	return showbank ? "DND_ULTIMATUM_REWARDBACK" : "DND_ULTIMATUM_REWARDS";
}

// Box ids are POSITIONAL, so every slot is registered whether or not it is used: 1-2 the challenges,
// 3 decline, 4 this round's reward, 5 the bank toggle, 6+ one per banked reward while it is open.
void BuildUltimatumOfferPane(menu_pane_T module& p, int offer_id, int yOff, bool showbank) {
	ResetPane(p);

	// The challenge is not reachable from inside the bank -- its buttons come off the screen, so
	// their boxes go with them. The three SLOTS stay: a box id is its position in the pane.
	int bi;
	if(showbank) {
		for(bi = 0; bi < DND_ULTBTN_COUNT; ++bi)
			AddBoxToPane_Points(p, -1, -1, -1, -1);
	}
	else {
		// An empty slot still takes its box: an id is a POSITION in the pane, so skipping one
		// slides decline up onto it.
		for(bi = 0; bi < ULTIMATUM_OFFER_SLOTS; ++bi) {
			int bo = GetUltimatumOfferOption(offer_id, bi);
			if(bo != -1)
				AddPromptButtonBox(p, GetUltimatumOfferButtonX(offer_id, bi), yOff,
					GetUltimatumOfferButtonWidth(offer_id, bi));
			else
				AddBoxToPane_Points(p, -1, -1, -1, -1);
		}

		AddPromptButtonBox(p, GetUltimatumOfferButtonX(offer_id, ULTIMATUM_OFFER_SLOTS), yOff,
			GetUltimatumOfferButtonWidth(offer_id, ULTIMATUM_OFFER_SLOTS));
	}

	AddUltimatumRewardCellBox(p, DND_ULTREWARD_OFFERX, DND_ULTREWARD_OFFERY);
	AddPromptButtonBox(p, DND_ULTREWARD_BTNX, DND_ULTREWARD_BTNY, GetPromptButtonWidth(GetUltimatumBankButtonLump(showbank)));

	if(showbank) {
		int n = GetUltimatumRewardCount();
		for(int i = 0; i < n; ++i) {
			AddUltimatumRewardCellBox(p,
				DND_ULTREWARD_GRIDX + (i % DND_ULTREWARD_PERROW) * DND_ULTREWARD_STEPX,
				DND_ULTREWARD_GRIDY + (i / DND_ULTREWARD_PERROW) * DND_ULTREWARD_STEPY);
		}
	}
}

// the offer text and the bank share the body area, so showing one clears the other
void DrawUltimatumBody(str body, bool showbank) {
	SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
	SetFont("SMALLFONT");
	SetHudClipRect(160, 128, 532, 600, 532);

	if(!showbank)
		HudMessage(s:body; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0);
	else if(GetUltimatumRewardCount())
		HudMessage(s:"\c[W3]", l:"DND_ULTIMATUM_REWARDBANK"; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0);
	else
		HudMessage(s:"\c[W3]", l:"DND_ULTIMATUM_REWARDBANK", s:"\c-\n\n", l:"DND_ULTIMATUM_REWARDNONE"; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0);

	SetHudClipRect(0, 0, 0, 0);
}

void ClosePrompt(bool non_npc_use = false) {
	if(!non_npc_use) {
		TakeInventory("NPC_Offer_Accepted", 1);
		TakeInventory("NPC_Offer_Declined", 1);
	}
	TakeInventory("ShowingPrompt", 1);
	TakeInventory("ShowingMerchantView", 1);
	GiveInventory("NPC_Trigger_Cooldown", 1);
	LocalAmbientSound("RPG/MenuClose", 127);
	SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
	TakeInventory("P_Frozen", 1);

	ACS_NamedExecuteAlways("DND Menu Cleanup", 0, PlayerNumber());
}

void ConcludeVoting(int npc_id, int result) {
	NPC_States[npc_id].n_state = result;
	NPC_States[npc_id].voting = false;
	TakeInventory("NPC_Offer_Accepted", 1);
	TakeInventory("NPC_Offer_Declined", 1);
	
	// reset vote list if voting has concluded
	ACS_NamedExecuteWithResult("DnD NPC Vote Sync", -1, -1, npc_id);

	// close prompts of players that had voted
	NPC_States[npc_id].vote_count = 0;
	int curr_activator = ActivatorTID();
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(NPC_States[npc_id].voters[i]) {
			SetActivator(i + P_TIDSTART);
			ACS_NamedExecuteAlways("DnD Close Prompt Delayed", 0, i, npc_id);
		}
	}
	
	// set npc state to go back state
	if(result == NPC_STATE_VOTE_DECLINE) {
		SetActorState(DND_NPC_TID, "GoBack", false);

		// declining an ultimatum ENDS it. here, not in the vote, so the timeout path lands too
		if(InformationInLevel[LEVELINFO_ISULTIMATUM])
			ACS_NamedExecuteAlways("DnD Handle Ultimatum Finish", 0);
	}
		
	// restore activator
	SetActivator(curr_activator);
}

void ApplyMarkOfChaos(int m_id) {
	int tid = m_id + DND_MONSTERTID_BEGIN;
	
	ACS_NamedExecuteWithResult("DnD Monster Trait Give Single", tid, DND_MARKOFCHAOS);

	// clear all weaknesses
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ENERGY_WEAKNESS, DND_SILVER_WEAKNESS, DND_FIRE_WEAKNESS);
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ICE_WEAKNESS, DND_MAGIC_WEAKNESS, DND_ELEMENTAL_WEAKNESS);
	
	// provide the benefits of this trait - doubled health, 50% more damage, 50% faster, extra aggressive, chaos imbued attacks
	int base = MonsterProperties[m_id].maxhp;
	int add = base * 2;
	base = Clamp_Between(base + add, 1, bcs::INT_MAX);
	MonsterProperties[m_id].basehp = base;
	MonsterProperties[m_id].maxhp = base;
	
	SetActorProperty(tid, APROP_HEALTH, base);
	SetActorProperty(tid, APROP_SPEED, 3 * GetActorProperty(tid, APROP_SPEED) / 2);
	GiveActorInventory(tid, "MakeAggressive", 1);
	
	GiveActorInventory(tid, "ChaosMark_Script_Run", 1);
}

void ApplyMarkOfAsmodeus(int m_id, int m_class) {
	int tid = m_id + DND_MONSTERTID_BEGIN;

	// this monster is the one, buff it
	ACS_NamedExecuteWithResult("DnD Monster Trait Give Single", tid, DND_MARKOFASMODEUS);

	// clear all weaknesses
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ENERGY_WEAKNESS, DND_SILVER_WEAKNESS, DND_FIRE_WEAKNESS);
	ACS_NamedExecuteWithResult("DnD Monster Trait Take - TID", tid, DND_ICE_WEAKNESS, DND_MAGIC_WEAKNESS, DND_ELEMENTAL_WEAKNESS);

	int base = MonsterProperties[m_id].maxhp;
	int add = base * DND_SUPERDEMON_HPBUFF;
	if(m_class != MONSTERCLASS_CYBERDEMON && m_class != MONSTERCLASS_SPIDERMASTERMIND)
		add *= DND_SUPERDEMON_NONBOSSBUFF;

	base = Clamp_Between(base + add, 1, bcs::INT_MAX);
	MonsterProperties[m_id].basehp = base;
	MonsterProperties[m_id].maxhp = base;
	SetActorProperty(tid, APROP_HEALTH, base);

	GiveActorInventory(tid, "AsmodeusMark_Script_Run", 1);

	// add to existing resist now that we changed the model (cap applies on the one area checking for it in damage deal)
	for(int i = 0; i < MAX_DAMAGE_CATEGORIES; ++i)
		MonsterProperties[m_id].resists[i] += DND_ASMODEUS_RESISTBUFF;
}

Script "DnD Chaos Mark Script" (void) {
	ACS_NamedExecuteWithResult("DnD Chaos Mark FX", ActivatorTID());
}

Script "DnD Asmodeus Mark Script" (void) {
	ACS_NamedExecuteAlways("DND Spawn Attachment", 0, ActivatorTID(), DND_SPECIALFX_ASMODEUSCIRCLE);
}

Script "DnD Chaos Mark FX" (int tid) CLIENTSIDE {
	int h = GetActorProperty(tid, APROP_HEIGHT);
	SpawnForced("ChaosMarkFX", GetActorX(tid), GetActorY(tid), GetActorZ(tid) + 16.0, DND_CHAOSMARKFX_TID);
	SetActorProperty(DND_CHAOSMARKFX_TID, APROP_MASTERTID, tid);
	// 127 and 121 are sprite dimensions
	SetActorProperty(DND_CHAOSMARKFX_TID, APROP_SCALEX, (GetActorProperty(tid, APROP_RADIUS) + 16.0) / 51);
	SetActorProperty(DND_CHAOSMARKFX_TID, APROP_SCALEY, (h + 16.0) / 52);
	
	SetActivator(DND_CHAOSMARKFX_TID);
	Thing_ChangeTID(0, 0);
	
	// always face viewer
	int tics = 0;
	bool reverse = false;
	while(isActorAlive(tid)) {
		int ang = AngleToFace(tid, ConsolePlayerNumber() + P_TIDSTART);
		SetActorPosition(0, GetActorX(tid) + 8 * cos(ang), GetActorY(tid) + 8 * sin(ang), GetActorZ(tid) + h / 2, 0);
		if(!reverse)
			SetActorProperty(0, APROP_ALPHA, 1.0 - tics * 0.0125);
		else
			SetActorProperty(0, APROP_ALPHA, 0.625 + tics * 0.0125);
		Delay(const:1);
		++tics;
		if(tics == 31) {
			tics = 0;
			reverse = !reverse;
		}
	}
	SetActorState(0, "Vanish", false);
	SetResultValue(0);
}

bool IsChallengeTrackException() {
	switch(NPC_States[DND_NPC_DARKWANDERER].offer) {
		case NPC_OFFER_SUPERDEMON:
		case NPC_OFFER_DUNGEON:
		return true;
	}
	return false;
}

#define DND_ARTIFACT_MAX_ANCHORS 64
#define DND_ARTIFACT_MAX_TRIES 256
#define DND_ARTIFACT_SPREAD_PICKUP 20.0
#define DND_ARTIFACT_SPREAD_SHARED 8.0

// Spawns up to `count` artifacts, each beside a randomly chosen anchor actor drawn from the pickup
// tid range, the shared item tid range, or both. Returns how many actually got placed.
//
// The anchor MUST be checked for still existing, and that is the whole bug this replaced. Those tid
// ranges are dense only at map start: GivePickupTID and GiveSharedItemTID hand out tids from
// counters that only ever go UP, and nothing decrements them when a player collects the item. So by
// the time the wanderer makes this offer, the count still includes every powerup already taken while
// their tids are vacant -- and GetActorX on a vacant tid returns 0. The old code trusted the count,
// read coordinates off tids with no actor behind them, and piled the artifacts at map origin. The
// longer the map had been played, the more of them landed there.
//
// The try budget matters just as much. The old loops were `do { ... } while(temp)` decrementing only
// on a successful spawn, with no bound at all -- a map where anchors keep failing spun the script
// forever rather than giving up.
int PlaceWandererArtifacts(int count, int pickup_count, int shared_count) {
	static int anchors_used[DND_ARTIFACT_MAX_ANCHORS];
	int used_count = 0;
	int placed = 0;
	int tries = 0;
	int k, j, spread;

	while(placed < count && tries < DND_ARTIFACT_MAX_TRIES) {
		++tries;

		// draw from whichever pool has anything in it; coin flip only when both do
		if(!pickup_count || (shared_count && random(0, 1))) {
			if(!shared_count)
				break;
			k = SHARED_ITEM_TID_BEGIN + random(0, shared_count - 1);
			spread = DND_ARTIFACT_SPREAD_SHARED;
		}
		else {
			k = DND_PICKUPTID_BEGIN + random(0, pickup_count - 1);
			spread = DND_ARTIFACT_SPREAD_PICKUP;
		}

		// the anchor has to still be on the map -- see above
		if(!ThingCount(T_NONE, k))
			continue;

		// don't stack two artifacts on the same anchor
		for(j = 0; j < used_count; ++j)
			if(anchors_used[j] == k)
				break;
		if(j != used_count)
			continue;

		if(used_count < DND_ARTIFACT_MAX_ANCHORS)
			anchors_used[used_count++] = k;

		if(Spawn("DarkWanderer_Artifact", GetActorX(k) + random(-spread, spread), GetActorY(k) + random(-spread, spread), GetActorZ(k)))
			++placed;
	}

	return placed;
}

void HandleNPC(int npc_id) {
	int mc = 0;
	int temp = 0;
	int count = 0;
	int i, j;

	// only the superdemon offer uses this now -- artifact placement keeps its own anchor list
	static int slots_occupied[64];
	for(i = 0; i < 64; ++i)
		slots_occupied[i] = 0;

	switch(npc_id) {
		case DND_NPC_DARKWANDERER:
			// dark wanderer's offer must affect the map now
			switch(NPC_States[DND_NPC_DARKWANDERER].offer) {
				case NPC_OFFER_SLAYCHAOSMARK:
					// kill marked monsters -- min of 5, max of 50 -- scaling with map difficulty
					mc = InformationInLevel[LEVELINFO_TID_MONSTER];
					temp = 5 * MapData[DND_MAPDATA_DIFFICULTY] + random(5, 10);

					// we dont care about uniqueness
					for(i = 0; i < temp; ++i) {
						// dont give this mark to elite monsters!
						do {
							j = random(0, mc - 1);
							j = UsedMonsterTIDs[j] - DND_MONSTERTID_BEGIN;
						} while((MonsterProperties[j].flags & DND_MONFLAG_ISELITE) || !isActorAlive(j + DND_MONSTERTID_BEGIN));
						
						// give it the mark of chaos
						if(!HasMonsterTrait(j, DND_MARKOFCHAOS)) {
							ApplyMarkOfChaos(j);
							++count;
						}
					}
					
					// we know the unique monster count that received the mark now
					// save this, we'll use it to track progress
					NPC_States[DND_NPC_DARKWANDERER].offer_progress = count;
				break;
				case NPC_OFFER_COLLECTARTIFACT:
					// retrieve artifacts -- place the artifacts
					mc = InformationInLevel[LEVELINFO_TID_PICKUPS];
					count = InformationInLevel[LEVELINFO_TID_SHAREDITEMS];
					temp = 2 + random(MapData[DND_MAPDATA_DIFFICULTY], 2 * MapData[DND_MAPDATA_DIFFICULTY]);

					// Both pools go in together. The old code split this into three cases -- enough
					// powerups, some, none -- but all three differed only in which pool they drew
					// from, which the picker decides for itself now.
					temp = PlaceWandererArtifacts(temp, mc, count);

					// Progress is what actually got PLACED, never what was asked for. It counts down
					// to zero as artifacts are collected, so promising more of them than exist on the
					// map left the quest impossible to finish.
					NPC_States[DND_NPC_DARKWANDERER].offer_progress = temp;
				break;
				case NPC_OFFER_SUPERDEMON:
					// super powered monster -- find a random monster with matching random id and pick it to be our monster
					for(i = 0; i < 6; ++i)
						slots_occupied[i] = MapData[DND_MAPDATA_BARONCOUNT + i];

					for(mc = 0; mc < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mc) {
						i = UsedMonsterTIDs[mc] - DND_MONSTERTID_BEGIN;
						temp = MonsterProperties[i].class >= MONSTERCLASS_SPIDERMASTERMIND ? 0.25 : 0; // bias to bosses
						if
						(
							MonsterProperties[i].class >= MONSTERCLASS_BARON && MonsterProperties[i].class <= MONSTERCLASS_CYBERDEMON &&
							random(0, 1.0) <= temp + 1.0 / Max(slots_occupied[MonsterProperties[i].class - MONSTERCLASS_BARON]--, 1)
						)
						{
							// if this monster is dead somehow, don't pick
							if(!isActorAlive(UsedMonsterTIDs[mc]))
								continue;

							ApplyMarkOfAsmodeus(i, MonsterProperties[i].class);
							NPC_States[DND_NPC_DARKWANDERER].offer_progress = 1;
							break;
						}
					}
				break;
				/*case NPC_OFFER_INCURSION:
					// cycle through entire list of monsters, roll rng and see if they are "destined" to drop an incursion marker on death
					temp = GetBaseMarkerSpawnChance(InformationInLevel[LEVELINFO_TID_MONSTER]);
					for(mc = 0; mc < InformationInLevel[LEVELINFO_TID_MONSTER]; ++mc) {
						i = UsedMonsterTIDs[mc] - DND_MONSTERTID_BEGIN;
						if(CanMonsterSpawnIncursionMarker(i, temp, mc, InformationInLevel[LEVELINFO_TID_MONSTER])) {
							MonsterProperties[i].spawnsIncursionMarker = true;
							//Log(s:"Confirmed marker on ", d:i, s:" ", s:GetActorClass(UsedMonsterTIDs[mc]), s:" with bchance: ", f:temp);
						}
					}

					NPC_States[DND_NPC_DARKWANDERER].aux_data = random(DND_INCURSION_BEGIN, DND_INCURSION_END);
				break;*/
			}
		break;
	}

	// Last line of defence for the artifact offer. Viability was checked when the offer was chosen,
	// but players collect things between then and the vote, so the anchors can be gone by now. A
	// challenge that placed nothing must NOT start: the tracker reads offer_progress <= 0 as the
	// quest being complete and hands every survivor a reward chest for doing nothing at all.
	if(NPC_States[DND_NPC_DARKWANDERER].offer == NPC_OFFER_COLLECTARTIFACT && NPC_States[DND_NPC_DARKWANDERER].offer_progress <= 0) {
		NPC_States[DND_NPC_DARKWANDERER].offer = NPC_OFFER_NA;
		return;
	}

	NPC_States[DND_NPC_DARKWANDERER].time = GetDarkWandererChallengeTime(NPC_States[DND_NPC_DARKWANDERER].offer);

	if(!IsChallengeTrackException())
		ACS_NamedExecuteAlways("DnD Dark Wanderer Challenge Track", 0);
}

int GetDarkWandererChallengeTime(int offer_id) {
	int x;
	switch(offer_id) {
		case NPC_OFFER_SLAYCHAOSMARK:
			// kill marked monsters
			x = NPC_States[DND_NPC_DARKWANDERER].offer_progress;
			if(x < 25)
				return x * 30;
			else if(x < 35)
				return x * 25 + 120;
			return x * 20 + 140;
		case NPC_OFFER_COLLECTARTIFACT:
			// retrieve artifacts
			return NPC_States[DND_NPC_DARKWANDERER].offer_progress * DND_ARTIFACT_BASETIME + MapData[DND_MAPDATA_DIFFICULTY] * DND_ARTIFACT_TIME_PER;
		case NPC_OFFER_SUPERDEMON:
			// super powered monster
			switch(NPC_States[DND_NPC_DARKWANDERER].offer_progress) {
				case DND_CYBERDEMONID:
					x = 180;
				break;
				case DND_SPIDERMASTERMINDID:
					x = 150;
				break;
				default:
					x = 120;
				break;
			}
			return x * (100 + MapData[DND_MAPDATA_DIFFICULTY] * DND_SUPERDEMON_DIFFBONUS) / 100;
		break;
	}
	return 0;
}

str TimeZeroPadding(int t) {
	if(t < 10)
		return "0";
	return "";
}

Script "DnD Dark Wanderer Challenge Track" (void) {
	Delay(const:TICRATE);
	int color = CR_WHITE;
	// players did not all die check at the end
	while(NPC_States[DND_NPC_DARKWANDERER].time && NPC_States[DND_NPC_DARKWANDERER].offer_progress > 0 && GetGameModeState() != GAMESTATE_INRESULTSEQUENCE && !IsSetupComplete(SETUP_STATE1, SETUP_PLAYERDATAFINISHED)) {
		// global display
		if(NPC_States[DND_NPC_DARKWANDERER].time < 30)
			color = CR_RED;
		else if(NPC_States[DND_NPC_DARKWANDERER].time < 90)
			color = CR_ORANGE;
		else if(NPC_States[DND_NPC_DARKWANDERER].time < 150)
			color = CR_YELLOW;
			
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		SetFont("DBIGFONT");
		HudMessageBold(
			s:TimeZeroPadding(NPC_States[DND_NPC_DARKWANDERER].time / 60), d:NPC_States[DND_NPC_DARKWANDERER].time / 60, 
			s:" : ", 
			s:TimeZeroPadding(NPC_States[DND_NPC_DARKWANDERER].time % 60), d:NPC_States[DND_NPC_DARKWANDERER].time % 60;
			HUDMSG_PLAIN,
			RPGMENUBACKGROUNDID + 1,
			color,
			480.4,
			32.0,
			2.0
		);
		
		Delay(const:TICRATE);
		--NPC_States[DND_NPC_DARKWANDERER].time;
	}
	
	if(NPC_States[DND_NPC_DARKWANDERER].offer_progress <= 0 && NPC_States[DND_NPC_DARKWANDERER].time) {
		// success
		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_WELLDONE"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_GREEN, 240.4, 16.0, 1.0, 1.0);
		AmbientSound("DarkWanderer/ChallengeWin", 127);

		str reward_chest = GetDarkWandererReward();

		// hand out rewards to survivors
		for(int i = 0; i < MAXPLAYERS; ++i) {
			int tid = i + P_TIDSTART;
			if(PlayerInGame(i) && isActorAlive(tid)) {
				SpawnDropAtActor(tid, reward_chest, 0, 0, i + 1, 0);
			}
		}
	}
	else {
		// fail, this means either we have progress left or there is no time, but we know theres no time from the loop above so...
		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_FAILEDIT"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_RED, 240.4, 16.0, 1.0, 1.0);
		AmbientSound("DarkWanderer/ChallengeFail", 127);
	}
	
	Delay(const:TICRATE);

	NPC_States[DND_NPC_DARKWANDERER].offer_progress = 0;
}

// One chest for every offer. What the offer was no longer changes the payout -- the chest rolls its
// own weighted pile (SpawnChallengeChestRewards) instead of the plain lootbox contents plus one
// guaranteed orb. Kept as a function so the single call site above still reads for itself, and so an
// offer that wants its own chest again has somewhere obvious to go.
//
// The three chests this used to return were the only source of Order / Destiny / Reverance outside
// the Ultimatum; those three are now Ultimatum only, deliberately.
str GetDarkWandererReward() {
	return "LootChest_ForPlayer_Challenge";
}

Script "DnD NPC Artifact Pickup" (void) {
	--NPC_States[DND_NPC_DARKWANDERER].offer_progress;
	NPC_States[DND_NPC_DARKWANDERER].time += DND_ARTIFACT_BASETIME + DND_ARTIFACT_TIME_PER * MapData[DND_MAPDATA_DIFFICULTY];
}

Script "DnD Prompt Dark Wanderer" (int first_time, int offer_id, int n_state) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;
		
	// draw the background once
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("THORM1");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, -1, 400.0, 160.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
	SetFont("PROMPT");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUBACKGROUNDID, -1, 480.0, 320.0, 0.0, 0.0);

	// draw greetings text
	SetFont("SMALLFONT");

	HudMessage(
		s:"\c[L7]", l:"CLASS_PRESS", s:" \ci", k:"+use", s: " \c[L7]", l:"CLASS_TOCLOSE", s:"!";
		HUDMSG_PLAIN, RPGMENUITEMID - 5, -1, (HUDMAX_X_PROMPT << 15) + 0.4, ((HUDMAX_Y_PROMPT << 16) - 100.0) + 0.1, 0.0, 0.0
	);

	SetHudClipRect(160, 128, 532, 600, 532);
	
	menu_pane_T module& CurrentPane = GetPane();
	ResetPane(CurrentPane);
	
	// vote state isnt concluded
	bool voting_ongoing = n_state != NPC_STATE_VOTE_DECLINE && n_state != NPC_STATE_VOTE_ACCEPT;
	int yOff = 384.0;

	// ultimatum offers, unpacked once. locals are function scoped in BCS, hence up here
	int i, j, k;
	int ult_opts[ULTIMATUM_OFFER_SLOTS];
	int ult_col[DND_ULTBTN_COUNT];
	int ult_dialog = -1;
	str ult_body = "";
	int declined = 0;
	// the bank view is local to this client -- nothing about looking at it concerns the server
	bool ult_showbank = false;
	int ult_banked = 0, ult_cell = -1;
	
	if(first_time && !InformationInLevel[LEVELINFO_ISULTIMATUM]) {
		if(n_state != NPC_STATE_VOTE_ACCEPT) {
			HudMessage(
				l:GetPromptText(DW_GREET_FIRST_TIME1), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME2), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME3),
				s:"\n\n\c[Y5]---------------------------------------\n\n",
				s:"\c[W3]", l:"DND_CHALLENGE", s:":\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
				HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
			);
		}
		else {
			HudMessage(
				l:GetPromptText(DW_GREET_FIRST_TIME1), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME2), 
				s:"\n\n", l:GetPromptText(DW_GREET_FIRST_TIME3),
				s:"\n\n\c[Y5]---------------------------------------\n\n",
				s:"\c[W3]", l:"DND_CHALLENGE", s:" - \cd", l:"DND_ACCEPTED", s:"\c[W3]:\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
				HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
			);

		}
		if(voting_ongoing) {
			AddBoxToPane_Points(CurrentPane, 340.0, 132.0, 300.0, 124.0);
			AddBoxToPane_Points(CurrentPane, 260.0, 132.0, 220.0, 124.0);
		}
	}
	else if(n_state != NPC_STATE_VOTE_ACCEPT) {
		if(!InformationInLevel[LEVELINFO_ISULTIMATUM]) {
			HudMessage(
				l:GetPromptText(CheckInventory("ReceivedDialogID")),
				s:"\n\n\c[Y5]---------------------------------------\n\n",
				s:"\c[W3]", l:"DND_CHALLENGE", s:":\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
				HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
			);
		}
		else {
			// both offers, described. built as a string since the second half is conditional
			for(i = 0; i < ULTIMATUM_OFFER_SLOTS; ++i)
				ult_opts[i] = GetUltimatumOfferOption(offer_id, i);

			ult_dialog = CheckInventory("ReceivedDialogID");
			ult_body = StrParam(l:GetPromptText(ult_dialog));

			// The opening offer is a greeting picked from three, then the explanation all three
			// share. Every other round is one line and adds nothing.
			if(ult_dialog >= DW_ULTIMATUM_INTRO_BEGIN && ult_dialog <= DW_ULTIMATUM_INTRO_END)
				ult_body = StrParam(s:ult_body, s:"\n\n", l:GetPromptText(DW_ULTIMATUM_INTRO_COMMON));

			ult_body = StrParam(
				s:ult_body,
				s:"\n\n\c[Y5]---------------------------------------\n\n",
				s:"\c[W3]", l:"DND_CHALLENGE", s:":\n\n"
			);

			// numbered by SLOT, so the text and the button under it always carry the same number
			for(i = 0; i < ULTIMATUM_OFFER_SLOTS; ++i) {
				if(ult_opts[i] == -1)
					continue;

				ult_body = StrParam(
					s:ult_body,
					s:"\c[Y5]", d:i + 1, s:") \c[W3]", l:GetUltimatumOptionName(ult_opts[i]), s:"\c-\n     ",
					l:GetUltimatumPromptText(ult_opts[i]),
					s:GetUltimatumTierText(ult_opts[i], GetUltimatumOptionTier(ult_opts[i]) + 1), s:"\n\n"
				);
			}

			HudMessage(s:ult_body; HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0);
		}
		
		// settled before the pane is built, so the hitboxes land on the drawn row
		yOff = InformationInLevel[LEVELINFO_ISULTIMATUM] ?
			GetUltimatumPromptButtonY(ult_body) : 256.0;

		if(voting_ongoing) {
			if(!InformationInLevel[LEVELINFO_ISULTIMATUM]) {
				AddBoxToPane_Points(CurrentPane, 340.0, 196.0, 300.0, 188.0);
				AddBoxToPane_Points(CurrentPane, 260.0, 196.0, 220.0, 188.0);
			}
			else
				BuildUltimatumOfferPane(CurrentPane, offer_id, yOff, false);
		}
	}
	else if(!InformationInLevel[LEVELINFO_ISULTIMATUM]) {
		HudMessage(
			s:"\c[W3]", l:"DND_CHALLENGE", s:" - \cd", l:"DND_ACCEPTED", s:"\c[W3]:\n\n", l:GetPromptText(DW_CHALLENGE_BEGIN + offer_id - 1);
			HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
		);
		
		yOff = 192.0;
	}
	else {
		// An ultimatum offer_id is a PACKED SET of options, not a challenge id, so it must never reach the
		// challenge lookup above -- that is what printed a raw DND_PROMPT lump name. Reachable by
		// anyone who opens the prompt between a vote settling and the next round arming the npc.
		i = NPC_States[DND_NPC_DARKWANDERER].voter_choice[PlayerNumber()];
		if(i != -1) {
			HudMessage(
				s:"\c[W3]", l:"DND_CHALLENGE", s:" - \cd", l:"DND_ACCEPTED", s:"\c[W3]:\n\n",
				s:"\c[Y5]", l:GetUltimatumOptionName(i), s:"\c-\n     ", l:GetUltimatumPromptText(i);
				HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
			);
		}
		else {
			HudMessage(
				s:"\c[W3]", l:"DND_CHALLENGE", s:" - \cd", l:"DND_ACCEPTED", s:"\c[W3]";
				HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
			);
		}
		
		yOff = 192.0;
	}

	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF / 2;

	int boxid = MAINBOX_NONE, boxid_prev = MAINBOX_NONE;
	int pnum = PlayerNumber();
	bool sendInput = false;
	while(isAlive() && CheckInventory("ShowingPrompt") && !CheckInventory("ShowingMenu")) {
		// cursor handling
		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_YAW), MOUSE_INPUT_X);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_PITCH), MOUSE_INPUT_Y);
		
		SetHudClipRect(0, 0, 0, 0);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		DrawCursor();
		SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
		
		// button id recognition
		// Cursor space is the draw space MIRRORED, so the LEFTMOST button carries the highest cursor
		// x -- and the 348 default cut it off entirely once the offer row widened. The class menu and
		// the merchant override this the same way.
		//
		// A four button row reaches 474 at the width cap, so this is simply the whole cursor space:
		// GetCursorPos clamps to HUDMAX_X, and a box only exists where one was added, so opening it
		// up adds reach rather than phantom targets.
		boxid = GetTriggeredBoxOnPane(CurrentPane, PlayerCursorData.posx, PlayerCursorData.posy, HUDMAX_XF);
		if(boxid != boxid_prev && boxid != MAINBOX_NONE)
			LocalAmbientSound("RPG/MenuMove", 127);
			
		boxid_prev = boxid;

		voting_ongoing = n_state != NPC_STATE_VOTE_DECLINE && n_state != NPC_STATE_VOTE_ACCEPT;
		
		// accept
		if(voting_ongoing && InformationInLevel[LEVELINFO_ISULTIMATUM]) {
			// the bank is modal, so the challenge is off the screen entirely while it is up
			if(!ult_showbank) {
				// picked state comes off the SYNCED choice -- the token only says a vote was cast
				i = NPC_States[DND_NPC_DARKWANDERER].voter_choice[pnum];
				j = CheckInventory("NPC_Offer_Accepted");

				for(k = 0; k < ULTIMATUM_OFFER_SLOTS; ++k) {
					if(ult_opts[k] == -1)
						continue;

					DrawPromptButton(
						GetUltimatumOptionName(ult_opts[k]),
						(j && i == ult_opts[k]) ? 2 : (boxid == MBOX_1 + k ? 1 : 0),
						DND_ULTBTN_LABELID(k), DND_ULTBTN_PLATEID(k),
						GetUltimatumOfferButtonX(offer_id, k), yOff
					);
				}

				DrawPromptButton(
					"DND_DECLINE",
					CheckInventory("NPC_Offer_Declined") ? 2 : (boxid == MBOX_1 + ULTIMATUM_OFFER_SLOTS ? 1 : 0),
					DND_ULTBTN_LABELID(ULTIMATUM_OFFER_SLOTS), DND_ULTBTN_PLATEID(ULTIMATUM_OFFER_SLOTS),
					GetUltimatumOfferButtonX(offer_id, ULTIMATUM_OFFER_SLOTS), yOff
				);

				// who sits under which button
				SetFont("SMALLFONT");
				for(k = 0; k < DND_ULTBTN_COUNT; ++k)
					ult_col[k] = 0;

				for(i = 0; i < MAXPLAYERS; ++i) {
					// a vote of 1 is for one of the options, -1 is the decline column on the end
					if(NPC_States[DND_NPC_DARKWANDERER].voters[i] == 1) {
						k = 0;
						for(j = 0; j < ULTIMATUM_OFFER_SLOTS; ++j) {
							if(ult_opts[j] != -1 && NPC_States[DND_NPC_DARKWANDERER].voter_choice[i] == ult_opts[j]) {
								k = j;
								break;
							}
						}
					}
					else if(NPC_States[DND_NPC_DARKWANDERER].voters[i] == -1)
						k = ULTIMATUM_OFFER_SLOTS;
					else
						continue;

					HudMessage(n:i + 1; HUDMSG_PLAIN, RPGMENUITEMID - 20 - i, CR_UNTRANSLATED,
						GetUltimatumOfferButtonX(offer_id, k), yOff + 16.0 * (ult_col[k] + 1), 0.0, 0.0);
					++ult_col[k];
				}
			}

			// what this round pays, and the way in and out of the bank
			DrawUltimatumRewardCell(ULTIMATUM_REWARD_PENDING, DND_ULTREWARD_OFFERX, DND_ULTREWARD_OFFERY, boxid == DND_ULTBOX_CELL, DND_ULTREWARD_HUDID);
			DrawPromptButton(
				GetUltimatumBankButtonLump(ult_showbank),
				boxid == DND_ULTBOX_BANKBTN ? 1 : 0,
				RPGMENUITEMID - 9, RPGMENUITEMID - 8, DND_ULTREWARD_BTNX, DND_ULTREWARD_BTNY
			);

			// look only -- the cells take no click, so nothing here changes any state
			if(ult_showbank) {
				for(i = 0; i < ult_banked; ++i) {
					DrawUltimatumRewardCell(i,
						DND_ULTREWARD_GRIDX + (i % DND_ULTREWARD_PERROW) * DND_ULTREWARD_STEPX,
						DND_ULTREWARD_GRIDY + (i / DND_ULTREWARD_PERROW) * DND_ULTREWARD_STEPY,
						boxid == DND_ULTBOX_REWARDCELL(i), DND_ULTREWARD_HUDID - 2 * (i + 1));
				}
			}

			// the full item tooltip, but only while an icon cell is actually under the cursor
			ult_cell = -1;
			if(boxid == DND_ULTBOX_CELL)
				ult_cell = ULTIMATUM_REWARD_PENDING;
			else if(ult_showbank && boxid >= DND_ULTBOX_REWARDCELL(0) && boxid < DND_ULTBOX_REWARDCELL(ult_banked))
				ult_cell = boxid - DND_ULTBOX_REWARDCELL(0);

			i = ult_cell != -1 ? GetUltimatumRewardItemType(ult_cell) : DND_ITEM_NULL;
			if(i != DND_ITEM_NULL) {
				UpdateCursorHoverData(ult_cell, DND_SYNC_ITEMSOURCE_TRADEVIEW, i, ULTIMATUM_REWARD_OWNER, 0, HUDMAX_X, HUDMAX_Y);
				DrawInventoryInfo(ULTIMATUM_REWARD_OWNER);
				SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
			}
			else if(PlayerCursorData.itemHovered != -1)
				ResetCursorHoverData();
		}
		else if(voting_ongoing) {
			SetFont("SMALLFONT");
			if(CheckInventory("NPC_Offer_Accepted")) {
				HudMessage(l:"DND_ACCEPT"; HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_GREEN, 320.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNC");
			}
			else if(boxid == MBOX_1) {
				HudMessage(s:"\c[M3]", l:"DND_ACCEPT"; HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_WHITE, 320.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNH");
			}
			else {
				HudMessage(s:"\c[Y5]", l:"DND_ACCEPT"; HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_WHITE, 320.4, yOff, 0.0, 0.0);
				SetFont("TRADBTN");
			}
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 1, CR_WHITE, 320.4, yOff, 0.0, 0.0);
			
			// decline
			SetFont("SMALLFONT");
			if(CheckInventory("NPC_Offer_Declined")) {
				HudMessage(l:"DND_DECLINE"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_GREEN, 480.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNC");
			}
			else if(boxid == MBOX_2) {
				HudMessage(s:"\c[M3]", l:"DND_DECLINE"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_WHITE, 480.4, yOff, 0.0, 0.0);
				SetFont("TRADBTNH");
			}
			else {
				HudMessage(s:"\c[Y5]", l:"DND_DECLINE"; HUDMSG_PLAIN, RPGMENUITEMID - 4, CR_WHITE, 480.4, yOff, 0.0, 0.0);
				SetFont("TRADBTN");
			}
			HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2, CR_WHITE, 480.4, yOff, 0.0, 0.0);
			
			// draw people voting yes and no here
			j = 0;
			k = 0;
			SetFont("SMALLFONT");
			for(i = 0; i < MAXPLAYERS; ++i) {
				if(NPC_States[DND_NPC_DARKWANDERER].voters[i] == 1) {
					HudMessage(n:i + 1; HUDMSG_PLAIN, RPGMENUITEMID - 6 - j - k, CR_UNTRANSLATED, 320.4, yOff + 16.0 * (j + 1), 0.0, 0.0);
					++j;
				}
				else if(NPC_States[DND_NPC_DARKWANDERER].voters[i] == -1) {
					HudMessage(n:i + 1; HUDMSG_PLAIN, RPGMENUITEMID - 6 - j - k, CR_UNTRANSLATED, 480.4, yOff + 16.0 * (k + 1), 0.0, 0.0);
					// was ++j -- k is this column's counter
					++k;
				}
			}
		}
		
		// check inputs
		ListenMouseInput();
		sendInput = CheckInventory("MenuInput") != 0;
		if(sendInput && !CheckInventory("DnD_ClickTicker")) {
			GiveInventory("DnD_ClickTicker", 1);

			// Every reward box is local to this client -- looking at your own spoils is nobody's
			// business but yours -- so they are handled ahead of the lockout and stay usable after a
			// vote. USE still has to close the prompt, hence the button check.
			if(InformationInLevel[LEVELINFO_ISULTIMATUM] && boxid >= DND_ULTBOX_CELL &&
				CheckInventory("MenuInput") != DND_MENUINPUT_USEBUTTON) {
				if(boxid == DND_ULTBOX_BANKBTN) {
					ult_showbank = !ult_showbank;
					ult_banked = GetUltimatumRewardCount();
					LocalAmbientSound("RPG/MenuChoose", 127);

					// the grid and the offer text share the body, and the pane has to match what shows
					DeleteTextRange(DND_ULTREWARD_HUDID - 2 * MAX_ULTIMATUM_REWARD_SLOTS - 1, DND_ULTREWARD_HUDID - 2);

					// A hud message with no hold time stays until something replaces it, so the
					// challenge buttons and the vote tally have to be taken down by hand. One range
					// because the plate and label blocks are adjacent.
					DeleteTextRange(DND_ULTBTN_LABELID(DND_ULTBTN_COUNT - 1), DND_ULTBTN_PLATEID(0));
					DeleteTextRange(RPGMENUITEMID - 19 - MAXPLAYERS, RPGMENUITEMID - 20);

					DrawUltimatumBody(ult_body, ult_showbank);
					BuildUltimatumOfferPane(CurrentPane, offer_id, yOff, ult_showbank);
				}
				SetInventory("MenuInput", 0);
			}
			// users can make only one valid click (accept or decline, no spam)
			else if(!CheckInventory("DnD_PromptLocked")) {
				// server gets a few extra info in boxid
				// recomputed per send: latching it meant a second click inside one round trip
				// re-sent the first box. DnD_PromptLocked already holds this to one real decision,
				// so no send pacing is needed here, unlike the item views.
				MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = (boxid | MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK]);
				i = PlayerNumber() | (CheckInventory("MenuInput") << 16);
				// guarantee nonzero input
				if(i) {
					//Log(s:"trying to send prev item ", d:MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] >> 16, s: " vs ", d:MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] >> 16);
					NamedRequestScriptPuke("DND Server Box Receive - NPC", i, MenuInputData[pnum][DND_MENUINPUT_PAYLOAD], DND_NPC_DARKWANDERER);
					// one press, one send -- see the merchant loop. DnD_PromptLocked only arrives
					// a round trip later, so it cannot stop the resend on its own.
					SetInventory("MenuInput", 0);
				}
			}
			else
				SetInventory("MenuInput", 0);
		}

		Delay(const:1);

		// retry ack
		if(CheckInventory("DND_ACK")) {
			sendInput = false;
			SetInventory("MenuInput", 0);
			//Log(s:"reset input data");
			MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = 0;
			MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
		}
	}

	if(InformationInLevel[LEVELINFO_ISULTIMATUM]) {
		// the reward cells sit below the range ClearMenuDisplay sweeps
		DeleteTextRange(DND_ULTREWARD_HUDID - 2 * MAX_ULTIMATUM_REWARD_SLOTS - 1, DND_ULTREWARD_HUDID);
		ResetCursorHoverData();
	}
}

Script "DND Server Box Receive - NPC" (int pnum, int boxid, int npc_id) NET {
	// don't let garbage data slip in
	if(!pnum)
		Terminate;
	int temp = pnum >> 16;
	pnum &= 0xFFFF;

	if(!MenuInputData[pnum][DND_MENUINPUT_DELAY]) {
		SetActivator(pnum + P_TIDSTART);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = DND_MENU_INPUTDELAYTICS;
		MenuInputData[pnum][DND_MENUINPUT] = temp;
		GiveInventory("DND_ACK", 1);
		
		if(temp == DND_MENUINPUT_USEBUTTON) {
			// force close the menu for us
			ClosePrompt();
		}
		else if(boxid != MAINBOX_NONE) {
			if(npc_id != -1) {
				// the option boxes are all an accept, and decline is the one after them
				if(InformationInLevel[LEVELINFO_ISULTIMATUM]) {
					int picked = -1;
					if(boxid >= MBOX_1 && boxid < MBOX_1 + ULTIMATUM_OFFER_SLOTS)
						picked = GetUltimatumOfferOption(NPC_States[npc_id].offer, boxid - MBOX_1);

					if(boxid >= MBOX_1 && boxid < MBOX_1 + ULTIMATUM_OFFER_SLOTS) {
						// empty second slot is not a button
						if(picked != -1) {
							// written here -- Vote Register is at the 3 arg limit already
							NPC_States[npc_id].voter_choice[pnum] = picked;
							ACS_NamedExecuteWithResult("DnD NPC Choice Sync", pnum, picked, npc_id);

							ACS_NamedExecuteWithResult("DnD NPC Vote Register", 1, npc_id, pnum);
							LocalAmbientSound("RPG/MenuChoose", 127);
							GiveInventory("NPC_Offer_Accepted", 1);
							TakeInventory("NPC_Offer_Declined", 1);
							GiveInventory("DnD_PromptLocked", 1);
						}
					}
					else if(boxid == MBOX_1 + ULTIMATUM_OFFER_SLOTS) {
						NPC_States[npc_id].voter_choice[pnum] = -1;
						ACS_NamedExecuteWithResult("DnD NPC Choice Sync", pnum, -1, npc_id);

						ACS_NamedExecuteWithResult("DnD NPC Vote Register", -1, npc_id, pnum);
						LocalAmbientSound("RPG/MenuChoose", 127);
						GiveInventory("NPC_Offer_Declined", 1);
						TakeInventory("NPC_Offer_Accepted", 1);
						GiveInventory("DnD_PromptLocked", 1);
					}
				}
				// any npc that offers challenges here
				else if(boxid == MBOX_1) {
					// in case of accept, declare a vote and wait majority or if only player in game, skip this phase
					ACS_NamedExecuteWithResult("DnD NPC Vote Register", 1, npc_id, pnum);
					LocalAmbientSound("RPG/MenuChoose", 127);
					GiveInventory("NPC_Offer_Accepted", 1);
					TakeInventory("NPC_Offer_Declined", 1);
					GiveInventory("DnD_PromptLocked", 1);
				}
				else if(boxid == MBOX_2) {
					// decline -- mark our vote as a no, wait for voting to conclude optionally
					ACS_NamedExecuteWithResult("DnD NPC Vote Register", -1, npc_id, pnum);
					LocalAmbientSound("RPG/MenuChoose", 127);
					GiveInventory("NPC_Offer_Declined", 1);
					TakeInventory("NPC_Offer_Accepted", 1);
					GiveInventory("DnD_PromptLocked", 1);
				}
			}
			else {

			}
		}
		
		ClearPlayerInput(pnum, true);
		
		Delay(const:DND_MENU_INPUTDELAYTICS);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = 0;
	}
}

Script "DnD NPC Vote Register" (int vote, int npc_id, int pnum) {
	// if concluded accept no further votes
	if(NPC_States[npc_id].n_state == NPC_STATE_VOTE_ACCEPT || NPC_States[npc_id].n_state == NPC_STATE_VOTE_DECLINE)
		Terminate;

	if(!NPC_States[npc_id].voting) {
		NPC_States[npc_id].voting = true;
		NPC_States[npc_id].n_state = NPC_STATE_VOTE_ONGOING;
		ACS_NamedExecuteWithResult("DnD NPC Voting", npc_id);
	}
	
	// register vote
	if(NPC_States[npc_id].voters[pnum] != vote) {
		NPC_States[npc_id].vote_count += vote;
		NPC_States[npc_id].voters[pnum] = vote;
	}
	else
		Terminate;
		
	// check if majority is met -- if our (yes vote count) - (current player count / 2) > 0 that means we can skip further checks, we have majority
	int majority = PlayerCount() / 2 + (PlayerCount() > 1 && (PlayerCount() & 1));
	int curr = NPC_States[npc_id].vote_count - majority;
	int i;
	if(curr > 0) {
		// settle the vote as majority voting yes
		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_VOTEPASSED", s:"!"; HUDMSG_FADEOUT, RPGMENUBACKGROUNDID + 1, CR_GREEN, 240.4, 16.0, 1.0, 1.0);

		// Settled BEFORE ConcludeVoting, which is the only moment the tally still exists: it fires
		// "DnD Close Prompt Delayed" per voter, that runs INLINE up to its delay, and the first thing
		// it does is zero voters[]. Resolved after, every vote reads as uncast and the tie branch hands
		// the win to the first option every single round.
		if(InformationInLevel[LEVELINFO_ISULTIMATUM])
			NPC_States[npc_id].chosen_option = ResolveUltimatumVoteChoice(npc_id);

		ConcludeVoting(npc_id, NPC_STATE_VOTE_ACCEPT);
		
		// handle npc completion
		while(!SpawnedChests)
			Delay(const:10);

		// send the npc away, no more interaction
		SetActorState(DND_NPC_TID, "Succeed", false);

		if(!InformationInLevel[LEVELINFO_ISULTIMATUM])
			HandleNPC(npc_id);
		else
			HandleNPCChallenges();
	}
	else {
		// check if majority actually declined instead of accepted, so we can close voting early
		curr = 0;
		for(i = 0; i < MAXPLAYERS; ++i) {
			if(PlayerInGame(i) && NPC_States[npc_id].voters[i] == -1)
				++curr;
		}

		// conclude voting
		if(curr >= majority) {
			HudMessageBold(s:""; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 1.0, 0.0);
			SetFont("DBIGFONT");
			SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
			HudMessageBold(l:"DND_VOTEFAILED", s:"!"; HUDMSG_FADEOUT, RPGMENUHELPCORNERID, CR_RED, 240.4, 16.0, 1.0, 1.0);
			ConcludeVoting(npc_id, NPC_STATE_VOTE_DECLINE);
		}
	}

	ACS_NamedExecuteWithResult("DnD NPC Vote Sync", pnum, vote, npc_id);
}

Script "DnD Close Prompt Delayed" (int pnum, int npc_id) {
	NPC_States[npc_id].voters[pnum] = 0;
	Delay(const:17);
	ClosePrompt();
}

Script "DnD NPC Voting" (int npc_id) {
	// this is the timer, when this runs out and a majority is not found, we auto decline
	int time = DND_VOTE_TIME * TICRATE;
	while(time > 0 && NPC_States[npc_id].voting) {
		// display the timer to everyone for vote countdown
		str col_tag = "\cd";
		int time_to_show = time / TICRATE;
		if(time_to_show < 2 * DND_VOTE_TIME / 3 && time_to_show > DND_VOTE_TIME / 2)
			col_tag = "\ck";
		else if(time_to_show < DND_VOTE_TIME / 3)
			col_tag = "\ci";
		else if(time_to_show < DND_VOTE_TIME / 5)
			col_tag = "\cg";

		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(s:col_tag, d:time_to_show, s:"\c- ", l:"DND_TOVOTE", s:"!"; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 2.0, 0.0);
		
		Delay(const:1);
		--time;
	}
	
	if(NPC_States[npc_id].n_state == NPC_STATE_VOTE_ONGOING) {
		HudMessageBold(s:""; HUDMSG_PLAIN, RPGMENUBACKGROUNDID + 1, CR_WHITE, 240.4, 16.0, 1.0, 0.0);
		SetFont("DBIGFONT");
		SetHUDSize(HUDMAX_X, HUDMAX_Y, 1);
		HudMessageBold(l:"DND_VOTEFAILED", s:"!"; HUDMSG_FADEOUT, RPGMENUHELPCORNERID, CR_RED, 240.4, 16.0, 1.0, 1.0);
		
		ConcludeVoting(npc_id, NPC_STATE_VOTE_DECLINE);
	}
}

// vote sync carries the yes/no, this carries WHICH option
Script "DnD NPC Choice Sync" (int pnum, int choice, int npc_id) CLIENTSIDE {
	if(pnum >= 0 && pnum < MAXPLAYERS)
		NPC_States[npc_id].voter_choice[pnum] = choice;
	SetResultValue(0);
}

Script "DnD NPC Vote Sync" (int pnum, int vote, int npc_id) CLIENTSIDE {
	if(pnum == -1) {
		// reset. was voters[pnum], and pnum is -1 here -- it cleared nothing and wrote over the
		// field in front of the array, so last round's votes stayed on screen.
		NPC_States[npc_id].vote_count = 0;
		for(int i = 0; i < MAXPLAYERS; ++i) {
			NPC_States[npc_id].voters[i] = 0;
			NPC_States[npc_id].voter_choice[i] = -1;
		}
	}
	else {
		// update
		NPC_States[npc_id].vote_count += vote;
		NPC_States[npc_id].voters[pnum] = vote;
	}
	SetResultValue(0);
}

// pass the accepted challenge modifier id here later
void HandleNPCChallenges() {
	using ultimatum;

	auto curr_tally = GetCurrentUltimatumTally();
	// not final wave that we finished
	if(curr_tally.curr_wave != MAX_ULTIMATUM_WAVES - 1) {
		// apply the challenge modifier, and continue
		// tier is bumped HERE once, so a repeat pick arrives as tier 2 rather than twice at tier 1
		int chosen = NPC_States[DND_NPC_DARKWANDERER].chosen_option;

		// Decided ONCE and handed to both the option and the wave, so the challenge's hazards and
		// the monsters arrive together. A second random() at either site would split them apart.
		// Declining plays no voice line, so it has nothing to wait for.
		int del = TICRATE;

		if(chosen != -1) {
			del = random(DND_ULTIMATUM_CHOICEVOICE_MINDELAY, DND_ULTIMATUM_CHOICEVOICE_MAXDELAY);

			// The option is armed for the wave that is ABOUT to run, but the wave that just ended is
			// still flagged complete. Every spawner polls this and shuts itself down on a 1 -- the saw
			// loop breaks before spawning any blade, and the miasma's own
			// A_JumpIf(CallACS("DnD Is Ultimatum Complete")) sends it to Vanish on its first tic. So it
			// is cleared here, up front, and stays clear across the hold both scripts are about to take.
			curr_tally.is_wave_complete = 0;

			int tier = AddUltimatumOptionTier(chosen);
			ACS_NamedExecuteAlways("DnD Ultimatum Apply Option", 0, chosen, tier);

			// taking a challenge earns the round's reward -- collected when the run ends
			BankUltimatumPendingReward();
		}

		ACS_NamedExecuteAlways("DnD Start Ultimatum Wave", 0, curr_tally.curr_wave + 1, del);
	}
	else {
		ACS_NamedExecuteAlways("DnD Handle Ultimatum Finish", 0);
	}
}

Script "DnD Spawn NPC Spot" (int stid, int npc_type) {
	int newtid = 0;
	str actortype = "";
	if(npc_type == DND_NPC_DARKWANDERER) {
		newtid = DND_NPC_TID;
		actortype = "DarkWanderer";
	}
	else if(npc_type == DND_NPC_MERCHANT) {
		newtid = DND_MERCHANT_TID;
		actortype = "DnD_Merchant";
	}

	SpawnSpotFacing(actortype, stid, newtid);

	if(npc_type == DND_NPC_DARKWANDERER)
		SetThingSpecial(newtid, ACS_ExecuteAlways, 895, 0, 0, DND_NPC_TID);
	else if(npc_type == DND_NPC_MERCHANT)
		SetThingSpecial(newtid, ACS_ExecuteAlways, 895, 0, 1, DND_MERCHANT_TID);

	SetResultValue(0);
}

#endif
