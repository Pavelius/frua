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

const markup* getmarkup(const bsreq* type);
void field_enum(const rect& rc, unsigned flags, const anyval& ev, const bsreq* meta_type, const void* object, const markup::proci* pri, const markup::propi* ppi);

static anyval					command_value;
static bsdata*					command_data;
static rect						command_rect;
static char						command_mask[260];
static agrw<picture_info>		file_data;
static const char*				file_exclude[] = {"fonts", "monsters", 0};
static const char*				file_ext_exclude[] = {"pma", 0};
static const char*				file_monster_exclude[] = {0};
static const sprite*			spr_monsters;
static const sprite*			spr_blood = (sprite*)loadb("art/sprites/blood.pma");
aref<sprite_name_info>			avatar_data;
static char						logger_buffer[2048];
stringobject					msg_logger(logger_buffer, logger_buffer + sizeof(logger_buffer) - 1);
typedef adat<const char*, 256>	strings_array;

static void set_command_value() {
	command_value = (const int)hot.param;
}

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
	if(draw::buttonh(rc, checked, focused, false, true, string, key, false))
		ev.execute();
	if(focused)
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
	return rc.width() + 2;
}

int	draw::button(int x, int y, int width, unsigned flags, const runable& cmd, const char* label, const char* tips, int key) {
	setposition(x, y, width);
	rect rc = {x, y, x + width, y + texth() + metrics::padding * 2};
	focusing((int)cmd.getid(), flags, rc);
	auto focused = (getfocus() == cmd.getid());
	if(draw::buttonh(rc, false, focused, false, true, label, key, false))
		cmd.execute();
	if(focused)
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
	return rc.height() + metrics::padding * 2;
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
	rectf(rc, colors::form);
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
		agrw<picture_info>	source;
		int getmaximum() const override { return source.getcount(); }
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

static int field(int x, int y, int width, const anyval& ev, const bsreq* type, const void* object, const markup::proci* pri, const markup::propi* ppi) {
	setposition(x, y, width);
	rect rc = {x, y, x + width, y + draw::texth() + 8};
	unsigned flags = AlignLeft;
	draw::focusing((int)ev.ptr(), flags, rc);
	field_enum(rc, flags, ev, type, object, pri, ppi);
	return rc.height() + metrics::padding * 2;
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

static void page_header(int& x, int& y, const char* title_prefix, const char* title, const char* page_title, int page = 0, int page_maximum = 0) {
	char temp[260]; stringcreator sc(temp);
	x = metrics::padding;
	y = metrics::padding;
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	if(title_prefix)
		sc.add(title_prefix);
	sc.adds(title);
	if(page_title)
		sc.adds("(%1)", page_title);
	text(x, y, temp);
	if(page_maximum > 0) {
		sc.clear();
		sc.add("Страница %1i из %2i", page + 1, page_maximum);
		auto height = texth();
		font = metrics::h3;
		fore = colors::h3;
		text(getwidth() - x - textw(temp), y + height - texth(), temp);
	}
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

struct image_info : point {
	const sprite*			ps;
	short unsigned			frame, flags;
	unsigned char			alpha;
	void*					object;
	void clear() { memset(this, 0, sizeof(*this)); }
};
static adat<image_info, 1024> scene_images;

static image_info* find_image(const void* object) {
	for(auto& e : scene_images) {
		if(e.object == object)
			return &e;
	}
	return 0;
}

static void draw_grid(int x0, int y0) {
	color c = colors::window;
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
	fore = colors::window;
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
	auto y = (index / combat_map_x)*combat_grid + combat_grid / 2;
	auto pi = scene_images.add();
	pi->frame = getavatar() * 2;
	pi->flags = 0;
	pi->alpha = 0xFF;
	pi->ps = spr_monsters;
	pi->x = x;
	pi->y = y;
	pi->object = this;
	if(direction == Right)
		pi->flags |= ImageMirrorH;
}

static int compare_image(const void* v1, const void* v2) {
	auto p1 = (image_info*)v1;
	auto p2 = (image_info*)v2;
	auto y1 = p1->y;
	auto y2 = p2->y;
	if(y1 != y2)
		return y1 - y2;
	return p1->x - p2->x;
}

static void update_scene() {
	qsort(scene_images.data, scene_images.count, sizeof(scene_images.data[0]), compare_image);
}

void combat_info::update() const {
	scene_images.clear();
	for(auto p : parcipants)
		p->addbattle();
	update_scene();
}

void combat_info::animate(const character* attacker, const character* defender, bool hit) {
	auto pc = getactive();
	if(!pc)
		return;
	auto pp = find_image(attacker);
	auto pe = find_image(defender);
	if(!pp || !pe)
		return;
	pp->frame++;
	if(hit) {
		auto pi = scene_images.add();
		pi->frame = 2;
		pi->flags = 0;
		pi->alpha = 0xFF;
		pi->ps = spr_blood;
		pi->x = pe->x;
		pi->y = pe->y - 16;
		pc->splash(100, false);
		if(!defender->isalive())
			pe->ps = 0;
		pi->frame = 3;
		pc->splash(100, false);
		scene_images.remove(scene_images.indexof(pi));
	} else
		pc->splash(200, false);
	pp->frame--;
	pc->splash(50, false);
	msg_logger.clear();
}

static void draw_images(int x, int y) {
	for(auto& e : scene_images) {
		if(!e.ps)
			continue;
		image(x + e.x, y + e.y, e.ps, e.frame, e.flags, e.alpha);
	}
}

void combat_info::visualize(bool use_update) const {
	render_background();
	auto x = (getwidth() - combat_grid * combat_map_x) / 2;
	auto y = y_buttons - metrics::padding * 2 - combat_grid * combat_map_y - texth() - 2;
	if(use_update)
		update();
	draw_grid(x, y);
	draw_cost(x, y);
	draw_active_player(x, y);
	draw_images(x, y);
	if(msg_logger) {
		draw::textf(x, y_buttons - metrics::padding - texth() - 2, getwidth() - metrics::padding * 2,
			msg_logger);
	}
}

void combat_info::splash(unsigned seconds, bool use_update) const {
	visualize(use_update);
	sysredraw();
	sleep(seconds);
}

int answer::choose(combat_info& ci) {
	// Command with focus on value rather that procedure
	struct cmdv : cmd {
		int getid() const override { return v + 1; }
		constexpr cmdv(void(*p)(), int v) : cmd(p, v) {}
	};
	auto player = character::getactive();
	if(!player)
		return 0;
	openform();
	while(ismodal()) {
		player->act("[%герой] (hp %1i/%2i, AC%3i, MV %4i/%5i)",
			player->gethp(), player->gethpmax(),
			player->get(AC),
			player->getmovepoints(), player->getmovement());
		player->act("готов%а действовать");
		ci.visualize(true);
		auto y = y_buttons;
		auto x = metrics::padding;
		for(unsigned i = 0; i < count; i++)
			x += button(x, y, data[i].name, cmdv(buttonparam, i), data[i].key ? data[i].key : Alpha + '1' + i);
		if(!count)
			x += button(x, y, "Продолжить", cmdv(buttoncancel, 0), KeySpace);
		domodal();
	}
	closeform();
	auto result = getresult();
	if(result >= (int)count)
		return 0;
	result = data[result].id;
	clear();
	return result;
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
		page_header(x, y, title, 0, 0, 0, 0);
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

int character::view_personal(int x, int y, int width, const void* object, const char* id, int index) {
	auto y0 = y;
	auto p = (character*)object;
	char temp[260]; zprint(temp, "%+2 %-1", getstr(p->gender), getstr(p->race));
	x += metrics::padding;
	width -= metrics::padding * 2;
	text(x, y, getstr(p->alignment)); y += texth() + 2;
	text(x, y, temp); y += texth() + 2;
	return y - y0;
}

int character::view_levels(int x, int y, int width, const void* object, const char* id, int index) {
	auto y0 = y;
	auto p = (character*)object;
	auto& col = bsmeta<class_s>::data;
	auto class_count = col[p->type].classes.count;
	if(!class_count)
		class_count = 1;
	auto exp = p->experience / class_count;
	for(unsigned i = 0; i < col[p->type].classes.count; i++) {
		auto e = col[p->type].classes.data[i];
		char temp[260]; zprint(temp, "%1i уровень (опыт %2i)", p->levels[i], exp);
		y += fieldv(x, y, width, getstr(e), temp);
	}
	return y - y0;
}

int character::view_skills(int x, int y, int width, const void* object, const char* id, int index) {
	auto p = (character*)object;
	auto y0 = y;
	for(auto i = FirstSave; i <= LastSkill; i = (skill_s)(i + 1)) {
		auto v = p->get(i);
		if(v <= 0)
			continue;
		y += fieldp(x, y, width, getstr(i), v);
	}
	return y - y0;
}

int character::view_ability(int x, int y, int width, const void* object, const char* id, int index) {
	auto p = (character*)object;
	auto y0 = y;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		auto v = p->get(i);
		if(i == Strenght && v == 18)
			y += fieldv(x, y, width, getstr(i), v, p->getstrper(), "%1i/%2.2i");
		else
			y += fieldv(x, y, width, getstr(i), v);
	}
	return y - y0;
}

int character::gethprollmax() const {
	auto& col = bsmeta<class_s>::data;
	auto type = getclass();
	auto rolled = 0;
	for(unsigned i = 0; i < col[type].classes.count; i++)
		rolled += col[col[type].classes[i]].hd * getlevel(i);
	return rolled;
}

int character::view_statistic(int x, int y, int width, const void* object, const char* id, int index) {
	char temp[260];
	auto p = (character*)object;
	attack_info ai = {}; p->get(p->get(Weapon), ai);
	auto y0 = y; stringcreator sc(temp); ai.getattacks(sc);
	y += fieldv(x, y, width, "Количество атак", temp);
	y += fieldv(x, y, width, "THAC0", ai.bonus);
	y += fieldv(x, y, width, "Урон", ai.damage.print(temp, zendof(temp)));
	y += fieldv(x, y, width, "Класс брони", p->get(AC));
	if(p->hp_rolled == 0) {
		auto min = p->gethpmax(0);
		auto max = p->gethpmax(p->gethprollmax());
		y += fieldv(x, y, width, "Хиты", min, max, "%1i-%2i");
	} else if(p->hp == 0)
		y += fieldv(x, y, width, "Хиты", p->gethpmax());
	else
		y += fieldv(x, y, width, "Хиты", p->gethp(), p->gethpmax(), "%1i/%2i");
	return y - y0;
}

int character::view_avatar(int x, int y, int width, const void* object, const char* id, int index) {
	auto p = (character*)object;
	unsigned flags = 0;
	auto height = 64;
	draw::setposition(x, y, width);
	rect rc = {x, y, x + width, y + height};
	focusing((int)&p->avatar, flags, rc);
	auto focused = isfocused(flags);
	auto a = area(rc);
	switch(a) {
	case AreaHilited: rectf(rc, colors::edit.mix(colors::window)); break;
	case AreaHilitedPressed: rectf(rc, colors::edit); break;
	}
	if(spr_monsters) {
		auto dx = width / 4;
		auto y1 = y + (height / 8) * 7;
		image(x + dx * 1, y1, spr_monsters, p->avatar * 2 + 0, 0);
		image(x + dx * 3, y1, spr_monsters, p->avatar * 2 + 1, 0);
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
	if(result)
		cmd(character::choose_avatar, (void*)object).execute();
	return rc.height() + metrics::padding * 2;
}

int item::view_state(int x, int y, int width, const void* object, const char* id, int index) {
	auto p = (item*)object;
	return field(x, y, width, anyval(&p->value, sizeof(p->value)), bsmeta<item_state_s>::meta, object, 0, 0);
}

int item::view_check(int x, int y, int width, const void* object, const char* id, int index) {
	auto p = (item*)object;
	setposition(x, y, width);
	return checkbox(x, y + 3, width, 0, cmd(buttonok), "Опознан", 0) + metrics::padding * 2;
}

void damage_info::edit(void* p) {
	decoration::edit((damage_info*)p);
}

int item_info::view_weapon(int x, int y, int width, const void* object, const char* id, int index) {
	char temp[260]; stringcreator sc(temp);
	auto p = (item_info*)object;
	p->getweapon(sc);
	szupper(sc, 1);
	return button(x, y, width, 0, cmdv(editweapon, (void*)object, &p->damage), temp, 0);
}

void character::apply_avatar(void* object) {
	auto p = (character*)object;
	if(!picture_info::choose(p->avatar, "Укажите картинку персонажа", "character*", 64))
		return;
}

static void add_value() {
	auto& p = *((int*)hot.param);
	p++;
}

static void sub_value() {
	auto& p = *((int*)hot.param);
	p--;
}

bool decoration::edit(const char* name, void* object, unsigned size, const bsreq* type, const markup* elements, bool creating) {
	int x, y;
	if(!elements) {
		// Получим форму элемента по умолчанию
		elements = getmarkup(type);
		auto fm = elements->getform(object, "element");
		if(fm) {
			elements = fm->value.child;
			if(fm->title)
				name = fm->title;
		}
	}
	if(!elements)
		return false;
	if(!name) {
		auto pd = decoration::find(type);
		if(pd)
			name = pd->name;
	}
	if(!name)
		name = "Форма редактирования";
	if(creating)
		elements->action("create", object);
	char r2_buffer[256];
	auto r2 = r2_buffer;
	if(size > sizeof(r2_buffer))
		r2 = new char[size];
	auto old_focus = getfocus();
	openform();
	auto current_page = 0;
	const markup* page_markup_last = 0;
	auto updater = elements->find("update", object, 0, false);
	auto type_key = type->getkey();
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
		if(type_key && type_key->istext()) {
			auto pn = (const char*)type_key->get(type_key->ptr(object));
			if(pn && pn[0])
				page_name = pn;
		}
		render_background();
		page_header(x, y, 0, page_name, page_title, current_page, page_maximum);
		auto width = getwidth() - x * 2;
		y += draw::field(x, y, width, page, bsval(object, type), 100);
		page_footer(x, y, true);
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

int decoration::choose(const char* title, int width, int height, bool choose_mode) const {
	struct table : controls::picker {
		adat<int, 512>		source;
		bsdata&				database;
		const decoration&	manager;
		static int get_avatar(const void* pv, const bsreq* type) {
			auto pf = type->find("avatar");
			if(!pf)
				return -1;
			return pf->get(pf->ptr(pv));
		}
		static const char* get_name(const void* pv, const bsreq* type) {
			auto pf = type->find("name");
			if(!pf)
				return "No name";
			return (const char*)pf->get(pf->ptr(pv));
		}
		int getmaximum() const override { return source.count; }
		void row(const rect& rcorigin, int index) override {
			auto pv = database.get(source.data[index]);
			rect rc = rcorigin;
			rowhilite(rc, index);
			auto avatar = get_avatar(pv, database.meta);
			if(avatar != -1) {
				rect rp = {rc.x1, rc.y1, rc.x1 + rc.height(), rc.y2};
				draw::state push;
				setclip(rp);
				image(rp.x1 + rp.width() / 2, rp.y2 - rp.height() / 4, spr_monsters, avatar * 2, 0);
				rc.x1 += rc.height() + 2;
			}
			auto pn = get_name(pv, database.meta);
			rc.offset(4, 4);
			if(pn) {
				draw::textc(rc.x1, rc.y1, rc.width(), pn);
				rc.y1 += texth() + 2;
			}
			char temp[260]; temp[0] = 0;
			if(database.meta == bsmeta<character>::meta) {
				pn = character::getdescription((character*)pv, temp, zendof(temp));
				if(pn) {
					auto old_fore = fore;
					fore = colors::text.mix(colors::form, 128);
					text(rc, pn);
					fore = old_fore;
				}
			}
		}
		int getcurrent() const {
			if(!source.count)
				return -1;
			return source[current];
		}
		void update() {
			source.clear();
			unsigned index = 0;
			if(manager.zero_element != 0)
				index++;
			auto index_max = database.count;
			for(; index < index_max; index++)
				source.add(index);
		}
		static void add(table* p) {
			if(p->manager.edit(p->database, 0, 0))
				p->update();
		}
		static void copy(table* p) {
			if(p->manager.edit(p->database, 0, (void*)p->database.get(p->getcurrent())))
				p->update();
		}
		static void change(table* p) {
			if(p->manager.edit(p->database, (void*)p->database.get(p->getcurrent()), 0))
				p->update();
		}
		table(bsdata& database, const decoration& manager) : database(database), manager(manager) {
			update();
		}
	};
	if(!database)
		return -1;
	table e1(*database, *this);
	e1.show_border = false;
	e1.pixels_per_line = height;
	e1.pixels_per_column = width;
	e1.hilite_odd_lines = false;
	setfocus(0, true);
	int x, y;
	while(ismodal()) {
		render_background();
		page_header(x, y, "Доступные", title, 0);
		e1.view({x, y, getwidth() - x, y_buttons - metrics::padding * 2});
		page_footer(x, y, choose_mode);
		if(database->count < database->maximum) {
			x += button(x, y, "Добавить", cmd(table::add, &e1), F3);
			if(database->count > 0)
				x += button(x, y, "Скопировать", cmd(table::copy, &e1), F4);
		}
		if(e1.getmaximum() > 0) {
			auto pc = cmd(table::change, &e1);
			x += button(x, y, "Редактировать", pc, KeyEnter);
			if(hot.key == F2)
				pc.execute();
		}
		domodal();
	}
	if(getresult()) {
		// DEBUG: сохраним модуль автоматически
		save_campaign();
		return e1.getcurrent();
	}
	return -1;
}

void* decoration::choose(const char* title, void** source, unsigned count, const bsreq* type, const markup* columns) {
	controls::reftable e1(source, count, type, columns);
	openform();
	while(ismodal()) {
		render_background();
		int x, y;
		page_header(x, y, 0, title, 0);
		e1.view({x, y, getwidth() - x, y_buttons - metrics::padding * 2});
		page_footer(x, y, false);
		domodal();
	}
	closeform();
	return (void*)getresult();
}