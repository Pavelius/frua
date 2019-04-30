#include "main.h"

const bsreq bsmeta<character>::meta[] = {
	BSREQ(name),
	BSREQ(gender),
	BSREQ(alignment),
	BSREQ(type),
	BSREQ(race),
	BSREQ(size),
	BSREQ(abilities),
	BSREQ(feats),
	BSREQ(usability),
	BSREQ(hp), BSREQ(hp_rolled),
	BSREQ(strenght_percent),
	BSREQ(avatar),
	BSREQ(levels),
	BSREQ(base_ac),
	BSREQ(movement),
	BSREQ(reaction),
	BSREQ(wears),
{}};
BSDATA(character, 512);

static bool allow_race(const void* source, int value) {
	return bsmeta<race_s>::data[value].playable != 0;
}
static bool allow_class(const void* source, int value) {
	if(!bsmeta<class_s>::data[value].playable)
		return false;
	return ((character*)source)->isallow((class_s)value);
}
static bool element_allow_class(const void* source, int value) {
	return ((character*)source)->isallow((class_s)value);
}
static bool allow_alignment(const void* source, int value) {
	return ((character*)source)->isallow((alignment_s)value);
}
static markup character_gender_radio[] = {{0, "#radiobuttons", {"gender"}}, {}};
static markup character_race_radio[] = {{0, "#radiobuttons", {"race"}, 0, {0, 0, allow_race}}, {}};
static markup character_class_radio[] = {{0, "#radiobuttons", {"type"}, 0, {0, 0, allow_class}}, {}};
static markup character_alignment_radio[] = {{0, "#radiobuttons", {"alignment"}, 0, {0, 0, allow_alignment}}, {}};
static markup generate_c1[] = {{0, "Пол", {0, 0, character_gender_radio}},
{0, "Раса", {0, 0, character_race_radio}},
{}};
static markup generate_c2[] = {{0, "Классы", {0, 0, character_class_radio}},
{}};
static markup generate_c3[] = {{0, "Мировозрение", {0, 0, character_alignment_radio}},
{}};
static markup generate_markup[] = {{2, 0, {0, 0, generate_c1}},
{3, 0, {0, 0, generate_c2}},
{4, 0, {0, 0, generate_c3}},
{}};
static markup generate_commands[] = {{0, "Перебросить", {}, 0, {0, 0, 0, 0, 0, character::recreate}},
{}};
static markup abilities_personal_group[] = {{0, 0, {"name"}},
{0, 0, {"personal"}, 0, {0, 0, 0, 0, character::view_personal}},
{}};
static markup abilities_levels_group[] = {{0, 0, {"levels"}, 0, {0, 0, 0, 0, character::view_levels}}, {}};
static markup abilities_ability_group[] = {{0, 0, {"abilities"}, 0, {0, 0, 0, 0, character::view_ability}}, {}};
static markup abilities_statistic_group[] = {{0, 0, {"statistics"}, 0, {0, 0, 0, 0, character::view_statistic}}, {}};
static markup abilities_skills_group[] = {{0, 0, {"skills"}, 0, {0, 0, 0, 0, character::view_skills}}, {}};
static markup abilities_c1[] = {{0, "Имя", {0, 0, abilities_personal_group}},
{0, "Атрибуты", {0, 0, abilities_ability_group}},
{0, "Боевая статистика", {0, 0, abilities_statistic_group}},
{}};
static markup abilities_c2[] = {{0, "Уровни и опыт", {0, 0, abilities_levels_group}},
{0, "Навыки", {0, 0, abilities_skills_group}},
{}};
static markup abilities_markup[] = {{3, 0, {0, 0, abilities_c1}},
{4, 0, {0, 0, abilities_c2}},
//{4, 0, {0, 0, generate_c3}},
{}};
static markup generate_form[] = {{0, 0, {"#create"}, 0, {0, 0, 0, 0, 0, character::random}},
{0, 0, {"#commands", 0, generate_commands}},
{0, "Раса, класс и мировозрение", {"#page", 0, generate_markup}},
{0, "Атрибуты", {"#page", 0, abilities_markup}, 0, {0, 0, 0, 0, 0, character::recreate}},
{0, 0, {"#apply"}, 0, {0, 0, 0, 0, 0, character::apply_avatar}},
{}};
static bool visible_levels(const void* object, int index) {
	auto p = (character*)object;
	auto t = p->getclass();
	return (unsigned)index < bsmeta<class_s>::data[t].classes.count;
}
static bool visible_strenght_percent(const void* object, int index) {
	auto p = (character*)object;
	return !p->is(NoExeptionalStrenght);
}
static markup element_levels[] = {{0, "/", {"levels", 1}, 0, {0, 0, 0, visible_levels}},
{0, "/", {"levels", 2}, 0, {0, 0, 0, visible_levels}},
{}};
static markup element_avatar[] = {{0, 0, {"avatar"}, 0, {0, 0, 0, 0, character::view_avatar}},
{}};
static markup element_other_left[] = {{0, "Класс брони", {"base_ac"}},
{0, "Броски хитов", {"hp_rolled"}},
{}};
static markup element_other_right[] = {{0, "Передвижение", {"movement"}},
{0, "Сила (%)", {"strenght_percent"}, 0, {0, 0, 0, visible_strenght_percent}},
{}};
static markup element_other[] = {{6, 0, {0, 0, element_other_left}},
{6, 0, {0, 0, element_other_right}},
{}};
static markup element_basic[] = {{0, "Имя", {"name"}},
{0, "Раса", {"race"}},
{0, "Пол", {"gender"}},
{0, "Размер", {"size"}},
{0, "Мировозрение", {"alignment"}, 0, {0, 0, allow_alignment}},
{0, "Реакция", {"reaction"}},
{0, "Класс", {"type"}, 0, {0, 0, element_allow_class}},
{0, "Уровни", {"levels", 0, element_levels}},
{}};
static markup element_ability[] = {{0, "Сила", {"abilities", Strenght}},
{0, "Ловкость", {"abilities", Dexterity}},
{0, "Толесложение", {"abilities", Constitution}},
{0, "Интеллект", {"abilities", Intellegence}},
{0, "Мудрость", {"abilities", Wisdow}},
{0, "Харизма", {"abilities", Charisma}},
{}};
static markup element_с1[] = {{0, "Базовые характеристики", {0, 0, element_basic}},
{0, "Другие характеристики", {0, 0, element_other}},
{}};
static markup element_с2[] = {{0, "Боевая картинка", {0, 0, element_avatar}},
{0, "Атрибуты", {0, 0, element_ability}},
{0, "Боевые характеристики", {0, 0, abilities_statistic_group}},
{}};
static markup element_с3[] = {{0, "Навыки", {0, 0, abilities_skills_group}},
{}};
static markup element_general[] = {{5, 0, {0, 0, element_с1}},
{3, 0, {0, 0, element_с2}},
{4, 0, {0, 0, element_с3}},
{}};
static markup element_special_feats[] = {{0, "#checkboxes", {"feats"}}, {}};
static markup element_special_usability[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup element_special_с1[] = {{0, "Способности расы или класса", {0, 0, element_special_feats}},
{}};
static markup element_special_с2[] = {{0, "Доступность предметов", {0, 0, element_special_usability}},
{}};
static markup element_special[] = {{4, 0, {0, 0, element_special_с1}},
{4, 0, {0, 0, element_special_с2}},
{4, 0, {0, 0, element_с3}},
{}};
static markup items_c1[] = {{0, "Голова", {"wears", Head}},
{0, "Шея", {"wears", Neck}},
{0, "Броня", {"wears", Armor}},
{0, "Главное оружие", {"wears", MeleeWeapon}},
{0, "Второстерепное", {"wears", OffhandWeapon}},
{0, "Дистанционное", {"wears", RangedWeapon}},
{0, "Пояс", {"wears", GridleWear}},
{0, "Ноги", {"wears", Legs}},
{}};
static markup items_c2[] = {{0, "Кольцо", {"wears", LeftRing}},
{0, "Кольцо", {"wears", RightRing}},
{}};
static void edit_items(void* object) { decoration::editlist(bsmeta<item_info>::meta); }
static markup items_commands[] = {{0, "Редактировать", {}, 0, {0, 0, 0, 0, 0, edit_items}},
{}};
static markup items_page[] = {{6, 0, {0, 0, items_c1}},
{6, 0, {0, 0, items_c2}},
{0, 0, {"#commands", 0, items_commands}},
{}};
static markup element_form[] = {{0, 0, {"#udate"}, 0, {0, 0, 0, 0, 0, 0, character::changed}},
{0, "Общие", {"#page", 0, element_general}},
{0, "Предметы", {"#page", 0, items_page}},
{0, "Специальные способности", {"#page", 0, element_special}},
{}};
markup character::markups[] = {{0, 0, {"#element", 0, element_form}},
{0, "Генерация персонажа", {"#element", 0, generate_form}},
{}};