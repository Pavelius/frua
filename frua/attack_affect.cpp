#include "main.h"

attack_affect_info bsmeta<attack_affect_info>::elements[] = {{"AttackRegular", "Бросок на попадание, 1 атака", 2},
{"AttackOneAndHalf", "Бросок на попадание, 3/2 атаки", 3},
{"AttackDouble", "Бросок на попадание, 2 атаки", 4},
{"AttackAdditional", "В дополнение к предыдущей атаке"},
{"AttackOnAnyOneHit", "При попадании любой атаки"},
{"AttackOnAllHit", "При попадании всех атак"},
{"AttackOnCritical", "При критическом попадании"},
};
assert_enum(attack_affect, AttackOnCritical);