#include "main.h"

const bsreq bsmeta<maparea>::meta[] = {
	BSREQ(name),
	BSREQ(moverate),
	BSREQ(fore),
{}};

static markup form[] = {{0, "Название", {"name"}},
{0, "Движение", {"moverate"}},
{0, "Цвет", {"fore"}, 0, {0, 0, decoration::button_color}},
{}};
static markup columns[] = {{32, "Цвет", {"fore"}, 0, {0, 0, decoration::column_button}},
{200, "Наименование", {"name"}},
{200, "Движение", {"moverate"}},
{}};

void maparea::choose(maparea* source) {
	unsigned count = 10;
	decoration::choose("Области",
		source, sizeof(source[0]), count, 16,
		bsmeta<maparea>::meta, columns, form,
		true);
}