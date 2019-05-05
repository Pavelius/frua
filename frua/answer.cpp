#include "main.h"

void answer::ask(int id, const char* name, int priority, unsigned key) {
	auto p = elements.add();
	p->id = id;
	p->name = name;
	p->key = key;
	p->priority = key;
}