#include "Parser.h"
#include <string>
#include <iostream>


//by adding these two arguments to main, we can use the game from the command line with arguments i.e. "DragonHunt.exe -Debug" 
int main(int argc, char** argv) {
	//create an instance of a parser
	Parser p;
	std::string input = "";
	//grabs user input
	std::getline(std::cin, input);
	//parse input until exit is typed
	while (input != "exit") {
		std::vector<TOKENDATA> data = p.parse(input);
		//a simple way to loop throug all the entries in a vector
		for (auto d : data) {
			//d has the value of the current entry
			std::cout << d.value << std::endl;
		}
		std::getline(std::cin, input);
	}
}