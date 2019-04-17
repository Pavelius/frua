#include "draw.h"
#include "draw_control.h"
#include "main.h"

using namespace draw;

struct focusable_element {
	int			id;
	rect		rc;
	operator bool() const { return id != 0; }
};
static focusable_element elements[96];
static focusable_element* render_control;
static draw::controls::control* current_focus_control;
static int		current_focus;
static bool		break_modal;
static int		break_result;
callback		draw::domodal;

static void standart_domodal() {
	int id;
	hot.key = draw::rawinput();
	if(current_focus_control) {
		if(current_focus_control->keyinput(hot.key))
			return;
	}
	switch(hot.key) {
	case KeyTab:
	case KeyTab | Shift:
	case KeyTab | Ctrl:
	case KeyTab | Ctrl | Shift:
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
	return false;
}

void draw::controls::control::view(const rect& rc) {
	if(isfocusable()) {
		addelement((int)this, rc);
		if(!getfocus())
			setfocus((int)this, true);
	}
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

void draw::setfocus(int id, bool instant) {
	if(id == current_focus)
		return;
	if(instant)
		current_focus = id;
	else
		execute(setfocus_callback, id);
}

int draw::getfocus() {
	return current_focus;
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
	render_control = elements;
	domodal = standart_domodal;
	if(!break_modal)
		return true;
	break_modal = false;
	return false;
}

void set_dark_theme() {
	colors::active = color::create(172, 128, 0);
	colors::border = color::create(73, 73, 80);
	colors::button = color::create(0, 122, 204);
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
	create(-1, -1, 800, 600, 0, 32);
}