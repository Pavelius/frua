#include "main.h"

BSENUMB(item_state) = {{"Mundane", "Обычное"},
{"Cursed", "Проклятое"},
{"Magic", "Волшебное"},
{"Artifact", "Артифакт"},
};
assert_enum(item_state, Artifact);