#include "TimeConditionAtom.h"

using namespace PPT;

void CRecordTimeConditionAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_TriggerObject		=	( TriggerObjectEnum )StreamUtils::ReadDWORD ( pStream );

    m_nTriggerEvent		=	StreamUtils::ReadDWORD ( pStream );
    m_nID				=	StreamUtils::ReadDWORD ( pStream );
    m_nTimeDelay		=	StreamUtils::ReadLONG ( pStream );
}
