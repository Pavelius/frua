#include "main.h"

effect_info effect_data[] = {{"NoEffect", "�� ������������"},
{"Bludgeon", "�������"},
{"Piercing", "�������"},
{"Slashing", "�������"},
{"Acid", "���������"},
{"Cold", "��������", ResistCold},
{"Electricity", "�������������"},
{"Fire", "��������", ResistFire},
{"Death", "���������� ������"},
{"Petrification", "����������� � ������"},
{"Paralize", "�������"},
{"WeakPoison", "������ ��"},
{"Poison", "��"},
{"StrongPoison", "������� ��"},
{"DeathPoison", "����������� ��"},
};
assert_enum(effect, DeathPoison);
getstr_enum(effect);

