#include "NotesAtom.h"

using namespace PPT;

void CRecordNotesAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nSlideIDRef	= (UINT)StreamUtils::ReadDWORD(pStream);

    USHORT nFlag		= StreamUtils::ReadWORD(pStream);
    m_bMasterObjects	= ((nFlag & 0x01) == 0x01);
    m_bMasterScheme		= ((nFlag & 0x02) == 0x02);
    m_bMasterBackground = ((nFlag & 0x04) == 0x04);

    StreamUtils::StreamSkip(2, pStream);
}
