#include "NotesPersistAtom.h"

using namespace PPT;

void CRecordNotesPersistAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
