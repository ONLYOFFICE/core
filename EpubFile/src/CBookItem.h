#ifndef CBOOKITEM_H
#define CBOOKITEM_H

#include <string>

#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookItem
{
    enum ItemType
    {
        Default,
        Xhtml,
        Image,
        Toc,
        Font,
        Css
    };
    std::wstring m_sID;
    std::wstring m_sRef;
    std::wstring m_sProperties;
    ItemType m_eType;

public:
    CBookItem();
    ~CBookItem();
    // void Clear();

    bool ReadItem(XmlUtils::CXmlLiteReader& oXmlLiteReader, int depth);
    const std::wstring GetID() const;
    const std::wstring GetRef() const;
};

#endif // CBOOKITEM_H
