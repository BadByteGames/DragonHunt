#pragma once
#include "json.hpp" //json parsing
#include <iostream> //cout, endl
#include <string> //getline
#include <stdlib.h> //getenv_s
#include <stdio.h> //printf
#include <vector> //vector
#include <unordered_map> //unordered map
#include <exception> //exception
#include <fstream> //ofstream, ifstream

#include "json.hpp" //json stuff

//define the OS for crossplatform integration
#ifndef EASYCMD
#if defined(WIN32) || defined(_WIN32)
#define EASYCMD
#define EASYCMDWINDOWS
#endif
//error codes for commands
#define EASYCMD_OK 0
#define EASYCMD_BADARGS 1
#define EASYCMD_FAILED 2
//user errors
#define EASYCMD_BADCOMMAND 3
//library errors
#define EASYCMD_INTERNAL 4
#define EASYCMD_INITFAILED 5
#endif // !EASYCMD

//windows specific includes
#ifdef EASYCMDWINDOWS
#include <Windows.h>
#include <ShlObj.h>
#endif

namespace EasyCmd {

	class InvalidArgumentName : public std::exception {
	public:
		InvalidArgumentName(std::string argumentName):m_argumentName("Invalid attempt to access argument "+argumentName) {}

		virtual const char * what() const throw()
		{
			return m_argumentName.c_str();
		}
	private:
		std::string m_argumentName;
	};

	class InvalidArgumentType: public std::exception {
	public:
		InvalidArgumentType(std::string argumentName) :m_argumentName("Incorrect call to get value of " + argumentName) {}

		virtual const char * what() const throw()
		{
			return m_argumentName.c_str();
		}
	private:
		std::string m_argumentName;
	};

	class GlobalParametersDisabled : public std::exception {
		virtual const char * what() const throw()
		{
			return "Attempted to get a global parameter when UseGlobalConfig disabled";
		}
	};

	struct Version {
		Version() {}
		Version(int major, int minor, int patch) :major(major), minor(minor), patch(patch) {}
		int major;
		int minor;
		int patch;
	};

	struct AppInfo {
		AppInfo() {
			AppName = "";
			AppDataFolder = "";
			UseGlobalConfig = true;
			OptionPrefix = "-";
			SegmentSeperator = "--";
			BasicUtilities = true;
			version = Version(0, 0, 0);
		}
		//must be set, the name of the app
		std::string AppName;
		//if set, name of folder in appdata to store globals
		std::string AppDataFolder;
		bool UseGlobalConfig;

		//special note on command parsing, when the first option is declared, segments end;
		//stuff for command parsing
		//prefix of options
		std::string OptionPrefix;
		//ends a segment and starts a new one
		std::string SegmentSeperator;

		//add basic commands such as 'help' and 'version'
		bool BasicUtilities;

		Version version;
	};

	enum class OPTIONTYPE {
		FLAG,
		STRING,
		NUMBER,
		ARRAY
	};

	struct Option{
		OPTIONTYPE type;
		bool bvalue;
		std::string svalue;
		int ivalue;
		std::vector<std::string> avalue;
	};

	struct CommandInfo {
		std::string name;
		std::string description;
	};

	//global variable management using json
	class Globals {
	public:
		Globals(){}
		
		void init(std::string installDir) {
			std::string jsonFileName = installDir + "\\globals.json";
			//check if file already exists by opening ifstream
			std::ifstream globalsFile;
			globalsFile.open(jsonFileName, std::ifstream::in);
			if (!globalsFile.good()) {
				globalsFile.close();
				//install
				install(installDir);
			}
			else {
				globalsFile.close();
			}
		}
		
		//adds parameter
		void setGlobalParameter(std::string name, std::string value, std::string installDir) {
			//double check user installed
			init(installDir);
			
			nlohmann::json j = getGlobalJson(installDir);
			j["globals"][name] = value;
			std::ofstream o(installDir + "\\globals.json");
			o << j.dump(4);
			o.close();
		}

		//gets parameter
		std::string getGlobalParameter(std::string name, std::string installDir) {
			//double check user installed
			init(installDir);

			nlohmann::json j = getGlobalJson(installDir);
			std::string retval = "";
			try {
				retval = j["globals"][name].get<std::string>();
			}
			catch (std::exception& e) {
				//call it to remove warnings
				e.what();
				//parameter doesn't exist
				setGlobalParameter(name, "", installDir);
				retval = "";
			}
			return retval;
		}	

	private:
		nlohmann::json getGlobalJson(std::string installDir) {
			std::string globalFileName = installDir + "\\globals.json";
			std::ifstream globals(globalFileName);
			nlohmann::json j;
			globals >> j;
			globals.close();
			return j;
		}

		void install(std::string installDir) {
			nlohmann::json j;
			j["globals"] = nlohmann::json::object();
			createDirectory(installDir);
			std::ofstream globals(installDir + "\\globals.json");
			globals << j.dump(4);
			globals.close();
		}

		void createDirectory(std::string directory) {
#ifdef EASYCMDWINDOWS
			if (!SUCCEEDED(CreateDirectory(directory.c_str(), NULL))) {
				std::cout << "error creating " << directory << ": " << GetLastError();
			}
#endif
		}
	};

	//command for user to implement
	class CMD {
	friend class CMDHandler;
	public:
		CMD(){}

		virtual int execute() = 0;

		virtual int cleanup() {
			return EASYCMD_OK;
		}

		CommandInfo getCommandInfo() const {
			return m_info;
		}

	private:
		void cmdInit(AppInfo appInfo, std::string InstallDir, Globals* globals, CommandInfo info) {
			this->m_appInfo = appInfo;
			this->m_installDir = InstallDir;
			this->m_globals = globals;
			this->m_info = info;
		}

		std::string m_installDir;
		Globals* m_globals;

		std::unordered_map<std::string, Option> m_options;

		CommandInfo m_info;

		std::vector<std::string> m_rawArgs;
		std::vector<std::vector<std::string>> m_segments;

	protected:
		void addOption(std::string name, OPTIONTYPE type) {
			Option o;
			o.type = type;
			switch (o.type) {
			case OPTIONTYPE::FLAG:
				o.bvalue = false;
				break;
			case OPTIONTYPE::STRING:
				o.svalue = "";
				break;
			case OPTIONTYPE::NUMBER:
				o.ivalue = 0;
				break;
			}

			m_options.insert(std::make_pair(name, o));
		}

		void addOption(std::string name, const char * defaultValue) {
			Option o;
			o.type = OPTIONTYPE::STRING;
			o.svalue = defaultValue;

			m_options.insert(std::make_pair(name, o));
		}

		void addOption(std::string name, int defaultValue) {
			Option o;
			o.type = OPTIONTYPE::NUMBER;
			o.ivalue = defaultValue;

			m_options.insert(std::make_pair(name, o));
		}

		std::vector<std::string> getRawArgs() const { return m_rawArgs; }

		void setGlobalParameter(std::string name, std::string value) {
			if(m_appInfo.UseGlobalConfig)
				m_globals->setGlobalParameter(name, value, m_installDir);
			else throw GlobalParametersDisabled();
		}

		std::string getGlobalParameter(std::string name){
			if (m_appInfo.UseGlobalConfig)
				return m_globals->getGlobalParameter(name, m_installDir);
			else throw GlobalParametersDisabled();
			return "";
		}

		bool getFlag(std::string name) {
			auto it = m_options.find(name);
			//check for user errors
			if (it == m_options.end()) throw InvalidArgumentName(name);
			if (it->second.type != OPTIONTYPE::FLAG) throw InvalidArgumentType(name);

			return it->second.bvalue;
		}

		std::string getString(std::string name) {
			auto it = m_options.find(name);
			//check for user errors
			if (it == m_options.end()) throw InvalidArgumentName(name);
			if (it->second.type != OPTIONTYPE::STRING) throw InvalidArgumentType(name);

			return it->second.svalue;
		}

		int getNumber(std::string name) {
			auto it = m_options.find(name);
			//check for user errors
			if (it == m_options.end()) throw InvalidArgumentName(name);
			if (it->second.type != OPTIONTYPE::NUMBER) throw InvalidArgumentType(name);

			return it->second.ivalue;
		}

		std::vector<std::string> getArray(std::string name) {
			auto it = m_options.find(name);
			//check for user errors
			if (it == m_options.end()) throw InvalidArgumentName(name);
			if (it->second.type != OPTIONTYPE::ARRAY) throw InvalidArgumentType(name);

			return it->second.avalue;
		}

		int segmentAmount() {
			return m_segments.size();
		}

		//get argument segments
		std::vector<std::string> getSegment(int index) {
			return m_segments[index];
		}

		AppInfo m_appInfo;
	};

	//event class for when a function is added
	class CMDAddedEvent {
	public:
		CMDAddedEvent() {}

		virtual void OnCommandAdded(CMD* command) = 0;

		std::vector<CommandInfo> getCmdInfo() const { return m_cmdInfo; }
	protected:
		std::vector<CommandInfo> m_cmdInfo;
	};

	//command to call if user didn't specify (clone of version)
	class DefaultMain:public CMD {
	public:
		DefaultMain() {}

		int execute() {
			std::cout << this->m_appInfo.AppName << " " << this->m_appInfo.version.major << "." << this->m_appInfo.version.minor << "." << this->m_appInfo.version.patch << std::endl;
			return EASYCMD_OK;
		}
	};

	//default add event that tracks commands for help()
	class DefaultAdd :public CMDAddedEvent {
	public:
		DefaultAdd(){}

		virtual void OnCommandAdded(CMD* command) {
			addHelpInfo(command);
		}

	protected:
		//command called for default help command
		void addHelpInfo(CMD* command) {
			m_cmdInfo.push_back(command->getCommandInfo());
		}
	};

	//basic version command
	class VersionCMD: public CMD {
	public:
		VersionCMD() {}

		int execute() {
			std::cout << this->m_appInfo.AppName <<" "<< this->m_appInfo.version.major<<"." << this->m_appInfo.version.minor << "." << this->m_appInfo.version.patch<< std::endl;
			return EASYCMD_OK;
		}
	};

	//basic help command
	class Help :public CMD {
	public:
		Help(CMDAddedEvent* added) :m_added(added){}

		virtual int execute() {
			for (auto c : m_added->getCmdInfo()) {
				if (c.description != "") {
					std::cout << c.name << ": " << c.description << std::endl;
				}
				else {
					std::cout << c.name << ": No Description Available" << std::endl;
				}
			}
			std::cout << "size: "<< m_added->getCmdInfo().size() << std::endl;
			return EASYCMD_OK;
		}

	private:
		CMDAddedEvent * m_added;
	};

	//basic config command
	class Config:public CMD {
	public:
		Config(){
			addOption("v", OPTIONTYPE::STRING);
		}

		virtual int execute() {
			if (getSegment(0).size() < 3) return EASYCMD_BADARGS;
			//if v set, change parameter, else change value
			if (getString("v") == "")
				std::cout << getGlobalParameter(getSegment(0)[2]) << std::endl;
			else
				setGlobalParameter(getSegment(0)[2], getString("v"));
			return EASYCMD_OK;
		}

	};
	

	//class for handling commands
	class CMDHandler {
	public:
		CMDHandler(AppInfo inf):m_info(inf){
			this->setDefaultCommand(new DefaultMain());
			this->setAddedEvent(new DefaultAdd());
			if (inf.BasicUtilities) {
				this->addCommand("version", new VersionCMD());
				this->addCommand("help", new Help(this->m_added));
				//add "config" command if UseGlobalConfig enabled
				if (inf.UseGlobalConfig) {
					this->addCommand("config", new Config());
				}
			}

			if (inf.UseGlobalConfig) {
				m_globals.init(getInstallDir());
			}
		}

		int addCommand(std::string name, CMD* command) {
			//won't show up in "help"
			CommandInfo inf;
			inf.description = "";
			inf.name = name;
			return addCommand(name, command, inf);
		}

		int addCommand(std::string name, CMD* command, CommandInfo info) {
			command->cmdInit(this->m_info, getInstallDir(), &m_globals, info);
			this->m_commands.insert(std::make_pair(name, command));
			m_added->OnCommandAdded(command);
			return EASYCMD_OK;
		}

		int setDefaultCommand(CMD* command) {
			delete this->m_mainCommand;
			command->cmdInit(this->m_info, getInstallDir(), &m_globals,CommandInfo());
			this->m_mainCommand = command;
			return EASYCMD_OK;
		}

		int setAddedEvent(CMDAddedEvent* eventHandler) {
			//delete the previouse thing
			delete this->m_added;
			this->m_added = eventHandler;
			return EASYCMD_OK;
		}

		int parseCommand(int argc, char** argv){
			int result = EASYCMD_INTERNAL;
			if (argc == 1) {
				//call basic command
				if (populateVariables(argc, argv, m_mainCommand)) {
					std::cout << "Bad arguments" << std::endl;
					return EASYCMD_BADARGS;
				}
				result = m_mainCommand->execute();
			}
			else {
				auto it = m_commands.find(argv[1]);
				if (it != m_commands.end()) {
					if (populateVariables(argc, argv, it->second)) {
						std::cout << "Bad arguments" << std::endl;
						return EASYCMD_BADARGS;
					}
					result = it->second->execute();
				}
				else {
					if (populateVariables(argc, argv, m_mainCommand)) {
						std::cout << "Bad arguments" << std::endl;
						return EASYCMD_BADARGS;
					}
					result = m_mainCommand->execute();
				}
			}
			return result;
		}

		/*
		*Cleanup command
		*Call ONLY if you don't want to manage pointers
		*/
		int cleanup() {
			m_mainCommand->cleanup();
			delete m_mainCommand;
			for (auto c : m_commands) {
				c.second->cleanup();
				delete c.second;
			}
			return EASYCMD_OK;
		}

		AppInfo GetAppInfo() const { return m_info; }
		std::unordered_map<std::string, CMD*> GetCommands() const { return m_commands; }

	private:
		//actual parsing
		int populateVariables(int argc, char** argv, CMD* command) {

			//add raw arguments
			for (int i = 0; i < argc; i++)
			{
				command->m_rawArgs.push_back(argv[i]);
			}

			bool segmentsEnded = false;
			bool inArray = false;

			std::vector<std::string> currentSegment;

			int firstOption = -1;

			//check for all the options the command has as well as segments
			if(command->m_options.size() > 0){
				//for use of array options
				Option* currentArrayOption = nullptr;
				//loop through arguments
				for (int i = 0; i < argc; i++) {
					for (auto& o : command->m_options) {
						if (command->m_appInfo.OptionPrefix + o.first == argv[i]) {
							inArray = false;
							if (firstOption == -1)
								firstOption = i;
							//change o's value based off type
							switch (o.second.type) {
							case OPTIONTYPE::FLAG:
								o.second.bvalue = true;
								break;
							case OPTIONTYPE::STRING:
								if (argc < i) return EASYCMD_BADARGS;
								o.second.svalue = argv[i + 1];
								i++;
								break;
							case OPTIONTYPE::NUMBER:
								if (argc < i) return EASYCMD_BADARGS;
								o.second.ivalue = atoi(argv[i + 1]);
								i++;
								break;
							case OPTIONTYPE::ARRAY:
								if (argc < i) return EASYCMD_BADARGS;
								i++;
								currentArrayOption = &o.second;
								inArray = true;
								break;
							}
						}
					}
					if (inArray) {
						currentArrayOption->avalue.push_back(argv[i]);
					}
				}
				//loop through segments now
				if(firstOption != -1)
					//just go through segments
					for (int i = 0; i < firstOption; i++) {
						if (command->m_appInfo.SegmentSeperator == argv[i]) {
							command->m_segments.push_back(currentSegment);
							currentSegment.clear();
						}
						else {
							currentSegment.push_back(argv[i]);
						}
					}
				else
					for (int i = 0; i < argc; i++) {
						if (command->m_appInfo.SegmentSeperator == argv[i]) {
							command->m_segments.push_back(currentSegment);
							currentSegment.clear();
						}
						else {
							currentSegment.push_back(argv[i]);
						}
					}
			}
			else {
				//just go through segments
				for (int i = 0; i < argc; i++) {
					if (command->m_appInfo.SegmentSeperator == argv[i]) {
						command->m_segments.push_back(currentSegment);
						currentSegment.clear();
					}
					else {
						currentSegment.push_back(argv[i]);
					}
				}
			}
			//terminate segments just in case
			if (!segmentsEnded) {
				command->m_segments.push_back(currentSegment);
				currentSegment.clear();
				segmentsEnded = true;
			}
			return EASYCMD_OK;
		}

		CMDAddedEvent* m_added;

		std::unordered_map<std::string, CMD*> m_commands;
		CMD* m_mainCommand;
		AppInfo m_info;

		Globals m_globals;

		std::string getInstallDir() {
#ifdef EASYCMDWINDOWS
			LPWSTR InstallDir;
			if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &InstallDir))) {
				std::cout << "Error getting location of appdata: " << GetLastError() << std::endl;
				CoTaskMemFree(InstallDir);
				return "";
			}
			std::string appdata = toAscii(InstallDir);
			CoTaskMemFree(InstallDir);
			appdata.append("\\");
			if (m_info.AppDataFolder == "")
				appdata.append(m_info.AppName);
			else
				appdata.append(m_info.AppDataFolder);

			return appdata;
#endif
			return "";
		}

		std::string toAscii(const std::wstring &wstr) {
#ifdef EASYCMDWINDOWS
			std::string strTo;
			char *szTo = new char[wstr.length() + 1];
			szTo[wstr.size()] = '\0';
			if (!WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length()+1, NULL, NULL)) {
				std::cout << "Error converting string: " << GetLastError() << std::endl;
			}
			strTo = szTo;
			delete[] szTo;
			return strTo;
#endif
			return "";
		}
	};
}