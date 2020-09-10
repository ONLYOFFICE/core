#include "CBookToc.h"

//#include <stdio.h>

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

    const int& nParentDepth = oXmlLiteReader.GetDepth();

    while(oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        const std::wstring& sName = oXmlLiteReader.GetName();

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

    return true;
}

bool CBookToc::ReadMap(XmlUtils::CXmlLiteReader &oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    ReadPoint(oXmlLiteReader);
}

std::wstring CBookToc::GetAttributeValue(XmlUtils::CXmlLiteReader &oXmlLiteReader, const std::wstring& sAttributeName) const
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

                const auto& posLastSlash = sText.find_last_of(L"/");

                if (posLastSlash != std::wstring::npos)
                    sText = sText.substr(posLastSlash + 1, sText.length() - 1);

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

void CBookToc::AddStruct()
{
    if (m_arMap.size() == 4)
    {
        m_structData structData;

        for (const std::pair<std::wstring, std::wstring>& sValue : m_arMap)
        {
            if (sValue.first == L"id")
                structData.sID = sValue.second;
            else if (sValue.first == L"playOrder")
                structData.sPlayOrder = sValue.second;
            else if (sValue.first == L"text")
                structData.sText = sValue.second;
            else if (sValue.first == L"src")
                structData.sRef = sValue.second;
        }

        m_arData.push_back(structData);
        m_arMap.clear();
    }
}

bool CBookToc::ReadPoint(XmlUtils::CXmlLiteReader &oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    const int& nParentDepth = oXmlLiteReader.GetDepth();

    while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        const std::wstring& sName = oXmlLiteReader.GetName();

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

void CBookToc::ShowToc() const
{
    std::wcout << L"-----TOC-----" << std::endl;
    std::wcout << "Title - " << m_sTitle << std::endl;
    for (const m_structData& oData : m_arData)
    {
        std::wcout << oData.sPlayOrder << " - " << oData.sID << " - " << oData.sText << " - " << oData.sRef << std::endl;
//        std::wstring sl = m_arData[i].sText + L"\n";
//        std::fputws(sl.c_str() , fl);
    }
}

int CBookToc::GetCountToc() const
{
    return m_arData.size();
}

const std::pair<std::wstring, std::wstring> CBookToc::GetTextAndRef(const int& nIndex) const
{
    if (nIndex < 0 || nIndex > GetCountToc())
        return {L"", L""};

    return {m_arData[nIndex].sText, m_arData[nIndex].sRef};
}

void CBookToc::Clear()
{
    m_sTitle.clear();
    m_arMap.clear();
    m_arData.clear();
}

