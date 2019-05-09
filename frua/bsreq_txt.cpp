#include "bsreq.h"
#include "crt.h"
#include "io_plugin.h"

using namespace io;

struct bsdata_writer_txt {
	writer&	ew;
	bsdata_writer_txt(writer& ew) : ew(ew) {}

	static bsdata* find_base(const bsreq* type) {
		auto p = bsdata::find(type, bsdata::first);
		if(!p)
			p = bsdata::find(type, bsdata::firstenum);
		return p;
	}

	bool write_object(const void* pv, const bsreq* pf, const char* id, bool run) {
		if(run) {
			if(!write_object(pv, pf, id, false))
				return false;
			ew.open(id);
			while(*pf) {
				write_field(pv, pf, pf->id, true);
				pf++;
			}
			ew.close(id);
		} else {
			while(*pf) {
				if(write_field(pv, pf, pf->id, false))
					return true;
				pf++;
			}
			return false;
		}
		return true;
	}

	bool write_element(const void* pv, const bsreq* pf, int index, const char* id, bool skip_zero, bool run) {
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
			return write_field((void*)value, pb->meta, id, run);
		} else if(pf->is(KindEnum)) {
			auto value = pf->get(pf->ptr(pv, index));
			auto pb = find_base(pf->type);
			if(!pb)
				return false;
			return write_field(pb->get(value), pf->type, id, run);
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
				ew.open(id, io::node::Array);
				for(unsigned i = 0; i < pb->count; i++) {
					if((value & (1 << i)) == 0)
						continue;
					auto pv1 = pb->get(i);
					auto nm = (const char*)pk->get(pk->ptr(pv1));
					if(!nm)
						continue;
					ew.set("value", nm);
				}
				ew.close(id, io::node::Array);
			}
		} else
			return write_object(pf->ptr(pv, index), pf->type, id, run);
		return true;
	}

	bool write_field(const void* pv, const bsreq* pf, const char* id, bool run) {
		if(pf->count > 1) {
			if(run) {
				auto minimal = -1;
				for(unsigned i = 0; i < pf->count; i++) {
					if(write_element(pv, pf, i, "element", true, false))
						minimal = i;
				}
				if(minimal < 0)
					return false;
				ew.open(id, node::Array);
				for(auto i = 0; i <= minimal; i++)
					write_element(pv, pf, i, "element", false, true);
				ew.close(id, node::Array);
			} else {
				for(unsigned i = 0; i < pf->count; i++) {
					if(write_element(pv, pf, i, "element", true, false))
						return true;
				}
				return false;
			}
			return true;
		} else
			return write_element(pv, pf, 0, id, true, run);
	}

};

struct bsdata_reader_txt : reader {

	enum param_s : unsigned char { Meta, Source, Database };

	int getvalue(const node& e, param_s v) const {
		auto p = &e;
		while(p->parent) {
			if(p->params[v])
				return p->params[v];
			p = p->parent;
		}
		return 0;
	}

	const bsreq* getmeta(const node& e) const { return (bsreq*)getvalue(e, Meta); }
	void setmeta(node& e, const bsreq* type) const {
		e.params[Meta] = (int)type;
	}
	void* getsource(const node& e) const { return (void*)getvalue(e, Source); }
	void setsource(node& e, void* source) const {
		e.params[Source] = (int)source;
	}
	bsdata* getbase(const node& e) const { return (bsdata*)getvalue(e, Database); }
	void setbase(node& e, void* v) const {
		e.params[Database] = (int)v;
	}

	void* findvalue(const char* value, bsdata* pd) const {
		void* pv = 0;
		auto pk = pd->meta;
		if(pk->istext())
			pv = (void*)pd->find(pk, value);
		else if(pk->isnum()) {
			auto number = sz2num(value);
			auto size = pk->size;
			if(size > sizeof(number))
				size = sizeof(number);
			pv = (void*)pd->find(pk, &number, size);
		} else
			return 0;
		if(!pv) {
			pv = pd->add();
			auto kv = getvalue(value, pk);
			pk->set(pk->ptr(pv), kv);
		}
		return pv;
	}

	int getvalue(const char* value, const bsreq* pf) const {
		if(pf->istext())
			return (int)szdup(value);
		else if(pf->isnum())
			return sz2num(value);
		else if(pf->isref() || pf->is(KindEnum)) {
			auto pd = bsdata::find(pf->type, bsdata::firstenum);
			if(!pd)
				pd = bsdata::find(pf->type, bsdata::first);
			if(!pd)
				return 0;
			auto pv = findvalue(value, pd);
			if(pf->isref())
				return (int)pv;
			return pd->indexof(pv);
		}
		return 0;
	}

	void open(node& e) override {
		if(e.name[0] == 0)
			return;
		if(e == "element" || !e.parent || e.type==e.Array)
			return;
		auto pf = getmeta(e)->find(e.name);
		if(!pf)
			return;
		if(!pf->isref() && pf->is(KindScalar) && !pf->isnum() && !pf->istext()) {
			auto pv = getsource(e);
			if(!pv)
				return;
			setmeta(e, pf->type);
			setsource(e, pf->ptr(pv));
		}
	}

	void set(node& e, const char* value) override {
		if(e == "typeid") {
			if(!e.parent)
				return;
			// Это идентификатор типа
			auto pd = bsdata::find(value, bsdata::first);
			if(!pd)
				pd = bsdata::find(value, bsdata::firstenum);
			setbase(*e.parent, pd);
			setmeta(*e.parent, pd->meta);
			return;
		}
		const bsreq* pf = 0;
		if(e == "element") {
			if(e.parent)
				pf = getmeta(e)->find(e.parent->name);
		} else
			pf = getmeta(e)->find(e.name);
		if(!pf)
			return;
		void* pv = getsource(e);
		if(!pv) {
			auto pd = getbase(e);
			if(!pd)
				return;
			// Допускаются только простые ключи
			pv = findvalue(value, pd);
			if(e.parent)
				setsource(*e.parent, pv);
		} else {
			auto v = getvalue(value, pf);
			auto i = 0;
			if(e.parent && e.parent->type == e.Array)
				i = e.index;
			pf->set(pf->ptr(pv, i), v);
		}
	};

	void close(node& e) override {
	}

};

int bsdata::writetxt(const char* url) {
	io::plugin* pp = plugin::find(szext(url));
	if(!pp)
		return 0;
	io::file file(url, StreamWrite | StreamText);
	if(!file)
		return 0;
	io::writer* pw = pp->write(file);
	if(!pw)
		return 0;
	auto object_count = 0;
	bsdata_writer_txt bw(*pw);
	pw->open("records", node::Array);
	for(auto ps = bsdata::first; ps; ps = ps->next) {
		for(unsigned i = 0; i < ps->count; i++) {
			auto p = ps->get(i);
			pw->open("record");
			pw->set("typeid", ps->id);
			for(auto pf = ps->meta; *pf; pf++)
				bw.write_field(p, pf, pf->id, true);
			pw->close("record");
			object_count++;
		}
	}
	pw->close("records", node::Array);
	return object_count;
}

int bsdata::readtxt(const char* url) {
	io::plugin* pp = plugin::find(szext(url));
	if(!pp)
		return 0;
	auto p = loadt(url);
	bsdata_reader_txt ev;
	pp->read(p, ev);
	delete p;
	return 1;
}