#include "bsreq.h"
#include "crt.h"
#include "io_plugin.h"

using namespace io;

static void write_field(io::writer& ew, const void* pv, const bsreq* pf, const char* id);

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
	} else if(pf->is(KindEnum)) {
		auto value = pf->get(pf->ptr(pv, index));
		auto pb = bsdata::find(pf->type, bsdata::firstenum);
		if(!pb)
			pb = bsdata::find(pf->type, bsdata::first);
		if(!pb)
			return;
		write_field(ew, pb->get(value), pf->type, id);
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