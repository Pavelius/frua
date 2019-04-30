#include "main.h"

const bsreq bsmeta<item>::meta[] = {
	BSREQ(type),
	BSREQ(value),
{}};

static bool allow_type(const void* object, int param) {
	auto p = (item*)object;
	if(!p->type)
		return false;
	auto ei = (item_info&)bsmeta<item_info>::data[p->type];
	return bsmeta<wear_s>::data[ei.type].item_type == (wear_type_s)param;
}
static bool allow_quility(const void* object, int param) {
	auto p = (item*)object;
	auto ei = (item_info&)bsmeta<item_info>::data[p->type];
	return bsmeta<wear_s>::data[ei.type].item_type == ChargeableItem;
}
static markup standart_item_c1[] = {{0, "��������", {"value"}},
{}};
static markup standart_item_c2[] = {{0, 0, {"value"}, 0, {0, 0, 0, 0, item::view_state}},{}};
static markup standart_item_c3[] = {{0, 0, {"value"}, 0, {0, 0, 0, 0, item::view_check}}, {}};
static markup standart_item[] = {{5, 0, {0,0,standart_item_c1}},
{3, 0, {0, 0, standart_item_c2}},
{4, 0, {0, 0, standart_item_c3}},
{}};
markup item::markups[] = {{0, "���", {"type"}, 0, {0, 0, 0, 0, item::view_type}},
{0, "�������", {"value", ChargeableItem}, 0, {0, 0, 0, allow_type}},
{0, 0, {0, StandartItem, standart_item}, 0, {0, 0, 0, allow_type}},
{}};