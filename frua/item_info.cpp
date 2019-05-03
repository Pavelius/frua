#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(name_unidentified),
	BSREQ(type),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(armor),
	BSREQ(usability),
	BSREQ(abilities),
	BSREQ(skills),
{}};
BSDATA(item_info, 256);

static bool hasdamage(const void* object, int index) {
	auto p = (item_info*)object;
	return bsmeta<item_type_s>::data[p->type].use_damage;
}
bool hasarmor(const void* object, int index) {
	auto p = (item_info*)object;
	return bsmeta<item_type_s>::data[p->type].use_armor;
}
bool hasability(const void* object, int index) {
	return hasarmor(object, index) && !hasdamage(object, index);
}
static markup weapon_block[] = {{0, 0, {"damage"}, 0, {0, 0, 0, 0, item_info::view_weapon}},
{}};
static markup armor_block[] = {{0, 0, {"armor"}}, {}};
static markup usability_block[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup item_feat_block[] = {{0, "#checkboxes", {"feat"}}, {}};
static markup basic_markup[] = {{0, "��������", {"name"}},
{0, "������������", {"name_unidentified"}},
{0, "������", {"type"}},
{0, "���� (�������)", {"cost"}, 6},
{0, "��� (������)", {"weight"}, 4},
{}};
static markup phisycal_addon[] = {{0, "/", {"abilities", 1}, 2}, {0, "/", {"abilities", 2}, 2}, {}};
static markup mental_addon[] = {{0, "/", {"abilities", 4}, 2}, {0, "/", {"abilities", 5}, 2}, {}};
static markup ability_block[] = {{0, "����������", {"abilities", 0, phisycal_addon}, 2},
{0, "����������", {"abilities", 3, mental_addon}, 2},
{}};
static markup item_general_�1[] = {{0, "������� ���������", {0, 0, basic_markup}},
{}};
static markup item_general_�2[] = {{0, "������", {0, 0, weapon_block}, 0, {0, 0, 0, hasdamage}},
{0, "�����", {0, 0, armor_block}, 0, {0, 0, 0, hasarmor}},
{0, "����� � ���������������", {0, 0, ability_block}, 0, {0, 0, 0, hasability}},
{}};
static markup item_general_�3[] = {{0, "�������������", {0, 0, usability_block}},
{0, "�����������", {0, 0, item_feat_block}},
{}};
static markup item_general[] = {{4, 0, {0, 0, item_general_�1}},
{5, 0, {0, 0, item_general_�2}},
{5, 0, {0, 0, item_general_�3}},
{}};
// ������ ������� ��������
static bool is_save_thrown(const void* object, int v) {
	return ((skill_s)v) >= FirstSave && ((skill_s)v) <= LastSave;
}
static bool is_theif_skill(const void* object, int v) {
	return ((skill_s)v) >= PickPockets && ((skill_s)v) <= ReadLanguages;
}
static bool is_other_skill(const void* object, int v) {
	return !is_theif_skill(object, v) && !is_save_thrown(object, v);
}
static markup saving_throws_block[] = {{0, "#skill", {"skills"}, 3, {0, 0, is_save_thrown}}, {}};
static markup theif_skills_block[] = {{0, "#skill", {"skills"}, 3, {0, 0, is_theif_skill}}, {}};
static markup other_skills_block[] = {{0, "#skill", {"skills"}, 3, {0, 0, is_other_skill}}, {}};
static markup item_skills_c1[] = {{0, "#title", {}, 180},
{0, "����� � ������� ��������", {0, 0, saving_throws_block}},
{}};
static markup item_skills_c2[] = {{0, "#title", {}, 180},
{0, "����� � ������� ����", {0, 0, theif_skills_block}},
{}};
static markup item_skills_c3[] = {{0, "#title", {}, 180},
{0, "����� � ������ �������", {0, 0, other_skills_block}},
{}};
static markup item_skills[] = {
{4, 0, {0, 0, item_skills_c1}},
{4, 0, {0, 0, item_skills_c2}},
{4, 0, {0, 0, item_skills_c3}},
{}};
static markup item_weapon[] = {{5, 0, {0, 0, weapon_block}},
{}};
markup item_info::markups[] = {{0, 0, {"#apply"}, 0, {}, item_info::apply},
{0, "�������� ��������", {"#page", 0, item_general}},
{0, "������ � �������", {"#page", 0, item_skills}},
{}};

const char* item_info::getname(const void* object, char* result, const char* result_max, int id) {
	auto p = (item_info*)object;
	switch(id) {
	case Name: return p->name;
	default: return "";
	}
}

int item_info::getvalue(const void* object, int id) {
	auto p = (item_info*)object;
	switch(id) {
	case Avatar: return -1;
	case Grade:
		if(p->name_unidentified)
			return Good;
		return Fair;
	default: return 0;
	}
}

void item_initialize() {
	auto p = bsmeta<item_info>::data.add();
	p->name = "��� ��������";
}