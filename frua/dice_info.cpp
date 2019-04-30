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

void dice_info::getname(stringcreator& sc) const {
	sc.adds("%1id%2i", c, d);
	if(b != 0)
		sc.add("%+1i", b);
}

const char* dice_info::getname(const void* object, char* result, const char* result_max, int id) {
	auto p = (dice_info*)object;
	stringcreator sc(result, result_max);
	switch(id) {
	case Name: p->getname(sc); return sc;
	default: return "";
	}
}