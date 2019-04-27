#include "anyval.h"
#include "bsreq.h"
#include "crt.h"
#include "draw_control.h"
#include "markup.h"

using namespace draw;

const markup* getmarkup(const bsreq* type);

namespace {
enum title_s : unsigned char { NoTitle, TitleNormal, TitleShort };
struct commandi {
	void*			object;
	bsdata*			data;
	markup::proci	proc;
	rect			rc;
	anyval			value;
	void clear() { memset(this, 0, sizeof(commandi)); }
};
static commandi	command;
static void check_flags() {
	unsigned v1 = command.value;
	unsigned v2 = 1 << hot.param;
	if(v1&v2)
		v1 &= ~v2;
	else
		v1 |= v2;
	command.value = (const int)v1;
}
static void set_command_value() {
	command.value = (const int)hot.param;
}
struct cmd_check : runable {
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
struct cmd_radio : runable {
	constexpr cmd_radio(const anyval& value, const void* source, unsigned index)
		: source(source), index(index), value(value) {}
	virtual int	getid() const { return (int)source; }
	virtual void execute() const { command.value = value; draw::execute(set_command_value, index); }
	virtual bool isdisabled() const { return false; }
	bool ischecked() const { return (int)value == index; }
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
	struct enum_view : controls::list, adat<int, 512> {
		markup::proci	proc;
		const bsdata&	source;
		const char*	getname(char* result, const char* result_max, int line, int column) const {
			if(proc.getname)
				return proc.getname(source.get(data[line]), result, result_max, column);
			switch(column) {
			case 0: return getpresent(source.get(data[line]));
			}
			return "";
		}
		int getmaximum() const override { return count; }
		int getcurrent() const {
			if(!count)
				return 0;
			return data[current];
		}
		constexpr enum_view(const bsdata& source) : source(source), proc() {}
	};
	enum_view ev(*command.data);
	ev.hilite_odd_lines = false;
	ev.proc = command.proc;
	auto i1 = 0;
	auto i2 = command.data->count - 1;
	for(unsigned i = i1; i < i2; i++) {
		if(command.proc.isallow) {
			if(!command.proc.isallow(command.object, i))
				continue;
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

static bsval getvalue(const bsval& source, const markup& e) {
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

static int element(int x, int y, int width, int title_width, const markup& e, const bsval& source, title_s title_state, const char*& title_override, int* right_x2 = 0);

static int group_vertial(int x, int y, int width, int title_width, const markup* elements, const bsval& source, title_s title_state, const char*& title_override) {
	if(!elements)
		return 0;
	auto y0 = y;
	for(auto p = elements; *p; p++)
		y += element(x, y, width, title_width, *p, source, title_state, title_override);
	return y - y0;
}

static int group_horizontal(int x, int y, int width, int title_width, const markup* elements, const bsval& source, title_s title_state, const char*& title_override) {
	if(!elements)
		return 0;
	auto ym = 0;
	auto wi = width / 12;
	auto wp = 0;
	for(auto p = elements; *p; p++) {
		auto x1 = x + wp * wi;
		auto we = p->width*wi;
		if(x1 + we > width - 12)
			we = width - x1;
		auto yc = element(x, y, we, title_width, *p, source, title_state, title_override);
		if(ym < yc)
			ym = yc;
		x += we;
	}
	return ym;
}

static void header(int& x, int y, int& width, const char* label, int title_width, title_s state, const char*& title_override) {
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
		titletext(x, y, width, 0, label, w, 0);
		x += metrics::padding;
		width -= metrics::padding;
	} else
		titletext(x, y, width, 0, label, title_width);
}

static int error(int x, int y, int width, int title_width, const markup& e, const char* error_text) {
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

static int field_main(int x, int y, int width, int title_width, const bsval& source, void* pv, const bsreq* type, int param, const char* title_text, title_s title_state, const char*& title_override, int* right_x2, const markup* child, const markup::proci* pri) {
	auto xe = x + width;
	auto y0 = y;
	setposition(x, y, width);
	header(x, y, width, title_text, title_width, title_state, title_override);
	rect rc = {x, y, x + width, y + draw::texth() + 8};
	unsigned flags = AlignLeft;
	if(type->isnum())
		flags = AlignRight;
	draw::focusing((int)pv, flags, rc);
	if(type->istext())
		draw::field(rc, flags, anyval(pv, type->size), -1, FieldText);
	else if(type->isnum()) {
		auto d = param;
		if(!d)
			d = 2;
		auto we = draw::textw("0")*(d + 1) + (draw::texth() + 8) + 4 * 2;
		rc.x2 = rc.x1 + we;
		draw::field(rc, flags, anyval(pv, type->size), d, FieldNumber);
		if(right_x2)
			*right_x2 = rc.x2;
		if(child) {
			for(auto p = child; *p && rc.x2 < xe; p++)
				element(rc.x2, y0, xe - rc.x2, title_width, *p, source, TitleShort, title_override, &rc.x2);
		}
	} else if(type->is(KindEnum)) {
		auto pb = bsdata::find(type->type, bsdata::firstenum);
		if(pb) {
			char temp[128];
			auto focused = isfocused(flags);
			auto result = focused && (hot.key == KeyEnter || hot.key == F2);
			anyval ev(pv, type->size);
			if(buttonh(rc, false, focused, false, true, 0, 0, false, 0))
				result = true;
			auto pn = "";
			auto pv = pb->get(ev);
			if(pri && pri->getname)
				pn = pri->getname(pv, temp, zendof(temp), 0);
			else
				pn = getpresent(pb->get(ev));
			textc(rc.x1 + 4, rc.y1 + 4, rc.width() - 4 * 2, pn);
			if(focused)
				rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
			if(result) {
				command.clear();
				command.rc = rc;
				command.value = ev;
				command.data = pb;
				if(pri)
					command.proc = *pri;
				execute(choose_enum);
			}
		}
	}
	return rc.height() + metrics::padding * 2;
}

static int element(int x, int y, int width, int title_width, const markup& e, const bsval& source, title_s title_state, const char*& title_override, int* right_x2) {
	if(e.proc.isvisible && !e.proc.isvisible(source.data, e))
		return 0;
	else if(e.value.id && e.value.id[0] == '#')
		// Страницы, команды, любые другие управляющие структуры.
		return 0;
	else if(e.proc.custom)
		return e.proc.custom(x, y, width, e.value.id, source.data);
	else if(e.proc.command)
		return button(x, y, width, 0, cmd(e.proc.command, source.data), e.title);
	else if(e.title && e.title[0] == '#') {
		auto pn = e.title + 1;
		auto y0 = y;
		if(e.value.id) {
			auto bv = getvalue(source, e);
			if(!bv.data || !bv.type)
				return 0;
			if(strcmp(pn, "checkboxes") == 0 && (bv.type->is(KindEnum) || bv.type->is(KindCFlags))) {
				auto pb = bsdata::find(bv.type->type, bsdata::firstenum);
				if(!pb || pb->count == 0)
					return error(x, y, width, title_width, e, "Не найдена база");
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
			} else if(strcmp(pn, "radiobuttons") == 0 && bv.type->is(KindEnum)) {
				auto pb = bsdata::find(bv.type->type, bsdata::firstenum);
				if(!pb || pb->count == 0)
					return error(x, y, width, title_width, e, "Не найдена база");
				auto size = bv.type->size;
				for(unsigned i = 0; i < pb->count; i++) {
					if(e.proc.isallow) {
						if(!e.proc.isallow(source.data, i))
							continue;
					}
					auto p = getpresent(pb->get(i));
					cmd_radio ev({bv.type->ptr(bv.data), size}, p, i);
					unsigned flags = 0;
					if(ev.ischecked())
						flags |= Checked;
					y += radio(x, y, width, flags, ev, p, 0) + 2;
				}
			} else {
				if(!bv.type->isnum())
					return error(x, y, width, title_width, e, bv.type->id);
				auto pb = bsdata::find(pn, bsdata::firstenum);
				if(!pb)
					return error(x, y, width, title_width, e, pn);
				if(pb->count == 0)
					return 0;
				auto count = pb->count;
				if(count > bv.type->count)
					count = bv.type->count;
				for(unsigned i = 0; i < pb->count; i++) {
					auto pv = bv.type->ptr(bv.data, i);
					y += field_main(x, y, width, title_width, source, pv, bv.type,
						e.param, getpresent(pb->get(i)), title_state, title_override, right_x2, 0, &e.proc);
				}
			}
		}
		return y - y0;
	} else if(e.value.id) {
		auto bv = getvalue(source, e);
		if(!bv.data || !bv.type)
			return 0; // Данные не найдены, но это не ошибка
		auto pv = bv.type->ptr(bv.data, e.value.index);
		// Вначале найдем целую форму объекта
		if(bv.type->is(KindScalar) && !bv.type->isnum() && !bv.type->istext() && !bv.type->isref()) {
			auto pm = getmarkup(bv.type->type);
			if(!pm)
				return error(x, y, width, title_width, e, "Не найдена разметка");
			auto new_title = e.title;
			return group_vertial(x, y, width, title_width, pm, bsval(pv, bv.type->type), title_state, new_title);
		} else {
			return field_main(x, y, width, title_width, source, pv, bv.type, e.param,
				e.title, title_state, title_override, right_x2, e.value.child,
				&e.proc);
		}
	} else if(e.value.child) {
		rect rgo = {};
		auto y0 = y;
		if(e.title)
			rgo = start_group(x, y, width, e.title);
		if(e.value.child[0].width)
			y += group_horizontal(x, y, width, title_width, e.value.child, source, title_state, title_override);
		else
			y += group_vertial(x, y, width, title_width, e.value.child, source, title_state, title_override);
		y += close_group(x, y, rgo);
		return y - y0;
	} else
		return 0;
}

int draw::field(int x, int y, int width, const markup* elements, const bsval& source, int title_width) {
	if(!elements)
		return 0;
	const char* title_override = 0;
	if(elements->width)
		return group_horizontal(x, y, width, title_width, elements, source, TitleNormal, title_override);
	else
		return group_vertial(x, y, width, title_width, elements, source, TitleNormal, title_override);
}