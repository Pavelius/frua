#include "main.h"

BSENUMB(effect) = {{"NoEffect", "�� ������������"},
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

