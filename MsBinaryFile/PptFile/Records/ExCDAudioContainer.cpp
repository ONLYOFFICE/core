#include "ExCDAudioContainer.h"

using namespace PPT;

void CRecordExCDAudioContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    SRecordHeader oExHeader;
    oExHeader.ReadFromStream(pStream);

    m_oMedia.ReadFromStream(oExHeader, pStream);

    StreamUtils::StreamSkip(8, pStream);

    BYTE nTrack1		= StreamUtils::ReadBYTE(pStream);
    BYTE nMinute1		= StreamUtils::ReadBYTE(pStream);
    BYTE nSecond1		= StreamUtils::ReadBYTE(pStream);
    BYTE nFrame1		= StreamUtils::ReadBYTE(pStream);

    BYTE nTrack2		= StreamUtils::ReadBYTE(pStream);
    BYTE nMinute2		= StreamUtils::ReadBYTE(pStream);
    BYTE nSecond2		= StreamUtils::ReadBYTE(pStream);
    BYTE nFrame2		= StreamUtils::ReadBYTE(pStream);

    m_dStartTime		= 60000 * nMinute1 + 1000 * nSecond1;
    m_dEndTime			= 60000 * nMinute2 + 1000 * nSecond2;
}
