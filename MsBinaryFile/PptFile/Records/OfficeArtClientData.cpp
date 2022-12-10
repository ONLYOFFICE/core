#include "OfficeArtClientData.h"

using namespace PPT;

void CRecordShapeProgBinaryTagContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    SRecordHeader ReadHeader;
    m_rec.ReadFromStream(ReadHeader, pStream);

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

CRecordOfficeArtClientData::~CRecordOfficeArtClientData()
{
    CRecordsContainer::Clear();
    for ( size_t i = 0; i < m_rgShapeClientRoundtripData.size(); ++i )
        RELEASEOBJECT ( m_rgShapeClientRoundtripData[i] );
}

CRecordShapeProgBinaryTagSubContainerOrAtom *CRecordOfficeArtClientData::getProgTag(const std::wstring &tagname)
{
    for (auto* progtag : m_rgShapeClientRoundtripData)
    {
        if (progtag->m_pTagName->m_strText == tagname)
            return progtag;
    }

    return nullptr;
}

void CRecordOfficeArtClientData::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

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
        case RT_ProgTags:
        {
            // ShapeProgBinaryTagContainer
            ReadHeader.ReadFromStream(pStream);

            auto pRec = new CRecordShapeProgBinaryTagSubContainerOrAtom;
            pRec->ReadFromStream(ReadHeader, pStream);
            m_rgShapeClientRoundtripData.push_back(pRec);
            break;
        }
        case RT_VbaInfo:
        {
            auto pRec = new CRecordsContainer;
            pRec->ReadFromStream(ReadHeader, pStream);
            break;
        }
        default:
            IRecord* pRecord = CreateByType(ReadHeader);
            pRecord->ReadFromStream(ReadHeader, pStream);

            m_arRecords.push_back(pRecord);
            break;
        }

    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
