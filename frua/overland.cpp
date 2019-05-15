#include "main.h"

const bsreq bsmeta<overland_info>::meta[] = {
	BSREQ(name),
	BSREQ(index),
	BSREQ(image),
	BSREQ(areas),
	BSREQ(data),
{}};
BSDATA(overland_info, 4);

void overland_info::setindex(short unsigned v) {
	if(v == Blocked)
		return;
	index = v;
}

void overland_info::move(direction_s v) {
	setindex(to(index, v));
}

void overland_info::set(const picture_info& v) {
	image = v;
}
