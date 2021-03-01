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
wchar_t _is_hex_char(wchar_t ch)
{
    if(L'0' <= ch && ch <= L'9')
        return (ch - L'0');
    else if(L'A' <= ch && ch <= L'F')
        return (ch - L'A') + 10;
    else if(L'a' <= ch && ch <= L'f')
        return (ch - L'a') + 10;
    return static_cast<wchar_t>(-1);
}

wchar_t _hex_str2int(wchar_t* str)
{
    return _is_hex_char(str[0]) << 4 | _is_hex_char(str[1]);
}

std::wstring URLDecode(const std::wstring& src)
{
    std::wstring dst;
    wchar_t* buf = new wchar_t[3];
    buf[2] = L'\0';
    for(size_t i = 0; i < src.length(); i++)
    {
        if(src[i] == L'%')
        {
            buf[0] = src.at(++i);
            buf[1] = src.at(++i);
            dst.append(1, _hex_str2int(buf));
        } else
            dst.append(1, src.at(i));
    }
    delete buf;
    return dst;
}

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
            sAttributeValue = URLDecode(sAttributeValue);
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
