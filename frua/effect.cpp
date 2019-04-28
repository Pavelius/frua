#include "main.h"

effect_info effect_data[] = {{"NoEffect", "Не используется"},
{"Death", "Мгновенная смерть"},
{"Petrification", "Превращение в камень"},
{"AdditionalDamage", "Дополнительные повреждения"},
{"WeakPoison", "Слабый яд"},
{"Poison", "Яд"},
{"StrongPoison", "Сильный яд"},
{"DeathPoison", "Смертельный яд"},
};
assert_enum(effect, DeathPoison);
getstr_enum(effect);

