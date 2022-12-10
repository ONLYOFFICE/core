#include "SSDocInfoAtom.h"

using namespace PPT;

CRecordSSDocInfoAtom::CRecordSSDocInfoAtom()
{
}

CRecordSSDocInfoAtom::~CRecordSSDocInfoAtom()
{
}

void CRecordSSDocInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    unsigned short flags;
    unsigned char nm[64];

    NSStreamReader::Read(pStream, m_oPenColor);
    m_nRestartTime	= StreamUtils::ReadLONG(pStream);
    m_nStartSlide	= StreamUtils::ReadSHORT(pStream);
    m_nEndSlide		= StreamUtils::ReadSHORT(pStream);

    pStream->read(nm, 64);
    m_pName = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)nm, 32);

    flags = StreamUtils::ReadWORD(pStream);

    fAutoAdvance		= GETBIT(flags, 15);
    fWillSkipBuilds		= GETBIT(flags, 14);
    fUseSlideRange		= GETBIT(flags, 13);
    fDocUseNamedShow	= GETBIT(flags, 12);
    fBrowseMode			= GETBIT(flags, 11);
    fKioskMode			= GETBIT(flags, 10);
    fWillSkipNarration	= GETBIT(flags, 9);
    fLoopContinuously	= GETBIT(flags, 8);
    fHideScrollBar		= GETBIT(flags, 7);

    unsigned short unused = StreamUtils::ReadWORD(pStream);
}
