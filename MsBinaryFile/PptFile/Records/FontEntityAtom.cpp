#include "FontEntityAtom.h"

using namespace PPT;

void CRecordFontEntityAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    //face name - utf16 string with 0, 64 bytes always allocated
    unsigned char utf16FaceName[64+2] = {};

    POLE::uint64 lReadByte = pStream->read(utf16FaceName, 64);

    if (sizeof(wchar_t) == 4)
    {
        ULONG lLen  = 0;
        for (lLen = 0; lLen < lReadByte; lLen +=2)
            if (utf16FaceName[lLen] == 0)break;

        lLen/=2;

        UTF32 *pStrUtf32 = new UTF32 [lLen + 1];
        pStrUtf32[lLen] = 0 ;

        const	UTF16 *pStrUtf16_Conv = (const UTF16 *) utf16FaceName;
                UTF32 *pStrUtf32_Conv =                 pStrUtf32;

         if (conversionOK == ConvertUTF16toUTF32 ( &pStrUtf16_Conv, &pStrUtf16_Conv[lLen]
                                           , &pStrUtf32_Conv, &pStrUtf32 [lLen]
                                           , strictConversion))
        {
            m_strFaceName = std::wstring((wchar_t*)pStrUtf32/*, lLen*/);
        }
        delete [] pStrUtf32;
     }
    else
    {
        m_strFaceName = std::wstring((wchar_t*)utf16FaceName/*, lReadByte/2*/); // по факту .. нули нам не нужны
    }
    m_lfCharSet = StreamUtils::ReadBYTE(pStream);

    BYTE Mem = 0;
    Mem = StreamUtils::ReadBYTE(pStream);
    m_bEmbedSubsetted = ((Mem & 0x01) == 0x01);

    Mem = 0;
    Mem = StreamUtils::ReadBYTE(pStream);

    m_bIsRaster = ((Mem & 0x01) == 0x01);
    m_bIsDevice = ((Mem & 0x02) == 0x02);
    m_bIsTrueType = ((Mem & 0x04) == 0x04);
    m_bIsNoFontSubstitution = ((Mem & 0x08) == 0x08);

    m_lfPitchAndFamily = StreamUtils::ReadBYTE(pStream);
}

void CRecordFontEmbedDataBlob::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
    pStream->read(data.first.get(), data.second);
}
