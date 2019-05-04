#include "main.h"

decoration decoration::data[] = {{"Персонажи или монстры", {250, 60}, character()},
{"Описания предетов", {200, 18 + 4 * 2}, item_info(), "-"},
{"Предметы", item()},
{"Кубик", dice_info()},
{"Специальная атака", damage_info()},
};

// Export procedure to find default markup
const markup* getmarkup(const bsreq* type) {
	auto p = decoration::find(type);
	if(!p)
		return 0;
	return p->markups;
}

bool decoration::edit(bsdata& source, void* object, void* copy_object, const char* form_name) {
	auto result = true;
	auto pd = find(source.meta);
	if(!pd)
		return false;
	char copy_buffer[256] = {};
	auto copy = copy_buffer;
	auto size = source.size;
	auto creating = false;
	if(size > sizeof(copy_buffer))
		copy = new char[size];
	if(object)
		memcpy(copy, object, size);
	else if(copy_object)
		memcpy(copy, copy_object, size);
	else {
		memset(copy, 0, size);
		creating = true;
	}
	if(edit(pd->name, copy, source.size, source.meta, 0, creating)) {
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

int decoration::choose(const bsreq* type, bool choose_mode) {
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
	return p->choose(p->name, x, y, choose_mode);
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

void decoration::initialize() {
	for(auto& e : data) {
		if(!e.database)
			continue;
		if(e.zero_element && e.database->count==0) {
			auto p = e.database->add();
			auto pf = e.meta->find("name");
			if(pf)
				pf->set(pf->ptr(p), (int)e.zero_element);
		}
	}
}