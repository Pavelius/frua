#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(type),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(armor),
	BSREQ(usability),
	BSREQ(resistance),
	BSREQ(threshold),
{}};
BSDATA(item_info, 512);
const bsreq bsmeta<armor_info>::meta[] = {
	BSREQ(ac),
	BSREQ(toughness),
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
static bool allow_item(const void* source, int value) {
	return bsmeta<wear_s>::data[value].name_type != 0;
}
static bool damage_visibility(const void* source, const markup& e) {
	auto p = (item_info*)source;
	return bsmeta<wear_s>::data[p->type].use_damage;
}
static bool armor_visibility(const void* source, const markup& e) {
	auto p = (item_info*)source;
	return bsmeta<wear_s>::data[p->type].use_armor;
}
static markup deflection_markup[] = {{0, "/", {"toughness"}}, {}};
static markup armor_markup[] = {{0, "Класс брони", {"ac", 0, deflection_markup}},
{0, "Отражение(%)", {"critical"}},
{}};
MARKUP(armor);
static markup multiplier_right[] = {{0, "X", {"multiplier"}}, {}};
static markup damage_markup[] = {{0, "Тип урона", {"type"}},
{0, "К-во атак", {"attacks"}},
{0, "Бонус", {"thac0"}},
{0, "Дистанция", {"range"}},
{0, "Критический", {"critical", 0, multiplier_right}},
{0, "Урон", {"damage"}},
{0, "Урон (большим)", {"damage_large"}},
{}};
MARKUP(damage);
static markup dice_markup_right[] = {{0, "d", {"d", 0, }}, {0, "+", {"m", 0, }}, {}};
static markup dice_markup[] = {{0, "Урон", {"c", 0, dice_markup_right}},
{}};
static plugin<markup> dice_markup_plugin(bsmeta<dice>::meta, dice_markup);
static markup weapon_block[] = {{0, 0, {"damage"}}, {}};
static markup armor_block[] = {{0, 0, {"armor"}}, {}};
static markup usability_block[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup resistance_block[] = {{0, "#dam", {"resistance"}, 3}, {}};
static markup threshold_block[] = {{0, "#dam", {"threshold"}, 3}, {}};
static markup basic_markup[] = {{0, "Название", {"name"}},
{0, "Тип", {"type"}, 0, {0, allow_item}},
{0, "Цена (серебра)", {"cost"}, 6},
{0, "Вес (фунтов)", {"weight"}, 4},
{}};
static markup column1[] = {{0, "Базовые параметры", {0, 0, basic_markup}},
{0, "Использование", {0, 0, usability_block}},
{}};
static markup column2[] = {{0, "Сопротивление", {0, 0, resistance_block}},
{0, "Порог урона", {0, 0, threshold_block}},
{}};
static markup column3[] = {{0, "Оружие", {0, 0, weapon_block}, 0, {damage_visibility}},
{0, "Броня", {0, 0, armor_block}, 0, {armor_visibility}},
{}};
static markup item_markup[] = {{4, 0, {0, 0, column1}},
{3, 0, {0, 0, column2}},
{5, 0, {0, 0, column3}},
{}};
MARKUP(item);

bool item_info::edit() {
	return decoration::edit("Предметы", this, sizeof(*this), bsmeta<item_info>::meta);
}