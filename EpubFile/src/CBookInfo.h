#ifndef CBOOKINFO_H
#define CBOOKINFO_H

#include <map>

#include "../../DesktopEditor/xml/include/xmlutils.h"

class CBookInfo
{
    std::wstring m_sTitle;
    std::vector<std::wstring> m_arCreators;
    std::vector<std::wstring> m_arPublishers;
    std::vector<std::wstring> m_arLanguages;
    std::vector<std::wstring> m_arContributors;
    std::vector<std::wstring> m_arDescriptions;
    std::vector<std::wstring> m_arSubjects;
    std::wstring m_sCoverage;
    std::wstring m_sIdentifier;
    std::wstring m_sDate;
public:
    CBookInfo();
    ~CBookInfo();
    void Clear();

    bool ReadInfo(XmlUtils::CXmlLiteReader &oXmlLiteReader);
    void ShowInfo();

    std::wstring GetTitle();
    std::wstring GetCreators();
    std::wstring GetPublishers();
    std::wstring GetLanguages();
    std::wstring GetContibutors();
    std::wstring GetDescriptions();
    std::wstring GetSubjects();
    std::wstring GetCoverage();
    std::wstring GetIndentifier();
    std::wstring GetDate();
};

#endif // CBOOKINFO_H
