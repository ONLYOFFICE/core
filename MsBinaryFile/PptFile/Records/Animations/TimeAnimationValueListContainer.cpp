#include "TimeAnimationValueListContainer.h"

using namespace PPT;

CRecordTimeAnimationValueListContainer::CRecordTimeAnimationValueListContainer()
{

}

CRecordTimeAnimationValueListContainer::~CRecordTimeAnimationValueListContainer()
{
    for (auto pEntry : m_arrEntry)
    {
        RELEASEOBJECT(pEntry);
    }
}

void CRecordTimeAnimationValueListContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos		=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    UINT lCurLen	=	0;

    SRecordHeader ReadHeader;
    while ( lCurLen < m_oHeader.RecLen )
    {
        if ( ReadHeader.ReadFromStream(pStream) == false )
        {
            break;
        }

        lCurLen += 8 + ReadHeader.RecLen;

        auto  Entry = new CRecordTimeAnimationValueListEntry;
        Entry->ReadFromStream ( lCurLen, ReadHeader, pStream );

        m_arrEntry.push_back ( Entry );

    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
