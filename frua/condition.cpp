#include "main.h"

condition_info bsmeta<condition_info>::elements[] = {{"AlwaysHappen", "������ ���������"},
{"PartyHasSpecialItem", "� ������ ���� ������ �������"},
{"PartyDoesNotHaveSpecialItem", "� ������ ��� ������� ��������"},
{"DayTime", "� ������� �����"},
{"NightTime", "� ������ �����"},
{"RandomPercentChance", "��������� ���� �������������"},
{"SpecifiedRaceInParty", "��������� ���� ���� � ������"},
{"SpecifiedClassInParty", "��������� ����� ���� � ������"},
};
assert_enum(condition, SpecifiedClassInParty);