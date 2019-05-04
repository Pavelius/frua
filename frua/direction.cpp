#include "draw.h"
#include "main.h"

direction_info bsmeta<direction_info>::elements[] = {{"Up", "Вверх", KeyUp},
{"Right", "Вправо", KeyRight},
{"Down", "Вниз", KeyDown},
{"Left", "Влево", KeyLeft},
{"Center", "Центр"},
};
assert_enum(direction, Center);