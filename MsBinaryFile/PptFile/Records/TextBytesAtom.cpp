#include "TextBytesAtom.h"

using namespace PPT;

CRecordTextBytesAtom::CRecordTextBytesAtom()
{
}

CRecordTextBytesAtom::~CRecordTextBytesAtom()
{
}

void CRecordTextBytesAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    //UTF-16 Unicode  character whose high byte is 0x00.
    unsigned short	*pUTF16		= new unsigned short[m_oHeader.RecLen];
    unsigned char	*pUTF16_low = new unsigned char	[m_oHeader.RecLen];

    if (pUTF16 && pUTF16_low)
    {
        pStream->read(pUTF16_low, m_oHeader.RecLen);

        for (UINT i = 0 ; i < m_oHeader.RecLen; i++)
        {
            pUTF16[i] = pUTF16_low[i];
        }

        m_strText = NSFile::CUtf8Converter::GetWStringFromUTF16(pUTF16, m_oHeader.RecLen);

    }

    RELEASEARRAYOBJECTS(pUTF16_low);
    RELEASEARRAYOBJECTS(pUTF16);

    //std::string tmpStrTextA = StreamUtils::ReadCStringA(pStream, m_oHeader.RecLen);

    //std::wstring tmpStrTextW (tmpStrTextA.begin(), tmpStrTextA.end());

    //m_strText = std_string2string(tmpStrTextW);
}
