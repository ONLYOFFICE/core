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
	return L"1" == XmlUtils::CXmlNode::GetAttribute(wsName);
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
}
