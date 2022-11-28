#include "TextBookmarkAtom.h"


CRecordTextBookmarkAtom::CRecordTextBookmarkAtom()
{
}

CRecordTextBookmarkAtom::~CRecordTextBookmarkAtom()
{
}

void CRecordTextBookmarkAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
