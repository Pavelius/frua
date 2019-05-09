#include "main.h"

item_state_info bsmeta<item_state_info>::elements[] = {{"Mundane", "Обычное"},
{"Cursed", "Проклятое"},
{"Magic", "Волшебное"},
{"Artifact", "Артифакт"},
};
assert_enum(item_state, Artifact);