#include "XMLNode.h"

namespace HWP
{
CXMLNode::CXMLNode()
	: XmlUtils::CXmlNode()
{}

CXMLNode::CXMLNode(const CXmlNode& oNode)
	: XmlUtils::CXmlNode(oNode)
{}

bool CXMLNode::GetAttributeBool(const std::wstring& wsName)
{
	return L"1" == XmlUtils::CXmlNode::GetAttribute(wsName, L"0");
}

int CXMLNode::GetAttributeColor(const std::wstring& wsName, const int& _default)
{
	std::wstring sColor = XmlUtils::CXmlNode::GetAttribute(wsName);

	if (L"none" != sColor)
	{
		if (L'#' == sColor.front())
			sColor.erase(0, 1);

		return std::stoi(sColor, 0, 16);
	}

	return _default;
}

CXMLNode CXMLNode::GetChild(const std::wstring& wsName)
{
	return CXMLNode(XmlUtils::CXmlNode::GetNode(wsName));
}

std::vector<CXMLNode> CXMLNode::GetChilds()
{
	std::vector<XmlUtils::CXmlNode> arChilds;
	XmlUtils::CXmlNode::GetChilds(arChilds);

	std::vector<CXMLNode> arNewChilds(arChilds.size());
	for (unsigned int unIndex = 0; unIndex < arChilds.size(); ++unIndex)
		arNewChilds[unIndex] = CXMLNode(arChilds[unIndex]);

	return arNewChilds;
}

std::vector<CXMLNode> CXMLNode::GetChilds(const std::wstring& wsName)
{
	std::vector<XmlUtils::CXmlNode> arChilds{XmlUtils::CXmlNode::GetNodes(wsName)};

	std::vector<CXMLNode> arNewChilds(arChilds.size());
	for (unsigned int unIndex = 0; unIndex < arChilds.size(); ++unIndex)
		arNewChilds[unIndex] = CXMLNode(arChilds[unIndex]);

	return arNewChilds;
}

int ConvertWidthToHWP(const std::wstring& wsValue)
{
	if (wsValue.empty() || L"0.1" == wsValue || L"0.1 mm" == wsValue)
		return 0;
	else if (L"0.12" == wsValue || L"0.12 mm" == wsValue)
		return 1;
	else if (L"0.15" == wsValue || L"0.15 mm" == wsValue)
		return 2;
	else if (L"0.2" == wsValue || L"0.2 mm" == wsValue)
		return 3;
	else if (L"0.25" == wsValue || L"0.25 mm" == wsValue)
		return 4;
	else if (L"0.3" == wsValue || L"0.3 mm" == wsValue)
		return 5;
	else if (L"0.4" == wsValue || L"0.4 mm" == wsValue)
		return 6;
	else if (L"0.5" == wsValue || L"0.5 mm" == wsValue)
		return 7;
	else if (L"0.6" == wsValue || L"0.6 mm" == wsValue)
		return 8;
	else if (L"0.7" == wsValue || L"0.7 mm" == wsValue)
		return 9;
	else if (L"1.0" == wsValue || L"1.0 mm" == wsValue)
		return 10;
	else if (L"1.5" == wsValue || L"1.5 mm" == wsValue)
		return 11;
	else if (L"2.0" == wsValue || L"2.0 mm" == wsValue)
		return 12;
	else if (L"3.0" == wsValue || L"3.0 mm" == wsValue)
		return 13;
	else if (L"4.0" == wsValue || L"4.0 mm" == wsValue)
		return 14;
	else if (L"5.0" == wsValue || L"5.0 mm" == wsValue)
		return 15;

	return 0;
}
}
