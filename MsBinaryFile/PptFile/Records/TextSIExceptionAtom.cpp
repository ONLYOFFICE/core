#include "TextSIExceptionAtom.h"

using namespace PPT;

CRecordTextSIExceptionAtom::CRecordTextSIExceptionAtom()
{
}

CRecordTextSIExceptionAtom::~CRecordTextSIExceptionAtom()
{
}

void CRecordTextSIExceptionAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPosition = 0;
    StreamUtils::StreamPosition(lPosition, pStream);

    NSStreamReader::Read(pStream, m_oSIRun, false);

    // это на всякий случай...
    StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
}
