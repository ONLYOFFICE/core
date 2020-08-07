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

bool CBookInfo::ReadInfo(XmlUtils::CXmlLiteReader &oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

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
                m_sDate = oXmlLiteReader.GetText2();
        }
    }

    return true;
}

void CBookInfo::ShowInfo()
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

std::wstring CBookInfo::GetTitle()
{
    return m_sTitle;
}

std::wstring CBookInfo::GetCreators()
{
    if (m_arCreators.empty())
        return L"";

    std::wstring sCreators;
    for (int i = 0; i < (int)m_arCreators.size() - 1; i++)
        sCreators += m_arCreators[i] + L", ";
    sCreators += m_arCreators[m_arCreators.size() - 1];

    return sCreators;
}

std::wstring CBookInfo::GetPublishers()
{
    if (m_arPublishers.empty())
        return L"";

    std::wstring sPublishers;
    for (int i = 0; i < (int)m_arPublishers.size() - 1; i++)
        sPublishers += m_arPublishers[i] + L", ";
    sPublishers += m_arPublishers[m_arPublishers.size() - 1];

    return sPublishers;
}

std::wstring CBookInfo::GetLanguages()
{
    if (m_arLanguages.empty())
        return L"";

    std::wstring sLanguages;
    for (int i = 0; i < (int)m_arLanguages.size() - 1; i++)
        sLanguages += m_arLanguages[i] + L", ";
    sLanguages += m_arLanguages[m_arLanguages.size() - 1];

    return sLanguages;
}

std::wstring CBookInfo::GetContibutors()
{
    if (m_arContributors.empty())
        return L"";

    std::wstring sContibutors;
    for (int i = 0; i < (int)m_arContributors.size() - 1; i++)
        sContibutors += m_arContributors[i] + L", ";
    sContibutors += m_arContributors[m_arContributors.size() - 1];

    return sContibutors;
}

std::wstring CBookInfo::GetDescriptions()
{
    if (m_arDescriptions.empty())
        return L"";

    std::wstring sDescriptions;
    for (int i = 0; i < (int)m_arDescriptions.size() - 1; i++)
        sDescriptions += m_arDescriptions[i] + L", ";
    sDescriptions += m_arDescriptions[m_arDescriptions.size() - 1];

    return sDescriptions;
}

std::wstring CBookInfo::GetCoverage()
{
    return m_sCoverage;
}

std::wstring CBookInfo::GetSubjects()
{
    if (m_arSubjects.empty())
        return L"";

    std::wstring sSubjects;
    for (int i = 0; i < (int)m_arSubjects.size() - 1; i++)
        sSubjects += m_arSubjects[i] + L", ";
    sSubjects += m_arSubjects[m_arSubjects.size() - 1];

    return sSubjects;
}

std::wstring CBookInfo::GetIndentifier()
{
    return m_sIdentifier;
}

std::wstring CBookInfo::GetDate()
{
    return m_sDate;
}
