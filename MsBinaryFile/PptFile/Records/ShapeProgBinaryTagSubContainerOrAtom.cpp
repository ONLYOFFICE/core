#include "ShapeProgBinaryTagSubContainerOrAtom.h"

using namespace PPT;

void CRecordPP9ShapeBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    m_styleTextPropAtom.ReadFromStream(ReadHeader, pStream);
}

void CRecordPP10ShapeBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    m_styleTextPropAtom.ReadFromStream(ReadHeader, pStream);
}

void CRecordPP11ShapeBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    m_styleTextPropAtom.ReadFromStream(ReadHeader, pStream);
}

CRecordShapeProgBinaryTagSubContainerOrAtom::CRecordShapeProgBinaryTagSubContainerOrAtom() : m_pTagContainer(NULL), m_pTagName(NULL){}

void CRecordShapeProgBinaryTagSubContainerOrAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    LONG lPos = 0; StreamUtils::StreamPosition(lPos, pStream);

    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    if (ReadHeader.RecType == RT_CString)
    {
        RELEASEOBJECT(m_pTagName);
        RELEASEOBJECT(m_pTagContainer);
        m_pTagName = new CRecordCString();
        m_pTagName->ReadFromStream(ReadHeader, pStream);

        SRecordHeader childHeader;
        if (!childHeader.ReadFromStream(pStream))
            return;

        if (m_pTagName->m_strText == TN_PPT9) {
            m_pTagContainer = new CRecordPP9ShapeBinaryTagExtension();
            dynamic_cast<CRecordPP9ShapeBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        } else if (m_pTagName->m_strText == TN_PPT10) {
            m_pTagContainer = new CRecordPP10ShapeBinaryTagExtension();
            dynamic_cast<CRecordPP10ShapeBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        } else if (m_pTagName->m_strText == TN_PPT11) {
            m_pTagContainer = new CRecordPP11ShapeBinaryTagExtension();
            dynamic_cast<CRecordPP11ShapeBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        }
    }
}
