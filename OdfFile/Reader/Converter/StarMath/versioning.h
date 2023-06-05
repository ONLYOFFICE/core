#ifndef SM_PARSER_VERSIONING_H
#define SM_PARSER_VERSIONING_H

#include <string>

namespace StarMathParser::Interfaces
{

class Versioning
{
public:
	struct Version {
		std::string sV;
		std::string sMajor;
		std::string sMinor;
	};

	virtual Version GetVersion() const = 0;
};

}

#endif // SM_PARSER_VERSIONING_H
