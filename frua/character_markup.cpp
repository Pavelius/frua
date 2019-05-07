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
static markup character_race_radio[] = {{0, "#radiobuttons", {"race"}, 0, {allow_race}}, {}};
static markup character_class_radio[] = {{0, "#radiobuttons", {"type"}, 0, {allow_class}}, {}};
static markup character_alignment_radio[] = {{0, "#radiobuttons", {"alignment"}, 0, {allow_alignment}}, {}};
static markup generate_c1[] = {{0, "���", {0, 0, character_gender_radio}},
{0, "����", {0, 0, character_race_radio}},
{}};
static markup generate_c2[] = {{0, "������", {0, 0, character_class_radio}},
{}};
static markup generate_c3[] = {{0, "������������", {0, 0, character_alignment_radio}},
{}};
static markup generate_markup[] = {{2, 0, {0, 0, generate_c1}},
{3, 0, {0, 0, generate_c2}},
{4, 0, {0, 0, generate_c3}},
{}};
static markup generate_commands[] = {{0, "�����������", {}, 0, {}, character::recreate},
{}};
static markup abilities_personal_group[] = {{0, 0, {"name"}},
{0, 0, {"personal"}, 0, {0, 0, character::view_personal}},
{}};
static markup abilities_levels_group[] = {{0, 0, {"levels"}, 0, {0, 0, character::view_levels}}, {}};
static markup abilities_ability_group[] = {{0, 0, {"abilities"}, 0, {0, 0, character::view_ability}}, {}};
static markup abilities_statistic_group[] = {{0, 0, {"statistics"}, 0, {0, 0, character::view_statistic}}, {}};
static markup abilities_skills_group[] = {{0, 0, {"skills"}, 0, {0, 0, character::view_skills}}, {}};
static markup abilities_c1[] = {{0, "���", {0, 0, abilities_personal_group}},
{0, "��������", {0, 0, abilities_ability_group}},
{0, "������ ����������", {0, 0, abilities_statistic_group}},
{}};
static markup abilities_c2[] = {{0, "������ � ����", {0, 0, abilities_levels_group}},
{0, "������", {0, 0, abilities_skills_group}},
{}};
static markup abilities_markup[] = {{3, 0, {0, 0, abilities_c1}},
{4, 0, {0, 0, abilities_c2}},
//{4, 0, {0, 0, generate_c3}},
{}};
static markup generate_form[] = {{0, 0, {"#create"}, 0, {}, character::random},
{0, 0, {"#commands", 0, generate_commands}},
{0, "����, ����� � ������������", {"#page", 0, generate_markup}},
{0, "��������", {"#page", 0, abilities_markup}, 0, {}, character::recreate},
{0, 0, {"#apply"}, 0, {}, character::apply_avatar},
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
static markup element_levels[] = {{0, "/", {"levels", 1}, 0, {0, visible_levels}},
{0, "/", {"levels", 2}, 0, {0, visible_levels}},
{}};
static markup element_avatar[] = {{0, 0, {"avatar"}, 0, {0, 0, character::view_avatar}},
{}};
static markup element_other_left[] = {{0, "����� �����", {"base_ac"}},
{0, "������ �����", {"hp_rolled"}},
{}};
static markup element_other_right[] = {{0, "������������", {"movement"}},
{0, "���� (%)", {"strenght_percent"}, 0, {0, visible_strenght_percent}},
{}};
static markup element_other[] = {{6, 0, {0, 0, element_other_left}},
{6, 0, {0, 0, element_other_right}},
{}};
static markup element_basic[] = {{0, "���", {"name"}},
{0, "����", {"race"}},
{0, "���", {"gender"}},
{0, "������", {"size"}},
{0, "������������", {"alignment"}, 0, {allow_alignment}},
{0, "�������", {"reaction"}},
{0, "�����", {"type"}, 0, {element_allow_class}},
{0, "������", {"levels", 0, element_levels}},
{}};
static markup element_ability[] = {{0, "����", {"abilities", Strenght}},
{0, "��������", {"abilities", Dexterity}},
{0, "������������", {"abilities", Constitution}},
{0, "���������", {"abilities", Intellegence}},
{0, "��������", {"abilities", Wisdow}},
{0, "�������", {"abilities", Charisma}},
{}};
static markup element_�1[] = {{0, "������� ��������������", {0, 0, element_basic}},
{0, "������ ��������������", {0, 0, element_other}},
{}};
static markup element_�2[] = {{0, "������ ��������", {0, 0, element_avatar}},
{0, "��������", {0, 0, element_ability}},
{0, "������ ��������������", {0, 0, abilities_statistic_group}},
{}};
static markup element_�3[] = {{0, "������", {0, 0, abilities_skills_group}},
{}};
static markup element_general[] = {{5, 0, {0, 0, element_�1}},
{3, 0, {0, 0, element_�2}},
{4, 0, {0, 0, element_�3}},
{0, 0, {"#commands", 0, generate_commands}},
{}};
static markup element_special_feats[] = {{0, "#checkboxes", {"feats"}}, {}};
static markup element_special_usability[] = {{0, "#checkboxes", {"usability"}}, {}};
static markup element_special_�1[] = {{0, "����������� ���� ��� ������", {0, 0, element_special_feats}},
{}};
static markup element_special_�2[] = {{0, "����������� ���������", {0, 0, element_special_usability}},
{}};
static markup element_special[] = {{4, 0, {0, 0, element_special_�1}},
{4, 0, {0, 0, element_special_�2}},
{4, 0, {0, 0, element_�3}},
{}};
bool character::isallowwear(const void* object, int param) {
	auto p = (character*)object;
	if(param && !p->wears[param - 1])
		return false;
	return true;
}
static markup items_c1[] = {{0, 0, {"wears", 0}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 1}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 2}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 3}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 4}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 5}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 6}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 7}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 8}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 9}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 10}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 11}, 0, {0, character::isallowwear}},
{}};
static markup items_c2[] = {{0, 0, {"wears", 12}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 13}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 14}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 15}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 16}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 17}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 18}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 19}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 20}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 21}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 22}, 0, {0, character::isallowwear}},
{0, 0, {"wears", 23}, 0, {0, character::isallowwear}},
{}};
static void edit_items(void* object) { decoration::editlist(bsmeta<item_info>::meta); }
static markup items_commands[] = {{0, "�������������", {}, 0, {}, edit_items},
{}};
static markup items_�3[] = {{0, "������", {0, 0, abilities_skills_group}},
{0, "������ ��������������", {0, 0, abilities_statistic_group}},
{}};
static markup items_page[] = {{4, 0, {0, 0, items_c1}},
{4, 0, {0, 0, items_c2}},
{4, 0, {0, 0, items_�3}},
{0, 0, {"#commands", 0, items_commands}},
{}};
static markup element_form[] = {{0, 0, {"#update"}, 0, {}, character::changed},
{0, 0, {"#create"}, 0, {}, character::clear},
{0, "�����", {"#page", 0, element_general}},
{0, "��������", {"#page", 0, items_page}},
{0, "����������� �����������", {"#page", 0, element_special}},
{}};
markup character::markups[] = {{0, 0, {"#element", 0, element_form}},
{0, "��������� ���������", {"#element", 0, generate_form}},
{}};
static markup charsheet_c1[] = {{0, 0, {"personal"}, 0, {0, 0, character::view_personal}},
{0, "��������", {0, 0, abilities_ability_group}},
{0, "����������", {0, 0, abilities_statistic_group}},
{}};
static markup charsheet_c2[] = {{0, "������ � ����", {0, 0, abilities_levels_group}},
{0, "������", {0, 0, abilities_skills_group}},
{}};
markup character::charsheet_markup[] = {{3, 0, {0,0, charsheet_c1}},
{4, 0, {0, 0, charsheet_c2}},
{}};