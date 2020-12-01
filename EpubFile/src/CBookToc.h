#ifndef CBOOKTOC_H
#define CBOOKTOC_H

#include <string>
#include <vector>

#include "../../DesktopEditor/xml/include/xmlutils.h"

struct m_structData
{
    std::wstring sText;
    std::wstring sID;
    std::wstring sRef;
    std::wstring sPlayOrder;

    bool Full()
    {
        return !sText.empty() && !sID.empty() && !sRef.empty() && !sPlayOrder.empty();
    }

    void Clear()
    {
        sText.clear();
        sID.clear();
        sRef.clear();
        sPlayOrder.clear();
    }
};

class CBookToc
{
    std::wstring m_sTitle;
    std::vector<m_structData> m_arData;

    void ReadTitle(XmlUtils::CXmlLiteReader& oXmlLiteReader);
    void ReadMap(XmlUtils::CXmlLiteReader& oXmlLiteReader);
    std::wstring GetAttributeValue(XmlUtils::CXmlLiteReader& oXmlLiteReader, const std::wstring& sAttributeName) const;
public:
    CBookToc();
    ~CBookToc();
    void Clear();
    bool ReadToc(XmlUtils::CXmlLiteReader& oXmlLiteReader);
    // void ShowToc() const;
    // int GetCountToc() const;
    // const std::pair<std::wstring, std::wstring> GetTextAndRef(int nIndex) const;
};

#endif // CBOOKTOC_H
