#include "main.h"

struct gender_change_string {
	const char*	id;
	const char*	female;
	const char*	male;
	const char*	multiply;
};
static gender_change_string player_gender[] = {{"�", "�", "", "�"},
{"��", "��", "", "��"},
{"���", "���", "��", "���"},
{"��", "��", "��", "��"},
{"��", "��", "���", "��"},
{"���", "���", "����", "���"},
{"���", "���", "��", "���"},
{"��", "��", "���", "��"},
};
static gender_change_string opponent_gender_change[] = {{"�", "�", "", "�"},
{"��", "��", "", "��"},
{"���", "���", "��", "���"},
{"��", "��", "���", "��"},
};

void stringobject::addidentifier(const char* identifier) {
	if(name && strcmp(identifier, "�����") == 0)
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