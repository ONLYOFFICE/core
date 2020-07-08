#ifndef CBOOKINFO_H
#define CBOOKINFO_H

#include <map>
#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookInfo
{
    std::map <std::wstring, std::wstring> _bookInfo;
    XmlUtils::CXmlLiteReader *_reader;
    void ReadNodes();

public:
    CBookInfo();
    CBookInfo(const XmlUtils::CXmlLiteReader &reader);
    ~CBookInfo();

    bool SetReader(const XmlUtils::CXmlLiteReader &reader);
    std::map <std::wstring, std::wstring> GetBookInfo();
    bool ReadInfo();
};

#endif // CBOOKINFO_H
