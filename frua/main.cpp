#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(map::random());
}

int	main(int argc, char *argv[]) {
	auto p = bsmeta<bsreq::btype<const alignment_s*>::value>::meta;
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