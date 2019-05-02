#include "main.h"

BSENUMB(item_type) = {{"Weapon", "Оружие", 1, 1},
{"Armor", "Доспех", 0, 1},
{"Shield", "Щит", 0, 1},
{"Helm", "Шлем", 0, 1},
{"Gridle", "Пояс", 0, 1},
//
{"Potion", "Зелье"},
{"Oil", "Масло"},
{"Scroll", "Свиток"},
{"Ring", "Кольцо", 0, 1},
{"Rod", "Прутик"},
{"Wand", "Палочка"},
{"Book", "Книжка"},
{"Jewelry", "Драгоценность"},
{"Cloack", "Плащ", 0, 1},
{"Robe", "Ряса", 0, 1},
{"Boots", "Сапоги", 0, 1},
{"Gloves", "Перчатки", 0, 1},
};
assert_enum(item_type, Gloves);