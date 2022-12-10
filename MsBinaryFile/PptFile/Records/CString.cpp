#include "CString.h"

using namespace PPT;

void CRecordCString::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_strText = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen / 2);
}
