#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(combat_info::random());
}

bool test_array();

static character* test_character(race_s r, gender_s g, class_s t, alignment_s a) {
	auto p1 = bsmeta<character>::data.add();
	p1->create(r, g, t, a, Player);
	party.add(p1);
	p1->setposition(combat_info::random()); 
	return p1;
}

static void test_party() {
	test_character(Human, Male, Fighter, LawfulGood);
	test_character(Elf, Female, Mage, LawfulGood);
	test_character(Halfling, Male, Theif, NeutralEvil);
}

static void test_combat() {
	test_party();
	combat_info ci;
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
	//auto index = decoration::choose(bsmeta<character>::meta);
	//auto index = decoration::choose(bsmeta<item_info>::meta);
	test_combat();
	return 0;
}

void main_unit();

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	//main_unit();
	return main(0, 0);
}