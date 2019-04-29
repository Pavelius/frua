#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(combat_info::random());
}

bool test_array();

int	main(int argc, char *argv[]) {
	auto p = bsmeta<weapon_info>::meta;
	if(!test_array())
		return -1;
	draw::initialize();
	//auto index = decoration::choose(bsmeta<character>::meta);
	auto index = decoration::choose(bsmeta<item_info>::meta);
	//character ii = {};
	//ii.edit_generate();
	//character::choose();
	//event_info ei;
	//ei.edit();
	//picture_info::pick_monster();
	//character::choose_avatar("character*", 10);
	//character_data.add()->generate();
	return 0;
}

void main_unit();

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	main_unit();
	return main(0, 0);
}