#ifndef XMLNODEH_H
#define XMLNODEH_H

#include "../../../DesktopEditor/xml/include/xmlutils.h"

namespace HWP
{
class CXMLNode : public XmlUtils::CXmlNode
{
public:
	CXMLNode();
	CXMLNode(const XmlUtils::CXmlNode& oNode);

	bool GetAttributeBool(const std::wstring& wsName);
	int GetAttributeColor(const std::wstring& wsName, const int& _default = 0x00000000);
	CXMLNode GetChild(const std::wstring& wsName);
	std::vector<CXMLNode> GetChilds();
	std::vector<CXMLNode> GetChilds(const std::wstring& wsName);
};

int ConvertWidthToHWP(const std::wstring& wsValue);
}

#endif // XMLNODEH_H
