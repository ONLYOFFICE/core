#ifndef CBOOKINFO_H
#define CBOOKINFO_H

#include <vector>
#include <string>

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

    void ReadInfo(XmlUtils::CXmlLiteReader& oXmlLiteReader);
    // void ShowInfo() const;

    const std::wstring GetTitle() const;
    const std::wstring GetCreators() const;
    const std::wstring GetPublishers() const;
    const std::wstring GetLanguages() const;
    const std::wstring GetContibutors() const;
    const std::wstring GetDescriptions() const;
    const std::wstring GetSubjects() const;
    const std::wstring GetCoverage() const;
    const std::wstring GetIndentifier() const;
    const std::wstring GetDate() const;
};

#endif // CBOOKINFO_H
