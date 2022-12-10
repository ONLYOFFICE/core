#include "DocumentAtom.h"

using namespace PPT;

void CRecordDocumentAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_oSlideSize.X = StreamUtils::ReadLONG(pStream);
    m_oSlideSize.Y = StreamUtils::ReadLONG(pStream);

    m_oNotesSize.X = StreamUtils::ReadLONG(pStream);
    m_oNotesSize.Y = StreamUtils::ReadLONG(pStream);

    m_oServerZoom.Number = StreamUtils::ReadLONG(pStream);
    m_oServerZoom.Denom = StreamUtils::ReadLONG(pStream);

    m_nNotesMasterPersistIDRef = (UINT)StreamUtils::ReadDWORD(pStream);
    m_nHandoutMasterPersistIDRef = (UINT)StreamUtils::ReadDWORD(pStream);

    m_nFirstSlideNum = StreamUtils::ReadWORD(pStream);
    m_nSlideSizeType = StreamUtils::ReadWORD(pStream);

    m_bSaveWithFonts	= StreamUtils::ReadBYTE(pStream);
    m_bOmitTitlePlace	= StreamUtils::ReadBYTE(pStream);
    m_bRightToLeft		= StreamUtils::ReadBYTE(pStream);
    m_bShowComments		= StreamUtils::ReadBYTE(pStream);
}
