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
	ci.create("������");
	ci.create("������");
	ci.create("��������");
	ci.create("������", Player);
	ci.create("��� ������", Player);
	ci.addenemies();
	ci.play();
}

void test_scene() {
	addparty("������", Player);
	addparty("��� ������", Player);
	scene sc;
	sc.img.set("adventure", "camp");
	sc.add("�� ���������� ����� � �����.");
	sc.ask(1, "���������");
	sc.ask(2, "��������");
	sc.choose();
}

static void test_event() {
	event_info e = {};
	decoration::edit(&e);
}

static void test_overland() {
	overland_info* pb;
	if(bsmeta<overland_info>::data.count>0)
		pb = (overland_info*)bsmeta<overland_info>::data.get(0);
	else
		pb = bsmeta<overland_info>::data.add();
	pb->set({"maps", "amn"});
	pb->edit();
	decoration::autosave();
}

static void main_menu() {
	answer e;
	while(true) {
		e.ask(1, "������������� ������ � ��������");
		e.ask(2, "������������� ��������");
		e.ask(3, "������������� �������");
		e.ask(8, "������������� �����");
		e.ask(4, "������������� �������� ��������");
		e.ask(6, "������������� ������");
		e.ask(7, "�������������� ������");
		e.ask(20, "����������� ���");
		e.ask(21, "������������� �������");
		e.ask(100, "����� � ��������");
		auto id = e.choose("����������� � ������� ������������", {"adventure", "forest_site"});
		switch(id) {
		case 1: decoration::choose(bsmeta<character>::meta); break;
		case 2: decoration::choose(bsmeta<item_info>::meta); break;
		case 3: picture_info::choose_image(); break;
		case 4: picture_info::edit_monsters(); break;
		case 6: decoration::database_import(); break;
		case 7: decoration::database_export(); break;
		case 8: test_overland(); break;
		case 20: test_combat(); break;
		case 21: test_event(); break;
		case 100: bsdata::writetxt("campaigns/autosave.json"); return;
		}
	}
}

int	main(int argc, char *argv[]) {
	auto s1 = sizeof(bsreq);
	if(!test_array())
		return -1;
	decoration::initialize();
	draw::initialize();
	//test_combat();
	main_menu();
	//test_overland();
	//auto index = decoration::choose(bsmeta<character>::meta);
	//auto index = decoration::choose(bsmeta<item_info>::meta);
	//test_combat();
	//bsdata::writetxt("test.json");
	//inport_base("test.json");
	//test_scene();
	return 0;
}

void main_unit();

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	//main_unit();
	return main(0, 0);
}