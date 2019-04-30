#pragma once

// Standart markup
struct markup {
	typedef const char* (*text_type)(const void* object, char* result, const char* result_maximum, int column);
	typedef int(*num_type)(const void* object, int column);
	typedef bool(*allow_type)(const void* object, int index);
	typedef int(*custom_type)(int x, int y, int width, const void* object, const char* id, int index); // Custom draw
	typedef void(*command_type)(void* object);
	typedef void(*change_type)(void* object, const void* previous_object);
	struct element {
		const char*		id; // Field identificator (0 for group)
		int				index; // Array index
		const markup*	child; // Group or next field
	};
	struct proci {
		text_type		getname;
		num_type		getvalue;
		allow_type		isallow; // Is allow special element or command
		allow_type		isvisible; // Is element visible
		custom_type		custom;
		command_type	command; // Any command of class object
		change_type		change; // When object changed
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
	const markup*		getform(const void* object, const char* id) const { return find(id, object, 0, true); }
	const markup*		getpage(const void* object, int index) const { return find("page", object, index, true); }
	int					getpagecount(const void* object) const { return getcount("page", object, true); }
	const markup*		find(const char* id, const void* object, int result, bool need_child) const;
	const markup*		findcommands(const void* object) const { return find("commands", object, 0, true); }
};