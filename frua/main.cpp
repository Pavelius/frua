#include "draw.h"
#include "main.h"

int	main(int argc, char *argv[]) {
	draw::initialize();
	picture_info::pick_monster();
	//character c1;
	//c1.generate();
	return 0;
}

void main_unit();

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((int)time(0));
	//srand(101);
	main_unit();
	return main(0, 0);
}