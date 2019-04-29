#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(type),
	BSREQ(power_name),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(special_attack),
	BSREQ(armor),
	BSREQ(usability),
	BSREQ(feat),
	BSREQ(abilities),
	BSREQ(skills),
{}};
BSDATA(item_info, 512);

static bool allow_item(const void* source, int value) {
	return bsmeta<wear_s>::data[value].name_type != 0;
}
static bool hasdamage(const void* object, int index) {
	auto p = (item_info*)object;
	return bsmeta<wear_s>::data[p->type].use_damage;
}
bool hasarmor(const void* object, int index) {
	auto p = (item_info*)object;
	return bsmeta<wear_s>::data[p->type].use_armor;
}
bool hasability(const void* object, int index) {
	return hasarmor(object, index) && !hasdamage(object, index);
}
static const char* wear_getname(const void* object, char* result, const char* result_max, int column) {
	switch(column) {
	case 0:return ((wear_info*)object)->name_type;
	}
	return "";
}
static markup weapon_block[] = {{0, 0, {"damage"}},
{0, 0, {"special_attack"}},
{}};
static markup armor_block[] = {{0, 0, {"armor"}}, {}};
static markup usability_block[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup item_feat_block[] = {{0, "#checkboxes", {"feat"}}, {}};
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
markup item_info::markups[] = {{0, "�������� ��������", {"#page", 0, item_general}},
{0, "������ � �������", {"#page", 0, item_skills}},
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