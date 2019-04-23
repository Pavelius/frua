#include "main.h"

const char* enum_info::get(int index) const {
	if(index == -1 || index<i1 || index>i2)
		return "Нет данных";
	return ((name_info*)((char*)data + size * index))->name;
}

const name_info* enum_info::getni(int index) const {
	if(index == -1 || index<i1 || index>i2)
		return 0;
	return ((name_info*)((char*)data + size * index));
}