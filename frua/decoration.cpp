#include "main.h"

decoration decoration::data[] = {{"Персонажи или монстры", {250, 60}, character()},
{"Предметы", {200, 18 + 4 * 2}, item_info()},
{"Броня", armor_info()},
{"Оружие", damage_info()},
{"Кубик", dice_info()},
};

// Export procedure to find default markup
const markup* getmarkup(const bsreq* type) {
	auto p = decoration::find(type);
	if(!p)
		return 0;
	return p->form_element;
}

bool decoration::edit(bsdata& source, void* object, void* copy_object) {
	auto result = true;
	auto pd = find(source.meta);
	if(!pd)
		return false;
	char copy_buffer[256] = {};
	auto copy = copy_buffer;
	auto size = source.size;
	if(size > sizeof(copy_buffer))
		copy = new char[size];
	if(object)
		memcpy(copy, object, size);
	else if(copy_object)
		memcpy(copy, copy_object, size);
	else
		pd->created(copy);
	if(edit(pd->name, copy, source.size, source.meta, pd->form_element, pd->changed)) {
		if(!object)
			object = source.add();
		if(object)
			memcpy(object, copy, size);
	} else
		result = false;
	if(copy != copy_buffer)
		delete copy;
	return result;
}

const decoration* decoration::find(const bsreq* type) {
	for(auto& e : data) {
		if(e.meta == type)
			return &e;
	}
	return 0;
}

int decoration::choose(const bsreq* type) {
	auto p = find(type);
	if(!p)
		return -1;
	if(!p->database)
		return -1;
	auto x = p->size.x;
	if(!x)
		x = 120;
	auto y = p->size.y;
	if(!y)
		y = 18 + 4 * 2;
	return p->choose(p->name, x, y, false);
}

bool decoration::choose(void** result, const bsreq* type) {
	auto index = choose(type);
	if(index != -1) {
		auto p = find(type);
		if(p->database)
			*result = (void*)p->database->get(index);
		return true;
	}
	return false;
}

const markup* decoration::findmarkup(const char* id) const {
	if(!form_element)
		return 0;
	for(auto p = form_element; *p; p++) {
		if(!p->value.id || p->value.id[0] != '#')
			return 0;
		if(strcmp(p->value.id + 1, id) == 0)
			return p;
	}
	return 0;
}

int decoration::getpagecount(const markup* p, const void* object) {
	if(!p)
		return 0;
	auto result = 0;
	while(*p && p->value.id[0] == '#') {
		if(!p->proc.isvisible(object, *p))
			continue;
		result++;
	}
	return result;
}

const markup* decoration::getpage(const markup* p, const void* object, int result) {
	if(!p)
		return 0;
	while(*p && p->value.id[0] == '#') {
		if(!p->proc.isvisible(object, *p))
			continue;
		if(--result <= 0)
			return p;
	}
	return 0;
}

const markup* decoration::findcommands(const markup* form) {
	if(!form
		|| !form->value.id
		|| form->value.id[0]!='#')
		return 0;
	for(auto p = form; *p; p++) {
		if(!p->value.id || p->value.id[0] != '#')
			return 0;
		if(!p->value.child)
			continue;
		if(strcmp(p->value.id + 1, "commands") == 0)
			return p->value.child;
	}
	return 0;
}