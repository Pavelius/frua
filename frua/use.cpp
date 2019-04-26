#include "main.h"

usability_info usability_data[] = {{"UseLeatherArmor", "Кожанные доспехи"},
{"UseMetalArmor", "Металические доспехи"},
{"UseShield", "Щит"},
{"UseTheifWeapon", "Оружие воров"},
{"UseMartialWeapon", "Ратное оружие"},
{"UseLargeWeapon", "Большое оружие"},
};
assert_enum(usability, UseLargeWeapon);
getstr_enum(usability);