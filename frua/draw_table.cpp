#include "bsreq.h"
#include "crt.h"
#include "draw_control.h"

using namespace draw;
using namespace draw::controls;

const char* table::getheader(char* result, const char* result_max, int column) const {
	return columns[column].title;
}

void table::row(const rect &rc, int index) {
	rowhilite(rc, index);
	rect re = rc; re.x2 = re.x1;
	auto w1 = rc.width() / 12;
	auto object = getrow(index);
	for(auto pc = columns; *pc; pc++) {
		if(pc->proc.isvisible && !pc->proc.isvisible(object, pc->value.index))
			continue;
		re.x2 = re.x1 + w1 * pc->width;
		re.x1 = re.x1 + 1;
		re.x1 = re.x2;
		char temp[260]; temp[0] = 0;
		auto p = getname(temp, temp + sizeof(temp) - 1, index, 0);
		if(p)
			draw::textc(rc.x1 + 4, rc.y1 + 4, rc.width() - 4 * 2, p);
	}
}

const char* reftable::getname(char* result, const char* result_end, int line, int column) const {
	auto& e = columns[column];
	auto object = getrow(line);
	if(e.value.id) {
		auto pf = type->find(e.value.id);
		if(!pf)
			return 0;
		auto pv = pf->ptr(object, e.value.index);
		if(e.prop.getname)
			e.prop.getname(pv, result, result_end);
		else if(pf->isnum())
			szprint(result, result_end, "%1i", pf->get(pv));
		else if(pf->istext()) {
			auto p = (const char*)pf->get(pv);
			if(p)
				szprint(result, result_end, p);
		} else {
			if(pf->is(KindEnum)) {
				auto pb = bsdata::find(type, bsdata::firstenum);
				if(!pb)
					pb = bsdata::find(type, bsdata::first);
				if(!pb)
					return 0;
				auto index = pf->get(pv);
				pv = (char*)pb->get(index);
			} else
				pv = (char*)pf->get(pv);
			if(pv) {
				auto ppf = pf->type->find("name");
				if(!ppf)
					ppf = pf->type->find("id");
				if(ppf) {
					auto p = (const char*)ppf->get(ppf->ptr(pv));
					if(p)
						szprint(result, result_end, p);
				}
			}
		}
		return result;
	}
	return 0;
}