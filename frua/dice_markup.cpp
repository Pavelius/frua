#include "main.h"

const bsreq bsmeta<dice_info>::meta[] = {
	BSREQ(c),
	BSREQ(d),
	BSREQ(b),
	BSREQ(m),
{}};
static markup dice_markup_right[] = {{0, "d", {"d", 0, }}, {0, "+", {"m", 0, }}, {}};
markup dice_info::markups[] = {{0, "Урон", {"c", 0, dice_markup_right}},
{}};