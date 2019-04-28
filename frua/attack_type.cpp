#include "main.h"

attack_type_info attack_type_data[] = {{"NoSave", "Без шанса на промах"},
{"SaveNegates", "Использовать бросок спасения"},
{"SaveHalf", "Бросок спасения уменьшает урон вполовину"},
{"StandartAttack", "Требует броска атаки"},
};
assert_enum(attack_type, NoSave);
getstr_enum(attack_type);