#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(undefined),
	BSREQ(type),
	BSREQ(magic),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(armor),
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
static markup column1[] = {{0, "��������", {"name"}},
{0, "������������", {"undefined"}},
{0, "���", {"type"}},
{0, "���� (�������)", {"cost"}, 6},
{0, "��� (������)", {"weight"}, 4},
{0, "�����", {"magic"}},
{0, "����", {"damage"}, 0, {damage_visibility}},
{0, "�������", {"armor"}, 0, {armor_visibility}},
{}};
static markup item_markup[] = {{5, 0, {0, 0, column1}},
{}};
MARKUP(item);
static markup armor_markup[] = {{0, "����� �����", {"ac"}},
{0, "���������", {"critical"}},
{}};
MARKUP(armor);
static markup damage_multiplier_part = {0, "X", {"multiplier"}};
static markup damage_markup[] = {{0, "��� �����", {"type"}},
{0, "�-�� ����", {"attacks"}},
{0, "�����", {"thac0"}},
{0, "���������", {"range"}},
{0, "�����������", {"critical", 0, &damage_multiplier_part}},
{}};
MARKUP(damage);

bool item_info::edit() {
	return design_info::edit("��������", this, bsmeta<item_info>::meta, 0);
}