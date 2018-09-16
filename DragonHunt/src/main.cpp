#include "Parser.h"
#include "Adventure.h"
#include "Logger.h"
#include "Events.h"

#include "EasyCmd.hpp"

#include <string>
#include <iostream>

std::string getFileFromSite(std::string address) {

	std::cout << "This platform does not support online requests." << std::endl;
	return "";
}

class DefaultDragonHuntCMD : public EasyCmd::CMD {
public:
	DefaultDragonHuntCMD() {
		//example: https://raw.githubusercontent.com/BadByteGames/DragonHunt/releasecandidate/DragonHunt/DragonHunt/exampleworld.xml
		addOption("https", "");
		addOption("file",  "");
	}
	~DefaultDragonHuntCMD() {}

	// Inherited via CMD
	virtual int execute() override
	{
		if (getString("file") != "") {
			//check if file exists
			std::ifstream testFile(getString("file"));
			if (testFile.fail()) {
				std::cout << "The file " << getString("file") << " failed to open." << std::endl;
				return EASYCMD_BADARGS;
			}

			//loads xml from file and starts adventure
			Adventure a;
			a.loadFromFile(getString("file"));
			a.destroy();
		}
		else if (getString("https") != "") {
			//creates a local copy of the file in the https request
			std::string https = getString("https");
			
			size_t fileStart = https.find_last_of("//");

			std::string fileName = "(https)"+https.substr(fileStart+1);

			//now get the value of the string
			std::string fileContents = getFileFromSite(https);
			if (fileContents == "")
				return EASYCMD_BADARGS;
			
			std::ofstream outFile(fileName);
			outFile << fileContents;
			outFile.close();

			//now start the adventure
			Adventure a;
			a.loadFromFile(fileName);
			a.destroy();
		}
		else {
			std::cout << "Usage:\n\t-file <filename>\n\t-https <fileadress>" << std::endl;
			return EASYCMD_BADARGS;
		}

		return EASYCMD_OK;
	}
};

//by adding these two arguments to main, we can use the game from the command line with arguments i.e. "DragonHunt.exe -Debug" 
int main(int argc, char** argv) {
	//start the logger
	Logger::init("runtime.log");
	
	//add some help tips
	Logger::logEvent("help", "errors will have the [error] tag proceeding them (hit ctrl+f to find them)");
	
	std::string args = "";
	for (int i = 0; i < argc; i++) {
		args += argv[i];
		args += " ";
	}
	Logger::logEvent("main", "Arguments: "+args);
	//create app info for easycmd
	EasyCmd::AppInfo inf;
	inf.AppName = "DragonHunt";
	inf.SegmentSeperator = "--";
	inf.OptionPrefix = "-";
	inf.BasicUtilities = false;
	inf.UseGlobalConfig = false;
	inf.version.major = 1;
	inf.version.minor = 0;
	inf.version.patch = 0;
	
	EasyCmd::CMDHandler handler(inf);
	handler.setDefaultCommand(new DefaultDragonHuntCMD());
	handler.parseCommand(argc, argv);
	handler.cleanup();

	Logger::logEvent("main", "program terminated");
	return 0;
}