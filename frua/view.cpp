#include "crt.h"
#include "draw_control.h"
#include "io.h"
#include "main.h"

using namespace draw;

const int picture_width = 300;
const int picture_height = 300;

typedef adat<const char*, 256> strings_array;

struct file_info {
	const char*			id;
	const char*			folder;
};

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
} char_code[] = {{'À', 'F'}, {'Á', ','}, {'Â', 'D'},
{'Ã', 'U'}, {'Ä', 'L'}, {'Å', 'T'},
{'Æ', ';'}, {'Ç', 'B'}, {'È', 'B'},
{'É', 'Q'}, {'Ê', 'R'}, {'Ë', 'K'},
{'Ì', 'V'}, {'Í', 'Y'}, {'Î', 'J'},
{'Ï', 'G'}, {'Ð', 'H'}, {'Ñ', 'C'},
{'Ò', 'N'}, {'Ó', 'E'}, {'Ô', 'A'},
{'Õ', '['}, {'Ö', 'W'}, {'×', 'X'},
{'Ø', 'I'}, {'Ù', 'O'}, {'Ú', ']'},
{'Û', 'S'}, {'Ü', 'M'}, {'Ý', '\''},
{'Þ', '.'}, {'ß', 'Z'},
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

answer* character::choose(const char* url, aref<answer> source) {
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

static void make_cash(agrw<file_info>& source, const char* folder) {
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

static void make_cash(strings_array& result, agrw<file_info>& source) {
	const char* v = 0;
	for(auto& e : source) {
		if(e.folder != v) {
			v = e.folder;
			auto pp = result.add();
			*pp = v;
		}
	}
}

bool picture_info::pick() {
	agrw<file_info> files;
	strings_array folders;
	strings_array filter;
	make_cash(files, "");
	make_cash(folders, files);
	string_view s1(folders);
	string_view s2(filter);
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
		auto current_folder = s1.getcurrent();
		filter.clear();
		for(auto& e : files) {
			if(e.folder != current_folder)
				continue;
			filter.add(e.id);
		}
		s2.correction();
		auto current_id = s2.getcurrent();
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
		s1.view({x, y, x + picture_width, getheight() - 32});
		s2.view({x + picture_width + gui.padding, y, getwidth() - gui.padding, getheight() - 32});
		domodal();
		//switch(hot.key) {
		//case KeyLeft: position.x--; break;
		//case KeyRight: position.x++; break;
		//case KeyUp: position.y--; break;
		//case KeyDown: position.y++; break;
		//default:
		//	break;
		//}
	}
	return true;
}