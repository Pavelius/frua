#include "main.h"

const bsreq bsmeta<item>::meta[] = {
	BSREQ(type),
	BSREQ(value),
{}};

static bool allow_charges(const void* object, int param) {
	auto p = (item*)object;
	auto ei = (item_info&)bsmeta<item_info>::data[p->type];
	return ei.type == Wand;
}

markup item::markups[] = {{0, "Тип", {"type"}, 0, {0, 0, 0, 0, item::view_type}},
{0, "Зарядов", {"value"}, 0, {0, 0, 0, allow_charges}},
{}};