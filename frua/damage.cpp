#include "main.h"

const bsreq bsmeta<damage_info>::meta[] = {
	BSREQ(type),
	BSREQ(thac0),
	BSREQ(critical),
	BSREQ(multiplier),
	BSREQ(attacks),
	BSREQ(range),
	BSREQ(damage),
	BSREQ(damage_large),
{}};
static markup multiplier_right[] = {{0, "X", {"multiplier"}}, {}};
static markup attack_right[] = {{0, "/", {"thac0"}}, {}};
markup damage_info::markups[] = {{0, "���", {"attack_type"}},
{0, "�-�� ����", {"attacks", 0, attack_right}},
{0, "���", {"type"}},
{0, "���������", {"range"}},
{0, "�����������", {"critical", 0, multiplier_right}},
{0, "����", {"damage"}},
{0, "���� (�������)", {"damage_large"}},
{}};