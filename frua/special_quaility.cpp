#include "main.h"

special_quality_info bsmeta<special_quality_info>::elements[] = {{"HitMagicWeapon", "Поражается только оружием +1 и лучше"},
{"HitSilverWeapon", "Поражается только серебрянным оружием"},
{"HalfDamagePierce", "Половина урона от колющих атак"},
{"HalfDamageSlashing", "Половина урона от режущих атак"},
{"HalfDamageBludgeon", "Половина урона от ударных атак"},
{"Regeneration", "Регенерация 3 хита за раунд"},
};
assert_enum(special_quality, Regeneration);