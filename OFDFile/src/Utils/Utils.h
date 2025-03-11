#ifndef UTILS_H
#define UTILS_H


#include <vector>
#include <string>
#include "../../../OOXML/Base/Unit.h"
#include "../../../DesktopEditor/common/StringExt.h"

bool GetBoolean(const std::wstring& wsValue)
{
	return false;
}

int GetInt(const std::wstring& wsValue)
{
	return 0;
}

unsigned int GetUInt(const std::wstring& wsValue)
{
	return 0;
}

double GetDouble(const std::wstring& wsValue)
{
	return 0.;
}



std::vector<double> GetDoubleValues(const std::wstring& wsValue)
{
	const std::vector<std::wstring> arValues{NSStringExt::Split(wsValue, L' ')};

	if(arValues.empty())
		return std::vector<double>();

	std::vector<double> arDoubleValues(arValues.size());

	for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
		arDoubleValues[unIndex] = XmlUtils::GetDouble(arValues[unIndex]);

	return arDoubleValues;
}

#define NO_VALID_NODE(lite_reader, node_name) node_name != oLiteReader.GetName() || oLiteReader.IsEmptyElement() || !oLiteReader.IsValid()

#endif // UTILS_H
