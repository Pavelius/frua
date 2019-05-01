#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(combat_info::random());
}

bool test_array();

static void test_combat() {
	combat_info ci;
	character p1; p1.create(Human, Male, Fighter, LawfulGood, Player);
	p1.setposition(ci.random());
	party.add(&p1);
	ci.create("Бандит");
	ci.create("Бандит");
	ci.addenemies();
	ci.addparty();
	ci.play();
}

int	main(int argc, char *argv[]) {
	if(!test_array())
		return -1;
	decoration::initialize();
	draw::initialize();
	auto index = decoration::choose(bsmeta<character>::meta);
	//auto index = decoration::choose(bsmeta<item_info>::meta);
	return 0;
}

void main_unit();

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	//main_unit();
	return main(0, 0);
}