#include "main.h"

void answer::add(int id, const char* name, int priority, unsigned key) {
	auto p = adat<answer_element, 32>::add();
	p->id = id;
	p->name = name;
	p->key = key;
	p->priority = key;
}