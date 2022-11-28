#include "StylesWriter.h"


void CStylesWriter::ConvertStyles(PPT_FORMAT::CTextStyles &oStyles, PPT_FORMAT::CStringWriter &oWriter, int nCount)
{
    for (int i = 0; i < nCount; ++i)
    {
        if (oStyles.m_pLevels[i].is_init())
            ConvertStyleLevel(oStyles.m_pLevels[i].get(), oWriter, i);
    }
}

std::wstring CStylesWriter::ConvertStyleLevel(PPT_FORMAT::CTextStyleLevel &oLevel, const int &nLevel)
{
    PPT_FORMAT::CStringWriter oWriter;
    ConvertStyleLevel(oLevel, oWriter, nLevel);
    return oWriter.GetData();
}

std::wstring CStylesWriter::ConvertStyles(PPT_FORMAT::CTextStyles &oStyles, int nCount)
{
    PPT_FORMAT::CStringWriter oWriter;
    ConvertStyles(oStyles, oWriter, nCount);
    return oWriter.GetData();
}

std::wstring CStylesWriter::GetTextAnchor(const WORD &value)
{
    if (0 == value)			return L"t";
    if (1 == value)			return L"ctr";
    if (2 == value)			return L"b";
    return L"t";
}

std::wstring CStylesWriter::GetTextAlign(const WORD &value)
{
    if (0 == value)			return L"l";
    if (1 == value)			return L"ctr";
    if (2 == value)			return L"r";
    if (3 == value)			return L"just";
    if (4 == value)			return L"dist";
    if (5 == value)			return L"thaiDist";
    if (6 == value)			return L"justLow";
    return L"l";
}

std::wstring CStylesWriter::GetColorInScheme(const LONG &lIndex)
{
    switch (lIndex)
    {
    case 0:	 return L"phClr";
    case 1:	 return L"bg1";
    case 2:	 return L"tx1";
    case 3:	 return L"bg2";
    case 4:	 return L"tx2";
    case 5:	 return L"accent1";
    case 6:	 return L"accent2";
    case 7:	 return L"accent3";
    case 8:	 return L"accent4";
    case 9:	 return L"accent5";
    case 10: return L"accent6";
    case 11: return L"hlink";
    case 12: return L"folHlink";
    case 13: return L"lt1";
    case 14: return L"dk1";
    case 15: return L"lt2";
    case 16: return L"dk2";
        break;
    };
    return L"none";
}

std::wstring CStylesWriter::GetFontAlign(const WORD &value)
{
    if (0 == value)			return L"base";
    if (1 == value)			return L"t";
    if (2 == value)			return L"ctr";
    if (3 == value)			return L"b";
    return L"auto";
}
