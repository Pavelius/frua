#include "main.h"

const bsreq bsmeta<damage_info>::meta[] = {
	BSREQ(type),
	BSREQ(attacks),
	BSREQ(bonus),
	BSREQ(range),
	BSREQ(damage),
	BSREQ(feats),
{}};
static bool visible_not_empthy(const void* object, int index) {
	auto p = (damage_info*)object;
	return p->type != NoEffect;
}
bool damage_info::visible_damage(const void* object, int index) {
	auto p = (damage_info*)object;
	switch(p->type) {
	case Bludgeon:
	case Slashing:
	case Piercing:
	case Cold:
	case Fire:
	case Electricity:
		return true;
	default:
		return false;
	}
}
static bool visible_duration(const void* object, int index) {
	auto p = (damage_info*)object;
	switch(p->type) {
	case Paralize:
		return true;
	default:
		return false;
	}
}
static markup attacks_right[] = {{0, "/", {"bonus"}}, {}};
markup damage_info::body_markup[] = {{0, "К-во атак", {"attacks", 0, attacks_right}, 0, {0, 0, 0, visible_not_empthy}},
{0, "Дистанция", {"range"}, 0, {0, 0, 0, visible_not_empthy}},
{0, "Урон", {"damage"}, 0, {0, 0, 0, visible_damage}},
{0, "Длительность", {"damage"}, 0, {0, 0, 0, visible_duration}},
{0, "#checkboxes", {"feats"}, 2, {0, 0, 0, visible_not_empthy}},
{}};
markup damage_info::body_full_markup[] = {{0, 0, {"type"}},
{0, 0, {0, 0, body_markup}},
{}};
markup damage_info::markups[] = {{6, 0, {0, 0, body_full_markup}},
{}};

void damage_info::getname(stringcreator& sc) const {
	if(range > 1)
		sc.adds("дистанция %1i футов", range * 5);
	sc.adds("%-1", getstr(type));
	if(attacks > 1)
		sc.adds("%1i атаки", attacks);
	if(damage)
		damage.getname(sc);
}

const char* damage_info::getname(const void* object, char* result, const char* result_max, int id) {
	auto p = (damage_info*)object;
	stringcreator sc(result, result_max);
	switch(id) {
	case Name: p->getname(sc); return sc;
	default: return "";
	}
}