#include "main.h"

struct size_info {
	const char*		id;
	const char*		name;
};
static name_info size_data[] = {{"Small", "�����"},
{"Medium", "�������"},
{"Large", "�������"},
};
assert_enum(size, Large);
getstr_enum(size);