#include "main.h"

reaction_info reaction_data[] = {{"Indifferent", "�����������"},
{"Friendly", "�������������"},
{"Flight", "���������"},
{"Cautions", "������������"},
{"Threatening", "����������"},
{"Hostile", "����������"},
{"Player", "�����"},
};
assert_enum(reaction, Player);
getstr_enum(reaction);