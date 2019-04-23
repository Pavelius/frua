#include "main.h"

static char hit_probability[] = {
	-5, -5, -3, -3, -2, -2, -1, -1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 2, 2, 2, 3,
	3, 4, 4, 5, 6, 7
};
static char damage_adjustment[] = {
	-5, -5, -3, -3, -2, -2, -1, -1, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 2,
	3, 3, 4, 5, 6,
	7, 8, 9, 10, 11, 12, 14
};
static int weight_allowance[] = {
	1, 1, 1, 5, 10, 10, 20, 20, 35, 35,
	40, 40, 45, 45, 55, 55, 70, 85, 110, 135, 160, 185, 235, 335, 485,
	535, 635, 785, 935, 1235, 1535,
};
static short unsigned max_press[] = {
	0
};
char reaction_adjustment[26] = {
	-6, -6, -4, -3, -2, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 2, 2, 3, 3,
	4, 4, 4, 5, 5
};
static char defence_adjustment[] = {
	-6, -6, -4, -3, -2, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 3, 4, 4, 4,
	5, 5, 5, 6, 6
};
static char hit_points_adjustment[] = {
	-3, -2, -2, -1, -1, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2
};
static char hit_points_adjustment_warriors[] = {
	-3, -2, -2, -1, -1, -1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 3, 4, 5,
	5, 6, 6, 6, 7, 7
};
static char number_languages[] = {
	0, 0, 1, 1, 1, 1, 1, 1, 1, 2,
	2, 2, 3, 3, 4, 4, 5, 6, 7, 8,
	9, 10, 11, 12, 15, 20
};
static char wp_melee_attacks[] = {
	0, 0, 1, 1
};
static char wp_melee_thac0[] = {
	0, 0, 1, 3,
};
adat<character, 128> character_data;
adat<character*, 8> party;
static character* active_player;

void character::clear() {
	memset(this, 0, sizeof(character));
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] = 10;
	base_ac = 10;
	apply_feats();
}

void character::roll_ability() {
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		auto v1 = dice::roll(3, 6);
		auto v2 = dice::roll(3, 6);
		abilities[i] = imax(v1, v2);
	}
	// ������ ����������� ������ ����� �������
	auto ability = class_data[type].ability;
	auto ability_best = Strenght;
	auto ability_value = 0;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(abilities[i] > ability_value) {
			ability_value = abilities[i];
			ability_best = i;
		}
	}
	iswap(abilities[ability], abilities[ability_best]);
}

void character::apply_ability_restriction() {
	// ����������� �� ���� � �� ������
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(abilities[i] < race_data[race].minimum[i])
			abilities[i] = race_data[race].minimum[i];
		if(abilities[i] > race_data[race].maximum[i])
			abilities[i] = race_data[race].maximum[i];
		abilities[i] += race_data[race].adjustment[i];
	}
	// �������� � ������� ����������� � ������������ ��������
	for(auto e : class_data[type].classes) {
		for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
			if(class_data[e].minimum[i] == 0)
				continue;
			if(abilities[i] < class_data[e].minimum[i])
				abilities[i] = class_data[e].minimum[i];
		}
	}
}

void character::apply_feats() {
	feats = race_data[race].feats.data;
	feats |= class_data[type].feats.data;
	movement = race_data[race].movement;
	size = race_data[race].size;
}

static int random_avatar(race_s race) {
	static const char* source[] = {"character9", "character49", "character10",
		"character40", "character2", "character24"};
	return character::select_avatar(maptbl(source, race));
}

void character::create(race_s race, gender_s gender, class_s type, alignment_s alignment, reaction_s reaction) {
	clear();
	this->race = race;
	this->gender = gender;
	this->type = type;
	this->alignment = alignment;
	this->reaction = reaction;
	roll_ability();
	apply_feats();
	apply_ability_restriction();
	if(!is(NoExeptionalStrenght))
		strenght_percent = xrand(1, 100);
	for(auto e : class_data[type].classes)
		raise(e);
	hp = gethpmax();
	avatar = random_avatar(race);
}

int character::getindex(class_s type, class_s v) {
	return class_data[type].classes.indexof(v);
}

void character::raise(class_s v) {
	auto index = getindex(type, v);
	if(index == -1)
		return;
	levels[index]++;
	// ��������� ������� ��������� ����
	if(class_data[v].hd) {
		int r = 0;
		if(levels[index] == 1) {
			auto r1 = xrand(1, class_data[v].hd);
			auto r2 = xrand(1, class_data[v].hd);
			r = imax(r1, r2);
			if(class_data[v].bonus_hd)
				r += xrand(1, class_data[v].hd);
		} else
			r = xrand(1, class_data[v].hd);
		hp_rolled += r;
	}
}

bool character::isallow(alignment_s v, class_s type) {
	auto& ev = class_data[type];
	for(unsigned i = 0; i < ev.classes.count; i++) {
		auto e = ev.classes.data[i];
		if(alignment_data[v].restricted.indexof(e) != -1)
			return false;
	}
	return true;
}

bool character::isallow(class_s v, race_s race) {
	auto& e = class_data[v].races;
	return !e || e.is(race);
}

int character::getstrex() const {
	auto result = get(Strenght);
	if(result > 18)
		result += 6;
	else if(result == 18 && strenght_percent > 0) {
		if(strenght_percent <= 50)
			result += 1;
		else if(strenght_percent <= 75)
			result += 2;
		else if(strenght_percent <= 90)
			result += 3;
		else if(strenght_percent <= 99)
			result += 4;
		else
			result += 5;
	}
	return result;
}

int character::getac() const {
	auto result = base_ac;
	auto dex = get(Dexterity);
	result -= maptbl(defence_adjustment, dex);
	return result;
}

int	character::gethpmax(int v) const {
	if(!class_data[type].classes.count)
		return 0;
	int result = v / class_data[type].classes.count;
	int level = levels[0];
	int con = get(Constitution);
	if(is(BonusHits))
		result += maptbl(hit_points_adjustment_warriors, con)*level;
	else
		result += maptbl(hit_points_adjustment, con)*level;
	if(result < level)
		result = level;
	return result;
}

void character::get(wear_s id, attack_info& ai) const {
	ai.attacks = 2;
	ai.thac0 = 20;
	ai.damage = dice::create(1, 2);
	if(id == MeleeWeapon || id == OffhandWeapon) {
		auto str = getstrex();
		ai.thac0 -= maptbl(hit_probability, str);
		ai.damage.b += maptbl(damage_adjustment, str);
	}
}

character* character::getactive() {
	return active_player;
}

void character::setactive() {
	active_player = this;
}

bool character::move(direction_s d) {
	auto i1 = map::to(getposition(), d);
	if(i1 == Blocked)
		return false;
	if(map::getcost(i1) == Blocked)
		return false;
	setposition(i1);
	switch(d) {
	case Left:
	case Right:
		direction = d;
		break;
	}
	return true;
}

int character::getmovement() const {
	return 12;
}

bool character::isenemy(const character* p) const {
	if(this == p)
		return false;
	if(reaction == Player || reaction == Friendly)
		return p->reaction == Hostile;
	else if(reaction == Hostile)
		return p->reaction == Player || p->reaction == Friendly;
	return false;
}

int	character::select_avatar(short unsigned* result, unsigned count, const char* mask) {
	auto pb = result;
	auto pe = result + count;
	auto ps = avatar_data.data;
	auto pc = avatar_data.count;
	for(unsigned i = 0; i < pc; i++) {
		if(mask && !szpmatch(ps[i].name, mask))
			continue;
		if(pb < pe)
			*pb++ = i;
	}
	return pb - result;
}

int character::select_avatar(const char* mask) {
	short unsigned source[256];
	auto count = select_avatar(source, sizeof(source) / sizeof(source[0]), mask);
	if(!count)
		return -1;
	return source[rand() % count];
}

void character::correct() {
	// ������� ������, ������� �� ���� ��������
	for(int i = class_data[type].classes.count; i < sizeof(levels) / sizeof(levels[0]); i++)
		levels[i] = 0;
	// �������������� �����
	if(!isallow(type)) {
		auto old = type;
		do {
			type = (class_s)(type + 1);
			if(type > FighterMageTheif)
				type = Cleric;
		} while(type != old && !isallow(type));
	}
	// �������������� ������������
	if(!isallow(alignment)) {
		auto old = alignment;
		do {
			alignment = (alignment_s)(alignment + 1);
			if(alignment > ChaoticEvil)
				alignment = LawfulGood;
		} while(alignment!=old && !isallow(alignment));
	}
}