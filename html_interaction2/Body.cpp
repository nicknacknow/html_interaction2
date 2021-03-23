#include "Body.h"
#include "Utility.h"

// struct Element

Element::Element(const char* content, int& pos) {
	if (!eq_char(content[pos], OPEN_TAG)) throw("Incorrect Element Position"); this->start[0] = pos++;
	
	static auto get_tag = [](const char* content, int pos) { // lambda function just to make code look tidier- name definition
		return util::read_til_char(content, pos, " >");
	};

	static auto get_empty = [](const char* tag) {
		return util::is_empty_element(tag);
	};

	this->tag = get_tag(content, pos);
	this->isempty = get_empty(this->tag);
	pos += strlen(this->tag) + 1; // pos is now the char after space after tag
}

// struct write_config

write_config::write_config(AttributeType T, bool e) {
	this->type = T;
	this->enabled = e;
	this->val = Value();

	switch (this->type) {
		case STRING_TYPE:
			this->val.string_val = (char*)"";
			break;
		case NUMBER_TYPE:
			this->val.number_val = 0;
			break;
		case BOOL_TYPE:
			this->val.bool_val = false;
			break;
		case CHAR_TYPE:
			this->val.char_val = '\x0';
			break;
	}
}

/*write_config::operator bool() const {
	printf("ok brouh.\n");
	return this->enabled;
}*/

Value* write_config::get() {
	return &this->val;
}

void write_config::set(Value v) {
	this->val = v;
}

template <class T> // this is only here so i can have any type for input
void write_config::set(T v) {
	switch (this->type) {
		case STRING_TYPE:
			setsvalue(this, v);
			break;
		case NUMBER_TYPE:
			setnvalue(this, v);
			break;
		case BOOL_TYPE:
			setbvalue(this, v);
			break;
		case CHAR_TYPE:
			setcvalue(this, v); // can be treated as byte etc
			break;
	}
}

void write_config::appendchar(char c) {
	int len = strlen(this->val.string_val);
	
	char* n = new char[len + 1];

	strcpy(n, this->get()->string_val);
	n[len] = c;
	n[len + 1] = '\x0';

	this->get()->string_val = n;
}

void write_config::appendstring(const char* c) {
	if (this->type != STRING_TYPE) throw("Config Type must be a String");

	int len = strlen(this->val.string_val);
	int clen = strlen(c);

	char* n = new char[len + clen + 1];

	strcpy(n, this->get()->string_val);
	strcat(n, c);
	strcat(n, "\x0");

	this->get()->string_val = n;
}


// class ParseString

char* ParseString::parse(const char* content, int pos) {
	write_config current_string(STRING_TYPE, true);
	write_config string_char(CHAR_TYPE);
}

bool ParseString::step(const char* content, int pos) {
	char c = content[pos];

	if ((string_char->enabled) && (!eq_char(c, pcvalue(string_char)))) { current_string->appendchar(c); return false; }
	if ((!string_char->enabled) && (eq_char(c, '"') || eq_char(c, '\''))) { string_char->enabled = true; string_char->set(c); return false; }
	if ((string_char->enabled) && (eq_char(c, pcvalue(string_char)) && !eq_char(content[pos - 1], '\\'))) { string_char->enabled = false; return true; }
	
	return false;
}

write_config* ParseString::poststep() {
	write_config cfg = *this->get_string();
	this->clear_string();
	return &cfg; // make sure to 3 this #3write_config
}

write_config* ParseString::get_string() {
	return this->current_string;
}

write_config* ParseString::get_char() {
	return this->string_char;
}

void ParseString::clear_string() {
	this->get_string()->set("");
}

bool ParseString::enabled() {
	return this->get_char()->enabled;
}

// class ParseElement

bool ParseElement::step(const char* content, int pos) {
	char c = content[pos];

	if (eq_char(c, OPEN_TAG) && !eq_char(content[pos + 1], EXCLAM)) { // begin element
		this->current_element = new Element(content, pos); // pos will now be the space
		//printf("%c %c\n", content[pos - 1], content[pos + 1]);

		write_config* atr_cfg = this->parse(content, pos); // this->attribute_cfg

		printf("attributecontent: %s\n", psvalue(atr_cfg));

		this->element_finished = false;
		return true;
	}
	
	if (eq_char(c, END_TAG) && this->get_element()) {
		// enable bool for below func
		// check func to see if rlly is end of func

		printf(" end tag\n");

		this->element_finished = true;
		return true;
	}

	return false;
}

Element* ParseElement::poststep() {
	Element e = *this->get_element();
	//this->reset_element();
	return &e;
}

Element* ParseElement::get_element() {
	return this->current_element;
}

void ParseElement::reset_element() {
	//delete[] current_element;
	current_element = new Element; // this doesnt matter much since u just reset it when first found
}

write_config* ParseElement::parse(const char* content, int pos) {
	ParseString parse_string{};

	for (int i = pos; i < strlen(content); i++) {
		char c = content[i];

		if (parse_string.step(content, i)) { // if its in a string
			attribute_cfg->appendchar(pcvalue(parse_string.get_char()));
			attribute_cfg->appendstring(psvalue(parse_string.poststep()));
			attribute_cfg->appendchar(pcvalue(parse_string.get_char()));
			continue;
		}

		if (!parse_string.enabled()) {
			printf("lolo: %c\n", content[i]);
			if (this->step(content, i)) { // (this means current char is opening of element) check func for step if element just ended, and whether or not to continue parse content. stop writing chars and ez
				printf("ParseElement->element_finished: %s\n", this->element_finished ? "true" : "false");

				if (this->element_finished) {
					this->get_element()->start[1] = i;
					
				}
				else {
					attribute_cfg->appendstring(psvalue(attribute_cfg));
				}
			}

			attribute_cfg->appendchar(c);
		}
	}

	return attribute_cfg;
}

// class Body

Body::Body() {}
Body::Body(const char* site) { Body::Body(); this->setBody(site); }

void Body::setBody(const char* site) {
	if (memcmp(site, "http", 4)) {
		if (memcmp(site, "www", 3)) { // string 'site' is just content of site
			this->content = site;
			return;
		}
		else {} // add http to 'site' string
	}
	// site should be a correct link now so just send request and ez
}

void Body::parseBody() {
	const char* content = this->content;

	ParseString parse_string{};
	ParseElement parse_element{};

	char** written_strings = new char*;
	int written_strings_size = 0;

 	for (int i = 0; i < strlen(content); i++) {
		char c = content[i];

		if (!parse_string.enabled() && parse_element.step(content, i)) {
			Element* e = parse_element.poststep();
			
			printf("new element: %s\n", e->tag);
		}

		if (parse_string.step(content, i)) {
			write_config* string = parse_string.poststep();
			printf("%c\n", pcvalue(parse_string.get_char()));
			written_strings[written_strings_size++] = psvalue(string);
		}

		if (eq_char(c, OPEN_TAG) && !eq_char(content[i+1], '/')) {}
	}
	
	if (written_strings_size) {
		printf("written_strings (%d) : \n{\n", written_strings_size);
		for (int i = 0; i < written_strings_size; i++)
			printf("	%s\n", written_strings[i]);
		printf("}\n");
	}
}