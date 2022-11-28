#include "SlideFlags10Atom.h"


void PPT_FORMAT::CRecordSlideFlags10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    _UINT32 data(StreamUtils::ReadLONG(pStream));

    m_fPreserveMaster           = ( 0x01 == ( 0x01 & ((BYTE)data) ) );
    m_fOverrideMasterAnimation  = ( 0x02 == ( 0x02 & ((BYTE)data) ) );
}
