#include "main.h"

usability_info usability_data[] = {{"UseLeatherArmor", "�������� �������"},
{"UseMetalArmor", "������������ �������"},
{"UseShield", "���"},
{"UseTheifWeapon", "������ �����"},
{"UseMartialWeapon", "������ ������"},
{"UseLargeWeapon", "������� ������"},
{"UseMagicDevice", "��������� ����������"},
{"UseScrolls", "������"},
};
assert_enum(usability, UseScrolls);
getstr_enum(usability);