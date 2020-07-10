#include "CBookInfo.h"
#include <iostream>

CBookInfo::CBookInfo()
{
}


CBookInfo::~CBookInfo()
{
//    delete m_XmlLiteReader;
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
            m_mBookInfo[sName.substr(3, sName.length())] = oXmlLiteReader.GetText2();
    }

    return true;
}

void CBookInfo::ReadManifest()
{
//    int nParentDepth = m_oXmlLiteReader->GetDepth();
//    while(m_oXmlLiteReader->ReadNextSiblingNode(nParentDepth))
//    {
//        if (m_oXmlLiteReader->GetAttributesCount() > 0 &&
//            m_oXmlLiteReader->MoveToFirstAttribute())
//        {
//            std::wstring sAttributeName = m_oXmlLiteReader->GetName();
//            while (!sAttributeName.empty())
//            {
//                std::wstring sAttributeValue = m_oXmlLiteReader->GetText();
//                if (sAttributeName == L"href")
//                {

//                    if ((sAttributeValue.find(L"/") != std::wstring::npos))
//                        sAttributeValue = sAttributeValue.substr(sAttributeValue.find_last_of(L"/") + 1, sAttributeValue.length());
//                    if (sAttributeValue.substr(sAttributeValue.find_last_of(L".") + 1, sAttributeValue.length()) == L"xhtml")
//                        m_vHrefs.push_back(sAttributeValue);
//                }
//                if (!m_oXmlLiteReader->MoveToNextAttribute())
//                    break;
//                sAttributeName = m_oXmlLiteReader->GetName();
//            }
//        }
//    }
}

std::map <std::wstring, std::wstring> CBookInfo::GetBookInfo()
{
    return m_mBookInfo;
}

void CBookInfo::ShowInfo()
{
    std::wcout << "-----DATA-----" << std::endl;
    for (auto item = m_mBookInfo.begin(); item != m_mBookInfo.end(); item++)
        std::wcout << item->first << " - " << item->second << std::endl;
//    std::wcout << "-----HREFS-----" << std::endl;
//    for (int i = 0; i < (int)m_vHrefs.size(); i++)
//        std::wcout << m_vHrefs[i] << std::endl;
}
