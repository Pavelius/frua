#include "draw.h"
#include "main.h"

static void add_position(aref<character*> source) {
	for(auto p : source)
		p->setposition(combat_info::random());
}

bool test_array();

static void addparty(const char* name, reaction_s reaction) {
	auto pc = (character*)bsmeta<character>::data.find(bsmeta<character>::meta->find("name"), name);
	if(!pc)
		return;
	party.add(pc);
	pc->rollhp();
	pc->set(reaction);
}

static void test_combat() {
	combat_info ci;
	ci.create("Бандит");
	ci.create("Бандит");
	ci.create("Гордек", Player);
	ci.addenemies();
	ci.play();
}

void test_item() {
	static const markup columns[] = {{10, 0, {"name"}},
	{1, 0, {"weight"}},
	{1, 0, {"cost"}},
	{}};
	adat<item_info*, 12> source;
	source.add(bsmeta<item_info>::elements + 0);
	source.add(bsmeta<item_info>::elements + 1);
	decoration::choose("Выбирайте оружие", (void**)source.data, source.count, bsmeta<item_info>::meta, columns);
}

void test_scene() {
	addparty("Гордек", Player);
	addparty("Сер Кеалах", Player);
	scene sc;
	sc.img.set("adventure", "camp");
	sc.add("Вы находитесь рядом с домом.");
	sc.ask(1, "Заглянуть");
	sc.ask(2, "Оставить");
	sc.choose();
}

int	main(int argc, char *argv[]) {
	if(!test_array())
		return -1;
	decoration::initialize();
	draw::initialize();
	//test_item();
	//auto index = decoration::choose(bsmeta<character>::meta);
	//auto index = decoration::choose(bsmeta<item_info>::meta);
	//picture_info::choose_image();
	//test_combat();
	test_scene();
	return 0;
}

void main_unit();

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	//main_unit();
	return main(0, 0);
}