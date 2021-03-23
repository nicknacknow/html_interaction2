#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>

#include "Body.h"
#include "Utility.h"

class aa {
public:
	const char* cc = "deez";
};

int main() {
	Body body{};

	std::string str = "";
	while (std::getline(std::cin, str)) {
		body.setBody(str.c_str());
		body.parseBody();
	}

	/*char* lol = (char*)"Yo, Whats Up Guys";
	char* aa = new char[strlen(lol)];
	strcpy(aa, lol);
	_strlwr(aa);

	std::cout << lol << aa << std::endl;


	const char* content = "hahahahahahahahlolololol n";

	char* n = util::read_til_char(content, 0, ' ');

	printf("%s (%d)\n", n, strlen(n));


	int cry = 5;
	printf("%d\n\n", cry++);*/

	//write_config cf(STRING_TYPE, true);
	//cf.val.number_val = 5;
	//std::cout << nvalue(cf) << std::endl;

	std::cin.get();
	return 1;
}