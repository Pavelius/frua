#include "main.h"

usability_info usability_data[] = {{"UseLeatherArmor", "�������� �������"},
{"UseMetalArmor", "������������ �������"},
{"UseShield", "���"},
{"UseTheifWeapon", "������ �����"},
{"UseMartialWeapon", "������ ������"},
{"UseLargeWeapon", "������� ������"},
};
assert_enum(usability, UseLargeWeapon);
getstr_enum(usability);