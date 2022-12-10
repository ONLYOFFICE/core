#include "InteractiveInfoAtom.h"

using namespace PPT;

void CRecordInteractiveInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nSoundIdRef		= StreamUtils::ReadDWORD(pStream);
    m_nExHyperlinkIdRef	= StreamUtils::ReadDWORD(pStream);
    m_nAction		= StreamUtils::ReadBYTE(pStream);
    m_nOleVerb		= StreamUtils::ReadBYTE(pStream);
    m_nJump			= StreamUtils::ReadBYTE(pStream);

    BYTE nFlag = StreamUtils::ReadBYTE(pStream);

    m_bAnimated         = ((nFlag & 0x01) == 0x01);
    m_bStopSound        = ((nFlag & 0x02) == 0x02);
    m_bCustomShowReturn = ((nFlag & 0x04) == 0x04);
    m_bVisited          = ((nFlag & 0x08) == 0x08);

    m_nHyperlinkType = StreamUtils::ReadBYTE(pStream);
    StreamUtils::StreamSkip(3, pStream);
//        if (m_nExHyperlinkIdRef)
//            std::wcout << L"HyperlinkIDRef: " << m_nExHyperlinkIdRef << std::endl;;

}
