#include "main.h"

bool design_info::edit(void* object, void* copy_object, bool run) {
	auto result = true;
	if(run) {
		char copy_buffer[256] = {};
		auto copy = copy_buffer;
		auto size = source.size;
		if(size > sizeof(copy_buffer))
			copy = new char[size];
		if(object)
			memcpy(copy, object, size);
		else if(copy_object)
			memcpy(copy, copy_object, size);
		else
			creating(copy);
		if(change(copy)) {
			if(!object)
				object = source.add();
			if(object)
				memcpy(object, copy, size);
		} else
			result = false;
		if(copy != copy_buffer)
			delete copy;
	}
	return result;
}