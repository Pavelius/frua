#include "anyval.h"
#include "crt.h"
#include "draw.h"
#include "draw_control.h"

using namespace draw;

static anyval		edit_value;

static const char* getvalue(const anyval& v, char* result, const char* result_end) {
	if(!v)
		return "";
	return szprint(result, result_end, "%1i", (int)v);
}

static void setvalue(const anyval& v, const char* result) {
	if(!v)
		return;
	auto value = sz2num(result);
	v = value;
}

static class edit_driver : public controls::textedit {
	char	source[32];
	anyval	value;
public:
	constexpr edit_driver() : textedit(source, sizeof(source) / sizeof(source), false), value(), source() {}
	bool isfocusable() const override {
		return false;
	}
	bool isfocused() const override {
		return getfocus() == (int)value.getptr();
	}
	void load() {
		getvalue(value, source, source + sizeof(source) / sizeof(source[0]) - 1);
		invalidate();
	}
	void save() {
		setvalue(value, source);
	}
	void update(const anyval& ev, int digits = -1) {
		if(value == ev)
			return;
		value = ev;
		if(digits==-1)
			setcount(sizeof(source)/ sizeof(source[0]));
		else
			setcount(digits);
		load();
		select_all(true);
	}
	void clear() {
		textedit::clear();
		value.clear();
	}
} edit;

void save_focus() {
	if(edit.isfocused())
		edit.save();
}

static void field_up() {
	edit_value = edit_value + 1;
	edit.load();
	edit.invalidate();
}

static void field_down() {
	edit_value = edit_value - 1;
	edit.load();
	edit.invalidate();
}

static void execute(callback proc, const anyval& ev) {
	edit_value = ev;
	draw::execute(proc);
}

int draw::field(int x, int y, int width, const char* header_label, const anyval& ev, int header_width, int digits) {
	draw::state push;
	setposition(x, y, width);
	if(header_label && header_label[0])
		titletext(x, y, width, 0, header_label, header_width);
	rect rc = {x, y, x + width, y + draw::texth() + 8};
	unsigned flags = AlignRight;
	focusing((int)ev.getptr(), flags, rc);
	bool focused = isfocused(flags);
	draw::rectb(rc, colors::border);
	rect rco = rc;
	auto result = addbutton(rc, focused,
		"+", KeyUp, "Увеличить",
		"-", KeyDown, "Уменьшить");
	switch(result) {
	case 1: execute(field_up, ev);  break;
	case 2: execute(field_down, ev); break;
	}
	auto a = area(rc);
	if(isfocused(flags)) {
		edit.align = flags & AlignMask;
		edit.update(ev, digits);
		edit.view(rc);
	} else {
		char temp[260];
		auto p = getvalue(ev, temp, temp + sizeof(temp) / sizeof(temp[0]));
		draw::texte(rc + metrics::edit, p, flags, -1, -1);
	}
	return rc.height() + metrics::padding * 2;
}