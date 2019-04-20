#include "main.h"

struct skill_info {
	const char*		id;
	const char*		name;
	cflags<class_s>	allow;
} skill_data[] = {{"SaveVsParalization", "Защита от парализации"},
{"SaveVsPoison", "Защита от яда"},
{"SaveVsDeath", "Защита от смерти"},
{"SaveVsWands", "Защита от палочек"},
{"SaveVsPetrification", "Защита от окаменения"},
{"SaveVsPolymorph", "Защита от превращения"},
{"SaveVsBreathWeapon", "Защита от дыхания"},
{"SaveVsSpells", "Защита от магии"},
//
{"PickPockets", "Очищать карманы", {Theif}},
{"OpenLocks", "Открывать замки", {Theif}},
{"FindRemoveTraps", "Обезвреживать ловушки", {Theif}},
{"MoveSilently", "Двигаться тихо", {Theif, Ranger}},
{"HideInShadows", "Скрываться в тенях", {Theif, Ranger}},
{"DetectNoise", "Слышать звуки", {Theif}},
{"ClimbWalls", "Лазить по стенам", {Theif}},
{"ReadLanguages", "Читать на всех языках", {Theif}},
};
getstr_enum(skill);
assert_enum(skill, ReadLanguages);

static char savevs_data[4][5][22] = {
	// Warriors - 0
	{{16, 14, 14, 13, 13, 11, 11, 10, 10, 8, 8, 7, 7, 5, 5, 4, 4, 3, 3, 2, 2, 2},
{18, 16, 16, 15, 15, 13, 13, 12, 12, 10, 10, 9, 9, 7, 7, 6, 6, 5, 5, 3, 3, 2},
{17, 15, 15, 14, 14, 12, 12, 11, 11, 9, 9, 8, 8, 6, 6, 5, 5, 4, 4, 3, 3, 2},
{20, 17, 17, 16, 16, 13, 13, 12, 12, 9, 9, 8, 8, 5, 5, 4, 4, 3, 3, 3, 3, 3},
{19, 17, 17, 16, 16, 14, 14, 13, 13, 11, 11, 10, 10, 8, 8, 7, 7, 6, 6, 5, 5, 4}},
// Priest - 1
{{10, 10, 10, 10, 9, 9, 9, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 2, 2, 2},
{14, 14, 14, 14, 13, 13, 13, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 6, 6, 6},
{13, 13, 13, 13, 12, 12, 12, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 5, 5, 5},
{16, 16, 16, 16, 15, 15, 15, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 8, 8, 8},
{15, 15, 15, 15, 14, 14, 14, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 7, 7, 7}},
// Rogues - 2
{{13, 13, 13, 13, 13, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 8},
{14, 14, 14, 14, 14, 12, 12, 12, 12, 10, 10, 10, 10, 8, 8, 8, 8, 6, 6, 6, 6, 4},
{12, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 8, 7},
{16, 16, 16, 16, 16, 15, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 12, 12, 12, 12, 11},
{15, 15, 15, 15, 15, 13, 13, 13, 13, 11, 11, 11, 11, 9, 9, 9, 9, 7, 7, 7, 7, 5}},
// Wizards - 3
{{14, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 8},
{11, 11, 11, 11, 11, 11, 9, 9, 9, 9, 9, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 3},
{13, 13, 13, 13, 13, 13, 11, 11, 11, 11, 11, 9, 9, 9, 9, 9, 7, 7, 7, 7, 7, 5},
{15, 15, 15, 15, 15, 15, 13, 13, 13, 13, 13, 11, 11, 11, 11, 11, 9, 9, 9, 9, 9, 7},
{12, 12, 12, 12, 12, 12, 10, 10, 10, 10, 10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 4}},
};
static char theive_skills_by_dex[ReadLanguages + 1][20] = {
	{-60, -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0, 0, 0, 0, 0, 5, 10, 15},
{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -5, 0, 0, 0, 0, 0, 5, 10, 15, 20},
{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -5, 0, 0, 0, 0, 0, 0, 5, 10},
{-20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -15, -10, -5, 0, 0, 0, 0, 5, 10, 15},
{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -5, 0, 0, 0, 0, 0, 0, 5, 10, 15},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
static char default_theive_skills[ReadLanguages + 1][18] = {
	{0, 30, 35, 40, 45, 50, 55, 60, 65, 70, 80, 90, 95, 99, 99, 99, 99, 99},
{0, 25, 29, 33, 37, 42, 47, 52, 57, 62, 67, 72, 77, 82, 87, 92, 97, 99},
{0, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 99},
{0, 15, 21, 27, 33, 40, 47, 55, 62, 70, 78, 86, 94, 99, 99, 99, 99, 99},
{0, 10, 15, 20, 25, 31, 37, 43, 49, 56, 63, 70, 77, 85, 93, 99, 99, 99},
{10, 10, 10, 15, 15, 20, 20, 25, 25, 30, 30, 35, 35, 40, 40, 50, 50, 55},
{40, 85, 86, 87, 88, 90, 92, 94, 96, 98, 99, 99, 99, 99, 99, 99, 99, 99},
{0, 0, 0, 0, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 80},
};
extern char reaction_adjustment[26];
static char darwen_bonus[] = {
	0, 0, 0, 0, 1, 1, 1, 2, 2, 2,
	2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5,
	6, 6, 6, 6, 7
};
static char widow_spell_bonus[] = {
	-4, -4, -4, -3, -2, -1, -1, -1, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 3, 4, 5
};
static char save_index[] = {
	0, 0, 0,
	1,
	2, 2,
	3,
	4,
};
static_assert(sizeof(save_index) / sizeof(save_index[0]) == SaveVsSpells + 1, "Invalid count of save index elements");

static int get_save_group(class_s value) {
	switch(value) {
	case Theif: return 2;
	case Mage: return 3;
	case Cleric: return 1;
	default: return 0;
	}
}

static int get_save_thrown(skill_s id, class_s type, const char* levels) {
	auto index = save_index[id];
	auto result = 20;
	for(unsigned i = 0; i < class_data[type].classes.count; i++) {
		auto n = levels[i];
		if(n < 1)
			continue;
		auto e = class_data[type].classes.data[i];
		auto g = get_save_group(e);
		if(n >= sizeof(savevs_data[0][0]) / sizeof(savevs_data[0][0][0]))
			n = sizeof(savevs_data[0][0]) / sizeof(savevs_data[0][0][0]) - 1;
		auto v = savevs_data[g][index][n];
		if(v && v < result)
			result = v;
	}
	return (result - 21) * 5;
}

static int get_theiv_skill(skill_s id, class_s type, const char* levels) {
	auto result = 0;
	for(unsigned i = 0; i < class_data[type].classes.count; i++) {
		auto n = levels[i];
		if(n < 1)
			continue;
		auto e = class_data[type].classes.data[i];
		if(skill_data[id].allow && !skill_data[id].allow.is(e))
			continue;
		if(n > 17)
			n = 17;
		result = default_theive_skills[id - PickPockets][n];
		break;
	}
	return result;
}

int	character::get(skill_s id) const {
	auto result = 0;
	if(id >= FirstSave && id <= LastSave) {
		auto index = save_index[id];
		result = get_save_thrown(id, type, levels);
		if(is(HolyGrace))
			result += 10;
		switch(id) {
		case SaveVsPoison:
			if(is(BonusSaveVsPoison))
				result += maptbl(darwen_bonus, get(Constitution)) * 5;
			break;
		case SaveVsWands:
			if(is(BonusSaveVsWands))
				result += maptbl(darwen_bonus, get(Constitution)) * 5;
			result += maptbl(reaction_adjustment, get(Dexterity)) * 5;
			break;
		case SaveVsSpells:
			if(is(BonusSaveVsSpells))
				result -= maptbl(darwen_bonus, get(Constitution));
			result += maptbl(widow_spell_bonus, get(Wisdow)) * 5;
			break;
		}
	} else if(id >= PickPockets && id <= ReadLanguages) {
		auto dex = get(Dexterity);
		result = get_theiv_skill(id, type, levels);
		result += race_data[race].theive_skills[id - PickPockets];
		result += maptbl(theive_skills_by_dex[id], dex);
	}
	return result;
}