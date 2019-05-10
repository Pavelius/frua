#include "main.h"

const bsreq bsmeta<item>::meta[] = {
	BSHIN(type, bsmeta<item_info>::meta),
	BSREQ(value),
{}};

markup item::markups[] = {{0, 0, {"type"}},
{}};

void item::clear() {
	memset(this, 0, sizeof(*this));
}

void item::getname(stringcreator& sc) const {
	if(!type)
		return;
	sc.add(bsmeta<item_info>::data[type].name);
}

int	item::getreach() const {
	return 1 + getinfo().damage.range;
}

int item::getquaility() const {
	if(!identify)
		return quality;
	switch(state) {
	case Cursed: return -(quality + 1);
	case Magic: return quality + 1;
	case Artifact: return quality + 2;
	default: return quality;
	}
}

int	item::get(ability_s id) const {
	auto& ii = bsmeta<item_info>::elements[type];
	return ii.abilities[id];
}

int	item::getq(ability_s id) const {
	auto r = get(id);
	if(r) {
		r += getquaility();
	}
	return r;
}

bool item::isallow(unsigned mask) const {
	auto v = getinfo().usability.data;
	return (mask & v) == v;
}