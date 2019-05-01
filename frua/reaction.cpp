#include "main.h"

reaction_info bsmeta<reaction_info>::elements[] = {{"Indifferent", "Нейтральная"},
{"Friendly", "Дружественная"},
{"Flight", "Трусливая"},
{"Cautions", "Внимательная"},
{"Threatening", "Угрожающая"},
{"Hostile", "Враждебная"},
{"Player", "Игрок"},
};
assert_enum(reaction, Player);