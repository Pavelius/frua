#include "main.h"

const bsreq bsmeta<item>::meta[] = {
	BSHIN(type, bsmeta<item_info>::meta),
	BSREQ(value),
{}};

markup item::markups[] = {{0, "“ËÔ", {"type"}},
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

int item::getquaility() const {
	switch(state) {
	case Cursed: return -(quality + 1);
	case Magic: return quality + 1;
	case Artifact: return quality + 2;
	default: return quality;
	}
}

bool item::iswearable() const {
	auto& e = bsmeta<item_type_info>::elements[bsmeta<item_info>::elements[type].type];
	return e.use_damage != 0 || e.use_armor != 0;
}