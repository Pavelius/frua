#include "main.h"

usability_info bsmeta<usability_info>::elements[] = {{"UseLeatherArmor", "Кожанные доспехи"},
{"UseMetalArmor", "Металические доспехи"},
{"UseShield", "Щит"},
{"UseTheifWeapon", "Оружие воров"},
{"UseElfWeapon", "Оружие эльфов"},
{"UseMartialWeapon", "Ратное оружие"},
{"UseLargeWeapon", "Большое оружие"},
{"UseMagicDevice", "Волшебное устройство"},
{"UseScrolls", "Свиток"},
};
assert_enum(usability, UseScrolls);