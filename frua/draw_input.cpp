#include "draw.h"
#include "draw_control.h"
#include "main.h"

using namespace draw;

void tooltips_after();
void tooltips_before();
void view_initialize();

struct focusable_element {
	int			id;
	rect		rc;
	operator bool() const { return id != 0; }
};
static focusable_element elements[96];
static focusable_element* render_control;
static draw::controls::control*		current_focus_control;
static draw::controls::control*		current_hilite_control;
extern rect		sys_static_area;
static int		current_focus;
static bool		break_modal;
static int		break_result;
static callback	leave_focus;
callback		draw::domodal;

static void standart_domodal() {
	int id;
	tooltips_after();
	hot.key = draw::rawinput();
	if(current_hilite_control) {
		switch(hot.key & CommandMask) {
		case MouseLeft:
		case MouseRight:
		case MouseLeftDBL:
			current_hilite_control->mouseinput(hot.key, hot.mouse);
			return;
		case MouseWheelDown:
			current_hilite_control->mousewheel(hot.key, hot.mouse, 1);
			return;
		case MouseWheelUp:
			current_hilite_control->mousewheel(hot.key, hot.mouse, -1);
			return;
		}
	}
	if(current_focus_control) {
		if(current_focus_control->keyinput(hot.key))
			return;
	}
	switch(hot.key) {
	case KeyTab:
	case KeyTab | Shift:
	case KeyTab | Ctrl:
	case KeyTab | Ctrl | Shift:
	case KeyLeft:
	case KeyRight:
	case KeyUp:
	case KeyDown:
		id = getnext(draw::getfocus(), hot.key);
		if(id)
			setfocus(id, true);
		break;
	case 0:
		exit(0);
		break;
	}
}

static void setfocus_callback() {
	setfocus(hot.param, true);
	if(hot.key == MouseLeft || hot.key == MouseRight)
		hot.key = InputUpdate;
}

static focusable_element* getby(int id) {
	if(!id)
		return 0;
	for(auto& e : elements) {
		if(!e)
			return 0;
		if(e.id == id)
			return &e;
	}
	return 0;
}

static focusable_element* getfirst() {
	for(auto& e : elements) {
		if(!e)
			return 0;
		return &e;
	}
	return 0;
}

static focusable_element* getlast() {
	auto p = elements;
	for(auto& e : elements) {
		if(!e)
			break;
		p = &e;
	}
	return p;
}

bool controls::control::isfocused() const {
	return (control*)current_focus == this;
}

bool controls::control::ishilited() const {
	return current_hilite_control == this;
}

void draw::controls::control::view(const rect& rc) {
	if(isfocusable()) {
		addelement((int)this, rc);
		if(!getfocus())
			setfocus((int)this, true);
	}
	if(areb(rc))
		current_hilite_control = this;
	if(isfocused())
		current_focus_control = this;
	if(show_border)
		rectb(rc, colors::border);
}

void draw::addelement(int id, const rect& rc) {
	if(!render_control
		|| render_control >= elements + sizeof(elements) / sizeof(elements[0]) - 1)
		render_control = elements;
	render_control[0].id = id;
	render_control[0].rc = rc;
	render_control[1].id = 0;
	render_control++;
}

int draw::getnext(int id, int key) {
	if(!key)
		return id;
	auto pc = getby(id);
	if(!pc)
		pc = getfirst();
	if(!pc)
		return 0;
	auto pe = pc;
	auto pl = getlast();
	int inc = 1;
	if(key == KeyLeft || key == KeyUp || key == (KeyTab | Shift))
		inc = -1;
	while(true) {
		pc += inc;
		if(pc > pl)
			pc = elements;
		else if(pc < elements)
			pc = pl;
		if(pe == pc)
			return pe->id;
		switch(key) {
		case KeyRight:
			if(pe->rc.y1 >= pc->rc.y1
				&& pe->rc.y1 <= pc->rc.y2
				&& pe->rc.x1 < pc->rc.x1)
				return pc->id;
			break;
		case KeyLeft:
			if(pe->rc.y1 >= pc->rc.y1
				&& pe->rc.y1 <= pc->rc.y2
				&& pe->rc.x1 > pc->rc.x1)
				return pc->id;
			break;
		case KeyDown:
			if(pc->rc.y1 >= pe->rc.y2)
				return pc->id;
			break;
		case KeyUp:
			if(pc->rc.y2 <= pe->rc.y1)
				return pc->id;
			break;
		default:
			return pc->id;
		}
	}
}

void save_focus();

void draw::setfocus(int id, bool instant) {
	if(id == current_focus)
		return;
	if(instant) {
		if(current_focus!=id)
			save_focus();
		current_focus = id;
	} else
		execute(setfocus_callback, id);
}

int draw::getfocus() {
	return current_focus;
}

void draw::openform() {
	setfocus(0, true);
}

void draw::closeform() {
}

void draw::execute(callback proc, int param) {
	domodal = proc;
	hot.key = 0;
	hot.param = param;
}

void draw::breakmodal(int result) {
	break_modal = true;
	break_result = result;
}

void draw::buttoncancel() {
	breakmodal(0);
}

void draw::buttonok() {
	breakmodal(1);
}

int draw::getresult() {
	return break_result;
}

bool draw::ismodal() {
	current_focus_control = 0;
	current_hilite_control = 0;
	render_control = elements;
	if(hot.mouse.x < 0 || hot.mouse.y < 0)
		sys_static_area.clear();
	else
		sys_static_area = {0, 0, draw::getwidth(), draw::getheight()};
	tooltips_before();
	domodal = standart_domodal;
	if(!break_modal)
		return true;
	break_modal = false;
	setfocus(0, true);
	return false;
}

void set_dark_theme() {
	colors::active = color::create(172, 128, 0);
	colors::border = color::create(73, 73, 80);
	colors::button = color::create(0x42, 0x42, 0x42);
	colors::form = color::create(64, 64, 64);
	colors::window = color::create(32, 32, 32);
	colors::text = color::create(255, 255, 255);
	colors::edit = color::create(38, 79, 120);
	colors::h1 = colors::text.mix(colors::edit, 64);
	colors::h2 = colors::text.mix(colors::edit, 96);
	colors::h3 = colors::text.mix(colors::edit, 128);
	colors::special = color::create(255, 244, 32);
	colors::border = colors::window.mix(colors::text, 128);
	colors::tips::text = color::create(255, 255, 255);
	colors::tips::back = color::create(100, 100, 120);
	colors::tabs::back = color::create(255, 204, 0);
	colors::tabs::text = colors::black;
}

void draw::initialize() {
	set_dark_theme();
	draw::font = metrics::font;
	draw::fore = colors::text;
	draw::fore_stroke = colors::blue;
	view_initialize();
	create(-1, -1, 800, 600, 0, 32);
}