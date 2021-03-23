#pragma once
#include <Windows.h>
#include <vector>
#include <string>

#define cast(val, type) (reinterpret_cast<type>(val))

#define eq_char(a,b) (a == b)

#define svalue(cf) (cf.get()->string_val)
#define nvalue(cf) (cf.get()->number_val)
#define bvalue(cf) (cf.get()->bool_val)
#define cvalue(cf) (cf.get()->char_val)

#define psvalue(cf) (cf->get()->string_val)
#define pnvalue(cf) (cf->get()->number_val)
#define pbvalue(cf) (cf->get()->bool_val)
#define pcvalue(cf) (cf->get()->char_val)

#define setsvalue(cf, v) (cf->get()->string_val = (char*)v)
#define setnvalue(cf, v) (cf->get()->number_val = *cast(&v, double*))
#define setbvalue(cf, v) (cf->get()->bool_val = (bool)v)
#define setcvalue(cf, v) (cf->get()->char_val = (char)v)

#define EMPTY_ELEMENT_SIZE 16

#define OPEN_TAG '<'
#define END_TAG '>'
#define EXCLAM '!'

enum AttributeType {
	NONE_TYPE,
	STRING_TYPE,
	NUMBER_TYPE,
	BOOL_TYPE,
	CHAR_TYPE
};

union Value {	
	char* string_val;
	double number_val;
	bool bool_val;
	char char_val;
};

struct Attribute {
	const char* name;
	AttributeType type;
	Value val;
};

struct Element { // add Element:parse
	Element() {}
	Element(const char* content, int& pos);

	const char* tag;
	const char* content;
	int start[2], end[2]{};
	bool isempty; // https://developer.mozilla.org/en-US/docs/Glossary/Empty_element

	const char* attribute_string;
	std::vector<Attribute> attributes;
};

struct write_config {
	write_config(AttributeType T, bool e = false);

	//operator bool() {
	//	printf("hi\n");
	//}

	Value* get();
	void set(Value v);

	template <class T> // get any for input
	void set(T v);

	void appendchar(char c);

	void appendstring(const char* c);

	AttributeType type;
	Value val;
	bool enabled;
};

class ParseString {
public:
	char* parse(const char* content, int pos);

	bool step(const char* content, int pos); // return true when end of string so can do ParseString.get() and get string_char

	write_config* poststep();

	write_config* get_string();
	write_config* get_char();
	void clear_string();

	bool enabled();
private:
	write_config* string_char = new write_config(CHAR_TYPE, false);
	write_config* current_string = new write_config(STRING_TYPE, true);
};

class ParseElement {
public:
	bool step(const char* content, int pos);

	Element* poststep();

	Element* get_element();
	void reset_element();

	write_config* parse(const char* content, int pos);
private:
	Element* current_element = new Element;
};

class Body {
public:
	Body();
	Body(const char* site);

	void setBody(const char* site);

	void parseBody();
private:
	const char* content;

	std::vector<Element> elements;
};