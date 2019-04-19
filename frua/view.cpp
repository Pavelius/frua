#include "crt.h"
#include "draw_control.h"
#include "io.h"
#include "main.h"

using namespace draw;

const int padding = 8;
const int border = 4;
const int picture_width = 320;
const int picture_height = 300;
const int title_width = 100;
const int field_width = 400;
const int buttons_height = 16 + 8 * 2;

static agrw<picture_info>		file_data;
typedef adat<const char*, 256>	strings_array;

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

static int button(const rect& rc, const char* string, const runable& ev) {
	auto id = ev.getid();
	auto dx = textw(string);
	addelement(id, rc);
	if(draw::buttonh(rc, false, getfocus() == ev.getid(), ev.isdisabled(), true, string, 0, false))
		ev.execute();
	return rc.height();
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
	int width = getwidth() - x - padding;
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	y += textf(x, y, width, title) + padding;
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
		auto x = padding, y = padding;
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
		y += picture_height + padding;
		s1.view({x, y, x + picture_width, y_buttons - padding});
		s2.view({x + picture_width + padding, y, getwidth() - padding, y_buttons - padding});
		header(x + picture_width + padding, padding, "Выбирайте картинку",
			"Используйте [Ctrl] и клавиши движения чтобы перемещать отображаемую область картинки, если она превышает размер окна в [300] на [300] точек.\n\nНажмите [Enter] для подтверждения или [Esc] для отмены.");
		y = y_buttons;
		x += button(x, y, "Выбрать", cmd(buttonok));
		x += button(x, y, "Отмена", cmd(buttoncancel));
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
	textf(x, y, width - padding, temp);
}

static void render_title(int x, int y, int width, const char* title) {
	char temp[260];
	render_title(x, y, width, temp, zendof(temp), title);
}

static void change_picture_info() {
	auto p = (picture_info*)hot.param;
	p->pick();
}

static int field(int x, int y, const char* title, picture_info& v) {
	char temp[260];
	int width = 300;
	render_title(x, y + 4, title_width, temp, zendof(temp), title);
	x += title_width;
	width -= title_width;
	rect rc = {x, y, x + width, y + texth() + 4 * 2};
	if(v)
		v.geturl(temp);
	else
		szprint(temp, zendof(temp), "Не изменяется");
	return button(rc, temp, cmd(change_picture_info, (int)&v)) + padding;
}

static int event_header(int x, int y, const char* title) {
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	text(x, y, title);
	return texth() + padding;
}

void event_info::edit() {
	char ti1[4096]; controls::textedit te1(ti1, sizeof(ti1), false); ti1[0] = 0;
	char ti2[4096]; controls::textedit te2(ti2, sizeof(ti2), false); ti2[0] = 0;
	int y_buttons = getheight() - buttons_height;
	const int width = 300;
	while(ismodal()) {
		render_background();
		int x = padding, y = padding;
		y += event_header(x, y, "Боевая сцена");
		y += field(x, y, "Картинка", picture);
		te1.view({x, y, getwidth() - padding, y + texth() * 4 + 4 * 2});
		y = y_buttons;
		x += button(x, y, "Выбрать", cmd(buttonok));
		x += button(x, y, "Отмена", cmd(buttoncancel));
		domodal();
	}
}