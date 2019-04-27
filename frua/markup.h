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
	};
	constexpr explicit operator bool() const { return title || value.id || value.child; }
	char				width;
	const char*			title;
	element				value;
	int					param;
	proci				proc;
	bool				isfield() const { return value.id != 0; }
};