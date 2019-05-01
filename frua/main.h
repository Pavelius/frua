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
	Goblinoid, Humanoid, Animal, Insectoid, Demon, Dragon, Undead,
};
enum alignment_s : unsigned char {
	LawfulGood, NeutralGood, ChaoticGood,
	LawfulNeutral, TrueNeutral, ChaoticNeutral,
	LawfulEvil, NeutralEvil, ChaoticEvil
};
enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
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
enum wear_s : unsigned char {
	Head, Neck, Armor, MeleeWeapon, OffhandWeapon, RangedWeapon, GridleWear, LeftRing, RightRing, Legs,
	Backpack, Book, Wand, Scroll, Potion, Edible, Gem,
	FirstWear = Head, LastWear = Gem,
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
	Darkvision,
	BonusSaveVsPoison, BonusSaveVsWands, BonusSaveVsSpells, BonusHits,
	DetectSecretDoors, DetectUndegroundPassages, CharmResistance,
	ElfWeaponTraining, BonusToHitOrcs, SmallSizeCombatAdvantage, LightSteps,
	HolyGrace, NoExeptionalStrenght,
	LightSensivity,
	UniqueCharacter,
};
enum usability_s : unsigned char {
	UseLeatherArmor, UseMetalArmor, UseShield,
	UseTheifWeapon, UseMartialWeapon, UseLargeWeapon,
	UseMagicDevice, UseScrolls,
};
enum reaction_s : unsigned char {
	Indifferent, Friendly, Flight, Cautions, Threatening, Hostile, Player,
};
enum effect_s : unsigned char {
	NoEffect,
	Bludgeon, Piercing, Slashing,
	Acid, Cold, Electricity, Fire,
	FirstDamage = Bludgeon, LastDamage=Fire,
	Death, Petrification, Paralize,
	WeakPoison, Poison, StrongPoison, DeathPoison,
};
enum size_s : unsigned char {
	Small, Medium, Large,
};
enum timezone_s : unsigned char {
	Morning, Noon, Afternoon, Evening, Midnight, Night,
};
enum magic_item_s : unsigned char {
	Any,
	Potions, Oils, Scrolls, Armors, Shields, Weapons, Rings, Rods, Staves, Wands,
	Books, Tomes, Jewels, Jewelry, Cloacks, Robes, Boots, Gloves,
	Gridles, Helms, Bags, Bottles, Dusts, Stones, Tools,
	MusicalInstruments, WeirdStuffs,
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
	Ram, Regeneration, ShockingGrasp, ShootingStars, SpellStoring, SpellTurning,
	Sustenance, SwimmingPower, Telekinesis, Truth, Warmth, WaterWalking, Weakness, Wizardry, DarkvisionPower,
};
enum gender_s : unsigned char {
	Male, Female,
};
enum direction_s : unsigned char {
	Up, Right, Down, Left,
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
enum decoration_value_s {
	Name, Description,
	Avatar, Grade,
};
enum damage_feat_s : unsigned char {
	SaveHalf, NoSave,
	AttackAdditional, AttackOnCritical,
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

struct character;
struct draw_events;
struct item;
struct sprite;

typedef alignment_s			alignmenta[8];
typedef race_s				racea[8];
typedef class_s				classa[3];

enum variant_s : unsigned char {
	NoVariant,
	Races, Classes, Alignments
};
struct variant {
	variant_s				type;
	union {
		race_s				race;
		class_s				clas;
		alignment_s			alignment;
		unsigned char		value;
	};
	constexpr variant() : type(NoVariant), value(0) {}
	constexpr variant(race_s v) : type(Races), race(v) {}
	constexpr variant(class_s v) : type(Classes), clas(v) {}
	constexpr variant(alignment_s v) : type(Alignments), alignment(v) {}
	constexpr bool operator==(const variant& e) const { return type == e.type && value == e.value; }
};
struct decoration {
	typedef void(*commandp)(void* p);
	const char*				name;
	bsdata*					database;
	const bsreq*			meta;
	unsigned				type_size;
	markup::proci			proc;
	point					size;
	const markup*			markups;
	const char*				zero_element;
	static decoration		data[];
	template<class T> decoration(const char* name, point size, const T& object, const char* zero_element = 0) : name(name), size(size),
		meta(bsmeta<T>::meta), type_size(sizeof(T)), database(&bsmeta<T>::data),
		proc{T::getname, T::getvalue},
		markups(T::markups), zero_element(zero_element) {}
	template<class T> decoration(const char* name, const T& object) : name(name), size(),
		meta(bsmeta<T>::meta), type_size(sizeof(T)), database(0),
		proc{T::getname, T::getvalue},
		markups(T::markups), zero_element(0) {}
	int						choose(const char* title, int width, int height, bool choose_mode) const;
	static int				choose(const bsreq* type, bool choose_mode = true);
	static bool				choose(void** result, const bsreq* type);
	static void				initialize();
	template<class T> static bool choose(T*& result) { return choose((void**)&result); }
	static bool				edit(bsdata& source, void* object, void* copy_object = 0, const char* form_name = 0);
	static bool				edit(const char* name, void* object, unsigned size, const bsreq* type, const markup* elements = 0, bool creating = false);
	template<class T> static bool edit(T* p) { return edit(0, p, sizeof(T), bsmeta<T>::meta); }
	static void				editlist(const bsreq* type) { choose(type, false); }
	static const decoration* find(const bsreq* type);
};

struct name_info {
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
struct size_info {
	const char*				id;
	const char*				name;
};
struct damage_feat_info {
	const char*				id;
	const char*				name;
};
struct wear_info {
	const char*				id;
	const char*				name;
	const char*				name_type;
	wear_s					wear[2];
	wear_type_s				item_type;
	char					use_damage;
	char					use_armor;
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
	point					size;
	constexpr picture_info() : folder(0), id(0), position(), size() {}
	explicit constexpr operator bool() const { return id != 0; }
	bool operator==(const picture_info& e) const;
	static bool				choose(short unsigned& result, const char* title, const char* mask, int size);
	static const picture_info* choose_image();
	static void				edit_monsters();
	const char*				geturl(char* temp) const;
	const char*				geturl(char* temp, int part) const;
	static int				random(const char* mask);
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
struct event_info {
	picture_info			picture;
};
struct dice_info : dice {
	static markup			markups[];
	void					getname(stringcreator& sc) const;
	static const char*		getname(const void* object, char* result, const char* result_max, int id);
	static int				getvalue(const void* object, int id) { return 0; }
};
struct damage_info {
	char					attacks; // per two rounds
	effect_s				type;
	char					bonus;
	char					range; // in squars (5x5 ft each)
	dice_info				damage;
	cflags<damage_feat_s>	feats;
	//
	static markup			body_markup[], body_full_markup[];
	static void				edit(void* p);
	void					getname(stringcreator& sc) const;
	//
	static markup			markups[];
	static const char*		getname(const void* object, char* result, const char* result_max, int id);
	static int				getvalue(const void* object, int id) { return 0; }
	static bool				visible_damage(const void* object, int index);
};
struct weapon_info : damage_info {
	char					critical, multiplier;
	dice_info				damage_large;
	explicit constexpr operator bool() const { return damage.d != 0; }
	static markup			body_markup[];
	static void				edit(void* p);
	void					getname(stringcreator& sc);
	static bool				isweapon(const void* object, int param);
	//
	static markup			markups[];
	static const char*		getname(const void* object, char* result, const char* result_max, int id);
	static int				getvalue(const void* object, int id) { return 0; }
};
struct armor_info {
	char					ac, toughness;
	char					critical;
	//
	static markup			markups[];
	static const char*		getname(const void* object, char* result, const char* result_max, int id) { return ""; }
	static int				getvalue(const void* object, int id) { return 0; }
};
struct attack_info : weapon_info {
	item*					weapon;
	char*					getattacks(char* result, const char* result_maximum) const;
};
struct item_info {
	const char*				name;
	const char*				name_unidentified;
	wear_s					type;
	cflags<usability_s>		usability;
	weapon_info				damage;
	damage_info				special_attack;
	armor_info				armor;
	char					abilities[Charisma + 1];
	char					skills[LastSkill + 1];
	int						cost, weight;
	static int				view_special(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_weapon(int x, int y, int width, const void* object, const char* id, int index);
	// Database engine methods
	static markup			markups[];
	static const char*		getname(const void* object, char* result, const char* result_max, int id);
	static int				getvalue(const void* object, int id);
};
struct item {
	unsigned short			type;
	union {
		struct {
			item_state_s	state : 2;
			unsigned char	quality : 2;
			unsigned char	identify : 1;
			unsigned char	damaged : 3;
		};
		unsigned char		value;
	};
	constexpr operator bool() const { return type != 0; }
	static markup			markups[];
	void					getname(stringcreator& sc) const;
	static const char*		getname(const void* object, char* result, const char* result_max, int id);
	static int				getvalue(const void* object, int id) { return 0; }
	static int				view_check(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_state(int x, int y, int width, const void* object, const char* id, int index);
};
struct character {
	operator bool() const { return name != 0; }
	void					addbattle();
	void					apply_ability_restriction();
	static void				apply_avatar(void* object);
	void					apply_feats();
	static void				choose_avatar(void* object);
	void					clear();
	static void				clear(void* object) { ((character*)object)->clear(); }
	void					correct();
	void					create(race_s race, gender_s gender, class_s type, alignment_s alignment, reaction_s reaction);
	void					get(wear_s id, attack_info& ai) const;
	int						get(ability_s v) const { return abilities[v]; }
	int						get(class_s v) const { return 0; }
	int						get(skill_s v) const;
	int						getac() const;
	static character*		getactive();
	alignment_s				getalignment() const { return alignment; }
	int						getavatar() const { return avatar; }
	class_s					getclass() const { return type; }
	gender_s				getgender() const { return gender; }
	int						gethp() const { return hp; }
	int						gethpmax(int v) const;
	int						gethpmax() const { return gethpmax(hp_rolled); }
	int						gethprollmax() const;
	int						getlevel() const { return levels[0]; }
	int						getlevel(int i) const { return levels[i]; }
	int						getmovement() const;
	const char*				getname() const { return name; }
	race_s					getrace() const { return race; }
	int						getstrex() const;
	int						getstrper() const { return strenght_percent; }
	short unsigned			getposition() const { return index; }
	bool					is(feat_s v) const { return feats.is(v); }
	bool					is(usability_s v) const { return usability.is(v); }
	bool					isalive() const { return hp > 0; }
	static bool				isallow(alignment_s v, class_s type);
	bool					isallow(alignment_s v) const { return isallow(v, type); }
	bool					isallow(class_s v) const { return isallow(v, race); }
	static bool				isallow(class_s v, race_s race);
	bool					isenemy(const character* p) const;
	bool					isplayable() const { return reaction == Player; }
	static const bsreq		metadata[];
	void					raise(class_s v);
	static void				random(void* object);
	void					recreate();
	static void				recreate(void* p) { ((character*)p)->recreate(); }
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
	static int				view_avatar(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_ability(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_levels(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_personal(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_skills(int x, int y, int width, const void* object, const char* id, int index);
	static int				view_statistic(int x, int y, int width, const void* object, const char* id, int index);
	static void				update_battle();
	// Database engine methods
	static void				changed(void* object, const void* previous);
	static markup			markups[];
	static const char*		getname(const void* object, char* result, const char* result_max, int id);
	static int				getvalue(const void* object, int id);
private:
	const char*				name;
	gender_s				gender;
	alignment_s				alignment;
	class_s					type;
	race_s					race;
	reaction_s				reaction;
	direction_s				direction;
	size_s					size;
	char					abilities[Charisma + 1];
	short					hp, hp_rolled;
	char					initiative;
	cflags<feat_s>			feats;
	cflags<usability_s>		usability;
	char					strenght_percent;
	char					movement;
	short unsigned			index;
	short unsigned			avatar;
	char					levels[3];
	char					base_ac;
	item					wears[Legs + 1];
	unsigned				coopers;
	unsigned				experience;
	friend struct bsmeta<character>;
	static int				getindex(class_s type, class_s v);
	void					roll_ability();
};
struct mapcore {
	static const short unsigned Blocked = 0xFFFF;
	static const short unsigned DefaultCost = 0xFFFE;
	static short unsigned	getcost(short unsigned index);
	static void				makewave(short unsigned start_index, short xm, short ym);
	static void				setblock();
	static void				setblock(short unsigned index, short unsigned v);
	static short unsigned	to(short unsigned i, direction_s d, short xm, short ym);
};
template<short XM, short YM> struct map_info : mapcore {
	static const short		xmax = XM, ymax = YM;
	unsigned char			data[XM*YM];
	static point			i2m(short unsigned i) { return {i % XM, i / XM}; }
	static short unsigned	m2i(short x, short y) { return y * XM + x; };
	static short unsigned	m2i(const point pt) { return pt.y * XM + pt.x; };
	static short unsigned	to(short unsigned i, direction_s d) { return mapcore::to(i, d, XM, YM); }
	static short unsigned	random() { return m2i(xrand(0, XM - 1), xrand(0, YM - 1)); }
};
struct combat_info : map_info<combat_map_x, combat_map_y> {
	int						round;
	int						movement;
	adat<character, 32>		enemies;
	adat<character*, 64>	parcipants;
	constexpr combat_info() : round(1), movement(0), enemies(), parcipants() {}
	character*				create(const char* name, reaction_s reaction = Hostile);
	void					addenemies();
	void					addparty();
	void					automove(character* player);
	bool					isenemy() const;
	void					makewave(short unsigned index);
	void					move(character* player);
	bool					move(character* player, direction_s d);
	void					play();
	void					playround();
	void					update();
	void					visualize();
};
DECLENUM(alignment);
DECLENUM(ability);
DECLENUM(class);
DECLENUM(damage_feat);
DECLENUM(effect);
DECLENUM(feat);
DECLENUM(gender);
DECLENUM(item_state);
DECLENUM(race);
DECLENUM(reaction);
DECLENUM(size);
DECLENUM(skill);
DECLENUM(wear);
DECLENUM(usability);
extern aref<sprite_name_info> avatar_data;
extern adat<character*, 8>	party;