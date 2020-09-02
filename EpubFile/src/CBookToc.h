#ifndef CBOOKTOC_H
#define CBOOKTOC_H

#include <iostream>
#include <map>

#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookToc
{
    std::wstring m_sTitle;
    struct m_structData
    {
        std::wstring sText;
        std::wstring sID;
        std::wstring sRef;
        std::wstring sPlayOrder;
    };

    std::vector<std::pair<std::wstring, std::wstring>> m_arMap;
    std::vector<m_structData> m_arData;

    bool ReadTitle(XmlUtils::CXmlLiteReader &oXmlLiteReader);
    bool ReadMap(XmlUtils::CXmlLiteReader &oXmlLiteReader);
    bool ReadPoint(XmlUtils::CXmlLiteReader &oXmlLiteReader);
    std::wstring GetAttributeValue(XmlUtils::CXmlLiteReader &oXmlLiteReader, const std::wstring& sAttributeName) const;
    void AddStruct();
public:
    CBookToc();
    ~CBookToc();
    void Clear();
    bool ReadToc(XmlUtils::CXmlLiteReader &oXmlLiteReader);
    void ShowToc() const;
    int GetCountToc() const;
    const std::pair<std::wstring, std::wstring> GetTextAndRef(const int& nIndex) const;
};

#endif // CBOOKTOC_H
