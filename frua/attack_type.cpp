#include "main.h"

attack_type_info attack_type_data[] = {{"NoSave", "��� ����� �� ������"},
{"SaveNegates", "������������ ������ ��������"},
{"SaveHalf", "������ �������� ��������� ���� ���������"},
{"StandartAttack", "������� ������ �����"},
};
assert_enum(attack_type, NoSave);
getstr_enum(attack_type);