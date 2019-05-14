#include "bsreq.h"
#include "crt.h"
#include "draw_control.h"

using namespace draw;
using namespace draw::controls;

const char* table::getheader(char* result, const char* result_max, int column) const {
	return columns[column].title;
}

void table::view(const rect& rc) {
	minimum_width = 0;
	resize_column_count = 0;
	column_count = 0;
	for(unsigned i = 0; columns[i]; i++) {
		auto& e = columns[i];
		if(e.width == -1)
			resize_column_count++;
		else
			minimum_width += e.width;
		column_count++;
	}
	list::view(rc);
}

const char* table::getname(char* result, const char* result_end, int line, int column) const {
	auto& e = columns[column];
	auto object = getrow(line);
	if(!e.value.id)
		return 0;
	auto pf = type->find(e.value.id);
	if(!pf)
		return 0;
	auto pv = pf->ptr(object, e.value.index);
	if(e.prop.getname)
		e.prop.getname(pv, result, result_end);
	else if(pf->is(KindNumber))
		szprint(result, result_end, "%1i", pf->get(pv));
	else if(pf->is(KindText)) {
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

void table::row(const rect &rc, int index) {
	rowhilite(rc, index);
	rect re = rc; re.x2 = re.x1;
	auto object = getrow(index);
	auto w = (resize_column_count > 0) ? (rc.width() - minimum_width) / resize_column_count : 0;
	for(unsigned i = 0; columns[i]; i++) {
		auto& e = columns[i];
		if(e.width == -1)
			re.x2 = re.x1 + w;
		else
			re.x2 = re.x1 + e.width;
		re.x1 = re.x1 + 1;
		char temp[260]; temp[0] = 0;
		auto p = getname(temp, temp + sizeof(temp) - 1, index, i);
		if(p && p[0]) {
			auto w = re.width();
			auto align = getalign(i);
			if(align==AlignRight)
				draw::text(re.x1 + w - 4 - textw(p), re.y1 + 4, p);
			else
				draw::textc(re.x1 + 4, re.y1 + 4, w - 4 * 2, p);
		}
		re.x1 = re.x2;
	}
}

image_flag_s table::getalign(int column) const {
	auto& e = columns[column];
	if(!e.value.id)
		return AlignLeft;
	auto pf = type->find(e.value.id);
	if(!pf)
		return AlignLeft;
	if(pf->is(KindNumber))
		return AlignRight;
	return AlignLeft;
}

void* table::getcurrent() const {
	if(getmaximum() <= 0)
		return 0;
	return getrow(current);
}