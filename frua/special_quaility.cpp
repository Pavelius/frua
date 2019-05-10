#include "main.h"

special_quality_info bsmeta<special_quality_info>::elements[] = {{"ImmunityDesease", "Иммунитет к болезням"},
{"ImmunityPoison", "Иммунитет к яду"},
{"ImmunityCharm", "Иммунитет к шарму и сну"},
{"ImmunityCold", "Иммунитет к холоду"},
{"ImmunityFire", "Иммунитет к огню"},
{"HitMagicWeapon", "Поражается оружием +1 и лучше"},
{"HitSilverWeapon", "Поражается серебрянным оружием"},
{"HalfDamagePierce", "Половина урона от колющих атак"},
{"HalfDamageSlashing", "Половина урона от режущих атак"},
{"HalfDamageBludgeon", "Половина урона от ударных атак"},
{"Regeneration", "Регенерация 3 хита за раунд"},
{"Unded", "Мертвец"},
};
assert_enum(special_quality, Undead);