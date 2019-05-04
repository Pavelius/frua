#include "main.h"

const int combat_moverate = 2;

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
	short unsigned target_cost = Blocked;
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
		if(c > target_cost)
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
	while(player->isready()) {
		auto i1 = player->getposition();
		auto i2 = enemy->getposition();
		makewave(i2, i1);
		if(getcost(i1) <= reach) {
			player->attack(weapon, enemy);
			player->stop();
			break;
		}
		if(!player->moveto(step(i1)))
			break;
		splash();
		msg_logger.clear();
	}
}

static combat_info* current_object;

combat_info* combat_info::getactive() {
	return current_object;
}

int combat_info::getdistance(short unsigned index) const {
	if(index == Blocked)
		return Blocked;
	auto c = getcost(index);
	if(c == Blocked) {
		index = getnear(index, xmax, ymax);
		if(index == Blocked)
			return Blocked;
		c = getcost(index) + 1;
	}
	return c;
}

void combat_info::playmove(character* player, character* enemy) {
	player->setactive();
	while(player->isready()) {
		auto position = player->getposition();
		makewave(position);
		auto enemy = getenemy(player);
		if(!enemy)
			break;
		auto enemy_position = enemy->getposition();
		auto weapon = player->get(Weapon);
		auto reach = player->getreach(weapon);
		answer an;
		if(enemy && getdistance(enemy_position) <= reach)
			an.add(variant(AttackRegular), "Атаковать", 2);
		for(auto d : all_directions) {
			auto i1 = to(position, d);
			if(i1 == Blocked || getcost(i1) == Blocked)
				continue;
			an.add(variant(d), getstr(d), 1, bsmeta<direction_s>::elements[d].key);
		}
		auto cid = (variant)an.choose(*this);
		switch(cid.type) {
		case Attacks:
			player->attack(weapon, enemy);
			player->stop();
			break;
		case Directions:
			player->moveto(cid.direction);
			break;
		}
	}
}

void combat_info::playround() {
	current_object = this;
	for(auto p : parcipants) {
		if(!p->isalive())
			continue;
		p->refresh();
	}
	for(auto p : parcipants) {
		if(!p->isready())
			continue;
		if(!isenemy())
			return;
		p->setactive();
		makewave(p->getposition());
		auto enemy = getenemy(p);
		if(p->isplayable())
			playmove(p, enemy);
		else
			automove(p, enemy);
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

bool character::moveto(direction_s d) {
	if(d == Center)
		return false;
	auto i1 = mapcore::to(getposition(), d, combat_map_x, combat_map_y);
	if(i1 == mapcore::Blocked)
		return false;
	if(mapcore::getcost(i1) == mapcore::Blocked)
		return false;
	setposition(i1);
	switch(d) {
	case Left:
	case Right:
		set(d);
		break;
	}
	current_movement -= combat_moverate;
	act("%герой двигается.");
	return true;
}