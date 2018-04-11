#pragma once

#include <unordered_map>

#include <tinyxml2\tinyxml2.h>

//generic class for Event
class Statement {
friend class Event;
public:
	Statement();
	~Statement();

	virtual Statement* create() { return nullptr; };

	virtual int execute() { return 0; };

	virtual void extraParsing(tinyxml2::XMLElement* ele, std::unordered_map<std::string, Statement*> m_statementPossibilities) {}

	virtual void destroy() {}

	std::unordered_map<std::string, bool> getRequiredArguments() const { return m_requiredArgs; }

	std::unordered_map<std::string, std::string>* getArguments() { return &m_arguments; }

protected:
	void requireArgument(std::string name);

	std::string getArgument(std::string name);

private:
	std::unordered_map<std::string, bool> m_requiredArgs;

	std::unordered_map<std::string, std::string> m_arguments;
};

//an item that can be found in an event list
class SequenceItem : public Statement
{
friend class Event;
public:
	SequenceItem();
	~SequenceItem();

	//create a print call from the current paramaters
	//returns nullptr if failed
	virtual Statement* create() { return nullptr;  };

	//DO NOT OVERRIDE THIS IS PART OF THE INNER API
	virtual int execute() override;
	//DO NOT OVERRIDE THIS IS PART OF THE INNER API
	virtual void extraParsing(tinyxml2::XMLElement* ele, std::unordered_map<std::string, Statement*> m_statementPossibilities) override;
	
	//call of a function
	virtual int onCall() { return 1; };

};

//control groups are "if" statements
class ControlGroup : public Statement {
friend class Event;
public:
	ControlGroup();
	~ControlGroup();

	//makes a copy
	virtual Statement* create() { return nullptr; }

	//DO NOT OVERRIDE THIS IS PART OF THE INNER API
	virtual int execute() override;
	//DO NOT OVERRIDE THIS IS PART OF THE INNER API
	virtual void extraParsing(tinyxml2::XMLElement* ele, std::unordered_map<std::string, Statement*> m_statementPossibilities) override;
	//DO NOT OVERRIDE THIS IS PART OF THE INNER API
	virtual void destroy();

	//wheter or not this evaluates as true
	virtual bool isTrue() { return false; }
private:
	std::vector<Statement*> m_sequence;

	bool m_inverted = false;
};

//events will use their own XML handler rather than the normal one
class Event
{
public:
	Event();
	~Event();

	int parseFromElement(tinyxml2::XMLElement* rootNode);

	void addStatementPossibility(std::string name, Statement* si);

	//adds a control group and by default its "not" counterpart
	void addControlGroup(std::string name, ControlGroup* controlGroup, bool addNotCounterPart = true);

	void execute();

	void destroyStatementsOnly();

	void destroy();

private:
	//name, sequence item a list of possible sequence items
	std::unordered_map<std::string, Statement*> m_statementPossibilities;

	//a vector defined at build time of what to do
	std::vector<Statement*> m_sequence;
};