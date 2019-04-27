#include "main.h"

static wear_info wear_data[] = {{"Head", "Голова", "Головной убор", {Head, Backpack}, 0, 1},
{"Neck", "Шея", "Амулет", {Neck, Backpack}, 0, 1},
{"Armor", "Броня", "Доспех", {Armor, Backpack}, 0, 1},
{"MeleeWeapon", "Оружие ближнего боя", "Оружие ближнего боя", {MeleeWeapon, Backpack}, 1, 1},
{"OffhandWeapon", "Оружие поддержки", "Оружие поддержки", {OffhandWeapon, Backpack}, 1, 1},
{"RangedWeapon", "Дистанционное оружие", "Дистанционное оружие", {RangedWeapon, Backpack}, 1, 0},
{"GridleWear", "Пояс", "Пояс", {GridleWear, Backpack}, 0, 1},
{"LeftRing", "Кольцо на левом пальце", 0, {LeftRing, RightRing}, 0, 1},
{"RightRing", "Кольцо на пальце", "Кольцо", {LeftRing, RightRing}, 0, 1},
{"Legs", "Ноги", "Обувь", {Legs, Backpack}, 0, 1},
{"Backpack", "Рюкзак", 0, {Backpack, Backpack}},
{"Book", "Книга", "Книга", {Backpack, Backpack}},
{"Wand", "Волшебное устройство", "Волшебное устройство", {Backpack, Backpack}},
{"Scroll", "Свиток", "Свиток", {Backpack, Backpack}},
{"Potion", "Зелье", "Зелье", {Backpack, Backpack}},
{"Edible", "Еда", "Еда", {Backpack, Backpack}},
{"Gem", "Драгоценность", "Драгоценность", {Backpack, Backpack}},
};
assert_enum(wear, LastWear);
getstr_enum(wear);