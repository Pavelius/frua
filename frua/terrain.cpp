#include "main.h"

terrain_info bsmeta<terrain_info>::elements[] = {{"Unpassable", "Непроходимая", 0, 0},
{"StandartMovement", "Обычное движение", 1, 1},
{"HalfMovement", "Движение х 1/2", 1, 2},
{"QuarterMovement", "Движение х 1/4", 1, 4},
};
assert_enum(terrain, QuarterMovement);