#pragma once

// Standart markup
struct markup {
	struct element {
		const char*		id; // Field identificator (0 for group)
		int				index; // Array index
		const markup*	child; // Group or next field
	};
	struct proci {
		const char* (*getname)(const void* object, char* result, const char* result_maximum, int column);
		int(*getvalue)(const void* object, int column);
		bool(*isallow)(const void* object, int index);
		bool(*isvisible)(const void* object, const markup& e);
		int(*custom)(int x, int y, int width, const char* id, const void* object);
		void(*command)(void* object);
	};
	constexpr explicit operator bool() const { return title || value.id || value.child; }
	char				width;
	const char*			title;
	element				value;
	int					param;
	proci				proc;
	//
	bool				action(const char* id, void* object) const;
	bool				isfield() const { return value.id != 0; }
	int					getcount(const char* id, const void* object, bool need_child) const;
	const markup*		getpage(const void* object, int index) const { return find("page", object, index, true); }
	int					getpagecount(const void* object) const { return getcount("page", object, true); }
	const markup*		find(const char* id, const void* object, int result, bool need_child) const;
	const markup*		findcommands(const void* object) const { return find("commands", object, 0, true); }
};