#include "main.h"

class_info class_data[] = {{"Cleric", "���������", Wisdow, {Cleric}, {}, {0, 0, 0, 0, 9, 0}},
{"Fighter", "����", Strenght, {Fighter}, {}, {9, 0, 7, 0, 0, 0}},
{"Mage", "���", Intellegence, {Mage}, {NoExeptionalStrenght}, {0, 6, 0, 9, 0, 0}},
{"Paladin", "�������", Strenght, {Paladin}, {HolyGrace}, {12, 0, 9, 13, 0, 17}},
{"Ranger", "��������", Strenght, {Ranger}, {}, {13, 0, 14, 13, 14, 0}},
{"Theif", "���", Dexterity, {Theif}, {NoExeptionalStrenght}, {0, 9, 0, 0, 0, 0}},
//
{"FighterCleric", "����-���������", Strenght, {Fighter, Cleric}, {}, {9, 0, 7, 0, 15, 0}},
{"FighterMage", "����-���", Strenght, {Fighter, Mage}, {}, {9, 0, 7, 9, 0, 0}},
{"FighterTheif", "����-���", Strenght, {Fighter, Theif}, {}, {9, 9, 7, 0, 0, 0}},
{"ClericMage", "���������-���", Wisdow, {Cleric, Mage}, {NoExeptionalStrenght}, {0, 0, 0, 9, 15, 0}},
{"MageTheif", "���-���", Intellegence, {Mage, Theif}, {NoExeptionalStrenght}, {0, 9, 0, 9, 0, 0}},
{"FighterClericMage", "����-���������-���", Strenght, {Fighter, Cleric, Mage}, {}, {9, 0, 7, 9, 15, 0}},
{"FighterMageTheif", "����-���-���", Strenght, {Fighter, Mage, Theif}, {}, {9, 9, 7, 9, 0, 0}},
};
assert_enum(class, FighterMageTheif);
getstr_enum(class);