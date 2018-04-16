#include "Parser.h"
#include "Adventure.h"
#include "Logger.h"
#include "Events.h"

#include <EasyCmd\EasyCmd.hpp>

#include <string>
#include <iostream>

#if defined(WIN32) || defined(_WIN32)
#include <winhttp.h>
#endif

std::string getFileFromSite(std::string address) {
#if defined(WIN32) || defined(_WIN32)
	std::string webAddress = "";
	std::string pathToFile = "";
	
	std::wstring WwebAddress = L"";
	std::wstring WpathToFile = L"";

	std::string copy = address;
	//remove https:// from front
	if (copy.find_first_of("https://") != copy.npos) {
		copy.replace(copy.find_first_of("https://"), copy.find_first_of("https://") + 8, "");
	}
	if (copy.find_first_of("/") != copy.npos) {
		webAddress = copy.substr(0, copy.find_first_of("/"));
		pathToFile = copy.substr(copy.find_first_of("/"));
		WpathToFile = std::wstring(pathToFile.begin(), pathToFile.end());
	}
	else {
		webAddress = copy;
	}

	WwebAddress = std::wstring(webAddress.begin(), webAddress.end());

	

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	//ignore these errors, the compile works fine

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"DragonHunt/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, WwebAddress.c_str(),
			INTERNET_DEFAULT_HTTPS_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET",
			WpathToFile.c_str(),
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS, 0,
			WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	std::string outval = "";

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				else {
					outval += pszOutBuffer;
				}
				// Free the memory allocated to the buffer.
				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
	}

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	return outval;
#endif
	return "This platform does not support online requests.";
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
			std::ofstream outFile(fileName);
			outFile << fileContents;
			outFile.close();

			//now start the adventure
			Adventure a;
			a.loadFromFile(fileName);
			a.destroy();
		}
		return 0;
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