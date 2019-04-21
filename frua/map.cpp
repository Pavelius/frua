#include "main.h"

short unsigned map::m2i(int x, int y) {
	return y * combat_map_x + x;
}

short unsigned map::random() {
	auto i = m2i(xrand(1, combat_map_x - 2), xrand(1, combat_map_y - 2));
	return i;
}

short unsigned map::to(short unsigned i, direction_s d) {
	if(i == Blocked)
		return i;
	auto x = i % combat_map_x;
	auto y = i / combat_map_x;
	switch(d) {
	case Left:
		if(x <= 0)
			return Blocked;
		return m2i(x - 1, y);
	case Right:
		if(x >= combat_map_x - 1)
			return Blocked;
		return m2i(x + 1, y);
	case Up:
		if(y <= 0)
			return Blocked;
		return m2i(x, y - 1);
	case Down:
		if(y >= combat_map_y - 1)
			return Blocked;
		return m2i(x, y + 1);
	default:
		return Blocked;
	}
}

static short unsigned map_block[combat_map_x*combat_map_x];

void map::makewave(short unsigned start_index) {
	static direction_s directions[] = {Left, Right, Up, Down};
	short unsigned stack[256 * 256];
	short unsigned push_counter = 0;
	short unsigned pop_counter = 0;
	map_block[start_index] = 0;
	stack[push_counter++] = start_index;
	while(pop_counter != push_counter) {
		auto index = stack[pop_counter++];
		auto cost = map_block[index] + 1;
		for(auto d : directions) {
			auto i1 = map::to(index, d);
			if(i1 == Blocked || map_block[i1] == Blocked)
				continue;
			if(map_block[i1] < cost)
				continue;
			map_block[i1] = cost;
			stack[push_counter++] = i1;
		}
	}
}

void map::setblock() {
	for(auto& e : map_block)
		e = DefaultCost;
}

void combat_info::setblock() {
	for(auto p : parcipants) {
		if(!p->isalive())
			continue;
		auto index = p->getposition();
		if(index == Blocked)
			continue;
		map_block[index] = Blocked;
	}
}

short unsigned map::getcost(short unsigned index) {
	if(index == Blocked)
		return 0;
	return map_block[index];
}