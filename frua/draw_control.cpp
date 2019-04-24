#include "draw_control.h"

using namespace draw;
using namespace draw::controls;

void control::mouseinput(unsigned id, point position) {
	switch(id) {
	case MouseLeft:
		if(isfocusable())
			setfocus((int)this, true);
		break;
	}
}