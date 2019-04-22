#include "draw_control.h"

void draw::controls::control::mouseinput(unsigned id, point position) {
	switch(id) {
	case MouseLeft:
		if(isfocusable())
			setfocus((int)this, true);
		break;
	}
}