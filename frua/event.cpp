#include "main.h"

const bsreq bsmeta<event_info>::meta[] = {
	BSREQ(type),
	BSREQ(image),
	BSREQ(text),
	BSREQ(condition),
	BSREQ(chance),
	BSREQ(macguffin),
{}};

static bool ischance(const void* object, int index) {
	auto p = (event_info*)object;
	return p->condition==RandomPercentChance;
}

static bool ismacguffin(const void* object, int index) {
	auto p = (event_info*)object;
	return p->condition == PartyHasSpecialItem
		|| p->condition == PartyDoesNotHaveSpecialItem;
}

markup event_info::markups[] = {{0, "���", {"type"}},
{0, "�������", {"condition"}},
{0, "����", {"chance"}, 0, {0, ischance}},
{0, "�������", {"macguffin"}, 0, {0, ismacguffin}},
{0, "�����������", {"image"}, 0, {}, picture_info::choose, picture_info::getname},
{0, "�����", {"text"}, 5, {}},
{}};