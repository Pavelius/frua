#pragma once

// Standart markup
struct markup {
	struct element {
		const char*		id; // Field identificator (0 for group)
		int				index; // Array index
		const markup*	child; // Group or next field
	};
	struct proci {
		bool(*isvisible)(const void* object, const markup& e);
		bool(*isallow)(const void* object, int param);
		void(*command)();
	};
	constexpr explicit operator bool() const { return title || value.id || value.child; }
	char			width;
	const char*		title;
	element			value;
	int				param;
	proci			proc;
};