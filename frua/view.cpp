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
const int combat_moverate = 3;

struct page_view;
struct command;

static anyval					command_value;
static agrw<picture_info>		file_data;
static agrw<picture_info>		file_monster_data;
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
	bool load(const char* folder, const char* id, int part = -1) {
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
		if(part == -1)
			geturl(temp);
		else
			geturl(temp, part);
		this->read(temp);
		return true;
	}
} picture;

struct string_view : controls::list {
	aref<const char*> source;
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
	constexpr string_view(const aref<const char*>& source) : source(source) {}
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

static void sprite_write(const sprite* p, const char* url) {
	io::file file(url, StreamWrite);
	if(!file)
		return;
	file.write(p, p->size);
	pma trail = {0};
	file.write(&trail, sizeof(trail));
}

void view_initialize() {
	spr_monsters = (sprite*)loadb("art/monsters.pma");
	if(spr_monsters) {
		avatar_data.data = (sprite_name_info*)spr_monsters->edata();
		avatar_data.count = spr_monsters->count / 2;
	}
}

int draw::button(int x, int y, const char* string, const runable& ev, unsigned key) {
	auto id = ev.getid();
	auto dx = textw(string);
	rect rc = {x, y, x + dx + metrics::padding * 2, y + texth() + metrics::padding * 2};
	addelement(id, rc);
	auto focused = getfocus() == ev.getid();
	if(draw::buttonh(rc, false, focused, ev.isdisabled(), true, string, key, false))
		ev.execute();
	if(focused)
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
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

const picture_info* picture_info::choose_image() {
	struct string_view : controls::list {
		aref<picture_info> source;
		int getmaximum() const override {
			return source.count;
		}
		const char*	getname(char* result, const char* result_max, int line, int column) const {
			if(source.data) {
				switch(column) {
				case 0: return source.data[line].id;
				case 1: return source.data[line].folder;
				default: return "";
				}
			}
			return "";
		}
		picture_info* getcurrent() const {
			if(!source)
				return 0;
			return source.data + current;
		}
		constexpr string_view(const aref<picture_info>& source) : source(source) {}
	};
	if(!file_data)
		make_cash(file_data, "", file_exclude, file_ext_exclude);
	string_view s1(aref<picture_info>(file_data.data, file_data.count));
	int y_buttons = getheight() - buttons_height;
	setfocus(0, true);
	while(ismodal()) {
		render_background();
		auto current_picture = s1.getcurrent();
		auto x = metrics::padding, y = metrics::padding;
		s1.view({x + picture_width + metrics::padding, y, getwidth() - metrics::padding, y_buttons - metrics::padding});
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

static void save_monsters() {
	sprite_write(spr_monsters, "art/monsters.pma");
	buttonok();
}

const picture_info* picture_info::edit_monsters() {
	struct string_view : controls::list {
		aref<sprite_name_info> source;
		int getmaximum() const override {
			return source.count;
		}
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
		return 0;
	unsigned index_maximum = spr_monsters->count / 2;
	string_view s1(aref<sprite_name_info>((sprite_name_info*)spr_monsters->edata(), index_maximum));
	int y_buttons = getheight() - buttons_height;
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
		x += button(x, y, "Выбрать", cmd(save_monsters, (int)spr_monsters), KeyEscape);
		x += button(x, y, "Сменить", cmd(change_monster_part), KeySpace);
		x += button(x, y, "Вверх", cmd(change_up, (int)current_frame), Ctrl + KeyUp);
		x += button(x, y, "Вниз", cmd(change_down, (int)current_frame), Ctrl + KeyDown);
		x += button(x, y, "Вправо", cmd(change_right, (int)current_frame), Ctrl + KeyRight);
		x += button(x, y, "Влево", cmd(change_left, (int)current_frame), Ctrl + KeyLeft);
		x += button(x, y, "Отмена", cmd(buttoncancel), KeyEscape);
		domodal();
	}
	return 0;
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
	auto pi = p->choose_image();
	if(pi)
		*p = *pi;
}

static int field(int x, int y, int width, const char* title, picture_info& v) {
	setposition(x, y, width);
	titletext(x, y, width, 0, title, title_width);
	rect rc = {x, y, x + width, y + texth() + 4 * 2};
	char temp[260];
	if(v)
		v.geturl(temp);
	else
		szprint(temp, zendof(temp), "Не изменяется");
	unsigned flags = 0;
	focusing((int)&v, flags, rc);
	if(buttonh(rc,
		ischecked(flags), isfocused(flags), isdisabled(flags), true, temp, 0, false, 0)
		|| (isfocused(flags) && hot.key == KeyEnter)) {
		execute(change_picture_info, (int)&v);
	}
	if(isfocused(flags))
		rectx({rc.x1 + 2, rc.y1 + 2, rc.x2 - 2, rc.y2 - 2}, colors::border);
	return rc.height() + metrics::padding * 2;
}

static int field(int x, int y, int width, const char* title, controls::textedit& v) {
	setposition(x, y, width);
	rect rc = {x, y, x + width, y + texth() * 4 + 4 * 2};
	v.view(rc);
	return rc.y2 - y + metrics::padding;
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

static int page_header(const char* title, int current_page, int maximum_page) {
	if(!title)
		return 0;
	auto x = metrics::padding, y = metrics::padding;
	draw::state push;
	font = metrics::h1;
	fore = colors::yellow;
	text(x, y, title);
	y += texth() + metrics::padding;
	return y;
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
	auto rc = start_group(x, y, width, title);
	for(auto i = i1; i <= i2; i++) {
		auto p = (name_info*)((char*)source + i * size);
		cmd ev(value, p, i);
		unsigned flags = 0;
		if(ev.ischecked())
			flags |= Checked;
		y += radio(x, y, width, flags, ev, p->name, 0);
	}
	y += close_group(x, y, rc);
	return y - y1 + metrics::padding;
}

void event_info::edit() {
	char ti1[4096]; controls::textedit te1(ti1, sizeof(ti1), false); ti1[0] = 0;
	char ti2[4096]; controls::textedit te2(ti2, sizeof(ti2), false); ti2[0] = 0;
	int y_buttons = getheight() - buttons_height;
	int width = getwidth() - metrics::padding * 2;
	int strenght = 10;
	setfocus(0, true);
	while(ismodal()) {
		render_background();
		auto x = metrics::padding;
		auto y = page_header("Боевая сцена", 1, 2);
		y += field(x, y, width, "Картинка", picture);
		y += field(x, y, 160, "Число", strenght, title_width, 2);
		y += field(x, y, width, "Текст, который увидят игроки", te1);
		y = y_buttons;
		x += button(x, y, "Выбрать", cmd(buttonok));
		x += button(x, y, "Отмена", cmd(buttoncancel));
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
			y = page_header(getheader(), 1, 2);
			redraw(x, y, getwidth() - x * 2);
			domodal();
		}
		return getresult();
	}
};

static int group_combat_ability(int x, int y, int width, const character* player) {
	char temp[260];
	attack_info ai = {}; player->get(MeleeWeapon, ai);
	auto y1 = y;
	auto rga = start_group(x, y, width, "Боевые параметры");
	y += fieldv(x, y, width, "Количество атак", ai.getattacks(temp, zendof(temp)));
	y += fieldv(x, y, width, "THAC0", ai.thac0);
	y += fieldv(x, y, width, "Урон", ai.damage.print(temp, zendof(temp)));
	y += fieldv(x, y, width, "Класс брони", player->getac());
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
	y += group(x, y, w1, "Пол", gender_data, Male, Female, sizeof(gender_info), gender);
	y += group(x, y, w1, "Раса", race_data, Human, Halfling, sizeof(race_info), race);
	x += w1 + metrics::padding * 3;
	y = y1;
	y += group(x, y, w1, "Класс", class_data, Cleric, FighterMageTheif, sizeof(class_info), type);
	x += w1 + metrics::padding * 3;
	y = y1;
	y += group(x, y, w1, "Мировозрение", alignment_data, LawfulGood, ChaoticEvil, sizeof(alignment_info), alignment);
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
		auto class_count = class_data[player->type].classes.count;
		if(!class_count)
			class_count = 1;
		for(unsigned i = 0; i < class_data[player->type].classes.count; i++) {
			auto e = class_data[player->type].classes.data[i];
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
		y += group_combat_ability(x, y, w1, player) + metrics::padding;
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
	avatar = 0;
	auto index = choose_avatar("Как выглядит ваш герой?", "character*", 64, avatar);
	if(index != -1)
		avatar = index;
	return true;
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
	if(i == Blocked)
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
			auto i = map::m2i(x, y);
			auto c = map::getcost(i);
			if(!c)
				continue;
			auto x1 = x0 + x * combat_grid + combat_grid / 2;
			auto y1 = y0 + y * combat_grid + combat_grid / 2;
			if(c == Blocked) {
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
	if(index == Blocked)
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
	p->move(d);
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
	auto y_buttons = getheight() - buttons_height;
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
	auto y_buttons = getheight() - buttons_height;
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

int character::choose_avatar(const char* title, const char* mask, int size, int current) {
	struct avatar_view : controls::control {
		adat<short unsigned, 512>	elements;
		point		size, origin;
		int			current, current_hilite, scanline;
		rect		view_rect;
		static void choose_mouse() {
			auto p = (avatar_view*)hot.param;
			p->current = p->current_hilite;
		}
		void view(const rect& rc) override {
			control::view(rc);
			view_rect = rc;
			auto x = rc.x1;
			auto y = rc.y1;
			auto c = 0;
			current_hilite = -1;
			for(unsigned i = 0; i < elements.count; i++) {
				if(c >= scanline) {
					x = rc.x1;
					y += size.y;
					c = 0;
				}
				rect rc = {x, y, x + size.x, y + size.y};
				rectb(rc, colors::border);
				auto index = elements.data[i];
				if(current == i) {
					rect rx = rc;
					rx.offset(2, 2);
					rectf({rx.x1, rx.y1, rx.x2 + 1, rx.y2 + 1}, colors::edit);
					if(isfocused())
						rectx(rx, colors::border);
				}
				image(x + origin.x, y + origin.y, spr_monsters, index * 2, 0);
				auto a = area(rc);
				if((a == AreaHilited || a == AreaHilitedPressed)) {
					current_hilite = i;
					if(hot.key == MouseLeft && hot.pressed)
						execute(choose_mouse, (int)this);
				}
				c++;
				x += size.x;
			}
		}
		unsigned getmaximum() const {
			return elements.count;
		}
		int gethorizcount() const {
			if(!size.x || !view_rect)
				return 0;
			return view_rect.width() / size.x;
		}
		int getcurrent() const {
			if(!elements.count)
				return -1;
			return elements.data[current];
		}
		void correct() {
			if(current > (int)elements.count - 1)
				current = elements.count - 1;
			if(current < 0)
				current = 0;
		}
		bool keyinput(unsigned id) override {
			switch(id) {
			case KeyLeft:
				current--;
				correct();
				break;
			case KeyRight:
				current++;
				correct();
				break;
			case KeyUp:
				current -= scanline;
				correct();
				break;
			case KeyDown:
				current += scanline;
				correct();
				break;
			default:
				return control::keyinput(id);
			}
			return false;
		}
		constexpr avatar_view(int size) : elements(), size{(short)size, (short)size}, origin{(short)size / 2, ((short)size / 4) * 3},
			current(0), current_hilite(-1), scanline((short)(800 / size)), view_rect() {}
	};
	avatar_view s1(size);
	s1.elements.count = select_avatar(s1.elements.data, s1.elements.getmaximum(), mask);
	s1.current = s1.elements.indexof(current);
	s1.correct();
	setfocus(0, true);
	auto y_buttons = getheight() - buttons_height;
	while(ismodal()) {
		render_background();
		auto x = metrics::padding;
		auto y = page_header(title, 0, 0);
		s1.view({x, y, getwidth() - metrics::padding, y_buttons - metrics::padding * 2});
		y = y_buttons;
		x += button(x, y, "Выбрать", cmd(buttonok), KeyEnter);
		x += button(x, y, "Отмена", cmd(buttoncancel), KeyEscape);
		domodal();
	}
	return s1.getcurrent();
}