#include "main.h"

usability_info usability_data[] = {{"UseLeatherArmor", "Кожанные доспехи"},
{"UseMetalArmor", "Металические доспехи"},
{"UseShield", "Щит"},
{"UseTheifWeapon", "Оружие воров"},
{"UseMartialWeapon", "Ратное оружие"},
{"UseLargeWeapon", "Большое оружие"},
{"UseMagicDevice", "Волшебное устройство"},
{"UseScrolls", "Свиток"},
};
assert_enum(usability, UseScrolls);
getstr_enum(usability);