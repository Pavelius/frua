#include "main.h"

static wear_info wear_data[] = {{"Head", "������"},
{"Neck", "���"},
{"Armor", "�����"},
{"MeleeWeapon", "������ �������� ���"},
{"OffhandWeapon", "������ ���������"},
{"RangedWeapon", "������������� ������"},
{"GridleWear", "����"},
{"LeftRing", "������ �� ����� ������"},
{"RightRing", "������ �� ������"},
{"Legs", "����"},
};
assert_enum(wear, LastWear);
getstr_enum(wear);