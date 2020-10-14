#include "CBookInfo.h"
#include <iostream>

CBookInfo::CBookInfo()
{
}

CBookInfo::~CBookInfo()
{
}

void CBookInfo::Clear()
{
    m_sTitle.clear();
    m_arCreators.clear();
    m_arPublishers.clear();
    m_arLanguages.clear();
    m_arContributors.clear();
    m_arDescriptions.clear();
    m_sCoverage.clear();
    m_arSubjects.clear();
    m_sIdentifier.clear();
    m_sDate.clear();
}

void CBookInfo::ReadInfo(XmlUtils::CXmlLiteReader& oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return;

    int nParentDepth = oXmlLiteReader.GetDepth();
    while(oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = oXmlLiteReader.GetName();
        if (oXmlLiteReader.GetNamespacePrefix() == L"dc")
        {
            if (sName == L"dc:title")
                m_sTitle = oXmlLiteReader.GetText2();
            else if (sName == L"dc:creator")
                m_arCreators.push_back(oXmlLiteReader.GetText2());
            else if (sName == L"dc:publisher")
                m_arPublishers.push_back(oXmlLiteReader.GetText2());
            else if (sName == L"dc:language")
                m_arLanguages.push_back(oXmlLiteReader.GetText2());
            else if (sName == L"dc:contributor")
                m_arContributors.push_back(oXmlLiteReader.GetText2());
            else if (sName == L"dc:description")
                m_arDescriptions.push_back(oXmlLiteReader.GetText2());
            else if (sName == L"dc:coverage")
                m_sCoverage = oXmlLiteReader.GetText2();
            else if (sName == L"dc:subject")
                m_arSubjects.push_back(oXmlLiteReader.GetText2());
            else if (sName == L"dc:identifier")
                m_sIdentifier = oXmlLiteReader.GetText2();
            else if (sName == L"dc:date")
            {
                m_sDate = oXmlLiteReader.GetText2();
                const size_t posSpace = m_sDate.find(L' ');
                if (posSpace != std::wstring::npos)
                    m_sDate[posSpace] = L'T';

                const size_t posPoint = m_sDate.find(L'.');
                if (posPoint != std::wstring::npos)
                    m_sDate = m_sDate.substr(0, posPoint) + L"Z";
            }
        }
    }
}

/*
void CBookInfo::ShowInfo() const
{
    std::wcout << "-----DATA-----" << std::endl;

    if (!m_sTitle.empty())
        std::wcout << L"   Title - " << GetTitle() << std::endl;

    if (!m_arCreators.empty())
        std::wcout << L"   Creators - " << GetCreators() << std::endl;

    if (!m_arPublishers.empty())
        std::wcout << L"   Publishers - " << GetPublishers() << std::endl;

    if (!m_arLanguages.empty())
        std::wcout << L"   Languages - " << GetLanguages() << std::endl;

    if (!m_arContributors.empty())
        std::wcout << L"   Contributors - " << GetContibutors() << std::endl;

    if (!m_arDescriptions.empty())
        std::wcout << L"   Descriptions - " << GetDescriptions() << std::endl;

    if (!m_arSubjects.empty())
        std::wcout << L"   Subjects - " << GetSubjects() << std::endl;

    if (!m_sCoverage.empty())
        std::wcout << L"   Coverage - " << GetCoverage() << std::endl;

    if (!m_sIdentifier.empty())
        std::wcout << L"   Identifier - " << GetIndentifier() << std::endl;

    if (!m_sDate.empty())
        std::wcout << L"   Date - " << GetDate() << std::endl;

//    std::wcout << "-----HREFS-----" << std::endl;
//    for (int i = 0; i < (int)m_vHrefs.size(); i++)
//        std::wcout << m_vHrefs[i] << std::endl;
}
*/

const std::wstring CBookInfo::GetTitle() const
{
    return m_sTitle;
}

const std::wstring CBookInfo::GetCreators() const
{
    if (m_arCreators.empty())
        return L"";
    std::wstring sCreators;
    for (const std::wstring& sCreator : m_arCreators)
        sCreators += sCreator + L"; ";
    sCreators.erase(sCreators.length() - 2, 2);
    return sCreators;
}

const std::wstring CBookInfo::GetPublishers() const
{
    if (m_arPublishers.empty())
        return L"";
    std::wstring sPublishers;
    for (const std::wstring& sPublisher : m_arPublishers)
        sPublishers += sPublisher + L", ";
    sPublishers.erase(sPublishers.length() - 2, 2);
    return sPublishers;
}

const std::wstring CBookInfo::GetLanguages() const
{
    if (m_arLanguages.empty())
        return L"";
    std::wstring sLanguages;
    for (const std::wstring& sLanguage : m_arLanguages)
        sLanguages += sLanguage + L", ";
    sLanguages.erase(sLanguages.length() - 2, 2);
    return sLanguages;
}

const std::wstring CBookInfo::GetContibutors() const
{
    if (m_arContributors.empty())
        return L"";
    std::wstring sContibutors;
    for (const std::wstring& sContibutor : m_arContributors)
        sContibutors += sContibutor + L", ";
    sContibutors.erase(sContibutors.length() - 2, 2);
    return sContibutors;
}

const std::wstring CBookInfo::GetDescriptions() const
{
    if (m_arDescriptions.empty())
        return L"";
    std::wstring sDescriptions;
    for (const std::wstring& sDescription : m_arDescriptions)
        sDescriptions += sDescription + L", ";
    sDescriptions.erase(sDescriptions.length() - 2, 2);
    return sDescriptions;
}

const std::wstring CBookInfo::GetCoverage() const
{
    return m_sCoverage;
}

const std::wstring CBookInfo::GetSubjects() const
{
    if (m_arSubjects.empty())
        return L"";
    std::wstring sSubjects;
    for (const std::wstring& sSubject : m_arSubjects)
        sSubjects += sSubject + L", ";
    sSubjects.erase(sSubjects.length() - 2, 2);
    return sSubjects;
}

const std::wstring CBookInfo::GetIndentifier() const
{
    return m_sIdentifier;
}

const std::wstring CBookInfo::GetDate() const
{
    return m_sDate;
}
