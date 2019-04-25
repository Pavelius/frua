#include "main.h"

static short unsigned map_block[64 * 64];

short unsigned mapcore::to(short unsigned i, direction_s d, short xm, short ym) {
	if(i == Blocked)
		return i;
	auto x = i % xm;
	auto y = i / xm;
	switch(d) {
	case Left:
		if(x <= 0)
			return Blocked;
		return y * xm + (x - 1);
	case Right:
		if(x >= xm - 1)
			return Blocked;
		return y * xm + (x + 1);
	case Up:
		if(y <= 0)
			return Blocked;
		return (y - 1) * xm + x;
	case Down:
		if(y >= ym - 1)
			return Blocked;
		return (y + 1) * xm + x;
	default:
		return Blocked;
	}
}

void mapcore::makewave(short unsigned start_index, short xm, short ym) {
	static direction_s directions[] = {Left, Right, Up, Down};
	short unsigned stack[256 * 64];
	auto stack_end = stack + sizeof(stack) / sizeof(stack[0]);
	auto push_counter = stack;
	auto pop_counter = stack;
	map_block[start_index] = 0;
	*push_counter++ = start_index;
	while(pop_counter != push_counter) {
		auto index = *pop_counter++;
		if(pop_counter >= stack_end)
			pop_counter = stack;
		auto cost = map_block[index] + 1;
		for(auto d : directions) {
			auto i1 = to(index, d, xm, ym);
			if(i1 == Blocked || map_block[i1] == Blocked)
				continue;
			if(map_block[i1] <= cost)
				continue;
			map_block[i1] = cost;
			*push_counter++ = i1;
			if(push_counter >= stack_end)
				push_counter = stack;
		}
	}
}

void mapcore::setblock() {
	for(auto& e : map_block)
		e = DefaultCost;
}

void mapcore::setblock(short unsigned index, short unsigned v) {
	if(index == Blocked)
		return;
	map_block[index] = v;
}

short unsigned mapcore::getcost(short unsigned index) {
	if(index == Blocked)
		return 0;
	return map_block[index];
}