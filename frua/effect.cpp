#include "main.h"

effect_info bsmeta<effect_info>::elements[] = {{"NoEffect", "�� ������������"},
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

