#include "main.h"

static wear_info wear_data[] = {{"Head", "������", "�������� ����", {Head, Backpack}, 0, 1},
{"Neck", "���", "������", {Neck, Backpack}, 0, 1},
{"Armor", "�����", "������", {Armor, Backpack}, 0, 1},
{"MeleeWeapon", "������ �������� ���", "������ �������� ���", {MeleeWeapon, Backpack}, 1, 1},
{"OffhandWeapon", "������ ���������", "������ ���������", {OffhandWeapon, Backpack}, 1, 1},
{"RangedWeapon", "������������� ������", "������������� ������", {RangedWeapon, Backpack}, 1, 0},
{"GridleWear", "����", "����", {GridleWear, Backpack}, 0, 1},
{"LeftRing", "������ �� ����� ������", 0, {LeftRing, RightRing}, 0, 1},
{"RightRing", "������ �� ������", "������", {LeftRing, RightRing}, 0, 1},
{"Legs", "����", "�����", {Legs, Backpack}, 0, 1},
{"Backpack", "������", 0, {Backpack, Backpack}},
{"Book", "�����", "�����", {Backpack, Backpack}},
{"Wand", "��������� ����������", "��������� ����������", {Backpack, Backpack}},
{"Scroll", "������", "������", {Backpack, Backpack}},
{"Potion", "�����", "�����", {Backpack, Backpack}},
{"Edible", "���", "���", {Backpack, Backpack}},
{"Gem", "�������������", "�������������", {Backpack, Backpack}},
};
assert_enum(wear, LastWear);
getstr_enum(wear);