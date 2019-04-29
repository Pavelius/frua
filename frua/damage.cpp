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
markup damage_info::body_markups[] = {{0, "К-во атак", {"attacks", 0, attacks_right}, 0, {0, 0, 0, visible_not_empthy}},
{0, "Дистанция", {"range"}, 0, {0, 0, 0, visible_not_empthy}},
{0, "Урон", {"damage"}, 0, {0, 0, 0, visible_damage}},
{0, "Длительность", {"damage"}, 0, {0, 0, 0, visible_duration}},
{0, "#checkboxes", {"feats"}, 0, {0, 0, 0, visible_not_empthy}},
{}};
markup damage_info::markups[] = {{0, 0, {"type"}},
{0, 0, {0, 0, body_markups}},
{}};