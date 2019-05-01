#include "main.h"

const bsreq bsmeta<item>::meta[] = {
	BSHIN(type, bsmeta<item_info>::meta),
	BSREQ(value),
{}};

markup item::markups[] = {{0, "“ип", {"type"}},
{}};

void item::getname(stringcreator& sc) const {
	if(!type)
		return;
	sc.add(bsmeta<item_info>::data[type].name);
}

const char* item::getname(const void* object, char* result, const char* result_max, int id) {
	auto p = (item*)object;
	stringcreator sc(result, result_max);
	switch(id) {
	case Name: p->getname(sc); return sc;
	default: return "";
	}
}

int	item::getreach() const {
	auto ei = (item_info&)bsmeta<item_info>::data[type];
	return 1 + ei.damage.range;
}