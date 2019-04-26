#include "anyval.h"
#include "bsreq.h"
#include "crt.h"
#include "draw_control.h"
#include "markup.h"
#include "plugin.h"

using namespace draw;

namespace {
struct commandi {
	void*			object;
	bsdata*			data;
	const markup*	element;
	int				range[2];
	rect			rc;
	anyval			value;
	void clear() { memset(this, 0, sizeof(commandi)); }
};
}

static commandi		command;

static const char* getpresent(const void* p) {
	return ((const char**)p)[1];
}

static void choose_enum() {
	struct enum_view : controls::list, adat<int, 256> {
		const bsdata&	source;
		const char*	getname(char* result, const char* result_max, int line, int column) const {
			switch(column) {
			case 0: return getpresent(source.get(data[line]));
			default: return "";
			}
			return "";
		}
		int getmaximum() const override { return count; }
		int getcurrent() const {
			if(!count)
				return 0;
			return data[current];
		}
		constexpr enum_view(const bsdata& source) : source(source) {}
	};
	enum_view ev(*command.data);
	ev.hilite_odd_lines = false;
	auto i1 = 0;
	auto i2 = command.data->count - 1;
	if(command.range[1]) {
		i1 = command.range[0];
		i2 = command.range[1];
	}
	for(unsigned i = i1; i < i2; i++) {
		if(command.element) {
			if(command.element->proc.isallow) {
				if(!command.element->proc.isallow(command.object, i))
					continue;
			}
		}
		ev.add(i);
	}
	auto mc = imin(12, ev.getmaximum());
	auto rc = command.rc;
	auto ci = ev.indexof(command.value);
	if(ci != -1)
		ev.current = ci;
	rc.y1 = rc.y2;
	rc.y2 = rc.y1 + ev.getrowheight() * mc + 1;
	rectf(rc, colors::form);
	if(dropdown(rc, ev, true))
		command.value = ev.getcurrent();
}

static void field_dropdown(const rect& rc, bool focused, const anyval& ev, bsdata& ei, int i1, int i2) {
	auto result = focused && (hot.key == KeyEnter || hot.key == F4 || hot.key == KeyDown);
	if(buttonh(rc, false, focused, false, true, 0, 0, false, 0))
		result = true;
	textc(rc.x1 + 4, rc.y1 + 4, rc.width() - 4 * 2, getpresent(ei.get(ev)));
	if(focused)
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
	if(result) {
		command.clear();
		command.value = ev;
		command.rc = rc;
		command.data = &ei;
		command.range[0] = i1;
		command.range[1] = i2;
		execute(choose_enum);
	}
}

struct markup_view {
	int typedef (markup_view::*callback)(int x, int y, int width, const markup& e, const bsval& source) const;
	int			title_width;
	virtual bsval getvalue(const bsval& source, const markup& e) const {
		bsval result;
		result.type = source.type->find(e.value.id);
		result.data = source.data;
		return result;
	}
	void field_part(rect& rc, const markup& e, const bsval& source, const bsval& bv, void* pv, bool part, int rc_x2) const {
		unsigned flags = AlignLeft;
		if(bv.type->isnum())
			flags = AlignRight;
		draw::focusing((int)pv, flags, rc);
		if(bv.type->istext())
			draw::field(rc, flags, anyval(pv, bv.type->size), -1, FieldText);
		else if(bv.type->isnum()) {
			auto d = e.param;
			if(!d)
				d = 2;
			auto we = draw::textw("0")*(d + 1) + (draw::texth() + 8) + 4 * 2;
			rc.x2 = rc.x1 + we;
			draw::field(rc, flags, anyval(pv, bv.type->size), d, FieldNumber);
			if(part) {
				for(auto p = e.value.child; p; p = p->value.child) {
					auto bv = getvalue(source, *p);
					if(!bv.data || !bv.type)
						continue;
					auto pv = bv.type->ptr(bv.data);
					rc.x1 = rc.x2 + 4;
					if(p->title) {
						auto tw = textw(p->title);
						text(rc.x1, rc.y1 + 4, p->title);
						rc.x1 += tw + 4;
					}
					rc.x2 = rc_x2;
					field_part(rc, *p, source, bv, pv, false, rc_x2);
				}
			}
		} else if(bv.type->is(KindEnum)) {
			auto pd = bsdata::find(bv.type->type, bsdata::firstenum);
			if(pd)
				field_dropdown(rc, isfocused(flags), anyval(pv, bv.type->size), *pd, 0, 0);
		}
	}
	int group_vertial(int x, int y, int width, const markup* elements, const bsval& source) const {
		if(!elements)
			return 0;
		auto y0 = y;
		for(auto p = elements; *p; p++)
			y += element(x, y, width, *p, source);
		return y - y0;
	}
	int group_horizontal(int x, int y, int width, const markup* elements, const bsval& source) const {
		if(!elements)
			return 0;
		auto ym = 0;
		auto wi = width / 12;
		auto wp = 0;
		for(auto p = elements; *p; p++) {
			auto x1 = x + wp*wi;
			auto we = p->width*wi;
			if(x1 + we > width - 12)
				we = width - x1;
			auto yc = element(x, y, we, *p, source);
			if(ym < yc)
				ym = yc;
			x += we;
		}
		return ym;
	}
	int element(int x, int y, int width, const markup& e, const bsval& source) const {
		if(e.proc.isvisible && !e.proc.isvisible(source.data, e))
			return 0;
		if(e.value.id) {
			auto bv = getvalue(source, e);
			if(!bv.data || !bv.type)
				return 0;
			auto pv = bv.type->ptr(bv.data);
			// Вначале найдем целую форму объекта
			if(bv.type->is(KindScalar) && !bv.type->isnum() && !bv.type->istext() && !bv.type->isref()) {
				auto pm = plugin<markup>::get(bv.type->type);
				if(!pm)
					return 0;
				return group_vertial(x, y, width, pm, bsval(pv, bv.type->type));
			}
			draw::state push;
			setposition(x, y, width);
			if(e.title && e.title[0])
				titletext(x, y, width, 0, e.title, title_width);
			rect rc = {x, y, x + width, y + draw::texth() + 8};
			field_part(rc, e, source, bv, pv, true, rc.x2);
			return rc.height() + metrics::padding * 2;
		} else if(e.value.child) {
			if(e.value.child[0].width)
				return group_horizontal(x, y, width, e.value.child, source);
			else
				return group_vertial(x, y, width, e.value.child, source);
		} else
			return 0;
	}
	constexpr markup_view() : title_width(128) {}
};

int draw::field(int x, int y, int width, const markup* elements, const bsval& source, int title_width) {
	markup_view ev;
	ev.title_width = title_width;
	if(elements->width)
		return ev.group_horizontal(x, y, width, elements, source);
	else
		return ev.group_vertial(x, y, width, elements, source);
}