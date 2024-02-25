#ifndef DND_ORBDEF_IN
#define DND_ORBDEF_IN

#define DND_MAX_ORB_REROLL_ATTEMPTS 100

enum {
	DND_ORB_ENHANCE,
	DND_ORB_CORRUPT,
	DND_ORB_PRISMATIC,
	DND_ORB_REPENT,
	DND_ORB_AFFLUENCE,

	DND_ORB_CALAMITY,
	DND_ORB_PROSPERITY,
	DND_ORB_NULLIFICATION,
	DND_ORB_DESTRUCTION,
	DND_ORB_VIOLENCE,

	DND_ORB_FORTITUDE,
	DND_ORB_SIN,
	DND_ORB_TREMORS,
	DND_ORB_TINKERER,
	DND_ORB_REFINEMENT,

	DND_ORB_SCULPTING,
	DND_ORB_ELEVATION,
	DND_ORB_TURMOIL,
	DND_ORB_HEXES,
	DND_ORB_GROWTH,

	DND_ORB_POTENCY,
	DND_ORB_CRACKLING,
	DND_ORB_BRUTE,
	DND_ORB_JAGGED,
	DND_ORB_ALCHEMIST,
	
	DND_ORB_EVOKER,
	DND_ORB_SAVAGERY,

	// unique drops from specific monsters
	DND_ORB_HOLLOW,
	DND_ORB_PHANTASMAL,
	DND_ORB_ASSIMILATION
};
#define DND_MAX_ORB_KINDS (DND_ORB_ASSIMILATION + 1)

#define MAX_TRANSMUTE_BOXES 3
typedef struct {
	int id;
	int count;
} comp_count_pair_T;

typedef struct {
	comp_count_pair_T components[MAX_TRANSMUTE_BOXES];
} orb_recipe_T;

#define DND_RANDOM_TAGGED_ORB_TRANSMUTE 420

orb_recipe_T& GetOrbRecipe(int orb_type) {
	static orb_recipe_T recipe;

	for(int i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		recipe.components[i].id = -1;
		recipe.components[i].count = 0;
	}

	switch(orb_type) {
		case DND_ORB_HOLLOW:
			recipe.components[0].id = DND_ORB_ELEVATION;
			recipe.components[0].count = 3;
		break;
		case DND_ORB_ELEVATION:
			recipe.components[0].id = DND_ORB_TURMOIL;
			recipe.components[0].count = 2;
			recipe.components[1].id = DND_ORB_REFINEMENT;
			recipe.components[1].count = 1;
		break;
		case DND_ORB_ASSIMILATION:
			recipe.components[0].id = DND_ORB_HOLLOW;
			recipe.components[0].count = 1;
			recipe.components[1].id = DND_ORB_ELEVATION;
			recipe.components[1].count = 1;
			recipe.components[2].id = DND_ORB_POTENCY;
			recipe.components[2].count = 1;
		break;
		case DND_ORB_TURMOIL:
			recipe.components[0].id = DND_ORB_SCULPTING;
			recipe.components[0].count = 1;
			recipe.components[1].id = DND_ORB_REFINEMENT;
			recipe.components[1].count = 1;
		break;
		case DND_ORB_NULLIFICATION:
			recipe.components[0].id = DND_ORB_SCULPTING;
			recipe.components[0].count = 3;
		break;
		case DND_ORB_ALCHEMIST:
			recipe.components[0].id = DND_ORB_ENHANCE;
			recipe.components[0].count = 2;
			recipe.components[1].id = DND_ORB_ELEVATION;
			recipe.components[1].count = 1;
		break;
		case DND_ORB_POTENCY:
			recipe.components[0].id = DND_ORB_EVOKER;
			recipe.components[0].count = 1;
			recipe.components[1].id = DND_ORB_ELEVATION;
			recipe.components[1].count = 1;
			recipe.components[2].id = DND_ORB_SCULPTING;
			recipe.components[2].count = 1;
		break;
		case DND_ORB_CALAMITY:
			recipe.components[0].id = DND_ORB_TURMOIL;
			recipe.components[0].count = 3;
		break;
		case DND_ORB_AFFLUENCE:
			recipe.components[0].id = DND_ORB_ALCHEMIST;
			recipe.components[0].count = 3;
		break;
		case DND_ORB_REPENT:
			recipe.components[0].id = DND_ORB_SIN;
			recipe.components[0].count = 1;
			recipe.components[1].id = DND_ORB_CORRUPT;
			recipe.components[1].count = 1;
			recipe.components[2].id = DND_ORB_NULLIFICATION;
			recipe.components[2].count = 1;
		break;
	}

	return recipe;
}

str GetOrbRecipeText(int orb_id) {
	auto recipe = GetOrbRecipe(orb_id);
	str res = "\c[Y5]Recipe: \c-";

	for(int i = 0; i < MAX_TRANSMUTE_BOXES; ++i) {
		if(recipe.components[i].id != -1)
			res = StrParam(s:res, s:"\cd", d:recipe.components[i].count,  s: "\c- x \c[Y5]", l:GetInventoryTag(DND_ISUBT_ORB_0 + recipe.components[i].id), s: "\c- ");
	}

	return res;
}

void MarkOrbRecipeComplete(int orb_id) {
	if(orb_id < 32)
		SetInventory("DnD_OrbRecipes_1", CheckInventory("DnD_OrbRecipes_1") | (1 << orb_id));
	else
		SetInventory("DnD_OrbRecipes_2", CheckInventory("DnD_OrbRecipes_2") | (1 << (orb_id - 32)));
}

bool CheckOrbRecipe(int orb_id) {
	if(orb_id < 32)
		return CheckInventory("DnD_OrbRecipes_1") & (1 << orb_id);
	return CheckInventory("DnD_OrbRecipes_2") & (1 << (orb_id - 32));
}

int PickLowTierOrb() {
	int o = random(0, 11);
	switch(o) {
		case 0:
		case 1:
		case 2:
			o = DND_ORB_ENHANCE;
		break;
		case 3:
		case 4:
		case 5:
			o = DND_ORB_TURMOIL;
		break;
		case 6:
		case 7:
			o = DND_ORB_REFINEMENT;
		break;
		case 8:
		case 9:
			o = DND_ORB_SCULPTING;
		break;
		case 10:
			o = DND_ORB_EVOKER;
		break;
		case 11:
			o = DND_ORB_CALAMITY;
		break;
	}
	return o;
}

int PickMidTierOrb() {
	int o = random(0, 13);
	switch(o) {
		case 0:
			o = DND_ORB_PRISMATIC;
		break;
		case 1:
			o = DND_ORB_DESTRUCTION;
		break;
		case 2:
			o = DND_ORB_JAGGED;
		break;
		case 3:
			o = DND_ORB_CRACKLING;
		break;
		case 4:
			o = DND_ORB_BRUTE;
		break;
		case 5:
			o = DND_ORB_TREMORS;
		break;
		case 6:
			o = DND_ORB_TINKERER;
		break;
		case 7:
			o = DND_ORB_VIOLENCE;
		break;
		case 8:
			o = DND_ORB_PROSPERITY;
		break;
		case 9:
			o = DND_ORB_FORTITUDE;
		break;
		case 10:
			o = DND_ORB_HEXES;
		break;
		case 11:
			o = DND_ORB_GROWTH;
		break;
		case 12:
			o = DND_ORB_ELEVATION;
		break;
		case 13:
			o = DND_ORB_SAVAGERY;
		break;
	}
	return o;
}

int PickHighTierOrb() {
	int o = random(0, 11);
	switch(o) {
		case 0:
		case 1:
			o = DND_ORB_CORRUPT;
		break;
		case 2:
		case 3:
			o = DND_ORB_SIN;
		break;
		case 4:
		case 5:
			o = DND_ORB_ALCHEMIST;
		break;
		case 6:
			o = DND_ORB_AFFLUENCE;
		break;
		case 7:
			o = DND_ORB_REPENT;
		break;
		case 8:
		case 9:
			o = DND_ORB_NULLIFICATION;
		break;
		case 10:
		case 11:
			o = DND_ORB_POTENCY;
		break;
	}
	return o;
}

int PickRandomTaggedOrb() {
	int o = random(0, 12);
	switch(o) {
		case 0:
			o = DND_ORB_PRISMATIC;
		break;
		case 1:
			o = DND_ORB_DESTRUCTION;
		break;
		case 2:
			o = DND_ORB_JAGGED;
		break;
		case 3:
			o = DND_ORB_CRACKLING;
		break;
		case 4:
			o = DND_ORB_BRUTE;
		break;
		case 5:
			o = DND_ORB_TREMORS;
		break;
		case 6:
			o = DND_ORB_TINKERER;
		break;
		case 7:
			o = DND_ORB_VIOLENCE;
		break;
		case 8:
			o = DND_ORB_PROSPERITY;
		break;
		case 9:
			o = DND_ORB_FORTITUDE;
		break;
		case 10:
			o = DND_ORB_HEXES;
		break;
		case 11:
			o = DND_ORB_GROWTH;
		break;
		case 12:
			o = DND_ORB_SAVAGERY;
		break;
	}
	return o;
}

#endif
