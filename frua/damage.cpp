#include "main.h"

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
static markup multiplier_right[] = {{0, "X", {"multiplier"}}, {}};
static markup attack_right[] = {{0, "/", {"thac0"}}, {}};
markup damage_info::markups[] = {{0, "Тип", {"attack_type"}},
{0, "К-во атак", {"attacks", 0, attack_right}},
{0, "Тип", {"type"}},
{0, "Дистанция", {"range"}},
{0, "Критический", {"critical", 0, multiplier_right}},
{0, "Урон", {"damage"}},
{0, "Урон (большим)", {"damage_large"}},
{}};