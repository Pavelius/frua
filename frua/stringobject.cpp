#include "main.h"

struct gender_change_string {
	const char*	id;
	const char*	female;
	const char*	male;
	const char*	multiply;
};
static gender_change_string player_gender[] = {{"а", "а", "", "и"},
{"ла", "ла", "", "ли"},
{"ась", "ась", "ся", "ись"},
{"ая", "ая", "ый", "ые"},
{"ей", "ей", "ему", "им"},
{"нее", "нее", "него", "них"},
{"она", "она", "он", "они"},
{"ее", "ее", "его", "их"},
};
static gender_change_string opponent_gender_change[] = {{"А", "а", "", "и"},
{"ЛА", "ла", "", "ли"},
{"ОНА", "она", "он", "они"},
{"ЕЕ", "ее", "его", "их"},
};

void stringobject::addidentifier(const char* identifier) {
	if(name && strcmp(identifier, "герой") == 0)
		add(name);
	else {
		for(auto& e : player_gender) {
			if(strcmp(e.female, identifier) != 0)
				continue;
			if(gender == Female)
				add(e.female);
			else
				add(e.male);
			return;
		}
		stringcreator::addidentifier(identifier);
	}
}