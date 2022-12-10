#include "TimeCommandBehaviorAtom.h"

using namespace PPT;

void CRecordTimeCommandBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    _UINT32 flags = StreamUtils::ReadLONG(pStream);

    m_fTypePropertyUsed     =	( 0x01 == ( 0x01 & (flags) ) );
    m_fCommandPropertyUsed  =	( 0x02 == ( 0x02 & (flags) ) );


    m_eCommandBehaviorType  = (TimeCommandBehaviorTypeEnum)
                                StreamUtils::ReadLONG(pStream);
}
