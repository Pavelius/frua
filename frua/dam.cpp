#include "main.h"

dam_info dam_data[] = {{"Bludgeon", "�������"},
{"Piercing", "�������"},
{"Slashing", "�������"},
{"Acid", "���������"},
{"Cold", "��������", ResistCold},
{"Electricity", "�������������"},
{"Fire", "��������", ResistFire},
};
assert_enum(dam, Fire);
getstr_enum(dam);