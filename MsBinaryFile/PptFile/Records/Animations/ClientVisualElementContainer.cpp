#include "ClientVisualElementContainer.h"

using namespace PPT;

CRecordClientVisualElementContainer::CRecordClientVisualElementContainer()
{
    m_bVisualPageAtom	=	false;
    m_bVisualShapeAtom	=	false;
}

CRecordClientVisualElementContainer::~CRecordClientVisualElementContainer()
{

}

void CRecordClientVisualElementContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

    SRecordHeader ReadHeader;
    if (ReadHeader.ReadFromStream(pStream) )
    {
        if ( RT_VisualPageAtom == ReadHeader.RecType )
        {
            m_bVisualPageAtom	=	true;
            m_oVisualPageAtom.ReadFromStream ( ReadHeader, pStream );
        }

        if ( RT_VisualShapeAtom == ReadHeader.RecType )
        {
            m_bVisualShapeAtom	=	true;
            m_oVisualShapeAtom.ReadFromStream ( ReadHeader, pStream );
        }
    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
