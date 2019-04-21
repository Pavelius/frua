#include "main.h"

character* combat_info::add(race_s race, gender_s gender, class_s type, int level, reaction_s reaction) {
	auto p = enemies.add();
	p->create(race, gender, type, ChaoticEvil, reaction);
	p->setposition(map::random());
	p->setavatar(2);
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

void combat_info::playround() {
	for(auto p : parcipants) {
		if(!isenemy())
			return;
		if(p->isplayable())
			move(p);
		else {
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

void combat_info::makewave(short unsigned index) {
	map::setblock();
	setblock();
	map::makewave(index);
}