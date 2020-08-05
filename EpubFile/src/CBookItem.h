#ifndef CBOOKITEM_H
#define CBOOKITEM_H

#include <iostream>

#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookItem
{
    enum ItemType
    {
        xhtml,
        image,
        toc,
        font,
        css
    };
    std::wstring m_sID;
    std::wstring m_sRef;
    std::wstring m_sProperties;
    ItemType m_eType;

public:
    CBookItem();
    ~CBookItem();

    bool ReadItem(XmlUtils::CXmlLiteReader &oXmlLiteReader, int depth);
    std::wstring GetID();
    std::wstring GetRef();
};

#endif // CBOOKITEM_H
