#include "draw.h"
#include "main.h"

using namespace draw;

static const char*	picture_url;
static surface		picture;

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

static void load_picture(const char* url) {
	if(!url)
		return;
	if(picture_url && strcmp(url, picture_url) == 0)
		return;
	picture_url = szdup(url);
	if(picture)
		picture.clear();
	picture.read(url);
}

static void render_picture(int x, int y) {
	auto w = 300;
	if(w > picture.width)
		w = picture.width;
	auto h = 300;
	if(h > picture.height)
		h = picture.height;
	blit(*canvas, x, y, w, h, 0, picture, 0, 0);
}

answer* character::choose(const char* url, aref<answer> source) {
	load_picture(url);
	while(ismodal()) {
		rect rc = {0, 0, getwidth(), getheight()};
		rectf(rc, colors::window);
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