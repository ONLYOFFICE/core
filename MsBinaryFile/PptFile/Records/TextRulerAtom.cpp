#include "TextRulerAtom.h"

using namespace PPT;

CRecordTextRulerAtom::CRecordTextRulerAtom()
{
}

CRecordTextRulerAtom::~CRecordTextRulerAtom()
{
}

void CRecordTextRulerAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    LONG lOffset = 0;
    StreamUtils::StreamPosition(lOffset, pStream);

    NSStreamReader::Read(pStream, m_oTextRuler);

    // на всякий случай...
    StreamUtils::StreamSeek(lOffset + m_oHeader.RecLen, pStream);
}
