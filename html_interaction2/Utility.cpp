#include "Utility.h"
#include "Body.h"

bool util::is_empty_element(const char* name) {
	static const char* empty_element_names[EMPTY_ELEMENT_SIZE] = { "area", "base", "br", "col", "embed", "hr", "img", "input", "keygen", "link", "meta", "param", "source", "track", "wbr", "!doctype" };
	char* c = new char[strlen(name)];
	strcpy(c, name);

	for (int i = 0; i < EMPTY_ELEMENT_SIZE; i++)
		if (!strcmp(_strlwr(c), empty_element_names[i])) return true;

	return false;
}

char* util::read_til_char(const char* content, int pos, char _c) {
	char* n = (char*)"";

	for (int i = pos; i < strlen(content); i++) {
		char c = content[i];

		if (eq_char(c, _c)) { strcat(n, "\x0"); break; }

		int len = strlen(n);
		char* l = new char[len + 1];
		strcpy(l, n);
		l[len] = c;
		l[len + 1] = '\x0';
		n = l;
	}
	return n;
}

int util::find_next_char(const char* content, int pos, char c) {
	for (int i = pos; i < strlen(content); i++) {
		if (eq_char(content[pos], c)) return i;
	}
	return -1;
}