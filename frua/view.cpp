#include "crt.h"
#include "draw_control.h"
#include "io.h"
#include "main.h"

using namespace draw;

const int border = 4;
const int picture_width = 320;
const int picture_height = 300;
const int title_width = 100;
const int field_width = 400;
const int buttons_height = 16 + 8 * 2;

struct page_view;
struct command;

static anyval					command_value;
static agrw<picture_info>		file_data;
typedef adat<const char*, 256>	strings_array;

static void set_command_value() {
	command_value = (const int)hot.param;
}

struct command {
	const char*			name;
	unsigned			key;
	callback			proc;
};

static struct char_code_info {
	char ru, en;
} char_code[] = {{'�', 'F'}, {'�', ','}, {'�', 'D'},
{'�', 'U'}, {'�', 'L'}, {'�', 'T'},
{'�', ';'}, {'�', 'B'}, {'�', 'B'},
{'�', 'Q'}, {'�', 'R'}, {'�', 'K'},
{'�', 'V'}, {'�', 'Y'}, {'�', 'J'},
{'�', 'G'}, {'�', 'H'}, {'�', 'C'},
{'�', 'N'}, {'�', 'E'}, {'�', 'A'},
{'�', '['}, {'�', 'W'}, {'�', 'X'},
{'�', 'I'}, {'�', 'O'}, {'�', ']'},
{'�', 'S'}, {'�', 'M'}, {'�', '\''},
{'�', '.'}, {'�', 'Z'},
};
struct main_picture_info : surface, picture_info {
	bool load(const char* folder, const char* id) {
		if(!id || !id[0])
			return false;
		if(this->id
			&& strcmp(this->id, id) == 0
			&& strcmp(this->folder, folder) == 0)
			return true;
		this->id = szdup(id);
		this->folder = szdup(folder);
		if(*static_cast<surface*>(this))
			this->clear();
		char temp[260];
		szprint(temp, zendof(temp), "art/%1/%2.png", folder, id);
		this->read(temp);
		return true;
	}
} picture;

struct string_view : controls::list {
	strings_array& source;
	int getmaximum() const override {
		return source.count;
	}
	const char*	getname(char* result, const char* result_max, int line, int column) const {
		if(source.data) {
			if(column == 0)
				return source.data[line];
		}
		return "";
	}
	const char* getcurrent() const {
		if(source.count == 0)
			return 0;
		return source.data[current];
	}
	constexpr string_view(strings_array& source) : source(source) {}
};

struct cmdv : runable {
	cmdv(const anyval& source, int param = 0) : source(source), param(param) {}
	void					execute() const override {
		command_value = source;
		draw::execute(set_command_value, param);
	}
	int						getid() const override { return (int)source.getptr(); }
	bool					isdisabled() const { return false; }
private:
	const anyval&			source;
	int						param;
};

void view_initialize() {}

static void window(rect rc, int bw = border) {
	rc.offset(-border, -border);
	color c = colors::form;
	color b = colors::border;
	draw::rectf(rc, c);
	draw::rectb(rc, b);
}

static int window(int x, int y, int width, const char* string) {
	rect rc = {x, y, x + width, y};
	draw::state push;
	draw::font = metrics::font;
	auto height = textf(rc, string);
	window(rc);
	textf(rc.x1, rc.y1, rc.width(), string);
	return height + border * 2;
}

static int button(int x, int y, const char* string, const runable& ev) {
	auto id = ev.getid();
	auto dx = textw(string);
	rect rc = {x, y, x + dx + metrics::padding * 2, y + texth() + metrics::padding * 2};
	addelement(id, rc);
	if(draw::buttonh(rc, false, getfocus() == ev.getid(), ev.isdisabled(), true, string, 0, false))
		ev.execute();
	return rc.width() + 2;
}

static void render_picture(int x, int y) {
	auto w = 300;
	if(w > picture.width)
		w = picture.width;
	auto h = 300;
	if(h > picture.height)
		h = picture.height;
	blit(*canvas, x, y, w, h, 0, picture, picture.position.x, picture.position.y);
}

static void render_background() {
	rect rc = {0, 0, getwidth(), getheight()};
	rectf(rc, colors::window);
}

answer* character::choose(const picture_info& image, aref<answer> source) {
	picture.load("tavern", "tavern2");
	while(ismodal()) {
		render_background();
		render_picture(8, 8);
		auto x = 6, y = 572;
		//for(auto& e : source)
		//	x += button(x, y, e.name);
		domodal();
	}
	if(!getresult())
		return 0;
	return 0;
}

static void make_cash(agrw<picture_info>& source, const char* folder) {
	char temp[260];
	char url_folder[260]; szprint(url_folder, zendof(url_folder), "art/%1", folder);
	for(auto file = io::file::find(url_folder); file; file.next()) {
		auto pn = file.name();
		if(pn[0] == '.')
			continue;
		auto ext = szext(pn);
		if(!ext) {
			if(strcmp(pn, "fonts") == 0)
				continue;
			if(folder[0])
				szprint(temp, zendof(temp), "%1/%2", folder, pn);
			else
				szprint(temp, zendof(temp), pn);
			make_cash(source, temp);
		} else {
			auto pi = source.add();
			szfnamewe(temp, pn);
			memset(pi, 0, sizeof(pi));
			pi->folder = szdup(folder);
			pi->id = szdup(temp);
		}
	}
}

static void make_cash(strings_array& result, const agrw<picture_info>& source) {
	const char* v = 0;
	for(auto& e : source) {
		if(e.folder != v) {
			v = e.folder;
			auto pp = result.add();
			*pp = v;
		}
	}
}

static void header(int x, int y, const char* title, const char* text) {
	int width = getwidth() - x - metrics::padding;
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	y += textf(x, y, width, title) + metrics::padding;
	fore = colors::text;
	font = metrics::font;
	y += textf(x, y, width, text);
}

static picture_info* find(const picture_info& v) {
	for(auto& e : file_data) {
		if(e == v)
			return &e;
	}
	return 0;
}

static picture_info* find(const char* id, const char* folder) {
	for(auto& e : file_data) {
		if(strcmp(e.id, id) == 0 && strcmp(e.folder, folder) == 0)
			return &e;
	}
	return 0;
}

bool picture_info::pick() {
	strings_array folders;
	strings_array filter;
	if(!file_data)
		make_cash(file_data, "");
	make_cash(folders, file_data);
	string_view s1(folders);
	string_view s2(filter);
	const char* current_folder = 0;
	const char* current_id = 0;
	int y_buttons = getheight() - buttons_height;
	setfocus(0, true);
	while(ismodal()) {
		render_background();
		current_folder = s1.getcurrent();
		filter.clear();
		for(auto& e : file_data) {
			if(e.folder != current_folder)
				continue;
			filter.add(e.id);
		}
		s1.correction();
		s2.correction();
		current_id = s2.getcurrent();
		picture_info* current_picture = find(current_id, current_folder);
		auto x = metrics::padding, y = metrics::padding;
		if(current_picture) {
			picture.load(current_picture->folder, current_picture->id);
			if(current_picture->position.x + picture_width > picture.width)
				current_picture->position.x = picture.width - picture_width;
			if(current_picture->position.x < 0)
				current_picture->position.x = 0;
			if(current_picture->position.y + picture_height > picture.height)
				current_picture->position.y = picture.height - picture_height;
			if(current_picture->position.y < 0)
				current_picture->position.y = 0;
			rect rc = {x, y, x + picture_width, y + picture_height};
			if(true) {
				draw::state push;
				setclip(rc);
				blit(*canvas, x, y, picture_width, picture_height, 0,
					picture, current_picture->position.x, current_picture->position.y);
			}
			rectb(rc, colors::border);
		}
		y += picture_height + metrics::padding;
		s1.view({x, y, x + picture_width, y_buttons - metrics::padding});
		s2.view({x + picture_width + metrics::padding, y, getwidth() - metrics::padding, y_buttons - metrics::padding});
		header(x + picture_width + metrics::padding, metrics::padding, "��������� ��������",
			"����������� [Ctrl] � ������� �������� ����� ���������� ������������ ������� ��������, ���� ��� ��������� ������ ���� � [300] �� [300] �����.\n\n������� [Enter] ��� ������������� ��� [Esc] ��� ������.");
		y = y_buttons;
		x += button(x, y, "�������", cmd(buttonok));
		x += button(x, y, "������", cmd(buttoncancel));
		domodal();
		if(current_picture) {
			switch(hot.key) {
			case Ctrl | KeyLeft: current_picture->position.x--; break;
			case Ctrl | KeyRight: current_picture->position.x++; break;
			case Ctrl | KeyUp: current_picture->position.y--; break;
			case Ctrl | KeyDown: current_picture->position.y++; break;
			default: break;
			}
		}
	}
	if(getresult()) {
		picture_info* current_picture = find(current_id, current_folder);
		if(!current_picture)
			return false;
		*this = *current_picture;
		return true;
	}
	return false;
}

static void render_title(int x, int y, int width, char* temp, const char* temp_end, const char* title) {
	szprint(temp, temp_end, "%1:", title);
	textf(x, y, width - metrics::padding, temp);
}

static void render_title(int x, int y, int width, const char* title) {
	char temp[260];
	render_title(x, y, width, temp, zendof(temp), title);
}

static void change_picture_info() {
	auto p = (picture_info*)hot.param;
	p->pick();
}

static int field(int x, int y, int width, const char* title, picture_info& v) {
	titletext(x, y + 4, width, 0, title, title_width);
	x += title_width;
	width -= title_width;
	rect rc = {x, y, x + width, y + texth() + 4 * 2};
	char temp[260];
	if(v)
		v.geturl(temp);
	else
		szprint(temp, zendof(temp), "�� ����������");
	return button(x, y, width, 0, cmd(change_picture_info, (int)&v), temp, 0, 0) + metrics::padding;
}

static int field(int x, int y, int width, const char* title, controls::textedit& v) {
	rect rc = {x, y, x + width, y + texth() * 4 + 4 * 2};
	setposition(x, y, width);
	v.view(rc);
	return rc.y2 - y + metrics::padding;
}

static int page_header(int x, int y, const char* title, int current_page, int maximum_page) {
	if(!title)
		return 0;
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	text(x, y, title);
	return texth() + metrics::padding;
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

static int group(int x, int y, int width, const char* title, const void* source, unsigned i1, unsigned i2, unsigned size, const anyval& value) {
	struct cmd : runable {
		constexpr cmd(const anyval& value, const name_info* source, unsigned index)
			: source(source), index(index), value(value) {}
		virtual int	getid() const { return (int)source; }
		virtual void execute() const { command_value = value; draw::execute(set_command_value, index); }
		virtual bool isdisabled() const { return false; }
		bool ischecked() const { return value == index; }
	private:
		const name_info*	source;
		unsigned			index;
		const anyval		value;
	};
	auto y1 = y;
	if(i1 >= i2)
		return 0;
	setposition(x, y, width);
	setposition(x, y, width);
	y += small_header(x, y, width, title);
	for(auto i = i1; i <= i2; i++) {
		auto p = (name_info*)((char*)source + i * size);
		cmd ev(value, p, i);
		unsigned flags = 0;
		if(ev.ischecked())
			flags |= Checked;
		y += radio(x, y, width, flags, ev, p->name, 0);
	}
	return y - y1;
}

void event_info::edit() {
	char ti1[4096]; controls::textedit te1(ti1, sizeof(ti1), false); ti1[0] = 0;
	char ti2[4096]; controls::textedit te2(ti2, sizeof(ti2), false); ti2[0] = 0;
	int y_buttons = getheight() - buttons_height;
	int width = getwidth() - metrics::padding * 2;
	auto race = Elf;
	auto gender = Male;
	setfocus(0, true);
	while(ismodal()) {
		render_background();
		int x = metrics::padding, y = metrics::padding;
		y += page_header(x, y, "������ �����", 1, 2);
		y += field(x, y, width, "��������", picture);
		y += field(x, y, width, "�����, ������� ������ ������", te1);
		y = y_buttons;
		x += button(x, y, "�������", cmd(buttonok));
		x += button(x, y, "������", cmd(buttoncancel));
		domodal();
	}
}

struct page_view {
	int	page_current, page_maximum;
	constexpr page_view(int page_maximum) : page_current(0), page_maximum(page_maximum) {}
	virtual const char*	getheader() const { return "��� ��������"; }
	virtual int	 getmaximum() const { return page_maximum; }
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
			x += button(x, y, "�����", cmd(next_page, (int)this, !nextpage(false)));
			x += button(x, y, "�����", cmd(prev_page, (int)this, !prevpage(false)));
			x += button(x, y, "������", cmd(buttoncancel));
			y = metrics::padding * 2;
			x = metrics::padding * 2;
			y += page_header(x, y, getheader(), 1, 2);
			redraw(x, y, getwidth() - x * 2);
			domodal();
		}
		return getresult();
	}
};

struct character_view : page_view {
	character* player;
	const char* getheader() const {
		return "�������� ���������";
	}
	void clear() override {
		player->clear();
	}
	void redraw(int x, int y, int width) override {
		const int column_width = 200;
		auto y1 = y;
		switch(page_current) {
		case 0:
			y += group(x, y, column_width, "���", gender_data, Male, Female, sizeof(gender_info), player->gender);
			y += group(x, y, column_width, "����", race_data, Human, Halfling, sizeof(race_info), player->race);
			x += column_width + metrics::padding;
			y = y1;
			y += group(x, y, column_width, "�����", class_data, Cleric, FighterMageTheif, sizeof(class_info), player->type);
			x += column_width + metrics::padding;
			y = y1;
			y += group(x, y, column_width, "������������", alignment_data, LawfulGood, ChaoticEvil, sizeof(alignment_info), player->alignment);
			break;
		case 1:
			break;
		}
	}
	constexpr character_view(character* player) : page_view(2), player(player) {}
};

bool character::generate() {
	character_view ev(this);
	return ev.view() != 0;
}