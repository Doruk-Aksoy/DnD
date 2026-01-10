#ifndef DND_MERCHANT_GUI_IN
#define DND_MERCHANT_GUI_IN

#include "DnD_Merchant.h"

#define DND_MERCHANT_CLASSBASED_CHANCE 0.33
#define DND_MERCHANT_MAXITEMSONROW 5
#define DND_MERCHANT_YSHIFTROW 72.0

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

#define DND_MERCHANT_HUDID_OFFSET 5

coord_T item_dimensions;

void AcquireItemDimensions(int pos) {
    item_dimensions.x = 40.0;
    item_dimensions.y = 40.0;

    int item_type = TradeViewList[MAXPLAYERS][pos].item_type & 0xFFFF;

    if(item_type != DND_ITEM_BODYARMOR) {
        item_dimensions.x = 40.0;

        if(item_type == DND_ITEM_CHARM && TradeViewList[MAXPLAYERS][pos].item_subtype != DND_CHARM_SMALL) {
            // we downsize the grand charms to fit the screen better
            item_dimensions.y = 56.0;
        }
    }
    else if(item_type == DND_ITEM_BODYARMOR) {
        item_dimensions.x = 50.0;
        item_dimensions.y = 50.0;
    }
    else
        item_dimensions.x = 56.0;
}

int DrawMerchantItemBox(int item_pos, int boxid, int thisboxid, int hudx, int hudy) {
    SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
    
    int txt_id = 0;
    int item_type = TradeViewList[MAXPLAYERS][item_pos].item_type & 0xFFFF;
    int temp;
    int cpn = ConsolePlayerNumber();
    int yoff = 0;
    int xscale = 1;
    int yscale = 1;
    str borderpic = "";
    if(item_type == DND_ITEM_CHARM) {
        borderpic = GetCharmBoxLabel(TradeViewList[MAXPLAYERS][item_pos].item_subtype, boxid == thisboxid);
        if(TradeViewList[MAXPLAYERS][item_pos].item_subtype == DND_CHARM_LARGE) {
            xscale = 3;
            yscale = 2;
            SetHudSize(HUDMAX_X * xscale / yscale, HUDMAX_Y * xscale / yscale, 1);
        }
    }
    else if(item_type == DND_ITEM_BODYARMOR) {
        if(boxid == thisboxid)
            borderpic = "ARMSELG";
        else
            borderpic = "ARMSELB";
    }
    else if(item_type == DND_ITEM_HELM || item_type == DND_ITEM_BOOT || (item_type >= FIRST_SPECIALTY_ITEM_TYPE && item_type <= LAST_SPECIALTY_ITEM_TYPE)) {
        borderpic = GetCharmBoxLabel(DND_CHARM_SMALL, boxid == thisboxid);
    }

    if(item_pos >= DND_MERCHANT_MAXITEMSONROW)
        yoff += DND_MERCHANT_YSHIFTROW;

    SetFont(GetItemImage(TradeViewList[MAXPLAYERS][item_pos].item_image));
    HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid - DND_MERCHANT_HUDID_OFFSET - 1, CR_WHITE, GetIntegerBits(hudx) * xscale / yscale + item_dimensions.x / 2 + 0.4, (hudy + yoff) * xscale / yscale, 0.0, 0.0);
    
    if(boxid == thisboxid) {
        UpdateCursorHoverData(thisboxid - 1, DND_SYNC_ITEMSOURCE_TRADEVIEW, TradeViewList[MAXPLAYERS][item_pos].item_type, MAXPLAYERS, 0, HUDMAX_X, HUDMAX_Y);

        // hovered so we can return some special text here
        if(random(0, 1.0) <= DND_MERCHANT_CLASSBASED_CHANCE) {
            temp = GetPlayerCredit(cpn);
            if(GetMerchantItemPrice(item_pos) > temp) {
                temp = GetActorPlayerClass(cpn + P_TIDSTART);
                if(temp == DND_PLAYER_DOOMGUY)
                    txt_id = DND_MRCHT_DOOMGUY_CANTAFFORD;
                else if(temp == DND_PLAYER_MARINE)
                    txt_id = DND_MRCHT_MARINE_CANTAFFORD;
                else if(temp == DND_PLAYER_HOBO)
                    txt_id = DND_MRCHT_HOBO_CANTAFFORD;
                else if(temp == DND_PLAYER_WANDERER)
                    txt_id = random(DND_MRCHT_WANDERER_CANTAFFORD1, DND_MRCHT_WANDERER_CANTAFFORD2);
                else if(temp == DND_PLAYER_TRICKSTER)
                    txt_id = DND_MRCHT_TRICKSTER_CANTAFFORD;
            }
        }
        else if(!TradeViewList[MAXPLAYERS][item_pos].item_stack) {
            temp = GetActorLevel(cpn + P_TIDSTART);
            if(temp > TradeViewList[MAXPLAYERS][item_pos].item_level * 3) {
                if(item_type != DND_ITEM_CHARM)    
                    txt_id = DND_MRCHT_TEXT_LOWLEVELARMOR;
                else if(item_type == DND_ITEM_CHARM)
                    txt_id = DND_MRCHT_TEXT_LOWLEVELCHARM;
            }
            else if(temp < TradeViewList[MAXPLAYERS][item_pos].item_level - 10) {
                txt_id = random(DND_MRCHT_HOVER_HIGHLEVELITEM1, DND_MRCHT_HOVER_HIGHLEVELITEM2);
            }
        }
    }
    else
        txt_id = DND_MRCHT_TEXT_DEFAULT;

    SetFont(borderpic);
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUITEMID - 2 * thisboxid - DND_MERCHANT_HUDID_OFFSET, CR_WHITE, hudx * xscale / yscale, (hudy + yoff) * xscale / yscale, 0.0, 0.0);
    SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);

    return txt_id;
}

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

void UpdateMerchantText(int txt_id) { 
    str merchant_text = GetMerchantText(txt_id);

    SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
    SetHudClipRect(160, 112, 532, 600, 532);
    SetFont("SMALLFONT");
    HudMessage(
        l:merchant_text;
        HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
    );

    SetHudClipRect(0, 0, 0, 0);
    SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
}

void UpdateMerchantItemPriceText(int boxid) {
    // show user the hovered item's price
    if(boxid != MAINBOX_NONE) {
        SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
        SetHudClipRect(160, 112, 532, 600, 532);
        SetFont("SMALLFONT");

        str price_col = "\cd";
        int price = GetMerchantItemPrice(boxid - 1);
        if(GetPlayerCredit(ConsolePlayerNumber()) < price)
            price_col = "\cg";

        HudMessage(
            s:"\c[Y5]", l:"DND_PRICE", s:": ", s:price_col, s:"$", d:price;
            HUDMSG_PLAIN, RPGMENUITEMID - 3, CR_WHITE, 160.1, 450.1, 0.0, 0.0
        );

        SetHudClipRect(0, 0, 0, 0);
        SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
    }
    else
        DeleteText(RPGMENUITEMID - 3);
}

// returns item count
int UpdateMerchantBoxes(menu_pane_T module& p) {
    ResetPane(p);

    int ipos = 0, offset = 0, yoff = 0;
    int item_type = TradeViewList[MAXPLAYERS][ipos].item_type & 0xFFFF;
    while(item_type != DND_ITEM_NULL) {
        AcquireItemDimensions(ipos);

        if(item_type == DND_ITEM_CHARM && TradeViewList[MAXPLAYERS][ipos].item_subtype == DND_CHARM_LARGE)
            item_dimensions.x = 26.0;

        AddBoxToPane_Points(p, 400.0 - offset, 200.0 + item_dimensions.y / 2 - yoff, 400.0 - offset - item_dimensions.x, 200.0 - item_dimensions.y / 2 - yoff);

        offset += item_dimensions.x + 10.0;
        ++ipos;
        if(ipos == DND_MERCHANT_MAXITEMSONROW) {
            yoff = DND_MERCHANT_YSHIFTROW;
            offset = 0;
        }

        item_type = TradeViewList[MAXPLAYERS][ipos].item_type & 0xFFFF;
    }
    
    return ipos;
}

Script "DnD Prompt Merchant" (void) CLIENTSIDE {
	if(ConsolePlayerNumber() != PlayerNumber())
		Terminate;

	// draw the background once
	SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
	SetFont("MRCHV0");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, -1, 392.0, 160.0, 0.0, 0.0);
	SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
	SetFont("PROMPT");
	HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUBACKGROUNDID, -1, 480.0, 320.0, 0.0, 0.0);

	// draw greetings text
	SetFont("SMALLFONT");

	HudMessage(
		s:"\c[L7]", l:"CLASS_PRESS", s:" \ci", k:"+use", s: " \c[L7]", l:"CLASS_TOCLOSE",
        s:" ", l:"DND_AND", s:" \ci", k:"+attack", s:" \c[L7]", l:"DND_TOPURCHASE", s:"!";
		HUDMSG_PLAIN, RPGMENUITEMID - 1, -1, (HUDMAX_X_PROMPT << 15) + 0.4, ((HUDMAX_Y_PROMPT << 16) - 100.0) + 0.1, 0.0, 0.0
	);

	SetHudClipRect(160, 112, 532, 600, 532);
	
	menu_pane_T module& CurrentPane = GetPane();

    int item_count = 0;
    int i = 0, j = 0, k = 0;
    item_count = UpdateMerchantBoxes(CurrentPane);

    HudMessage(
        l:GetMerchantText(DND_MRCHT_TEXT_DEFAULT);
        HUDMSG_PLAIN, RPGMENUITEMID, CR_WHITE, 160.1, 128.1, 0.0, 0.0
    );
	HudMessage(
		l:"DND_MERCHANTHEADER";
		HUDMSG_PLAIN, RPGMENUITEMID - 2, CR_WHITE, 440.4, 112.1, 0.0, 0.0
	);

	PlayerCursorData.posx = HUDMAX_XF / 2;
	PlayerCursorData.posy = HUDMAX_YF / 2;

	int boxid = MAINBOX_NONE, boxid_prev = MAINBOX_NONE;
	int pnum = PlayerNumber();
	bool sendInput = false;
    bool redraw = false;
    GiveInventory("DnD_RefreshPane", 1); // force items to draw first time
	while(isAlive() && CheckInventory("ShowingPrompt") && !CheckInventory("ShowingMenu")) {
        redraw = false;
		// cursor handling
		PlayerCursorData.posx = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_YAW), MOUSE_INPUT_X);
		PlayerCursorData.posy = GetCursorPos(GetPlayerInput(ConsolePlayerNumber(), INPUT_PITCH), MOUSE_INPUT_Y);
		
		SetHudClipRect(0, 0, 0, 0);
		SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
		DrawCursor();
		
		// button id recognition
		boxid = GetTriggeredBoxOnPane(CurrentPane, PlayerCursorData.posx, PlayerCursorData.posy, 402.0);
		if(boxid != boxid_prev) {
            redraw = true;

            if(boxid != MAINBOX_NONE)
			    LocalAmbientSound("RPG/MenuMove", 127);
        }

        boxid_prev = boxid;

        UpdateMerchantItemPriceText(boxid);

        // check inputs
        ListenMouseInput();
        sendInput = CheckInventory("MenuInput") != 0;
        if(sendInput && !CheckInventory("DnD_ClickTicker")) {
            GiveInventory("DnD_ClickTicker", 1);
            // server gets a few extra info in boxid
            if(!MenuInputData[pnum][DND_MENUINPUT_PAYLOAD])
                MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = (boxid | MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK]);
            i = PlayerNumber() | (CheckInventory("MenuInput") << 16);
            // guarantee nonzero input
            if(i) {
                redraw = true;
                //Log(s:"trying to send prev item ", d:MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] >> 16, s: " vs ", d:MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] >> 16);
                NamedRequestScriptPuke("DND Server Box Receive - Merchant", i, MenuInputData[pnum][DND_MENUINPUT_PAYLOAD]);
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

        // cant buy dynamic text
        if(CheckInventory("DnD_SpecialText_Timer")) {
            SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
            SetFont("MRCHT0");
            HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, -1, 388.0, 162.0, 0.0, 0.0);

            SetHudSize(HUDMAX_X_PROMPT, HUDMAX_Y_PROMPT, 1);
            SetFont("SMALLFONT");
            HudMessage(
                l:"DND_POPUP_NOFUNDS";
                HUDMSG_PLAIN, RPGMENUITEMID - 4, -1, (HUDMAX_X_PROMPT << 15) + 0.4, ((HUDMAX_Y_PROMPT << 16) - 110.0) + 0.1, 0.0, 0.0
            );
        }
        else {
            SetHudSize(HUDMAX_X, HUDMAX_Y, 1);
            SetFont("MRCHV0");
            HudMessage(s:"A"; HUDMSG_PLAIN, RPGMENUID, -1, 392.0, 160.0, 0.0, 0.0);

            DeleteText(RPGMENUITEMID - 4);
        }

        if(redraw || CheckInventory("DnD_RefreshPane")) {
            // store previous count, check if there's difference, if there is, remove the last item from right as items are possibly shifted
            k = item_count;
            item_count = 0;
            j = 0;
            item_count = UpdateMerchantBoxes(CurrentPane);

            // k will be greater or equal to new item_count at all times, since we are not later resupplying this guy
            if(k != item_count) {
                for(i = item_count; i < k; ++i) {
                    DeleteText(RPGMENUITEMID - 2 * (i + 1) - DND_MERCHANT_HUDID_OFFSET);
                    DeleteText(RPGMENUITEMID - 2 * (i + 1) - DND_MERCHANT_HUDID_OFFSET - 1);
                }
            }

            j  = 0;
            for(i = 0; i < item_count; ++i) {
                AcquireItemDimensions(i);
                if((k = DrawMerchantItemBox(i, boxid, i + 1, 80.1 + j, 120.0)) != DND_MRCHT_TEXT_DEFAULT)
                    UpdateMerchantText(k);

                if((TradeViewList[MAXPLAYERS][i].item_type & 0xFFFF) == DND_ITEM_CHARM && TradeViewList[MAXPLAYERS][i].item_subtype == DND_CHARM_LARGE)
                    item_dimensions.x = 26.0;

                j += item_dimensions.x + 10.0;
                if(i == DND_MERCHANT_MAXITEMSONROW - 1)
                    j = 0;
            }

            if(boxid == MAINBOX_NONE)
                UpdateMerchantText(DND_MRCHT_TEXT_DEFAULT);
        }

        // retry ack
        if(CheckInventory("DND_ACK")) {
            sendInput = false;
            SetInventory("MenuInput", 0);
            //Log(s:"reset input data");
            MenuInputData[pnum][DND_MENUINPUT_PAYLOAD] = 0;
            MenuInputData[pnum][DND_MENUINPUT_PLAYERCRAFTCLICK] = 0;
        }
	}

    CleanInventoryInfo();
    DeleteText(RPGMENUID);
    DeleteTextRange(RPGMENUITEMID - 6 - MAXPLAYERS, RPGMENUITEMID);
}

Script "DND Server Box Receive - Merchant" (int pnum, int boxid) NET {
	// don't let garbage data slip in
	if(!pnum)
		Terminate;
	int temp = pnum >> 16;
    int item_count = 0;
    int i;
	pnum &= 0xFFFF;

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
                    while(TradeViewList[MAXPLAYERS][item_count].item_type != DND_ITEM_NULL)
                        ++item_count;

                    TakeCredit(GetMerchantItemPrice(boxid - 1));

                    // if its a stackable item, clear its ilvl that we stored pricing stuff, we are through price obtaining
                    if(TradeViewList[MAXPLAYERS][boxid - 1].item_stack)
                        TradeViewList[MAXPLAYERS][boxid - 1].item_level = 0;

                    CarryItemTo(boxid - 1, temp, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, MAXPLAYERS, pnum, false, false, true); // false here prevents freeing of width and height stuff
                    // move every item left by 1
                    for(i = boxid - 1; i < item_count - 1; ++i)
                        CarryItemTo(i + 1, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, DND_SYNC_ITEMSOURCE_TRADEVIEW, MAXPLAYERS, MAXPLAYERS, false, true, true);

                    SyncItemData(pnum, temp, DND_SYNC_ITEMSOURCE_PLAYERINVENTORY, -1, -1);
                    for(i = 0; i < item_count - 1; ++i)
                        SyncItemData(MAXPLAYERS, i, DND_SYNC_ITEMSOURCE_TRADEVIEW, -1, -1, true);
                    SyncItemData_Null(MAXPLAYERS, item_count - 1, DND_SYNC_ITEMSOURCE_TRADEVIEW, 1, 1, true);

                    TakeInventory("DnD_SpecialText_Timer", 1);
                    GiveInventory("DnD_RefreshPane", 1);
                }
                else
                    ACS_NamedExecuteAlways("DnD Inventory Full CS", 0, pnum);
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