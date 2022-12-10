#include "TimeCommandBehaviorContainer.h"

using namespace PPT;

void CRecordTimeCommandBehaviorContainer::ReadFromStream(SRecordHeader &thisHeader, POLE::Stream *pStream)
{
    m_oHeader = thisHeader;

    SRecordHeader oHeader;
    if (oHeader.ReadFromStream(pStream))
        m_oCommandBehaviorAtom.ReadFromStream ( oHeader, pStream );

    if (oHeader.ReadFromStream(pStream))
        m_oVarCommand.ReadFromStream ( oHeader, pStream );

    if (oHeader.ReadFromStream(pStream))
        m_oBevavior.ReadFromStream ( oHeader, pStream );
}
