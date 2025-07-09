#include "XMLReader.h"

namespace HWP
{
CXMLReader::CXMLReader()
	: m_pReader(new XmlUtils::CXmlLiteReader), m_bParseAttribute(false)
{}

CXMLReader::~CXMLReader()
{
	if (nullptr != m_pReader)
		delete m_pReader;
}

XmlUtils::CXmlLiteReader* CXMLReader::GetReader()
{
	return m_pReader;
}

unsigned int CXMLReader::GetDepth()
{
	return (nullptr != m_pReader) ? m_pReader->GetDepth() : 0;
}

bool CXMLReader::IsEmptyNode()
{
	return (nullptr != m_pReader) ? m_pReader->IsEmptyNode() : true;
}

bool CXMLReader::GetBool()
{
	return "1" == GetTextAValue(*this);
}

int CXMLReader::GetColor(const int& nDefault)
{
	return ConvertHexToInt(GetTextAValue(*this), nDefault);
}

int CXMLReader::GetInt()
{
	return GetIntValue(*this);
}

double CXMLReader::GetDouble()
{
	return GetDoubleValue(*this);
}

std::string CXMLReader::GetTextA()
{
	if (nullptr == m_pReader)
		return std::string();

	return (m_bParseAttribute) ? m_pReader->GetTextA() : m_pReader->GetText2A();
}

std::wstring CXMLReader::GetText()
{
	if (nullptr == m_pReader)
		return std::wstring();

	return (m_bParseAttribute) ? m_pReader->GetText() : m_pReader->GetText2();
}

const char* CXMLReader::GetTextChar()
{
	if (nullptr == m_pReader)
		return "";

	return m_pReader->GetTextChar();
}

template<typename T>
T CXMLReader::GetAttribute(const std::string& sName, T _default, T (*GetValue)(CXMLReader&))
{
	if (!MoveToFirstAttribute())
		return _default;

	T oValue = _default;

	do
	{
		if (sName == m_pReader->GetNameA())
		{
			oValue = GetValue(*this);
			break;
		}
	}while(m_pReader->MoveToNextAttribute());

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

bool CXMLReader::MoveToFirstAttribute()
{
	if (nullptr == m_pReader || !m_pReader->MoveToFirstAttribute())
		return false;

	m_bParseAttribute = true;

	return true;
}

bool CXMLReader::MoveToNextAttribute()
{
	return (nullptr != m_pReader) ? m_pReader->MoveToNextAttribute() : false;
}

bool CXMLReader::MoveToElement()
{
	if (nullptr == m_pReader || !m_pReader->MoveToElement())
		return false;

	m_bParseAttribute = false;

	return true;
}

std::wstring CXMLReader::GetInnerXml()
{
	return m_pReader->GetInnerXml();
}

std::string CXMLReader::GetName()
{
	return (nullptr != m_pReader) ? m_pReader->GetNameA() : std::string();
}

bool CXMLReader::ReadNextSiblingNode(unsigned int unDepth)
{
	return (nullptr != m_pReader) ? m_pReader->ReadNextSiblingNode(unDepth) : false;
}

bool CXMLReader::ReadNextNode()
{
	return (nullptr != m_pReader) ? m_pReader->ReadNextNode() : false;
}

bool CXMLReader::Read(XmlUtils::XmlNodeType& eNodeType)
{
	return (nullptr != m_pReader) ? m_pReader->Read(eNodeType) : false;
}

int CXMLReader::GetIntValue(CXMLReader& oXmlReader)
{
	return std::atoi(oXmlReader.GetTextA().c_str());
}

bool CXMLReader::GetBoolValue(CXMLReader& oXmlReader)
{
	return "1" == oXmlReader.GetTextA();
}

double CXMLReader::GetDoubleValue(CXMLReader& oXmlReader)
{
	const std::string sValue{oXmlReader.GetTextA()};
	const char* pCur = sValue.c_str();

	while (std::isspace(*pCur))
		++pCur;

	if (!pCur)
		return 0.;

	bool bNegative = false;

	while ('-' == *pCur || '+' == *pCur)
	{
		if ('-' == *pCur)
			bNegative = !bNegative;

		++pCur;
	}

	if (!pCur)
		return 0.;

	double dResult = 0.;

	while (std::isdigit(*pCur))
		dResult = dResult * 10. + (*pCur++ - '0');

	if (',' == *pCur || '.' == *pCur)
	{
		++pCur;

		double dFraction = 0.0;
		double dDivisor  = 1.0;

		while (std::isdigit(*pCur))
		{
			dFraction = dFraction * 10. + (*pCur++ - '0');
			dDivisor *= 10;
		}

		dResult += dFraction / dDivisor;
	}

	if ('e' == *pCur || 'E' == *pCur)
	{
		++pCur;

		bool bExpNegative = false;
		while ('-' == *pCur || '+' == *pCur)
		{
			if ('-' == *pCur)
				bExpNegative = !bExpNegative;

			++pCur;
		}

		int nExponent = 0;

		while (std::isdigit(*pCur))
			nExponent = nExponent * 10 + (*pCur++ - '0');

		if (bExpNegative)
			nExponent = -nExponent;

		dResult *= pow(10., nExponent);
	}

	return bNegative ? -dResult : dResult;
}

std::string CXMLReader::GetTextAValue(CXMLReader& oXmlReader)
{
	return oXmlReader.GetTextA();
}

std::wstring CXMLReader::GetTextValue(CXMLReader& oXmlReader)
{
	return oXmlReader.GetText();
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
