#ifndef XMLREADER_H
#define XMLREADER_H

#include <vector>
#include <string>
#include "../../../DesktopEditor/xml/include/xmlutils.h"

namespace OFD
{
class CXmlReader : public XmlUtils::CXmlLiteReader
{
	std::wstring GetTextValue(bool bIsAttribute = false);
	std::string  GetTextValueA(bool bIsAttribute = false);
public:
	CXmlReader();

	bool GetBoolean(bool bIsAttribute = false);
	int GetInteger(bool bIsAttribute = false);
	unsigned int GetUInteger(bool bIsAttribute = false);
	double GetDouble(bool bIsAttribute = false);
	std::vector<double> GetArrayDoubles(bool bIsAttribute = false);
};
}

#endif // XMLREADER_H
