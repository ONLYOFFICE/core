#include "NamedShowSlidesAtom.h"


void CRecordNamedShowSlidesAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
