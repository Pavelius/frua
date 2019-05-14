#include "bsreq.h"
#include "crt.h"
#include "draw_control.h"

using namespace draw;
using namespace draw::controls;

int draw::button(int x, int y, const char* string, const runable& ev, unsigned key, bool checked) {
	auto id = ev.getid();
	auto dx = textw(string);
	rect rc = {x, y, x + dx + metrics::padding * 2, y + texth() + metrics::padding * 2};
	addelement(id, rc);
	auto focused = getfocus() == ev.getid();
	if(draw::buttonh(rc, checked, focused, false, true, string, key, false))
		ev.execute();
	if(focused)
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
	return rc.width() + 2;
}

static void add_value() {
	auto& p = *((int*)hot.param);
	p++;
}

static void sub_value() {
	auto& p = *((int*)hot.param);
	p--;
}

bool draw::edit(const char* name, void* object, unsigned size, const bsreq* type, const markup* elements, bool creating) {
	int x, y;
	if(creating)
		elements->action("create", object);
	char r2_buffer[256];
	auto r2 = r2_buffer;
	if(size > sizeof(r2_buffer))
		r2 = new char[size];
	auto old_focus = getfocus();
	auto current_page = 0;
	const markup* page_markup_last = 0;
	auto updater = elements->find("update", object, 0, false);
	auto type_key = type;
	openform();
	while(ismodal()) {
		auto page_maximum = elements->getpagecount(object);
		if(current_page < 0)
			current_page = 0;
		if(current_page > page_maximum)
			current_page = page_maximum - 1;
		auto page_name = name;
		const char* page_title = 0;
		auto page = elements;
		auto page_markup = elements->getpage(object, current_page);
		if(page_markup) {
			page = page_markup->value.child;
			if(page_markup->title)
				page_title = page_markup->title;
			if(page_markup_last != page_markup) {
				page_markup_last = page_markup;
				setfocus(0, true);
				if(page_markup->cmd.execute)
					cmd(page_markup->cmd.execute, object).execute();
			}
		}
		if(type_key && type_key->is(KindText)) {
			auto pn = (const char*)type_key->get(type_key->ptr(object));
			if(pn && pn[0])
				page_name = pn;
		}
		pageheader(x, y, 0, page_name, page_title, current_page, page_maximum);
		auto width = getwidth() - x * 2;
		y += draw::field(x, y, width, page, bsval(object, type), 100);
		pagefooter(x, y, true);
		if(page_maximum > 0) {
			if(current_page < page_maximum - 1)
				x += button(x, y, "Далее", cmd(add_value, (int)&current_page), KeyPageDown);
			if(current_page > 0)
				x += button(x, y, "Назад", cmd(sub_value, (int)&current_page), KeyPageUp);
		}
		auto commands = page->findcommands(object);
		if(commands && commands->value.child) {
			for(auto p = commands->value.child; *p; p++)
				x += button(x, y, p->title, cmd(p->cmd.execute, object), 0);
		}
		memcpy(r2, object, size);
		domodal();
		if(updater) {
			// Если ПОСЛЕ обработки команды менялся оригинальный объект вызываем специальное событие
			// В нем мы можем проанализировать что именно поменялось и если надо что-то обновить.
			if(updater->cmd.change && memcmp(object, r2, size) != 0)
				updater->cmd.change(object, r2);
		}
	}
	closeform();
	setfocus(old_focus, true);
	if(r2 != r2_buffer)
		delete r2;
	auto result = getresult() != 0;
	if(result)
		elements->action("apply", object);
	return result;
}