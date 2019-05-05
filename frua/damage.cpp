#include "main.h"

const bsreq bsmeta<damage_info>::meta[] = {
	BSREQ(type),
	BSREQ(attack_type),
	BSREQ(save_type),
	BSREQ(range),
	BSREQ(damage),
{}};
static bool is_type_not_empthy(const void* object, int index) {
	auto p = (damage_info*)object;
	return p->type != NoEffect;
}
static bool is_weapon(const void* object, int param) {
	return param >= FirstDamage && param <= LastDamage;
}
static bool is_type_weapon(const void* object, int param) {
	auto p = (damage_info*)object;
	return is_weapon(object, p->type);
}
static bool only_attacks(const void* object, int param) {
	return bsmeta<attack_affect_s>::elements[param].attacks != 0;
}
static markup attacks_right[] = {{0, "/", {"bonus"}}, {}};
markup damage_info::weapon_markup[] = {{0, 0, {"type"}, 0, {is_weapon}},
{0, 0, {"attack_type"}, 0, {only_attacks, is_type_not_empthy}},
{0, "Дистанция", {"range"}},
{0, "Урон", {"damage"}},
{}};
static markup attack_markup[] = {{0, 0, {"type"}},
{0, "Срабатывает", {"attack_type"}, 0, {0, is_type_not_empthy}},
{0, "Спасение", {"save_type"}, 0, {0, is_type_not_empthy}},
{0, "Дистанция", {"range"}, 0, {0, is_type_not_empthy}},
{0, "Урон", {"damage"}, 0, {0, is_type_weapon}},
{}};
markup damage_info::markups[] = {{6, 0, {0, 0, attack_markup}},
{}};

void damage_info::getattacks(stringcreator& sc) const {
	auto format = "%1i/2";
	auto value = damage_info::getattacks();
	if((value & 1) == 0) {
		value /= 2;
		format = "%1i";
	}
	sc.adds(format, value);
}

void damage_info::getname(stringcreator& sc) const {
	if(range > 1)
		sc.adds("дистанция %1i футов", range * 5);
	sc.adds("%-1", getstr(type));
	if(getattacks() > 2) {
		getattacks(sc);
		sc.adds("атаки");
	}
	if(damage)
		damage.getname(sc);
}

void item_info::getweapon(stringcreator& sc) const {
	damage.getname(sc);
	if(damage_large) {
		sc.adds("/");
		damage_large.getname(sc);
	}
	if(abilities[CriticalThread] || abilities[CriticalMultiplier]) {
		sc.adds("(крит.");
		if(abilities[CriticalThread])
			sc.add("%1i-20", 20 - abilities[CriticalThread]);
		if(abilities[CriticalMultiplier])
			sc.add("x%1i", 2 + abilities[CriticalMultiplier]);
		sc.add(")");
	}
}

int	damage_info::getattacks() const {
	return bsmeta<attack_affect_info>::elements[attack_type].attacks;
}