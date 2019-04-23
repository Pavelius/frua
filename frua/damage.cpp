#include "main.h"

name_info damage_data[] = {{"Bludgeon", "Ударное"},
{"Piercing", "Колющее"},
{"Slashing", "Режущее"},
{"Acid", "Кислотное"},
{"Cold", "Холодное"},
{"Electricity", "Электрическое"},
{"Fire", "Огненное"},
};
assert_enum(damage, Fire);
getstr_enum(damage);