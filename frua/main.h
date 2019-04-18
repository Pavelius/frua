#include "collection.h"
#include "crt.h"
#include "point.h"

#pragma once

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
	NoClass,
	Cleric, Fighter, Mage, Paladin, Ranger, Theif,
	FighterCleric, FighterMage, FighterTheif,
	ClericMage, MageTheif,
	FighterClericMage, FighterMageTheif,
};
enum race_s : unsigned char {
	Human, Dwarf, Elf, Gnome, HalfElf, Halfling,
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
};
enum god_s : unsigned char {
	Bane, Mystra, Tor, Tempos
};
enum group_s : unsigned char {
	GeneralGroup,
	Warriors, Priests, Rogues, Wizards
};
enum monster_s : unsigned char {
	Character,
	Orc, Rogue,
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
	NoFeat, Darkvision,
	BonusSaveVsPoison, BonusSaveVsWands, BonusSaveVsSpells,
	DetectSecretDoors, DetectUndegroundPassages, CharmResistance,
	ElfWeaponTraining, DwarfCombatTactic, SmallSizeCombatAdvantage, LightSteps,
	HolyGrace,
};
enum reaction_s : unsigned char {
	Indifferent, Friendly, Flight, Cautions, Threatening, Hostile, Player,
};
enum side_s : unsigned char {
	PartySide, EnemySide,
};
enum save_result_s : unsigned char {
	NoSave, Negate, Half,
};
enum damage_s : unsigned char {
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
	NoGender,
	Male, Female,
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

typedef alignment_s			alignmenta[8];
typedef race_s				racea[8];
typedef feat_s				feata[8];
typedef class_s				classa[3];

struct answer {
	int						value;
	const char*				name;
};
struct answers : adat<answer, 32> {
	void					add(int id, const char* text);
private:
	char					text[4096];
};
struct picture_info {
	const char*				folder;
	const char*				id;
	point					position;
	point					size;
	constexpr picture_info() : folder(0), id(0), position(), size() {}
	bool operator==(const picture_info& e) const;
	bool					pick();
};
struct class_info {
	const char*				id;
	const char*				name;
	adat<class_s, 4>		classes;
	cflags<feat_s>			flags;
};
struct damageinfo {
	damage_s				type;
	//dice					damage, damage_large;
	//const dice&			getdamage(size_s value) const;
};
struct attackinfo : damageinfo {
	char					thac0;
	char					attacks_per_two_rounds;
	char					critical;
	char					multiplier;
	struct item*			weapon;
	attackinfo() { clear(); }
	void					clear();
};
struct itemweight {
	item_s					key;
	short					weight;
	static int				compare(const void* p1, const void* p2);
};
struct item {
	enum state_s : unsigned char {
		Mundane, Cursed, Magic, Artifact
	};
	item_s					type;
	magic_power_s			power;
	state_s					state : 2;
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
	character() = default;
	operator bool() const { return name != 0; }
	void					clear();
	int						get(ability_s v) const { return abilities[v]; }
	int						get(class_s v) const { return 0; }
	int						get(skill_s v) const;
	bool					is(feat_s v) const { return (feats & (1 << v)) != 0; }
	static answer*			choose(const picture_info& image, aref<answer> source);
private:
	gender_s				gender;
	alignment_s				alignment;
	class_s					type;
	race_s					race;
	monster_s				monster;
	char					abilities[Charisma + 1];
	short					hp, hp_maximum;
	char					initiative;
	unsigned				feats;
	side_s					side;
	char					strenght_percent;
	short unsigned			name;
	char					levels[3];
	item					wears[Legs + 1];
	unsigned				coopers;
	unsigned				experience;
};
extern adat<character, 128> characters;
extern class_info			class_data[];
extern adat<character*, 8>	party;