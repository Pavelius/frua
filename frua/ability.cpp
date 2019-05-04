#include "main.h"

ability_info bsmeta<ability_info>::elements[] = {{"Strenght", "Сила"},
{"Dexterity", "Ловкость"},
{"Constitution", "Телосложение"},
{"Intellegence", "Интеллект"},
{"Wisdow", "Мудрость"},
{"Charisma", "Харизма"},
//
{"AC", "КБ"},
{"DR", "ПУ"},
{"CriticalDeflection", "Отражение"},
{"CriticalThread", "Угроза"},
{"CriticalMultiplier", "Множитель"},
};
assert_enum(ability, CriticalMultiplier);