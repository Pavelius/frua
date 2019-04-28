#include "main.h"

dam_info dam_data[] = {{"Bludgeon", "Ударное"},
{"Piercing", "Колющее"},
{"Slashing", "Режущее"},
{"Acid", "Кислотное"},
{"Cold", "Холодное", ResistCold},
{"Electricity", "Электрическое"},
{"Fire", "Огненное", ResistFire},
};
assert_enum(dam, Fire);
getstr_enum(dam);