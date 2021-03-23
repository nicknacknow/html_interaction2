#pragma once
#include <Windows.h>

namespace util {
	bool is_empty_element(const char* name);
	char* read_til_char(const char* content, int pos, char _c); // strlen of return will be where c is found
	char* read_til_char(const char* content, int pos, const char* _c); // 2nd arg is just a list of chars that can be accepted, e.g. aAbBcC
	int find_next_char(const char* content, int pos, char c);
}