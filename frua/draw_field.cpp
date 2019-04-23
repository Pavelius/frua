#include "anyval.h"
#include "crt.h"
#include "draw.h"
#include "draw_control.h"

using namespace draw;

enum field_type_s : unsigned char {
	FieldNumber, FieldText,
};

static anyval		edit_value;
static draw_events*	edit_events;

static const char* getvalue(const anyval& v, field_type_s t, char* result, const char* result_end) {
	if(!v)
		return "";
	const char* p;
	switch(t) {
	case FieldNumber: return szprint(result, result_end, "%1i", (int)v);
	case FieldText:
		p = (const char*)((int)v);
		if(p)
			return szprint(result, result_end, p);
		result[0] = 0;
		return result;
	default: return "";
	}
}

static void setvalue(const anyval& v, field_type_s t, const char* result) {
	if(!v)
		return;
	int value = 0;
	switch(t) {
	case FieldNumber: value = sz2num(result); break;
	case FieldText: value = (int)szdup(result); break;
	}
	v = value;
}

static class edit_driver : public controls::textedit {
	char			source[2048];
	anyval			value;
	field_type_s	type;
public:
	constexpr edit_driver() : textedit(source, sizeof(source) / sizeof(source), false),
		value(), source(), type(FieldNumber) {}
	bool isfocusable() const override {
		return false;
	}
	bool isfocused() const override {
		return getfocus() == (int)value.ptr();
	}
	void load() {
		auto p = getvalue(value, type, source, source + sizeof(source) / sizeof(source[0]) - 1);
		if(p != source)
			zcpy(source, p, sizeof(source));
		invalidate();
	}
	void save() {
		setvalue(value, type, source);
		if(edit_events)
			edit_events->changed(value);
	}
	void update(const anyval& ev, field_type_s et, int digits = -1) {
		if(value == ev)
			return;
		value = ev;
		type = et;
		if(digits==-1)
			setcount(sizeof(source)/ sizeof(source[0]) - 1);
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

int draw::field(int x, int y, int width, const char* header_label, const anyval& ev, int header_width, int digits, draw_events* pev) {
	draw::state push;
	setposition(x, y, width);
	if(header_label && header_label[0])
		titletext(x, y, width, 0, header_label, header_width);
	rect rc = {x, y, x + width, y + draw::texth() + 8};
	unsigned flags = AlignRight;
	focusing((int)ev.ptr(), flags, rc);
	bool focused = isfocused(flags);
	draw::rectb(rc, colors::border);
	rect rco = rc;
	auto result = addbutton(rc, focused,
		"+", KeyUp, "���������",
		"-", KeyDown, "���������");
	switch(result) {
	case 1: execute(field_up, ev);  break;
	case 2: execute(field_down, ev); break;
	}
	auto a = area(rc);
	if(isfocused(flags)) {
		edit_events = pev;
		edit.align = flags & AlignMask;
		edit.update(ev, FieldNumber, digits);
		edit.view(rc);
	} else {
		char temp[32];
		auto p = getvalue(ev, FieldNumber, temp, temp + sizeof(temp) / sizeof(temp[0]));
		draw::texte(rc + metrics::edit, p, flags, -1, -1);
	}
	return rc.height() + metrics::padding * 2;
}

int draw::field(int x, int y, int width, const char* header_label, const char*& sev, int header_width, draw_events* pev) {
	draw::state push;
	setposition(x, y, width);
	if(header_label && header_label[0])
		titletext(x, y, width, 0, header_label, header_width);
	rect rc = {x, y, x + width, y + draw::texth() + 8};
	unsigned flags = 0;
	anyval ev = sev;
	focusing((int)ev.ptr(), flags, rc);
	bool focused = isfocused(flags);
	draw::rectb(rc, colors::border);
	auto a = area(rc);
	if(isfocused(flags)) {
		edit_events = pev;
		edit.align = flags & AlignMask;
		edit.update(ev, FieldText);
		edit.view(rc);
	} else {
		char temp[260];
		auto p = getvalue(ev, FieldText, temp, temp + sizeof(temp) / sizeof(temp[0]));
		draw::texte(rc + metrics::edit, p, flags, -1, -1);
	}
	return rc.height() + metrics::padding * 2;
}

void field_before() {
	edit_events = 0;
}