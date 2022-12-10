#include "MouseInteractiveInfoContainer.h"

using namespace PPT;

void CRecordMouseInteractiveInfoContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    isOver = oHeader.RecInstance;

    SRecordHeader header;
    header.ReadFromStream(pStream);
    interactiveInfoAtom.ReadFromStream(header, pStream);

    if (m_oHeader.RecLen > 24)
    {
        header.ReadFromStream(pStream);
        macroNameAtom.reset(new CRecordCString);
        macroNameAtom->ReadFromStream(header, pStream);
    }
}
