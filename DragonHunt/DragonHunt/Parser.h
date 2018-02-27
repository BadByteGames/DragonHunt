#pragma once
#include <string>
#include <vector>
//a list of TOKEN types for every word we will use 
//We're gonna drop excessive things like 'the' for simplicity
enum class TOKEN {
	GO,
	PICKUP,
	DROP,
	OBJECT,
	ENTITY
};

//struct for comparing specific words or phrases to immediate tokens i.e. go = TOKEN::GO
//lowercase
struct TOKENPAIR {
	TOKENPAIR() :result(TOKEN::GO),
		keyword("go") {}

	TOKENPAIR(std::string keyword,TOKEN result):result(result),
												keyword(keyword){}

	std::string keyword;
	TOKEN result;
};

//stores everything about a token and will be return value of parse
struct TOKENDATA {
	//structs can have constructors
	//you can also define multiple methods with the same name if they take different arguments
	TOKENDATA():type(TOKEN::GO),
				value("Go"){}

	//left of the ':' is where you can init variables with arguments
	TOKENDATA(TOKEN type, std::string value): type(type),
											value(value){}

	TOKEN type;
	std::string value;
};

class Parser
{
public:
	Parser();
	~Parser();

	//returns a vector of tokendata
	std::vector<TOKENDATA> parse(std::string toParse);
private:
	//finds if a token exists in a single word
	std::string foundTokenInString(std::vector<std::string> words, int startIndex, TOKENPAIR target);

	//keyword to token lookup
	std::vector<TOKENPAIR> m_keywords;
};

