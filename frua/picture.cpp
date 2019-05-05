#include "main.h"

bool picture_info::operator==(const picture_info& e) const {
	return strcmp(e.folder, folder) == 0 && strcmp(e.id, id) == 0;
}

const char* picture_info::geturl(char* temp) const {
	return szprint(temp, temp + 260, "art/%1/%2.png", folder, id);
}

const char* picture_info::geturl(char* temp, int part) const {
	return szprint(temp, temp + 260, "art/%1/%2_%3i.png", folder, id, part);
}

void picture_info::set(const char* folder, const char* id) {
	this->folder = folder;
	this->id = id;
}