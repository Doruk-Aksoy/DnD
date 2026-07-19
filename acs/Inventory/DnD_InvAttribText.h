#ifndef DND_INVATTRIBTEXT_IN
#define DND_INVATTRIBTEXT_IN

str GetDetailedModRange(int attr, int item_type, int item_subtype, int tier, int trunc_factor = 0, int extra = -1, bool isPercentage = false) {
	if(extra != -1)
		return GetDetailedModRange_Unique(tier, trunc_factor, extra, isPercentage);
	
	// limit this to here at t10...
	str col_tag = GetCharmString(Clamp_Between(tier, 0, 9), CHARMSTR_COLORCODE);
	int tier_mapping = GetModTierRangeMapper(attr, tier);

	// visually change the attribute values depending on item scale factors
	int f = GetItemAttributeFactor(item_type, item_subtype);
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, f),
			s:"\c--",
			s:col_tag, d:GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_LOW, f), isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTier(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), isPercentage), s:"\c-)"
	);
}

str GetDetailedModRangeExtra(int attr, int item_type, int item_subtype, int tier, int trunc_factor = 0, bool isPercentage = false) {
	// limit this to here at t10...
	str col_tag = GetCharmString(Clamp_Between(tier, 0, 9), CHARMSTR_COLORCODE);
	int tier_mapping = GetModTierRangeMapperExtra(attr, tier);

	// visually change the attribute values depending on item scale factors
	int f = GetItemAttributeFactor(item_type, item_subtype);
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, f),
			s:"\c--",
			s:col_tag, d:GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_LOW, f), isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(GetModRangeWithTierExtra(attr, tier_mapping, ITEM_MODRANGE_HIGH, f), isPercentage), s:"\c-)"
	);
}

str GetDetailedImplicitModRange(int attr, int item_type, int item_subtype, int trunc_factor = 0, bool isPercentage = false) {
	str col_tag = "\cg";
	
	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:col_tag, d:ItemModTable[attr].attrib_low,
			s:"\c--",
			s:col_tag, d:ItemModTable[attr].attrib_high, s:"\c-)"
		);
	}
	return StrParam(
		s:"\c-(",
		s:col_tag, s:GetFixedRepresentation(ItemModTable[attr].attrib_low, isPercentage),
		s:"\c--",
		s:col_tag, s:GetFixedRepresentation(ItemModTable[attr].attrib_high, isPercentage), s:"\c-)"
	);
}

str GetDetailedModRange_Unique(int unique_id, int trunc_factor = 0, int unique_roll_id = 0, bool isPercentage = false, bool isExtra = false) {
	int low = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_low;
	int high = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_high;

	if(isExtra) {
		low = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_extra_low;
		high = UniqueItemList[unique_id].rolls[unique_roll_id].attrib_extra_high;
	}

	if(!trunc_factor) {
		return StrParam(
			s:"\c-(",
			s:"\c[D1]", d:low,
			s:"\c--",
			s:"\c[D1]", d:high, s:"\c-)"
		);
	}
	
	if(unique_id != UITEM_WELLOFPOWER) {
		return StrParam(
			s:"\c-(",
			s:"\c[D1]", s:GetFixedRepresentation(low, isPercentage),
			s:"\c--",
			s:"\c[D1]", s:GetFixedRepresentation(high, isPercentage), s:"\c-)"
		);
	}

	// this item is a little odd, so we need to treat it as such
	// since ACS can't round floats to shit (bad representation, w.e) we need to do a custom one for weird numbers like this

	return StrParam(
		s:"\c-(",
		s:"\c[D1]", d:low / 1000, s:".", d:(low / 100) % 10, d:(low / 10) % 10,
		s:"\c--",
		s:"\c[D1]", d:high / 1000, s:".", d:(high / 100) % 10, d:(high / 10) % 10, s:"\c-)"
	);
}

str GetModTierText(int tier, int extra) {
	if(extra != -1)
		return StrParam(s:"\c[D1]", s:"U");
	return StrParam(s:GetCharmString(Clamp_Between(tier, 0, 9), CHARMSTR_COLORCODE), s:"T", d:tier);
}

str GetInventoryAttributeText(int attr) {
	if(attr <= LAST_INV_ATTRIBUTE)
		return StrParam(s:"IATTR_T", d:attr);

	if(attr <= LAST_CORRUPT_IMPLICIT)
		return StrParam(s:"IATTR_TC", d:attr + 1 - FIRST_CORRUPT_IMPLICIT);

	if(attr <= LAST_REGULAR_IMPLICIT)
		return StrParam(s:"IATTR_TI", d:attr + 1 - FIRST_REGULAR_IMPLICIT);
		
	// essences are mapped to 1 again for language file
	if(attr <= LAST_ESSENCE_ATTRIBUTE)
		return StrParam(s:"IATTR_TE", d:attr + 1 - FIRST_ESSENCE_ATTRIBUTE);

	if(attr <= LAST_INCURSION_ATTRIBUTE)
		return StrParam(s:"IATTR_TINC", d:attr + 1 - FIRST_INCURSION_ATTRIBUTE);

	if(attr <= LAST_FLASK_IMPICIT)
		return StrParam(s:"IATTR_IMP_FLASK", d:attr - FIRST_FLASK_IMPLICIT);

	if(attr <= LAST_FLASK_ATTRIBUTE)
		return StrParam(s:"IATTR_FLASK", d:attr - FIRST_FLASK_ATTRIBUTE);
		
	// only option left is unique exotic attributes
	return StrParam(s:"IATTR_TX", d:UNIQUE_MAP_MACRO(attr));
}

str ItemAttributeString(
	int attr, int item_type, int item_subtype, 
	int val, int tier = 0, bool showDetailedMods = false, 
	int extra = -1, bool isFractured = false, int qual = 0, 
	int attr_extra = 0, int craftAffected = 0
)
{
	str text = GetInventoryAttributeText(attr);
	str ess_tag = "\c[Q7]";
	str col_tag = "\c[Q9]";
	str no_tag = "\c- ";

	if(!(craftAffected & 0xFF)) {
		if(isFractured) {
			col_tag = "\c[E2]";
			ess_tag = "\c[E2]";
			no_tag = "\c[E2] ";
		}
		else if(attr >= FIRST_INCURSION_ATTRIBUTE && attr <= LAST_INCURSION_ATTRIBUTE) {
			ess_tag = "\c[Q0]";
			col_tag = "\c[Q9]";
			no_tag = "\c[Q0] ";
		}
	}
	else {
		if((craftAffected >> 8) != DND_ORBEFFECT_NUMBER) {
			ess_tag = "\ck";
			no_tag = "\ck ";
		}
		col_tag = "\ck";
	}

	// dont draw essence mods as special if they are on unique items
	if(extra != -1)
		ess_tag = "";

	if(qual) {
		if(!IsAttributeQualityException(attr)) {
			if(val < 100000) {
				val *= qual + 100;
				val /= 100;
			}
			else {
				val /= 100;
				val *= qual + 100;
			}
		}

		if(attr_extra && !IsAttributeExtraException(attr)) {
			if(attr_extra > 100000) {
				attr_extra /= 100;
				attr_extra *= qual + 100;
			}
			else {
				attr_extra *= qual + 100;
				attr_extra /= 100;
			}
		}
	}
	
	switch(attr) {
		case INV_CORR_CYBERNETIC:
			if(!isFractured)
				col_tag = "\c[R5]";
			return StrParam(s:col_tag, l:text);

		case INV_CYBERNETIC:
			if(!isFractured)
				col_tag = "\c[R5]";
			if(showDetailedMods) {
				return StrParam(
					s:col_tag, l:text, s:no_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:col_tag, l:text);
			
		// essences with specific writing
		case INV_ESS_SSRATH:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:"IATTR_MAGICRES",
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"% ", s:ess_tag, l:"IATTR_MAGICRES");
			
		// text + val%
		case INV_ESS_CHEGOVAX:
		case INV_ESS_ZRAVOG:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%",
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"%");

		// essence with no numeric values
		case INV_ESS_LESHRAC:
		case INV_ESS_THORAX:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text);
			
		// essences with percentages in them
		case INV_ESS_OMNISIGHT2:
			// this ones used in a unique so
			if(extra == -1) {
				if(showDetailedMods) {
					return StrParam(
						s:ess_tag, s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:ess_tag, l:text,
						s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:ess_tag, s:"+", s:col_tag, d:val, s:"%", s:ess_tag, l:text);
			}
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, d:val, s:"\c-", s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:ess_tag, s:"%", l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+", s:col_tag, d:val, s:"%", s:"\c-", l:text);

		case INV_ESS_HARKIMONDE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:ess_tag, l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, s:"+", s:col_tag, d:val, s:"%", s:ess_tag, l:text);

		case INV_ESS_KRULL:
			if(showDetailedMods) {
				if(extra == -1) {
					return StrParam(
						s:ess_tag, s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:ess_tag, l:text,
						s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(
					s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%\c-", l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			if(extra == -1)
				return StrParam(s:ess_tag, s:"+", s:col_tag, d:val, s:"%", s:ess_tag, l:text);
			return StrParam(s:"+", s:col_tag, d:val, s:"%\c-", l:text);
			
		// essences that are like regular mods, just have color code
		case INV_ESS_OMNISIGHT:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:ess_tag, l:text,
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, s:"+", s:col_tag, d:val, s:ess_tag, l:text);

		case INV_ESS_VAAJ:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:text,
					s:ess_tag, d:attr_extra, s:GetDetailedModRangeExtra(attr, item_type, item_subtype, tier, 0), s:col_tag, s:"% ", s:ess_tag, l:"IATTR_TE1S",
					s:no_tag, s:ess_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, s:"+", s:col_tag, d:val, s:"% ", s:ess_tag, l:text, s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TE1S");
		
		// since percentages are handled in default case, we will handle all flat value attributes under here
		case INV_HP_INCREASE:
		case INV_ARMOR_INCREASE:
		case INV_SHIELD_INCREASE:

		case INV_FLATPHYS_DAMAGE:
		case INV_FLATENERGY_DAMAGE:
		case INV_FLATMAGIC_DAMAGE:
		case INV_FLATELEM_DAMAGE:
		case INV_FLATRADIUS_DAMAGE:

		case INV_FLAT_ARTILLERY:
		case INV_FLAT_AUTOMATIC:
		case INV_FLAT_HANDGUN:
		case INV_FLAT_SHOTGUN:
		case INV_FLAT_PRECISION:
		case INV_FLAT_TECH:
		case INV_FLAT_MAGIC:

		case INV_PEN_ELEMENTAL:
		case INV_PEN_ENERGY:
		case INV_PEN_PHYSICAL:
		case INV_PEN_OCCULT:
		case INV_PEN_FIRE:
		case INV_PEN_ICE:
		case INV_PEN_LIGHTNING:
		case INV_PEN_POISON:

		case INV_KNOCKBACK_RESIST:
		case INV_ACCURACY_INCREASE:
		case INV_STAT_STRENGTH:
		case INV_STAT_DEXTERITY:
		case INV_STAT_INTELLECT:
		case INV_FLAT_FIREDMG:
		case INV_FLAT_ICEDMG:
		case INV_FLAT_LIGHTNINGDMG:
		case INV_FLAT_POISONDMG:
		case INV_OVERLOAD_ZAPCOUNT:
		case INV_IGNITE_PROLIFCOUNT:

		case INV_INC_MAXPOISONSTACK:
			if(val > 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:no_tag, l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+", s:col_tag, d:val, s:no_tag, l:text);
			}
			else if(val < 0)
				return StrParam(s:col_tag, s:"-\cg", d:val, s:no_tag, l:text);
				
		// corrupted implicits of certain kinds have +X% text TO (extra)
		case INV_CORR_WEAPONCRITDMG:
		case INV_CORR_WEAPONPOISONPCT:
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, d:val, s:GetDetailedImplicitModRange(attr, item_type, item_subtype, 0), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra)
				);
			}
			return StrParam(s:"+", s:col_tag, d:val, s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra));

		// flat text with weapon mention
		case INV_CORR_WEPCULL:
			return StrParam(s:col_tag, l:GetWeaponTag(extra), s: " ", l:"DND_HAS", s:" ", l:text);

		// this doesn't have detailed mod explanation on purpose, each item has their own presets
		case INV_IMP_INCARMOR:
		case INV_IMP_INCSHIELD:
		case INV_IMP_INCARMORSHIELD:
			text = StrParam(s:"+", s:col_tag, d:val, s:no_tag, l:text);
		return text;
		case INV_IMP_INCMIT:
			text = StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, false), s:"%", s:no_tag, l:text);
		return text;
		case INV_IMP_INCMITARMOR:
			text = StrParam(s:"+", s:col_tag, d:val, s:no_tag, l:text, s: " ", s:col_tag, s:GetFixedRepresentation((val << 16) / DND_ARMOR_TO_MIT_RATIO, false), s:"%", s:no_tag, l:"IATTR_TI4");
		return text;
		case INV_IMP_INCMITSHIELD:
			text = StrParam(s:"+", s:col_tag, d:val, s:no_tag, l:text, s: " ", s:col_tag, s:GetFixedRepresentation((val << 16) / DND_SHIELD_TO_MIT_RATIO, false), s:"%", s:no_tag, l:"IATTR_TI4");
		return text;
		case INV_IMP_POWERCORE:
			text = StrParam(s:"+", s:col_tag, d:extra, s:"%", s:no_tag, l:text, s: "\n", s:"+", s:col_tag, d:val, s:no_tag, l:"IATTR_T98", s:"\n\c[R5]", l:"IATTR_T72");
		return text;
		case INV_IMP_UNSTABLECORE:
			text = StrParam(
				s:"+", s:col_tag, d:val, s:no_tag, l:"IATTR_T98", s:"\n\cd",
				s:GetFixedRepresentation(extra, true), s:"%\c-", s:no_tag, l:text, s:" \cd", s:GetFixedRepresentation(extra * UNSTABLE_DMG_MULT, true), s:"%\c- ", l:"IATTR_TI8S", s:"\n\c[R5]",
				l:"IATTR_T72"
			);
		return text;
		case INV_IMP_INCMITARMORSHIELD:
			text = StrParam(s:"+", s:col_tag, d:val, s:no_tag, l:text, s: " +", s:col_tag, s:GetFixedRepresentation((val << 16) / DND_ARMOR_TO_MIT_RATIO, false), s:"%", s:no_tag, l:"IATTR_TI4");
		return text;

		case INV_IMP_RAVAGER:					// ravager armor
			text = StrParam(s:col_tag, d:val, s:"%\c-", l:text, s:col_tag, d:extra, s:"%\c- ", l:"IATTR_TI10S");
		return text;

		// just the wording
		case INV_IMP_DOUBLEESHIELDRECOVERY:
		case INV_IMP_CANROLL_PHYS:
		case INV_IMP_CANROLL_MAGIC:
		case INV_IMP_CANROLL_EXPLOSIVE:
		case INV_IMP_CANROLL_ENERGY:
		case INV_IMP_CANROLL_ELEMENTAL:
		case INV_IMP_HIGHREFLECTREDUCE:			// synth metal
		case INV_IMP_ESHIELDBLOCKSALL:
		case INV_IMP_BONUSPETCAP:
		case INV_IMP_MELEEIGNORESSHIELDS:
		case INV_IMP_RECOVERESONUNDEADKILL:
		case INV_IMP_QUALITYCAPFIFTY:
		case INV_CORR_DMGDOESNTSTOPREGEN:
		return StrParam(l:text);

		case INV_IMP_ABSORBLIGHTNING:			// lightning coil
			text = StrParam(s:col_tag, d:val, s:"%\c-", l:text, s:col_tag, d:extra, s:"%\c- ", l:"IATTR_TI12S");
		return text;
		case INV_IMP_KNIGHTARMOR:
			text = StrParam(s:col_tag, d:val, s:"%\c-", l:text, s:col_tag, d:extra, s:"%\c- ", l:"IATTR_TI13S");
		return text;
		case INV_IMP_HANDGUNBONUS:
			text = StrParam(l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:"%\c-", l:"IATTR_TI14S");
		return text;

		// value and text only as percentage
		case INV_IMP_NECROARMOR:
		case INV_IMP_LESSSELFDAMAGETAKEN:
		case INV_IMP_REDUCEDVISIONIMPAIR:		// synthmetal mask
		case INV_IMP_PERCENTSTR:
		case INV_IMP_PERCENTDEX:
		case INV_IMP_PERCENTINT:
		case INV_CORR_INSTALEECHPCT:
			text = StrParam(s:col_tag, d:val, s:"%\c- ", l:text);
		return text;

		case INV_IMP_FASTEROVERHEATDISS:
			if(extra == -1)
				text = StrParam(s:col_tag, d:val, s:"%\c- ", l:text);
			else {
				if(val < 0) {
					col_tag = "\cg";
					no_tag = "\c[D4]";
				}
				else
					no_tag = "\c-";

				if(showDetailedMods)
					text = StrParam(s:col_tag, d:val, s:GetDetailedModRange_Unique(tier, 0, extra), s:"% ", s:no_tag, l:text, s:" - ", s:GetModTierText(tier, extra));
				else
					text = StrParam(s:col_tag, d:val, s:"% ", s:no_tag, l:text);
			}
		return text;

		case INV_IMP_STAMINAONKILL:
			text = StrParam(s:col_tag, d:val, s:"%\c- ", l:text, s:" ", s:col_tag, d:extra, s:"\c- ", l:"IATTR_TI44S");
		return text;

		// fixed point implicits
		case INV_IMP_LESSFIRETAKEN:
		case INV_IMP_LESSPOISONTAKEN:
		case INV_IMP_LESSLIGHTNINGTAKEN:
			text = StrParam(s:col_tag, s:GetFixedRepresentation(val, true), s:"%\c- ", l:text);
		return text;

		case INV_IMP_MOREDAMAGETOBOSSES:
			text = StrParam(l:text, s:col_tag, d:val, s:"%\c-", s:"\c- ", l:"IATTR_TI31S");
		return text;
		case INV_IMP_PRECISIONCRITBONUS:
			text = StrParam(l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:"%\c-", l:"IATTR_TI33S");
		return text;

		case INV_CORR_WEAPONFORCEPAIN:
			return StrParam(l:GetWeaponTag(extra), s:" ", l:"DND_HAS", s:col_tag, s: " ", d:val, s:"%", s:no_tag, l:text);

		// fixed point corrupted
		case INV_CORR_DROPCHANCE:
		case INV_CORR_SPEED:
		case INV_CORR_MOREAOE:
			if(showDetailedMods) {
				return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedImplicitModRange(attr, item_type, item_subtype, FACTOR_FIXED_RESOLUTION, true), s:"%", s:no_tag, l:text);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:no_tag, l:text);

		// fixed point positive wep stuff
		case INV_CORR_WEAPONDMG:
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, s:GetFixedRepresentation(val - 1.0, true), s:GetDetailedImplicitModRange(attr, item_type, item_subtype, FACTOR_FIXED_RESOLUTION, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra)
				);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val - 1.0, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra));

		// fixed point corrupted wep stuff
		case INV_CORR_WEAPONCRIT:
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedImplicitModRange(attr, item_type, item_subtype, FACTOR_FIXED_RESOLUTION, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra)
				);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:no_tag, l:text, s: " ", l:GetWeaponTag(extra));

		// % corruption mods with ranges
		case INV_CORR_PERCENTSTATS:
			if(showDetailedMods) {
				return StrParam(s:"+", s:col_tag, d:val, s:GetDetailedImplicitModRange(attr, item_type, item_subtype, 0), s:"%", s:no_tag, l:text);
			}
			return StrParam(s:"+", s:col_tag, d:val, s:"%", s:no_tag, l:text);

		// flat corruption mods with no ranges
		case INV_CORR_MAXFRENZY:
		case INV_CORR_MAXENDURANCE:
		case INV_CORR_MAXPOWER:
			return StrParam(s:"+", s:col_tag, d:val, s:no_tag, l:text);

		// flat corruption mods with ranges
		case INV_CORR_ALLPIERCE:
			if(showDetailedMods) {
				return StrParam(s:"+", s:col_tag, d:val, s:GetDetailedImplicitModRange(attr, item_type, item_subtype, 0), s:no_tag, l:text);
			}
			return StrParam(s:"+", s:col_tag, d:val, s:no_tag, l:text);

		// fixed point stuff
		case INV_PROJSPEED:
		case INV_EXPGAIN_INCREASE:
		case INV_CREDITGAIN_INCREASE:
		case INV_DROPCHANCE_INCREASE:
		case INV_ITEMRARITY:
		case INV_LUCK_INCREASE:
		case INV_PELLET_INCREASE:
		case INV_CRITCHANCE_INCREASE:
		case INV_CRITPERCENT_INCREASE:
		case INV_BLOCKERS_MOREDMG:
		case INV_OVERLOAD_DMGINCREASE:
		case INV_LIFESTEAL_DAMAGE:
		case INV_IGNITE_PROLIFRANGE:
		case INV_DAMAGEPERCENT_MORE:
		case INV_MELEECRIT_NOTONLOWSTAMINA:
			if(showDetailedMods) {
				return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%", s:no_tag, l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:no_tag, l:text);

		case INV_CRITPERCENT_FORWEPTYPE:
			if(showDetailedMods) {
				return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%", s:no_tag, l:text, s:" ", s:col_tag, l:GetWeaponTag(attr_extra),
					s:no_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:no_tag, l:text, s:" ", s:col_tag, l:GetWeaponTag(attr_extra));
		
		case INV_ESS_ERYXIA:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%", s:ess_tag, s:" ", l:"IATTR_MOREDMG",
					s:no_tag, s:"- ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:"%", s:ess_tag, s:" ", l:"IATTR_MOREDMG");
		
		// damage reduction attributes are shown as they are
		case INV_DMGREDUCE_ELEM:
		case INV_DMGREDUCE_PHYS:
		case INV_DMGREDUCE_REFL:
		case INV_DMGREDUCE_ENERGY:
		case INV_DMGREDUCE_MAGIC:
		case INV_DMGREDUCE_ALL:
		case INV_ADDEDMAXRESIST:
		case INV_OVERLOAD_DURATION:
		case INV_LIFESTEAL:
		case INV_MIT_INCREASE:
		case INV_MITEFFECT_INCREASE:
			if(showDetailedMods) {
				return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, false), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, false), s:"%", s:no_tag, l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val, false), s:"%", s:no_tag, l:text);

		// incursion special attribs
		case INV_INC_DOUBLEHPBONUS:
		return StrParam(s:"\cg-", d:attr_extra, s:"\c- ", s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC1S");

		case INV_INC_CRITFORDOT:
		return StrParam(s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC4S");
		case INV_INC_ALLOVERLOAD:
		return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"%\n", s:ess_tag, l:"IATTR_TINC5S");
		case INV_INC_ESHIELDNOINTERRUPT:
		return StrParam(s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC6S");
		
		case INV_INC_PLUSPROJ:
		case INV_INC_PLUSTWOPROJ:
		return StrParam(s:ess_tag, l:text, s:col_tag, s:" ", l:GetWeaponTag(attr_extra >> 16), s:"\n", s:col_tag, d:((1.0 - (attr_extra & 0xFFFF)) * 100) >> 16, s:"% ", s:ess_tag, l:"IATTR_TINC7S", s:col_tag, l:GetWeaponTag(attr_extra >> 16));
		
		case INV_CORR_WEAPONPLUSPROJ:
		return StrParam(s:ess_tag, l:text, s:col_tag, s:" ", l:GetWeaponTag(extra >> 16));

		case INV_INC_INSTANTLIFESTEAL:
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text,
					s:" - ", s:GetModTierText(tier, extra),
					s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC9S"
				);
			}
			return StrParam(s:"+", s:col_tag, d:val, s:"% ", s:ess_tag, l:text, s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC9S");
		
			case INV_INC_MITIGATIONTODODGE:
		return StrParam(s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC11S");

		// single text incursion
		case INV_INC_ACCURACYFORPRECISION:
		case INV_INC_HPREGENINTERRUPT:
		case INV_INC_ACCURACYREVERSED:
		if(showDetailedMods) {
			return StrParam(s:ess_tag, l:text, s:" - ", s:GetModTierText(tier, extra));
		}
		return StrParam(s:ess_tag, l:text);

		case INV_FLASK_IMMUNE_BLEED:
		case INV_FLASK_IMMUNE_CHILLFREEZE:
		case INV_FLASK_IMMUNE_IGNITE:
		case INV_FLASK_IMMUNE_POISON:
		case INV_FLASK_IMMUNE_SHOCK:
		if(showDetailedMods)
			return StrParam(l:text, s:" - ", s:GetModTierText(tier, extra));
		return StrParam(l:text);
		
		case INV_INC_PASSIVEREGEN:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"% ", s:ess_tag, l:"IATTR_TINC14S1",
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra),
					s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC14S2"
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, true), s:"% ", s:ess_tag, l:"IATTR_TINC14S1", s:"\n", s:col_tag, d:attr_extra, s:"% ", s:ess_tag, l:"IATTR_TINC14S2");
		
		case INV_INC_ENEMYRIPCHANCE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:"IATTR_TINC15S",
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"% ", s:ess_tag, l:"IATTR_TINC15S");
		case INV_INC_BLOCKPREVENTION:
		case INV_INC_INVERTRESISTANCES:
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:text,
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+", s:col_tag, d:val, s:"% ", s:ess_tag, l:text);
		case INV_INC_RIPPERSEXPLODE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"% ", s:ess_tag, l:"IATTR_TINC17S",
					s:ess_tag, s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, d:val, s:"% ", s:ess_tag, l:"IATTR_TINC17S");

		case INV_INC_PROJREVERSE:
			if(showDetailedMods) {
				return StrParam(
					s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, false), s:GetDetailedModRange(attr, item_type, item_subtype, tier, FACTOR_FIXED_RESOLUTION, extra, false),
					s:ess_tag, l:"IATTR_TINC20S"
				);
			}
			return StrParam(s:ess_tag, l:text, s:col_tag, s:GetFixedRepresentation(val, false), s:ess_tag, l:"IATTR_TINC20S");

		case INV_INC_EXCESSCRIT:
		return StrParam(s:col_tag, s:GetFixedRepresentation(val, true), s:ess_tag, l:text, s:"\n", s:ess_tag, l:"IATTR_TINC21S");

		// fix coloration issue on these text
		case INV_CRUSHINGBLOW:
		case INV_DEEPCUTS:
		case INV_OPENWOUNDS:
		case INV_DEADLYSTRIKE:
		case INV_REAPINGCLEAVE:
				if(showDetailedMods) {
					return StrParam(
						s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text, s:no_tag, l:"IATTR_ONMELEEHIT",
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+", s:col_tag, d:val, s:"%", s:no_tag, l:text, s:no_tag, l:"IATTR_ONMELEEHIT");

		// flasks
		case INV_FLASK_IMP_CHARGECOUNT:
		return StrParam(s:no_tag, l:text, s: " ", s:col_tag, d:val, s:no_tag, l:"IATTR_IMP_FLASK0X", s:" ", s:col_tag, d:extra, s:no_tag, l:"IATTR_IMP_FLASK0XX");
		case INV_FLASK_IMP_LIFE:
		return StrParam(s:col_tag, d:val, s:no_tag, l:text, s: " ", s:col_tag, s:GetFixedRepresentation(extra * 1.0 / TICRATE, false), s:no_tag, l:"IATTR_IMP_FLASK_SECONDS");

		case INV_FLASK_IMP_GRANITE:
		return StrParam(s:col_tag, d:val, s:no_tag, l:text, s: " ", s:col_tag, s:GetFixedRepresentation(extra * 1.0 / TICRATE, false), s:no_tag, l:"IATTR_IMP_FLASK_SECONDS");
		case INV_FLASK_IMP_BASALT:
		case INV_FLASK_IMP_BISMUTH:
		case INV_FLASK_IMP_INSULAR:
		case INV_FLASK_IMP_OAK:
		case INV_FLASK_IMP_ARCANE:
		case INV_FLASK_IMP_DIAMOND:
		case INV_FLASK_IMP_SULPHUR:
		case INV_FLASK_IMP_QUICKSILVER:
		case INV_FLASK_IMP_QUARTZ:
		return StrParam(s:col_tag, d:val, s:"%", s:no_tag, l:text, s: " ", s:col_tag, s:GetFixedRepresentation(extra * 1.0 / TICRATE, false), s:no_tag, l:"IATTR_IMP_FLASK_SECONDS");

		// this one has text not a number besides use amount
		case INV_FLASK_IMP_SILVER:
		return StrParam(l:text, s: " ", s:col_tag, s:GetFixedRepresentation(extra * 1.0 / TICRATE, false), s:no_tag, l:"IATTR_IMP_FLASK_SECONDS");
		
		case INV_FLASK_INSTANTRECOVERY:
		case INV_FLASK_INSTANTONLOWLIFE:
		if(showDetailedMods) {
			return StrParam(
				l:text, s:"\n", s:col_tag, d:attr_extra, s:GetDetailedModRangeExtra(attr, item_type, item_subtype, tier, 0), s:"%", 
				s:no_tag, l:"IATTR_FLASK3X", s:" - ", s:GetModTierText(tier, extra)
			);
		}
		return StrParam(l:text, s:"\n", s:col_tag, d:attr_extra, s:"%", s:no_tag, l:"IATTR_FLASK3X");

		case INV_FLASK_INCCHARGERECOVERY:
		if(showDetailedMods) {
			return StrParam(
				s:col_tag, d:val, s:"%", s:no_tag, l:text, s:"\n", s:col_tag, d:attr_extra, s:GetDetailedModRangeExtra(attr, item_type, item_subtype, tier, 0),
				s:"%", s:no_tag, l:"IATTR_FLASK13X", s:" - ", s:GetModTierText(tier, extra)
			);
		}
		return StrParam(s:col_tag, d:val, s:"%", s:no_tag, l:text, s:"\n", s:col_tag, d:attr_extra, s:"%", s:no_tag, l:"IATTR_FLASK13X");
		
		case INV_FLASK_CHANCEGAINONHIT:
		if(showDetailedMods) {
			return StrParam(
				s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0), 
				s:"%", s:no_tag, l:text, s:" - ", s:GetModTierText(tier, extra));
		}
		return StrParam(s:col_tag, d:val, s:"%", s:no_tag, l:text);

		case INV_FLASK_INCEFFECT:
		if(showDetailedMods) {
			return StrParam(
				s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text, s:"\n",
				s:col_tag, d:attr_extra, s:GetDetailedModRangeExtra(attr, item_type, item_subtype, tier, 0), s:"%", s:no_tag, l:"IATTR_FLASK4X",
				s:" - ", s:GetModTierText(tier, extra)
			);
		}
		return StrParam(s:col_tag, d:val, s:"%", s:no_tag, l:text, s:"\n", s:col_tag, d:attr_extra, s:"%", s:no_tag, l:"IATTR_FLASK4X");

		// default takes percentage values
		default:
			if(val > 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"+", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+", s:col_tag, d:val, s:"%", s:no_tag, l:text);
			}
			else if(val < 0) {
				if(showDetailedMods) {
					return StrParam(
						s:"-", s:col_tag, d:val, s:GetDetailedModRange(attr, item_type, item_subtype, tier, 0, extra), s:"%", s:no_tag, l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"-\cg", d:val, s:"%", s:no_tag, l:text);
			}
	}
	return "";
}

// this definitely needs some better cleaning up for sure... in the future
str GetItemAttributeText(
	int attr, int item_type, int item_subtype, 
	int val1, int val2 = -1, int tier = 0, 
	bool showDetailedMods = false, int extra = -1, bool isFractured = false, 
	int qual = 0, int craftAffected = 0
)
{
	// treat it as normal inv attribute range
	// check last essence as its an all encompassing range except exotics
	if(item_type == DND_ITEM_DUNGEONKEY)
		return DungeonAttributeString(attr, val1, tier, showDetailedMods, extra, isFractured, qual, val2, craftAffected);

	if(attr <= LAST_FLASK_ATTRIBUTE)
		return ItemAttributeString(attr, item_type, item_subtype, val1, tier, showDetailedMods, extra, isFractured, qual, val2, craftAffected);

	if(qual) {
		if(!IsAttributeQualityException(attr)) {
			if(val1 < 100000) {
				val1 *= qual + 100;
				val1 /= 100;
			}
			else {
				val1 /= 100;
				val1 *= qual + 100;
			}

			if(val2 < 100000) {
				val2 *= qual + 100;
				val2 /= 100;
			}
			else {
				val2 /= 100;
				val2 *= qual + 100;
			}
		}
	}

	str col_tag = "\c[Q9]";
	str bad_tag = "\cg";
	if(craftAffected & 0xFF) {
		col_tag = "\ck";
		bad_tag = "\ck";
	}

	// if the item is unique extra is not -1
	str text = GetInventoryAttributeText(attr);
	switch(attr) {
		case INV_EX_FACTOR_SMALLCHARM:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:col_tag, d:val1 / 1000, s:".", d:(val1 / 100 % 10), d:(val1 / 10) % 10, s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, false), s:"\c-",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:col_tag, d:val1 / 1000, s:".", d:(val1 / 100 % 10), d:(val1 / 10) % 10);
		
		case INV_EX_CHANCE_HEALMISSINGONPAIN:
			if(showDetailedMods) {
				return StrParam(
					s:col_tag, d:val2, s:GetDetailedModRange_Unique(tier, 0, extra, false, true), s:"%\c- ",
					l:text, s:col_tag, d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_RECOVERHPHURT",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:col_tag, d:val2, s:"%\c- ", l:"IATTR_TX_CHANCE", l:text, s:col_tag, d:val1, s:"%\c- ", l:"IATTR_RECOVERHPHURT");
		
		case INV_EX_PHYSDAMAGEPER_FLATHEALTH:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:col_tag, d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_MAXHEALTH",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:col_tag, d:val1, s:"\c- ", l:"IATTR_MAXHEALTH");
		
		case INV_EX_ONKILL_HEALMISSING:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:col_tag, d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_PMISSHP",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:col_tag, d:val1, s:"%\c- ", l:"IATTR_PMISSHP");
		
		case INV_EX_ABILITY_RALLY:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:col_tag, d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_RALLY",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:col_tag, d:val1, s:"\c- ", l:"IATTR_RALLY");
		
		case INV_EX_CRITIGNORERESCHANCE:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:col_tag, d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_CHANCEIGNORERES",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:col_tag, d:val1, s:"%\c- ", l:"IATTR_CHANCEIGNORERES");
		
		// float factor stuff
		case INV_EX_MORECRIT_LIGHTNING:
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, s:GetFixedRepresentation(val1, true), s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"%\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val1, true), s:"%\c- ", l:text);

		// float factor that isnt a percentage by representation, but has % at the end
		case INV_EX_MOREDMGPEROVERHEAT:
			if(showDetailedMods) {
				return StrParam(
					s:"+", s:col_tag, s:GetFixedRepresentation(val1, false), s:GetDetailedModRange_Unique(tier, 1, extra, false), s:"%\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+", s:col_tag, s:GetFixedRepresentation(val1, false), s:"%\c- ", l:text);

		case INV_EX_INTBONUSTOMELEE:
			if(showDetailedMods) {
				return StrParam(
					l:text, s: " ", s:col_tag, s:"x", s:GetFixedRepresentation(val1, false), s:GetDetailedModRange_Unique(tier, 1, extra, false),
					s:"\c- ", l:"IATTR_TX41S", s:"\n", l:"IATTR_TX41B", s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s: " ", s:col_tag, s:"x", s:GetFixedRepresentation(val1, false), s:"\c- ", l:"IATTR_TX41S", s:"\n", l:"IATTR_TX41B");
			
		case INV_EX_SOULWEPSPEN:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_MAGICRES",
					s:"\c- - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_MAGICRES");

		case INV_EX_SECONDEXPBONUS:
			if(showDetailedMods) {
				return StrParam(
					l:text, s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX8B", s:"\n",
					l:"IATTR_TX8S", s:bad_tag, d:val2, s:"%\c-", l:"IATTR_TX8S2",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(l:text, s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX8B", s:"\n", l:"IATTR_TX8S", s:bad_tag, d:val2, s:"%\c-", l:"IATTR_TX8S2");

		case INV_EX_DEADEYEBONUS:
			if(showDetailedMods)
				return StrParam(l:text, s:"\n", l:"IATTR_TX33A", s:"\n", l:"IATTR_TX33B", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:text, s:"\n", l:"IATTR_TX33A", s:"\n", l:"IATTR_TX33B");

		case INV_EX_UNITY_RES_BONUS:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX_UNITY", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX38", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX_UNITY", s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX38");
		case INV_EX_UNITY_PEN_BONUS:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX_UNITY", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX39", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX_UNITY", s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX39");

		case INV_EX_ESEXPLOSIONHPDMG:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX44", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX44S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX44", s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX44S");
		case INV_EX_ESCHARGE_USEHP:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX45", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX45S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX45", s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX45S");

		case INV_EX_HPTOESHIELD:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX46", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX46S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX46", s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX46S");

		case INV_EX_RESPERESHIELD:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX49", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_TX49S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX49", s:col_tag, s:" ", d:val1, s:"\c- ", l:"IATTR_TX49S");
		case INV_EX_ESHIELDONLYBLOCKPCT:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX50", s:bad_tag, s: " ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\cg%\c- ", l:"IATTR_TX50S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX50", s:bad_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX50S");

		case INV_EX_REFILLAMMOONMELEEKILL:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX51", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX51", s:col_tag, s:" ", d:val1, s:"%");
		case INV_EX_SWAPFROMMELEECRIT:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX52", s:col_tag, s:" ", s:GetFixedRepresentation(val1, true), s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, true), s:"\c- ", l:"IATTR_TX52S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX52", s:col_tag, s:" ", s:GetFixedRepresentation(val1, true), s:"%\c- ", l:"IATTR_TX52S");

		case INV_EX_SOULPICKUPSINFAMMO:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX55", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_TX55S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX55", s:col_tag, s:" ", d:val1, s:"\c- ", l:"IATTR_TX55S");

		case INV_EX_SOULPICKUPSONLYAMMO:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX56", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:"IATTR_TX56S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX56", s:col_tag, s:" ", d:val1, s:"\c- ", l:"IATTR_TX56S");

		case INV_EX_STREXTRABONUSTOMELEE:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX57", s:col_tag, s:" ", s:GetFixedRepresentation(val1, false), s:GetDetailedModRange_Unique(tier, FACTOR_FIXED_RESOLUTION, extra, false), s:"\c- ", l:"IATTR_TX57S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX57", s:col_tag, s:" ", s:GetFixedRepresentation(val1, false), s:"\c- ", l:"IATTR_TX57S");

		case INV_EX_LESSDMGTAKENMAXOVERHEAT:
			if(showDetailedMods)
				return StrParam(l:"IATTR_TX62", s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:"IATTR_TX62S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:"IATTR_TX62", s:col_tag, s:" ", d:val1, s:"%\c- ", l:"IATTR_TX62S");
		
		case INV_EX_CHANCEGAINXCHARGE:
			if(showDetailedMods)
				return StrParam(s:col_tag, d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:text, s:" ", s:col_tag, l:StrParam(s:"LCHARGE_", d:val2), s: "\c- ", l:"IATTR_TX69S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(s:col_tag, d:val1, s:"%\c- ", l:text, s:" ", s:col_tag, l:StrParam(s:"LCHARGE_", d:val2), s: "\c- ", l:"IATTR_TX69S");
		
		// single text things, no mod ranges, just tier U
		case INV_EX_KNOCKBACK_IMMUNITY:
		case INV_EX_DOUBLE_HEALTHCAP:
		case INV_EX_BEHAVIOR_PELLETSFIRECIRCLE:
		case INV_EX_FORSHOW_BURSTGETSPELLETBONUS:
		case INV_EX_DMGREDUCE_SHAREWITHPETS:
		case INV_EX_SOULWEPS_FULLDAMAGE:
		case INV_EX_ABILITY_MONSTERSRIP:
		case INV_EX_BEHAVIOR_SPELLSFULLDAMAGE:
		case INV_EX_ABILITY_LUCKYCRIT:
		case INV_EX_CURSEIMMUNITY:
		case INV_EX_DAMAGPERMISSINGAMMO:
		case INV_EX_UNITY:
		case INV_EX_UNITY_NOBONUS:
		case INV_EX_ESCHARGE_DMGNOINTERRUPT:
		case INV_EX_HEALTHATONE:
		case INV_EX_ESHIELDFULLABSORB:
		case INV_EX_DEMONBARRIERS:
		case INV_EX_ELEPENHARMONY:
		case INV_EX_CANFIREOVERHEATED:
		case INV_EX_WEAPONSUSEHEALTH:
		case INV_EX_RIPPERSRIPALL:
		case INV_EX_MIRROROTHERMEDIUM:
			if(showDetailedMods)
				return StrParam(l:text, s:" - ", s:GetModTierText(tier, extra));
			return StrParam(l:text);
		
		case INV_EX_LIMITEDSMALLCHARMS:
			if(showDetailedMods) {
				// singular matters here
				if(val1 > 1)
					return StrParam(
						l:text, s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s: "\c- ", l:"IATTR_TX29_2",
						s:" - ", s:GetModTierText(tier, extra)
					);
				return StrParam(
					l:text, s:col_tag, s:" ", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s: "\c- ", l:"IATTR_TX29_2S",
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			if(val1 > 1)
				return StrParam(l:text, s:col_tag, s:" ", d:val1, s: "\c- ", l:"IATTR_TX29_2");
			return StrParam(l:text, s:col_tag, s:" ", d:val1, s: "\c- ", l:"IATTR_TX29_2S");

		case INV_EX_COUNTASHAVINGMAXCHARGEOF:
			return StrParam(l:text, s:" ", l:StrParam(s:"LCHARGE_NOPRE_", d:val2));
		
		case INV_EX_ALLSTATS:
		case INV_EX_FLATDMG_ALL:
		case INV_EX_FLATDOT:
		case INV_EX_FLATPERSHOTGUNOWNED:
			if(showDetailedMods) {
				return StrParam(
					s:"+\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c- ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+\c[Q9]", d:val1, s:"\c- ", l:text);
		
		// negative %
		case INV_EX_LESSHEALING:
			if(val1) {
				if(showDetailedMods) {
					return StrParam(
						s:"-\cg", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c[D4] ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"-\cg", d:val1, s:"%\c[D4] ", l:text);
			}
			return StrParam(l:text);
			
		// negative effects are shown with different color -- these are % ones of those, these are positive numerically
		case INV_EX_DMGINCREASE_TAKEN:
			if(showDetailedMods) {
				return StrParam(s:"+\cg", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c[D4] ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+\cg", d:val1, s:"%\c[D4] ", l:text);

		// negative effect without any %
		case INV_EX_AMMOCOSTMULTIPLIER:
			if(showDetailedMods) {
				return StrParam(s:"+\cg", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"\c[D4] ", l:text,
					s:" - ", s:GetModTierText(tier, extra)
				);
			}
			return StrParam(s:"+\cg", d:val1, s:"%\c[D4] ", l:text);

		case INV_EX_MOREAMMOUSE:
			if(showDetailedMods)
				return StrParam(s:"\c[D4] ", l:text, s:" ", s:bad_tag, d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c[D4] ", l:"IATTR_TX35S", s:" - ", s:GetModTierText(tier, extra));
			return StrParam(s:"\c[D4] ", l:text, s:" ", s:bad_tag, d:val1, s:"%\c[D4] ", l:"IATTR_TX35S");
			
		// single text negative effects
		case INV_EX_FORBID_ARMOR:
		case INV_EX_CANNOTPICKAMMO:
		case INV_EX_CANTFIRENONOVERHEAT:
		case INV_EX_RIPPERSONETIMEONLY:
		case INV_EX_CHARGEDURATIONHALVED:
			if(showDetailedMods) {
				return StrParam(s:"\c[D4]", l:text, s:"\c- - ", s:GetModTierText(tier, extra));
			}
			return StrParam(s:"\c[D4]", l:text);
		
		// default are %
		default:
			if(val1) {
				if(showDetailedMods) {
					return StrParam(
						s:"+\c[Q9]", d:val1, s:GetDetailedModRange_Unique(tier, 0, extra), s:"%\c- ", l:text,
						s:" - ", s:GetModTierText(tier, extra)
					);
				}
				return StrParam(s:"+\c[Q9]", d:val1, s:"%\c- ", l:text);
			}
			return StrParam(l:text);
	}
	return "";
}

#endif