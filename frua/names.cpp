#include "main.h"

struct name_i
{
	race_s		race;
	gender_s	gender;
	class_s		cls;
	const char*	name[2];
};
static name_i name_data[] = {
	{Human, NoGender, NoClass, {"",""}},
	{Human, Male, Fighter, {"Rudiger","Рудигер"}},
	{Human, Male, Fighter, {"Gregor","Грегор"}},
	{Human, Female, Fighter, {"Brianne","Бриан"}},
	{Human, Male, Fighter, {"Walton","Вальтон"}},
	{Human, Male, Fighter, {"Castor","Кастор"}},
	{Human, Female, Fighter, {"Shanna","Шанна"}},
	{Human, Male, Fighter, {"Ajax","Айакс"}},
	{Human, NoGender, Fighter, {"Hob","Хоб"}},
	{Halfling, NoGender, Fighter, {"Finnegan","Финганн"}},
	{Halfling, Female, Fighter, {"Olive","Оливия"}},
	{Halfling, Male, Fighter, {"Randolph","Рэндольф"}},
	{Halfling, NoGender, Fighter, {"Bartleby","Батлбай"}},
	{Halfling, Male, Fighter, {"Aubrey","Аурбей"}},
	{Halfling, Male, Fighter, {"Baldwin","Балдвин"}},
	{Halfling, Female, Fighter, {"Becca","Бэкки"}},
	{Elf, NoGender, Fighter, {"Elohiir","Эйлохир"}},
	{Elf, Female, Fighter, {"Sharaseth","Харасез"}},
	{Elf, Male, Fighter, {"Hasrith","Хазрич"}},
	{Elf, Male, Fighter, {"Shevara","Шеварал"}},
	{Elf, Male, Fighter, {"Cadeus","Кадиус"}},
	{Elf, Male, Fighter, {"Eldar","Эльдар"}},
	{Elf, Female, Fighter, {"Kithracet","Котораса"}},
	{Elf, Female, Fighter, {"Thelian","Фелианна"}},
	{Dwarf, Male, Fighter, {"Ozruk","Озрук"}},
	{Dwarf, Male, Fighter, {"Surtur","Суртур"}},
	{Dwarf, Female, Fighter, {"Brunhilda","Брундилла"}},
	{Dwarf, Female, Fighter, {"Annika","Анника"}},
	{Dwarf, Male, Fighter, {"Janos","Джанос"}},
	{Dwarf, Female, Fighter, {"Greta","Гретта"}},
	{Dwarf, Male, Fighter, {"Dim","Дим"}},
	{Dwarf, Male, Fighter, {"Rundrig","Рундриг"}},
	{Dwarf, Male, Fighter, {"Jar","Жарл"}},
	{Dwarf, Male, Fighter, {"Xotoq","Ксоток"}},
	//
	{Dwarf, Female, Cleric, {"Durga", "Дурга"}},
	{Dwarf, Male, Cleric, {"Aelfar", "Ольфар"}},
	{Dwarf, Female, Cleric, {"Gerda", "Герда"}},
	{Dwarf, Male, Cleric, {"Rurgosh", "Ругрош"}},
	{Dwarf, Male, Cleric, {"Bjorn", "Бьерн"}},
	{Dwarf, Male, Cleric, {"Drummond", "Друмонд"}},
	{Dwarf, Female, Cleric, {"Helga", "Хельга"}},
	{Dwarf, Female, Cleric, {"Siggrun", "Сигрин"}},
	{Dwarf, Female, Cleric, {"Freya", "Фрея"}},
	{Human, Male, Cleric, {"Wesley", "Вислей"}},
	{Human, Male, Cleric, {"Brinton", "Бринтон"}},
	{Human, Male, Cleric, {"Jon", "Йон"}},
	{Human, Female, Cleric, {"Sara", "Сара"}},
	{Human, Male, Cleric, {"Hawthorn", "Хавторн"}},
	{Human, Female, Cleric, {"Elise", "Элис"}},
	{Human, Male, Cleric, {"Clarke", "Кларк"}},
	{Human, Male, Cleric, {"Lenore", "Линор"}},
	{Human, Male, Cleric, {"Piotr", "Петр"}},
	{Human, Male, Cleric, {"Dahlia", "Данила"}},
	{Human, Female, Cleric, {"Carmine", "Кармин"}},
};

static int selectnames(short unsigned* elements, class_s type, race_s* race, gender_s gender)
{
	auto p = elements;
	for(int i = 0; i < sizeof(name_data) / sizeof(name_data[0]); i++)
	{
		if(gender != NoGender && name_data[i].gender != gender)
			continue;
		if(type != NoClass && name_data[i].cls != type)
			continue;
		if(race && name_data[i].race != *race)
			continue;
		*p++ = i;
	}
	return p - elements;
}

short unsigned character::getrandomname(race_s race, gender_s gender, class_s type)
{
	short unsigned elements[sizeof(name_data) / sizeof(name_data[0])];
	int count = selectnames(elements, type, &race, gender);
	if(!count)
		count = selectnames(elements, NoClass, &race, gender);
	if(!count)
		count = selectnames(elements, NoClass, 0, gender);
	if(!count)
		return 0;
	return elements[rand() % count];
}

const char* character::getname() const {
	if(!name)
		return getstr(monster);
	return name_data[name].name[1];
}