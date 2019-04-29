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
static markup multiplier_right[] = {{0, "X", {"multiplier"}}, {}};
markup weapon_info::markups[] = {{0, 0, {0, 0, damage_info::markups}},
{0, "Урон (большим)", {"damage_large"}, 0, {0, 0, 0, damage_info::visible_damage}},
{0, "Критический", {"critical", 0, multiplier_right}, 0, {0, 0, 0, damage_info::visible_damage}},
{}};