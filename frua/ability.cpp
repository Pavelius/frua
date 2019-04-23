#include "main.h"

struct ability_info {
	const char*		id;
	const char*		name;
};
name_info ability_data[] = {
	{"Strenght", "Сила"},
	{"Dexterity", "Ловкость"},
	{"Constitution", "Телосложение"},
	{"Intellegence", "Интеллект"},
	{"Wisdow", "Мудрость"},
	{"Charisma", "Харизма"},
};
assert_enum(ability, Charisma);
getstr_enum(ability);