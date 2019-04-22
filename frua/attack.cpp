#include "main.h"

char* attack_info::getattacks(char* result, const char* result_maximum) const {
	auto format = "%1i/2";
	auto value = attacks;
	if((value & 1) == 0) {
		value /= 2;
		format = "%1i";
	}
	return szprint(result, result_maximum, format, value);
}