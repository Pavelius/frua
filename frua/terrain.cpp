#include "main.h"

terrain_info bsmeta<terrain_info>::elements[] = {{"Unpassable", "������������", 0, 0},
{"StandartMovement", "������� ��������", 1, 1},
{"HalfMovement", "�������� � 1/2", 1, 2},
{"QuarterMovement", "�������� � 1/4", 1, 4},
};
assert_enum(terrain, QuarterMovement);