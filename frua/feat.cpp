#include "main.h"

feat_info feat_data[] = {{"Darkvision", "����� � �������"},
{"BonusSaveVsPoison", "����� � ������ �� ���"},
{"BonusSaveVsWands", "����� � ������ �� �������"},
{"BonusSaveVsSpells", "����� � ������ �� ����������"},
{"BonusHits", "�������������� ����"},
{"DetectSecretDoors", "������� ��������� �����"},
{"DetectUndegroundPassages", "������� ��������� �������"},
{"CharmResistance", "90% ������������� ���������� � ���"},
{"ElfWeaponTraining", "+1 � ����� � ������ ��� ������"},
{"BonusToHitOrcs", "+1 � ����� �� �����������"},
{"SmallSizeCombatAdvantage", "+4 � �� ��� ������ �� ��������"},
{"LightSteps", "-10% � �������������� ���������"},
{"HolyGrace", "+10% �� ���� ������� ������"},
{"NoExeptionalStrenght", "�� ���������� �������������� ����"},
{"UniqueCharacter", "���������� ��������"},
};
assert_enum(feat, UniqueCharacter);
getstr_enum(feat);