#include "main.h"

static item_state_info item_state_data[] = {{"Mundane", "�������"},
{"Cursed", "���������"},
{"Magic", "���������"},
{"Artifact", "��������"},
};
assert_enum(item_state, Artifact);
getstr_enum(item_state);