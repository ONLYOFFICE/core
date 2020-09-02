#include "CBookItem.h"

CBookItem::CBookItem()
{

}

CBookItem::~CBookItem()
{

}

void CBookItem::Clear()
{
    m_sID.clear();
    m_sRef.clear();
    m_sProperties.clear();
    m_eType = Default;
}

bool CBookItem::ReadItem(XmlUtils::CXmlLiteReader &oXmlLiteReader, const int& depth)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    if (oXmlLiteReader.ReadNextSiblingNode(depth))
    {
        if (oXmlLiteReader.GetAttributesCount() > 0 &&
            oXmlLiteReader.MoveToFirstAttribute())
        {
            std::wstring sAttributeName = oXmlLiteReader.GetName();

            while (!sAttributeName.empty())
            {
                const std::wstring& sAttributeValue = oXmlLiteReader.GetText();

                if (sAttributeName == L"href")
                {
                    const auto& posLastSlash = sAttributeValue.find_last_of(L'/');

                    if (posLastSlash == std::wstring::npos)
                        m_sRef = sAttributeValue;
                    else
                        m_sRef = sAttributeValue.substr(posLastSlash + 1, sAttributeValue.length() - 1);
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

                if (!oXmlLiteReader.MoveToNextAttribute())
                    break;

                sAttributeName = oXmlLiteReader.GetName();
            }
        }
        else
            return false;
    }
    else
        return false;
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
