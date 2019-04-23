#include "main.h"

struct size_info {
	const char*		id;
	const char*		name;
};
static name_info size_data[] = {{"Small", "Малый"},
{"Medium", "Средний"},
{"Large", "Большой"},
};
assert_enum(size, Large);
getstr_enum(size);