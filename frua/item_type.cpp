#include "main.h"

BSENUMB(item_type) = {{"Weapon", "������", 1, 1},
{"Armor", "������", 0, 1},
{"Shield", "���", 0, 1},
{"Helm", "����", 0, 1},
{"Gridle", "����", 0, 1},
//
{"Potion", "�����"},
{"Oil", "�����"},
{"Scroll", "������"},
{"Ring", "������", 0, 1},
{"Rod", "������"},
{"Wand", "�������"},
{"Book", "������"},
{"Jewelry", "�������������"},
{"Cloack", "����", 0, 1},
{"Robe", "����", 0, 1},
{"Boots", "������", 0, 1},
{"Gloves", "��������", 0, 1},
};
assert_enum(item_type, Gloves);