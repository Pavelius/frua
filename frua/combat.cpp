#include "main.h"

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

void combat_info::automove(character* player) {
	makewave(player->getposition());
}

void combat_info::playround() {
	for(auto p : parcipants) {
		if(!isenemy())
			return;
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
	return true;
}