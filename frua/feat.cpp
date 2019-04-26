#include "main.h"

feat_info feat_data[] = {{"Darkvision", "Видит в темноте"},
{"BonusSaveVsPoison", "Бонус к защите от яда"},
{"BonusSaveVsWands", "Бонус к защите от палочек"},
{"BonusSaveVsSpells", "Бонус к защите от заклинаний"},
{"BonusHits", "Дополнительные хиты"},
{"DetectSecretDoors", "Находит секретные двери"},
{"DetectUndegroundPassages", "Находит подземные проходы"},
{"CharmResistance", "90% сопротивление очарованию и сну"},
{"ElfWeaponTraining", "+1 к атаке с мечами или луками"},
{"BonusToHitOrcs", "+1 к атаке по гоблиноидам"},
{"SmallSizeCombatAdvantage", "+4 к КБ при защите от гигантов"},
{"LightSteps", "-10% к внимательности оппонента"},
{"HolyGrace", "+10% ко всем навыкам защиты"},
{"NoExeptionalStrenght", "Не использует исключительную силу"},
{"UniqueCharacter", "Уникальный персонаж"},
};
assert_enum(feat, UniqueCharacter);
getstr_enum(feat);