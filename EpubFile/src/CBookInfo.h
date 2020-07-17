#ifndef CBOOKINFO_H
#define CBOOKINFO_H

#include <map>

#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookInfo
{
    std::wstring m_sTitle;
    std::wstring m_sCreator;
    std::wstring m_sPublisher;
    std::wstring m_sLanguage;
    std::wstring m_sContributor;
    std::wstring m_sDescription;
    std::wstring m_sCoverage;
public:
    CBookInfo();
    ~CBookInfo();

    std::map <std::wstring, std::wstring> GetBookInfo();
    bool ReadInfo(XmlUtils::CXmlLiteReader &oXmlLiteReader);
    void ShowInfo();
};

#endif // CBOOKINFO_H
