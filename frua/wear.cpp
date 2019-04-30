#include "main.h"

static wear_info wear_data[] = {{"Head", "Голова", "Головные уборы", {Head, Backpack}, StandartItem, 0, 1},
{"Neck", "Шея", "Амулеты", {Neck, Backpack}, StandartItem, 0, 1},
{"Armor", "Броня", "Доспехи", {Armor, Backpack}, StandartItem, 0, 1},
{"MeleeWeapon", "Оружие ближнего боя", "Оружие ближнего боя", {MeleeWeapon, Backpack}, StandartItem, 1, 1},
{"OffhandWeapon", "Оружие поддержки", "Оружие поддержки", {OffhandWeapon, Backpack}, StandartItem, 1, 1},
{"RangedWeapon", "Дистанционное оружие", "Дистанционное оружие", {RangedWeapon, Backpack}, StandartItem, 1, 0},
{"GridleWear", "Пояс", "Пояса", {GridleWear, Backpack}, StandartItem, 0, 1},
{"LeftRing", "Кольцо на левом пальце", 0, {LeftRing, RightRing}, StandartItem, 0, 1},
{"RightRing", "Кольцо на пальце", "Кольца", {LeftRing, RightRing}, StandartItem, 0, 1},
{"Legs", "Ноги", "Обувь", {Legs, Backpack}, StandartItem, 0, 1},
{"Backpack", "Рюкзак", 0, {Backpack, Backpack}},
{"Book", "Книга", "Книги и тома", {Backpack, Backpack}},
{"Wand", "Волшебное устройство", "Волшебные устройства", {Backpack, Backpack}, ChargeableItem},
{"Scroll", "Свиток", "Свитки", {Backpack, Backpack}},
{"Potion", "Зелье", "Зелья", {Backpack, Backpack}},
{"Edible", "Еда", "Еда", {Backpack, Backpack}},
{"Gem", "Драгоценность", "Драгоценности", {Backpack, Backpack}},
};
assert_enum(wear, LastWear);
getstr_enum(wear);