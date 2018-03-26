#include "Parser.h"
#include "Adventure.h"
#include "Logger.h"
#include "Events.h"
#include <string>
#include <iostream>

//by adding these two arguments to main, we can use the game from the command line with arguments i.e. "DragonHunt.exe -Debug" 
int main(int argc, char** argv) {
	//start the logger
	Logger::init("runtime.log");
	
	//add some help tips
	Logger::logEvent("help", "errors will have the [error] tag proceeding them (hit ctrl+f to find them)");

	//load in xml file
	Adventure a;
	a.loadFromFile("exampleworld.xml");

	//get input then quit
	std::string input = "";
	std::getline(std::cin, input);
	return 0;
}