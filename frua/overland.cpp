#include "main.h"

overland_info::overland_info(const picture_info& source, short unsigned index) : source(source), index(index) {
	for(auto i = 0; i < 16; i++) {
		char temp[260]; zprint(temp, "Область %1i", i+1);
		areas[i].name = szdup(temp);
	}
}

void overland_info::setindex(short unsigned v) {
	if(v == Blocked)
		return;
	index = v;
}

void overland_info::move(direction_s v) {
	setindex(to(index, v));
}