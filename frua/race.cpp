#include "main.h"

race_info bsmeta<race_info>::elements[] = {{"Human", "�������", 1, {3, 3, 3, 3, 3, 3}, {18, 18, 18, 18, 18, 18}, {0, 0, 0, 0, 0, 0}, {0},
{}, {UseLargeWeapon}, Medium, 12,
"���� ���� � ���� AD&D ��������������� ��� ��������� ����, ��� ���������� �� ���� ��������������, ��������� � �������� ����. ������� ��������-������� ����� �������� ������ �������� ����������������, ������� ��������� ������."},
//
{"Dwarf", "������", 1, {8, 3, 12, 3, 3, 3}, {18, 17, 18, 18, 18, 17}, {0, 0, 1, 0, 0, -1},
{0, 10, 15, 0, 0, 0, -10, -5},
{Darkvision, BonusSaveVsPoison, BonusSaveVsSpells, BonusSaveVsWands, BonusToHitOrcs, SmallSizeCombatAdvantage, DetectUndegroundPassages}, {UseLargeWeapon}, Medium, 12,
"������� - ��� ���������, ���������� ��������, ������� ����� �������� �� �� ������� � �������� ����. �� ���� ���������� �������� 120-140 ��. � ��� ������� ����, ������ ����� � ������ ������. ������� � �������� ����� 350 ���."},
//
{"Elf", "����", 1, {3, 6, 7, 8, 3, 8}, {18, 18, 18, 18, 18, 18}, {0, 1, -1, 0, 0, 0},
{5, -5, 0, 5, 10, 5, 0, 0},
{Darkvision, CharmResistance90, ElfWeaponTraining, LightSteps, DetectSecretDoors}, {UseLargeWeapon}, Medium, 12,
"�����, ��� �������, ��������� ���� � �������� ������� �����. �� ����� ������ ������ � ���������, ��� ������������� � ����������� �����������. ���� ��� � ������� �������� � �������, ������������� ���� ���� �������� � ������. ����� ������ ����� ����� 1200 ���, ���� ������� �� ����� ������� ��� ��������� ���������� �������� ��� ��������. ����������, ���� ��� ������, �� ��� ����������� ���������� �� ����."},
//
{"Gnome", "����", 1, {6, 3, 8, 6, 3, 3}, {18, 18, 18, 18, 18, 18}, {0, 0, 0, 1, -1, 0},
{0, 5, 10, 5, 5, 10, -15, 0},
{BonusSaveVsSpells, BonusSaveVsWands, BonusToHitOrcs, SmallSizeCombatAdvantage, DetectUndegroundPassages}, {UseLargeWeapon}, Small, 12,
"�������� � ������� � ���������, ����� ������� ������ ������, ��� �� ������� ������������. �����, ��� ����� ����� ����������, ��� ����� �������, ��� �������. ������ �� ���� ����������� ������. � ����������� ������ ������ ���������-���������� ��� ���������� ���� � ����� ������. �������� ���� ����� 350 ���."},
//
{"Halfelf", "����-����", 1, {3, 6, 6, 4, 3, 3}, {18, 18, 18, 18, 18, 18}, {0, 0, 0, 0, 0, 0},
{10, 0, 0, 0, 5, 0, 0, 0},
{Darkvision, CharmResistance30, DetectSecretDoors}, {UseLargeWeapon}, Medium, 12,
"��������� - ��� �������� ���������������� ��������, ����������� � ��������� ����. �������������� � ������, ���������� ��� ����� ������������ ��������� �������: ������, ����� ������� �������� ���� � �����, � ����, �������� ���� ���������, ���� ���������� (����� ������� ������ ����� ���� ������ �����). ���� ����� ������� ������ �����, ��� ������, �������� �������� ���������, ���� ������� ������� ��� ������ ����� ��� ������, �������� �������� ����������."},
//
{"Halfling", "������", 1, {7, 7, 10, 6, 3, 3}, {18, 18, 18, 18, 17, 18}, {-1, 1, 0, 0, 0, 0},
{5, 5, 5, 10, 15, 5, -15, -5},
{BonusSaveVsSpells, BonusSaveVsWands, BonusSaveVsPoison, LightSteps, NoExeptionalStrenght}, {}, Small, 12,
"������� - ��� ����������� ���������� ��������, ����� ������� �� ��������� �����. � ��� �������� ������� ����, ����� ������� ������ ������� ����. � �������� ������ ��������� ��������, � ������ ����� �� ��� ������� ������� ��������. ��� ������������ �� ������ �����, ��������� ��� ��������. �� ������������ ���� �����, �������� ���������� ����� 150 ���."},
//
{"Goblinoid", "���������", 0, {3, 3, 3, 3, 3, 3}, {18, 18, 18, 18, 18, 18}, {0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{Darkvision}, {UseLargeWeapon}, Medium, 12,
""},
//
{"Humanoid", "��������", 0, {3, 3, 3, 3, 3, 3}, {25, 25, 25, 20, 20, 20}, {0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{}, {UseLargeWeapon}, Medium, 12,
""},
//
{"Animal", "��������", 0, {3, 3, 3, 3, 3, 3}, {25, 25, 25, 3, 15, 15}, {0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{Darkvision}, {}, Medium, 12,
""},
//
{"Insectoid", "���������", 0, {3, 3, 3, 1, 3, 3}, {25, 25, 25, 1, 10, 10}, {0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{Darkvision}, {}, Medium, 12,
""},
};
assert_enum(race, Insectoid);