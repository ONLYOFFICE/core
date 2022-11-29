#include "ExtXmlUtils.h"


std::vector<std::wstring> XmlUtils::FindAttrValues(const std::wstring &xml, std::wstring attrName)
{
    attrName += L"=\"";
    std::vector<std::wstring> values;
    size_t posBeg = xml.find(attrName);
    auto endTagPos = posBeg + attrName.size();
    size_t posEnd = xml.find(L"\"", endTagPos);

    // Repeat till end is reached
    while( posBeg != std::string::npos)
    {
        auto strValue = xml.substr(endTagPos, posEnd - (endTagPos));
        values.push_back(strValue);

        // Get the next occurrence from the current position
        posBeg = xml.find(attrName, endTagPos);
        endTagPos = posBeg + attrName.size();
        posEnd = xml.find(L"\"", endTagPos);
    }
    return values;
}

std::vector<int> XmlUtils::FindAttrValuesInt(const std::wstring &xml, std::wstring attrName)
{
    auto vecStrValues = FindAttrValues(xml, attrName);
    std::vector<int> vecIntValues;
    for (const auto& strValue : vecStrValues)
    {
        try {
            vecIntValues.push_back(std::stoi(strValue));
        } catch (...) {
        }
    }
    return vecIntValues;
}
