#include "main.h"

static char hit_probability[] = {
	-5, -5, -3, -3, -2, -2, -1, -1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3,
	3, 4, 4, 5, 6, 7
};
static char damage_adjustment[] = {
	-5, -5, -3, -3, -2, -2, -1, -1, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7,
	8, 9, 10, 11, 12, 14
};
static int weight_allowance[] = {
	1, 1, 1, 5, 10, 10, 20, 20, 35, 35,
	40, 40, 45, 45, 55, 55, 70, 85, 110, 135, 160, 185, 235, 335, 485,
	535, 635, 785, 935, 1235, 1535,
};
static short unsigned max_press[] = {
	0
};
static char open_doors[] = {
	1, 1, 1, 2, 3, 3, 4, 4, 5, 5,
	6, 6, 7, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 16,
	17, 17, 18, 18, 19, 19
};
static char open_close_doors[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 8,
	10, 12, 14, 16, 17, 18
};
static char bend_bars[] = {
	-5, -5, -3, -3, -2, -2, -1, -1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3,
};
char reaction_adjustment[26] = {
	-6, -6, -4, -3, -2, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 2, 2, 3, 3,
	4, 4, 4, 5, 5
};
static char defence_adjustment[] = {
	-6, -6, -4, -3, -2, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, -1, -2, -3, -4, -4, -4,
	-5, -5, -5, -6, -6
};
static char hit_points_adjustment[] = {
	-3, -2, -2, -1, -1, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2
};
static char hit_points_adjustment_warriors[] = {
	-3, -2, -2, -1, -1, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 3, 4, 5,
	5, 6, 6, 6, 7, 7
};
static char number_languages[] = {
	0, 0, 1, 1, 1, 1, 1, 1, 1, 2,
	2, 2, 3, 3, 4, 4, 5, 6, 7, 8,
	9, 10, 11, 12, 15, 20
};
static char chance_learn_spells[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 35,
	40, 45, 50, 55, 60, 65, 70, 75, 85, 95,
	96, 97, 98, 99, 100
};
static char wp_melee_attacks[] = {
	0, 0, 1, 1
};
static char wp_melee_thac0[] = {
	0, 0, 1, 3,
};

void character::clear() {
	memset(this, 0, sizeof(character));
}

void character::roll_ability() {
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		auto v1 = dice::roll(3, 6);
		auto v2 = dice::roll(3, 6);
		abilities[i] = imax(v1, v2);
	}
	auto best_ability = class_data[type].ability;
}

bool character::isallow(alignment_s v) const {
	auto& ev = class_data[type];
	for(unsigned i = 0; i < ev.classes.count; i++) {
		auto e = ev.classes.data[i];
		if(alignment_data[v].restricted.indexof(e) != -1)
			return false;
	}
	return true;
}