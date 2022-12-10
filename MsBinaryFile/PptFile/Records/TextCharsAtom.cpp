#include "TextCharsAtom.h"

using namespace PPT;

CRecordTextCharsAtom::CRecordTextCharsAtom()
{
}

CRecordTextCharsAtom::~CRecordTextCharsAtom()
{
}

void CRecordTextCharsAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_strText = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen / 2);
}
