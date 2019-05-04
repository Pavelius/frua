#include "main.h"

const bsreq bsmeta<item_info>::meta[] = {
	BSREQ(name),
	BSREQ(name_unidentified),
	BSREQ(type),
	BSREQ(cost),
	BSREQ(weight),
	BSREQ(damage),
	BSREQ(damage_large),
	BSREQ(special),
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
static markup multiplier_right[] = {{0, "X", {"abilities", CriticalMultiplier}}, {}};
static markup weapon_c1[] = {{0, 0, {"damage", 0, damage_info::weapon_markup}},
{0, "Урон (большим)", {"damage_large"}},
{0, "Критический", {"abilities", CriticalThread, multiplier_right}},
{0, 0, {"special"}},
{}};
static markup weapon_markup[] = {{6, 0, {0, 0, weapon_c1}},
{}};
static markup weapon_block[] = {{0, 0, {"damage"}, 0, {0, 0, 0, 0, item_info::view_weapon}},
{}};
void item_info::editweapon(void* p) {
	decoration::edit("Оружие", p, sizeof(item_info), bsmeta<item_info>::meta, weapon_markup);
}
static markup deflection_markup[] = {{0, "/", {"abilities", DR}}, {}};
markup armor_block[] = {{0, "Класс брони", {"abilities", AC, deflection_markup}},
{0, "Отражение(%)", {"abilities", CriticalDeflection}},
{}};
static markup usability_block[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup item_feat_block[] = {{0, "#checkboxes", {"feat"}}, {}};
static markup basic_markup[] = {{0, "Название", {"name"}},
{0, "Неопределено", {"name_unidentified"}},
{0, "Группа", {"type"}},
{0, "Цена (серебра)", {"cost"}, 6},
{0, "Вес (фунтов)", {"weight"}, 4},
{}};
static markup phisycal_addon[] = {{0, "/", {"abilities", 1}, 2}, {0, "/", {"abilities", 2}, 2}, {}};
static markup mental_addon[] = {{0, "/", {"abilities", 4}, 2}, {0, "/", {"abilities", 5}, 2}, {}};
static markup ability_block[] = {{0, "Физические", {"abilities", 0, phisycal_addon}, 2},
{0, "Ментальные", {"abilities", 3, mental_addon}, 2},
{}};
static markup item_general_с1[] = {{0, "Базовые параметры", {0, 0, basic_markup}},
{}};
static markup item_general_с2[] = {{0, "Оружие", {0, 0, weapon_block}, 0, {0, 0, 0, hasdamage}},
{0, "Броня", {0, 0, armor_block}, 0, {0, 0, 0, hasarmor}},
{0, "Бонус к характеристикам", {0, 0, ability_block}, 0, {0, 0, 0, hasability}},
{}};
static markup item_general_с3[] = {{0, "Использование", {0, 0, usability_block}},
{0, "Особенности", {0, 0, item_feat_block}},
{}};
static markup item_general[] = {{4, 0, {0, 0, item_general_с1}},
{5, 0, {0, 0, item_general_с2}},
{5, 0, {0, 0, item_general_с3}},
{}};
// Группа навыков предмета
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
{0, "Бонус к броскам спасения", {0, 0, saving_throws_block}},
{}};
static markup item_skills_c2[] = {{0, "#title", {}, 180},
{0, "Бонус к навыкам вора", {0, 0, theif_skills_block}},
{}};
static markup item_skills_c3[] = {{0, "#title", {}, 180},
{0, "Бонус к другим навыкам", {0, 0, other_skills_block}},
{}};
static markup item_skills[] = {
{4, 0, {0, 0, item_skills_c1}},
{4, 0, {0, 0, item_skills_c2}},
{4, 0, {0, 0, item_skills_c3}},
{}};
static markup item_weapon[] = {{5, 0, {0, 0, weapon_block}},
{}};
markup item_info::markups[] = {{0, 0, {"#apply"}, 0, {}, item_info::apply},
{0, 0, {"#create"}, 0, {}, item_info::create},
{0, "Основные свойства", {"#page", 0, item_general}},
{0, "Бонусы к навыкам", {"#page", 0, item_skills}},
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
	p->name = "Нет предмета";
}

void item_info::apply(item_info* p) {
	auto& wi = bsmeta<item_type_s>::elements[p->type];
	if(!wi.use_damage)
		memset(&p->damage, 0, sizeof(p->damage));
	if(!wi.use_armor)
		memset(p->abilities + AC, 0, CriticalMultiplier - AC + 1);
}

void item_info::create(item_info* p) {
	p->type = Weapon;
	p->damage.type = Bludgeon;
	p->damage.attacks = 1;
	p->damage.damage.c = 1;
	p->damage.damage.d = 3;
}