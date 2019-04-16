#include "draw.h"
#include "main.h"

int	main(int argc, char *argv[]) {
	answer source[] = {{0, "Персонаж"}, {0, "Магия"}, {0, "Отменить"}};
	draw::initialize();
	character e;
	//e.choose("art/settlements/city_night.png", source);
	e.choose("art/tavern/tavern1.png", source);
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	return main(0, 0);
}