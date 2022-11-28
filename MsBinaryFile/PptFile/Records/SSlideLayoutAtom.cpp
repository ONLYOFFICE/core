#include "SSlideLayoutAtom.h"


CRecordSSlideLayoutAtom::CRecordSSlideLayoutAtom()
{
}

CRecordSSlideLayoutAtom::~CRecordSSlideLayoutAtom()
{
}

void CRecordSSlideLayoutAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
