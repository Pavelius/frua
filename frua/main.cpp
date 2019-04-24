#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(map::random());
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
	character c1 = {};
	if(!bsdata::read("character.dat", &c1, bsmeta<bsreq::btype<decltype(c1)>::value>::meta))
		return false;
	return true;
}

int	main(int argc, char *argv[]) {
	if(!test_write())
		return -1;
	if(!test_read())
		return -1;
	draw::initialize();
	character ev;
	ev.clear();
	ev.edit();
	//event_info ei;
	//ei.edit();
	//picture_info::pick_monster();
	//character::choose_avatar("character*", 10);
	//character_data.add()->generate();
	bsmeta<character>::data.add()->create(Human, Male, Paladin, LawfulGood, Player);
	bsmeta<character>::data.add()->create(Dwarf, Male, Fighter, LawfulGood, Player);
	party.add(&bsmeta<character>::data[0]);
	party.add(&bsmeta<character>::data[1]);
	add_position(party);
	combat_info ci;
	ci.add(Elf, Female, FighterTheif);
	ci.add(Elf, Male, Fighter);
	ci.addenemies();
	ci.addparty();
	//ci.play();
	return 0;
}

void main_unit();

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	main_unit();
	return main(0, 0);
}