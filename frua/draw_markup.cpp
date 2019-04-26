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
	rect			rc;
	anyval			value;
	void clear() { memset(this, 0, sizeof(commandi)); }
};
static commandi	command;
struct cmd_check : runable {
	static void check_flags() {
		unsigned v1 = command.value;
		unsigned v2 = 1 << hot.param;
		if(v1&v2)
			v1 &= ~v2;
		else
			v1 |= v2;
		command.value = (const int)v1;
	}
	constexpr cmd_check(const anyval& value, const void* source, unsigned index)
		: source(source), index(index), value(value) {}
	virtual int	getid() const { return (int)source; }
	virtual void execute() const {
		command.clear();
		command.value = value;
		draw::execute(check_flags, index);
	}
	virtual bool isdisabled() const { return false; }
	bool ischecked() const { return (((int)value) & (1 << index)) != 0; }
private:
	const void*		source;
	unsigned		index;
	const anyval	value;
};
}

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

static void field_dropdown(const rect& rc, bool focused, const anyval& ev, bsdata& ei) {
	auto result = focused && (hot.key == KeyEnter || hot.key == F2);
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
		execute(choose_enum);
	}
}

struct markup_view {
	enum title_s : unsigned char {NoTitle, TitleNormal, TitleShort};
	int typedef (markup_view::*callback)(int x, int y, int width, const markup& e, const bsval& source) const;
	typedef decltype(markup::proc.isallow) callback_allow;
	int			title_width;
	static void render_title(int& x, int y, int& width, const char* title, title_s state) {

	}
	bsval getvalue(const bsval& source, const markup& e) const {
		bsval result;
		result.type = source.type->find(e.value.id);
		result.data = source.data;
		return result;
	}
	static rect start_group(int& x, int& y, int& width, const char* title) {
		setposition(x, y, width);
		setposition(x, y, width);
		rect rc = {x - metrics::padding, y, x + width + metrics::padding, y + texth() + 4 * 2};
		gradv(rc, colors::border, colors::edit);
		text(rc, title, AlignCenterCenter);
		y = rc.y2 + metrics::padding * 2;
		return rc;
	}
	static int close_group(int x, int y, const rect& rc) {
		if(!rc)
			return 0;
		rectb({rc.x1, rc.y1, rc.x2, y + metrics::padding}, colors::border);
		return metrics::padding * 2;
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
			if(part && e.value.child) {
				for(auto p = e.value.child; *p; p++) {
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
				field_dropdown(rc, isfocused(flags), anyval(pv, bv.type->size), *pd);
		}
	}
	int group_vertial(int x, int y, int width, const markup* elements, const bsval& source, const char*& title_override) const {
		if(!elements)
			return 0;
		auto y0 = y;
		for(auto p = elements; *p; p++)
			y += element(x, y, width, *p, source, p->title, TitleNormal, title_override);
		return y - y0;
	}
	int group_horizontal(int x, int y, int width, const markup* elements, const bsval& source, const char*& title_override) const {
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
			auto yc = element(x, y, we, *p, source, p->title, TitleNormal, title_override);
			if(ym < yc)
				ym = yc;
			x += we;
		}
		return ym;
	}
	void header(int& x, int y, int& width, const char* label, int title_width, title_s state, const char*& title_override) const {
		if(title_override && title_override[0]) {
			label = title_override;
			title_override = 0;
		}
		if(!label || !label[0])
			return;
		if(state == NoTitle)
			return;
		if(state == TitleShort) {
			auto w = textw(label);
			x += 4;
			width -= 4;
			titletext(x, y, width, 0, label, w, 0);
		} else
			titletext(x, y, width, 0, label, title_width);
	}
	int error(int x, int y, int width, const markup& e, const char* error_text) const {
		auto old_color = fore;
		fore = colors::red;
		setposition(x, y, width);
		titletext(x, y, width, 0, e.title ? e.title : e.value.id, title_width, ":");
		auto h = texth() + 4 * 2;
		rect rc = {x, y, x + width, y + h};
		rectb(rc, colors::red);
		text(rc, error_text, AlignCenterCenter);
		fore = old_color;
		return rc.height() + metrics::padding * 2;
	}
	int element(int x, int y, int width, const markup& e, const bsval& source, const char* title_text, title_s title_state, const char*& title_override) const {
		if(e.proc.isvisible && !e.proc.isvisible(source.data, e))
			return 0;
		if(e.title && e.title[0] == '#') {
			auto pn = e.title + 1;
			auto y0 = y;
			if(e.value.id) {
				auto bv = getvalue(source, e);
				if(!bv.data || !bv.type)
					return 0;
				if(strcmp(pn, "checkboxes") == 0 && (bv.type->is(KindEnum) || bv.type->is(KindCFlags))) {
					auto pb = bsdata::find(bv.type->type, bsdata::firstenum);
					if(!pb || pb->count==0)
						return error(x, y, width, e, "Не найдена база");
					auto size = bv.type->size;
					if(bv.type->is(KindCFlags))
						size = bv.type->lenght;
					for(unsigned i = 0; i < pb->count; i++) {
						if(e.proc.isallow) {
							if(!e.proc.isallow(source.data, i))
								continue;
						}
						auto p = getpresent(pb->get(i));
						cmd_check ev({bv.type->ptr(bv.data), size}, p, i);
						unsigned flags = 0;
						if(ev.ischecked())
							flags |= Checked;
						y += checkbox(x, y, width, flags, ev, p, 0) + 2;
					}
				}
			}
			return y - y0;
		} else if(e.value.id) {
			auto bv = getvalue(source, e);
			if(!bv.data || !bv.type)
				return 0;
			auto pv = bv.type->ptr(bv.data);
			// Вначале найдем целую форму объекта
			if(bv.type->is(KindScalar) && !bv.type->isnum() && !bv.type->istext() && !bv.type->isref()) {
				auto pm = plugin<markup>::get(bv.type->type);
				if(!pm)
					return error(x, y, width, e, "Не найдена разметка");
				auto new_title = e.title;
				return group_vertial(x, y, width, pm, bsval(pv, bv.type->type), new_title);
			} else {
				draw::state push;
				setposition(x, y, width);
				header(x, y, width, e.title, title_width, TitleNormal, title_override);
				rect rc = {x, y, x + width, y + draw::texth() + 8};
				field_part(rc, e, source, bv, pv, true, rc.x2);
				return rc.height() + metrics::padding * 2;
			}
		} else if(e.value.child) {
			rect rgo = {};
			auto y0 = y;
			if(e.title)
				rgo = start_group(x, y, width, e.title);
			if(e.value.child[0].width)
				y += group_horizontal(x, y, width, e.value.child, source, title_override);
			else
				y += group_vertial(x, y, width, e.value.child, source, title_override);
			y += close_group(x, y, rgo);
			return y - y0;
		} else
			return 0;
	}
	constexpr markup_view() : title_width(128) {}
};

int draw::field(int x, int y, int width, const markup* elements, const bsval& source, int title_width) {
	markup_view ev;
	ev.title_width = title_width;
	const char* title_override = 0;
	if(elements->width)
		return ev.group_horizontal(x, y, width, elements, source, title_override);
	else
		return ev.group_vertial(x, y, width, elements, source, title_override);
}