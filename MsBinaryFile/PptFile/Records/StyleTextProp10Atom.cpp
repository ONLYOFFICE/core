#include "StyleTextProp10Atom.h"

using namespace PPT;

void CRecordStyleTextProp10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    LONG lCurPos		=	0;
    StreamUtils::StreamPosition ( lCurPos, pStream );

    while ( lPos + (LONG)m_oHeader.RecLen > lCurPos)
    {
        STextCFException10 style;
        style.ReadFromStream(pStream);
        rgStyleTextProp10.push_back(style);

        StreamUtils::StreamPosition ( lCurPos, pStream );
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
