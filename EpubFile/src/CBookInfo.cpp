#include "CBookInfo.h"
#include <iostream>

CBookInfo::CBookInfo()
{
}


CBookInfo::~CBookInfo()
{
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
                m_sCreator = oXmlLiteReader.GetText2();
            else if (sName == L"dc:publisher")
                m_sPublisher = oXmlLiteReader.GetText2();
            else if (sName == L"dc:language")
                m_sLanguage = oXmlLiteReader.GetText2();
            else if (sName == L"dc:contributor")
                m_sContributor = oXmlLiteReader.GetText2();
            else if (sName == L"dc:description")
                m_sDescription = oXmlLiteReader.GetText2();
            else if (sName == L"dc:coverage")
                m_sCoverage = oXmlLiteReader.GetText2();
        }
    }

    return true;
}

std::map <std::wstring, std::wstring> CBookInfo::GetBookInfo()
{
    std::map <std::wstring, std::wstring> mBookInfo;

    if (!m_sTitle.empty())
        mBookInfo.insert(std::make_pair(L"Title", m_sTitle));
    if (!m_sCreator.empty())
        mBookInfo.insert(std::make_pair(L"Creator", m_sCreator));
    if (!m_sCoverage.empty())
        mBookInfo.insert(std::make_pair(L"Coverage", m_sCoverage));
    if (!m_sLanguage.empty())
        mBookInfo.insert(std::make_pair(L"Language", m_sLanguage));
    if (!m_sPublisher.empty())
        mBookInfo.insert(std::make_pair(L"Publisher", m_sPublisher));
    if (!m_sContributor.empty())
         mBookInfo.insert(std::make_pair(L"Contributor", m_sContributor));
    if (!m_sDescription.empty())
         mBookInfo.insert(std::make_pair(L"Description", m_sDescription));

    return mBookInfo;
}

void CBookInfo::ShowInfo()
{
    std::map <std::wstring, std::wstring> mBookInfo = GetBookInfo();

    std::wcout << "-----DATA-----" << std::endl;
    for (auto item = mBookInfo.begin(); item != mBookInfo.end(); item++)
        std::wcout << item->first << " - " << item->second << std::endl;
//    std::wcout << "-----HREFS-----" << std::endl;
//    for (int i = 0; i < (int)m_vHrefs.size(); i++)
//        std::wcout << m_vHrefs[i] << std::endl;
}
