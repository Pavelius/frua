#include "main.h"

struct ability_info {
	const char*		id;
	const char*		name;
};
name_info ability_data[] = {
	{"Strenght", "����"},
	{"Dexterity", "��������"},
	{"Constitution", "������������"},
	{"Intellegence", "���������"},
	{"Wisdow", "��������"},
	{"Charisma", "�������"},
};
assert_enum(ability, Charisma);
getstr_enum(ability);