#include "BookmarkSeedAtom.h"

using namespace PPT;

void CRecordBookmarkSeedAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
