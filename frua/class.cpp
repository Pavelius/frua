#include "main.h"

class_info class_data[] = {{"Cleric", "���������", 8, Wisdow, {Cleric},
{NoExeptionalStrenght, UseLeatherArmor, UseMetalArmor, UseShield}, {0, 0, 0, 0, 9, 0}},
{"Fighter", "����", 10, Strenght, {Fighter},
{UseLeatherArmor, UseMetalArmor, UseShield}, {9, 0, 7, 0, 0, 0}},
{"Mage", "���", 4, Intellegence, {Mage},
{NoExeptionalStrenght}, {0, 6, 0, 9, 0, 0}},
{"Paladin", "�������", 10, Strenght, {Paladin},
{HolyGrace, UseLeatherArmor, UseMetalArmor, UseShield}, {12, 0, 9, 13, 0, 17}},
{"Ranger", "��������", 8, Strenght, {Ranger},
{UseLeatherArmor, UseMetalArmor, UseShield}, {13, 0, 14, 13, 14, 0}, 1},
{"Theif", "���", 6, Dexterity, {Theif},
{NoExeptionalStrenght, UseLeatherArmor}, {0, 9, 0, 0, 0, 0}},
//
{"FighterCleric", "����-���������", 0, Strenght, {Fighter, Cleric}, {}, {9, 0, 7, 0, 15, 0}},
{"FighterMage", "����-���", 0, Strenght, {Fighter, Mage}, {}, {9, 0, 7, 9, 0, 0}},
{"FighterTheif", "����-���", 0, Strenght, {Fighter, Theif}, {}, {9, 9, 7, 0, 0, 0}},
{"ClericMage", "���������-���", 0, Wisdow, {Cleric, Mage}, {NoExeptionalStrenght}, {0, 0, 0, 9, 15, 0}},
{"MageTheif", "���-���", 0, Intellegence, {Mage, Theif}, {NoExeptionalStrenght}, {0, 9, 0, 9, 0, 0}},
{"FighterClericMage", "����-���������-���", 0, Strenght, {Fighter, Cleric, Mage}, {}, {9, 0, 7, 9, 15, 0}},
{"FighterMageTheif", "����-���-���", 0, Strenght, {Fighter, Mage, Theif}, {}, {9, 9, 7, 9, 0, 0}},
};
assert_enum(class, FighterMageTheif);
getstr_enum(class);