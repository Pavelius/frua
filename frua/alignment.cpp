#include "main.h"

alignment_info alignment_data[] = {{"Lawful Good", "��������������� ������", {Theif}},
{"Neutral Good", "���������� ������", {Paladin}},
{"Chaotic Good", "�������� ������", {Paladin}},
{"Lawful Neutral", "��������������� �����������", {Theif, Ranger, Paladin}},
{"Neutral", "�����������", {Ranger, Paladin}},
{"Chaotic Neutral", "�������� �����������", {Ranger, Paladin}},
{"Lawful Evil", "��������������� ����", {Theif, Ranger, Paladin}},
{"Neutral Evil", "���������� ����", {Ranger, Paladin}},
{"Chaotic Evil", "�������� ����", {Ranger, Paladin}},
};
assert_enum(alignment, ChaoticEvil);
getstr_enum(alignment);