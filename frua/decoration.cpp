#include "main.h"

bool decoration::edit(bsdata& source, void* object, void* copy_object, bool run) {
	auto result = true;
	if(run) {
		char copy_buffer[256] = {};
		auto copy = copy_buffer;
		auto size = source.size;
		if(size > sizeof(copy_buffer))
			copy = new char[size];
		if(object)
			memcpy(copy, object, size);
		else if(copy_object)
			memcpy(copy, copy_object, size);
//		else
//			creating(copy);
		//if(change(copy)) {
		//	if(!object)
		//		object = source.add();
		//	if(object)
		//		memcpy(object, copy, size);
		//} else
		//	result = false;
		if(copy != copy_buffer)
			delete copy;
	}
	return result;
}

decoration decoration::data[] = {{"Персонажи или монстры", {250, 60}, character()},
{"Предметы", {}, item_info()},
};

const decoration* decoration::find(const bsreq* type) {
	for(auto& e : data) {
		if(e.source.meta == type)
			return &e;
	}
	return 0;
}

int decoration::choose(const bsreq* type) {
	auto p = find(type);
	if(!p)
		return -1;
	auto x = p->size.x;
	if(!x)
		x = 120;
	auto y = p->size.y;
	if(!y)
		y = 20 + 4 * 2;
	return choose(p->name, x, y, true, p->source, p->proc);
}

bool decoration::choose(void** result, const bsreq* type) {
	auto index = choose(type);
	if(index != -1) {
		auto p = find(type);
		*result = (void*)p->source.get(index);
		return true;
	}
	return false;
}