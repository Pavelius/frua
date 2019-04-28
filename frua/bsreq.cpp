#include "bsreq.h"

extern "C" int strcmp(const char* s1, const char* s2);
extern "C" int memcmp(const void* s1, const void* s2, unsigned size);

bsdata*	bsdata::first;
bsdata*	bsdata::firstenum;
const bsreq bsmeta<int>::meta[] = {{"number"}, {}};
const bsreq bsmeta<const char*>::meta[] = {{"text"}, {}};
const bsreq bsmeta<bsreq>::meta[] = {
	BSREQ(id),
	BSREQ(offset),
	BSREQ(size),
	BSREQ(lenght),
	BSREQ(count),
	BSREQ(reference),
	BSREQ(type),
{}};

bool bsreq::istext() const {
	return type == bsmeta<const char*>::meta;
}

bool bsreq::isnum() const {
	return type == bsmeta<int>::meta;
}

const bsreq* bsreq::getkey() const {
	auto f = find("id", bsmeta<const char*>::meta);
	if(!f)
		f = find("name", bsmeta<const char*>::meta);
	if(!f)
		f = find("text", bsmeta<const char*>::meta);
	return f;
}

const bsreq* bsreq::find(const char* name) const {
	if(!this)
		return 0;
	for(auto p = this; p->id; p++) {
		if(strcmp(p->id, name) == 0)
			return p;
	}
	return 0;
}

const bsreq* bsreq::find(const char* name, const bsreq* type) const {
	if(!this)
		return 0;
	for(auto p = this; p->id; p++) {
		if(p->type != type)
			continue;
		if(strcmp(p->id, name) == 0)
			return p;
	}
	return 0;
}

const bsreq* bsreq::find(const char* name, unsigned count) const {
	if(!this)
		return 0;
	for(auto p = this; p->id; p++) {
		if(p->type != type)
			continue;
		auto found = true;
		for(unsigned i = 0; i < count; i++) {
			if(p->id[i] != name[i]) {
				found = false;
				break;
			}
		}
		if(found)
			return p;
	}
	return 0;
}

int bsreq::get(const void* p) const {
	switch(size) {
	case sizeof(char) : return *((char*)p);
	case sizeof(short) : return *((short*)p);
	default: return *((int*)p);
	}
}

void bsreq::set(const void* p, int value) const {
	switch(size) {
	case sizeof(char) : *((char*)p) = value; break;
	case sizeof(short) : *((short*)p) = value; break;
	default: *((int*)p) = value; break;
	}
}

bool bsreq::match(const void* p, const char* name) const {
	auto value = (const char*)get(p);
	if(!value || type != bsmeta<const char*>::meta)
		return false;
	for(int i = 0; name[i]; i++) {
		if(value[i] != name[i])
			return false;
	}
	return true;
}

bsdata::bsdata(const char* id, const bsreq* meta,
	void* data, unsigned size, unsigned count, unsigned maximum,
	bstype_s subtype) :
	id(id), meta(meta), next(0),
	data(data), count(count), maximum(maximum), size(size),
	subtype(subtype) {
	auto pf = &first;
	if(subtype == KindEnum)
		pf = &firstenum;
	while(*pf)
		pf = &((*pf)->next);
	*pf = this;
}

void* bsdata::add() {
	if(count < maximum)
		return (char*)data + (count++)*size;
	return data;
}

bsdata* bsdata::find(const char* v, bsdata* first) {
	if(!v || !v[0])
		return 0;
	for(auto p = first; p; p = p->next) {
		if(strcmp(p->id, v) == 0)
			return p;
	}
	return 0;
}

bsdata* bsdata::find(const bsreq* v, bsdata* first) {
	if(!v)
		return 0;
	for(auto p = first; p; p = p->next) {
		if(p->meta == v)
			return p;
	}
	return 0;
}

bsdata* bsdata::findbyptr(const void* object, bsdata* first) {
	if(!object)
		return 0;
	for(auto p = first; p; p = p->next)
		if(p->has(object))
			return p;
	return 0;
}

int	bsdata::indexof(const void* object) const {
	if(!has(object))
		return -1;
	return ((char*)object - (char*)data) / size;
}

const void* bsdata::find(const bsreq* id, const char* value) const {
	if(!id || id->type != bsmeta<const char*>::meta)
		return 0;
	if(!value)
		return find(id, &value, sizeof(value));
	auto ps = (char*)id->ptr(data);
	auto pe = ps + size*count;
	for(; ps < pe; ps += size) {
		auto ps_value = (const char*)id->get(ps);
		if(!ps_value)
			continue;
		if(strcmp(ps_value, value) == 0) {
			auto i = indexof(ps);
			if(i == -1)
				return 0;
			return get(i);
		}
	}
	return 0;
}

const void* bsdata::find(const bsreq* id, const void* value, unsigned size) const {
	if(!id)
		return 0;
	auto ps = (char*)id->ptr(data);
	auto pe = ps + size * count;
	for(; ps < pe; ps += size) {
		if(memcmp(ps, value, size) == 0) {
			auto i = indexof(ps);
			if(i == -1)
				return 0;
			return get(i);
		}
	}
	return 0;
}