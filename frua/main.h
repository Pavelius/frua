#include "anyval.h"
#include "bsreq.h"
#include "collection.h"
#include "crt.h"
#include "dice.h"
#include "point.h"
#include "stringcreator.h"

#pragma once

const int combat_grid = 32;
const int combat_map_x = 16;
const int combat_map_y = 16;

#define assert_enum(e, last) static_assert(sizeof(e##_data) / sizeof(e##_data[0]) == last + 1, "Invalid count of " #e " elements");\
const bsreq bsmeta<e##_info>::meta[] = {BSREQ(id), BSREQ(name), {}};\
bsdatat<e##_info> bsmeta<e##_info>::data(#e, e##_data, KindEnum);

enum item_s : unsigned char {
	NoItem,
	Axe, BattleAxe, Mace, MorningStar, Hammer,
	Spear, Staff, Halberd,
	Dagger, ShortSword, LongSword, BastardSword, TwoHandedSword, Scimitar,
	ShortBow, LongBow, Crossbow, Sling,
	FirstWeapon = Axe, LastWeapon = Sling,
	//
	LeatherArmor, StuddedLeatherArmor, ChainMail, ScaleMail, Brigandine, PlateMail, RingMail, SplintMail, BandedMail, FieldPlate, FullPlate,
	Shield,
	FirstArmor = LeatherArmor, LastArmor = Shield,
	//
	Stone, Arrow, Bolt,
	Potion, Scroll, Ring, Rod, Wand, Book, Tome, Necklage, Cloack, Robe, Boot, Glove, Gridle,
	Helm, Bag, Bottle, Dust,
	Flute, Guitar,
	Raspberry, Grain,
	// Gems
	Azurite, BandedAgate, BlueQuartz, EyeAgate, Hematite, LapisLazuli, Malachite, MossAgate, Obsidian, Rhodochrosite, TigerEyeAgate, Turquoise,
	Bloodstone, Chalcedony, Chrysoprase, Citrine, Jasper, Moonstone, Onyx, RockCrystal, Sardonyx, SmokyQuartz, StarRoseQuartz, Zircon,
	Amber, Alexandrite, Amethyst, Aquamarine, Chrysoberyl, Coral, Garnet, Jade, Jet, Pearl, Peridot, Spinel, Topaz, Tourmaline,
	BlackOpal, BlackSapphire, Diamond, Emerald, FireOpal, Jacinth, Opal, OrientalAmethyst, OrientalEmerald, OrientalTopaz, Ruby, Sapphire, StarRuby, StarSapphire,
	FirstGem = Azurite, LastGem = StarSapphire,
	LastItem = StarSapphire
};
enum class_s : unsigned char {
	Monster,
	Cleric, Fighter, Mage, Paladin, Ranger, Theif,
	FighterCleric, FighterMage, FighterTheif,
	ClericMage, MageTheif,
	FighterClericMage, FighterMageTheif,
};
enum race_s : unsigned char {
	Human, Dwarf, Elf, Gnome, HalfElf, Halfling,
	Humanoid, Animal, Insectoid, Demon, Dragon, Undead,
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
	Head, Neck, Armor, MeleeWeapon, OffhandWeapon, RangedWeapon, GridleWear, Legs,
	FirstWear = Head, LastWear = Legs
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
	UseLeatherArmor, UseMetalArmor, UseShield,
	UseTheifWeapon, UseMartialWeapon, UseLargeWeapon,
	UniqueCharacter,
};
enum reaction_s : unsigned char {
	Indifferent, Friendly, Flight, Cautions, Threatening, Hostile, Player,
};
enum dam_s : unsigned char {
	Bludgeon, Piercing, Slashing,
	Acid, Cold, Electricity, Fire,
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
	PlantControl, Poison, Polymorph, RainbowHues, Speed, SuperHeroism,
	SweetWater, TreasureFinding, UndeadControl, VentriloquismPower, Vitality, WaterBreathing,
	// Rings
	Blinking, ChameleonPower, Clumsiness, Contrariness, DjinniSummoning, ElementalControl, FeatherFalling,
	FreeAction, JumpingPower, MindShielding, Protection,
	Ram, Regeneration, ShockingGrasp, ShootingStars, SpellStoring, SpellTurning,
	Sustenance, SwimmingPower, Telekinesis, Truth, Warmth, WaterWalking, Weakness, Wizardry, DarkvisionPower,
	// Character status non magic
	Surprised, Blessed, Lighted,
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
enum attack_feat_s : unsigned char {
	PreviousAttackHit,
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
typedef cflags<feat_s>		feata;
typedef class_s				classa[3];

struct name_info {
	const char*				id;
	const char*				name;
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
struct gender_info {
	const char*				id;
	const char*				name;
};
struct dam_info {
	const char*				id;
	const char*				name;
};
struct feat_info {
	const char*				id;
	const char*				name;
};
struct size_info {
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
	static const picture_info* choose_image();
	static const picture_info* edit_monsters();
	const char*				geturl(char* temp) const;
	const char*				geturl(char* temp, int part) const;
};
struct class_info {
	const char*				id;
	const char*				name;
	char					hd;
	ability_s				ability;
	adat<class_s, 4>		classes;
	cflags<feat_s>			feats;
	char					minimum[Charisma + 1];
	adat<race_s, 12>		races;
	char					bonus_hd;
};
struct race_info {
	const char*				id;
	const char*				name;
	char					minimum[Charisma + 1];
	char					maximum[Charisma + 1];
	char					adjustment[Charisma + 1];
	char					theive_skills[(ReadLanguages - PickPockets) + 1];
	feata					feats;
	size_s					size;
	char					movement;
	const char*				info;
};
struct event_info {
	picture_info			picture;
	void					edit();
};
struct damage_info {
	dam_s					type;
	char					thac0;
	char					critical, multiplier;
	char					attacks; // per two rounds
	char					range; // in squars.
	dice					damage;
	dice					damage_large;
	explicit constexpr operator bool() const { return damage.d != 0; }
};
struct attack_info : damage_info {
	char					attacks; // per two rounds
	item*					weapon;
	char*					getattacks(char* result, const char* result_maximum) const;
};
struct special_info : damage_info {
	char					used, use_per_day;
	unsigned				feats;
	bool					edit();
};
struct itemweight {
	item_s					key;
	short					weight;
	static int				compare(const void* p1, const void* p2);
};
struct item {
	item_s					type;
	magic_power_s			power;
	item_state_s			state : 2;
	unsigned char			quality : 2;
	unsigned char			identify : 1;
	unsigned char			charges;
	item() = default;
	constexpr item(item_s type) : type(type), quality(0), state(Mundane), identify(1), power(NoMagicPower), charges(0) {}
	constexpr operator bool() const { return type != NoItem; }
};
struct treasure {
	int						cp, sp, gp, pp, gems, art, magic;
	item					items[64];
	treasure();
	void					add(item value);
	void					add(magic_item_s type);
	void					addarts(int count);
	void					addgems(int count);
	void					addmagic(int count);
	static item				anyart();
	static item_s			anygem();
	static magic_item_s		anymagic();
	static item				anymagic(magic_item_s type, char level = 0);
	void					clear();
	void					generate(char symbol);
	void					generate(const char* type);
	static item				gemquality(item_s type);
};
struct character {
	operator bool() const { return name != 0; }
	void					addbattle();
	void					apply_ability_restriction();
	void					apply_feats();
	void					clear();
	static bool				choose(short unsigned& result, const char* title, const char* mask, int size);
	static bool				choose(const char* title, bsdata& source, const anyval& result, int width, bool choose_mode, void(*edit_proc)());
	static int				select_avatar(short unsigned* result, unsigned count, const char* mask);
	static int				select_avatar(const char* mask);
	void					correct();
	void					create(race_s race, gender_s gender, class_s type, alignment_s alignment, reaction_s reaction);
	bool					edit();
	bool					generate();
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
	int						getlevel() const { return levels[0]; }
	int						getlevel(int i) const { return levels[i]; }
	int						getmovement() const;
	race_s					getrace() const { return race; }
	int						getstrex() const;
	int						getstrper() const { return strenght_percent; }
	short unsigned			getposition() const { return index; }
	void					group_generate(int x, int y, int width);
	bool					is(feat_s v) const { return (feats & (1 << v)) != 0; }
	bool					isalive() const { return hp > 0; }
	static bool				isallow(alignment_s v, class_s type);
	bool					isallow(alignment_s v) const { return isallow(v, type); }
	bool					isallow(class_s v) const { return isallow(v, race); }
	static bool				isallow(class_s v, race_s race);
	bool					isenemy(const character* p) const;
	bool					isplayable() const { return reaction == Player; }
	static const bsreq		metadata[];
	void					raise(class_s v);
	void					set(direction_s v) { direction = v; }
	void					setactive();
	void					setavatar(int v) { avatar = v; }
	void					setname(const char* v) { name = v; }
	void					setposition(short unsigned v) { index = v; }
	static void				update_battle();
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
	unsigned				feats;
	char					strenght_percent;
	char					movement;
	short unsigned			index;
	short unsigned			avatar;
	char					levels[3];
	char					base_ac;
	item					wears[Legs + 1];
	unsigned				coopers;
	unsigned				experience;
	special_info			special_attacks[4];
	friend struct character_view;
	friend struct character_events;
	friend struct bsmeta<character>;
	int						edit_abilities(int x, int y, int width);
	int						edit_attacks(int x, int y, int width);
	int						edit_basic(int x, int y, int width, draw_events* pev);
	int						edit_feats(int x, int y, int width);
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
	character*				add(race_s race, gender_s gender, class_s type, int level = 1, reaction_s reaction = Hostile);
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
DECLENUM(class);
DECLENUM(dam);
DECLENUM(feat);
DECLENUM(gender);
DECLENUM(race);
DECLENUM(size);
extern aref<sprite_name_info> avatar_data;
extern adat<character*, 8>	party;