#include "main.h"

static wear_info wear_data[] = {{"Head", "Голова"},
{"Neck", "Шея"},
{"Armor", "Броня"},
{"MeleeWeapon", "Оружие ближнего боя"},
{"OffhandWeapon", "Оружие поддержки"},
{"RangedWeapon", "Дистанционное оружие"},
{"GridleWear", "Пояс"},
{"LeftRing", "Кольцо на левом пальце"},
{"RightRing", "Кольцо на пальце"},
{"Legs", "Ноги"},
};
assert_enum(wear, LastWear);
getstr_enum(wear);