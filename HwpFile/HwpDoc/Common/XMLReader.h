#ifndef XMLNODEH_H
#define XMLNODEH_H

#include "../../../DesktopEditor/xml/include/xmlutils.h"

namespace HWP
{
class CXMLReader
{
	XmlUtils::CXmlLiteReader *m_pReader;

	bool m_bParseAttribute;
public:
	CXMLReader();
	~CXMLReader();

	XmlUtils::CXmlLiteReader* GetReader();
	unsigned int GetDepth();
	bool IsEmptyNode();

	bool GetBool();
	int GetColor(const int& nDefault = 0x000000);
	int GetInt();
	double GetDouble();
	std::string GetTextA();
	std::wstring GetText();
	const char* GetTextChar();

	int GetAttributeInt(const std::string& sName, int nDefault = 0);
	bool GetAttributeBool(const std::string& sName);
	double GetAttributeDouble(const std::string& sName);
	std::string GetAttributeA(const std::string& sName);
	std::wstring GetAttribute(const std::string& sName);

	bool MoveToFirstAttribute();
	bool MoveToNextAttribute();
	bool MoveToElement();

	std::wstring GetInnerXml();

	std::string GetName();
	bool ReadNextSiblingNode(unsigned int unDepth);
	bool ReadNextNode();
	bool Read(XmlUtils::XmlNodeType& eNodeType);
private:
	static int GetIntValue(CXMLReader& oXmlReader);
	static bool GetBoolValue(CXMLReader& oXmlReader);
	static double GetDoubleValue(CXMLReader& oXmlReader);
	static std::string GetTextAValue(CXMLReader& oXmlReader);
	static std::wstring GetTextValue(CXMLReader& oXmlReader);

	template<typename T>
	T GetAttribute(const std::string& sName, T _default, T (*GetValue)(CXMLReader& oXmlReader));
};

#define WHILE_READ_NEXT_NODE_WITH_DEPTH(xml_reader, name_depth)\
	if (!xml_reader.IsEmptyNode())\
	{\
	const int n##name_depth = xml_reader.GetDepth();\
	while (xml_reader.ReadNextSiblingNode(n##name_depth))\
	{

#define WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(xml_reader, name_depth)\
	if (!xml_reader.IsEmptyNode())\
	{\
	const int n##name_depth##Depth = xml_reader.GetDepth();\
	std::string sNode##name_depth##Name;\
	while (xml_reader.ReadNextSiblingNode(n##name_depth##Depth))\
	{\
		sNode##name_depth##Name = xml_reader.GetName();

#define WHILE_READ_NEXT_NODE(xml_reader) WHILE_READ_NEXT_NODE_WITH_DEPTH(xml_reader, Depth)

#define WHILE_READ_NEXT_NODE_WITH_NAME(xml_reader)\
	if (!xml_reader.IsEmptyNode())\
	{\
	const int nDepth = xml_reader.GetDepth();\
	std::string sNodeName;\
	while (xml_reader.ReadNextSiblingNode(nDepth))\
	{\
		sNodeName = xml_reader.GetName();

#define WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(xml_reader, name_depth, node_name)\
	WHILE_READ_NEXT_NODE_WITH_DEPTH(xml_reader, name_depth)\
		if (node_name != xml_reader.GetName())\
			continue;

#define WHILE_READ_NEXT_NODE_WITH_ONE_NAME(xml_reader, node_name) WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(xml_reader, Depth, node_name)

#define END_WHILE } }

#define START_READ_ATTRIBUTES(xml_reader)\
	if (xml_reader.MoveToFirstAttribute())\
	{\
		std::string sAttributeName;\
		do\
		{\
			sAttributeName = xml_reader.GetName();

#define END_READ_ATTRIBUTES(xml_reader)\
	}\
	while(xml_reader.MoveToNextAttribute());\
	xml_reader.MoveToElement();\
	}

int ConvertWidthToHWP(const std::string& sValue);
int ConvertHexToInt(const std::string& sValue, const int& _default = 0x00000000);
}

#endif // XMLNODEH_H
