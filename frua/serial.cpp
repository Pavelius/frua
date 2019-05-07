#include "bsreq.h"
#include "crt.h"
#include "io_plugin.h"

using namespace io;

static void write_field(io::writer& ew, const void* pv, const bsreq* pf, const char* id);

static bsdata* find_base(const bsreq* type) {
	auto p = bsdata::find(type, bsdata::first);
	if(!p)
		p = bsdata::find(type, bsdata::firstenum);
	return p;
}

static void write_element(io::writer& ew, const void* pv, const bsreq* pf, int index, const char* id, bool skip_zero) {
	if(pf->isnum()) {
		auto value = pf->get(pf->ptr(pv, index));
		if(!value && skip_zero)
			return;
		ew.set(id, value);
	} else if(pf->istext()) {
		auto value = (const char*)pf->get(pf->ptr(pv, index));
		if(!value)
			value = "";
		if(!value[0] && skip_zero)
			return;
		ew.set(id, value);
	} else if(pf->isref()) {
		if(pf->isnum() || pf->istext() || pf->is(KindEnum))
			return;
		auto value = pf->get(pf->ptr(pv, index));
		if(!value) {
			if(!skip_zero)
				ew.set(id, value);
			return;
		}
		auto pb = bsdata::findbyptr((void*)value, bsdata::first);
		if(!pb)
			pb = bsdata::findbyptr((void*)value, bsdata::firstenum);
		if(!pb)
			return;
		write_field(ew, (void*)value, pb->meta, id);
	} else if(pf->is(KindEnum)) {
		auto value = pf->get(pf->ptr(pv, index));
		auto pb = find_base(pf->type);
		if(!pb)
			return;
		write_field(ew, pb->get(value), pf->type, id);
	} else if(pf->is(KindCFlags)) {
		unsigned value = pf->get(pf->ptr(pv, index));
		if(!value && skip_zero)
			return;
		auto pb = find_base(pf->type);
		if(!pb)
			return;
		auto pk = pb->meta->getkey();
		if(!pk)
			return;
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
	} else {
		pv = pf->ptr(pv, index);
		ew.open(id);
		for(auto pf1 = pf->type; *pf1; pf1++)
			write_field(ew, pv, pf1, pf1->id);
		ew.close(id);
	}
}

static void write_field(io::writer& ew, const void* pv, const bsreq* pf, const char* id) {
	if(pf->count > 1) {
		ew.open(id, io::plugin::Array);
		for(unsigned i = 0; i < pf->count; i++)
			write_element(ew, pv, pf, i, "element", false);
		ew.close(id, io::plugin::Array);
	} else
		write_element(ew, pv, pf, 0, id, true);
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
				write_field(*pw, p, pf, pf->id);
			pw->close("record");
		}
	}
	pw->close("records", io::plugin::Array);
	return true;
}