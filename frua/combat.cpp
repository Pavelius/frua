#include "main.h"

const int combat_moverate = 3;

character* combat_info::create(const char* name, reaction_s reaction) {
	auto pc = bsmeta<character>::data.find(bsmeta<character>::meta->find("name"), name);
	if(!pc)
		return 0;
	auto p = enemies.add();
	memcpy(p, pc, bsmeta<character>::data.size);
	p->rollhp();
	p->set(reaction);
	p->setposition(random());
	return p;
}

void combat_info::addenemies() {
	for(auto& e : enemies)
		parcipants.add(&e);
}

void combat_info::addparty() {
	for(auto p : party)
		parcipants.add(p);
}

static void get_lesser_cost(short unsigned index, short unsigned& cost) {
	auto v = mapcore::getcost(index);
	if(v == 0)
		return;
	if(v < cost)
		cost = v;
}

short unsigned combat_info::getmovecost(short unsigned index) const {
	auto cost = getcost(index);
	if(cost == 0 || cost == Blocked) {
		for(auto e : std::initializer_list<direction_s>{Left, Right, Up, Down})
			get_lesser_cost(to(index, e), cost);
	}
	return cost;
}

character* combat_info::getenemy(const character* player) const {
	character* target = 0;
	short unsigned target_cost = 0;
	for(auto enemy : parcipants) {
		if(!enemy->isalive())
			continue;
		if(enemy == player)
			continue;
		if(!player->isenemy(enemy))
			continue;
		auto index = getnear(enemy->getposition(), xmax, ymax);
		if(index == Blocked)
			continue;
		auto c = getcost(index);
		if(target_cost > 0 && c > target_cost)
			continue;
		target_cost = c;
		target = enemy;
	}
	return target;
}

void combat_info::automove(character* player, character* enemy) {
	if(!enemy)
		return;
	auto weapon = player->get(Weapon);
	auto reach = player->getreach(weapon);
	auto ismelee = (reach <= 2);
	while(movement > 0) {
		auto i1 = player->getposition();
		auto i2 = enemy->getposition();
		makewave(i2, i1);
		if(getcost(i1) <= reach) {
			player->attack(weapon, enemy);
			movement = 0;
			break;
		}
		if(!move(player, step(i1)))
			break;
		splash();
		msg_logger.clear();
	}
}

static combat_info* current_object;

combat_info* combat_info::getactive() {
	return current_object;
}

void combat_info::playround() {
	current_object = this;
	for(auto p : parcipants) {
		if(!isenemy())
			return;
		movement = p->getmovement();
		p->setactive();
		makewave(p->getposition());
		if(p->isplayable()) {
			choose();
			//move(p);
		} else {
			auto enemy = getenemy(p);
			automove(p, enemy);
		}
	}
	round++;
}

void combat_info::play() {
	while(isenemy()) {
		playround();
		round++;
	}
}

bool combat_info::isenemy() const {
	character* last_parcipant = 0;
	for(auto p : parcipants) {
		if(!p->isalive())
			continue;
		if(!last_parcipant)
			last_parcipant = p;
		if(last_parcipant->isenemy(p))
			return true;
	}
	return false;
}

void combat_info::makewave(short unsigned index, short unsigned dest_index) const {
	setblock();
	for(auto p : parcipants) {
		if(!p->isalive())
			continue;
		setblock(p->getposition(), Blocked);
	}
	if(dest_index != Blocked)
		setblock(dest_index, DefaultCost);
	mapcore::makewave(index, xmax, ymax);
}

bool combat_info::move(character* player, direction_s d) {
	if(!player)
		return false;
	if(d == Center)
		return false;
	auto i1 = to(player->getposition(), d);
	if(i1 == Blocked)
		return false;
	if(getcost(i1) == Blocked)
		return false;
	player->setposition(i1);
	switch(d) {
	case Left:
	case Right:
		player->set(d);
		break;
	}
	movement -= combat_moverate;
	player->act("%герой двигается.");
	return true;
}

void combat_info::add(int value, const char* name) {

}