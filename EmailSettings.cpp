#include "stdafx.h"
#include "EmailSettings.h"


EmailSettings::EmailSettings()
{
}


EmailSettings::~EmailSettings()
{
}

void EmailSettings::ReadXML() {
	xml_document<> doc;  
	xml_node<> * root_node;

	// read the xml file into a vector
	std::ifstream inputFile(this->getFilename());

	vector<char> buffer((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	// parse the buffer via the parsing library
	doc.parse<0>(&buffer[0]);

	// find the root node
	root_node = doc.first_node("performance");

	// iterate over the email attributes
	xml_node<> *email_node = root_node->first_node("email");
		
	}

	
}
