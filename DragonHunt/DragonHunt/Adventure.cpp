#include "Adventure.h"
#include <iostream>

Adventure::Adventure()
{
}


Adventure::~Adventure()
{
}

void Adventure::loadFromFile(std::string originFile)
{
	//open the file
	m_doc.LoadFile(originFile.c_str());

	if (m_doc.FirstChildElement("adventure") == NULL) {
		std::cout << "Element \"adventure\" was not found." << std::endl;
		return;
	}
	//perform attribute checks on adventure
	XMLElement* adv = m_doc.FirstChildElement("adventure");
	if (adv->Attribute("name") == NULL) {
		std::cout << "Attribute \"name\" expected in \"adventure\" at line "<<adv->GetLineNum() << std::endl;
		return;
	}
	else {
		m_name = adv->Attribute("name");
	}

	if (adv->Attribute("version") == NULL) {
		std::cout << "Attribute \"version\" expected in \"adventure\" at line " << adv->GetLineNum() << std::endl;
		return;
	}
	else {
		//cross check engine version
		if (strcmp(adv->Attribute("version"), FORMATVERSION)) {
			std::cout << "Expected format version " << FORMATVERSION << " but adventure was written for version " << adv->Attribute("version") << " and no conversion specified." << std::endl;;
			return;
		}
	}

	if (adv->Attribute("start") == NULL) {
		std::cout << "Attribute \"start\" expected in \"adventure\" at line " << adv->GetLineNum() << std::endl;
		return;
	}
	else {
		m_startLocation = adv->Attribute("start");
	}

	//get first child THEN cast to element just so we can know if was an error
	XMLElement* childOfAdv = adv->FirstChild()->ToElement();
	if (!(adv->FirstChild() == NULL) &&childOfAdv == NULL) {
		std::cout << "Unknown element at line " << adv->FirstChild()->GetLineNum() <<": \""<<adv->FirstChild()->Value()<<"\""<< std::endl;
		return;
	}
	//loop through elements
	while (childOfAdv != NULL) {
		//perform actions based off element type
		if (!strcmp("location", childOfAdv->Name())) {
			//handle location
			handleLocation(childOfAdv);
		}
		else if (!strcmp("cc", childOfAdv->Name())) {
			//handle copyright info
			handleCC(childOfAdv);
		}
		else {
			std::cout << "Unknown element \"" << childOfAdv->Name() << "\" at line " << childOfAdv->GetLineNum() << std::endl;
		}

		//check if something there that's not supposed to be
		if (!(childOfAdv->NextSibling() == NULL) && childOfAdv->NextSibling()->ToElement() == NULL && childOfAdv->NextSibling()->ToComment() == NULL) {
			std::cout << "Unknown element at line " << adv->FirstChild()->GetLineNum() << ": \"" << adv->FirstChild()->Value() << "\"" << std::endl;
			return;
		}
		//check if its not a comment
		else if(childOfAdv->NextSibling()->ToComment() == NULL){
			childOfAdv = childOfAdv->NextSiblingElement();
		}
		else {
			//go through until not a comment
			XMLNode* p = childOfAdv->NextSibling();
			while (p != NULL && p->ToComment() != NULL) {
				p = p->NextSibling();
			}
			if (p != NULL) {
				if (p->ToElement() != NULL) {
					childOfAdv = p->ToElement();
				}
				else {
					std::cout << "Unknown element at line " << adv->FirstChild()->GetLineNum() << ": \"" << adv->FirstChild()->Value() << "\"" << std::endl;
					return;
				}
			}
			else {
				break;
			}
		}
	}
}

void Adventure::handleLocation(XMLElement * locationElement)
{
}

void Adventure::handleCC(XMLElement * ccElement)
{
}
