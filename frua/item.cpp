#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(type),
	BSREQ(power_name),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(armor),
	BSREQ(usability),
	BSREQ(resistance),
	BSREQ(threshold),
	BSREQ(abilities),
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
static bool ability_visibility(const void* source, const markup& e) {
	auto p = (item_info*)source;
	return bsmeta<wear_s>::data[p->type].use_ability;
}
static const char* wear_getname(const void* object, char* result, const char* result_max, int column) {
	switch(column) {
	case 0:return ((wear_info*)object)->name_type;
	}
	return "";
}
static markup deflection_markup[] = {{0, "/", {"toughness"}}, {}};
markup armor_info::form_element[] = {{0, "����� �����", {"ac", 0, deflection_markup}},
{0, "���������(%)", {"critical"}},
{}};
static markup multiplier_right[] = {{0, "X", {"multiplier"}}, {}};
markup damage_info::form_element[] = {{0, "��� �����", {"type"}},
{0, "�-�� ����", {"attacks"}},
{0, "�����", {"thac0"}},
{0, "���������", {"range"}},
{0, "�����������", {"critical", 0, multiplier_right}},
{0, "����", {"damage"}},
{0, "���� (�������)", {"damage_large"}},
{}};
static markup dice_markup_right[] = {{0, "d", {"d", 0, }}, {0, "+", {"m", 0, }}, {}};
static markup dice_markup[] = {{0, "����", {"c", 0, dice_markup_right}},
{}};
static plugin<markup> dice_markup_plugin(bsmeta<dice>::meta, dice_markup);
static markup weapon_block[] = {{0, 0, {"damage"}}, {}};
static markup armor_block[] = {{0, 0, {"armor"}}, {}};
static markup usability_block[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup resistance_block[] = {{0, "#dam", {"resistance"}, 3}, {}};
static markup threshold_block[] = {{0, "#dam", {"threshold"}, 3}, {}};
static markup basic_markup[] = {{0, "��������", {"name"}},
{0, "��� ����", {"power_name"}},
{0, "������", {"type"}, 0, {wear_getname, 0, allow_item}},
{0, "���� (�������)", {"cost"}, 6},
{0, "��� (������)", {"weight"}, 4},
{}};
static markup phisycal_addon[] = {{0, "/", {"abilities", 1}, 2}, {0, "/", {"abilities", 2}, 2}, {}};
static markup mental_addon[] = {{0, "/", {"abilities", 4}, 2}, {0, "/", {"abilities", 5}, 2}, {}};
static markup ability_block[] = {{0, "����������", {"abilities", 0, phisycal_addon}, 2},
{0, "����������", {"abilities", 3, mental_addon}, 2},
{}};
static markup column1[] = {{0, "������� ���������", {0, 0, basic_markup}},
{0, "�������������", {0, 0, usability_block}},
{}};
static markup column2[] = {{0, "�������������", {0, 0, resistance_block}},
{0, "����� �����", {0, 0, threshold_block}},
{}};
static markup column3[] = {{0, "������", {0, 0, weapon_block}, 0, {0, 0, 0, damage_visibility}},
{0, "�����", {0, 0, armor_block}, 0, {0, 0, 0, armor_visibility}},
{0, "��������", {0, 0, ability_block}, 0, {0, 0, 0, ability_visibility}},
{}};
markup item_info::form_element[] = {{4, 0, {0, 0, column1}},
{3, 0, {0, 0, column2}},
{5, 0, {0, 0, column3}},
{}};

const char* item_info::getname(const void* object, char* result, const char* result_max, int id) {
	auto p = (item_info*)object;
	switch(id) {
	case Name:
		if(!p->name)
			return "��� ��������";
		return p->name;
	default: return "";
	}
}

int item_info::getvalue(const void* object, int id) {
	auto p = (item_info*)object;
	switch(id) {
	case Avatar: return -1;
	case Grade:
		if(p->power_name)
			return Good;
		return Fair;
	default: return 0;
	}
}

decoration::command item_info::commands[] = {
{}};