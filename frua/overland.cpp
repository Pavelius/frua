#include "main.h"

void overland_info::setindex(short unsigned v) {
	if(v == Blocked)
		return;
	index = v;
}

void overland_info::move(direction_s v) {
	setindex(to(index, v));
}