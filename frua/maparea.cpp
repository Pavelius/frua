#include "main.h"

const bsreq bsmeta<maparea>::meta[] = {
	BSREQ(name),
	BSREQ(impassable),
	BSREQ(move_rate),
{}};

static markup form[] = {{0, "��������", {"name"}},
{0, "������������", {"impassable"}},
{0, "��������", {"move_rate"}},
{}};
static markup columns[] = {{-1, "������������", {"name"}},
{32, "��������", {"move_rate"}},
{32, "����������", {"impassable"}},
{}};

void maparea::choose(maparea* source) {
	unsigned count = 16;
	decoration::choose("�������",
		source, sizeof(source[0]), count, 16,
		bsmeta<maparea>::meta, columns, form,
		true);
}