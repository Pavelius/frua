#include "main.h"

const bsreq bsmeta<event_info>::meta[] = {
	BSREQ(type),
	BSREQ(image),
	BSREQ(text),
{}};

static void choose_image(picture_info* p) {

}

markup event_info::markups[] = {{0, "Тип", {"type"}},
{0, "Изображение", {"image"}, 0, {}, picture_info::choose, picture_info::getname},
{0, "Текст", {"text"}, 5, {}},
{}};