#include "TextBookmarkAtom.h"

using namespace PPT;

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
