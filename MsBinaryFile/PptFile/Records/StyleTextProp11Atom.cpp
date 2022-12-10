#include "StyleTextProp11Atom.h"

using namespace PPT;

void CRecordStyleTextProp11Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    LONG lCurPos		=	0;
    StreamUtils::StreamPosition ( lCurPos, pStream );

    while ( lPos + (LONG)m_oHeader.RecLen > lCurPos)
    {
        SStyleTextProp11 style;
        style.ReadFromStream(pStream);
        m_rgStyleTextProp11.push_back(style);

        StreamUtils::StreamPosition ( lCurPos, pStream );
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
