#include "main.h"

class_info class_data[] = {{"Cleric", "Священник", Wisdow, {Cleric}, {}, {0, 0, 0, 0, 9, 0}},
{"Fighter", "Боец", Strenght, {Fighter}, {}, {9, 0, 7, 0, 0, 0}},
{"Mage", "Маг", Intellegence, {Mage}, {NoExeptionalStrenght}, {0, 6, 0, 9, 0, 0}},
{"Paladin", "Паладин", Strenght, {Paladin}, {HolyGrace}, {12, 0, 9, 13, 0, 17}},
{"Ranger", "Следопыт", Strenght, {Ranger}, {}, {13, 0, 14, 13, 14, 0}},
{"Theif", "Вор", Dexterity, {Theif}, {NoExeptionalStrenght}, {0, 9, 0, 0, 0, 0}},
//
{"FighterCleric", "Воин-священник", Strenght, {Fighter, Cleric}, {}, {9, 0, 7, 0, 15, 0}},
{"FighterMage", "Воин-маг", Strenght, {Fighter, Mage}, {}, {9, 0, 7, 9, 0, 0}},
{"FighterTheif", "Воин-вор", Strenght, {Fighter, Theif}, {}, {9, 9, 7, 0, 0, 0}},
{"ClericMage", "Священник-маг", Wisdow, {Cleric, Mage}, {NoExeptionalStrenght}, {0, 0, 0, 9, 15, 0}},
{"MageTheif", "Маг-вор", Intellegence, {Mage, Theif}, {NoExeptionalStrenght}, {0, 9, 0, 9, 0, 0}},
{"FighterClericMage", "Воин-священник-маг", Strenght, {Fighter, Cleric, Mage}, {}, {9, 0, 7, 9, 15, 0}},
{"FighterMageTheif", "Воин-маг-вор", Strenght, {Fighter, Mage, Theif}, {}, {9, 9, 7, 9, 0, 0}},
};
assert_enum(class, FighterMageTheif);
getstr_enum(class);