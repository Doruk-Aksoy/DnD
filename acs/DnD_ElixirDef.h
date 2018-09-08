#ifndef DND_ELIXIRDEF_IN
#define DND_ELIXIRDEF_IN

typedef struct {
	int hp_flat_bonus;
	int armor_flat_bonus;
	
	int hp_percent_bonus;
	int armor_percent_bonus;
	
	int speed_bonus;
	
	int luck; // this is in fixed!
	
	int damage_type_bonus[MAX_TALENTS];
} elixir_stat_T;

global elixir_stat_T 16: Player_Elixir_Bonuses[MAXPLAYERS];

#endif