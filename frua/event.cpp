#include "main.h"

const bsreq bsmeta<event_info>::meta[] = {
	BSREQ(type),
	BSREQ(image),
	BSREQ(text),
{}};

static void choose_image(picture_info* p) {

}

markup event_info::markups[] = {{0, "���", {"type"}},
{0, "�����������", {"image"}, 0, {}, picture_info::choose, picture_info::getname},
{0, "�����", {"text"}, 5, {}},
{}};