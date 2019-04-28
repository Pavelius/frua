#include "main.h"

reaction_info reaction_data[] = {{"Indifferent", "Нейтральная"},
{"Friendly", "Дружественная"},
{"Flight", "Трусливая"},
{"Cautions", "Внимательная"},
{"Threatening", "Угрожающая"},
{"Hostile", "Враждебная"},
{"Player", "Игрок"},
};
assert_enum(reaction, Player);
getstr_enum(reaction);