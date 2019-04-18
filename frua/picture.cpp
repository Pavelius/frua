#include "main.h"

bool picture_info::operator==(const picture_info& e) const {
	return strcmp(e.folder, folder) == 0 && strcmp(e.id, id) == 0;
}