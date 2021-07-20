#ifndef DND_STATISTICS_IN
#define DND_STATISTICS_IN

#define STAT_THOUSAND_BREAKPOINT 1000000
#define STAT_NEXTORDER_BREAKPOINT 1000

enum {
	DND_STATISTIC_DAMAGEDEALT,
	DND_STATISTIC_DAMAGETAKEN,
};

// returns the statistic in a properly formatted way respecting millions count
str GetStatistic(int stat, int tid = 0) {
	if(!tid)
		tid = ActivatorTID();

	// a couple guarantees
	// we keep mil < 1000 because 1000 mil is just 1 bil
	// bil has no limit but its unlikely people would reach more than 999
	// val can be anything less than 999,999 which will be filtered to show using a K
	int val = 0;
	int mil = 0;
	int bil = 0;
	int temp;
	
	switch(stat) {
		case DND_STATISTIC_DAMAGEDEALT:
			val = CheckActorInventory(tid, "Damage_Dealt");
			mil = CheckActorInventory(tid, "Damage_Dealt_Millions");
			bil = CheckActorInventory(tid, "Damage_Dealt_Billions");
		break;
		case DND_STATISTIC_DAMAGETAKEN:
			val = CheckActorInventory(tid, "Damage_Taken");
			mil = CheckActorInventory(tid, "Damage_Taken_Millions");
			bil = CheckActorInventory(tid, "Damage_Taken_Billions");
		break;
	}
	
	// integer part is always the greater value, fractional part is the lesser
	// 3 digits . 2 digits
	
	if(bil) {
		mil /= 10;
		if(mil < 10)
			return StrParam(d:bil, s:".0", d:mil, s:"B");
		return StrParam(d:bil, s:".", d:mil, s:"B");
	}
	else if(mil) {
		val /= 10000;
		if(val < 10)
			return StrParam(d:mil, s:".0", d:val, s:"M");
		return StrParam(d:mil, s:".", d:val, s:"M");
	}
	else if(val > 1000) {
		temp = (val / 10) % 100;
		if(temp < 10)
			return StrParam(d:val / 1000, s:".0", d:temp, s:"K");
		return StrParam(d:val / 1000, s:".", d:temp, s:"K");
	}
	
	return StrParam(d:val);
}

void IncrementStatistic(int stat, int amt, int tid = 0) {
	// a couple guarantees
	// we keep mil < 1000 because 1000 mil is just 1 bil
	// bil has no limit but its unlikely people would reach more than 999
	// val can be anything less than 999,999 which will be filtered to show using a K
	
	if(!tid)
		tid = ActivatorTID();
	
	int temp, val;
	bool use_KMB = false;
	str prefix = "";
	
	switch(stat) {
		case DND_STATISTIC_DAMAGEDEALT:
			use_KMB = true;
			prefix = "Damage_Dealt";
		break;
		case DND_STATISTIC_DAMAGETAKEN:
			use_KMB = true;
			prefix = "Damage_Taken";
		break;
	}
	
	if(use_KMB) {
		temp = CheckActorInventory(tid, prefix);
		temp += amt;
		if(temp < STAT_THOUSAND_BREAKPOINT)
			SetActorInventory(tid, prefix, temp);
		else {
			val = temp / STAT_THOUSAND_BREAKPOINT;
			temp -= STAT_THOUSAND_BREAKPOINT * val;
			SetActorInventory(tid, prefix, temp);
			
			str milstr = StrParam(s:prefix, s:"_Millions");
			
			temp = CheckActorInventory(tid, milstr);
			temp += val;
			
			if(temp < STAT_NEXTORDER_BREAKPOINT)
				SetActorInventory(tid, milstr, temp);
			else {
				val = temp / STAT_NEXTORDER_BREAKPOINT;
				temp -= STAT_NEXTORDER_BREAKPOINT * val;
				SetActorInventory(tid, milstr, temp);
				
				milstr = StrParam(s:prefix, s:"_Billions");
				
				temp = CheckActorInventory(tid, milstr);
				temp += val;
				
				SetActorInventory(tid, milstr, temp);
			}
		}
	}
}

void ResetStatistics() {
	SetInventory("Damage_Dealt", 0);
	SetInventory("Damage_Dealt_Millions", 0);
	SetInventory("Damage_Dealt_Billions", 0);
	SetInventory("Damage_Taken", 0);
	SetInventory("Damage_Taken_Millions", 0);
	SetInventory("Damage_Taken_Billions", 0);
}

#endif
