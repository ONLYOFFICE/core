/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "ReadStructures.h"

#include "../../DocFile/MemoryStream.h"
#include "../../../OfficeCryptReader/source/CryptTransform.h"

namespace PPT
{
static ODRAW::CColor GetStandartPaletteColor(int index)
{
    ODRAW::CColor color;
    static BYTE palette [56][3] =
    {
        {	0	,	0	,	0	},
        {	255	,	255	,	255	},
        {	255	,	0	,	0	},
        {	0	,	255	,	0	},
        {	0	,	0	,	255	},
        {	255	,	255	,	0	},
        {	255	,	0	,	255	},
        {	0	,	255	,	255	},
        {	128	,	0	,	0	},
        {	0	,	128	,	0	},
        {	0	,	0	,	128	},
        {	128	,	128	,	0	},
        {	128	,	0	,	128	},
        {	0	,	128	,	128	},
        {	192	,	192	,	192	},
        {	128	,	128	,	128	},
        {	153	,	153	,	255	},
        {	153	,	51	,	102	},
        {	255	,	255	,	204	},
        {	204	,	255	,	255	},
        {	102	,	0	,	102	},
        {	255	,	128	,	128	},
        {	0	,	102	,	204	},
        {	204	,	204	,	255	},
        {	0	,	0	,	128	},
        {	255	,	0	,	255	},
        {	255	,	255	,	0	},
        {	0	,	255	,	255	},
        {	128	,	0	,	128	},
        {	128	,	0	,	0	},
        {	0	,	128	,	128	},
        {	0	,	0	,	255	},
        {	0	,	204	,	255	},
        {	204	,	255	,	255	},
        {	204	,	255	,	204	},
        {	255	,	255	,	153	},
        {	153	,	204	,	255	},
        {	255	,	153	,	204	},
        {	204	,	153	,	255	},
        {	255	,	204	,	153	},
        {	51	,	102	,	255	},
        {	51	,	204	,	204	},
        {	153	,	204	,	0	},
        {	255	,	204	,	0	},
        {	255	,	153	,	0	},
        {	255	,	102	,	0	},
        {	102	,	102	,	153	},
        {	150	,	150	,	150	},
        {	0	,	51	,	102	},
        {	51	,	153	,	102	},
        {	0	,	51	,	0	},
        {	51	,	51	,	0	},
        {	153	,	51	,	0	},
        {	153	,	51	,	102	},
        {	51	,	51	,	153	}
    };
    color.SetRGB(palette[index][0], palette[index][1], palette[index][2]);
    return color;
}

namespace NSStreamReader
{
void Read(POLE::Stream* pStream, PPT::CTextRuler& oRun)
{
    double dScaleX = 625 * 2.54;
    //1/576 inch = 72/576 pt = 360000 *72 * 2.54 /(72*576) emu

    _UINT32 dwFlags = StreamUtils::ReadDWORD(pStream);
    BYTE flag1 = (BYTE)(dwFlags);
    BYTE flag2 = (BYTE)(dwFlags >> 8);
    BYTE flag3 = (BYTE)(dwFlags >> 16);
    BYTE flag4 = (BYTE)(dwFlags >> 24);

    bool bDefaultTabSize_ = (0x01 == (0x01 & flag1));
    bool bCLevels_ = (0x02 == (0x02 & flag1));
    bool bTabStops_ = (0x04 == (0x04 & flag1));

    bool bLeftMargin1_ = (0x08 == (0x08 & flag1));
    bool bLeftMargin2_ = (0x10 == (0x10 & flag1));
    bool bLeftMargin3_ = (0x20 == (0x20 & flag1));
    bool bLeftMargin4_ = (0x40 == (0x40 & flag1));
    bool bLeftMargin5_ = (0x80 == (0x80 & flag1));

    bool bIndent1_ = (0x01 == (0x01 & flag2));
    bool bIndent2_ = (0x02 == (0x02 & flag2));
    bool bIndent3_ = (0x04 == (0x04 & flag2));
    bool bIndent4_ = (0x08 == (0x08 & flag2));
    bool bIndent5_ = (0x10 == (0x10 & flag2));

    if (bCLevels_)
        oRun.CLevels = StreamUtils::ReadSHORT(pStream);
    if (bDefaultTabSize_)
        oRun.DefaultTabSize = (long)(StreamUtils::ReadSHORT(pStream) * dScaleX);

    if (bTabStops_)
    {
        WORD tabStopsCount = StreamUtils::ReadWORD(pStream);
        oRun.tabsStops.clear();

        for (WORD i = 0; i < tabStopsCount; ++i)
        {
            WORD tabPos = StreamUtils::ReadWORD(pStream);
            WORD tabType = StreamUtils::ReadWORD(pStream);

            if ((tabPos & 0xff00) == 0xff00)
                break;
            tabType = 0x0000;
            oRun.tabsStops.push_back(std::pair<int, int>(tabPos * dScaleX, tabType));
        }
    }

    if (bLeftMargin1_)	oRun.LeftMargin1 = StreamUtils::ReadSHORT(pStream) * dScaleX;
    if (bIndent1_)		oRun.Indent1 = StreamUtils::ReadSHORT(pStream) * dScaleX;

    if (bLeftMargin2_)	oRun.LeftMargin2 = StreamUtils::ReadSHORT(pStream) * dScaleX;
    if (bIndent2_)		oRun.Indent2 = StreamUtils::ReadSHORT(pStream) * dScaleX;

    if (bLeftMargin3_)	oRun.LeftMargin3 = StreamUtils::ReadSHORT(pStream) * dScaleX;
    if (bIndent3_)		oRun.Indent3 = StreamUtils::ReadSHORT(pStream) * dScaleX;

    if (bLeftMargin4_)	oRun.LeftMargin4 = StreamUtils::ReadSHORT(pStream) * dScaleX;
    if (bIndent4_)		oRun.Indent4 = StreamUtils::ReadSHORT(pStream) * dScaleX;

    if (bLeftMargin5_)	oRun.LeftMargin5 = StreamUtils::ReadSHORT(pStream) * dScaleX;
    if (bIndent5_)		oRun.Indent5 = StreamUtils::ReadSHORT(pStream) * dScaleX;
}
void Read(POLE::Stream* pStream, PPT::CTextSIRun& oRun, bool bIsIndentation)
{
    if (bIsIndentation)
    {
        oRun.lCount = StreamUtils::ReadDWORD(pStream);
    }

    _UINT32 dwFlags = StreamUtils::ReadDWORD(pStream);
    BYTE flag1 = (BYTE)(dwFlags);
    BYTE flag2 = (BYTE)(dwFlags >> 8);

    oRun.bSpell = (0x01 == (0x01 & flag1));
    oRun.bLang = (0x02 == (0x02 & flag1));
    oRun.bAltLang = (0x04 == (0x04 & flag1));
    // unused
    // unused
    oRun.bPp10ext = (0x20 == (0x20 & flag1));
    oRun.bBidi = (0x40 == (0x40 & flag1));
    // unused
    // reserved
    oRun.bSmartTag = (0x02 == (0x02 & flag2));

    if (oRun.bSpell)
    {
        oRun.Spell = StreamUtils::ReadWORD(pStream);
    }
    if (oRun.bLang)
    {
        oRun.Lang = StreamUtils::ReadWORD(pStream);
    }
    if (oRun.bAltLang)
    {
        oRun.AltLang = StreamUtils::ReadWORD(pStream);
    }
    if (oRun.bBidi)
    {
        oRun.Bidi = StreamUtils::ReadWORD(pStream);
    }
    if (oRun.bPp10ext)
    {
        _UINT32 dwFlags = StreamUtils::ReadDWORD(pStream);
        BYTE flag1 = (BYTE)(dwFlags);
        BYTE flag2 = (BYTE)(dwFlags >> 8);
        BYTE flag3 = (BYTE)(dwFlags >> 16);
        BYTE flag4 = (BYTE)(dwFlags >> 24);

        oRun.pp10runid = (0x0F & flag1);
        oRun.bGramma = (0x80 == (0x80 & flag4));
    }

    /*if (bSmartTag)
            {
            _UINT32 tabStopsCount = StreamUtils::ReadDWORD(pStream);
            arSmartTags.clear();

            for (int i = 0; i < (int)tabStopsCount; ++i)
            {
            arSmartTags.Add(StreamUtils::ReadDWORD(pStream));
            }
            }*/
}

void Read(POLE::Stream *pStream, ODRAW::SPointAtom &oAtom)
{
    oAtom.X = StreamUtils::ReadLONG(pStream);
    oAtom.Y = StreamUtils::ReadLONG(pStream);
}

void Read(POLE::Stream *pStream, ODRAW::SColorAtom &oAtom)
{
    oAtom.R = StreamUtils::ReadBYTE(pStream);
    oAtom.G = StreamUtils::ReadBYTE(pStream);
    oAtom.B = StreamUtils::ReadBYTE(pStream);
    oAtom.Index = StreamUtils::ReadBYTE(pStream);

    oAtom.bPaletteIndex = oAtom.bPaletteRGB = oAtom.bSystemRGB = oAtom.bSysIndex = oAtom.bSchemeIndex = false;

    if (oAtom.Index != 0xFF)
    {
        oAtom.bPaletteRGB = (oAtom.Index == 0xFE);
        oAtom.bSchemeIndex = (oAtom.Index != 0xFE);
    }
}

}

CTextPFRunRecord::CTextPFRunRecord() : m_oRun()
{
    m_lLevel = -1;
    m_lCount = 0;
}

CTextPFRunRecord::CTextPFRunRecord(const CTextPFRunRecord &oSrc)
{
    *this = oSrc;
}

CTextPFRunRecord &CTextPFRunRecord::operator=(const CTextPFRunRecord &oSrc)
{
    m_oRun = oSrc.m_oRun;
    m_lLevel = oSrc.m_lLevel;
    m_lCount = oSrc.m_lCount;
    return *this;
}

void CTextPFRunRecord::LoadFromStream(POLE::Stream* pStream, bool bIsIndentation)
{
    double dScaleX = 625 * 2.54;
    //1/576 inch = 72/576 pt = 360000 *72 * 2.54 /(72*576) emu

    if (bIsIndentation)
    {
        m_lCount = StreamUtils::ReadLONG(pStream);
        m_lLevel = (LONG)StreamUtils::ReadWORD(pStream);

        if (m_lLevel > 0x0004)
            m_lLevel = 0x0004;
    }

    _UINT32 dwFlags = StreamUtils::ReadDWORD(pStream);
    BYTE flag1 = (BYTE)(dwFlags);
    BYTE flag2 = (BYTE)(dwFlags >> 8);
    BYTE flag3 = (BYTE)(dwFlags >> 16);
    BYTE flag4 = (BYTE)(dwFlags >> 24);

    //флаги чтения
    bool hasBullet_ = (0x01 == (0x01 & flag1));
    bool bulletHasFont_ = (0x02 == (0x02 & flag1));
    bool bulletHasColor_ = (0x04 == (0x04 & flag1));
    bool bulletHasSize_ = (0x08 == (0x08 & flag1));

    bool bulletFontRef_ = (0x10 == (0x10 & flag1));
    bool bulletColor_ = (0x20 == (0x20 & flag1));
    bool bulletSize_ = (0x40 == (0x40 & flag1));
    bool bulletChar_ = (0x80 == (0x80 & flag1));

    bool leftMargin_ = (0x01 == (0x01 & flag2));
    // reserved
    bool indent_ = (0x04 == (0x04 & flag2));
    bool textAlignment_ = (0x08 == (0x08 & flag2));
    bool lineSpacing_ = (0x10 == (0x10 & flag2));
    bool spaceBefore_ = (0x20 == (0x20 & flag2));
    bool spaceAfter_ = (0x40 == (0x40 & flag2));
    bool defaultTabSize_ = (0x80 == (0x80 & flag2));

    bool fontAlign_ = (0x01 == (0x01 & flag3));
    bool charWrap_ = (0x02 == (0x02 & flag3));
    bool wordWrap_ = (0x04 == (0x04 & flag3));
    bool overflow_ = (0x08 == (0x08 & flag3));
    bool tabStops_ = (0x10 == (0x10 & flag3));
    bool textDirection_ = (0x20 == (0x20 & flag3));
    //reserved
    bool bulletBlip_ = (0x80 == (0x80 & flag3));

    bool bulletScheme_ = (0x01 == (0x01 & flag4));
    bool bulletHasScheme_ = (0x02 == (0x02 & flag4));

    WORD bulletFlag = 0;

    if (hasBullet_ || bulletHasFont_ || bulletHasColor_ || bulletHasSize_)
    {
        bulletFlag = StreamUtils::ReadWORD(pStream);
        if (bulletFlag & 0x0F)
        {
            m_oRun.hasBullet = (bool)(0x01 == (bulletFlag & 0x01));
        }
        else
            m_oRun.hasBullet = false;
    }

    if (bulletChar_)
    {
        unsigned short	utf16 = (unsigned short)StreamUtils::ReadWORD(pStream);

        if (utf16 > 0x0013)
        {
            if (sizeof(wchar_t) == 2)
            {
                m_oRun.bulletChar = utf16;
            }
            else
            {
                std::wstring	utf32 = NSFile::CUtf8Converter::GetWStringFromUTF16(&utf16, 1);
                if (!utf32.empty())
                    m_oRun.bulletChar = utf32.c_str()[0];
            }
        }

    }
    if (bulletFontRef_)
    {
        m_oRun.bulletFontRef = StreamUtils::ReadWORD(pStream);

        if (bulletFlag & 0x0F)
        {
            if (!(0x02 == (bulletFlag & 0x02)))
                m_oRun.bulletFontRef.reset();
        }
    }
    if (bulletSize_)
    {
        m_oRun.bulletSize = StreamUtils::ReadWORD(pStream);

        if (bulletFlag & 0x0F)
        {
            if (!(0x08 == (bulletFlag & 0x08)))
                m_oRun.bulletSize.reset();
        }
    }
    if (bulletColor_)
    {
        ODRAW::SColorAtom oColorAtom;
        NSStreamReader::Read(pStream, oColorAtom);

        ODRAW::CColor oColor;

        oColor.R = oColorAtom.R;
        oColor.G = oColorAtom.G;
        oColor.B = oColorAtom.B;
        oColor.A = 255;
        oColor.m_lSchemeIndex = -1;

        if (oColorAtom.bSchemeIndex)
        {
            oColor.m_lSchemeIndex = oColorAtom.Index;
            ODRAW::CorrectColorPPT(oColor.m_lSchemeIndex);
        }

        if (oColorAtom.bSchemeIndex || oColorAtom.bPaletteRGB)
        {
            m_oRun.bulletColor = oColor;

            if (bulletFlag & 0x0F)
            {
                if (!(0x04 == (bulletFlag & 0x04)))
                    m_oRun.bulletColor.reset();
            }
        }
    }

    if (textAlignment_)		m_oRun.textAlignment = StreamUtils::ReadWORD(pStream);
    if (lineSpacing_)		m_oRun.lineSpacing = -StreamUtils::ReadSHORT(pStream);
    if (spaceBefore_)		m_oRun.spaceBefore = -StreamUtils::ReadSHORT(pStream);
    if (spaceAfter_)		m_oRun.spaceAfter = -StreamUtils::ReadSHORT(pStream);
    if (leftMargin_)		m_oRun.leftMargin = StreamUtils::ReadSHORT(pStream)			* dScaleX;
    if (indent_)			m_oRun.indent = StreamUtils::ReadSHORT(pStream)			* dScaleX;
    if (defaultTabSize_)	m_oRun.defaultTabSize = (_INT32)StreamUtils::ReadWORD(pStream)	* dScaleX;
    if (tabStops_)
    {
        WORD tabStopsCount = StreamUtils::ReadWORD(pStream);
        m_oRun.tabStops.clear();

        tabStopsCount = tabStopsCount & 0x000f;

        for (int i = 0; i < (int)tabStopsCount; ++i)
        {
            WORD tabPos = StreamUtils::ReadWORD(pStream);
            WORD tabType = StreamUtils::ReadWORD(pStream);

            if ((tabPos & 0xff00) == 0xff00)
                break;

            m_oRun.tabStops.push_back(std::pair<int, int>(tabPos * dScaleX, tabType));
        }

        //if (0 < m_oRun.tabStops.size())
        //	m_oRun.defaultTabSize = m_oRun.tabStops[0];
    }

    if (fontAlign_)
        m_oRun.fontAlign = StreamUtils::ReadWORD(pStream);

    if (charWrap_ || wordWrap_ || overflow_)
    {
        m_oRun.wrapFlags = StreamUtils::ReadWORD(pStream);
    }

    if (textDirection_)
        m_oRun.textDirection = StreamUtils::ReadWORD(pStream);
}

CTextCFRunRecord::CTextCFRunRecord() : m_oRun()
{
    m_lCount = 0;
}

CTextCFRunRecord::CTextCFRunRecord(const CTextCFRunRecord &oSrc)
{
    *this = oSrc;
}

CTextCFRunRecord &CTextCFRunRecord::operator=(const CTextCFRunRecord &oSrc)
{
    m_oRun = oSrc.m_oRun;
    m_lCount = oSrc.m_lCount;
    return *this;
}

void CTextCFRunRecord::LoadFromStream(POLE::Stream* pStream, bool bIsIndentation)
{
    if (bIsIndentation)
    {
        m_lCount = StreamUtils::ReadLONG(pStream);
    }

    _UINT32 dwFlags = StreamUtils::ReadDWORD(pStream);
    BYTE flag1 = (BYTE)(dwFlags);
    BYTE flag2 = (BYTE)(dwFlags >> 8);
    BYTE flag3 = (BYTE)(dwFlags >> 16);
    BYTE flag4 = (BYTE)(dwFlags >> 24);

    bool hasBold = (0x01 == (0x01 & flag1));
    bool hasItalic = (0x02 == (0x02 & flag1));
    bool hasUnderline = (0x04 == (0x04 & flag1));
    // unused
    bool hasShadow = (0x10 == (0x10 & flag1));
    bool hasFehint = (0x20 == (0x20 & flag1));
    // unused
    bool hasKimi = (0x80 == (0x80 & flag1));

    // unused
    bool hasEmboss = (0x02 == (0x02 & flag2));
    // unused
    BYTE hasStyle = ((0x3C & flag2) >> 2);
    // unused

    bool typeface_ = (0x01 == (0x01 & flag3));
    bool size_ = (0x02 == (0x02 & flag3));
    bool color_ = (0x04 == (0x04 & flag3));
    bool BaseLineOffset_ = (0x08 == (0x08 & flag3));
    bool EAFontRef_ = (0x20 == (0x20 & flag3));
    bool AnsiFontRef_ = (0x40 == (0x40 & flag3));
    bool SymbolFontRef_ = (0x80 == (0x80 & flag3));

    bool hasNewEATypeface = (0x01 == (0x01 & flag4));
    bool hasCsTypeface = (0x02 == (0x02 & flag4));
    bool hasPp11ext = (0x04 == (0x04 & flag4));

    bool bIsFontStylePresent = (hasBold || hasItalic || hasUnderline || hasShadow ||
                                hasFehint || hasKimi || hasEmboss || hasStyle != 0);

    if (bIsFontStylePresent)
    {
        WORD fontStyle = StreamUtils::ReadWORD(pStream);

        m_oRun.FontBold = 0x01 == (0x01 & fontStyle);
        m_oRun.FontItalic = 0x02 == (0x02 & fontStyle);
        m_oRun.FontUnderline = 0x04 == (0x04 & fontStyle);
        m_oRun.FontShadow = 0x10 == (0x10 & fontStyle);
        m_oRun.pp9rt = (0x3c00 & fontStyle) >> 10;
    }

    if (typeface_)
        m_oRun.fontRef = StreamUtils::ReadWORD(pStream);

    if (EAFontRef_)
        m_oRun.eaFontRef = StreamUtils::ReadWORD(pStream);

    if (AnsiFontRef_)
        m_oRun.ansiFontRef = StreamUtils::ReadWORD(pStream);

    if (SymbolFontRef_)
        m_oRun.symbolFontRef = StreamUtils::ReadWORD(pStream);

    if (size_)
        m_oRun.Size = StreamUtils::ReadWORD(pStream);

    if (color_)
    {
        ODRAW::SColorAtom oColorAtom;
        NSStreamReader::Read(pStream, oColorAtom);

        ODRAW::CColor oColor;

        oColor.R = oColorAtom.R;
        oColor.G = oColorAtom.G;
        oColor.B = oColorAtom.B;
        oColor.A = 255;
        oColor.m_lSchemeIndex = -1;

        if (oColorAtom.Index < 8 && oColorAtom.bSchemeIndex)
        {
            oColor.m_lSchemeIndex = oColorAtom.Index;
            ODRAW::CorrectColorPPT(oColor.m_lSchemeIndex);
        }
        if (oColorAtom.bSchemeIndex || oColorAtom.bPaletteRGB)
        {
            m_oRun.Color = oColor;
        }
    }

    if (BaseLineOffset_)
        m_oRun.BaseLineOffset = (double)StreamUtils::ReadSHORT(pStream);

    // или два последних наоборот????
}


void ConvertPPTTextToEditorStructure(std::vector<CTextPFRunRecord>& oArrayPF, std::vector<CTextCFRunRecord>& oArrayCF,
                                     std::wstring& strText, PPT::CTextAttributesEx& oAttributes)
{
    int nCountPFs = (int)oArrayPF.size();
    int nCountCFs = (int)oArrayCF.size();

    oAttributes.m_arParagraphs.clear();
    oAttributes.m_originalText = strText;

    int nCurrentPF = 0;
    int nCurrentCF = 0;
    int nOffsetCF = 0;

    int nIndexLast = strText.length();
    int nIndexText = 0;

    for (int nIndexPF = 0; nIndexPF < nCountPFs; ++nIndexPF)
    {
        CParagraph elm;
        oAttributes.m_arParagraphs.push_back(elm);
        PPT::CParagraph* pPar = &oAttributes.m_arParagraphs[nIndexPF];

        pPar->m_lTextLevel = oArrayPF[nIndexPF].m_lLevel;
        pPar->m_oPFRun = oArrayPF[nIndexPF].m_oRun;

        pPar->m_lTextType = oAttributes.m_lTextType;
        pPar->m_lStyleThemeIndex = oAttributes.m_lStyleThemeIndex;

        int nCountInPF = oArrayPF[nIndexPF].m_lCount;

        while (true)
        {
            if (nCurrentCF >= nCountCFs)
                break;

            int nCountAdd = oArrayCF[nCurrentCF].m_lCount - nOffsetCF;

            int nCountAddTrue = nCountAdd;
            if (nIndexText + nCountAdd > nIndexLast)
            {
                nCountAddTrue = nIndexLast - nIndexText;
                if (nCountAddTrue < 0) nCountAddTrue = 0;
            }

            if (nCountAdd > nCountInPF)
            {
                nOffsetCF += nCountInPF;

                PPT::CSpan oSpan;
                oSpan.m_oRun = oArrayCF[nCurrentCF].m_oRun;

                nCountAddTrue = nCountInPF;
                if (nIndexText + nCountInPF > nIndexLast)
                {
                    nCountAddTrue = nIndexLast - nIndexText;
                    if (nCountAddTrue < 0) nCountAddTrue = 0;
                }
                if (nCountAddTrue > 0)
                {
                    oSpan.m_strText = strText.substr(nIndexText, nCountAddTrue);
                    pPar->m_arSpans.push_back(oSpan);
                }

                nIndexText += nCountInPF;
                break;
            }
            else if (nCountAdd == nCountInPF)
            {
                nOffsetCF = 0;

                PPT::CSpan oSpan;
                oSpan.m_oRun = oArrayCF[nCurrentCF].m_oRun;

                if (nCountAddTrue > 0)
                {
                    oSpan.m_strText = strText.substr(nIndexText, nCountAddTrue);
                    pPar->m_arSpans.push_back(oSpan);
                }
                nIndexText += nCountAdd;
                ++nCurrentCF;
                break;
            }
            else
            {
                nOffsetCF = 0;

                PPT::CSpan oSpan;
                oSpan.m_oRun = oArrayCF[nCurrentCF].m_oRun;

                if (nCountAddTrue > 0)
                {
                    oSpan.m_strText = strText.substr(nIndexText, nCountAddTrue);
                    pPar->m_arSpans.push_back(oSpan);
                }

                nIndexText += nCountAdd;
                nCountInPF -= nCountAdd;
                ++nCurrentCF;
            }
        }
    }
}
//------------------------------------------------------------------------------------
CMetaHeader::CMetaHeader()
{
}

void CMetaHeader::FromStream(POLE::Stream* pStream, CRYPT::ECMADecryptor *pDecryptor)
{
    BYTE	pData[34];
    pStream->read(pData, 34);

    if (pDecryptor)
    {
        pDecryptor->Decrypt((char*)pData, 34, 0);
    }
    MemoryStream memStream(pData, 34, false);

    cbSize = memStream.ReadUInt32();

    rcBounds.left = memStream.ReadInt32();
    rcBounds.top = memStream.ReadInt32();
    rcBounds.right = memStream.ReadInt32();
    rcBounds.bottom = memStream.ReadInt32();

    ptSize.x = memStream.ReadInt32();
    ptSize.y = memStream.ReadInt32();

    cbSave = memStream.ReadUInt32();

    compression = memStream.ReadByte();
    filter = memStream.ReadByte();
}

void CMetaHeader::ToEMFHeader(Gdiplus::ENHMETAHEADER3* pHeader)
{
    if (NULL == pHeader)
        return;

    pHeader->iType = 0x00000001;
    pHeader->nSize = 88;

    pHeader->rclBounds.left = rcBounds.left;
    pHeader->rclBounds.top = rcBounds.top;
    pHeader->rclBounds.right = rcBounds.right;
    pHeader->rclBounds.bottom = rcBounds.bottom;

    // нужно перевести в мм
    pHeader->rclFrame.left = rcBounds.left;
    pHeader->rclFrame.top = rcBounds.top;
    pHeader->rclFrame.right = rcBounds.right;
    pHeader->rclFrame.bottom = rcBounds.bottom;

    pHeader->dSignature = 0x464D4520;
    pHeader->nVersion = 0x00010000;
    pHeader->nBytes = cbSize;

    pHeader->nRecords = 1;
    pHeader->nHandles = 0;

    pHeader->sReserved = 0;

    pHeader->nDescription = 0;
    pHeader->offDescription = 0;

    pHeader->nPalEntries = 0;

    pHeader->szlDevice.cx = 200;
    pHeader->szlDevice.cy = 200;

    // нужно перевести в мм
    pHeader->szlMillimeters.cx = 100;
    pHeader->szlMillimeters.cy = 100;
}

void CMetaHeader::ToWMFHeader(Gdiplus::WmfPlaceableFileHeader* pHeader)
{
    if (NULL == pHeader)
        return;

    pHeader->Key = 0x9AC6CDD7;
    pHeader->Hmf = 0;

    pHeader->BoundingBox.Left = (short)rcBounds.left;
    pHeader->BoundingBox.Top = (short)rcBounds.top;
    pHeader->BoundingBox.Right = (short)rcBounds.right;
    pHeader->BoundingBox.Bottom = (short)rcBounds.bottom;

    pHeader->Inch = 1440; // 1:1
    pHeader->Reserved = 0;

    pHeader->Checksum = 0;
    pHeader->Checksum ^= (pHeader->Key & 0x0000FFFFL);
    pHeader->Checksum ^= ((pHeader->Key & 0xFFFF0000L) >> 16);

    pHeader->Checksum ^= pHeader->Hmf;

    pHeader->Checksum ^= pHeader->BoundingBox.Left;
    pHeader->Checksum ^= pHeader->BoundingBox.Top;
    pHeader->Checksum ^= pHeader->BoundingBox.Right;
    pHeader->Checksum ^= pHeader->BoundingBox.Bottom;

    pHeader->Checksum ^= pHeader->Inch;
    pHeader->Checksum ^= (pHeader->Reserved & 0x0000FFFFL);
    pHeader->Checksum ^= ((pHeader->Reserved & 0xFFFF0000L) >> 16);
}

#define CLIPSIZE 12
#define PIXMAPRECSIZE 50
#define HEADERSIZE 40
#define MAXCOLORTABLESIZE 256*8+8
#define OPCODEMISCSIZE 2+8+8+2	/* opcode+srcRect+dstRect+mode */
#define ENDOFPICTSIZE 2
#define PICSIZE PIXMAPRECSIZE + HEADERSIZE + MAXCOLORTABLESIZE + ENDOFPICTSIZE + OPCODEMISCSIZE + CLIPSIZE

void CMetaHeader::ToPICTHeader(BYTE *& pHeader, int & size)
{
    short 		myRowBytes;
    short 		*picPtr;
    //	short 		iii;
    //	long 		handleSize;

    myRowBytes = cbSize & 0x3fff;


    //Skip picSize and put out picFrame (10 bytes).
    picPtr = (short *)pHeader;

    *picPtr++ = (short)myRowBytes;

    *picPtr++ = (short)rcBounds.top;
    *picPtr++ = (short)rcBounds.left;
    *picPtr++ = (short)rcBounds.bottom;
    *picPtr++ = (short)rcBounds.right;

    // Put out header (30 bytes). This could be done from a resource or taken from an existing picture.
    *picPtr++ = 0x11;		/* Version opcode. */
    *picPtr++ = 0x2ff;	/* Version number. */
    *picPtr++ = 0xC00;	/* Header opcode. */
    *picPtr++ = 0xFFFF;	/* Put out PICT header version. */
    *picPtr++ = 0xFFFF;
    //The rest of the header is ignored--0 it out.
    for (int iii = 10; iii > 0; iii--)
        *picPtr++ = 0;		/* Write out 24 bytes of 0. */

    /* Put out current port's clip region. */
    *picPtr++ = 0x01;
    *picPtr++ = 0x0A;		/* Clip region only has bounds rectangle. */
    *picPtr++ = (short)rcBounds.top;
    *picPtr++ = (short)rcBounds.left;
    *picPtr++ = (short)rcBounds.bottom;
    *picPtr++ = (short)rcBounds.right;

    //filter ???
    //if(srcBits->pixelType == RGBDirect)
    //{			/* Must be 32-bits/pixel */
    ///* Put out opCode $9A, DirectBitsRect. */
    *picPtr++ = 0x9A;


    //	*picPtr++ = 0;	/* BaseAddr for direct pixMaps is 0x000000FF. */
    //	*picPtr++ = 0xFF;
    //	PutOutPixMapSrcRectDstRectAndMode(srcBits, &picPtr, srcRect,
    //		dstRect, mode);
    //	if(PutOutPackedDirectPixData(srcBits, &picPtr))		/* Nonzero
    //		indicates an error. */
    //		goto errorExit;
    //}
    //else
    //{
    /* Put out opCode $98, PackBitsRect. */
    //*picPtr++ = 0x98;
    //PutOutPixMapSrcRectDstRectAndMode(srcBits, &picPtr, srcRect, dstRect, mode);
    //if(PutOutPackedIndexedPixData(srcBits, &picPtr))
    //{
    //	/* Nonzero indicates an error. */
    //}

    //}
    int sz = ((BYTE*)picPtr - pHeader);
    size = sz;
}

CMetaFileBuffer::CMetaFileBuffer()
{
    m_bIsCompressed = false;
    m_bIsValid = false;

    m_pMetaHeader = NULL;
    m_pMetaFile = NULL;

    m_lMetaHeaderSize = 0;
    m_lMetaFileSize = 0;
}

CMetaFileBuffer::~CMetaFileBuffer()
{
    RELEASEARRAYOBJECTS(m_pMetaHeader);
    RELEASEARRAYOBJECTS(m_pMetaFile);

    if (m_bIsCompressed)
        RELEASEARRAYOBJECTS(m_pMetaFile);
    m_bIsCompressed = false;
}

void CMetaFileBuffer::SetHeader(BYTE *pHeader, LONG lSize)
{
    m_pMetaHeader = pHeader;
    m_lMetaHeaderSize = lSize;
}

void CMetaFileBuffer::SetData(BYTE *pCompress, LONG lCompressSize, LONG lUncompressSize, bool bIsCompressed)
{
    m_bIsCompressed = bIsCompressed;
    if (!m_bIsCompressed)
    {
        m_pMetaFile = pCompress;
        m_lMetaFileSize = lUncompressSize;
    }
    else
    {
        ULONG lSize = lUncompressSize;
        m_pMetaFile = new BYTE[lUncompressSize];
        bool bRes = NSZip::Decompress(pCompress, (ULONG)lCompressSize, m_pMetaFile, lSize);
        if (bRes)
        {
            m_lMetaFileSize = (LONG)lSize;
            m_bIsCompressed = true;
        }
        else
        {
            RELEASEARRAYOBJECTS(m_pMetaFile);

            m_pMetaFile = pCompress;
            m_lMetaFileSize = lUncompressSize;
            m_bIsCompressed = false;
        }
    }
}

void CMetaFileBuffer::ToFile(NSFile::CFileBinary *pFile)
{
    if (NULL != m_pMetaHeader)
    {
        pFile->WriteFile(m_pMetaHeader, m_lMetaHeaderSize);
    }
    if (NULL != m_pMetaFile)
    {
        pFile->WriteFile(m_pMetaFile, m_lMetaFileSize);
    }
}

void SScalingAtom::FromStream(POLE::Stream *pStream)
{
    X.FromStream(pStream);
    Y.FromStream(pStream);
}

void SRatioAtom::FromStream(POLE::Stream *pStream)
{
    Number = StreamUtils::ReadLONG(pStream);
    Denom = StreamUtils::ReadLONG(pStream);
}

void SFileIdCluster::ReadFromStream(POLE::Stream *pStream)
{
    DrawingGroupID = (UINT)StreamUtils::ReadDWORD(pStream);
    CurrentShapeID = (UINT)StreamUtils::ReadDWORD(pStream);
}

}
