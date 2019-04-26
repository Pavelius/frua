#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(type),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(armor),
	BSREQ(restricted),
{}};
BSDATA(item_info, 512);
const bsreq bsmeta<armor_info>::meta[] = {
	BSREQ(ac),
	BSREQ(critical),
{}};
const bsreq bsmeta<damage_info>::meta[] = {
	BSREQ(type),
	BSREQ(thac0),
	BSREQ(critical),
	BSREQ(multiplier),
	BSREQ(attacks),
	BSREQ(range),
	BSREQ(damage),
	BSREQ(damage_large),
{}};
const bsreq bsmeta<dice>::meta[] = {
	BSREQ(c),
	BSREQ(d),
	BSREQ(b),
	BSREQ(m),
{}};
static bool damage_visibility(const void* source, const markup& e) {
	auto p = (item_info*)source;
	return p->type == MeleeWeapon || p->type == RangedWeapon || p->type == OffhandWeapon;
}
static bool armor_visibility(const void* source, const markup& e) {
	auto p = (item_info*)source;
	return p->type == Armor || p->type == MeleeWeapon || p->type==OffhandWeapon;
}
static bool feat_allow(const void* source, int value) {
	return bsmeta<feat_s>::data[value].use_item!=0;
}
static markup column1[] = {{0, "Название", {"name"}},
{0, "Тип", {"type"}},
{0, "Цена (серебра)", {"cost"}, 6},
{0, "Вес (фунтов)", {"weight"}, 4},
{0, "#checkboxes", {"restricted"}, 0, {0, feat_allow}},
{0, "Урон", {"damage"}, 0, {damage_visibility}},
{0, "Доспехи", {"armor"}, 0, {armor_visibility}},
{}};
static markup item_markup[] = {{5, "Базовые параметры", {0, 0, column1}},
{}};
MARKUP(item);
static markup armor_markup[] = {{0, "Класс брони", {"ac"}},
{0, "Отражение", {"critical"}},
{}};
MARKUP(armor);
static markup damage_multiplier_part = {0, "X", {"multiplier"}};
static markup damage_markup[] = {{0, "Тип урона", {"type"}},
{0, "К-во атак", {"attacks"}},
{0, "Бонус", {"thac0"}},
{0, "Дистанция", {"range"}},
{0, "Критический", {"critical", 0, &damage_multiplier_part}},
{}};
MARKUP(damage);

bool item_info::edit() {
	return design_info::edit("Предметы", this, bsmeta<item_info>::meta, 0);
}