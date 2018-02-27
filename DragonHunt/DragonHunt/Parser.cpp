#include "Parser.h"
#include <sstream>
#include <iterator>
#include <iostream>

Parser::Parser()
{
}


Parser::~Parser()
{
}

std::vector<TOKENDATA> Parser::parse(std::string toParse)
{
	//clears m_keywords
	m_keywords.clear();
	//add all keywords to the vector
	//push_back adds new items to the vector
	m_keywords.push_back(TOKENPAIR("go", TOKEN::GO));
	m_keywords.push_back(TOKENPAIR("pick up", TOKEN::PICKUP));
	m_keywords.push_back(TOKENPAIR("drop", TOKEN::DROP));
	m_keywords.push_back(TOKENPAIR("take", TOKEN::TAKE));
	m_keywords.push_back(TOKENPAIR("push", TOKEN::PUSH));
	m_keywords.push_back(TOKENPAIR("help", TOKEN::HELP));
	

	std::vector<TOKENDATA> tokens;
	std::istringstream iss(toParse);

	//seperates string at spaces
	std::vector<std::string> words{ std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{} };

	//loops through tokens to find keywords
	for (int i = 0; i < words.size(); i++) {
		for (auto token : m_keywords) {
			std::string str = foundTokenInString(words, i, token);
			if (str!="")
				tokens.push_back(TOKENDATA(token.result, str));
		}
	}

	return tokens;
}

std::string Parser::foundTokenInString(std::vector<std::string> words, int startIndex, TOKENPAIR target)
{
	std::string returnvalue = "";

	int index = startIndex;

	//find length of token so ENTIRE token is found
	size_t len = target.keyword.length();
	size_t nextPos = 0;

	//find word in token (allows for spaces)
	size_t pos = target.keyword.find(returnLower(words[index]));
	if (pos == nextPos) {
		returnvalue += words[index];
		len -= words[index].length();	
	}

	//check if we've reached the end of the array
	if (index+1== words.size())
		//check if the token was complete
		if (len = 0)
			return returnvalue;
		else
			return "";

	//if the correct position has the next part of the token
	while(pos == nextPos && len != 0) {
		nextPos += words[index].length() + 1;
		//move to next word
		index += 1;
		pos = target.keyword.find(returnLower(words[index]));
		//if word found, add it to the return value
		if (pos == nextPos) {
			returnvalue += " "+words[index];
			nextPos += words[index].length() + 1;
			len -= words[index].length() + 1;
		}
		else
			break;
		
		index += 1;
		if (index == words.size())
			break;
	}

	if (len == 0)
		return returnvalue;
	else
		return "";
}

std::string Parser::returnLower(std::string target)
{
	std::string retVal = target;
	for (auto &c : retVal) {
		c = tolower(c);
	}
	return retVal;
}
