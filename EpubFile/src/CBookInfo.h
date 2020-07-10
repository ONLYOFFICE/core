#ifndef CBOOKINFO_H
#define CBOOKINFO_H

#include <map>

#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookInfo
{
    std::map <std::wstring, std::wstring> m_mBookInfo;
    void ReadManifest();
public:
    CBookInfo();
    ~CBookInfo();

    std::map <std::wstring, std::wstring> GetBookInfo();
    bool ReadInfo(XmlUtils::CXmlLiteReader &oXmlLiteReader);
    void ShowInfo();
};

#endif // CBOOKINFO_H
