#ifndef DND_BUFFS_IN
#define DND_BUFFS_IN

// Has all sorts of generalized buff/debuff book keeping here

// when giving durations to buffs based on seconds, make sure to accommodate for this here, a duration tic is lost every 7 tics, so
// if a buff lasts for 10 seconds, its duration value should be 10 * 7 = 70, because 70 tic times each lasting 5 tics will result in 350 tics = 10 seconds
#define DND_BUFF_TICKTIME 5
#define DND_BUFF_TICK_SECOND_ADJUST(x) ((x) * (TICRATE / DND_BUFF_TICKTIME))

enum {
	BUFF_SPEED,
	BUFF_DAMAGEDEALT,
	BUFF_DAMAGETAKEN,
	BUFF_ARMORINCREASE,
	BUFF_STUN,
	BUFF_VISIONLOSS,
	BUFF_HEALTHREGEN,
	BUFF_CRITPERCENT,

	BUFF_PHASING,

	BUFF_TYPES_MAX
};

enum {
	BUFF_F_MONSTERSOURCE 				= 0b1,
	BUFF_F_PLAYERSOURCE 				= 0b10,
	BUFF_F_WEAPONSOURCE					= 0b100,
	BUFF_F_MORETYPE						= 0b1000, 						// multiplicative instead of additive
	BUFF_F_TICKERREQUIRED 				= 0b10000,
	BUFF_F_NODUPLICATE					= 0b100000,						// doesn't allow duplicates where source, type, flags and value are same
	BUFF_F_NODUPLICATE_STRICT			= 0b1000000,					// doesn't allow duplicates from ANYTHING, can only have one, and strongest will overwrite it
	BUFF_F_REPLACEMENTVALUE				= 0b10000000,					// same buff with different value attachment
	BUFF_F_DURATIONINTICS				= 0b100000000,					// duration is in real tics
	BUFF_F_UNIQUETOCLASS				= 0b1000000000,					// can only have one of this buff from this class (implied NODUPLICATE)
	BUFF_F_ADDIFNODUPLICATE				= 0b10000000000,				// adds value on top if BUFF_F_NODUPLICATE_STRICT is used instead of replace
};
#define BUFF_SOURCE_FLAGMASK (BUFF_F_WEAPONSOURCE | BUFF_F_PLAYERSOURCE | BUFF_F_MONSTERSOURCE)

struct buff_T {
	int source;			// source of this buff, we will never insert a buff of same type from same source multiple times, we'll replace weakest with this instead
	
	int type;			// type of buff
	int bt_index;		// buff table index
	int value;			// value to be used for effect of the buff

	int flags;			// holds flags for this buff, what kind of source, does it require ticker etc.
	int duration;		// duration of buff

	int next_id;		// next buff pointer
};

#define DND_BUFF_SOURCE_BITS 14 // 14 bits for source tids is enough with 16384
#define DND_BUFF_FLAG_BITS 7 // 128 as max value of buff type, leaves 11 bits for extra info

#define DND_BUFF_FLAG_SHIFT (DND_BUFF_SOURCE_BITS + DND_BUFF_FLAG_BITS)

#define DND_MAX_PLAYER_BUFFS 64
struct buffData_T {
	int head;										// holds first buff index
	buff_T buff_list[DND_MAX_PLAYER_BUFFS];			// unsorted buff list

	int buff_count;									// how many buffs in list total at the moment
	bool state;										// needs to be true to be good to use (fixes runaway script problems in weapons)

	ValueComponent_T buff_net_values[BUFF_TYPES_MAX];
};

buffData_T module& GetPlayerBuffData(int pnum) {
	static buffData_T buffs[MAXPLAYERS];
	return buffs[pnum];
}

void ResetPlayerBuffs(int pnum) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);
	int i;

	pbuffs.buff_count = 0;

	// dummy node to make logic easier
	pbuffs.head = 0;

	for(i = 0; i < BUFF_TYPES_MAX; ++i) {
		pbuffs.buff_net_values[i].additive = 0;
		pbuffs.buff_net_values[i].multiplicative = 1.0;
	}

	for(i = 0; i < DND_MAX_PLAYER_BUFFS; ++i) {
		pbuffs.buff_list[i].source = 0;
		pbuffs.buff_list[i].next_id = -1;
	}

	pbuffs.state = true;

	//Log(s:"reset buffs, head next: ", d:pbuffs.buff_list[pbuffs.head].next_id);
	if(GetGameModeState() != GAMESTATE_COUNTDOWN)
		ACS_NamedExecuteWithResult("DnD Buff Ticker", pnum);

	i = pnum + P_TIDSTART;
	TakeActorInventory(i, "DnD_HasPhasing", 1);
	TakeActorInventory(i, "DnD_HasAmphetamine", 1);
}

bool IsPlayerBuffStateOK(int pnum) {
	return GetPlayerBuffData(pnum).state;
}

void HandleBuffValueComponent(int pnum, int buff_index, bool remove = false) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);
	buff_T module& toHandle = pbuffs.buff_list[buff_index];

	int type = toHandle.type;
	int val = 1.0;

	if(toHandle.flags & BUFF_F_MORETYPE) {
		// go through all buffs for final result, as mathematically this could be irreversible (result was 0 on "less" multipliers)
		int i = pbuffs.buff_list[pbuffs.head].next_id;
		while(i != -1) {
			if(pbuffs.buff_list[i].type == type && (pbuffs.buff_list[i].flags & BUFF_F_MORETYPE) && (!remove || i != buff_index)) {
				val = CombineFactors(val, pbuffs.buff_list[i].value);
				//Log(s:"factor ", f:pbuffs.buff_list[i].value);
			}
			i = pbuffs.buff_list[i].next_id;
		}

		pbuffs.buff_net_values[type].multiplicative = val;

		//Log(s:"multiplicative value: ", f:pbuffs.buff_net_values[type].multiplicative);
	}
	else {
		val = toHandle.value;
		if(remove)
			val = -val;
		pbuffs.buff_net_values[type].additive += val;
		//Log(s:"additive value: ", f:pbuffs.buff_net_values[type].additive);
	}

	if(!pbuffs.buff_net_values[type].additive && pbuffs.buff_net_values[type].multiplicative == 1.0) {
		val = pnum + P_TIDSTART;
		switch(type) {
			case BUFF_PHASING:
				TakeActorInventory(val, "DnD_HasPhasing", 1);
			break;
			case BUFF_SPEED:
				if(toHandle.bt_index == BTI_AMPHETAMINE)
					TakeActorInventory(val, "DnD_HasAmphetamine", 1);
			break;
		}
	}
}

void HandleBuffApplication(int pnum, int buff_type) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);
	
	int ptid = P_TIDSTART + pnum;
	int base = 0;

	switch(buff_type) {
		case BUFF_SPEED:
			// checks regarding having immunity to slowdowns can be done here
			if(CheckActorInventory(ptid, "GryphonCheck"))
				return;

		case BUFF_STUN:

			// multiplicative here implies "less" movement speed, therefore if the positive value for example was 20% less speed, which would be 0.2 in the code, we'd really want 1.0 - 0.2 as the factor here
			// which is 80% of your normal speed
			base = GetPlayerSpeed(pnum) + pbuffs.buff_net_values[BUFF_SPEED].additive;
			if(pbuffs.buff_net_values[BUFF_SPEED].multiplicative != 1.0) // default is 1.0 if theres none so nothing to worry about here
				base = FixedMul(base, pbuffs.buff_net_values[BUFF_SPEED].multiplicative);

			// ie. not stunned, go ahead
			if(pbuffs.buff_net_values[BUFF_STUN].multiplicative == 1.0) {
				if(base < DND_LOWEST_PLAYERSPEED)
					base = DND_LOWEST_PLAYERSPEED;
			}
			else // since we have stun related things and this'll set movement to 0, might as well include it here -- plus fixes cases where player receives a slow while stunned still letting them move
				base = FixedMul(base, pbuffs.buff_net_values[BUFF_STUN].multiplicative);

			SetActorProperty(P_TIDSTART + pnum, APROP_SPEED, base);
			//Log(s:"New speed factor: ", f:base);
		break;
	}
}

int SetDuration(int bduration, bool tick_adjust) {
	if(tick_adjust)
		return bduration / DND_BUFF_TICKTIME;
	return DND_BUFF_TICK_SECOND_ADJUST(bduration);
}

// accepts in seconds for duration parameter
void GivePlayerBuff(int pnum, int bsource, int btype, int bbt_index, int bvalue, int bflags, int bduration = 0, int breplacement = 0) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);

	int i = pbuffs.head;
	int j = -1;
	int found = 0;
	// different search methodology if we have strictness on duplications
	if(!(bflags & BUFF_F_NODUPLICATE_STRICT)) {
		while(i != -1 && !found) {
			// if same source and same type
			if
			(
				pbuffs.buff_list[i].type == btype &&
				(pbuffs.buff_list[i].flags & BUFF_SOURCE_FLAGMASK) == (bflags & BUFF_SOURCE_FLAGMASK) &&
				pbuffs.buff_list[i].value == bvalue &&
				(
					// no duplicate case checks for duplicate instances from same tid
					// unique class checks for cases where tid may not be the same, but source actor name can be
					((pbuffs.buff_list[i].flags & BUFF_F_NODUPLICATE) && (bflags & BUFF_F_NODUPLICATE) && pbuffs.buff_list[i].source == bsource) ||
					((pbuffs.buff_list[i].flags & BUFF_F_UNIQUETOCLASS) && (bflags & BUFF_F_UNIQUETOCLASS) && GetActorClass(pbuffs.buff_list[i].source) == GetActorClass(bsource))
				)
			)
			{
				// if we don't care for duplicate, then don't even consider it
				//Log(s:"found duplicate");
				
				if(bflags & BUFF_F_REPLACEMENTVALUE) {
					// if this is a thing, make sure we refactor the buff immediately
					pbuffs.buff_list[i].value = breplacement;
					found = 2;
					break;
				}

				// refresh timer if any
				if(pbuffs.buff_list[i].duration < bduration)
					pbuffs.buff_list[i].duration = bduration;

				found = 1;
				return;
			}

			// if there is a gap, stop here, this guy's +1 is the gap we can immediately insert something to -- save this position into j once, and keep checking for duplicates just in case
			//Log(s:"next id check? ", d:pbuffs.buff_list[i].next_id, s: " " , d:i+1);
			if(j == -1 && pbuffs.buff_list[i].next_id != i + 1)
				j = i;
			i = pbuffs.buff_list[i].next_id;
		}
	}
	else {
		while(i != -1 && !found) {
			// if type is same and also strict, we will compare immediately
			// check for buff table index to determine uniqueness, as there can be multiple slows but of different sources of uniqueness (auras etc.)
			if(pbuffs.buff_list[i].type == btype && pbuffs.buff_list[i].bt_index == bbt_index && (pbuffs.buff_list[i].flags & BUFF_F_NODUPLICATE_STRICT)) {
				if(bflags & BUFF_F_ADDIFNODUPLICATE) {
					// increase its value
					pbuffs.buff_list[i].value += bvalue;
					pbuffs.buff_list[i].duration = bduration;
					//Log(s:"found duplicate, adding value to index: ", d:i, s:" value: ", f:bvalue, s:" ", d:pbuffs.buff_list[i].duration);
					found = 2;
					break;
				}
				else if(pbuffs.buff_list[i].value < bvalue) {
					pbuffs.buff_list[i].value = bvalue;
					pbuffs.buff_list[i].duration = bduration;

					// transfer flags and source if we are strict, this is the stronger
					pbuffs.buff_list[i].flags = bflags;
					pbuffs.buff_list[i].source = bsource;
					found = 2;
					break;
				}
				else if(pbuffs.buff_list[i].value == bvalue && pbuffs.buff_list[i].duration < bduration) {
					pbuffs.buff_list[i].duration = bduration;
					// transfer flags aand source if we are strict, this is the stronger
					pbuffs.buff_list[i].flags = bflags;
					pbuffs.buff_list[i].source = bsource;
				}
				found = 1;
				return;
			}

			// if there is a gap, stop here, this guy's +1 is the gap we can immediately insert something to -- save this position into j once, and keep checking for duplicates just in case
			//Log(s:"next id check? ", d:pbuffs.buff_list[i].next_id, s: " " , d:i+1);
			if(j == -1 && pbuffs.buff_list[i].next_id != i + 1)
				j = i;
			i = pbuffs.buff_list[i].next_id;
		}
	}

	// update the valid slot using j
	if(j != -1)
		i = j;

	// insert to the array now since we couldn't find a duplicate
	if(!found && i + 1 < DND_MAX_PLAYER_BUFFS) {
		// i stops at the node that has next == -1, so i+1 is the place to add unless we reached max somehow already
		// store the previous next of this guy
		j = pbuffs.buff_list[i].next_id;
		pbuffs.buff_list[i].next_id = i + 1;
		//Log(s:"Prev node ", d:i, s:" and its next is now ", d:i + 1);

		// current node is now the new node that needs to be set with proper data
		i = i + 1;
		pbuffs.buff_list[i].source = bsource;

		// if there was a replacement but it wasn't found, add it as this buff
		if(bflags & BUFF_F_REPLACEMENTVALUE) {
			// if this is a thing, make sure we refactor the buff immediately
			pbuffs.buff_list[i].value = breplacement;
		}
		else
			pbuffs.buff_list[i].value = bvalue;

		pbuffs.buff_list[i].flags = bflags;
		pbuffs.buff_list[i].type = btype;
		pbuffs.buff_list[i].bt_index = bbt_index;

		// dont adjust if this is meant to be in actual tics
		pbuffs.buff_list[i].duration = bduration;

		pbuffs.buff_list[i].next_id = j; // new next is the one that was next of previous
		//Log(s:"New node ", d:i, s:" and its next is now ", d:j);

		++pbuffs.buff_count;

		found = 2;
	}
	
	// handle the buff's effect as its a new buff or an update on it
	if(found > 1) {
		HandleBuffValueComponent(pnum, i);
		HandleBuffApplication(pnum, btype);
	}
}

// Non-ticker buffs removal lies on the user
Script "DnD Buff Ticker" (int pnum) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);

	while(IsAlive() && !CheckInventory("DnD_IntermissionState")) {
		int i = pbuffs.buff_list[pbuffs.head].next_id;
		while(i != -1) {
			int rem = -5;
			//Log(s:"duration: ", d:pbuffs.buff_list[i].duration);
			if((pbuffs.buff_list[i].flags & BUFF_F_TICKERREQUIRED)) {
				--pbuffs.buff_list[i].duration;
				//Log(s:"Dec ", d:i, s:" duration: ", d:pbuffs.buff_list[i].duration);
				// either timed out or conditional removal in the form of thawing for chill & freeze on player
				if
				(
					pbuffs.buff_list[i].duration <= 0 ||
					((pbuffs.buff_list[i].bt_index == BTI_CHILL || pbuffs.buff_list[i].bt_index == BTI_FREEZE) && CheckInventory("DnD_ThawPlayer"))
				)
				{
					rem = RemoveBuff(pnum, i - 1); // assumes +1 so this corrects it
					//Log(s:"removed ", d:i, s: " next: ", d:rem);
				}
			}

			if(rem == -5)
				i = pbuffs.buff_list[i].next_id;
			else
				i = rem;
			//Log(s:"tick next to ", d:i);
		}

		// buffs count down every 7 tics
		//Log(s:"ticker tick");
		Delay(const:DND_BUFF_TICKTIME);
	}
	RemoveAllBuffs(pnum);

	SetResultValue(0);
}

void RemoveAllBuffs(int pnum) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);
	int i = pbuffs.buff_list[pbuffs.head].next_id;
	while(i != -1) {
		int rem = -5;
		rem = RemoveBuff(pnum, i - 1); // assumes +1 so this corrects it

		if(rem == -5)
			i = pbuffs.buff_list[i].next_id;
		else
			i = rem;
	}
}

// send 0 indexed, it'll add one since we have dummy node
// returns the next node of the removed node
int RemoveBuff(int pnum, int buff_index, int precalc_prev = -1) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);

	++buff_index;
	if(buff_index >= DND_MAX_PLAYER_BUFFS)
		buff_index = DND_MAX_PLAYER_BUFFS - 1;

	// reverse scan to find whose next we were --- if nobody's that means we were first element/only element
	int prev = buff_index - 1;
	bool validPrev = false;
	if(precalc_prev == -1) {
		while(prev >= 0 && !validPrev) {
			if(pbuffs.buff_list[prev].next_id == buff_index)
				validPrev = true;
			else
				--prev;
		}
	}
	else {
		validPrev = true;
		prev = precalc_prev;
	}

	if(validPrev) {
		pbuffs.buff_list[prev].next_id = pbuffs.buff_list[buff_index].next_id;
		//Log(s:"setting prev's which is ", d:prev, s:" next to ", d:pbuffs.buff_list[buff_index].next_id);
		prev = pbuffs.buff_list[buff_index].next_id;
	}
	else
		prev = -1;

	//Log(s:"buff value to remove ", d:pbuffs.buff_list[buff_index].value);

	HandleBuffValueComponent(pnum, buff_index, true);
	HandleBuffApplication(pnum, pbuffs.buff_list[buff_index].type);
	HandleSpecialBuffRemoval(pnum, pbuffs.buff_list[buff_index].bt_index);

	pbuffs.buff_list[buff_index].source = 0;
	pbuffs.buff_list[buff_index].type = -1;
	pbuffs.buff_list[buff_index].bt_index = -1;
	pbuffs.buff_list[buff_index].flags = 0;
	pbuffs.buff_list[buff_index].value = 0;
	pbuffs.buff_list[buff_index].duration = 0;
	pbuffs.buff_list[buff_index].next_id = -1;

	//Log(s:"Removed buff index ", d:buff_index, s:" returning ", d:prev);

	--pbuffs.buff_count;

	// got nothing left, so make head's next -1
	if(!pbuffs.buff_count)
		pbuffs.buff_list[pbuffs.head].next_id = -1;

	return prev;
}

void RemoveBuffMatching(int pnum, int bsource, int btype, int bbt_index, int bvalue, int bflags) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);
	int i = pbuffs.buff_list[pbuffs.head].next_id;
	int prev = pbuffs.head;
	//Log(s:"remove buff start ", d:i);
	while(i != -1) {
		//Log(s:"comp val ", f:pbuffs.buff_list[i].value, s: " ", f:bvalue);
		if
		(
			pbuffs.buff_list[i].source == bsource &&
			pbuffs.buff_list[i].type == btype &&
			pbuffs.buff_list[i].bt_index == bbt_index &&
			pbuffs.buff_list[i].value == bvalue &&
			pbuffs.buff_list[i].flags == bflags
		)
		{
			// matched, remove -- map to 0 indexed here
			RemoveBuff(pnum, i - 1, prev);
			return;
		}
		prev = i;
		i = pbuffs.buff_list[i].next_id;
	}
}

void HandleSpecialBuffRemoval(int pnum, int buff_table_index) {
	int ptid = pnum + P_TIDSTART;
	if(buff_table_index == BTI_FRENZYCHARGE) {
		SetActorInventory(ptid, "DnD_FrenzyChargeCount", 0);

		// delete frenzy charge actors
		GiveActorInventory(ptid, "DnD_FrenzyChargeRemoveSignal", 1);
		SetActorProperty(ptid, APROP_SPEED, GetPlayerSpeed(pnum));
	}
	else if(buff_table_index == BTI_ENDURANCECHARGE) {
		SetActorInventory(ptid, "DnD_EnduranceChargeCount", 0);
		GiveActorInventory(ptid, "DnD_EnduranceChargeRemoveSignal", 1);
	}
	else if(buff_table_index == BTI_POWERCHARGE) {
		SetActorInventory(ptid, "DnD_PowerChargeCount", 0);
		GiveActorInventory(ptid, "DnD_PowerChargeRemoveSignal", 1);
	}
}

// checks by buff table index, mostly should be used for strict no duplication cases
bool HasPlayerBuff(int pnum, int bbt_index) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);
	int i = pbuffs.buff_list[pbuffs.head].next_id;
	
	while(i != -1) {
		if(pbuffs.buff_list[i].bt_index == bbt_index)
			return true;
		i = pbuffs.buff_list[i].next_id;
	}

	return false;
}

void SlowPlayer(int amt, int mode, int pnum) {
	if(!pnum)
		pnum = PlayerNumber();
	// don't slow if player has gryphon boots
	if(CheckActorInventory(P_TIDSTART + pnum, "GryphonCheck") && !(mode & SF_FREEZE))
		return;
	if(mode & SF_FREEZE) {
		SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
		SetActorProperty(0, APROP_SPEED, 0.0);
		GiveInventory("P_Frozen", 1);
	}
	else
		SetActorProperty(P_TIDSTART + pnum, APROP_SPEED, FixedMul(GetPlayerSpeed(pnum), amt));
}

void LogBuffList(int pnum) {
	buffData_T module& pbuffs = GetPlayerBuffData(pnum);
	
	int i =  pbuffs.buff_list[pbuffs.head].next_id;
	int count = 0;
	while(i != -1 && count < 8) {
		Log(s:"Buff ", d:count + 1, s: "(", d:i, s:") -- Source: ", d:pbuffs.buff_list[i].source, s: " Type: ", d:pbuffs.buff_list[i].type, s:" Flags: ", d:pbuffs.buff_list[i].flags, s: " Value: ", d:pbuffs.buff_list[i].value, s:" Duration: ", d:pbuffs.buff_list[i].duration);
		++count;
		i = pbuffs.buff_list[i].next_id;
	}
}

#include "DnD_BuffTable.h"

#endif