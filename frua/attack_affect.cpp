#include "main.h"

attack_affect_info bsmeta<attack_affect_info>::elements[] = {{"AttackRegular", "������ �� ���������, 1 �����", 2},
{"AttackOneAndHalf", "������ �� ���������, 3/2 �����", 3},
{"AttackDouble", "������ �� ���������, 2 �����", 4},
{"AttackAdditional", "� ���������� � ���������� �����"},
{"AttackOnAnyOneHit", "��� ��������� ����� �����"},
{"AttackOnAllHit", "��� ��������� ���� ����"},
{"AttackOnCritical", "��� ����������� ���������"},
};
assert_enum(attack_affect, AttackOnCritical);