#include "ExOleLinkAtom.h"

using namespace PPT;

void CRecordExOleLinkAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
