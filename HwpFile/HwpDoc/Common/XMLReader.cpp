#include "XMLReader.h"

namespace HWP
{
CXMLReader::CXMLReader()
{}

CXMLReader::CXMLReader(CXmlLiteReader& oReader)
	// : XmlUtils::CXmlLiteReader(oReader)
{
	oReader.ReadNextNode();
	FromString(oReader.GetInnerXml());
}

bool CXMLReader::GetBool()
{
	return "1" == GetText2A();
}

int CXMLReader::GetColor(const int& nDefault)
{
	return ConvertHexToInt(GetText2A(), nDefault);
}

int CXMLReader::GetInt()
{
	return GetIntValue(*this);
}

double CXMLReader::GetDouble()
{
	return GetDoubleValue(*this);
}

template<typename T>
T CXMLReader::GetAttribute(const std::string& sName, T _default, T (*GetValue)(CXMLReader& oReader))
{
	if (!MoveToFirstAttribute())
		return _default;

	T oValue = _default;

	do
	{
		if (sName == GetNameA())
		{
			oValue = GetValue(*this);
			break;
		}
	}while(MoveToNextAttribute());

	MoveToElement();

	return oValue;
}

int CXMLReader::GetAttributeInt(const std::string& sName, int nDefault)
{
	return GetAttribute<int>(sName, nDefault, &GetIntValue);
}

bool CXMLReader::GetAttributeBool(const std::string& sName)
{
	return GetAttribute<bool>(sName, false, &GetBoolValue);
}

double CXMLReader::GetAttributeDouble(const std::string& sName)
{
	return GetAttribute<double>(sName, 0., &GetDoubleValue);
}

std::string CXMLReader::GetAttributeA(const std::string& sName)
{
	return GetAttribute<std::string>(sName, "", &GetTextAValue);
}

std::wstring CXMLReader::GetAttribute(const std::string& sName)
{
	return GetAttribute<std::wstring>(sName, L"", &GetTextValue);
}

int CXMLReader::GetIntValue(CXMLReader& oXmlReader)
{
	return std::atoi(oXmlReader.GetText2A().c_str());
}

bool CXMLReader::GetBoolValue(CXMLReader& oXmlReader)
{
	return "1" == oXmlReader.GetText2A();
}

double CXMLReader::GetDoubleValue(CXMLReader& oXmlReader)
{
	return std::atof(oXmlReader.GetText2A().c_str());
}

std::string CXMLReader::GetTextAValue(CXMLReader& oXmlReader)
{
	return oXmlReader.GetText2A();
}

std::wstring CXMLReader::GetTextValue(CXMLReader& oXmlReader)
{
	return oXmlReader.GetText2();
}

int ConvertWidthToHWP(const std::string& sValue)
{
	if (sValue.empty() || "0.1" == sValue || "0.1 mm" == sValue)
		return 0;
	else if ("0.12" == sValue || "0.12 mm" == sValue)
		return 1;
	else if ("0.15" == sValue || "0.15 mm" == sValue)
		return 2;
	else if ("0.2" == sValue || "0.2 mm" == sValue)
		return 3;
	else if ("0.25" == sValue || "0.25 mm" == sValue)
		return 4;
	else if ("0.3" == sValue || "0.3 mm" == sValue)
		return 5;
	else if ("0.4" == sValue || "0.4 mm" == sValue)
		return 6;
	else if ("0.5" == sValue || "0.5 mm" == sValue)
		return 7;
	else if ("0.6" == sValue || "0.6 mm" == sValue)
		return 8;
	else if ("0.7" == sValue || "0.7 mm" == sValue)
		return 9;
	else if ("1.0" == sValue || "1.0 mm" == sValue)
		return 10;
	else if ("1.5" == sValue || "1.5 mm" == sValue)
		return 11;
	else if ("2.0" == sValue || "2.0 mm" == sValue)
		return 12;
	else if ("3.0" == sValue || "3.0 mm" == sValue)
		return 13;
	else if ("4.0" == sValue || "4.0 mm" == sValue)
		return 14;
	else if ("5.0" == sValue || "5.0 mm" == sValue)
		return 15;

	return 0;
}

int ConvertHexToInt(const std::string& wsValue, const int& _default)
{
	if (wsValue.empty() || "none" == wsValue)
		return _default;

	std::string::const_iterator itStart = wsValue.cbegin();

	if ('#' == *itStart)
		++itStart;

	if (wsValue.cend() - itStart < 6)
		return _default;

	itStart = wsValue.cend() - 6;

	int nResult = 0;

	while (itStart != wsValue.cend())
	{
		if ('0' <= *itStart && *itStart <= '9')
			nResult = (nResult << 4) | (*itStart++ - '0');
		else if ('A' <= *itStart && *itStart <= 'F')
			nResult = (nResult << 4) | (*itStart++ - 'A' + 10);
		else if ('a' <= *itStart && *itStart <= 'f')
			nResult = (nResult << 4) | (*itStart++ - 'a' + 10);
		else
			return _default;
	}

	return nResult;
}
}
