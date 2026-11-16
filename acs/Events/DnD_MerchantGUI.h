#ifndef DND_MERCHANT_GUI_IN
#define DND_MERCHANT_GUI_IN

#include "DnD_Merchant.h"

#define DND_MERCHANT_CLASSBASED_CHANCE 0.33

enum {
    DND_MRCHT_TEXT_DEFAULT,
    DND_MRCHT_TEXT_LOWLEVELARMOR,
    DND_MRCHT_TEXT_LOWLEVELCHARM,

    DND_MRCHT_HOVER_HIGHLEVELITEM1,
    DND_MRCHT_HOVER_HIGHLEVELITEM2,

    DND_MRCHT_DOOMGUY_CANTAFFORD,

    DND_MRCHT_MARINE_CANTAFFORD,

    DND_MRCHT_HOBO_CANTAFFORD,

    DND_MRCHT_WANDERER_CANTAFFORD1,
    DND_MRCHT_WANDERER_CANTAFFORD2,

    DND_MRCHT_TRICKSTER_CANTAFFORD
};

str GetMerchantText(int id) {
    str txt = "";
    switch(id) {
        case DND_MRCHT_TEXT_DEFAULT:
            txt = "DND_MRCHT_GREET";
        break;
        case DND_MRCHT_TEXT_LOWLEVELARMOR:
            txt = "DND_MRCHT_HOVER_LOWLEVELARMOR";
        break;
        case DND_MRCHT_TEXT_LOWLEVELCHARM:
            txt = "DND_MRCHT_HOVER_LOWLEVELCHARM";
        break;
        case DND_MRCHT_HOVER_HIGHLEVELITEM1:
            txt = "DND_MRCHT_HOVER_HIGHLEVELITEM1";
        break;
        case DND_MRCHT_HOVER_HIGHLEVELITEM2:
            txt = "DND_MRCHT_HOVER_HIGHLEVELITEM2";
        break;

        case DND_MRCHT_DOOMGUY_CANTAFFORD:
            txt = "DND_MRCHT_DOOMGUY_CANTAFFORD";
        break;

        case DND_MRCHT_MARINE_CANTAFFORD:
            txt = "DND_MRCHT_MARINE_CANTAFFORD";
        break;

        case DND_MRCHT_HOBO_CANTAFFORD:
            txt = "DND_MRCHT_HOBO_CANTAFFORD";
        break;

        case DND_MRCHT_WANDERER_CANTAFFORD1:
            txt = "DND_MRCHT_WANDERER_CANTAFFORD1";
        break;
        case DND_MRCHT_WANDERER_CANTAFFORD2:
            txt = "DND_MRCHT_WANDERER_CANTAFFORD2";
        break;

        case DND_MRCHT_TRICKSTER_CANTAFFORD:
            txt = "DND_MRCHT_TRICKSTER_CANTAFFORD";
        break;
    }

    return txt;
}

// The merchant window on the menu's canvas: his portrait and line up top, his stock as wells with prices below.
// Mutually exclusive with the menu, so it borrows the crafting id band.
#define MRC_ID MCH_ID_CRAFT_FIRST
#define MRC_PANE_X 232
#define MRC_PANE_Y 160
#define MRC_CELL 140
#define MRC_CELL_GAP 36
#define MRC_CELL_X 298
#define MRC_CELL_Y 400
#define MRC_ROW_STEP 190
#define MRC_COLS 5

int MerchantItemCount() {
	int n = 0;
	while(n < DND_MERCHANT_LIMITITEMS && (GlobalItemStorage.TradeViewList[MAXPLAYERS][n].item_type & 0xFFFF) != DND_ITEM_NULL)
		++n;
	return n;
}

int MerchantCellX(int i) {
	return MRC_CELL_X + (MRC_CELL + MRC_CELL_GAP) * (i % MRC_COLS);
}

int MerchantCellY(int i) {
	return MRC_CELL_Y + MRC_ROW_STEP * (i / MRC_COLS);
}

// the box the server knows: item i is box i + 1
int MerchantHit(int count) {
	int cx = MchCursorX(), cy = MchCursorY();
	for(int i = 0; i < count; ++i)
		if(MchIn(cx, cy, MerchantCellX(i), MerchantCellY(i), MRC_CELL, MRC_CELL))
			return i + 1;
	return MAINBOX_NONE;
}

// what he says about a hovered item: a jab when it is out of reach, a remark on its level, or nothing new
int MerchantHoverText(int pos) {
	auto item = GetMerchantItem(pos);
	int cpn = ConsolePlayerNumber(), temp, item_type = item.item_type & 0xFFFF;
	if(random(0, 1.0) <= DND_MERCHANT_CLASSBASED_CHANCE) {
		if(GetMerchantItemPrice(pos) > GetPlayerCredit(cpn)) {
			temp = GetActorPlayerClass(cpn + P_TIDSTART);
			if(temp == DND_PLAYER_DOOMGUY)
				return DND_MRCHT_DOOMGUY_CANTAFFORD;
			if(temp == DND_PLAYER_MARINE)
				return DND_MRCHT_MARINE_CANTAFFORD;
			if(temp == DND_PLAYER_HOBO)
				return DND_MRCHT_HOBO_CANTAFFORD;
			if(temp == DND_PLAYER_WANDERER)
				return random(DND_MRCHT_WANDERER_CANTAFFORD1, DND_MRCHT_WANDERER_CANTAFFORD2);
			if(temp == DND_PLAYER_TRICKSTER)
				return DND_MRCHT_TRICKSTER_CANTAFFORD;
		}
	}
	else if(!item.item_stack) {
		temp = GetActorLevel(cpn + P_TIDSTART);
		if(temp > item.item_level * 3)
			return (item_type != DND_ITEM_CHARM) ? DND_MRCHT_TEXT_LOWLEVELARMOR : DND_MRCHT_TEXT_LOWLEVELCHARM;
		if(temp < item.item_level - 10)
			return random(DND_MRCHT_HOVER_HIGHLEVELITEM1, DND_MRCHT_HOVER_HIGHLEVELITEM2);
	}
	return DND_MRCHT_TEXT_DEFAULT;
}

// a sprite k times larger with its feet on (cx, by): the portraits carry bottom-centre offsets, so they anchor top left
void MerchantPortrait(str pic, int id, int cx, int by, int k) {
	MenuHudSize(FixedDiv(MCH_W << 16, k) >> 16, FixedDiv(MCH_H << 16, k) >> 16);
	SetFont(pic);
	HudMessage(s:"A"; HUDMSG_PLAIN, id, CR_UNTRANSLATED, MenuX((FixedDiv(cx << 16, k) & MMASK) + 0.1), MenuY((FixedDiv(by << 16, k) & MMASK) + 0.1), 0.0);
	MenuHudSize(MCH_W, MCH_H);
}

void DrawMerchantWindow(int boxid, int count, int txt, bool poor) {
	int i, credit = GetPlayerCredit(ConsolePlayerNumber());
	MenuHudSize(MCH_W, MCH_H);
	MchPicScaled("MCDIM", MRC_ID + 44, MCH_W / 2, MCH_H / 2, 120.0);
	MchPic("MCPANE", MRC_ID + 43, MRC_PANE_X, MRC_PANE_Y);
	MchPic("MCWELL", MRC_ID + 41, MRC_PANE_X + 30, MRC_PANE_Y + 30);
	// a purchase he cannot make for you swaps him to his displeased look
	MerchantPortrait(poor ? "MRCHT0" : "MRCHV0", MRC_ID + 40, MRC_PANE_X + 160, MRC_PANE_Y + 192, 2.8);
	MchText("MENUHEAD", StrParam(s:"\c[UIGold]", l:"DND_MRC_NAME"), MRC_ID, MRC_PANE_X + 320, MRC_PANE_Y + 52, 1);
	// his line, then "insufficient funds" right under it; his longest lines take the smaller font so both still fit
	str line = StrParam(l:GetMerchantText(txt)), body = StrParam(s:"\c[UIWhite]", s:line);
	if(poor)
		body = StrParam(s:body, s:"\n\c[UIRed]", l:"DND_POPUP_NOFUNDS");
	MenuClipRect(MRC_PANE_X + 320, MRC_PANE_Y + 80, 620, 136, 620);
	MchText((StrLen(line) > 150) ? "MENUBDYS" : "MENUBODY", body, MRC_ID + 1, MRC_PANE_X + 320, MRC_PANE_Y + 84, 3);
	MenuClipRect(0, 0, 0, 0, 0);
	MenuClipRect(MRC_PANE_X + 30, MRC_PANE_Y + 222, 916, 2);
	MchPic("MCDIV", MRC_ID + 42, MRC_PANE_X + 30, MRC_PANE_Y + 222);
	MenuClipRect(0, 0, 0, 0, 0);

	// the stock: a well each, the art fitted to it, the price below in gold or in red when it is out of reach
	DeleteTextRange(MRC_ID + 10, MRC_ID + 39);
	for(i = 0; i < count; ++i) {
		auto item = GetMerchantItem(i);
		int x = MerchantCellX(i), y = MerchantCellY(i), price = GetMerchantItemPrice(i);
		int k = Min(3.2, FixedDiv(124.0, (24 * Max(Max(item.width, item.height), 1)) << 16));
		MchPic((boxid == i + 1) ? "MCMCEL1" : "MCMCEL0", MRC_ID + 30 + i, x - 10, y - 10);
		MchPicScaled(GetItemImage(item.item_image), MRC_ID + 20 + i, x + MRC_CELL / 2, y + MRC_CELL / 2, k);
		MchText("MENUBODY", StrParam(s:(price > credit) ? "\c[UIRed]" : "\c[UIGold]", s:MchMoney(price)), MRC_ID + 10 + i, x + MRC_CELL / 2, y + MRC_CELL + 18, 0);
		if(boxid == i + 1)
			UpdateCursorHoverData(i, DND_SYNC_ITEMSOURCE_TRADEVIEW, item.item_type, MAXPLAYERS, 0, HUDMAX_X, HUDMAX_Y);
	}
	MchText("MENUSUB", StrParam(s:"\c[UIGrey]", l:"DND_MENU_CREDITS", s:" \c[UIGold]", s:MchMoney(credit)), MRC_ID + 3, MRC_PANE_X + 30, MRC_PANE_Y + 636, 1);
	MchText("MENUSUB", StrParam(s:"\c[UIGrey]", k:"+attack", s:"  ", l:"DND_MRC_BUY", s:"      ", k:"+use", s:"  ", l:"DND_MRC_CLOSE"), MRC_ID + 2, MRC_PANE_X + 946, MRC_PANE_Y + 636, 2);
	MenuHudSize(HUDMAX_X, HUDMAX_Y);
}

Script "DnD Prompt Merchant" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	UpdateMenuZoom(true);
	SyncMenuTheme();
	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF / 2;

	int boxid = MAINBOX_NONE, boxid_prev = MAINBOX_NONE, i, k, count;
	int pnum = PlayerNumber(), txt = DND_MRCHT_TEXT_DEFAULT;
	// roll: he picks a line again, as the old window did on every redraw a hover, a click or a purchase caused
	bool sendInput = false, redraw = true, poor = false, poor_prev = false, roll = false;
	// tics left before we may send another input, see the send site below
	int op_cooldown = 0;
	while(isAlive() && CheckInventory("ShowingPrompt") && !CheckInventory("ShowingMenu")) {
		// the cursor reaches past the design space like the menu's
		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(-1, INPUT_YAW), MOUSE_INPUT_X, HUDMAX_X + MCH_CURSOR_OVER, HUDMAX_Y, -MCH_CURSOR_OVER << 16);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(-1, INPUT_PITCH), MOUSE_INPUT_Y);
		if(UpdateMenuZoom(true))
			redraw = true;

		count = MerchantItemCount();
		boxid = MerchantHit(count);
		if(boxid != boxid_prev) {
			redraw = true;
			roll = true;
			if(boxid != MAINBOX_NONE)
				LocalAmbientSound("RPG/MenuMove", 127);
			else
				txt = DND_MRCHT_TEXT_DEFAULT;
		}
		boxid_prev = boxid;

		// check inputs
		ListenMouseInput();

		if(op_cooldown)
			--op_cooldown;

		// DnD_ClickTicker only debounces us for 3 tics while the server discards anything inside its own 4 tic
		// window, so sends are paced to DND_MENU_SENDINTERVAL as the menu's loop does
		sendInput = CheckInventory("MenuInput") != 0;
		if(sendInput && !op_cooldown && !CheckInventory("DnD_ClickTicker")) {
			GiveInventory("DnD_ClickTicker", 1);
			// recomputed per send: latching it meant every click inside one round trip re-sent the FIRST box
			MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = (boxid | MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK]);
			i = PlayerNumber() | (CheckInventory("MenuInput") << 16);
			// guarantee nonzero input
			if(i) {
				redraw = true;
				roll = true;
				op_cooldown = DND_MENU_SENDINTERVAL;
				NamedRequestScriptPuke("DND Server Box Receive - Merchant", i, MenuInputData[pnum][DND_MENUINPUT_PAYLOAD]);
				// one press, one send: cleared here, not on the ack, or a high ping bought the item twice
				SetInventory("MenuInput", 0);
			}
		}

		Delay(const:1);

		if(PlayerCursorData.itemHovered != -1) {
			if(boxid == MAINBOX_NONE || PlayerCursorData.hoverNeedsReset)
				ResetCursorHoverData();
			else
				DrawInventoryInfo(PlayerCursorData.owner_pnum);
		}
		else if(PlayerCursorData.hoverNeedsReset)
			ResetCursorHoverData();

		// the server's "cannot afford" timer
		poor = CheckInventory("DnD_SpecialText_Timer");
		if(poor != poor_prev) {
			redraw = true;
			poor_prev = poor;
		}

		if(redraw || CheckInventory("DnD_RefreshPane")) {
			// a hover keeps his last line unless he has something to say about this item
			if(boxid != MAINBOX_NONE && (roll || CheckInventory("DnD_RefreshPane"))) {
				k = MerchantHoverText(boxid - 1);
				if(k != DND_MRCHT_TEXT_DEFAULT)
					txt = k;
			}
			DrawMerchantWindow(boxid, count, txt, poor);
			redraw = false;
			roll = false;
		}
		MenuHudSize(HUDMAX_X, HUDMAX_Y);
		DrawCursor();

		// retry ack
		if(CheckInventory("DND_ACK")) {
			sendInput = false;
			SetInventory("MenuInput", 0);
			MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = 0;
			MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
		}
	}

	CleanInventoryInfo();
	DeleteTextRange(MRC_ID, MRC_ID + 44);
	DeleteText(RPGMENUCURSORID);
	UpdateMenuZoom(false);
}

Script "DND Server Box Receive - Merchant" (int pnum, int boxid) NET {
	// don't let garbage data slip in
	if(!pnum)
		Terminate;
	int temp = pnum >> 16;
    int item_count = 0;
    int i;
	pnum &= 0xFFFF;
	if(!IsRequestFromPlayer(pnum))
		Terminate;

	if(!MenuInputData[pnum][DND_MENUINPUT_DELAY]) {
		SetActivator(pnum + P_TIDSTART);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = DND_MENU_INPUTDELAYTICS;
		MenuInputData[pnum][DND_MENUINPUT] = temp;
		GiveInventory("DND_ACK", 1);
		
		if(temp == DND_MENUINPUT_USEBUTTON) {
			// force close the menu for us
			ClosePrompt(true);
		}
		else if(boxid != MAINBOX_NONE) {
            // check money spend conditions and item space availability
            if(GetMerchantItemPrice(boxid - 1) <= GetPlayerCredit(pnum)) {
                temp = GetFreeSpotForItem_Trade(boxid - 1, MAXPLAYERS, pnum, DND_SYNC_ITEMSOURCE_TRADEVIEW, true);
                if(temp != -1) {
                    LocalAmbientSound("RPG/MenuChoose", 127);

                    // get the item count of merchant
                    while(GlobalItemStorage.TradeViewList[MAXPLAYERS][item_count].item_type != DND_ITEM_NULL)
                        ++item_count;

                    TakeCredit(GetMerchantItemPrice(boxid - 1));

                    // if its a stackable item, clear its ilvl that we stored pricing stuff, we are through price obtaining
                    if(GlobalItemStorage.TradeViewList[MAXPLAYERS][boxid - 1].item_stack)
                        GlobalItemStorage.TradeViewList[MAXPLAYERS][boxid - 1].item_level = 0;

                    CarryItemTo(boxid - 1, temp, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, MAXPLAYERS, pnum, false, false, true); // false here prevents freeing of width and height stuff
                    // move every item left by 1
                    for(i = boxid - 1; i < item_count - 1; ++i)
                        CarryItemTo(i + 1, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_TRADEVIEW, MAXPLAYERS, MAXPLAYERS, false, true, true);

                    SyncItemData(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, -1, -1);
                    for(i = 0; i < item_count - 1; ++i)
                        SyncItemData(MAXPLAYERS, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, -1, -1, true);
                    SyncItemData_Null(MAXPLAYERS, item_count - 1, DND_SYNC_ITEMSOURCE_TRADEVIEW, 1, 1, true);

                    TakeInventory("DnD_SpecialText_Timer", 1);

                    // make every player refresh
                    for(i = 0; i < MAXPLAYERS; ++i) {
                        temp = i + P_TIDSTART;
                        if(PlayerInGame(i) && !PlayerIsSpectator(i) && IsActorAlive(temp))
                            GiveActorInventory(temp, "DnD_RefreshPane", 1);
                    }
                }
                else
                    SendOwnerScript("DnD Inventory Full CS", pnum, pnum);
            }
            else {
                LocalAmbientSound("RPG/MenuError", 127);
                GiveInventory("DnD_SpecialText_Timer", 1);
            }
		}
		
		ClearPlayerInput(pnum, true);
		
		Delay(const:DND_MENU_INPUTDELAYTICS);
		MenuInputData[pnum][DND_MENUINPUT_DELAY] = 0;
	}
}

#endif