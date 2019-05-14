#include "main.h"

const bsreq bsmeta<maparea>::meta[] = {
	BSREQ(name),
	BSREQ(impassable),
	BSREQ(move_rate),
{}};

static markup form[] = {{0, "Название", {"name"}},
{0, "Проходимость", {"impassable"}},
{0, "Движение", {"move_rate"}},
{}};
static markup columns[] = {{-1, "Наименование", {"name"}},
{32, "Движение", {"move_rate"}},
{32, "Проходимое", {"impassable"}},
{}};

void maparea::choose(maparea* source) {
	unsigned count = 16;
	decoration::choose("Области",
		source, sizeof(source[0]), count, 16,
		bsmeta<maparea>::meta, columns, form,
		true);
}