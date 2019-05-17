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

markup event_info::markups[] = {{0, "Тип", {"type"}},
{0, "Условие", {"condition"}},
{0, "Шанс", {"chance"}, 0, {0, ischance}},
{0, "Предмет", {"macguffin"}, 0, {0, ismacguffin}},
{0, "Изображение", {"image"}, 0, {}, picture_info::choose, picture_info::getname},
{0, "Текст", {"text"}, 5, {}},
{}};