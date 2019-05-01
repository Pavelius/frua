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
	if(cost == 0 || cost==Blocked) {
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
		auto c = getmovecost(enemy->getposition());
		if(c == 0 || (target_cost>0 && c>target_cost))
			continue;
		target_cost = c;
		target = enemy;
	}
	return target;
}

void combat_info::automove(character* player) {
	makewave(player->getposition());
	auto enemy = getenemy(player);
	auto weapon = player->get(MeleeWeapon);
	auto reach = weapon->getreach();
	if(enemy) {
		while(movement > 0) {
			auto i1 = player->getposition();
			auto i2 = enemy->getposition();
			makewave(i2);
			auto d = step(i1);
			auto i3 = to(i1, d);
			if(getcost(i3) < reach)
				break;
			if(!move(player, d))
				break;
			splash();
		}
	}
}

void combat_info::playround() {
	for(auto p : parcipants) {
		if(!isenemy())
			return;
		movement = p->getmovement();
		if(p->isplayable())
			move(p);
		else
			automove(p);
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

void combat_info::makewave(short unsigned index) {
	mapcore::setblock();
	for(auto p : parcipants) {
		if(!p->isalive())
			continue;
		mapcore::setblock(p->getposition(), Blocked);
	}
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
	return true;
}