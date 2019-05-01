#include "main.h"

BSENUMB(damage_feat) = {{"SaveHalf", "Спасает на полонину"},
{"NoSave", "Нет спасения"},
{"AttackAdditional", "Дополнительная"},
{"AttackOnCritical", "Критическая"},
};
assert_enum(damage_feat, AttackOnCritical);