#pragma once

#include <string>
#include <vector>
#include <algorithm>


namespace XmlUtils {
std::vector<std::wstring> FindAttrValues(const std::wstring& xml, std::wstring attrName);
std::vector<int> FindAttrValuesInt(const std::wstring& xml, std::wstring attrName);
}

