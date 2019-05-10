#include "anyval.h"
#include "bsreq.h"
#include "collection.h"
#include "crt.h"
#include "dice.h"
#include "markup.h"
#include "point.h"
#include "stringcreator.h"

#pragma once

const int combat_grid = 32;
const int combat_map_x = 24;
const int combat_map_y = 15;

enum class_s : unsigned char {
	Monster,
	Cleric, Fighter, Mage, Paladin, Ranger, Theif,
	FighterCleric, FighterMage, FighterTheif,
	ClericMage, MageTheif,
	FighterClericMage, FighterMageTheif,
};
enum race_s : unsigned char {
	Human, Dwarf, Elf, Gnome, HalfElf, Halfling,
	Goblinoid, Humanoid, Animal, Insectoid, Demon, Dragon
};
enum alignment_s : unsigned char {
	LawfulGood, NeutralGood, ChaoticGood,
	LawfulNeutral, TrueNeutral, ChaoticNeutral,
	LawfulEvil, NeutralEvil, ChaoticEvil
};
enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
	AC, DR,
	CriticalDeflection, CriticalThread, CriticalMultiplier,
};
enum skill_s : unsigned char {
	SaveVsParalization, SaveVsPoison, SaveVsDeath,
	SaveVsWands,
	SaveVsPetrification, SaveVsPolymorph,
	SaveVsBreathWeapon,
	SaveVsSpells,
	FirstSave = SaveVsParalization, LastSave = SaveVsSpells,
	//
	ResistCold, ResistFire, ResistCharm,
	FirstResist = ResistCold, LastResist = ResistCharm,
	//
	PickPockets, OpenLocks, FindRemoveTraps, MoveSilently, HideInShadows, DetectNoise, ClimbWalls, ReadLanguages,
	SystemShockSurvival, LearnSpell, OpenDoor, LiftGate,
	LastSkill = LiftGate
};
enum god_s : unsigned char {
	Bane, Mystra, Tor, Tempos
};
enum group_s : unsigned char {
	GeneralGroup,
	Warriors, Priests, Rogues, Wizards
};
enum landscape_s : unsigned  char {
	Plain, Brush, Forest, Desert, Hills, Mountains, Swamp, Jungle, Ocean, Arctic,
};
enum school_s : unsigned char {
	NoSchool,
	Abjuration, Alteration, Charm, Conjuration, Divination, Enchantment,
	Invocation, Necromantic,
	Summoning,
};
enum distance_s : unsigned char {
	Self, Touch, Range1, Range3, Range6, Range12,
};
enum duration_s : unsigned char {
	Instantaneous, Concetration, Hour, Day, Month, Year,
	Duration1Round, Duration1RoundPerLevel, Duration1RoundPerLevelPlus1, Duration1RoundPerLevelPlus2, Duration1RoundPerLevelPlus3, Duration1RoundPerLevelPlus1d3, Duration1RoundPerLevelPlus1d4,
	Duration1d10Rounds, Duration2d20Rounds,
	Duration2RoundPerLevel, Duration2RoundPlus1RoundPerLevel,
	Duration4RoundPlus1RoundPerLevel,
	Duration5RoundPerLevel,
	Duration6Round,
	Duration1Turn, Duration1TurnPerLevel, Duration1TurnPlus1Per2Level,
	Duration3Turn,
	Duration5Turn,
	Duration6TurnPlus1PerLevel,
	Duration4Hours,
	Duration10Hours,
	Duration1DayPerLevel,
	Permanent,
};
enum feat_s : unsigned char {
	Darkvision, TurnUndead, HuntEnemy,
	BonusSaveVsPoison, BonusSaveVsWands, BonusSaveVsSpells, BonusHits,
	DetectSecretDoors, DetectUndegroundPassages, CharmResistance90, CharmResistance30,
	ElfWeaponTraining, BonusToHitOrcs, SmallSizeCombatAdvantage, LightSteps,
	HolyGrace, NoExeptionalStrenght,
	LightSensivity,
	UniqueCharacter,
};
enum special_quality_s : unsigned char {
	ImmunityDesease, ImmunityPoison, ImmunityCharm,
	ImmunityCold, ImmunityFire,
	HitMagicWeapon, HitSilverWeapon,
	HalfDamagePierce, HalfDamageSlashing, HalfDamageBludgeon,
	Regeneration, Undead,
};
enum usability_s : unsigned char {
	UseLeatherArmor, UseMetalArmor, UseShield,
	UseTheifWeapon, UseElfWeapon, UseMartialWeapon, UseLargeWeapon,
	UseMagicDevice, UseScrolls,
};
enum reaction_s : unsigned char {
	Indifferent, Friendly, Flight, Cautions, Threatening, Hostile, Player,
};
enum effect_s : unsigned char {
	NoEffect,
	Bludgeon, Piercing, Slashing,
	Acid, Cold, Electricity, Fire,
	FirstDamage = Bludgeon, LastDamage = Fire,
	Death, Petrification, Paralize,
	WeakPoison, Poison, StrongPoison, DeathPoison,
};
enum size_s : unsigned char {
	Small, Medium, Large,
};
enum timezone_s : unsigned char {
	Morning, Noon, Afternoon, Evening, Midnight, Night,
};
enum item_type_s : unsigned char {
	Weapon, Armor, Shield, Helm, Gridle,
	Potion, Oil, Scroll, Ring, Rod, Wand,
	Book, Jewelry, Cloack, Robe, Boots, Gloves,
};
enum spell_s : unsigned char {
	NoSpell,
	// 1 - level cleric
	SpellBless, SpellCommand, SpellCreateWater, SpellCureLightWounds, SpellDetectEvil,
	SpellDetectMagic, SpellLight, SpellProtectionFromEvil, SpellPurifyFoodDrink, SpellRemoveFear,
	SpellResistCold, SpellSanctuary,
	// 2 - level cleric
	SpellAugury, SpellChant, SpellDetectCharm, SpellFindTraps, SpellHoldPerson,
	SpellKnowAlignment, SpellResistFire, SpellSilence15Radius, SpellSlowPoison, SpellSnakeCharm,
	SpellSpeakWithAnimals, SpellSpiritualHammer,
};
enum magic_power_s : unsigned char {
	NoMagicPower,
	// Potions
	AnimalControl, Clairaudience, Clairvoyance, Climbing, Delusion, Diminution, DragonControl,
	Health, Madness, Youth, ESP, ExtraHealing, FireBreath, FireResistance, Flying, GaseousForm,
	GiantControl, GiantStrength, Growth, HealingPower, Heroism, HumanControl, Invisibility,
	Invulnerability, Levitation, Longevity,
	AcidResistance, Disenchantment, ElementalInvulnerability,
	Etherealness, FieryBurning, Fumbling, Impact, Slipperiness,
	Timelessness, Glibness, Love, Persuasiveness, StammeringAndStuttering,
	PlantControl, Polymorph, RainbowHues, Speed, SuperHeroism,
	SweetWater, TreasureFinding, UndeadControl, VentriloquismPower, Vitality, WaterBreathing,
	// Rings
	Blinking, ChameleonPower, Clumsiness, Contrariness, DjinniSummoning, ElementalControl, FeatherFalling,
	FreeAction, JumpingPower, MindShielding, Protection,
	Ram, ShockingGrasp, ShootingStars, SpellStoring, SpellTurning,
	Sustenance, SwimmingPower, Telekinesis, Truth, Warmth, WaterWalking, Weakness, Wizardry, DarkvisionPower,
};
enum gender_s : unsigned char {
	Male, Female,
};
enum direction_s : unsigned char {
	Up, Right, Down, Left,
	Center,
};
enum item_state_s : unsigned char {
	Mundane, Cursed, Magic, Artifact
};
enum wear_type_s : unsigned char {
	StandartItem,
	ConsumableItem, ChargeableItem
};
enum grade_s : unsigned char {
	Fair, Good, Excellent,
	Bad,
};
enum result_s : unsigned char {
	Fail, Success, CriticalSuccess,
};
enum save_s : unsigned char {
	NoSave, SaveHalved, SaveNegate,
};
enum attack_affect_s : unsigned char {
	AttackRegular, AttackOneAndHalf, AttackDouble,
	AttackAdditional, AttackOnAnyOneHit, AttackOnAllHit, AttackOnCritical
};
enum action_s : unsigned char {
	Guard, Move,
};

const unsigned CP = 1; // One cooper coin
const unsigned SP = 10; // One silver coin
const unsigned GP = 100; // One gold coin
const unsigned PP = 500; // One platinum or electra coin

const unsigned RRound = 1;
const unsigned RTurn = RRound * 10;
const unsigned RHour = RTurn * 6;
const unsigned RDay = 24 * RHour;
const unsigned RMonth = 30 * RDay;
const unsigned RYear = 12 * 30 * RDay;

class item;
class character;
struct sprite;

enum variant_s : unsigned char {
	NoVariant,
	Alignments, Attacks, Classes, Directions, Races
};
struct variant {
	variant_s				type;
	union {
		action_s			action;
		attack_affect_s		attack;
		direction_s			direction;
		race_s				race;
		class_s				clas;
		alignment_s			alignment;
		unsigned char		value;
	};
	constexpr variant() : type(NoVariant), value(0) {}
	constexpr variant(action_s v) : type(Attacks), action(v) {}
	constexpr variant(alignment_s v) : type(Alignments), alignment(v) {}
	constexpr variant(attack_affect_s v) : type(Attacks), attack(v) {}
	constexpr variant(class_s v) : type(Classes), clas(v) {}
	constexpr variant(direction_s v) : type(Directions), direction(v) {}
	constexpr variant(race_s v) : type(Races), race(v) {}
	constexpr variant(const int v) : type((variant_s)(v >> 8)), value(v & 0xFF) {}
	constexpr bool operator==(const variant& e) const { return type == e.type && value == e.value; }
	constexpr operator int() const { return ((unsigned char)type << 8) | value; }
};
struct decoration {
	const char*				name;
	bsdata*					database;
	const bsreq*			meta;
	unsigned				type_size;
	point					size;
	const markup*			markups;
	const char*				zero_element;
	static decoration		data[];
	template<class T> decoration(const char* name, point size, const T& object, const char* zero_element = 0) : name(name), size(size),
		meta(bsmeta<T>::meta), type_size(sizeof(T)), database(&bsmeta<T>::data),
		markups(T::markups), zero_element(zero_element) {}
	template<class T> decoration(const char* name, const T& object) : name(name), size(),
		meta(bsmeta<T>::meta), type_size(sizeof(T)), database(0),
		markups(T::markups), zero_element(0) {}
	int						choose(const char* title, int width, int height, bool choose_mode) const;
	static int				choose(const bsreq* type, bool choose_mode = true);
	static bool				choose(void** result, const bsreq* type);
	static void*			choose(const char* title, void** source, unsigned count, const bsreq* type, const markup* columns);
	static void				database_export();
	static void				database_import();
	static void				initialize();
	template<class T> static bool choose(T*& result) { return choose((void**)&result); }
	static bool				edit(bsdata& source, void* object, void* copy_object = 0, const char* form_name = 0);
	static bool				edit(const char* name, void* object, unsigned size, const bsreq* type, const markup* elements = 0, bool creating = false);
	static bool				edit(const char* name, void* object, const bsreq* type, const markup* elements);
	template<class T> static bool edit(T* p) { return edit(0, p, sizeof(T), bsmeta<T>::meta); }
	static void				editlist(const bsreq* type) { choose(type, false); }
	static const decoration* find(const bsreq* type);
};
struct name_info {
	const char*				id;
	const char*				name;
};
struct special_quality_info {
	const char*				id;
	const char*				name;
};
struct direction_info {
	const char*				id;
	const char*				name;
	unsigned				key;
};
struct save_info {
	const char*				id;
	const char*				name;
};
struct skill_info {
	const char*				id;
	const char*				name;
	cflags<class_s>			allow;
};
struct ability_info {
	const char*				id;
	const char*				name;
};
struct alignment_info {
	const char*				id;
	const char*				name;
	adat<class_s, 4>		restricted;
};
struct reaction_info {
	const char*				id;
	const char*				name;
};
struct gender_info {
	const char*				id;
	const char*				name;
};
struct dam_info {
	const char*				id;
	const char*				name;
	skill_s					resist;
};
struct feat_info {
	const char*				id;
	const char*				name;
};
struct effect_info {
	const char*				id;
	const char*				name;
	skill_s					save;
};
struct item_feat_info {
	const char*				id;
	const char*				name;
};
struct item_state_info {
	const char*				id;
	const char*				name;
};
struct item_type_info {
	const char*				id;
	const char*				name;
	char					use_damage;
	char					use_armor;
};
struct size_info {
	const char*				id;
	const char*				name;
};
struct attack_affect_info {
	const char*				id;
	const char*				name;
	char					attacks;
};
struct usability_info {
	const char*				id;
	const char*				name;
};
struct sprite_name_info {
	char					name[32];
};
struct picture_info {
	const char*				folder;
	const char*				id;
	point					position;
	constexpr picture_info() : folder(0), id(0), position() {}
	constexpr picture_info(const char* folder, const char* id) : folder(folder), id(id), position() {}
	explicit constexpr operator bool() const { return id != 0; }
	bool operator==(const picture_info& e) const;
	static bool				choose(short unsigned& result, const char* title, const char* mask, int size);
	static const picture_info* choose_image();
	static void				edit_monsters();
	const char*				geturl(char* temp) const;
	const char*				geturl(char* temp, int part) const;
	static int				random(const char* mask);
	void					set(const char* folder, const char* id);
	static int				select(short unsigned* result, unsigned count, const char* mask);
};
struct class_info {
	const char*				id;
	const char*				name;
	char					playable;
	char					hd;
	ability_s				ability;
	adat<class_s, 4>		classes;
	cflags<feat_s>			feats;
	cflags<usability_s>		usability;
	char					minimum[Charisma + 1];
	adat<race_s, 12>		races;
	char					bonus_hd;
};
struct race_info {
	const char*				id;
	const char*				name;
	char					playable;
	char					minimum[Charisma + 1];
	char					maximum[Charisma + 1];
	char					adjustment[Charisma + 1];
	char					theive_skills[(ReadLanguages - PickPockets) + 1];
	cflags<feat_s>			feats;
	cflags<usability_s>		usability;
	size_s					size;
	char					movement;
	const char*				info;
};
struct dice_info : dice {
	static markup			markups[];
	void					getname(stringcreator& sc) const;
};
struct damage_info {
	effect_s				type;
	attack_affect_s			attack_type;
	save_s					save_type;
	char					range; // in squars (5x5 ft each)
	dice_info				damage;
	//
	static void				edit(void* p);
	int						getattacks() const;
	void					getattacks(stringcreator& sc) const;
	void					getname(stringcreator& sc) const;
	bool					ismelee() const { return range <= 2; }
	bool					isranged() const { return range >= 3; }
	static markup			weapon_markup[];
	static markup			markups[];
};
struct attack_info : damage_info {
	item*					weapon;
	char					bonus, critical, multiplier;
};
struct item_info {
	const char*				name;
	const char*				name_unidentified;
	item_type_s				type;
	cflags<usability_s>		usability;
	damage_info				damage;
	dice_info				damage_large;
	damage_info				special;
	char					abilities[CriticalMultiplier + 1];
	char					skills[LastSkill + 1];
	int						cost, weight;
	//
	static void				apply(item_info* p);
	static void				create(item_info* p);
	static void				editweapon(void* p);
	void					getweapon(stringcreator& sc) const;
	static int				view_special(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_weapon(int x, int y, int width, const void* object, const char* id, int index);
	// Database engine methods
	static markup			markups[];
	static const char*		getweapon(const void* object, char* result, const char* result_max, int id);
};
class item {
	unsigned short			type;
	union {
		struct {
			item_state_s	state : 2;
			unsigned char	quality : 2;
			unsigned char	identify : 1;
			unsigned char	ready : 1; // Ready to use
			unsigned char	damaged : 2;
		};
		unsigned char		value;
	};
	friend struct bsmeta<item>;
public:
	constexpr operator bool() const { return type != 0; }
	void					clear();
	int						get(ability_s id) const;
	const item_info&		getinfo() const { return bsmeta<item_info>::elements[type]; }
	item_type_s				getkind() const { return getinfo().type; }
	void					getname(stringcreator& sc) const;
	int						getdamaged() const { return damaged; }
	int						getq(ability_s id) const;
	int						getquaility() const;
	int						getreach() const;
	bool					is(item_state_s v) const { return state == v; }
	bool					is(item_type_s v) const { return getinfo().type == v; }
	bool					isallow(unsigned mask) const;
	bool					isarmor() const { return bsmeta<item_type_info>::elements[getinfo().type].use_armor != 0; }
	bool					isidentified() const { return identify != 0; }
	bool					isready() const { return ready != 0; }
	bool					iswearable() const { return isweapon() || isarmor(); }
	bool					isweapon() const { return bsmeta<item_type_info>::elements[getinfo().type].use_damage != 0; }
	static markup			markups[];
	void					set(item_state_s v) { state = v; }
	void					setidentify(unsigned char v) { identify = v; }
	void					setquality(unsigned char v) { quality = v; }
	void					setready(int v) { ready = v; }
	static int				view_check(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_state(int x, int y, int width, const void* object, const char* id, int index);
};
class character {
	const char*				name;
	gender_s				gender;
	alignment_s				alignment;
	class_s					type;
	race_s					race;
	reaction_s				reaction;
	direction_s				direction;
	size_s					size;
	char					abilities[CriticalMultiplier + 1];
	short					hp, hp_rolled;
	char					initiative;
	cflags<feat_s>			feats;
	cflags<usability_s>		usability;
	cflags<special_quality_s> special_quality;
	char					strenght_percent;
	char					movement, current_movement;
	short unsigned			index;
	short unsigned			avatar;
	char					levels[3];
	char					base_ac;
	unsigned				coopers;
	unsigned				experience;
	item					wears[12 * 2];
	friend struct bsmeta<character>;
	static int				getindex(class_s type, class_s v);
	void					roll_ability();
public:
	operator bool() const { return name != 0; }
	void					act(const char* text, ...) const;
	void					addbattle();
	void					apply_ability_restriction();
	static void				apply_avatar(void* object);
	void					apply_feats();
	result_s				attack(item* weapon, character * enemy);
	static void				changed(void* object, const void* previous);
	static markup			charsheet_markup[];
	static void				choose_avatar(void* object);
	void					clear();
	static void				clear(void* object) { ((character*)object)->clear(); }
	void					correct();
	void					create(race_s race, gender_s gender, class_s type, alignment_s alignment, reaction_s reaction);
	void					damage(effect_s type, int v);
	void					get(item* weapon, attack_info& ai, bool large_enemy = false) const;
	int						get(ability_s v) const;
	int						get(class_s v) const { return 0; }
	item*					get(item_type_s v) const;
	int						get(skill_s v) const;
	static character*		getactive();
	alignment_s				getalignment() const { return alignment; }
	int						getavatar() const { return avatar; }
	int						getbonus(ability_s v) const;
	class_s					getclass() const { return type; }
	static const char*		getdescription(const character* object, char* result, const char* result_max);
	gender_s				getgender() const { return gender; }
	int						gethp() const { return hp; }
	int						gethpmax(int v) const;
	int						gethpmax() const { return gethpmax(hp_rolled); }
	int						gethprollmax() const;
	int						getlevel() const { return levels[0]; }
	int						getlevel(int i) const { return levels[i]; }
	int						getmovement() const;
	int						getmovepoints() const { return current_movement; }
	const char*				getname() const { return name; }
	race_s					getrace() const { return race; }
	int						getreach(const item* weapon) const;
	int						getstrex() const;
	int						getstrper() const { return strenght_percent; }
	short unsigned			getposition() const { return index; }
	bool					is(feat_s v) const { return feats.is(v); }
	bool					is(special_quality_s v) const { return special_quality.is(v); }
	bool					is(usability_s v) const { return usability.is(v); }
	bool					isalive() const { return hp > 0; }
	static bool				isallow(alignment_s v, class_s type);
	bool					isallow(alignment_s v) const { return isallow(v, type); }
	bool					isallow(class_s v) const { return isallow(v, race); }
	static bool				isallow(class_s v, race_s race);
	bool					isallow(const item& e) const { return e.isallow(usability.data); }
	static bool				isallowwear(const void* object, int param);
	bool					isenemy(const character* p) const;
	bool					islarge() const { return size == Large; }
	bool					isplayable() const { return reaction == Player; }
	bool					isready() const { return isalive() && current_movement > 0; }
	static markup			markups[];
	static const bsreq		metadata[];
	bool					moveto(direction_s d);
	void					raise(class_s v);
	static void				random(void* object);
	void					recreate();
	static void				recreate(void* p) { ((character*)p)->recreate(); }
	void					refresh();
	void					reroll();
	static void				reroll(void* p) { ((character*)p)->reroll(); }
	void					rollhp();
	void					set(direction_s v) { direction = v; }
	void					set(feat_s v) { feats.add(v); }
	void					set(usability_s v) { usability.add(v); }
	void					set(reaction_s v) { reaction = v; }
	void					setactive();
	void					setavatar(int v) { avatar = v; }
	void					setname(const char* v) { name = v; }
	void					setposition(short unsigned v) { index = v; }
	void					stop();
	void					update_items();
	static int				view_avatar(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_ability(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_levels(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_personal(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_skills(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_statistic(int x, int y, int width, const void* object, const char* id, int index);
};
struct mapcore {
	enum block_s : short unsigned {DefaultCost = 0xFFFE, Blocked};
	static const direction_s all_directions[4];
	static short unsigned	getcost(short unsigned index);
	static direction_s		getdirection(short unsigned i1, short unsigned i2, short xm, short ym);
	static short unsigned	getnear(short unsigned index, short xm, short ym);
	static bool				isreachable(short unsigned index, short xm, short ym);
	static void				makewave(short unsigned start_index, short xm, short ym, short unsigned block_value = Blocked);
	static void				setblock();
	static void				setblock(short unsigned index, short unsigned v);
	static direction_s		step(short unsigned i, short xm, short ym);
	static direction_s		step(short unsigned i, short xm, short ym, short range);
	static short unsigned	to(short unsigned i, direction_s d, short xm, short ym);
};
template<short XM, short YM> struct map_info : mapcore {
	static const short		xmax = XM, ymax = YM;
	unsigned char			data[XM*YM];
	static direction_s		getdirection(short unsigned i1, short unsigned i2) { return mapcore::getdirection(i1, i2, XM, YM); }
	static point			i2m(short unsigned i) { return {i % XM, i / XM}; }
	static short unsigned	m2i(short x, short y) { return y * XM + x; };
	static short unsigned	m2i(const point pt) { return pt.y * XM + pt.x; };
	static short unsigned	random() { return m2i(xrand(0, XM - 1), xrand(0, YM - 1)); }
	static direction_s		step(short unsigned i) { return mapcore::step(i, XM, YM); }
	static short unsigned	to(short unsigned i, direction_s d) { return mapcore::to(i, d, XM, YM); }
};
struct stringobject : stringcreator {
	const char*				name;
	gender_s				gender;
	stringobject(char* result, const char* result_max) : stringcreator(result, result_max),
		name(""), gender(Male) {}
	void					addidentifier(const char* id) override;
};
struct combat_info : map_info<combat_map_x, combat_map_y> {
	int						round;
	adat<character, 32>		enemies;
	adat<character*, 64>	parcipants;
	constexpr combat_info() : round(1), enemies(), parcipants() {}
	static void				animate(const character* attacker, const character* defender, bool hit);
	character*				create(const char* name, reaction_s reaction = Hostile);
	void					addenemies();
	void					addparty();
	void					automove(character* player, character* enemy);
	static combat_info*		getactive();
	int						getdistance(short unsigned index) const;
	character*				getenemy(const character* player) const;
	short unsigned			getmovecost(short unsigned index) const;
	bool					isenemy() const;
	void					makewave(short unsigned index, short unsigned dest_index = Blocked) const;
	void					play();
	void					playround();
	void					playmove(character* player, character* enemy);
	void					splash(unsigned seconds = 100, bool use_update = true) const;
	void					update() const;
	void					visualize(bool use_update) const;
};
struct answer {
	struct element {
		int					id;
		const char*			name;
		int					priority;
		unsigned			key;
	};
	adat<element, 32>		elements;
	void					ask(int id, const char* name, int priority = 0, unsigned key = 0);
	int						choose(combat_info& ci);
	int						choose(const char* title, const picture_info& pi);
};
class scene : public stringcreator, public answer {
	char					buffer[2048];
public:
	constexpr scene() : stringcreator(buffer), buffer() {}
	int						choose();
	picture_info			img;
};
DECLENUM(alignment);
DECLENUM(ability);
DECLENUM(attack_affect);
DECLENUM(class);
DECLENUM(direction);
DECLENUM(effect);
DECLENUM(feat);
DECLENUM(gender);
DECLENUM(item_state);
DECLENUM(item_type);
DECLENUM(race);
DECLENUM(reaction);
DECLENUM(save);
DECLENUM(size);
DECLENUM(skill);
DECLENUM(special_quality);
DECLENUM(usability);
extern aref<sprite_name_info> avatar_data;
extern adat<character*, 8>	party;
extern stringobject			msg_logger;