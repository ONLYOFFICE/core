#include "BookmarkEntityAtom.h"


void CRecordBookmarkEntityAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
