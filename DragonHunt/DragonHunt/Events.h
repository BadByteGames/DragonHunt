#pragma once

#include <unordered_map>

#include <tinyxml2\tinyxml2.h>

//an item that can be found in an event list
class SequenceItem
{
friend class Event;
public:
	SequenceItem();
	~SequenceItem();

	//create a print call from the current paramaters
	//returns nullptr if failed
	virtual SequenceItem* create() { return nullptr;  };

	//call of a function
	virtual int onCall() { return 1; };

protected:
	void requireArgument(std::string name);

	std::string getArgument(std::string name);

	void copyArgs(SequenceItem* other);

private:
	std::unordered_map<std::string, bool> m_requiredArgs;

	std::unordered_map<std::string, std::string> m_arguments;
};

class Print : public SequenceItem{
public:
	Print();
	~Print();

	virtual SequenceItem* create() override;
	virtual int onCall() override;
};

//events will use their own XML handler rather than the normal one
class Event
{
public:
	Event();
	~Event();

	int parseFromElement(tinyxml2::XMLElement* rootNode);

	void addSequencePossibility(std::string name, SequenceItem* si);

	void execute();

	void destroySequenceOnly();

	void destroy();

private:
	//name, sequence item a list of possible sequence items
	std::unordered_map<std::string, SequenceItem*> m_sequencePossibilities;

	//a vector defined at build time of what to do
	std::vector<SequenceItem*> m_sequence;
};