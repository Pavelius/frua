#include "main.h"

const bsreq bsmeta<maparea>::meta[] = {
	BSREQ(name),
	BSREQ(moverate),
	BSREQ(fore),
{}};

static markup form[] = {{0, "��������", {"name"}},
{0, "��������", {"moverate"}},
{0, "����", {"fore"}, 0, {0, 0, decoration::button_color}},
{}};
static markup columns[] = {{32, "����", {"fore"}, 0, {0, 0, decoration::column_button}},
{200, "������������", {"name"}},
{200, "��������", {"moverate"}},
{}};

void maparea::choose(maparea* source) {
	unsigned count = 10;
	decoration::choose("�������",
		source, sizeof(source[0]), count, 16,
		bsmeta<maparea>::meta, columns, form,
		true);
}