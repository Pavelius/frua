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
adat<character*, 8> party;
static character* active_player;

void character::clear() {
	memset(this, 0, sizeof(character));
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] = 10;
	base_ac = 10;
	movement = 12;
	levels[0] = 1;
	apply_feats();
}

void character::roll_ability() {
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		auto v1 = dice::roll(3, 6);
		auto v2 = dice::roll(3, 6);
		abilities[i] = imax(v1, v2);
	}
	// Лучшая способность всегда самая высокая
	auto ability = bsmeta<class_s>::data[type].ability;
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
	// Ограничения от расы и ее бонусы
	auto& rcl = bsmeta<race_s>::data;
	auto& ccl = bsmeta<class_s>::data;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(abilities[i] < rcl[race].minimum[i])
			abilities[i] = rcl[race].minimum[i];
		if(abilities[i] > rcl[race].maximum[i])
			abilities[i] = rcl[race].maximum[i];
		abilities[i] += rcl[race].adjustment[i];
	}
	// Приведем в порядок минимальные и максимальные атрибуты
	for(auto e : ccl[type].classes) {
		for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
			if(ccl[e].minimum[i] == 0)
				continue;
			if(abilities[i] < ccl[e].minimum[i])
				abilities[i] = ccl[e].minimum[i];
		}
	}
}

void character::apply_feats() {
	feats = bsmeta<race_s>::data[race].feats;
	feats |= bsmeta<class_s>::data[type].feats;
	usability = bsmeta<race_s>::data[race].usability;
	usability |= bsmeta<class_s>::data[type].usability;
	movement = bsmeta<race_s>::data[race].movement;
	size = bsmeta<race_s>::data[race].size;
}

void character::recreate() {
	create(race, gender, type, alignment, Player);
}

void character::random(void* object) {
	auto p = (character*)object;
	p->clear();
	p->gender = (gender_s)xrand(Male, Female);
	p->race = (race_s)xrand(Human, Halfling);
	p->type = (class_s)xrand(Cleric, Theif);
	p->alignment = (alignment_s)xrand(LawfulGood, ChaoticEvil);
	p->recreate();
}

void character::reroll() {
	roll_ability();
	apply_feats();
	apply_ability_restriction();
	if(!is(NoExeptionalStrenght))
		strenght_percent = xrand(1, 100);
	else
		strenght_percent = 0;
}

void character::create(race_s race, gender_s gender, class_s type, alignment_s alignment, reaction_s reaction) {
	static const char* source[] = {"character9", "character49", "character10",
		"character40", "character2", "character24"};
	clear();
	this->race = race;
	this->gender = gender;
	this->type = type;
	this->alignment = alignment;
	this->reaction = reaction;
	correct();
	roll_ability();
	apply_feats();
	apply_ability_restriction();
	if(!is(NoExeptionalStrenght))
		strenght_percent = xrand(1, 100);
	for(auto e : bsmeta<class_s>::data[type].classes)
		raise(e);
	hp = gethpmax();
	avatar = picture_info::random(maptbl(source, race));
}

int character::getindex(class_s type, class_s v) {
	return bsmeta<class_s>::data[type].classes.indexof(v);
}

void character::raise(class_s v) {
	auto index = getindex(type, v);
	if(index == -1)
		return;
	levels[index]++;
	// Случайным обраом определим хиты
	if(bsmeta<class_s>::data[v].hd) {
		int r = 0;
		if(levels[index] == 1) {
			auto r1 = xrand(1, bsmeta<class_s>::data[v].hd);
			auto r2 = xrand(1, bsmeta<class_s>::data[v].hd);
			r = imax(r1, r2);
			if(bsmeta<class_s>::data[v].bonus_hd)
				r += xrand(1, bsmeta<class_s>::data[v].hd);
		} else
			r = xrand(1, bsmeta<class_s>::data[v].hd);
		hp_rolled += r;
	}
}

bool character::isallow(alignment_s v, class_s type) {
	auto& ev = bsmeta<class_s>::data[type];
	for(unsigned i = 0; i < ev.classes.count; i++) {
		auto e = ev.classes.data[i];
		if(bsmeta<alignment_s>::data[v].restricted.indexof(e) != -1)
			return false;
	}
	return true;
}

bool character::isallow(class_s v, race_s race) {
	auto& e = bsmeta<class_s>::data[v].races;
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

int character::getbonus(ability_s id) const {
	auto result = 0;
	for(auto& e : wears) {
		if(!e)
			break;
		if(e.isready())
			result += e.getq(id);
	}
	return result;
}

int	character::get(ability_s v) const {
	int r, a;
	switch(v) {
	case AC:
		r = base_ac;
		a = get(Dexterity);
		r -= maptbl(defence_adjustment, a);
		r -= getbonus(AC);
		return r;
	case Strenght:
	case Dexterity:
	case Constitution:
	case Intellegence:
	case Wisdow:
	case Charisma:
		return abilities[v];
	default:
		return 0;
	}
}

int	character::gethpmax(int v) const {
	if(!bsmeta<class_s>::data[type].classes.count)
		return 0;
	int result = v / bsmeta<class_s>::data[type].classes.count;
	int level = imin(1, (int)levels[0]);
	int con = get(Constitution);
	if(is(BonusHits))
		result += maptbl(hit_points_adjustment_warriors, con)*level;
	else
		result += maptbl(hit_points_adjustment, con)*level;
	if(result < level)
		result = level;
	return result;
}

void character::get(item* weapon, attack_info& ai, bool large_enemy) const {
	ai.weapon = weapon;
	if(ai.weapon) {
		*static_cast<damage_info*>(&ai) = ai.weapon->getinfo().damage;
		if(large_enemy)
			ai.damage = ai.weapon->getinfo().damage_large;
		ai.critical = ai.weapon->get(CriticalThread);
		ai.multiplier = ai.weapon->get(CriticalMultiplier);
		if(is(ElfWeaponTraining) && ai.weapon->is(UseElfWeapon))
			ai.bonus += 1;
	} else {
		ai.damage.c = 1;
		ai.damage.d = 2;
	}
	if(ai.ismelee()) {
		auto str = getstrex();
		ai.bonus += maptbl(hit_probability, str);
		ai.damage.b += maptbl(damage_adjustment, str);
	}
	if(ai.isranged()) {
		auto dex = get(Dexterity);
		ai.bonus += maptbl(reaction_adjustment, dex);
	}
}

character* character::getactive() {
	return active_player;
}

void character::setactive() {
	active_player = this;
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

int	picture_info::select(short unsigned* result, unsigned count, const char* mask) {
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

int picture_info::random(const char* mask) {
	short unsigned source[256];
	auto count = select(source, sizeof(source) / sizeof(source[0]), mask);
	if(!count)
		return -1;
	return source[rand() % count];
}

void character::changed(void* object, const void* previous) {
	auto p1 = (character*)object;
	auto p2 = (character*)previous;
	p1->update_items();
	if(p1->race != p2->race || p1->type != p2->type) {
		p1->correct();
		p1->apply_feats();
		p1->apply_ability_restriction();
	}
	for(auto& e : p1->wears) {
		if(!e)
			break;
		if(e.isready())
			continue;
		e.setready(1);
	}
}

void character::correct() {
	// Обнулим классы, которые не надо выводить
	for(int i = bsmeta<class_s>::data[type].classes.count; i < sizeof(levels) / sizeof(levels[0]); i++)
		levels[i] = 0;
	// Откорректируем класс
	if(!isallow(type)) {
		auto old = type;
		do {
			type = (class_s)(type + 1);
			if(type > FighterMageTheif)
				type = Cleric;
		} while(type != old && !isallow(type));
	}
	// Откорректируем мировозрение
	if(!isallow(alignment)) {
		auto old = alignment;
		do {
			alignment = (alignment_s)(alignment + 1);
			if(alignment > ChaoticEvil)
				alignment = LawfulGood;
		} while(alignment != old && !isallow(alignment));
	}
}

const char* character::getdescription(const character* p, char* result, const char* result_max) {
	stringcreator sc(result, result_max);
	sc.addn("Str:%1i, Int:%2i, Con:%3i", p->get(Strenght), p->get(Intellegence), p->get(Constitution));
	sc.addn("HD:%1i, AC:%2i, HP:%3i", p->getlevel(), p->get(AC), p->gethpmax());
	return sc;
}

void character::choose_avatar(void* object) {
	auto p = (short unsigned*)object;
	picture_info::choose(*p, "Какую картинку использовать?", 0, 64);
}

void character::rollhp() {
	if(!hp_rolled) {
		hp_rolled = xrand(1, gethprollmax());
		hp = gethpmax();
	}
}

result_s character::attack(attack_info& ai, character* enemy) {
	if(is(BonusToHitOrcs) && enemy->race == Goblinoid)
		ai.bonus++;
	if(is(HuntEnemy) && (enemy->race == Goblinoid || (enemy->race == Humanoid && enemy->islarge())))
		ai.damage.b += getlevel();
	auto chance_hit = (enemy->get(AC) + ai.bonus) * 5;
	auto chance_crit = (1 + ai.critical) * 5;
	if(chance_hit < chance_crit)
		chance_hit = chance_crit;
	auto result_value = d100();
	if(result_value > chance_hit) {
		act("%герой промазал%а");
		combat_info::animate(this, enemy, false);
		return Fail;
	}
	result_s result = Success;
	if(result_value < chance_crit && (chance_hit - chance_crit) <= 10)
		result = CriticalSuccess;
	if(result == CriticalSuccess)
		act("%герой [+критически попал%а].");
	else
		act("%герой попал%а.");
	if(result == CriticalSuccess) {
		auto chance_deflect = enemy->get(CriticalDeflection);
		if(chance_deflect > 0) {
			auto value = d100();
			if(value < chance_deflect) {
				act("Критический удар удачно предотвращен доспехами.");
				result = Success;
			}
		}
	}
	auto damage = ai.damage.roll();
	if(result == CriticalSuccess) {
		ai.damage.b = 0;
		for(auto i = ai.multiplier; i >= 0; i--)
			damage += ai.damage.roll();
	}
	enemy->damage(ai.type, damage);
	combat_info::animate(this, enemy, true);
	return result;
}

result_s character::attack(item* weapon, character* enemy) {
	attack_info ai = {}; get(weapon, ai, enemy->islarge());
	return attack(ai, enemy);
}

void character::damage(effect_s type, int v) {
	if(v < 0)
		return;
	hp -= v;
	act("%герой получил%а [%1i] урона", v);
	if(hp <= 0)
		act("и умер%ла");
	act(".");
}

item* character::get(item_type_s v) const {
	for(auto& e : wears) {
		if(!e.is(v))
			continue;
		if(e.isready())
			return const_cast<item*>(&e);
	}
	return 0;
}

void character::update_items() {
	unsigned i = 0;
	for(auto& e : wears) {
		if(!e)
			continue;
		wears[i++] = e;
	}
	for(; i < sizeof(wears) / sizeof(wears[0]); i++)
		wears[i].clear();
}

void character::act(const char* text, ...) const {
	msg_logger.name = getname();
	msg_logger.gender = gender;
	msg_logger.addx(' ', text, xva_start(text));
}

int	character::getreach(const item* weapon) const {
	if(weapon)
		return weapon->getreach();
	return 1;
}

void character::refresh() {
	auto m = getmovement();
	current_movement += m;
	if(current_movement > m)
		current_movement = m;
}

void character::stop() {
	current_movement = 0;
}