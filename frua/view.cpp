#include "crt.h"
#include "draw_control.h"
#include "io.h"
#include "main.h"

using namespace draw;

const int picture_width = 320;
const int picture_height = 300;
const int title_width = 100;
const int buttons_height = 16 + 8 * 2;
const int y_buttons = 600 - buttons_height;
const int combat_moverate = 3;

struct page_view;
struct command;

static anyval					command_value;
static bsdata*					command_data;
static rect						command_rect;
static draw_events*				command_draw_events;
static int						command_range[2];
static char						command_mask[260];
static agrw<picture_info>		file_data;
static const char*				file_exclude[] = {"fonts", "monsters", 0};
static const char*				file_ext_exclude[] = {"pma", 0};
static const char*				file_monster_exclude[] = {0};
static const sprite*			spr_monsters;
aref<sprite_name_info>			avatar_data;
typedef adat<const char*, 256>	strings_array;

static void set_command_value() {
	command_value = (const int)hot.param;
}

struct command {
	const char*			name;
	callback			proc;
	unsigned			key;
	explicit operator bool() const { return name != 0; }
};

static struct char_code_info {
	char ru, en;
} char_code[] = {{'А', 'F'}, {'Б', ','}, {'В', 'D'},
{'Г', 'U'}, {'Д', 'L'}, {'Е', 'T'},
{'Ж', ';'}, {'З', 'B'}, {'И', 'B'},
{'Й', 'Q'}, {'К', 'R'}, {'Л', 'K'},
{'М', 'V'}, {'Н', 'Y'}, {'О', 'J'},
{'П', 'G'}, {'Р', 'H'}, {'С', 'C'},
{'Т', 'N'}, {'У', 'E'}, {'Ф', 'A'},
{'Х', '['}, {'Ц', 'W'}, {'Ч', 'X'},
{'Ш', 'I'}, {'Щ', 'O'}, {'Ъ', ']'},
{'Ы', 'S'}, {'Ь', 'M'}, {'Э', '\''},
{'Ю', '.'}, {'Я', 'Z'},
};
struct main_picture_info : surface, picture_info {
	bool load(const picture_info& pi) {
		if(id && folder
			&& strcmp(id, pi.id) == 0
			&& strcmp(folder, pi.folder) == 0)
			return true;
		*static_cast<picture_info*>(this) = pi;
		if(*static_cast<surface*>(this))
			this->clear();
		char temp[260]; geturl(temp);
		this->read(temp);
		return true;
	}
} picture;
struct draw_events {
	virtual bool		isallow(const bsdata& ei, int index) const { return true; };
};

static void sprite_write(const sprite* p, const char* url) {
	io::file file(url, StreamWrite);
	if(!file)
		return;
	file.write(p, p->size);
	pma trail = {0};
	file.write(&trail, sizeof(trail));
}

static void save_monsters() {
	sprite_write(spr_monsters, "art/monsters.pma");
	buttonok();
}

static void save_campaign() {
	bsdata::write("campaigns/autosave.dat");
}

static void load_campaign() {
	bsdata::read("campaigns/autosave.dat");
}

void view_initialize() {
	spr_monsters = (sprite*)loadb("art/monsters.pma");
	if(spr_monsters) {
		avatar_data.data = (sprite_name_info*)spr_monsters->edata();
		avatar_data.count = spr_monsters->count / 2;
	}
	load_campaign();
}

static int button(int x, int y, const char* string, const runable& ev, unsigned key, bool checked = false) {
	auto id = ev.getid();
	auto dx = textw(string);
	rect rc = {x, y, x + dx + metrics::padding * 2, y + texth() + metrics::padding * 2};
	addelement(id, rc);
	auto focused = getfocus() == ev.getid();
	if(draw::buttonh(rc, checked, focused, ev.isdisabled(), true, string, key, false))
		ev.execute();
	if(focused)
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
	return rc.width() + 2;
}

static void render_picture(int x, int y) {
	auto x1 = x, y1 = y;
	auto w = picture_width;
	if(w > picture.width) {
		x = x + (picture_width - picture_width) / 2;
		w = picture.width;
	}
	auto h = picture_height;
	if(h > picture.height) {
		y = y + (picture.height - picture_height) / 2;
		h = picture.height;
	}
	if(true) {
		rect rc = {x, y, x + w, y + h};
		draw::state push; setclip(rc);
		blit(*canvas, x, y, w, h, 0, picture, picture.position.x, picture.position.y);
	}
	rectb({x1, y1, x1 + picture_width, y1 + picture_height}, colors::border);
}

static void render_background() {
	rect rc = {0, 0, getwidth(), getheight()};
	rectf(rc, colors::window);
}

static bool need_exclude(const char* text, const char** exclude) {
	for(auto pe = exclude; *pe; pe++) {
		if(strcmp(text, *pe) == 0)
			return true;
	}
	return false;
}

static void make_cash(agrw<picture_info>& source, const char* folder, const char** exclude, const char** exclude_ext) {
	char temp[260];
	char url_folder[260]; szprint(url_folder, zendof(url_folder), "art/%1", folder);
	for(auto file = io::file::find(url_folder); file; file.next()) {
		auto pn = file.name();
		if(pn[0] == '.')
			continue;
		auto ext = szext(pn);
		if(!ext) {
			if(exclude && need_exclude(pn, exclude))
				continue;
			if(folder[0])
				szprint(temp, zendof(temp), "%1/%2", folder, pn);
			else
				szprint(temp, zendof(temp), pn);
			make_cash(source, temp, exclude, exclude_ext);
		} else {
			if(exclude_ext && need_exclude(ext, exclude_ext))
				continue;
			auto pi = source.add();
			szfnamewe(temp, pn);
			memset(pi, 0, sizeof(pi));
			pi->folder = szdup(folder);
			pi->id = szdup(temp);
		}
	}
}

static void header(int x, int y, int width, const char* title, const char* text) {
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	y += textf(x, y, width, title) + metrics::padding;
	fore = colors::text;
	font = metrics::font;
	y += textf(x, y, width, text);
}

const picture_info* picture_info::choose_image() {
	struct string_view : controls::list {
		agrw<picture_info> source;
		int getmaximum() const override {
			return source.getcount();
		}
		const char*	getname(char* result, const char* result_max, int line, int column) const {
			if(source) {
				switch(column) {
				case 0: return source[line].id;
				case 1: return source[line].folder;
				default: return "";
				}
			}
			return "";
		}
		picture_info* getcurrent() const {
			if(!source)
				return 0;
			return source.get(current);
		}
		constexpr string_view(const agrw<picture_info>& source) : source(source) {}
	};
	if(!file_data)
		make_cash(file_data, "", file_exclude, file_ext_exclude);
	string_view s1(file_data);
	setfocus(0, true);
	while(ismodal()) {
		render_background();
		auto current_picture = s1.getcurrent();
		auto x = metrics::padding, y = metrics::padding;
		s1.view({x + picture_width + metrics::padding, y, getwidth() - metrics::padding, y_buttons - metrics::padding});
		if(current_picture) {
			picture.load(*current_picture);
			picture.position = current_picture->position;
			render_picture(x, y);
		}
		y += picture_height + metrics::padding;
		header(x, y, picture_width, "Выбирайте картинку",
			"Используйте [Ctrl] и клавиши движения чтобы перемещать отображаемую область картинки, если она превышает размер окна в [300] на [300] точек.");
		y = y_buttons;
		x += button(x, y, "Выбрать", cmd(buttonok), KeyEnter);
		x += button(x, y, "Отмена", cmd(buttoncancel), KeyEscape);
		domodal();
		if(current_picture) {
			switch(hot.key) {
			case Ctrl | KeyLeft: current_picture->position.x--; break;
			case Ctrl | KeyRight: current_picture->position.x++; break;
			case Ctrl | KeyUp: current_picture->position.y--; break;
			case Ctrl | KeyDown: current_picture->position.y++; break;
			default: break;
			}
			if(current_picture->position.x + picture_width > picture.width)
				current_picture->position.x = picture.width - picture_width;
			if(current_picture->position.y + picture_height > picture.height)
				current_picture->position.y = picture.height - picture_height;
			if(current_picture->position.x < 0)
				current_picture->position.x = 0;
			if(current_picture->position.y < 0)
				current_picture->position.y = 0;
		}
	}
	if(getresult())
		return s1.getcurrent();
	return 0;
}

static int monster_part = 0;
static void change_monster_part() {
	if(monster_part >= 1)
		monster_part = 0;
	else
		monster_part++;
}

static void change_position(sprite::frame* frame, int ox, int oy) {
	if(!frame)
		return;
	frame->ox += ox;
	frame->oy += oy;
}
static void change_left() {
	change_position((sprite::frame*)hot.param, 1, 0);
}
static void change_right() {
	change_position((sprite::frame*)hot.param, -1, 0);
}
static void change_up() {
	change_position((sprite::frame*)hot.param, 0, 1);
}
static void change_down() {
	change_position((sprite::frame*)hot.param, 0, -1);
}

static int show_information(int x, int y, int width, const sprite* ps, const sprite::frame* pf) {
	if(!pf)
		return 0;
	auto index = (pf - ps->frames) / 2;
	auto pi = (sprite_name_info*)ps->edata() + index;
	char temp[2048]; temp[0] = 0;
	stringcreator sc(temp);
	sc.add("#%1", pi->name);
	sc.addn("Ширина [%1i], высота [%2i]", pf->sx, pf->sy);
	sc.addn("Сдвиг влево [%1i], вправо [%2i]", pf->ox, pf->oy);
	return textf(x, y, width, temp) + metrics::padding;
}

void picture_info::edit_monsters() {
	struct string_view : controls::list {
		aref<sprite_name_info> source;
		int getmaximum() const override { return source.count; }
		const char*	getname(char* result, const char* result_max, int line, int column) const {
			if(source.data) {
				if(column == 0)
					return source.data[line].name;
			}
			return "";
		}
		constexpr string_view(const aref<sprite_name_info>& source) : source(source) {}
	};
	if(!spr_monsters)
		return;
	unsigned index_maximum = spr_monsters->count / 2;
	string_view s1(aref<sprite_name_info>((sprite_name_info*)spr_monsters->edata(), index_maximum));
	setfocus(0, true);
	while(ismodal()) {
		render_background();
		auto x = metrics::padding, y = metrics::padding;
		auto index = s1.current;
		sprite::frame* current_frame = 0;
		if(spr_monsters) {
			rectb({x, y, x + picture_width, y + picture_height}, colors::border);
			auto x1 = x + picture_width / 2;
			auto y1 = y + picture_height / 2;
			rectb({x1 - combat_grid / 2, y1 - combat_grid / 2, x1 + combat_grid / 2, y1 + combat_grid / 2}, colors::white);
			if(index != -1) {
				auto frame = index * 2 + monster_part;
				image(x1, y1, spr_monsters, frame, 0);
				current_frame = (sprite::frame*)spr_monsters->frames + frame;
			}
		}
		y += picture_height + metrics::padding;
		rectb({x, y, x + picture_width, y_buttons - metrics::padding * 2}, colors::border);
		y += metrics::padding;
		y += show_information(x + metrics::padding, y, picture_width - metrics::padding, spr_monsters, current_frame);
		s1.view({x + picture_width + metrics::padding, metrics::padding, getwidth() - metrics::padding, y_buttons - metrics::padding * 2});
		y = y_buttons;
		x += button(x, y, "Готово", cmd(save_monsters, (int)spr_monsters), KeyEnter);
		x += button(x, y, "Сменить", cmd(change_monster_part), KeySpace);
		x += button(x, y, "Вверх", cmd(change_up, (int)current_frame), Ctrl + KeyUp);
		x += button(x, y, "Вниз", cmd(change_down, (int)current_frame), Ctrl + KeyDown);
		x += button(x, y, "Вправо", cmd(change_right, (int)current_frame), Ctrl + KeyRight);
		x += button(x, y, "Влево", cmd(change_left, (int)current_frame), Ctrl + KeyLeft);
		if(hot.key == KeyEscape)
			execute(save_monsters, (int)spr_monsters);
		domodal();
	}
}

static void render_title(int x, int y, int width, char* temp, const char* temp_end, const char* title) {
	szprint(temp, temp_end, "%1:", title);
	textf(x, y, width - metrics::padding, temp);
}

static void render_title(int x, int y, int width, const char* title) {
	char temp[260];
	render_title(x, y, width, temp, zendof(temp), title);
}

static void button_handle(bool& result) {
	switch(hot.key) {
	case KeyEnter:
	case F2:
		result = true;
		break;
	}
}

static int button(int x, int y, int width, callback proc, void* param, const char* string, unsigned flags = AlignCenterCenter, unsigned key = 0) {
	rect rc = {x, y, x + width, y + texth() + 4 * 2};
	focusing((int)param, flags, rc);
	auto focused = isfocused(flags);
	auto result = false;
	if(draw::buttonh(rc, false, focused, false, true, string, key, false))
		result = true;
	if(focused) {
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
		button_handle(focused);
	}
	if(result)
		execute(proc, (int)param);
	return rc.height() + metrics::padding;
}

static void choose_picture() {
	auto p = (picture_info*)hot.param;
	auto pi = p->choose_image();
	if(pi)
		*p = *pi;
}

static int field(int x, int y, int width, const char* title, picture_info& v) {
	char temp[260];
	setposition(x, y, width);
	titletext(x, y, width, 0, title, title_width);
	if(v)
		v.geturl(temp);
	else
		szprint(temp, zendof(temp), "Не меняется");
	return button(x, y, width, choose_picture, &v, temp, AlignLeftCenter);
}

static int get_field_width(int digits) {
	return 12 * (digits + 1) + 4 * 2 + 16;;
}

static int field(int x, int y, const char* title, const anyval& v, int digits) {
	return draw::field(x, y, title_width + get_field_width(digits), title, v, title_width, digits);
}

static int field(int x, int y, const char* t1, const anyval& v1, int digits, const char* t2, const anyval& v2) {
	auto w = get_field_width(digits);
	auto d = draw::field(x, y, title_width + w, t1, v1, title_width, digits);
	x += title_width + w - 4;
	auto tw = textw(t2);
	draw::field(x, y, tw + w, t2, v2, tw, digits);
	return d;
}

static int field(int x, int y, const char* t1, const anyval& v1, int digits, const char* t2) {
	auto w = get_field_width(digits);
	auto d = draw::field(x, y, title_width + w, t1, v1, title_width, digits);
	x += title_width + w - 4;
	auto tw = textw(t2);
	text(x + 2 + metrics::padding, y + metrics::padding + 4, t2);
	return d;
}

static int field(int x, int y, const char* t1, const anyval& v1, int digits, const char* t2, const anyval& v2, const char* t3, const anyval& v3) {
	int tw;
	auto w = get_field_width(digits);
	auto d = draw::field(x, y, title_width + w, t1, v1, title_width, digits);
	x += title_width + w - 4;
	tw = textw(t2);
	draw::field(x, y, tw + w, t2, v2, tw, digits);
	x += tw + w - 4;
	tw = textw(t3);
	draw::field(x, y, tw + w, t3, v3, tw, digits);
	return d;
}

static int field(int x, int y, int width, const char* title, controls::textedit& v) {
	setposition(x, y, width);
	rect rc = {x, y, x + width, y + texth() * 4 + 4 * 2};
	v.view(rc);
	return rc.y2 - y + metrics::padding;
}

static void choose_enum() {
	struct enum_view : controls::list, adat<int, 256> {
		const bsdata&	source;
		const char*	getname(char* result, const char* result_max, int line, int column) const {
			switch(column) {
			case 0: return ((name_info*)source.get(data[line]))->name;
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
	auto events = command_draw_events;
	enum_view ev(*command_data);
	ev.hilite_odd_lines = false;
	auto i1 = 0;
	auto i2 = command_data->count - 1;
	if(command_range[1]) {
		i1 = command_range[0];
		i2 = command_range[1];
	}
	for(unsigned i = i1; i < i2; i++) {
		if(events) {
			if(!events->isallow(*command_data, i))
				continue;
		}
		ev.add(i);
	}
	auto mc = imin(12, ev.getmaximum());
	auto rc = command_rect;
	auto ci = ev.indexof(command_value);
	if(ci != -1)
		ev.current = ci;
	rc.y1 = rc.y2;
	rc.y2 = rc.y1 + ev.getrowheight() * mc + 1;
	rectf(rc, colors::form);
	if(draw::dropdown(rc, ev, true))
		command_value = ev.getcurrent();
}

static int field(int x, int y, int width, const char* title, const anyval& ev, bsdata& ei, draw_events* pev, int i1, int i2) {
	setposition(x, y, width);
	titletext(x, y, width, 0, title, title_width);
	rect rc = {x, y, x + width, y + texth() + 4 * 2};
	unsigned flags = 0;
	focusing((int)ev.ptr(), flags, rc);
	auto focused = isfocused(flags);
	auto result = focused && (hot.key == KeyEnter || hot.key == F2);
	if(buttonh(rc, ischecked(flags), focused, isdisabled(flags), true, 0, 0, false, 0))
		result = true;
	textc(rc.x1 + 4, rc.y1 + 4, rc.width() - 4 * 2, ((name_info*)ei.get(ev))->name);
	if(focused)
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
	if(result) {
		command_value = ev;
		command_rect = rc;
		command_data = &ei;
		command_draw_events = pev;
		command_range[0] = i1;
		command_range[1] = i2;
		execute(choose_enum);
	}
	return rc.height() + metrics::padding * 2;
}

template<class T> static int field(int x, int y, int width, const char* title, T& ev, draw_events* pev = 0) {
	return field(x, y, width, title, ev, bsmeta<bsreq::btype<T>::value>::data, pev, 0, 0);
}

static void choose_avatar() {
	auto p = (short unsigned*)hot.param;
	if(!p)
		return;
	auto old_focus = getfocus();
	picture_info::choose(*p, "Какую картинку использовать?", command_mask, 64);
	setfocus(old_focus, true);
}

static int fieldv(int x, int y, int width, const char* title, const char* value) {
	char temp[260]; zprint(temp, "%1:", title);
	x += metrics::padding;
	width -= metrics::padding * 2;
	text(x, y, temp);
	auto old_fore = fore;
	fore = fore.mix(colors::form, 192);
	text(x + width - textw(value), y, value);
	fore = old_fore;
	return texth() + 2;
}

static int fieldv(int x, int y, int width, const char* title, int value) {
	char temp[16]; zprint(temp, "%1i", value);
	return fieldv(x, y, width, title, temp);
}

static int fieldp(int x, int y, int width, const char* title, int value) {
	char temp[16]; zprint(temp, "%1i%%", value);
	return fieldv(x, y, width, title, temp);
}

static int fieldv(int x, int y, int width, const char* title, int value, int value_maximum, const char* format = "%1i/%2i") {
	char temp[32]; zprint(temp, format, value, value_maximum);
	return fieldv(x, y, width, title, temp);
}

static void page_header(int& x, int& y, const char* title, bool padding = true) {
	x = metrics::padding;
	y = metrics::padding;
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	text(x, y, title);
	y += texth() + metrics::padding;
}

static void page_footer(int& x, int& y, bool allow_cancel = false) {
	x = metrics::padding;
	y = y_buttons;
	if(allow_cancel) {
		x += button(x, y, "OK", cmd(buttonok), KeyEnter);
		x += button(x, y, "Отмена", cmd(buttoncancel), KeyEscape);
	} else
		x += button(x, y, "Готово", cmd(buttonok), KeyEscape);
}

static void set_page() {
	command_value = (const int)hot.param;
	setfocus((int)command_value.ptr(), true);
}

static int page_tabs(int x, int y, const char** source, int& current_page) {
	if(!source)
		return 0;
	x += 2;
	auto focused = false;
	auto id = (int)&current_page;
	if(!getfocus())
		setfocus(id, true);
	if(getfocus() == id)
		focused = true;
	auto x0 = x;
	for(unsigned i = 0; source[i]; i++) {
		auto string = source[i];
		auto dx = textw(string);
		rect rc = {x, y, x + dx + metrics::padding * 2, y + texth() + metrics::padding * 2};
		auto checked = (current_page == i);
		if(draw::buttonh(rc, checked, (focused && checked), false, true, string, Ctrl + (Alpha + '1' + i), false)) {
			command_value = anyval(current_page);
			execute(set_page, i);
		}
		if(focused && checked)
			rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
		if(focused) {
			switch(hot.key) {
			case KeyLeft:
				if(current_page > 0) {
					command_value = anyval(current_page);
					execute(set_page, current_page - 1);
				}
				break;
			case KeyRight:
				if(source[current_page + 1] != 0) {
					command_value = anyval(current_page);
					execute(set_page, current_page + 1);
				}
				break;
			}
		}
		x += rc.width();
	}
	addelement(id, {x0, y, x, y + texth() + metrics::padding * 2});
	return x - x0;
}

static int small_header(int x, int y, int width, const char* title) {
	if(!title)
		return 0;
	draw::state push;
	font = metrics::h3;
	fore = colors::yellow.mix(colors::border);
	text(x, y, title);
	return texth() + metrics::padding;
}

static rect start_group_light(int x, int& y, int width, const char* title, unsigned flags = AlignLeft) {
	auto old_font = font;
	auto old_fore = fore;
	font = metrics::h3;
	fore = colors::h3;
	rect rc = {x, y, x + width, y + texth() + 1};
	text(rc, title, flags);
	font = old_font;
	fore = old_fore;
	y = rc.y2 + metrics::padding * 2;
	return rc;
}

static int close_group_light(int x, int y, const rect& rc) {
	return metrics::padding;
}

static rect start_group(int x, int& y, int width, const char* title, unsigned flags = AlignCenterCenter) {
	rect rc = {x - metrics::padding, y, x + width + metrics::padding, y + texth() + 4 * 2};
	gradv(rc, colors::border, colors::edit);
	text(rc, title, flags);
	y = rc.y2 + metrics::padding * 2;
	return rc;
}

static int close_group(int x, int y, const rect& rc) {
	rectb({rc.x1, rc.y1, rc.x2, y + metrics::padding}, colors::border);
	return metrics::padding * 2;
}

static int group(int x, int y, int width, const char* title, bsdata& source, const anyval& value) {
	struct cmd : runable {
		constexpr cmd(const anyval& value, const name_info* source, unsigned index)
			: source(source), index(index), value(value) {}
		virtual int	getid() const { return (int)source; }
		virtual void execute() const { command_value = value; draw::execute(set_command_value, index); }
		virtual bool isdisabled() const { return false; }
		bool ischecked() const { return (int)value == index; }
	private:
		const name_info*	source;
		unsigned			index;
		const anyval		value;
	};
	auto y1 = y;
	if(!source.count)
		return 0;
	auto rc = start_group(x, y, width, title);
	for(unsigned i = 0; i <= source.count; i++) {
		auto p = (name_info*)source.get(i);
		cmd ev(value, p, i);
		unsigned flags = 0;
		if(ev.ischecked())
			flags |= Checked;
		y += radio(x, y, width, flags, ev, p->name, 0);
	}
	y += close_group(x, y, rc);
	return y - y1 + metrics::padding;
}

template<class T> static int group(int x, int y, int width, const char* title, T& value) {
	return group(x, y, width, title, bsmeta<bsreq::btype<T>::value>::data, value);
}

static void check_flags() {
	unsigned v1 = command_value;
	unsigned v2 = 1 << hot.param;
	if(v1&v2)
		v1 &= ~v2;
	else
		v1 |= v2;
	command_value = (const int)v1;
}

static int checkboxes(int x, int y, int width, const char* title, const anyval& value, const bsdata& ei, draw_events* pev = 0) {
	struct cmd : runable {
		constexpr cmd(const anyval& value, const name_info* source, unsigned index)
			: source(source), index(index), value(value) {}
		virtual int	getid() const { return (int)source; }
		virtual void execute() const {
			command_value = value;
			draw::execute(check_flags, index);
		}
		virtual bool isdisabled() const { return false; }
		bool ischecked() const { return (((int)value) & (1 << index)) != 0; }
	private:
		const name_info*	source;
		unsigned			index;
		const anyval		value;
	};
	auto y1 = y;
	if(ei.count == 0)
		return 0;
	auto rc = start_group(x, y, width, title);
	for(unsigned i = 0; i < ei.count; i++) {
		if(pev) {
			if(!pev->isallow(ei, i))
				continue;
		}
		auto p = (name_info*)ei.get(i);
		cmd ev(value, p, i);
		unsigned flags = 0;
		if(ev.ischecked())
			flags |= Checked;
		y += checkbox(x, y, width, flags, ev, p->name, 0);
	}
	y += metrics::padding;
	y += close_group(x, y, rc);
	return y - y1 /*+ metrics::padding*/;
}

static int field_picture(int x, int y, int width, int height, short unsigned& ev, const char* title, const char* mask) {
	auto y0 = y;
	unsigned flags = 0;
	rect rgo = start_group(x, y, width, title);
	rect rc = {x, y, x + width, y + height};
	focusing((int)&ev, flags, rc);
	auto focused = isfocused(flags);
	auto a = area(rc);
	if(a == AreaHilited || a == AreaHilitedPressed)
		rectf(rc, colors::edit);
	rectb(rc, colors::border);
	if(spr_monsters) {
		auto dx = width / 4;
		auto y1 = y + (height / 4) * 3;
		image(x + dx * 1, y1, spr_monsters, ev * 2 + 0, 0);
		image(x + dx * 3, y1, spr_monsters, ev * 2 + 1, 0);
	}
	auto result = false;
	if(focused) {
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
		button_handle(result);
	}
	if(a == AreaHilited || a == AreaHilitedPressed) {
		if(hot.key == MouseLeft && !hot.pressed)
			result = true;
	}
	if(result) {
		command_mask[0] = 0;
		if(mask)
			zcpy(command_mask, mask);
		execute(choose_avatar, (int)&ev);
	}
	y += rc.height();
	y += close_group(x, y, rgo);
	return y - y0;
}

void event_info::edit() {
	char ti1[4096]; controls::textedit te1(ti1, sizeof(ti1), false); ti1[0] = 0;
	int width = getwidth() - metrics::padding * 2;
	int x, y;
	setfocus(0, true);
	while(ismodal()) {
		render_background();
		page_header(x, y, "Боевая сцена");
		y += field(x, y, width, "Картинка", picture);
		y += field(x, y, width, "Текст, который увидят игроки", te1);
		page_footer(x, y, true);
		domodal();
	}
}

struct page_view {
	int	page_current, page_maximum;
	constexpr page_view(int page_maximum) : page_current(0), page_maximum(page_maximum) {}
	virtual const char*	getheader() const { return "Нет названия"; }
	virtual int	 getmaximum() const { return page_maximum; }
	virtual const command* getcommands() { return 0; }
	virtual bool nextpage(bool run) {
		if(page_current >= getmaximum() - 1)
			return false;
		if(run)
			page_current++;
		return true;
	}
	virtual bool prevpage(bool run) {
		if(page_current < 1)
			return false;
		if(run)
			page_current--;
		return true;
	}
	virtual void clear() {}
	virtual void redraw(int x, int y, int width) {}
	static void next_page() {
		auto p = (page_view*)hot.param;
		p->nextpage(true);
	}
	static void prev_page() {
		auto p = (page_view*)hot.param;
		p->prevpage(true);
	}
	int view() {
		setfocus(0, true);
		clear();
		while(ismodal()) {
			render_background();
			auto y = getheight() - texth() - metrics::padding * 3;
			auto x = metrics::padding * 2;
			x += button(x, y, "Готово", cmd(buttonok), KeyEnter);
			x += button(x, y, "Далее", cmd(next_page, (int)this, !nextpage(false)), Alpha + 'N');
			x += button(x, y, "Назад", cmd(prev_page, (int)this, !prevpage(false)), Alpha + 'P');
			x += button(x, y, "Отмена", cmd(buttoncancel), KeyEscape);
			for(auto pc = getcommands(); pc && *pc; pc++)
				x += button(x, y, pc->name, cmd(pc->proc, (int)this), pc->key);
			y = metrics::padding * 2;
			x = metrics::padding * 2;
			page_header(x, y, getheader());
			redraw(x, y, getwidth() - x * 2);
			domodal();
		}
		return getresult();
	}
};

static int group_combat_ability(int x, int y, int width, const character* player, bool range_hits) {
	char temp[260];
	auto& col = bsmeta<class_s>::data;
	attack_info ai = {}; player->get(MeleeWeapon, ai);
	auto y1 = y;
	auto rga = start_group(x, y, width, "Боевые параметры");
	y += fieldv(x, y, width, "Количество атак", ai.getattacks(temp, zendof(temp)));
	y += fieldv(x, y, width, "THAC0", ai.thac0);
	y += fieldv(x, y, width, "Урон", ai.damage.print(temp, zendof(temp)));
	y += fieldv(x, y, width, "Класс брони", player->getac());
	if(range_hits) {
		auto type = player->getclass();
		auto rolled = 0;
		for(unsigned i = 0; i < col[type].classes.count; i++)
			rolled += col[col[type].classes[i]].hd * player->getlevel(i);
		auto min = player->gethpmax(0);
		auto max = player->gethpmax(rolled);
		y += fieldv(x, y, width, "Хиты", min, max, "%1i-%2i");
	} else
		y += fieldv(x, y, width, "Хиты", player->gethp(), player->gethpmax());
	y += close_group(x, y, rga);
	return y - y1;
}

static int group_ability(int x, int y, int width, const character* player) {
	auto y1 = y;
	auto rga = start_group(x, y, width, "Атрибуты");
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		auto v = player->get(i);
		if(i == Strenght && v == 18)
			y += fieldv(x, y, width, getstr(i), v, player->getstrper(), "%1i/%2.2i");
		else
			y += fieldv(x, y, width, getstr(i), v);
	}
	y += metrics::padding;
	y += close_group(x, y, rga);
	return y - y1;
}

void character::group_generate(int x, int y, int width) {
	const int w1 = 200;
	auto y1 = y;
	y += group(x, y, w1, "Пол", gender);
	y += group(x, y, w1, "Раса", race);
	x += w1 + metrics::padding * 3;
	y = y1;
	y += group(x, y, w1, "Класс", type);
	x += w1 + metrics::padding * 3;
	y = y1;
	y += group(x, y, w1, "Мировозрение", alignment);
}

struct character_view : page_view {
	character*	player;
	const char* getheader() const {
		return "Создание персонажа";
	}
	void reroll() {
		player->create(player->race, player->gender, player->type, player->alignment, Player);
	}
	void clear() override {
		player->clear();
		player->race = (race_s)xrand(Human, Halfling);
		player->gender = (gender_s)xrand(Male, Female);
		player->alignment = (alignment_s)xrand(LawfulGood, ChaoticEvil);
		reroll();
	}
	static void reroll_command() {
		auto p = (character_view*)hot.param;
		p->reroll();
	}
	virtual const command* getcommands() {
		static command information_command[] = {{"Перебросить", reroll_command, Alpha + 'R'},
		{}};
		switch(page_current) {
		case 1: return information_command;
		default: return 0;
		}
	}
	int group_basic(int x, int y, int w1, int w2) {
		char temp[260];
		auto y1 = y;
		auto width = w1 + w2 + metrics::padding * 4;
		auto old_stroke = fore_stroke;
		fore_stroke = colors::black;
		auto rga = start_group(x, y, width, "Павелиус великий", AlignCenterCenter | TextBold);
		fore_stroke = old_stroke;
		auto y2 = y;
		zprint(temp, "%1 %-2", getstr(player->gender), getstr(player->race));
		text(x, y, temp); y += texth();
		text(x, y, getstr(player->alignment)); y += texth();
		y = y2;
		auto x1 = x + w1 + metrics::padding * 4;
		auto& col = bsmeta<class_s>::data;
		auto class_count = col[player->type].classes.count;
		if(!class_count)
			class_count = 1;
		for(unsigned i = 0; i < col[player->type].classes.count; i++) {
			auto e = col[player->type].classes.data[i];
			text(x1, y, getstr(e));
			auto exp = player->experience / class_count;
			zprint(temp, "%1i уровень (опыт %2i)", player->levels[i], exp);
			text(x1 + w2 - textw(temp), y, temp);
			y += texth();
		}
		y2 += texth() * 3;
		y2 += close_group(x, y2, rga);
		return y2 - y1;
	}
	int group_skills(int x, int y, int width) {
		auto y1 = y;
		auto rga = start_group(x, y, width, "Навыки");
		for(auto i = FirstSave; i <= LastSkill; i = (skill_s)(i + 1)) {
			auto v = player->get(i);
			if(v <= 0)
				continue;
			y += fieldp(x, y, width, getstr(i), v);
		}
		y += close_group(x, y, rga);
		return y - y1;
	}
	void information(int x, int y, int width) {
		auto w1 = 160;
		auto w2 = 250;
		auto y1 = y;
		y += group_basic(x, y, w1, w2) + metrics::padding;
		auto y2 = y;
		y += group_ability(x, y, w1, player) + metrics::padding;
		y += group_combat_ability(x, y, w1, player, false) + metrics::padding;
		y = y2;
		x += w1 + metrics::padding * 4;
		y += group_skills(x, y, w2) + metrics::padding;
	}
	bool nextpage(bool run) override {
		if(!page_view::nextpage(run))
			return false;
		if(run) {
			switch(page_current) {
			case 1: reroll(); break;
			}
		}
		return true;
	}
	void redraw(int x, int y, int width) override {
		switch(page_current) {
		case 0: player->group_generate(x, y, width); break;
		case 1: information(x, y, width); break;
		}
	}
	constexpr character_view(character* player) : page_view(2), player(player) {}
};

bool character::generate() {
	character_view ev(this);
	if(!ev.view())
		return false;
	return picture_info::choose(avatar, "Как выглядит ваш герой?", "character*", 64);
}

struct image_info : point {
	const sprite*			ps;
	short unsigned			frame, flags;
	unsigned char			alpha;
	void clear() { memset(this, 0, sizeof(*this)); }
};
static adat<image_info, 1024> scene_images;

static void draw_grid(int x0, int y0) {
	color c = colors::form;
	for(auto y = 0; y <= combat_map_y; y++) {
		auto y1 = y0 + y * combat_grid;
		auto x2 = x0 + combat_map_x * combat_grid;
		line(x0, y1, x2, y1, c);
	}
	for(auto x = 0; x <= combat_map_x; x++) {
		auto x1 = x0 + x * combat_grid;
		auto y2 = y0 + combat_map_y * combat_grid;
		line(x1, y0, x1, y2, c);
	}
}

static void draw_active_player(int x0, int y0) {
	auto player = character::getactive();
	if(!player)
		return;
	auto i = player->getposition();
	if(i == mapcore::Blocked)
		return;
	auto x = i % combat_map_x;
	auto y = i / combat_map_x;
	auto x1 = x0 + x * combat_grid;
	auto y1 = y0 + y * combat_grid;
	rectb({x1 + 1, y1 + 1, x1 + combat_grid - 1, y1 + combat_grid - 1}, colors::white);
}

static void draw_cost(int x0, int y0) {
	auto old_fore = fore;
	fore = colors::form;
	for(auto y = 0; y < combat_map_y; y++) {
		for(auto x = 0; x < combat_map_x; x++) {
			auto i = combat_info::m2i(x, y);
			auto c = combat_info::getcost(i);
			if(!c)
				continue;
			auto x1 = x0 + x * combat_grid + combat_grid / 2;
			auto y1 = y0 + y * combat_grid + combat_grid / 2;
			if(c == mapcore::Blocked) {
				line(x1 - combat_grid / 2, y1 - combat_grid / 2, x1 + combat_grid / 2, y1 + combat_grid / 2);
				line(x1 - combat_grid / 2, y1 + combat_grid / 2, x1 + combat_grid / 2, y1 - combat_grid / 2);
			} else {
				char temp[16]; zprint(temp, "%1i", c);
				text(x1 - textw(temp) / 2, y1 - texth() / 2, temp);
			}
		}
	}
	fore = old_fore;
}

void character::addbattle() {
	if(!isalive())
		return;
	auto index = getposition();
	if(index == mapcore::Blocked)
		return;
	auto x = (index%combat_map_x)*combat_grid + combat_grid / 2;
	auto y = (index / combat_map_y)*combat_grid + combat_grid / 2;
	auto pi = scene_images.add();
	pi->frame = getavatar() * 2;
	pi->flags = 0;
	pi->alpha = 0xFF;
	pi->ps = spr_monsters;
	pi->x = x;
	pi->y = y;
	if(direction == Right)
		pi->flags |= ImageMirrorH;
}

void combat_info::update() {
	scene_images.clear();
	for(auto p : parcipants)
		p->addbattle();
}

static combat_info* current_combat;

static void move_direction() {
	auto d = (direction_s)hot.param;
	auto p = character::getactive();
	if(!p)
		return;
	//p->move(d);
	current_combat->movement -= combat_moverate;
}

static void set_defend() {
	auto p = character::getactive();
	if(!p)
		return;
	current_combat->movement = 0;
}

static void draw_images(int x, int y) {
	for(auto& e : scene_images)
		image(x + e.x, y + e.y, e.ps, e.frame, e.flags, e.alpha);
}

void combat_info::visualize() {
	render_background();
	auto x = metrics::padding, y = y_buttons - metrics::padding * 2 - combat_grid * combat_map_y;
	update();
	draw_grid(x, y);
	draw_active_player(x, y);
	draw_images(x, y);
}

void combat_info::move(character* player) {
	current_combat = this;
	player->setactive();
	setfocus(0, true);
	movement = player->getmovement();
	makewave(player->getposition());
	while(ismodal() && movement > 0) {
		render_background();
		auto x = metrics::padding, y = y_buttons - metrics::padding * 2 - combat_grid * combat_map_y;
		// Нарисуем все объекты, которые просчитали ранее
		update();
		draw_grid(x, y);
		draw_cost(x, y);
		draw_active_player(x, y);
		draw_images(x, y);
		y = y_buttons;
		x += button(x, y, "Атаковать", cmd(buttonok), Alpha + 'A');
		x += button(x, y, "Стрелять", cmd(buttonok), Alpha + 'S');
		x += button(x, y, "Метать", cmd(buttonok), Alpha + 'T');
		x += button(x, y, "Заклинание", cmd(buttonok), Alpha + 'C');
		x += button(x, y, "Вверх", cmd(move_direction, Up), KeyUp);
		x += button(x, y, "Вниз", cmd(move_direction, Down), KeyDown);
		x += button(x, y, "Вправо", cmd(move_direction, Right), KeyRight);
		x += button(x, y, "Влево", cmd(move_direction, Left), KeyLeft);
		x += button(x, y, "Защита", cmd(set_defend), KeySpace);
		domodal();
	}
}

bool picture_info::choose(short unsigned& result, const char* title, const char* mask, int size) {
	struct avatar_view : controls::picker {
		adat<short unsigned, 512> elements;
		int getmaximum() const override { return elements.count; }
		void row(const rect& rc, int index) override {
			rowhilite(rc, index);
			auto avatar = elements[index];
			image(rc.x1 + rc.width() / 2, rc.y2 - rc.height() / 4, spr_monsters, avatar * 2, 0);
		}
		int getcurrent() const {
			if(!elements.count)
				return -1;
			return elements.data[current];
		}
	};
	avatar_view s1;
	s1.pixels_per_line = 2 * 2 + 64;
	s1.pixels_per_column = 2 * 2 + 64;
	s1.show_grid_lines = true;
	s1.show_border = true;
	s1.elements.count = select(s1.elements.data, s1.elements.getmaximum(), mask);
	s1.current = s1.elements.indexof(result);
	s1.ensurevisible();
	setfocus(0, true);
	int x, y;
	while(ismodal()) {
		render_background();
		page_header(x, y, title, false);
		s1.view({x, y, getwidth() - metrics::padding, y_buttons - metrics::padding * 2});
		page_footer(x, y, true);
		x += button(x, y, "Редактировать", cmd(picture_info::edit_monsters), Ctrl + Alpha + 'E');
		domodal();
	}
	if(getresult() != 0) {
		result = s1.getcurrent();
		return true;
	}
	return false;
}

int character::edit_abilities(int x, int y, int width) {
	auto y0 = y;
	auto rga = start_group(x, y, width, "Атрибуты");
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		y += field(x, y, width, getstr(i), abilities[i], 100, 2);
	y += metrics::padding;
	y += close_group(x, y, rga);
	return y - y0;
}

int character::edit_basic(int x, int y, int width, draw_events* pev) {
	const int nw = 58;
	const int tw = 12;
	auto y0 = y;
	auto x0 = x;
	auto& col = bsmeta<class_s>::data;
	auto rga = start_group(x, y, width, "Базовые значения");
	y += field(x, y, width, "Имя", name, title_width);
	y += field(x, y, width, "Мировозрение", alignment, pev);
	y += field(x, y, width, "Раса", race, pev);
	y += field(x, y, width, "Пол", gender);
	y += field(x, y, width, "Размер", size);
	y += field(x, y, width, "Классы", type, pev);
	auto d = field(x, y, title_width + nw, "Уровень", levels[0], title_width, 2); x += title_width + nw;
	if(col[type].classes.count >= 2) {
		field(x, y, tw + nw, ":/", levels[1], tw, 2);
		x += tw + nw;
	}
	if(col[type].classes.count >= 3) {
		field(x, y, tw + nw, ":/", levels[2], tw, 2);
		x += tw + nw;
	}
	x = x0; y += d;
	y += field(x, y, title_width + nw * 2, "Опыт", experience, title_width, 8);
	d = field(x, y, title_width + nw, "Хиты", hp, title_width, 3); x += title_width + nw;
	field(x, y, tw + nw, ":/", hp_rolled, tw, 3);
	x = x0; y += d;
	y += field(x, y, title_width + nw, "Класс брони", base_ac, title_width, 2);
	if(!is(NoExeptionalStrenght))
		y += field(x, y, title_width + nw, "Сила (%)", strenght_percent, title_width, 3);
	y += field(x, y, title_width + nw, "Движение", movement, title_width, 2);
	y += metrics::padding;
	y += close_group(x, y, rga);
	return y - y0;
}

static void choose_attack() {
	auto p = (special_info*)hot.param;
	auto e = *p;
	auto old_focus = getfocus();
	if(e.edit())
		*p = e;
	setfocus(old_focus, true);
}

static void choose_item() {
	auto p = (item_info*)hot.param;
	auto e = *p;
	auto old_focus = getfocus();
	if(e.edit())
		*p = e;
	setfocus(old_focus, true);
}

int	character::edit_attacks(int x, int y, int width) {
	auto x0 = x, y0 = y;
	auto rga = start_group(x, y, width, "Специальные атаки");
	for(auto& e : special_attacks)
		y += button(x, y, width, choose_attack, &e, "Нет");
	y += close_group(x, y, rga);
	return y - y0;
}

int	character::edit_feats(int x, int y, int width) {
	auto x0 = x, y0 = y;
	y += checkboxes(x, y, width, "Расовые или класса", feats, bsmeta<feat_s>::data);
	return y - y0;
}

static void choose_spells() {}

static void choose_feats() {}

bool design_info::choose(const char* title, const anyval& result, int width, int height, bool choose_mode) {
	struct table : controls::picker {
		design_info& source;
		int getmaximum() const override { return source.source.count; }
		void rowhilite(const rect& rc, int index) const override {
			static color grade_colors[] = {colors::window,
				colors::window.mix(color::create(0, 121, 191), 192),
				colors::window.mix(color::create(97, 189, 79), 224),
				colors::window.mix(color::create(242, 214, 0), 192),
			};
			auto p = source.source.get(index);
			auto g = source.getgrade(p);
			auto f = grade_colors[g];
			if(g != Fair)
				rectf(rc, grade_colors[g]);
			if(show_selection) {
				area({rc.x1, rc.y1, rc.x2 - 1, rc.y2 - 1});
				if(index == current)
					hilight(rc);
				else if(index == current_hilite)
					rectf(rc, colors::edit.mix(f, 96));
			}
		}
		void row(const rect& rcorigin, int index) override {
			char temp[260]; temp[0] = 0;
			stringcreator sc(temp);
			auto pv = source.source.get(index);
			rect rc = rcorigin;
			rowhilite(rc, index);
			auto avatar = source.getavatar(pv);
			if(avatar != -1) {
				rect rp = {rc.x1, rc.y1, rc.x1 + rc.height(), rc.y2};
				draw::state push;
				setclip(rp);
				image(rp.x1 + rp.width() / 2, rp.y2 - rp.height() / 4, spr_monsters, avatar * 2, 0);
				rc.x1 += rc.height() + 2;
			}
			auto p = source.getname(pv, sc, 0);
			rc.offset(4, 4);
			if(p) {
				draw::textc(rc.x1, rc.y1, rc.width(), p);
				rc.y1 += texth() + 2;
			}
			sc.clear();
			p = source.getname(pv, sc, 1);
			if(p) {
				auto old_fore = fore;
				fore = colors::text.mix(colors::form, 128);
				text(rc, p);
				fore = old_fore;
			}
		}
		int getcurrent() const {
			if(!source.source.count)
				return -1;
			return current;
		}
		void add() {
			source.edit(0, 0, true);
		}
		static void command_add() { ((table*)hot.param)->add(); }
		void copy() {
			source.edit(0, (void*)source.source.get(getcurrent()), true);
		}
		static void command_copy() { ((table*)hot.param)->copy(); }
		void change() {
			source.edit((void*)source.source.get(getcurrent()), 0, true);
		}
		static void command_change() { ((table*)hot.param)->change(); }
		constexpr table(design_info& source) : source(source) {}
	} e1(*this);
	e1.show_border = false;
	e1.pixels_per_line = height;
	e1.pixels_per_column = width;
	e1.show_grid_lines = true;
	setfocus(0, true);
	int x, y;
	while(ismodal()) {
		render_background();
		page_header(x, y, title, false);
		e1.view({x, y, getwidth() - x, y_buttons - metrics::padding * 2});
		page_footer(x, y, choose_mode);
		if(source.count < source.maximum && edit(0, 0, false)) {
			x += button(x, y, "Добавить", cmd(table::command_add, (int)&e1), F3);
			x += button(x, y, "Скопировать", cmd(table::command_copy, (int)&e1), F4);
		}
		if(source.count > 0) {
			auto p = source.get(e1.current);
			x += button(x, y, "Редактировать", cmd(table::command_change, (int)&e1), KeyEnter);
		}
		if(hot.key == F2)
			execute(table::command_change, (int)&e1);
		domodal();
	}
	if(getresult()) {
		result = e1.getcurrent();
		if(!choose_mode)
			save_campaign();
		return true;
	}
	return false;
}

static void character_reroll() {
	auto p = (character*)hot.param;
	p->reroll();
	p->set(UniqueCharacter);
}

bool character::edit() {
	struct character_events : draw_events {
		character& player;
		bool isallow(const bsdata& ei, int index) const override {
			if(&ei == &bsmeta<alignment_s>::data) {
				if(!player.isallow((alignment_s)index))
					return false;
			} else if(&ei == &bsmeta<class_s>::data) {
				if(!player.isallow((class_s)index))
					return false;
			}
			return true;
		}
		constexpr character_events(character& player) : player(player) {}
	} pev(*this);
	static const char* page_strings[] = {"Базовый", "Особенности", "Заклинания", 0};
	int x, y;
	setfocus(0, true);
	int page = 0;
	while(ismodal()) {
		render_background();
		page_header(x, y, "Монстр/Персонаж");
		if(page == 0) {
			auto y0 = y;
			auto c1 = 300, c2 = 160;
			y += edit_basic(x, y, c1, &pev);
			y = y0;
			x += c1 + metrics::padding * 3;
			y += edit_abilities(x, y, c2);
			y += group_combat_ability(x, y, c2, this, hp_rolled == 0);
			y = y0;
			x += c2 + metrics::padding * 3;
			auto c3 = getwidth() - x - metrics::padding * 2;
			y += field_picture(x, y, c3, 120, avatar, "Боевые картинки", 0);
			//y += edit_attacks(x, y, c3);
			auto rgo = start_group(x, y, c3, "Предметы");
			for(auto i = Head; i <= Legs; i = (wear_s)(i + 1))
				y += button(x, y, c3, choose_item, &wears[i], getstr(i));
			close_group(x, y, rgo);
		} else if(page == 1) {
			auto c1 = 300;
			y += edit_feats(x, y, c1);
		}
		page_footer(x, y, true);
		x += button(x, y, "Перебросить", cmd(character_reroll, (int)this), Ctrl + Alpha + 'R');
		x += page_tabs(x, y, page_strings, page);
		character previous = *this;
		domodal();
		changing(previous);
	}
	return getresult() != 0;
}

static int field(int x, int y, const char* title, dice& ev) {
	return field(x, y, title, ev.c, 2, ":d", ev.d, ":+", ev.b);
}

static int group(int x, int y, int width, damage_info& ev) {
	auto y0 = y;
	auto rgo = start_group(x, y, width, "Нанесение урона");
	y += field(x, y, width, "Тип", ev.type);
	y += field(x, y, "К-во атак", ev.attacks, 2);
	y += field(x, y, "Бонус THACO", ev.thac0, 2);
	y += field(x, y, "Дистанция", ev.range, 2);
	y += field(x, y, "Урон", ev.damage);
	y += field(x, y, "Урон (большим)", ev.damage_large);
	y += field(x, y, "Критический", ev.critical, 2, ":x", ev.multiplier);
	y += metrics::padding;
	y += close_group(x, y, rgo);
	return y - y0;
}

bool special_info::edit() {
	int x, y;
	openform();
	while(ismodal()) {
		render_background();
		page_header(x, y, "Специальная атака");
		auto y0 = y, c1 = 300;
		y += group(x, y, c1, *static_cast<damage_info*>(this));
		y += field(x, y, "Использовать", use_per_day, 2, "раз в день");
		page_footer(x, y, true);
		domodal();
	}
	closeform();
	return getresult() != 0;
}

static const markup* getmarkup(bsreq* type) {
}

bool design_info::edit(const char* name, void* object, const bsreq* type, const markup* elements) {
	int x, y;
	if(!elements)
		elements = plugin<markup>::get(type);
	if(!elements)
		return false;
	openform();
	while(ismodal()) {
		render_background();
		page_header(x, y, name);
		auto width = getwidth() - x * 2;
		y += draw::field(x, y, width, elements, bsval(object, type), 100);
		page_footer(x, y, true);
		domodal();
	}
	closeform();
	return getresult() != 0;
}

//bool item_info::edit() {
//	struct item_event : draw_events {
//		item_info& source;
//		bool isallow(const bsdata& e, int i) const override {
//			if(&e == &bsmeta<feat_s>::data) {
//				return bsmeta<feat_s>::data[i].use_item != 0;
//			}
//			return false;
//		}
//		constexpr item_event(item_info& v) : source(v) {}
//	} pev(*this);
//	int x, y;
//	openform();
//	while(ismodal()) {
//		render_background();
//		page_header(x, y, "Предмет");
//		auto y0 = y, c1 = 300;
//		auto rgo = start_group(x, y, c1, "Базовые значения");
//		y += field(x, y, c1, "Наименование", name, title_width);
//		y += field(x, y, c1, "Тип", type);
//		y += close_group(x, y, rgo);
//		y += checkboxes(x, y, c1, "Ограничение", restrictions, bsmeta<feat_s>::data, &pev);
//		//x += c1 + metrics::padding * 3;
//		//y = y0;
//		y += group(x, y, c1, damage);
//		page_footer(x, y, true);
//		domodal();
//	}
//	closeform();
//	return getresult() != 0;
//}