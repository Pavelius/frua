#include "draw.h"

void draw::scrollv(const rect& scroll, int& origin, int count, int maximum, bool focused) {
	// count - elements per page
	// maximum - maximum elements
	if(!maximum || maximum == count)
		return;
	int ss = (scroll.height()*count) / maximum; // scroll size (in pixels)
	int ds = scroll.height() - ss;
	int dr = maximum - count;
	int p = (origin*ds) / dr + scroll.y1;
	auto a = area(scroll);
	// show scroll area
	if(focused)
		rectf({scroll.x2 - 2, p, scroll.x2 + 2, p + ss}, colors::blue, 128);
}

void draw::scrollh(const rect& scroll, int& origin, int per_page, int maximum, bool focused) {
	if(!maximum)
		return;
	int ss = ((scroll.x2 - scroll.x1)*per_page) / maximum;
	int ds = scroll.x2 - scroll.x1 - ss;
	int dr = maximum - per_page;
	if(!dr)
		return;
	int p = (origin*ds) / dr + scroll.x1;
	if(focused)
		rectf({p, scroll.y2 - 2, p + ss, scroll.y2 + 2}, colors::blue, 128);
}