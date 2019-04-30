#include "main.h"

static wear_info wear_data[] = {{"Head", "������", "�������� �����", {Head, Backpack}, StandartItem, 0, 1},
{"Neck", "���", "�������", {Neck, Backpack}, StandartItem, 0, 1},
{"Armor", "�����", "�������", {Armor, Backpack}, StandartItem, 0, 1},
{"MeleeWeapon", "������ �������� ���", "������ �������� ���", {MeleeWeapon, Backpack}, StandartItem, 1, 1},
{"OffhandWeapon", "������ ���������", "������ ���������", {OffhandWeapon, Backpack}, StandartItem, 1, 1},
{"RangedWeapon", "������������� ������", "������������� ������", {RangedWeapon, Backpack}, StandartItem, 1, 0},
{"GridleWear", "����", "�����", {GridleWear, Backpack}, StandartItem, 0, 1},
{"LeftRing", "������ �� ����� ������", 0, {LeftRing, RightRing}, StandartItem, 0, 1},
{"RightRing", "������ �� ������", "������", {LeftRing, RightRing}, StandartItem, 0, 1},
{"Legs", "����", "�����", {Legs, Backpack}, StandartItem, 0, 1},
{"Backpack", "������", 0, {Backpack, Backpack}},
{"Book", "�����", "����� � ����", {Backpack, Backpack}},
{"Wand", "��������� ����������", "��������� ����������", {Backpack, Backpack}, ChargeableItem},
{"Scroll", "������", "������", {Backpack, Backpack}},
{"Potion", "�����", "�����", {Backpack, Backpack}},
{"Edible", "���", "���", {Backpack, Backpack}},
{"Gem", "�������������", "�������������", {Backpack, Backpack}},
};
assert_enum(wear, LastWear);
getstr_enum(wear);