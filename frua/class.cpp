#include "main.h"

class_info class_data[] = {{},
{"Cleric", "Священник", {Cleric}},
{"Fighter", "Боец", {Fighter}},
{"Mage", "Маг", {Mage}},
{"Paladin", "Паладин", {Paladin}, {HolyGrace}},
{"Ranger", "Следопыт", {Ranger}},
{"Theif", "Вор", {Theif}},
//
{"FighterCleric", "Воин-священник", {Fighter, Cleric}},
{"FighterMage", "Воин-маг", {Fighter, Mage}},
{"FighterTheif", "Воин-вор", {Fighter, Theif}},
{"ClericMage", "Священник-маг", {Cleric, Mage}},
{"MageTheif", "Маг-вор", {Mage, Theif}},
{"FighterClericMage", "Воин-священник-маг", {Fighter, Cleric, Mage}},
{"FighterMageTheif", "Воин-маг-вор", {Fighter, Mage, Theif}},
};