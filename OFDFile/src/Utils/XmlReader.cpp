#include "XmlReader.h"

#include "Utils.h"

namespace OFD
{
CXmlReader::CXmlReader()
{}

std::wstring CXmlReader::GetTextValue(bool bIsAttribute)
{
	return (bIsAttribute) ? XmlUtils::CXmlLiteReader::GetText() : XmlUtils::CXmlLiteReader::GetText2();
}

std::string CXmlReader::GetTextValueA(bool bIsAttribute)
{
	return (bIsAttribute) ? XmlUtils::CXmlLiteReader::GetTextA() : XmlUtils::CXmlLiteReader::GetText2A();
}

bool CXmlReader::GetBoolean(bool bIsAttribute)
{
	bool bValue = false;
	StringToBoolean(GetTextValueA(bIsAttribute), bValue);
	return bValue;
}

int CXmlReader::GetInteger(bool bIsAttribute)
{
	int nValue = 0;
	StringToInteger(GetTextValueA(bIsAttribute), nValue);
	return nValue;
}

unsigned int CXmlReader::GetUInteger(bool bIsAttribute)
{
	unsigned int unValue = 0;
	StringToUInteger(GetTextValueA(bIsAttribute), unValue);
	return unValue;
}

double CXmlReader::GetDouble(bool bIsAttribute)
{
	double dValue = 0.;
	StringToDouble(GetTextValueA(bIsAttribute), dValue);
	return dValue;
}

std::vector<std::string> CXmlReader::GetArrayStrings(bool bIsAttribute)
{
	return Split(GetTextValueA(bIsAttribute), ' ');
}

std::vector<double> CXmlReader::GetArrayDoubles(bool bIsAttribute)
{
	const std::vector<std::string> arValues{Split(GetTextValueA(bIsAttribute), ' ')};

	if(arValues.empty())
		return std::vector<double>();

	std::vector<double> arDoubleValues(arValues.size());

	for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
		if (!StringToDouble(arValues[unIndex], arDoubleValues[unIndex]))
			return std::vector<double>();

	return arDoubleValues;
}
}
