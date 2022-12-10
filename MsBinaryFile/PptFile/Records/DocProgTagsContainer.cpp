#include "DocProgTagsContainer.h"

using namespace PPT;

CRecordPP9DocBinaryTagExtension::~CRecordPP9DocBinaryTagExtension()
{
    for (auto* pEl : m_rgTextMasterStyleAtom)
    {
        RELEASEOBJECT(pEl)
    }
}

void CRecordPP9DocBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    UINT lCurLen		=	0;

    SRecordHeader ReadHeader;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen += 8 + ReadHeader.RecLen;
        switch (ReadHeader.RecType)
        {
        case RT_TextMasterStyle9Atom:
        {
            auto pStyle = new CRecordTextMasterStyle9Atom;
            pStyle->ReadFromStream(ReadHeader, pStream);
            m_rgTextMasterStyleAtom.push_back(pStyle);
            break;
        }
        case RT_TextDefaults9Atom:
        {
            m_textDefaultsAtom = new CRecordTextDefaults9Atom;
            m_textDefaultsAtom->ReadFromStream(ReadHeader, pStream);
            break;
        }
        case RT_OutlineTextProps9:
        {
            m_outlineTextPropsContainer = new CRecordOutlineTextProps9Container;
            m_outlineTextPropsContainer->ReadFromStream(ReadHeader, pStream);
            break;
        }
        case RT_BlipCollection9:
        {
            m_blipCollectionContainer = new CRecordBlipCollection9Container;
            m_blipCollectionContainer->ReadFromStream(ReadHeader, pStream);
            break;
        }
        default:
            IRecord* pRecord = CreateByType(ReadHeader);
            pRecord->ReadFromStream(ReadHeader, pStream);

            m_arRecords.push_back(pRecord);
            break;
        }

    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}

void CRecordPP10DocBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}

void CRecordPP11DocBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}

void CRecordPP12DocBinaryTagExtension::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}

CRecordDocProgBinaryTagSubContainerOrAtom::CRecordDocProgBinaryTagSubContainerOrAtom() :
    m_pTagName(nullptr), m_pTagContainer(nullptr)
{}

CRecordDocProgBinaryTagSubContainerOrAtom::~CRecordDocProgBinaryTagSubContainerOrAtom()
{
    RELEASEOBJECT(m_pTagName);
    RELEASEOBJECT(m_pTagContainer);
}

void CRecordDocProgBinaryTagSubContainerOrAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    LONG lPos = 0; StreamUtils::StreamPosition(lPos, pStream);

    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    if (m_oHeader.RecType == RT_ProgBinaryTag)
    {
        m_pTagName = new CRecordCString();
        m_pTagName->ReadFromStream(ReadHeader, pStream);

        SRecordHeader childHeader;
        if (!childHeader.ReadFromStream(pStream))
            return;

        if (m_pTagName->m_strText == ___PPT9) {
            m_pTagContainer = new CRecordPP9DocBinaryTagExtension();
            dynamic_cast<CRecordPP9DocBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        } else if (m_pTagName->m_strText == ___PPT10) {
            m_pTagContainer = new CRecordPP10DocBinaryTagExtension();
            dynamic_cast<CRecordPP10DocBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        } else if (m_pTagName->m_strText == ___PPT11) {
            m_pTagContainer = new CRecordPP11DocBinaryTagExtension();
            dynamic_cast<CRecordPP11DocBinaryTagExtension*>
                    (m_pTagContainer)->ReadFromStream(childHeader, pStream);
        } else if (m_pTagName->m_strText == ___PPT12) {
        m_pTagContainer = new CRecordPP12DocBinaryTagExtension();
        dynamic_cast<CRecordPP12DocBinaryTagExtension*>
                (m_pTagContainer)->ReadFromStream(childHeader, pStream);
    }

    } else if (m_oHeader.RecType == RT_ProgStringTag)
    {
        m_pTagContainer = new CRecordProgStringTagContainer();
        dynamic_cast<CRecordProgStringTagContainer*>
                (m_pTagContainer)->ReadFromStream(ReadHeader, pStream);
    }
    LONG lSeek = lPos + m_oHeader.RecLen;
    StreamUtils::StreamSeek(lSeek, pStream);
}

CRecordDocProgTagsContainer::CRecordDocProgTagsContainer()
{

}

CRecordDocProgTagsContainer::~CRecordDocProgTagsContainer()
{
    for (auto pEl : m_arrRgChildRec)
        RELEASEOBJECT(pEl)
}

void CRecordDocProgTagsContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader	=	oHeader;

    LONG lPos = 0;
    StreamUtils::StreamPosition ( lPos, pStream );

    LONG lCurLen(0);
    SRecordHeader ReadHeader;

    while (lCurLen < (LONG)m_oHeader.RecLen)
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;
        lCurLen +=	8 + ReadHeader.RecLen;
        CRecordDocProgBinaryTagSubContainerOrAtom* pRecord =
                new CRecordDocProgBinaryTagSubContainerOrAtom();
        pRecord->ReadFromStream(ReadHeader, pStream);
        m_arrRgChildRec.push_back(pRecord);
    }

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

CRecordPP9DocBinaryTagExtension *CRecordDocProgTagsContainer::getPP9DocBinaryTagExtension()
{
    for (auto* rec : m_arrRgChildRec)
        if (rec->m_pTagName->m_strText == ___PPT9)
            return (CRecordPP9DocBinaryTagExtension*)rec->m_pTagContainer;

    return nullptr;
}

IRecord *CRecordDocProgTagsContainer::getDocBinaryTagExtension(const std::wstring &extVersion)
{
    for (auto* rec : m_arrRgChildRec)
        if (rec->m_pTagName != nullptr && rec->m_pTagName->m_strText == extVersion)
            return rec->m_pTagContainer;

    return nullptr;
}
