#include "main.h"

feat_info bsmeta<feat_info>::elements[] = {{"Darkvision", "Видит в темноте"},
{"TurnUndead", "Изгоняет нежить"},
{"HuntEnemy", "+1/уровень к урону врагам"},
{"BonusSaveVsPoison", "Бонус к спасению от яда"},
{"BonusSaveVsWands", "Бонус к спасению от палочек"},
{"BonusSaveVsSpells", "Бонус к спасению от заклинаний"},
{"BonusHits", "Дополнительные хиты"},
{"DetectSecretDoors", "Находит секретные двери"},
{"DetectUndegroundPassages", "Находит подземные проходы"},
{"CharmResistance90", "90% сопротивление сну и шарму"},
{"CharmResistance30", "30% сопротивление сну и шарму"},
{"ElfWeaponTraining", "+1 к атаке оружием эльфов"},
{"BonusToHitOrcs", "+1 к атаке по гоблиноидам"},
{"SmallSizeCombatAdvantage", "+4 к КБ при защите от гигантов"},
{"LightSteps", "-10% к внимательности оппонента"},
{"HolyGrace", "+10% ко всем навыкам спасения"},
{"NoExeptionalStrenght", "Не использует исключительную силу"},
{"LightSensivity", "-1 к атаке при ярком свете"},
{"UniqueCharacter", "Уникальный персонаж"},
};
assert_enum(feat, UniqueCharacter);