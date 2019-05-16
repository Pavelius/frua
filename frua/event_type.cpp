#include "main.h"

event_type_info bsmeta<event_type_info>::elements[] = {{"NoEvent", "Нет события"},
{"Combat", "Бой"},
};
assert_enum(event_type, Combat);