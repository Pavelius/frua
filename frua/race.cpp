#include "main.h"

race_info race_data[] = {{"Human", "�������", {3, 3, 3, 3, 3, 3}, {18, 18, 18, 18, 18, 18}, {0, 0, 0, 0, 0, 0}, {0},
{},
"���� ���� � ���� AD&D ��������������� ��� ��������� ����, ��� ���������� �� ���� ��������������, ��������� � �������� ����. ������� ��������-������� ����� �������� ������ �������� ����������������, ������� ��������� ������."},
//
{"Dwarf", "������", {8, 3, 12, 3, 3, 3}, {18, 17, 18, 18, 18, 17}, {0, 0, 1, 0, 0, -1},
{0, 10, 15, 0, 0, 0, -10, -5},
{Darkvision, BonusSaveVsPoison, BonusSaveVsSpells, BonusSaveVsWands, DwarfCombatTactic, DetectUndegroundPassages},
"������� - ��� ���������, ���������� ��������, ������� ����� �������� �� �� ������� � �������� ����. �� ���� ���������� �������� 120-140 ��. � ��� ������� ����, ������ ����� � ������ ������. ������� � �������� ����� 350 ���."},
//
{"Elf", "����", {3, 6, 7, 8, 3, 8}, {18, 18, 18, 18, 18, 18}, {0, 1, -1, 0, 0, 0},
{5, -5, 0, 5, 10, 5, 0, 0},
{Darkvision, CharmResistance, ElfWeaponTraining, LightSteps},
"�����, ��� �������, ��������� ���� � �������� ������� �����. �� ����� ������ ������ � ���������, ��� ������������� � ����������� �����������. ���� ��� � ������� �������� � �������, ������������� ���� ���� �������� � ������. ����� ������ ����� ����� 1200 ���, ���� ������� �� ����� ������� ��� ��������� ���������� �������� ��� ��������. ����������, ���� ��� ������, �� ��� ����������� ���������� �� ����."},
//
{"Gnome", "����", {6, 3, 8, 6, 3, 3}, {18, 18, 18, 18, 18, 18}, {0, 0, 0, 1, -1, 0},
{0, 5, 10, 5, 5, 10, -15, 0},
{BonusSaveVsSpells, BonusSaveVsWands, DwarfCombatTactic, DetectUndegroundPassages},
"�������� � ������� � ���������, ����� ������� ������ ������, ��� �� ������� ������������. �����, ��� ����� ����� ����������, ��� ����� �������, ��� �������. ������ �� ���� ����������� ������. � ����������� ������ ������ ���������-���������� ��� ���������� ���� � ����� ������. �������� ���� ����� 350 ���."},
//
{"Halfelf", "����-����", {3, 6, 6, 4, 3, 3}, {18, 18, 18, 18, 18, 18}, {0, 0, 0, 0, 0, 0},
{10, 0, 0, 0, 5, 0, 0, 0},
{Darkvision},
"��������� - ��� �������� ���������������� ��������, ����������� � ��������� ����. �������������� � ������, ���������� ��� ����� ������������ ��������� �������: ������, ����� ������� �������� ���� � �����, � ����, �������� ���� ���������, ���� ���������� (����� ������� ������ ����� ���� ������ �����). ���� ����� ������� ������ �����, ��� ������, �������� �������� ���������, ���� ������� ������� ��� ������ ����� ��� ������, �������� �������� ����������."},
//
{"Halfling", "������", {7, 7, 10, 6, 3, 3}, {18, 18, 18, 18, 17, 18}, {-1, 1, 0, 0, 0, 0},
{5, 5, 5, 10, 15, 5, -15, -5},
{LightSteps},
"������� - ��� ����������� ���������� ��������, ����� ������� �� ��������� �����. � ��� �������� ������� ����, ����� ������� ������ ������� ����. � �������� ������ ��������� ��������, � ������ ����� �� ��� ������� ������� ��������. ��� ������������ �� ������ �����, ��������� ��� ��������. �� ������������ ���� �����, �������� ���������� ����� 150 ���."},
};
assert_enum(race, Halfling);
getstr_enum(race);