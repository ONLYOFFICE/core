#include "CFMasks.h"

using namespace PPT;

void SCFMasks::ReadFromStream(POLE::Stream *pStream)
{
    BYTE data1  = StreamUtils::ReadBYTE(pStream);
    BYTE data2  = StreamUtils::ReadBYTE(pStream);
    BYTE data3  = StreamUtils::ReadBYTE(pStream);
    BYTE data4  = StreamUtils::ReadBYTE(pStream);

    m_bold              = 0x01 == (0x01 & data1);
    m_italic            = 0x02 == (0x02 & data1);
    m_underline         = 0x04 == (0x04 & data1);
    // unused
    m_shadow            = 0x10 == (0x10 & data1);
    m_fehint            = 0x20 == (0x20 & data1);
    // unused2
    m_kumi              = 0x80 == (0x80 & data1);
    // unused3

    m_emboss            = 0x02 == (0x02 & data2);
    m_fHasStyle         = 0x3C & data2; // 4 bits
    // unused 2 bits

    m_typeface          = 0x01 == (0x01 & data3);
    m_size              = 0x02 == (0x02 & data3);
    m_color             = 0x04 == (0x04 & data3);
    m_position          = 0x08 == (0x08 & data3);
    m_pp10ext           = 0x10 == (0x10 & data3);
    m_oldEATypeface     = 0x20 == (0x20 & data3);
    m_ansiTypeface      = 0x40 == (0x40 & data3);
    m_symbolTypeface    = 0x80 == (0x80 & data3);

    m_newEATypeface     = 0x01 == (0x01 & data4);
    m_csTypeface        = 0x02 == (0x02 & data4);
    m_pp11ext           = 0x04 == (0x04 & data4);
    // reserved 5 bits
}
