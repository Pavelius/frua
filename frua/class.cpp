#include "main.h"

class_info class_data[] = {{"Cleric", "���������", Wisdow, {Cleric}},
{"Fighter", "����", Strenght, {Fighter}},
{"Mage", "���", Intellegence, {Mage}},
{"Paladin", "�������", Strenght, {Paladin}, {HolyGrace}},
{"Ranger", "��������", Strenght, {Ranger}},
{"Theif", "���", Dexterity, {Theif}},
//
{"FighterCleric", "����-���������", Strenght, {Fighter, Cleric}},
{"FighterMage", "����-���", Strenght, {Fighter, Mage}},
{"FighterTheif", "����-���", Strenght, {Fighter, Theif}},
{"ClericMage", "���������-���", Wisdow, {Cleric, Mage}},
{"MageTheif", "���-���", Intellegence, {Mage, Theif}},
{"FighterClericMage", "����-���������-���", Strenght, {Fighter, Cleric, Mage}},
{"FighterMageTheif", "����-���-���", Strenght, {Fighter, Mage, Theif}},
};
assert_enum(class, FighterMageTheif);
getstr_enum(class);