#include "CBookContentItem.h"

CBookContentItem::CBookContentItem()
{
}

CBookContentItem::~CBookContentItem()
{
}

void CBookContentItem::Clear()
{
    m_sID    .clear();
    m_sLinear.clear();
}

bool CBookContentItem::ReadContentItem(XmlUtils::CXmlLiteReader& oXmlLiteReader, int depth)
{
    if (!oXmlLiteReader.ReadNextSiblingNode(depth) || oXmlLiteReader.GetAttributesCount() == 0)
        return false;

    while(oXmlLiteReader.MoveToNextAttribute())
    {
        std::wstring sAttributeName = oXmlLiteReader.GetName();
        if (sAttributeName == L"idref")
            m_sID = oXmlLiteReader.GetText();
        else if (sAttributeName == L"linear")
            m_sLinear = oXmlLiteReader.GetText();
    }
    oXmlLiteReader.MoveToElement();

    return true;
}
