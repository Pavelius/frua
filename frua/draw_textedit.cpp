#include "crt.h"
#include "draw_control.h"

using namespace draw;
using namespace draw::controls;

static bool isspace(char sym) {
	return sym == ' ' || sym == 10 || sym == 13 || sym == 9;
}

textedit::textedit(char* string, unsigned maxlenght, bool select_text) : string(string),
maxlenght(maxlenght),
p1(0), p2(-1),
align(0),
readonly(false),
rctext(metrics::edit),
cashed_width(-1),
cashed_string(0),
cashed_origin(0) {
	wheels.y = 8;
	if(select_text)
		select(zlen(string), true);
}

int	textedit::getbegin() const {
	if(p2 == -1)
		return p1;
	return imin(p1, p2);
}

int	textedit::getend() const {
	if(p2 == -1)
		return p1;
	return imax(p1, p2);
}

void textedit::clear() {
	if(p2 != -1 && p1 != p2) {
		char* s1 = string + getbegin();
		char* s2 = string + getend();
		while(*s2)
			*s1++ = *s2++;
		*s1 = 0;
		if(p1 > p2)
			p1 = p2;
	}
	p2 = -1;
}

void textedit::paste(const char* input) {
	clear();
	int i1 = zlen(string);
	int i2 = zlen(input);
	memmove(string + p1 + i2, string + p1, (i1 - p1 + 1) * sizeof(char));
	memcpy(string + p1, input, i2);
	select(p1 + i2, false);
	invalidate();
}

void textedit::correct() {
	int lenght = zlen(string);
	if(p2 != -1 && p2 > lenght)
		p2 = lenght;
	if(p1 > lenght)
		p1 = lenght;
	if(p1 < 0)
		p1 = 0;
}

void textedit::ensurevisible(int linenumber) {
	// TODO: ��������������� ������, ����� ��� ���� �����.
	// 1) ��������� ����� ���������� �����.
	// 2) ��������� ���������� ����� �� ������.
}

void textedit::select(int index, bool shift) {
	int lenght = zlen(string);
	if(index < 0)
		index = 0;
	else if(index > lenght)
		index = lenght;
	if(shift) {
		if(p2 == -1)
			p2 = p1;
	} else
		p2 = -1;
	p1 = index;
}

void textedit::left(bool shift, bool ctrl) {
	int n = p1;
	if(!ctrl)
		n -= 1;
	else {
		for(; n > 0 && isspace(string[n - 1]); n--);
		for(; n > 0 && !isspace(string[n - 1]); n--);
	}
	select(n, shift);
}

void textedit::right(bool shift, bool ctrl) {
	int n = p1;
	if(!ctrl)
		n += 1;
	else {
		for(; string[n] && !isspace(string[n]); n++);
		for(; string[n] && isspace(string[n]); n++);
	}
	select(n, shift);
}

int	textedit::lineb(int index) const {
	return draw::textlb(string, index, cashed_width);
}

int	textedit::linee(int index) const {
	auto line_count = 0;
	auto line_start = draw::textlb(string, index, cashed_width, 0, &line_count);
	auto n = line_start + line_count;
	if(string[n] == 0)
		return n;
	if(n)
		return n - 1;
	return 0;
}

int	textedit::linen(int index) const {
	int result = 0;
	draw::textlb(string, index, cashed_width, &result);
	return result;
}

point textedit::getpos(rect rc, int index, unsigned state) const {
	auto line_number = 0;
	auto line_count = 0;
	auto line_start = draw::textlb(string, index, rc.width(), &line_number, &line_count);
	auto p = string + line_start;
	auto x1 = aligned(rc.x1, rc.width(), state, textw(p, line_count));
	auto y1 = rc.y1 + draw::alignedh(rc, string, state);
	return{
		(short)(x1 + textw(string + line_start, index - line_start)),
		(short)(y1 + line_number * texth() - origin.y)
	};
}

int	textedit::hittest(rect rc, point pt, unsigned state) const {
	return draw::hittest({rc.x1, rc.y1 - origin.y, rc.x2, rc.y2}, string, state, pt);
}

void textedit::cashing(rect rc) {
	rcclient = rc;
	if(rc.width() != cashed_width) {
		cashed_width = rc.width();
		draw::state push;
		draw::setclip({0, 0, 0, 0});
		maximum.y = texte({0, 0, cashed_width, 256 * 256 * 256 * 64}, string, align, -1, -1);
	}
}

void textedit::invalidate() {
	cashed_width = -1;
	string[maxlenght] = 0;
}

void textedit::redraw(rect rc) {
	rc = rc + rctext;
	rc.y1 -= origin.y;
	cashing(rc);
	if(isfocused()) {
		auto ev = hot.key&CommandMask;
		if((/*ev==MouseMove || */ev == MouseLeft || ev == MouseRight || ev == MouseLeftDBL)
			&& draw::mouseinput && hot.pressed) {
			auto lenght = zlen(string);
			auto index = hittest(rc, hot.mouse, align);
			if(index == -3)
				index = lenght;
			else if(index == -2)
				index = 0;
			if(index >= 0) {
				switch(hot.key&CommandMask) {
				case MouseMove:
					select(index, true);
					break;
				case MouseLeftDBL:
					select(index, false);
					left(false, true);
					right(true, true);
					break;
				default:
					select(index, (hot.key&Shift) != 0);
					break;
				}
			}
		}
		texte(rc, string, align, p1, p2);
	} else
		text(rc, string, align);
}

bool textedit::keyinput(unsigned id) {
	switch(id) {
	case KeyDown:
		if(true) {
			auto pt = getpos(rcclient, p1, align);
			auto i = hittest(rcclient, {pt.x, (short)(pt.y + texth())}, align);
			if(i == -3)
				i = linee(linee(p1) + 1);
			if(i >= 0)
				select(i, (id & Shift) != 0);
			else
				return false;
		}
		break;
	case KeyUp:
		if(true) {
			auto pt = getpos(rcclient, p1, align);
			auto i = hittest(rcclient, {pt.x, (short)(pt.y - texth())}, align);
			if(i == -3)
				i = linee(lineb(p1) - 1);
			if(i >= 0)
				select(i, (id & Shift) != 0);
			else
				return false;
		}
		break;
	case KeyRight:
	case KeyRight | Shift:
	case KeyRight | Ctrl:
	case KeyRight | Shift | Ctrl:
		right((id&Shift) != 0, (id&Ctrl) != 0);
		break;
	case KeyLeft:
	case KeyLeft | Shift:
	case KeyLeft | Ctrl:
	case KeyLeft | Shift | Ctrl:
		left((id&Shift) != 0, (id&Ctrl) != 0);
		break;
	case KeyBackspace:
		if(!readonly) {
			if((p2 == -1 || p1 == p2) && p1 > 0)
				select(p1 - 1, true);
			clear();
		}
		break;
	case KeyDelete:
		if(!readonly) {
			if(p2 == -1 || p1 == p2)
				select(p1 + 1, true);
			clear();
		}
		break;
	case InputSymbol:
		if(hot.param >= 0x20 && !readonly) {
			char temp[8];
			paste(szput(temp, hot.param));
			invalidate();
		}
		break;
	case KeyHome:
	case KeyHome | Shift:
		select(lineb(p1), (id&Shift) != 0);
		break;
	case KeyEnd:
	case KeyEnd | Shift:
		select(linee(p1), (id&Shift) != 0);
		break;
	default:
		return scrollable::keyinput(id);
	}
	return true;
}

unsigned textedit::select_all(bool run) {
	if(run) {
		select(0, false);
		select(zlen(string), true);
	}
	return 0;
}

unsigned textedit::copy(bool run) {
	if(p2 == -1 || p1 == p2)
		return Disabled;
	if(run) {
		char* s1 = string + imin(p1, p2);
		char* s2 = string + imax(p1, p2);
		clipboard::copy(s1, s2 - s1);
	}
	return 0;
}

unsigned textedit::paste(bool run) {
	if(p1 == -1 || readonly)
		return Disabled;
	if(run) {
		clear();
		auto p = clipboard::paste();
		if(p)
			paste(p);
		delete p;
	}
	return 0;
}