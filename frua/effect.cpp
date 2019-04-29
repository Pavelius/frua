#include "main.h"

effect_info effect_data[] = {{"NoEffect", "Не используется"},
{"Bludgeon", "Ударное"},
{"Piercing", "Колющее"},
{"Slashing", "Режущее"},
{"Acid", "Кислотное"},
{"Cold", "Холодное", ResistCold},
{"Electricity", "Электрическое"},
{"Fire", "Огненное", ResistFire},
{"Death", "Мгновенная смерть"},
{"Petrification", "Превращение в камень"},
{"Paralize", "Паралич"},
{"WeakPoison", "Слабый яд"},
{"Poison", "Яд"},
{"StrongPoison", "Сильный яд"},
{"DeathPoison", "Смертельный яд"},
};
assert_enum(effect, DeathPoison);
getstr_enum(effect);

