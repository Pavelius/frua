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

static void test_table() {
	struct character_driver : table_driver {
		constexpr character_driver() : table_driver(bsmeta<character>::data) {}
		bool editing(void* object, bool run) override {
			if(run) {
				character copy;
				if(object)
					memcpy(&copy, object, sizeof(character));
				else
					copy.clear();
				if(!copy.edit())
					return false;
				if(!object)
					object = source.add();
				if(object)
					memcpy(object, &copy, sizeof(character));
			}
			return true;
		}
		const char* getname(const void* object, stringcreator& sc, int column) const override {
			auto p = ((character*)object);
			switch(column) {
			case 0: return p->getname();
			case 1:
				sc.addn("Str:%1i, Int:%2i, Con:%3i", p->get(Strenght), p->get(Intellegence), p->get(Constitution));
				sc.addn("HD:%1i, AC:%2i, HP:%3i", p->getlevel(), p->getac(), p->gethpmax());
				return sc;
			}
			return "";
		}
	} e;
	int result = 0;
	e.choose("Выбирайте героя, персонажа или монстра", result, 256, false);
}

bool test_array();

int	main(int argc, char *argv[]) {
	if(!test_array())
		return -1;
	draw::initialize();
	test_table();
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