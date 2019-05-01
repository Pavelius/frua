#include "main.h"

alignment_info bsmeta<alignment_info>::elements[] = {{"Lawful Good", "Законопослушный добрый", {Theif}},
{"Neutral Good", "Нейтрально добрый", {Paladin}},
{"Chaotic Good", "Хаотично добрый", {Paladin}},
{"Lawful Neutral", "Законопослушный нейтральный", {Theif, Ranger, Paladin}},
{"Neutral", "Нейтральный", {Ranger, Paladin}},
{"Chaotic Neutral", "Хаотично нейтральный", {Ranger, Paladin}},
{"Lawful Evil", "Законопослушный злой", {Theif, Ranger, Paladin}},
{"Neutral Evil", "Нейтрально злой", {Ranger, Paladin}},
{"Chaotic Evil", "Хаотично злой", {Ranger, Paladin}},
};
assert_enum(alignment, ChaoticEvil);