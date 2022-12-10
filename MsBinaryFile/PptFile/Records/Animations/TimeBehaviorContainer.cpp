#include "TimeBehaviorContainer.h"

using namespace PPT;

CRecordTimeBehaviorContainer::CRecordTimeBehaviorContainer() :
    m_pStringList(nullptr),
    m_pPropertyList(nullptr),

    m_haveStringList(false),
    m_havePropertyList(false)
{

}

CRecordTimeBehaviorContainer::~CRecordTimeBehaviorContainer()
{
    RELEASEOBJECT (m_pStringList);
    RELEASEOBJECT (m_pPropertyList);
}

void CRecordTimeBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);

    SRecordHeader ReadHeader;

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

    UINT lCurLen	=	8 + ReadHeader.RecLen;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if (ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen			+=	8 + ReadHeader.RecLen;

        RecordType nRecord	=	ReadHeader.RecType;

        switch (nRecord)
        {
        case RT_TimeVariantList:
        {
            m_pStringList = new CRecordTimeStringListContainer();
            m_pStringList->ReadFromStream(ReadHeader, pStream);
            m_haveStringList = true;
            break;
        }

        case RT_TimePropertyList:
        {
            m_pPropertyList = new CRecordTimePropertyList4TimeBehavior();
            m_pPropertyList->ReadFromStream(ReadHeader, pStream);
            m_havePropertyList = true;
            break;
        }

        case RT_TimeClientVisualElement:
        {
            m_oClientVisualElement.ReadFromStream( ReadHeader, pStream );
        }

        default:
            break ;
        }
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

_UINT32 CRecordTimeBehaviorContainer::GetObjectID()
{
    return m_oClientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;
}
