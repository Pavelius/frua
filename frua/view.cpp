#include "crt.h"
#include "draw.h"
#include "io.h"
#include "main.h"

using namespace draw;

static struct gui_info {
	unsigned char	border;
	short			button_width, window_width, window_height;
	short			padding;
	void initialize() {
		memset(this, 0, sizeof(*this));
		border = 8;
		padding = 4;
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
		if(!id)
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

static void make_cash(agrw<picture_info>& source, const char* folder) {
	char temp[260];
	for(auto file = io::file::find(folder); file; file.next()) {
		auto pn = file.name();
		if(pn[0] == '.')
			continue;
		auto ext = szext(pn);
		if(!ext) {
			szprint(temp, zendof(temp), "%1/%2", folder, pn);
			make_cash(source, temp);
		} else {
			auto pi = source.add();
			memset(pi, 0, sizeof(pi));
			pi->folder = szdup(folder);
			pi->id = szdup(pn);
		}
	}
}

bool picture_info::pick() {
	auto x = 8, y = 8;
	agrw<picture_info> source;
	make_cash(source, "art");
	while(ismodal()) {
		render_background();
		if(position.x < 0)
			position.x = 0;
		if(position.y < 0)
			position.y = 0;
		picture.load(folder, id);
		if(true) {
			setclip({x, y, x + 300, y + 300});
			auto w = 300;
			if(position.x + w > picture.width)
				w = picture.width - position.x;
			auto h = 300;
			if(position.y + h > picture.height)
				h = picture.height - position.y;
			blit(*canvas, x, y, w, h, 0,
				(surface&)picture, position.x, position.y);
		}
		domodal();
		switch(hot.key) {
		case KeyLeft: position.x--; break;
		case KeyRight: position.x++; break;
		case KeyUp: position.y--; break;
		case KeyDown: position.y++; break;
		default:
			break;
		}
	}
	return true;
}