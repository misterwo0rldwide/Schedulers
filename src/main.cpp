#include <iostream>
#include <string>

int main(void) {
	std::string itzik;
	std::cout << "What's your name: ";
	std::cin >> itzik;
	
	std::cout << "Oh cool, your name is " << itzik << std::endl;
	return 0;
}