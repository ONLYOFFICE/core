#include "TextCFExceptionAtom.h"

using namespace PPT;

CRecordTextCFExceptionAtom::CRecordTextCFExceptionAtom()
{
}

CRecordTextCFExceptionAtom::~CRecordTextCFExceptionAtom()
{
}

void CRecordTextCFExceptionAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPosition = 0;
    StreamUtils::StreamPosition(lPosition, pStream);

    m_oCFRun.LoadFromStream(pStream, false);

    // это на всякий случай...
    StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
}
