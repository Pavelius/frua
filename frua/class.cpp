#include "main.h"

class_info class_data[] = {{"Cleric", "Священник", 8, Wisdow, {Cleric},
{NoExeptionalStrenght, UseLeatherArmor, UseMetalArmor, UseShield}, {0, 0, 0, 0, 9, 0}},
{"Fighter", "Боец", 10, Strenght, {Fighter},
{UseLeatherArmor, UseMetalArmor, UseShield}, {9, 0, 7, 0, 0, 0}},
{"Mage", "Маг", 4, Intellegence, {Mage},
{NoExeptionalStrenght}, {0, 6, 0, 9, 0, 0}},
{"Paladin", "Паладин", 10, Strenght, {Paladin},
{HolyGrace, UseLeatherArmor, UseMetalArmor, UseShield}, {12, 0, 9, 13, 0, 17}},
{"Ranger", "Следопыт", 8, Strenght, {Ranger},
{UseLeatherArmor, UseMetalArmor, UseShield}, {13, 0, 14, 13, 14, 0}, 1},
{"Theif", "Вор", 6, Dexterity, {Theif},
{NoExeptionalStrenght, UseLeatherArmor}, {0, 9, 0, 0, 0, 0}},
//
{"FighterCleric", "Воин-священник", 0, Strenght, {Fighter, Cleric}, {}, {9, 0, 7, 0, 15, 0}},
{"FighterMage", "Воин-маг", 0, Strenght, {Fighter, Mage}, {}, {9, 0, 7, 9, 0, 0}},
{"FighterTheif", "Воин-вор", 0, Strenght, {Fighter, Theif}, {}, {9, 9, 7, 0, 0, 0}},
{"ClericMage", "Священник-маг", 0, Wisdow, {Cleric, Mage}, {NoExeptionalStrenght}, {0, 0, 0, 9, 15, 0}},
{"MageTheif", "Маг-вор", 0, Intellegence, {Mage, Theif}, {NoExeptionalStrenght}, {0, 9, 0, 9, 0, 0}},
{"FighterClericMage", "Воин-священник-маг", 0, Strenght, {Fighter, Cleric, Mage}, {}, {9, 0, 7, 9, 15, 0}},
{"FighterMageTheif", "Воин-маг-вор", 0, Strenght, {Fighter, Mage, Theif}, {}, {9, 9, 7, 9, 0, 0}},
};
assert_enum(class, FighterMageTheif);
getstr_enum(class);