#include "BookmarkEntityAtom.h"

using namespace PPT;

void CRecordBookmarkEntityAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
