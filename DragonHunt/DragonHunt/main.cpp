#include "Parser.h"
#include "Adventure.h"
#include "Logger.h"
#include <string>
#include <iostream>

//by adding these two arguments to main, we can use the game from the command line with arguments i.e. "DragonHunt.exe -Debug" 
int main(int argc, char** argv) {
	//load in xml file
	Adventure a;
	a.loadFromFile("exampleworld.xml");

	//test logger
	Logger l;
	l.init("test.log");

	//get input then quit
	std::string input = "";
	std::getline(std::cin, input);
	return 0;
}