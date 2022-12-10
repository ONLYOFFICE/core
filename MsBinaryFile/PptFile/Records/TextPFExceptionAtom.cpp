#include "TextPFExceptionAtom.h"

using namespace PPT;

CRecordTextPFExceptionAtom::CRecordTextPFExceptionAtom()
{
}

CRecordTextPFExceptionAtom::~CRecordTextPFExceptionAtom()
{
}

void CRecordTextPFExceptionAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPosition = 0;
    StreamUtils::StreamPosition(lPosition, pStream);

    m_oPFRun.LoadFromStream(pStream, m_oHeader.RecLen == 12);

    // это на всякий случай...
    POLE::uint64 current_pos = pStream->tell();
    StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
}
