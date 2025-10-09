#include "SvgReader.h"

#include <cmath>
#include <cctype>

namespace SVG
{
int ConvertHexToInt(const std::string& sValue, const int& _default)
{
	if (sValue.empty() || "none" == sValue)
		return _default;

	std::string::const_iterator itStart = sValue.cbegin();

	if ('#' == *itStart)
		++itStart;

	if (sValue.cend() - itStart != 6)
		return _default;

	itStart = sValue.cend() - 6;

	int nResult = 0;

	while (itStart != sValue.cend())
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

CSvgReader::CSvgReader()
	: m_pReader(new XmlUtils::CXmlLiteReader), m_bParseAttribute(false)
{}

CSvgReader::~CSvgReader()
{
	if (nullptr != m_pReader)
		delete m_pReader;
}

bool SVG::CSvgReader::ReadFromFile(const std::wstring& wsFilePath)
{
	return (nullptr != m_pReader) ? m_pReader->FromFile(wsFilePath) : false;
}

bool SVG::CSvgReader::ReadFromString(const std::wstring& wsSvg)
{
	return (nullptr != m_pReader) ? m_pReader->FromString(wsSvg) : false;
}

// XmlUtils::CXmlLiteReader* CSvgReader::GetReader()
// {
// 	return m_pReader;
// }

unsigned int CSvgReader::GetDepth()
{
	return (nullptr != m_pReader) ? m_pReader->GetDepth() : 0;
}

bool CSvgReader::IsEmptyNode()
{
	return (nullptr != m_pReader) ? m_pReader->IsEmptyNode() : true;
}

bool CSvgReader::GetBool()
{
	return "1" == GetTextAValue(*this) || "true" == GetTextAValue(*this);
}

int StringToInt(const std::string& sValue, const int& _default)
{
	std::string::const_iterator itPos{sValue.cbegin()};

	while (std::isspace(*itPos))
		++itPos;

	if (sValue.cend() == itPos)
		return _default;

	while (sValue.cend() != itPos && !std::isdigit(*itPos))
		++itPos;

	if (sValue.cend() == itPos)
		return _default;

	int nResult = 0;

	while (itPos != sValue.cend() && std::isdigit(*itPos))
	{
		nResult = nResult * 10 + (*itPos - '0');
		++itPos;
	}

	return ((nResult & 0xFF) << 16) | (((nResult >> 8)  & 0xFF) << 8) | ((nResult >> 16) & 0xFF);
}

int CSvgReader::GetColor(const int& nDefault)
{
	const std::string sValue{GetTextAValue(*this)};

	if (sValue.empty())
		return nDefault;

	if ('#' == sValue[0])
		return ConvertHexToInt(sValue, nDefault);

	return StringToInt(sValue, nDefault);
}

int CSvgReader::GetInt()
{
	return GetIntValue(*this);
}

double CSvgReader::GetDouble()
{
	return GetDoubleValue(*this);
}

std::string CSvgReader::GetTextA()
{
	if (nullptr == m_pReader)
		return std::string();

	return (m_bParseAttribute) ? m_pReader->GetTextA() : m_pReader->GetText2A();
}

std::wstring CSvgReader::GetText()
{
	if (nullptr == m_pReader)
		return std::wstring();

	return (m_bParseAttribute) ? m_pReader->GetText() : m_pReader->GetText2();
}

const char* CSvgReader::GetTextChar()
{
	if (nullptr == m_pReader)
		return "";

	return m_pReader->GetTextChar();
}

template<typename T>
T CSvgReader::GetAttribute(const std::string& sName, T _default, T (*GetValue)(CSvgReader&))
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

int CSvgReader::GetAttributeInt(const std::string& sName, int nDefault)
{
	return GetAttribute<int>(sName, nDefault, &GetIntValue);
}

bool CSvgReader::GetAttributeBool(const std::string& sName)
{
	return GetAttribute<bool>(sName, false, &GetBoolValue);
}

double CSvgReader::GetAttributeDouble(const std::string& sName)
{
	return GetAttribute<double>(sName, 0., &GetDoubleValue);
}

std::string CSvgReader::GetAttributeA(const std::string& sName)
{
	return GetAttribute<std::string>(sName, "", &GetTextAValue);
}

std::wstring CSvgReader::GetAttribute(const std::string& sName)
{
	return GetAttribute<std::wstring>(sName, L"", &GetTextValue);
}

bool CSvgReader::MoveToStart()
{
	return (nullptr != m_pReader) ? m_pReader->MoveToStart() : false;
}

bool CSvgReader::MoveToFirstAttribute()
{
	if (nullptr == m_pReader || !m_pReader->MoveToFirstAttribute())
		return false;

	m_bParseAttribute = true;

	return true;
}

bool CSvgReader::MoveToNextAttribute()
{
	return (nullptr != m_pReader) ? m_pReader->MoveToNextAttribute() : false;
}

bool CSvgReader::MoveToElement()
{
	if (nullptr == m_pReader || !m_pReader->MoveToElement())
		return false;

	m_bParseAttribute = false;

	return true;
}

std::string CSvgReader::GetName()
{
	return (nullptr != m_pReader) ? m_pReader->GetNameA() : std::string();
}

std::wstring CSvgReader::GetNameW()
{
	return (nullptr != m_pReader) ? m_pReader->GetName() : std::wstring();
}

bool CSvgReader::ReadNextSiblingNode(unsigned int unDepth)
{
	return (nullptr != m_pReader) ? m_pReader->ReadNextSiblingNode(unDepth) : false;
}

bool CSvgReader::ReadNextNode()
{
	return (nullptr != m_pReader) ? m_pReader->ReadNextNode() : false;
}

bool CSvgReader::Read(XmlUtils::XmlNodeType& eNodeType)
{
	return (nullptr != m_pReader) ? m_pReader->Read(eNodeType) : false;
}

int CSvgReader::GetIntValue(CSvgReader& oXmlReader)
{
	return std::atoi(oXmlReader.GetTextA().c_str());
}

bool CSvgReader::GetBoolValue(CSvgReader& oXmlReader)
{
	return "1" == oXmlReader.GetTextA();
}

double CSvgReader::GetDoubleValue(CSvgReader& oXmlReader)
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

		dResult *= std::pow(10., nExponent);
	}

	return bNegative ? -dResult : dResult;
}

std::string CSvgReader::GetTextAValue(CSvgReader& oXmlReader)
{
	return oXmlReader.GetTextA();
}

std::wstring CSvgReader::GetTextValue(CSvgReader& oXmlReader)
{
	return oXmlReader.GetText();
}
}
