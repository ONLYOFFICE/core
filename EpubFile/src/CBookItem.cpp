#include "CBookItem.h"

CBookItem::CBookItem()
{
}

CBookItem::~CBookItem()
{
}

/*
void CBookItem::Clear()
{
    m_sID.clear();
    m_sRef.clear();
    m_sProperties.clear();
    m_eType = Default;
}
*/

bool CBookItem::ReadItem(XmlUtils::CXmlLiteReader& oXmlLiteReader, int depth)
{
    if (!oXmlLiteReader.ReadNextSiblingNode(depth) || oXmlLiteReader.GetAttributesCount() == 0)
        return false;

    while(oXmlLiteReader.MoveToNextAttribute())
    {
        std::wstring sAttributeName = oXmlLiteReader.GetName();
        std::wstring sAttributeValue = oXmlLiteReader.GetText();

        if (sAttributeName == L"href")
        {
            size_t posLastSlash = sAttributeValue.rfind(L'/');
            m_sRef = posLastSlash == std::wstring::npos ? sAttributeValue : sAttributeValue.substr(posLastSlash + 1);
        }
        else if (sAttributeName == L"id")
            m_sID = sAttributeValue;
        else if (sAttributeName == L"media-type")
        {
            if (sAttributeValue == L"image/jpeg")
                m_eType = ItemType::Image;
            else if (sAttributeValue == L"application/xhtml+xml")
                m_eType = ItemType::Xhtml;
            else if (sAttributeValue == L"text/css")
                m_eType = ItemType::Css;
            else if (sAttributeValue == L"application/x-dtbncx+xml")
                m_eType = ItemType::Toc;
            else if (sAttributeValue == L"application/x-font-ttf")
                m_eType = ItemType::Font;
        }
    }
    return true;
}

const std::wstring CBookItem::GetID() const
{
    return m_sID;
}

const std::wstring CBookItem::GetRef() const
{
    return m_sRef;
}
