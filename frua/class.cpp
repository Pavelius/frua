#include "main.h"

class_info class_data[] = {{},
{"Cleric", "���������", {Cleric}},
{"Fighter", "����", {Fighter}},
{"Mage", "���", {Mage}},
{"Paladin", "�������", {Paladin}, {HolyGrace}},
{"Ranger", "��������", {Ranger}},
{"Theif", "���", {Theif}},
//
{"FighterCleric", "����-���������", {Fighter, Cleric}},
{"FighterMage", "����-���", {Fighter, Mage}},
{"FighterTheif", "����-���", {Fighter, Theif}},
{"ClericMage", "���������-���", {Cleric, Mage}},
{"MageTheif", "���-���", {Mage, Theif}},
{"FighterClericMage", "����-���������-���", {Fighter, Cleric, Mage}},
{"FighterMageTheif", "����-���-���", {Fighter, Mage, Theif}},
};