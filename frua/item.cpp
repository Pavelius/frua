#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(type),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(armor),
	BSREQ(usability),
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
static markup deflection_markup[] = {{0, "���������", {"critical"}}, {}};
static markup armor_markup[] = {{0, "����� �����", {"ac", 0, deflection_markup}},
{}};
MARKUP(armor);
static markup multiplier_right[] = {{0, "X", {"multiplier"}}, {}};
static markup damage_markup[] = {{0, "��� �����", {"type"}},
{0, "�-�� ����", {"attacks"}},
{0, "�����", {"thac0"}},
{0, "���������", {"range"}},
{0, "�����������", {"critical", 0, multiplier_right}},
{0, "����", {"damage"}},
{0, "���� (�������)", {"damage_large"}},
{}};
MARKUP(damage);
static markup dice_markup_right[] = {{0, "d", {"d", 0, }}, {0, "+", {"m", 0, }}, {}};
static markup dice_markup[] = {{0, "����", {"c", 0, dice_markup_right}},
{}};
static plugin<markup> dice_markup_plugin(bsmeta<dice>::meta, dice_markup);
static markup weapon_block[] = {{0, 0, {"damage"}}, {}};
static markup armor_block[] = {{0, 0, {"armor"}}, {}};
static markup usability_block[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup column1[] = {{0, "��������", {"name"}},
{0, "���", {"type"}},
{0, "���� (�������)", {"cost"}, 6},
{0, "��� (������)", {"weight"}, 4},
{}};
static markup column2[] = {{0, "�������������", {0, 0, usability_block}},
{}};
static markup column3[] = {{0, "������", {0, 0, weapon_block}, 0, {damage_visibility}},
{0, "�����", {0, 0, armor_block}, 0, {armor_visibility}},
{}};
static markup item_markup[] = {{4, "������� ���������", {0, 0, column1}},
{3, 0, {0, 0, column2}},
{5, 0, {0, 0, column3}},
{}};
MARKUP(item);

bool item_info::edit() {
	return decoration::edit("��������", this, sizeof(*this), bsmeta<item_info>::meta);
}