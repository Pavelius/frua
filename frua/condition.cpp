#include "main.h"

condition_info bsmeta<condition_info>::elements[] = {{"AlwaysHappen", "Всегда случается"},
{"PartyHasSpecialItem", "У партии есть особый предмет"},
{"PartyDoesNotHaveSpecialItem", "У партии нет особого предмета"},
{"DayTime", "В дневное время"},
{"NightTime", "В ночное время"},
{"RandomPercentChance", "Случайный шанс возникновения"},
{"SpecifiedRaceInParty", "Указанная раса есть в партии"},
{"SpecifiedClassInParty", "Указанный класс есть в партии"},
};
assert_enum(condition, SpecifiedClassInParty);