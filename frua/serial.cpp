#include "bsreq.h"
#include "crt.h"
#include "io_plugin.h"

using namespace io;

static bool write_field(io::writer& ew, const void* pv, const bsreq* pf, const char* id, bool run);

static bool write_object(io::writer& ew, const void* pv, const bsreq* pf, const char* id, bool run) {
	if(run) {
		if(!write_object(ew, pv, pf, id, false))
			return false;
		ew.open(id);
		while(*pf) {
			write_field(ew, pv, pf, pf->id, true);
			pf++;
		}
		ew.close(id);
	} else {
		while(*pf) {
			if(write_field(ew, pv, pf, pf->id, false))
				return true;
			pf++;
		}
		return false;
	}
	return true;
}

static bsdata* find_base(const bsreq* type) {
	auto p = bsdata::find(type, bsdata::first);
	if(!p)
		p = bsdata::find(type, bsdata::firstenum);
	return p;
}

static bool write_element(io::writer& ew, const void* pv, const bsreq* pf, int index, const char* id, bool skip_zero, bool run) {
	if(pf->isnum()) {
		auto value = pf->get(pf->ptr(pv, index));
		if(!value && skip_zero)
			return false;
		if(run)
			ew.set(id, value);
	} else if(pf->istext()) {
		auto value = (const char*)pf->get(pf->ptr(pv, index));
		if(!value)
			value = "";
		if(!value[0] && skip_zero)
			return false;
		if(run)
			ew.set(id, value);
	} else if(pf->isref()) {
		if(pf->isnum() || pf->istext() || pf->is(KindEnum))
			return false;
		auto value = pf->get(pf->ptr(pv, index));
		if(!value) {
			if(!skip_zero)
				ew.set(id, value);
			return false;
		}
		auto pb = bsdata::findbyptr((void*)value, bsdata::first);
		if(!pb)
			pb = bsdata::findbyptr((void*)value, bsdata::firstenum);
		if(!pb)
			return false;
		return write_field(ew, (void*)value, pb->meta, id, run);
	} else if(pf->is(KindEnum)) {
		auto value = pf->get(pf->ptr(pv, index));
		auto pb = find_base(pf->type);
		if(!pb)
			return false;
		return write_field(ew, pb->get(value), pf->type, id, run);
	} else if(pf->is(KindCFlags)) {
		unsigned value = pf->get(pf->ptr(pv, index));
		if(!value && skip_zero)
			return false;
		auto pb = find_base(pf->type);
		if(!pb)
			return false;
		auto pk = pb->meta->getkey();
		if(!pk)
			return false;
		if(run) {
			ew.open(id, io::plugin::Array);
			for(unsigned i = 0; i < pb->count; i++) {
				if((value & (1 << i)) == 0)
					continue;
				auto pv1 = pb->get(i);
				auto nm = (const char*)pk->get(pk->ptr(pv1));
				if(!nm)
					continue;
				ew.set("value", nm);
			}
			ew.close(id, io::plugin::Array);
		}
	} else
		return write_object(ew, pf->ptr(pv, index), pf->type, id, run);
	return true;
}

static bool write_field(io::writer& ew, const void* pv, const bsreq* pf, const char* id, bool run) {
	if(pf->count > 1) {
		if(run) {
			auto minimal = -1;
			for(unsigned i = 0; i < pf->count; i++) {
				if(write_element(ew, pv, pf, i, "element", true, false))
					minimal = i;
			}
			if(minimal < 0)
				return false;
			ew.open(id, io::plugin::Array);
			for(auto i = 0; i <= minimal; i++)
				write_element(ew, pv, pf, i, "element", false, true);
			ew.close(id, io::plugin::Array);
		} else {
			for(unsigned i = 0; i < pf->count; i++) {
				if(write_element(ew, pv, pf, i, "element", true, false))
					return true;
			}
			return false;
		}
		return true;
	} else
		return write_element(ew, pv, pf, 0, id, true, run);
}

bool export_base(const char* url) {
	io::plugin* pp = plugin::find(szext(url));
	if(!pp)
		return false;
	io::file file(url, StreamWrite | StreamText);
	if(!file)
		return false;
	io::writer* pw = pp->write(file);
	if(!pw)
		return false;
	pw->open("records", io::plugin::Array);
	for(auto ps = bsdata::first; ps; ps = ps->next) {
		for(unsigned i = 0; i < ps->count; i++) {
			auto p = ps->get(i);
			pw->open("record");
			pw->set("typeid", ps->id);
			for(auto pf = ps->meta; *pf; pf++)
				write_field(*pw, p, pf, pf->id, true);
			pw->close("record");
		}
	}
	pw->close("records", io::plugin::Array);
	return true;
}