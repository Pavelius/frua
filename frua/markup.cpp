#include "markup.h"

extern "C" int strcmp(const char* s1, const char* s2);

const markup* markup::find(const char* id, const void* object, int result) const {
	if(!this)
		return 0;
	for(auto p = this; *p; p++) {
		if(!p->value.id || p->value.id[0] != '#')
			return 0;
		if(!p->value.child)
			continue;
		if(p->proc.isvisible && !p->proc.isvisible(object, *p))
			continue;
		if(strcmp(p->value.id + 1, id) != 0)
			continue;
		if(--result < 0)
			return p;
	}
	return 0;
}

int	markup::getcount(const char* id, const void* object) const {
	if(!this)
		return 0;
	auto result = 0;
	for(auto p = this; *p; p++) {
		if(!p->value.id || p->value.id[0] != '#')
			return 0;
		if(!p->value.child)
			continue;
		if(p->proc.isvisible && !p->proc.isvisible(object, *p))
			continue;
		if(strcmp(p->value.id + 1, id) != 0)
			continue;
		result++;
	}
	return result;
}