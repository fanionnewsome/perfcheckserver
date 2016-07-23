#ifndef H_SETTINGS
#define H_SETTINGS

#include <iostream>
#include <rapidxml.hpp>

using namespace rapidxml;

class Settings
{
public:
	Settings();
	Settings(std::string filename);
	~Settings();

	virtual void ReadXML() = 0;
	std::string getFilename() const { return _filename;  }

private:
	
	std::string _filename;
	xml_document<> doc;
};

#endif // !H_SETTINGS