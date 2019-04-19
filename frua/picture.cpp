#include "main.h"

bool picture_info::operator==(const picture_info& e) const {
	return strcmp(e.folder, folder) == 0 && strcmp(e.id, id) == 0;
}

const char* picture_info::geturl(char* temp) const {
	return szprint(temp, temp + 260, "%1/%2.png", folder, id);
}