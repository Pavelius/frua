#include "main.h"

damage_feat_info damage_feat_data[] = {{"SaveHalf", "Спасает на полонину"},
{"NoSave", "Нет спасения"},
{"AttackAdditional", "Дополнительная"},
{"AttackOnCritical", "Критическая"},
};
assert_enum(damage_feat, AttackOnCritical);
getstr_enum(damage_feat);