#include "main.h"

save_info bsmeta<save_info>::elements[] = {{"NoSave", "Недоступно"},
{"SaveHalf", "Спасает наполовину"},
{"SaveNegate", "Спасает полностью"},
};
assert_enum(save, SaveNegate);