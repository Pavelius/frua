#include "pointl.h"
#include "draw.h"
#include "markup.h"

#pragma once

namespace clipboard {
void					copy(const void* string, int lenght);
char*					paste();
}
struct					bsreq;
struct					bsval;
struct					markup;
namespace draw {
enum column_size_s : unsigned char {
	SizeDefault,
	SizeResized, SizeFixed, SizeInner, SizeAuto,
};
enum select_mode_s : unsigned char {
	SelectCell, SelectText, SelectRow,
};
enum column_type_s {
	Text = 0x00000, Number = 0x10000,
	ControlMask = 0xF0000
};
enum field_type_s : unsigned char {
	FieldNumber, FieldText,
};
struct runable {
	virtual void			execute() const = 0;
	virtual int				getid() const = 0;
};
// Standart command
struct cmd : runable {
	typedef void(*p0)();
	typedef void(*p1)(void* v);
	constexpr cmd(p0 p, int v = 0) : p(p), v(v), call(&cmd::call0) {}
	template<typename T> constexpr cmd(void(*p)(T*), T* v) : p((p0)p), v((int)v), call(&cmd::call1) {}
	void					execute() const override { (this->*call)(); }
	int						getid() const override { return (int)p; }
protected:
	void					call0() const;
	void					call1() const;
	void					(cmd::*call)() const;
	p0						p;
	int						v;
};
// Command with focus on integer value rather that procedure
struct cmdi : cmd {
	int getid() const override { return v + 1; }
	constexpr cmdi(void(*p)(), int v) : cmd(p, v) {}
};
// Command with custom focus
struct cmdv : cmd {
	void*					focus;
	int getid() const override { return (int)focus; }
	template<typename T> constexpr cmdv(void(*p)(T*), T* v, void* f) : cmd(p, v), focus(f) {}
};
namespace controls {
struct control {
	bool					show_border;
	constexpr control() : show_border(true) {}
	virtual ~control() {}
	virtual const char*		getlabel(char* result, const char* result_maximum) const { return 0; }
	virtual bool			isdisabled() const { return false; }
	virtual bool			isfocusable() const { return true; }
	virtual bool			isfocused() const;
	bool					ishilited() const;
	virtual bool			keyinput(unsigned id) { return false; }
	virtual void			mouseinput(unsigned id, point mouse); // Default behaivor set focus
	virtual void			mousewheel(unsigned id, point mouse, int value) {}
	virtual void			redraw() {}
	virtual void			view(const rect& rc);
};
// Analog of listbox element
struct list : control {
	int						origin, current, current_hilite, origin_width;
	int						lines_per_page, pixels_per_line, pixels_per_width;
	bool					show_grid_lines, show_selection, show_header;
	bool					hilite_odd_lines;
	rect					current_rect, view_rect;
	constexpr list() : origin(0), current(0), current_hilite(-1), origin_width(0),
		lines_per_page(0), pixels_per_line(0), pixels_per_width(0),
		show_grid_lines(false), show_selection(true), show_header(true), hilite_odd_lines(true),
		current_rect(), view_rect() {}
	void					correction();
	void					correction_width();
	virtual void			ensurevisible(); // Ånsure that current selected item was visible on screen if current 'count' is count of items per line
	int						find(int line, int column, const char* name, int lenght = -1) const;
	virtual int				getcolumn() const { return 0; } // Get current column
	virtual int				getident() const { return 6 * 2 + 4; } // get row ident in levels
	virtual int				getlevel(int index) const { return 0; } // get row ident in levels
	inline int				getline() const { return current; } // get current line
	int						getlinesperpage(int height) const { return height / pixels_per_line; }
	virtual const char*		getname(char* result, const char* result_max, int line, int column) const { return 0; }
	virtual int				getmaximum() const { return 0; }
	virtual int				getmaximumwidth() const { return 0; }
	static int				getrowheight(); // Get default row height for any List Control
	virtual int				getwidth(int column) const { return 0; } // Get column width
	void					hilight(const rect& rc, const rect* prc = 0) const;
	virtual bool			isgroup(int index) const { return false; }
	bool					isopen(int index) const;
	bool					keyinput(unsigned id) override;
	void					mousehiliting(const rect& rc, point mouse);
	virtual void			mouseinput(unsigned id, point position) override;
	virtual void			mouseselect(int id, bool pressed);
	virtual void			mousewheel(unsigned id, point position, int step) override;
	void					redraw() override;
	virtual void			row(const rect& rc, int index);
	virtual int				rowheader(const rect& rc) const { return 0; }
	virtual void			rowhilite(const rect& rc, int index) const;
	virtual void			select(int index, int column);
	void					treemark(rect rc, int index, int level) const;
	virtual bool			treemarking(bool run) { return true; }
	void					view(const rect& rc) override;
};
struct picker : list {
	int						pixels_per_column, elements_per_line;
	constexpr picker() : pixels_per_column(64), elements_per_line(0) {}
	void					ensurevisible();
	bool					keyinput(unsigned id);
	void					mousewheel(unsigned id, point position, int step);
	void					view(const rect& rc) override;
};
struct table : list {
	const bsreq*			type;
	const markup*			columns;
	int						minimum_width, column_count, resize_column_count;
	constexpr table(const markup* columns, const bsreq* type) : columns(columns), type(type),
		minimum_width(0), column_count(0), resize_column_count(0) {}
	virtual image_flag_s	getalign(int column) const;
	virtual const char*		getheader(char* result, const char* result_max, int column) const;
	const char*				getname(char* result, const char* result_end, int line, int column) const override;
	virtual void*			getrow(int index) const { return 0; }
	void					row(const rect &rc, int index) override;
	int						rowheader(const rect& rc) const { return 0; }
	void					view(const rect& rc) override;
};
struct listtable : table {
	void*					source;
	unsigned				count;
	unsigned				size;
	constexpr listtable(void* source, unsigned count, unsigned size, const bsreq* type, const markup* columns) : table(columns, type),
		source(source), count(count), size(size) {}
	int						getmaximum() const override { return count; }
	void*					getrow(int index) const override { return (char*)source + index*size; }
};
struct reftable : table {
	void**					source;
	unsigned				count;
	constexpr reftable(void** source, unsigned count, const bsreq* type, const markup* columns) : table(columns, type),
		source(source), count(count) {}
	int						getmaximum() const override { return count; }
	void*					getrow(int index) const override { return source[index]; }
};
struct scrollable : control {
	pointl					origin;
	pointl					maximum;
	point					wheels;
	scrollable();
	rect					centerview(const rect& rc);
	virtual void			invalidate();
	virtual void			redraw(rect rc) {}
	void					view(const rect& rc) override;
};
class textedit : public scrollable {
	char*					string;
	unsigned				maxlenght;
	int						cashed_width;
	int						cashed_string;
	int						cashed_origin;
	int						p1, p2;
public:
	rect					rctext, rcclient;
	unsigned				align;
	bool					readonly;
	//
	textedit(char* string, unsigned maximum_lenght, bool select_text);
	//
	void					clear();
	virtual void			cashing(rect rc);
	unsigned				copy(bool run);
	void					correct();
	void					ensurevisible(int linenumber);
	int						hittest(rect rc, point pt, unsigned state) const;
	void					invalidate() override;
	bool					keyinput(unsigned id) override;
	int						lineb(int index) const;
	int						linee(int index) const;
	int						linen(int index) const;
	void					left(bool shift, bool ctrl);
	int						getbegin() const;
	int						getend() const;
	point					getpos(rect rc, int index, unsigned state) const;
	unsigned				paste(bool run);
	void					paste(const char* string);
	void					redraw(rect rc) override;
	void					right(bool shift, bool ctrl);
	void					select(int index, bool shift);
	unsigned				select_all(bool run);
	void					setcount(unsigned v) { maxlenght = v; }
};
}
namespace dialog {
bool						color(struct color& result, struct color* custom = 0);
bool						folder(const char* title, char* path);
bool						open(const char* title, char* path, const char* filter, int filter_index = 0, const char* ext = 0);
bool						save(const char* title, char* path, const char* filter, int filter_index = 0);
}
int							button(int x, int y, int width, unsigned flags, const runable& cmd, const char* label, const char* tips = 0, int key = 0);
int							button(int x, int y, const char* string, const runable& ev, unsigned key, bool checked = false);
int							checkbox(int x, int y, int width, bool& value, const char* label, const char* tips);
int							checkbox(int x, int y, int width, unsigned flags, const runable& cmd, const char* label, const char* tips);
bool						dropdown(const rect& rc, draw::controls::control& e, bool choose_mode = false);
bool						edit(const char* title, void* object, unsigned size, const bsreq* type, const markup* elements, bool creating);
bool						edit(const char* title, void* source, unsigned size, unsigned& count, unsigned maximum, const bsreq* meta, const markup* elements, void* object, void* copy_object);
void						field(const rect& rco, unsigned flags, const anyval& ev, int digits, field_type_s type);
int							field(int x, int y, int width, const markup* elements, const bsval& source, int title_width = 128);
int							field(int x, int y, int width, const char* header_label, const char*& ev, int header_width);
int							field(int x, int y, int width, const char* header_label, const anyval& ev, int header_width, int digits);
void						open(const char* name, void* source, unsigned size, unsigned& count, unsigned maxcount, const bsreq* meta, const markup* columns, const markup* element);
void						pagefooter(int& x, int& y, bool allow_cancel = false);
void						pageheader(int& x, int& y, const char* title_prefix, const char* title, const char* page_title, int page = 0, int page_maximum = 0);
int							radio(int x, int y, int width, unsigned flags, const runable& cmd, const char* label, const char* tips);
void						setposition(int& x, int& y, int& width, int padding = -1);
}