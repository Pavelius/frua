#include "crt.h"
#include "draw_control.h"
#include "io.h"
#include "main.h"

using namespace draw;

const int picture_width = 300;
const int picture_height = 300;

typedef adat<const char*, 256> strings_array;

static struct gui_info {
	unsigned char	border;
	short			button_width, window_width, window_height;
	short			padding;
	void initialize() {
		memset(this, 0, sizeof(*this));
		border = 8;
		padding = 8;
		window_width = 400;
		button_width = 64;
	}
	gui_info() { initialize(); }
} gui;
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
		if(*this)
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
		if(source.count==0)
			return 0;
		return source.data[current];
	}
	constexpr string_view(strings_array& source) : source(source) {}
};
static agrw<picture_info> file_data;

static void window(rect rc, int border = 0) {
	if(border == 0)
		border = gui.border;
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
	return height + gui.border * 2;
}

static int button(int x, int y, const char* string) {
	const int border = 2;
	draw::state push;
	//font = metrics::h3;
	auto dy = texth();
	auto dx1 = textw(string[0]) + 1;
	auto dx2 = textw(string + 1);
	auto dx = dx1 + dx2 + gui.padding + border * 2;
	rect rc = {x, y, x + dx, y + dy};
	window(rc, border);
	auto old_fore = fore;
	fore = fore.mix(colors::special, 128);
	text(rc.x1, rc.y1, string, 1, TextBold); rc.x1 += dx1;
	fore = old_fore;
	text(rc.x1 + dx1, rc.y1, string + 1);
	return dx + gui.padding + border * 2;
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
		for(auto& e : source)
			x += button(x, y, e.name);
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
	int width = getwidth() - x;
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	y += textf(x, y, width, title) + gui.padding;
	fore = colors::text;
	font = metrics::font;
	y += textf(x, y, width, text);
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
	while(ismodal()) {
		render_background();
		if(position.x + picture_width > picture.width)
			position.x = picture.width - picture_width;
		if(position.x < 0)
			position.x = 0;
		if(position.y + picture_height > picture.height)
			position.y = picture.height - picture_height;
		if(position.y < 0)
			position.y = 0;
		current_folder = s1.getcurrent();
		filter.clear();
		for(auto& e : file_data) {
			if(e.folder != current_folder)
				continue;
			filter.add(e.id);
		}
		s2.correction();
		current_id = s2.getcurrent();
		picture.load(current_folder, current_id);
		auto x = gui.padding, y = gui.padding;
		rect rc = {x, y, x + picture_width, y + picture_height};
		if(true) {
			draw::state push;
			setclip(rc);
			blit(*canvas, x, y, picture_width, picture_height, 0,
				picture, position.x, position.y);
		}
		rectb(rc, colors::border);
		y += picture_height + gui.padding * 2;
		s1.view({x, y, x + picture_width, getheight() - gui.padding});
		s2.view({x + picture_width + gui.padding, y, getwidth() - gui.padding, getheight() - gui.padding});
		header(x + picture_width + gui.padding, gui.padding, "Выбирайте картинку",
			"Используйте [Ctrl] и клавиши движения чтобы перемещать отображаемую область картинки, если она превышает размер окна в [300] на [300] точек.\n\nНажмите [Enter] для подтверждения или [Esc] для отмены.");
		domodal();
		switch(hot.key) {
		case Ctrl | KeyLeft: position.x--; break;
		case Ctrl | KeyRight: position.x++; break;
		case Ctrl | KeyUp: position.y--; break;
		case Ctrl | KeyDown: position.y++; break;
		case KeyEnter: breakmodal(1); break;
		case KeyEscape: breakmodal(0); break;
		default:
			break;
		}
	}
	if(getresult()) {
		this->id = current_id;
		this->folder = current_folder;
		return true;
	}
	return false;
}