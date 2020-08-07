#ifndef CBOOKCONTENTITEM_H
#define CBOOKCONTENTITEM_H

#include <iostream>

#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookContentItem
{
public:
    std::wstring m_sID;
    std::wstring m_sLinear;
    CBookContentItem();
    ~CBookContentItem();

    void Clear();

    bool ReadContentItem(XmlUtils::CXmlLiteReader &oXmlLiteReader, int depth);

};

#endif // CBOOKCONTENTITEM_H
