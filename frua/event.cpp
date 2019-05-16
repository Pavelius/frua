#include "main.h"

const bsreq bsmeta<event_info>::meta[] = {
	BSREQ(type),
	BSREQ(image),
{}};

static void choose_image(picture_info* p) {

}

markup event_info::markups[] = {{0, "Тип", {"type"}},
{0, "Изображение", {"image"}, 0, {}, choose_image, picture_info::getname},
{}};