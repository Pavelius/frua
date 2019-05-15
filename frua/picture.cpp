#include "main.h"

const bsreq bsmeta<picture_info>::meta[] = {
	BSREQ(id),
	BSREQ(folder),
	BSREQ(position),
{}};

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

void picture_info::correct(int xmax, int ymax, int width, int height) {
	if(position.x + xmax > width)
		position.x = width - xmax;
	if(position.y + ymax > height)
		position.y = height - ymax;
	if(position.x < 0)
		position.x = 0;
	if(position.y < 0)
		position.y = 0;
}