#include "PlaceHolderAtom.h"

using namespace PPT;

void CRecordPlaceHolderAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nPosition = StreamUtils::ReadLONG(pStream);
    m_nPlacementID = StreamUtils::ReadBYTE(pStream);
    m_nSize = StreamUtils::ReadBYTE(pStream);

    StreamUtils::StreamSkip(2, pStream);
}
