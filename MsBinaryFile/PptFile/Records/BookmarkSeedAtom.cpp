#include "BookmarkSeedAtom.h"


void CRecordBookmarkSeedAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
