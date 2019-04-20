#include "main.h"

class_info class_data[] = {{"Cleric", "Священник", Wisdow, {Cleric}},
{"Fighter", "Боец", Strenght, {Fighter}},
{"Mage", "Маг", Intellegence, {Mage}},
{"Paladin", "Паладин", Strenght, {Paladin}, {HolyGrace}},
{"Ranger", "Следопыт", Strenght, {Ranger}},
{"Theif", "Вор", Dexterity, {Theif}},
//
{"FighterCleric", "Воин-священник", Strenght, {Fighter, Cleric}},
{"FighterMage", "Воин-маг", Strenght, {Fighter, Mage}},
{"FighterTheif", "Воин-вор", Strenght, {Fighter, Theif}},
{"ClericMage", "Священник-маг", Wisdow, {Cleric, Mage}},
{"MageTheif", "Маг-вор", Intellegence, {Mage, Theif}},
{"FighterClericMage", "Воин-священник-маг", Strenght, {Fighter, Cleric, Mage}},
{"FighterMageTheif", "Воин-маг-вор", Strenght, {Fighter, Mage, Theif}},
};
assert_enum(class, FighterMageTheif);
getstr_enum(class);