#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(map::random());
}

int	main(int argc, char *argv[]) {
	draw::initialize();
	event_info ev;
	ev.edit();
	//picture_info::pick_monster();
	//character::choose_avatar("character*", 10);
	//character_data.add()->generate();
	character_data.add()->create(Human, Male, Paladin, LawfulGood, Player);
	character_data.add()->create(Dwarf, Male, Fighter, LawfulGood, Player);
	party.add(character_data.data + 0);
	party.add(character_data.data + 1);
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