#include "main.h"

effect_info effect_data[] = {{"NoEffect", "�� ������������"},
{"Death", "���������� ������"},
{"Petrification", "����������� � ������"},
{"AdditionalDamage", "�������������� �����������"},
{"WeakPoison", "������ ��"},
{"Poison", "��"},
{"StrongPoison", "������� ��"},
{"DeathPoison", "����������� ��"},
};
assert_enum(effect, DeathPoison);
getstr_enum(effect);

