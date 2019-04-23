#include "main.h"

dam_info dam_data[] = {{"Bludgeon", "Ударное"},
{"Piercing", "Колющее"},
{"Slashing", "Режущее"},
{"Acid", "Кислотное"},
{"Cold", "Холодное"},
{"Electricity", "Электрическое"},
{"Fire", "Огненное"},
};
assert_enum(dam, Fire);
getstr_enum(dam);