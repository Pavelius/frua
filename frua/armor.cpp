#include "main.h"

const bsreq bsmeta<armor_info>::meta[] = {
	BSREQ(ac),
	BSREQ(toughness),
	BSREQ(critical),
{}};
static markup deflection_markup[] = {{0, "/", {"toughness"}}, {}};
markup armor_info::markups[] = {{0, "Класс брони", {"ac", 0, deflection_markup}},
{0, "Отражение(%)", {"critical"}},
{}};