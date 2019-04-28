#include "main.h"

const bsreq bsmeta<special_attack_info>::meta[] = {
	BSREQ(effect),
	BSREQ(damage),
	BSREQ(range),
	BSREQ(modifier),
{}};
static bool visible(const void* source, const markup& e) {
	auto p = (special_attack_info*)source;
	return e.param == p->effect;
}
static bool visible_modifier(const void* source, const markup& e) {
	auto p = (special_attack_info*)source;
	return p->effect!=NoEffect && p->effect != AdditionalDamage;
}
static markup additional_damage[] = {{0, "���", {"damage"}},
{0, "����", {"range"}},
{}};
static markup modifier_damage[] = {{0, "�����������", {"modifier"}},
{}};
markup special_attack_info::markups[] = {{0, "�������������", {"effect"}},
{0, 0, {0, 0, additional_damage}, AdditionalDamage, {0, 0, 0, visible}},
{0, 0, {0, 0, modifier_damage}, 0, {0, 0, 0, visible_modifier}},
{}};