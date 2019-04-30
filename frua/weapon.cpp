#include "main.h"

const bsreq bsmeta<weapon_info>::meta[] = {
	BSREQ(type),
	BSREQ(bonus),
	BSREQ(critical),
	BSREQ(multiplier),
	BSREQ(attacks),
	BSREQ(range),
	BSREQ(damage),
	BSREQ(damage_large),
{}};
bool weapon_info::isweapon(const void* object, int param) {
	return param >= FirstDamage && param <= LastDamage;
}
static markup multiplier_right[] = {{0, "X", {"multiplier"}}, {}};
markup weapon_info::body_markup[] = {{0, 0, {"type"}, 0, {0, 0, isweapon}},
{0, 0, {0, 0, damage_info::body_markup}},
{0, "Урон (большим)", {"damage_large"}, 0, {0, 0, 0, damage_info::visible_damage}},
{0, "Критический", {"critical", 0, multiplier_right}, 0, {0, 0, 0, damage_info::visible_damage}},
{}};
markup weapon_info::markups[] = {{6, 0, {0, 0, body_markup}},
{}};

void weapon_info::getname(stringcreator& sc) {
	damage_info::getname(sc);
	if(damage_large) {
		sc.adds("/");
		damage_large.getname(sc);
	}
	if(critical || multiplier) {
		sc.adds("(крит.");
		if(critical)
			sc.add("%1i-20", 20 - critical);
		if(multiplier)
			sc.add("x%1i", 2 + multiplier);
		sc.add(")");
	}
}

const char* weapon_info::getname(const void* object, char* result, const char* result_max, int id) {
	auto p = (weapon_info*)object;
	stringcreator sc(result, result_max);
	switch(id) {
	case Name: p->getname(sc); return sc;
	default: return "";
	}
}