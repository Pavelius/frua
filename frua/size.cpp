#include "main.h"

static size_info size_data[] = {{"Small", "Малый"},
{"Medium", "Средний"},
{"Large", "Большой"},
};
assert_enum(size, Large);
getstr_enum(size);