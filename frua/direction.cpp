#include "draw.h"
#include "main.h"

direction_info bsmeta<direction_info>::elements[] = {{"Up", "�����", KeyUp},
{"Right", "������", KeyRight},
{"Down", "����", KeyDown},
{"Left", "�����", KeyLeft},
{"Center", "�����"},
};
assert_enum(direction, Center);