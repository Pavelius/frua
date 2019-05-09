#include "bsreq.h"
#include "crt.h"
#include "io_plugin.h"

using namespace io;

struct bsdata_writer_txt {
	io::writer&			ew;
	bsdata_writer_txt(io::writer& ew) : ew(ew) {}

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

struct bsdata_reader_txt : io::reader {

	enum param_s : unsigned char { Meta, Source, Database };
	
	const bsreq* getmeta(const node& e) const { return e.parent ? (bsreq*)e.parent->params[Meta] : 0; }
	const bsreq* getmeta(const node& e, const char* id) const { return getmeta(e)->find(id); }
	void setmeta(node& e, const bsreq* type) const {
		if(e.parent)
			e.parent->params[Meta] = (int)type;
	}
	void* getsource(const node& e) const { return e.parent ? (void*)e.parent->params[Source] : 0; }
	void setsource(node& e, void* source) const {
		if(e.parent)
			e.parent->params[Source] = (int)source;
	}
	bsdata* getbase(const node& e) const { return e.parent ? (bsdata*)e.parent->params[Database] : 0; }
	void setbase(node& e, void* v) const {
		if(e.parent)
			e.parent->params[Database] = (int)v;
	}

	void open(node& e) override {
		if(e.name[0] == 0)
			return;
		else if(e == "element")
			return;
		if(!e.parent)
			return;
		auto pf = getmeta(e, e.name);
		if(!pf)
			return;
		setmeta(e, pf);
		auto pv = getsource(e);
		if(!pv)
			return;
		setsource(e, pf->ptr(pv, e.index));
	}

	void set(node& e, const char* value) override {
		if(e == "typeid") {
			// Это идентификатор типа
			auto pd = bsdata::find(value, bsdata::first);
			if(!pd)
				pd = bsdata::find(value, bsdata::firstenum);
			setbase(e, pd);
			setmeta(e, pd->meta);
			return;
		}
		auto pf = getmeta(e, e.name);
		if(!pf)
			return;
		void* pv = getsource(e);
		if(!pv) {
			auto pd = getbase(e);
			if(!pd)
				return;
			// Поиск по ключу
			if(pf->istext())
				pv = (void*)pd->find(pf, value);
			else if(pf->isnum()) {
				auto number = sz2num(value);
				auto size = pf->size;
				if(size > sizeof(number))
					size = sizeof(number);
				pv = (void*)pd->find(pf, &number, size);
			} else
				return;
			if(!pv)
				pv = pd->add();
			// Значение ключа не заполняем - оно будет позже
			setsource(e, pv);
		}
		if(!pv)
			return;
		if(pf->istext())
			pf->set(pf->ptr(pv, e.index), (int)szdup(value));
		else if(pf->isnum())
			pf->set(pf->ptr(pv, e.index), sz2num(value));
	};

	void close(node& e) override {}

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