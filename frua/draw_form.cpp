#include "bsreq.h"
#include "crt.h"
#include "draw_control.h"

using namespace draw;

static void add_value() {
	auto& p = *((int*)hot.param);
	p++;
}

static void sub_value() {
	auto& p = *((int*)hot.param);
	p--;
}

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

bool draw::edit(const char* name, void* source, unsigned size, unsigned& count, unsigned maximum,
	const bsreq* meta, const markup* elements,
	void* object, void* copy_object) {
	if(!size)
		return false;
	if(!object && count >= maximum)
		return false;
	if(!elements)
		return false;
	auto fm = elements->getform(source, "element");
	if(fm) {
		elements = fm->value.child;
		if(fm->title)
			name = fm->title;
	}
	if(!name)
		name = "Форма редактирования";
	auto result = true;
	char copy_buffer[256] = {};
	auto copy = copy_buffer;
	auto creating = false;
	if(size > sizeof(copy_buffer))
		copy = new char[size];
	if(object)
		memcpy(copy, object, size);
	else if(copy_object)
		memcpy(copy, copy_object, size);
	else {
		memset(copy, 0, size);
		creating = true;
	}
	if(draw::edit(name, copy, size, meta, elements, creating)) {
		if(!object)
			object = (char*)source + (count++)*size;
		if(object)
			memcpy(object, copy, size);
	} else
		result = false;
	if(copy != copy_buffer)
		delete copy;
	return result;
}

class cmdfm : public runable {
	const char*		name;
	const markup*	columns;
	const bsreq*	type;
	void*			object;
	void*			source;
	unsigned*		count;
	unsigned		size;
	unsigned		maximum;
	callback		proc;
	static cmdfm	e;
public:
	cmdfm() = default;
	constexpr cmdfm(callback proc, void* object, const bsreq* type = 0, const markup* columns = 0, const char* name = 0, void* source = 0, unsigned size = 0, unsigned* count = 0, unsigned maximum = 0) : proc(proc),
		object(object), type(type), columns(columns),
		name(name),
		source(source), size(size), count(count), maximum(maximum) {
	}
	int	getid() const override { return (int)proc; }
	void execute() const override {
		e = *this;
		draw::execute(proc);
	}
	static void change() {
		if(!e.count)
			return;
		draw::edit(e.name, e.source, e.size, *e.count, e.maximum,
			e.type, e.columns,
			e.object, 0);
	}
	static void add() {
		draw::edit(e.name, e.source, e.size, *e.count, e.maximum,
			e.type, e.columns,
			0, 0);
	}
	static void copy() {
		draw::edit(e.name, e.source, e.size, *e.count, e.maximum,
			e.type, e.columns,
			0, e.object);
	}
};
cmdfm cmdfm::e;

void* draw::choose(const char* name, void* source, unsigned size, unsigned& count, unsigned maxcount, const bsreq* meta,
	const markup* columns, const markup* element, bool choose_mode) {
	controls::listtable e1(source, count, size, meta, columns);
	e1.show_border = false;
	int x, y;
	openform();
	while(ismodal()) {
		pageheader(x, y, "Доступные", name, 0);
		auto w = getwidth() - metrics::padding * 2;
		e1.view({x, y, x + w, (getheight() - 16 + 8 * 2) - metrics::padding});
		pagefooter(x, y, choose_mode);
		auto pr = e1.getrow(e1.current);
		if(e1.getmaximum() > 0) {
			cmdfm pc(cmdfm::change, pr, meta, element, name, source, size, &count, maxcount);
			x += button(x, y, "Редактировать", pc, F2);
			if(hot.key == KeyEnter) {
				if(choose_mode)
					execute(buttonok);
				else
					pc.execute();
			}
		}
		if(count < maxcount) {
			x += button(x, y, "Добавить",
				cmdfm(cmdfm::add, pr, meta, element, name, source, size, &count, maxcount),
				F3);
			if(e1.getmaximum() > 0) {
				x += button(x, y, "Скопировать",
					cmdfm(cmdfm::copy, pr, meta, element, name, source, size, &count, maxcount),
					F4);
			}
		}
		domodal();
	}
	closeform();
	if(!getresult())
		return 0;
	return e1.getcurrent();
}