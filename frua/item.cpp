#include "main.h"

const bsreq bsmeta<item>::meta[] = {
	BSREQ(type),
	BSREQ(charges),
{}};

markup item::markups[] = {{0, "���", {"type"}, 0, {0,0,0,0,item::type_view}},
{0, "�������", {"charges"}},
{}};