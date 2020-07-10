#include "CBookToc.h"

#include <stdio.h>

CBookToc::CBookToc()
{

}

CBookToc::~CBookToc()
{
}

bool CBookToc::ReadToc(XmlUtils::CXmlLiteReader &oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    int nParentDepth = oXmlLiteReader.GetDepth();

    while(oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = oXmlLiteReader.GetName();
        if (sName == L"docTitle")
            ReadTitle(oXmlLiteReader);
        else if(sName == L"navMap")
            ReadMap(oXmlLiteReader);
    }

    return true;
}

bool CBookToc::ReadTitle(XmlUtils::CXmlLiteReader &oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    oXmlLiteReader.ReadNextNode();
    m_sTitle = oXmlLiteReader.GetText2();
//    std::wcout << m_sTitle << std::endl;
    return true;
}

bool CBookToc::ReadMap(XmlUtils::CXmlLiteReader &oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    ReadPoint(oXmlLiteReader);
}

std::wstring CBookToc::GetAttributeValue(XmlUtils::CXmlLiteReader &oXmlLiteReader, std::wstring sAttributeName)
{
    if (oXmlLiteReader.GetAttributesCount() > 0 &&
        oXmlLiteReader.MoveToFirstAttribute())
    {
        std::wstring _sAttributeName = oXmlLiteReader.GetName();

        while (!_sAttributeName.empty())
        {
            if (_sAttributeName == sAttributeName)
            {
                std::wstring sText = oXmlLiteReader.GetText();
                sText = sText.substr(sText.find_last_of(L"/") + 1, sText.length());
                oXmlLiteReader.MoveToElement();
                return sText;
            }
            if (!oXmlLiteReader.MoveToNextAttribute())
                break;

            _sAttributeName = oXmlLiteReader.GetName();
        }
        oXmlLiteReader.MoveToElement();
    }

    return L"";
}

//bool CBookToc::ReadPoint(XmlUtils::CXmlLiteReader &oXmlLiteReader)
//{
//    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
//        return false;

//    m_structData structData;

//    if (oXmlLiteReader.GetName() == L"navPoint")
//        structData.sID = GetAttributeValue(oXmlLiteReader, L"id");

//    int nParentDepth = oXmlLiteReader.GetDepth();
//    while(oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
//    {
//        std::wstring sName = oXmlLiteReader.GetName();
//        if (sName == L"navLabel")
//        {
//            oXmlLiteReader.ReadNextNode();
//            structData.sText = oXmlLiteReader.GetText2();
//        }
//        else if (sName == L"content")
//        {
//            structData.sRef = GetAttributeValue(oXmlLiteReader, L"src");
//        }
//    }


//    m_arData.push_back(structData);

//    return true;
//}

void CBookToc::AddStruct()
{
    if (m_arMap.size() == 4)
    {
        m_structData structData;
        for (int i = 0; i < (int)m_arMap.size(); i++)
        {
            if (m_arMap[i].first == L"id")
                structData.sID = m_arMap[i].second;
            else if (m_arMap[i].first == L"playOrder")
                structData.sPlayOrder = m_arMap[i].second;
            else if (m_arMap[i].first == L"text")
                structData.sText = m_arMap[i].second;
            else if (m_arMap[i].first == L"src")
                structData.sRef = m_arMap[i].second;
        }
        m_arData.push_back(structData);
        m_arMap.clear();
    }
}

bool CBookToc::ReadPoint(XmlUtils::CXmlLiteReader &oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    int nParentDepth = oXmlLiteReader.GetDepth();

    while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = oXmlLiteReader.GetName();

        if (sName == L"navPoint")
        {
            m_arMap.push_back({L"id", GetAttributeValue(oXmlLiteReader, L"id")});
            m_arMap.push_back({L"playOrder", GetAttributeValue(oXmlLiteReader, L"playOrder")});
            AddStruct();
        }
        else if (sName == L"navLabel")
        {
            oXmlLiteReader.ReadNextNode();
            m_arMap.push_back({L"text", oXmlLiteReader.GetText2()});
            AddStruct();
        }
        else if (sName == L"content")
        {
            m_arMap.push_back({L"src" ,GetAttributeValue(oXmlLiteReader, L"src")});
            AddStruct();
        }

        ReadPoint(oXmlLiteReader);
    }
}

void CBookToc::ShowToc()
{
//    FILE *fl = std::fopen("text.txt", "w");
    std::wcout << L"-----TOC-----" << std::endl;
    std::wcout << "Title - " << m_sTitle << std::endl;
    for (int i = 0; i < (int)m_arData.size(); i++)
    {
        std::wcout << m_arData[i].sPlayOrder << " - " << m_arData[i].sID << " - " << m_arData[i].sText << " - " << m_arData[i].sRef << std::endl;
//        std::wstring sl = m_arData[i].sText + L"\n";
//        std::fputws(sl.c_str() , fl);
    }
}


