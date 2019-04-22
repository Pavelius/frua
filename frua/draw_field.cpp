#include "anyval.h"
#include "crt.h"
#include "draw.h"
#include "draw_control.h"

using namespace draw;

static anyval		edit_value;
static callback		edit_proc;

static const char* getvalue(const anyval& v, char* result, const char* result_end) {
	if(!v)
		return "";
	return szprint(result, result_end, "%1i", (int)v);
}

static void setvalue(const anyval& v, const char* result) {
	if(!v)
		return;
}

static struct edit_field_widget : controls::textedit {

	char	buffer[4192];
	anyval	value;

	edit_field_widget() : textedit(buffer, sizeof(buffer) - 1, false) {
		buffer[0] = 0;
		show_border = false;
	}

	//bool isfocused() const override {
	//	return (void*)getfocus() == value.getptr();
	//}

	bool isfocusable() const override {
		return false;
	}

	void setfocus(bool instant) {
		draw::setfocus((int)value.getptr(), instant);
	}

	void write() {
		setvalue(value, buffer);
	}

	void read() {
		getvalue(value, buffer, zendof(buffer));
		select_all(true);
		invalidate();
	}

	void set(const anyval& e) {
		if(value==e)
			return;
		write();
		value = e;
		read();
	}

} edit;

void draw::updatefocus() {
	if(edit.isfocused())
		edit.write();
}

static void loadfocus() {
	if(edit.isfocused())
		edit.read();
}

static void field_up() {
	edit.write();
	edit_value = edit_value + 1;
	loadfocus();
}

static void field_down() {
	edit.write();
	edit_value = edit_value - 1;
	loadfocus();
}

static void execute(callback proc, const anyval& ev) {
	edit_value = ev;
	draw::execute(proc);
}

int draw::field(int x, int y, int width, unsigned flags, const anyval& ev, const char* header_label, int header_width, const char* tips) {
	draw::state push;
	setposition(x, y, width);
	decortext(flags);
	if(header_label && header_label[0])
		titletext(x, y, width, flags, header_label, header_width);
	rect rc = {x, y, x + width, y + draw::texth() + 8};
	if(!isdisabled(flags))
		draw::rectf(rc, colors::window);
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
		edit.set(ev);
		edit.view(rc);
	} else {
		char temp[260];
		auto p = getvalue(ev, temp, temp + sizeof(temp)/ sizeof(temp[0]));
		draw::texte(rc + metrics::edit, p, flags, -1, -1);
	}
	if(tips && a == AreaHilited)
		tooltips(tips);
	return rc.height() + metrics::padding * 2;
}