#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(combat_info::random());
}

static bool test_write() {
	auto p1 = bsmeta<character>::data.add();
	p1->create(Elf, Female, Mage, LawfulEvil, Player);
	p1->setname("Мирандина");
	auto p2 = bsmeta<character>::data.add();
	p2->create(Human, Male, Paladin, LawfulGood, Player);
	p2->setname("Вольган");
	auto p3 = bsmeta<character>::data.add();
	p3->create(Dwarf, Male, Fighter, LawfulGood, Player);
	p3->setname("Йорген");
	return bsdata::write("character.dat")==3;
}

static bool test_read() {
	if(bsdata::read("character.dat")<=0)
		return false;
	return true;
}

bool test_array();

int	main(int argc, char *argv[]) {
	//auto p = bsmeta<test_type>::meta;
	if(!test_array())
		return -1;
	draw::initialize();
	character::choose();
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