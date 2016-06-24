/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef READER_CLASSES
#define READER_CLASSES

#include "../../Common/ATLDefine.h"

namespace BinDocxRW {

class SectPr
{
public: 
	CString sHeaderFooterReference;
	double W;
	double H;
	BYTE cOrientation;
	double Left;
	double Top;
	double Right;
	double Bottom;
	double Header;
	double Footer;
	bool TitlePg;
	bool EvenAndOddHeaders;
	BYTE SectionType;
	int PageNumStart;
	CString sectPrChange;
	CString cols;
	CString pgBorders;

	bool bHeader;
	bool bFooter;
	bool bTitlePg;
	bool bEvenAndOddHeaders;
	bool bSectionType;
	bool bPageNumStart;
	SectPr()
	{
		W = Page_Width;
		H = Page_Height;
		cOrientation = orientation_Portrait;
		Left = 30;
		Top = 20;
		Right = 15;
		Bottom = 20;
		Header = 12.5;
		Footer = 12.5;
		sHeaderFooterReference = _T("");
		cols = _T("<w:cols w:space=\"708\"/>");

		bHeader = false;
		bFooter = false;
		bTitlePg = false;
		bEvenAndOddHeaders = false;
		bSectionType = false;
		bPageNumStart = false;
	}
	CString Write()
	{
		CString sRes = _T("");
		long nWidth = SerializeCommon::Round(W * g_dKoef_mm_to_twips);
		long nHeight = SerializeCommon::Round(H * g_dKoef_mm_to_twips);
		long nMLeft = SerializeCommon::Round(Left * g_dKoef_mm_to_twips);
		long nMTop = SerializeCommon::Round(Top * g_dKoef_mm_to_twips);
		long nMRight = SerializeCommon::Round(Right * g_dKoef_mm_to_twips);
		long nMBottom = SerializeCommon::Round(Bottom * g_dKoef_mm_to_twips);
		long nMHeader = SerializeCommon::Round(Header * g_dKoef_mm_to_twips);
		long nMFooter = SerializeCommon::Round(Footer * g_dKoef_mm_to_twips);
		if(!sHeaderFooterReference.IsEmpty())
			sRes.Append(sHeaderFooterReference);
		if(bSectionType)
		{
			CString sType;
			switch(SectionType)
			{
			case 0: sType = _T("continuous");break;
			case 1: sType = _T("evenPage");break;
			case 2: sType = _T("nextColumn");break;
			case 3: sType = _T("nextPage");break;
			case 4: sType = _T("oddPage");break;
			default: sType = _T("nextPage");break;
			}
            sRes.AppendFormat(_T("<w:type w:val=\"%ls\"/>"), (const TCHAR *) sType);
		}
		if(orientation_Portrait == cOrientation)
			sRes.AppendFormat(_T("<w:pgSz w:w=\"%d\" w:h=\"%d\"/>"), nWidth, nHeight);
		else
			sRes.AppendFormat(_T("<w:pgSz w:w=\"%d\" w:h=\"%d\" w:orient=\"landscape\"/>"), nWidth, nHeight);
		sRes.AppendFormat(_T("<w:pgMar w:top=\"%d\" w:right=\"%d\" w:bottom=\"%d\" w:left=\"%d\" w:gutter=\"0\""), nMTop, nMRight, nMBottom, nMLeft);
		if(bHeader)
			sRes.AppendFormat(_T(" w:header=\"%d\""), nMHeader);
		if(bFooter)
			sRes.AppendFormat(_T(" w:footer=\"%d\""), nMFooter);
		sRes.Append(_T("/>"));
		if(!pgBorders.IsEmpty())
			sRes.Append(pgBorders);
		if(bPageNumStart)
			sRes.AppendFormat(_T("<w:pgNumType w:start=\"%d\"/>"), PageNumStart);
		if(!cols.IsEmpty())
			sRes.Append(cols);
		sRes.Append(_T("<w:docGrid w:linePitch=\"360\"/>"));
		if(bTitlePg && TitlePg)
			sRes.Append(_T("<w:titlePg/>"));
		if(!sectPrChange.IsEmpty())
			sRes.Append(sectPrChange);
		return sRes;
	}
};
class docRGB
{
public:
	BYTE R;
	BYTE G;
	BYTE B;
	docRGB()
	{
		R = 255;
		G = 255;
		B = 255;
	}
	CString ToString()
	{
		CString sR;sR.Format(_T("%02X"), R);
		CString sG;sG.Format(_T("%02X"), G);
		CString sB;sB.Format(_T("%02X"), B);
		return sR + sG + sB;
	}
};
class CThemeColor{
public:
	bool Auto;
	BYTE Color;
	BYTE Tint;
	BYTE Shade;

	bool bShade;
	bool bTint;
	bool bColor;
	CThemeColor(){
		Reset();
	}
	void Reset()
	{
		bShade = false;
		bTint = false;
		bColor = false;
		Auto = false;
	}
	bool IsNoEmpty()
	{
		return bShade || bTint || bColor || Auto;
	}
	CString ToStringColor()
	{
		CString sRes;
		if(bColor)
		{
			switch(Color)
			{
			case 0: sRes = _T("accent1");break;
			case 1: sRes = _T("accent2");break;
			case 2: sRes = _T("accent3");break;
			case 3: sRes = _T("accent4");break;
			case 4: sRes = _T("accent5");break;
			case 5: sRes = _T("accent6");break;
			case 6: sRes = _T("background1");break;
			case 7: sRes = _T("background2");break;
			case 8: sRes = _T("dark1");break;
			case 9: sRes = _T("dark2");break;
			case 10: sRes = _T("followedHyperlink");break;
			case 11: sRes = _T("hyperlink");break;
			case 12: sRes = _T("light1");break;
			case 13: sRes = _T("light2");break;
			case 14: sRes = _T("none");break;
			case 15: sRes = _T("text1");break;
			case 16: sRes = _T("text2");break;
			default : sRes = _T("none");break;
			}
		}
		return sRes;
	}
	CString ToStringTint()
	{
		CString sRes;
		if(bTint)
		{
			if(Tint > 0xF)
				sRes.AppendFormat(_T("%X"), Tint);
			else
				sRes.AppendFormat(_T("0%X"), Tint);
		}
		return sRes;
	}
	CString ToStringShade()
	{
		CString sRes;
		if(bShade)
		{
			if(Shade > 0xF)
				sRes.AppendFormat(_T("%X"), Shade);
			else
				sRes.AppendFormat(_T("0%X"), Shade);
		}
		return sRes;
	}
};
class Spacing
{
public:
	bool bLineRule;
	bool bLine;
	bool bAfter;
	bool bBefore;
	bool bAfterAuto;
	bool bBeforeAuto;

	BYTE LineRule;
	double Line;
	double After;
	double Before;
	bool AfterAuto;
	bool BeforeAuto;
	Spacing()
	{
		bLineRule = false;
		bLine = false;
		bAfter = false;
		bBefore = false;
		bAfterAuto = false;
		bBeforeAuto = false;
	}
};
class Shd
{
public:
	BYTE Value;
	docRGB Color;
	CThemeColor ThemeColor; 

	bool bColor;
	bool bThemeColor;
	Shd()
	{
		Value = shd_Nil;
		bColor = false;
		bThemeColor = false;
	}
	CString ToString()
	{
		CString sShd;
		if(bColor || (bThemeColor && ThemeColor.IsNoEmpty()))
		{
			sShd.Append(_T("<w:shd w:val=\"clear\" w:color=\"auto\""));
			if(bColor)
                sShd.AppendFormat(_T(" w:fill=\"%ls\""), (const TCHAR *) Color.ToString());
			if(bThemeColor && ThemeColor.IsNoEmpty())
			{
				if(ThemeColor.Auto && !bColor)
					sShd.Append(_T(" w:fill=\"auto\""));
				if(ThemeColor.bColor)
                    sShd.AppendFormat(_T(" w:themeFill=\"%ls\""), (const TCHAR *) ThemeColor.ToStringColor());
				if(ThemeColor.bTint)
                    sShd.AppendFormat(_T(" w:themeFillTint=\"%ls\""), (const TCHAR *) ThemeColor.ToStringTint());
				if(ThemeColor.bShade)
                    sShd.AppendFormat(_T(" w:themeFillShade=\"%ls\""), (const TCHAR *) ThemeColor.ToStringShade());
			}
			sShd.Append(_T("/>"));
		}
		return sShd;
	}
};
class Tab
{
public:
	BYTE Val;
	double Pos;
	Tab()
	{
		Val = shd_Nil;
		Pos = 0;
	}
};
class Tabs
{
public:
	std::vector<Tab> m_aTabs;
};
class rPr
{
private:
	std::map<CString, int>& m_mapFonts;
public:
	bool Bold;
	bool Italic;
	bool Underline;
	bool Strikeout;
	CString FontAscii;
	CString FontHAnsi;
	CString FontAE;
	CString FontCS;
	long FontSize;
	docRGB Color;
	BYTE VertAlign;
	docRGB HighLight;
	CString Shd;
	CString RStyle;
	double Spacing;
	bool DStrikeout;
	bool Caps;
	bool SmallCaps;
	double Position;
	BYTE FontHint;
	bool BoldCs;
	bool ItalicCs;
	long FontSizeCs;
	bool Cs;
	bool Rtl;
	CString Lang;
	CString LangBidi;
	CString LangEA;
	CThemeColor ThemeColor;
	bool Vanish;
	CString Outline;
	CString Fill;
	CString Del;
	CString Ins;
	CString rPrChange;

	bool bBold;
	bool bItalic;
	bool bUnderline;
	bool bStrikeout;
	bool bFontAscii;
	bool bFontHAnsi;
	bool bFontAE;
	bool bFontCS;
	bool bFontSize;
	bool bColor;
	bool bVertAlign;
	bool bHighLight;
	bool bShd;
	bool bRStyle;
	bool bSpacing;
	bool bDStrikeout;
	bool bCaps;
	bool bSmallCaps;
	bool bPosition;
	bool bFontHint;
	bool bBoldCs;
	bool bItalicCs;
	bool bFontSizeCs;
	bool bCs;
	bool bRtl;
	bool bLang;
	bool bLangBidi;
	bool bLangEA;
	bool bThemeColor;
	bool bVanish;

	bool bDoNotWriteNullProp;
public:
	rPr(std::map<CString, int>& mapFonts) : m_mapFonts(mapFonts)
	{
		Reset();

		bDoNotWriteNullProp = false;
	}
	void Reset()
	{
		Bold = false;
		Italic = false;
		Underline = false;
		Strikeout = false;
		DStrikeout = false;
		Caps = false;
		SmallCaps = false;

		bBold = false;
		bItalic = false;
		bUnderline = false;
		bStrikeout = false;
		bFontAscii = false;
		bFontHAnsi = false;
		bFontAE = false;
		bFontCS = false;
		bFontSize = false;
		bColor = false;
		bVertAlign = false;
		bHighLight = false;
		bShd = false;
		bRStyle = false;
		bSpacing = false;
		bDStrikeout = false;
		bCaps = false;
		bSmallCaps = false;
		bPosition = false;
		bFontHint = false;
		bBoldCs = false;
		bItalicCs = false;
		bFontSizeCs = false;
		bCs = false;
		bRtl = false;
		bLang = false;
		bLangBidi = false;
		bLangEA = false;
		bThemeColor = false;
		ThemeColor.Reset();
		bVanish = false;
		Outline.Empty();
		Fill.Empty();
		Del.Empty();
		Ins.Empty();
		rPrChange.Empty();
	}
	bool IsNoEmpty()
	{
		return bBold || bItalic || bUnderline || bStrikeout || bFontAscii || bFontHAnsi || bFontAE || bFontCS || bFontSize || bColor || bVertAlign || bHighLight || bShd ||
			bRStyle || bSpacing || bDStrikeout || bCaps || bSmallCaps || bPosition || bFontHint || bBoldCs || bItalicCs || bFontSizeCs || bCs || bRtl || bLang || bLangBidi || bLangEA || bThemeColor || bVanish ||
			!Outline.IsEmpty() || !Fill.IsEmpty() || !Del.IsEmpty() || !Ins.IsEmpty() || !rPrChange.IsEmpty();
	}
	void Write(XmlUtils::CStringWriter*  pCStringWriter)
	{
		pCStringWriter->WriteString(CString(_T("<w:rPr>")));
		if(bRStyle)
		{
            CString sRStyle;sRStyle.Format(_T("<w:rStyle w:val=\"%ls\" />"), RStyle);
			pCStringWriter->WriteString(sRStyle);
		}
		if(bFontAscii || bFontHAnsi || bFontAE || bFontCS || bFontHint)
		{
			CString sFont = _T("<w:rFonts");
			if(bFontAscii)
			{
                sFont.AppendFormat(_T(" w:ascii=\"%ls\""), (const TCHAR *) FontAscii);
				m_mapFonts[FontAscii] = 1;
			}
			if(bFontHAnsi)
			{
                sFont.AppendFormat(_T(" w:hAnsi=\"%ls\""), (const TCHAR *) FontHAnsi);
				m_mapFonts[FontHAnsi] = 1;
			}
			if(bFontCS)
			{
                sFont.AppendFormat(_T(" w:cs=\"%ls\""), (const TCHAR *) FontCS);
				m_mapFonts[FontCS] = 1;
			}
			if(bFontAE)
			{
                sFont.AppendFormat(_T(" w:eastAsia=\"%ls\""), (const TCHAR *) FontAE);
				m_mapFonts[FontAE] = 1;
			}
			if(bFontHint)
			{
				switch(FontHint)
				{
				case 0: sFont.Append(_T(" w:hint=\"cs\""));break;
				case 2: sFont.Append(_T(" w:hint=\"eastAsia\""));break;
				default:sFont.Append(_T(" w:hint=\"default\""));break;
				}
			}
			sFont += _T("/>");
			pCStringWriter->WriteString(sFont);
		}
		if(bBold)
		{
			if(Bold)
				pCStringWriter->WriteString(CString(_T("<w:b />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:b w:val=\"false\"/>")));
		}
		if(bBoldCs)
		{
			if(BoldCs)
				pCStringWriter->WriteString(CString(_T("<w:bCs />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:bCs w:val=\"false\"/>")));
		}
		if(bItalic)
		{
			if(Italic)
				pCStringWriter->WriteString(CString(_T("<w:i />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:i w:val=\"false\"/>")));
		}
		if(bItalicCs)
		{
			if(ItalicCs)
				pCStringWriter->WriteString(CString(_T("<w:iCs />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:iCs w:val=\"false\"/>")));
		}
		if(bCaps)
		{
			if(Caps)
				pCStringWriter->WriteString(CString(_T("<w:caps />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:caps w:val=\"false\"/>")));
		}
		if(bSmallCaps)
		{
			if(SmallCaps)
				pCStringWriter->WriteString(CString(_T("<w:smallCaps />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:smallCaps w:val=\"false\"/>")));
		}
		if(bStrikeout)
		{
			if(Strikeout)
				pCStringWriter->WriteString(CString(_T("<w:strike/>")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:strike w:val=\"false\"/>")));
		}
		if(bDStrikeout)
		{
			if(DStrikeout)
				pCStringWriter->WriteString(CString(_T("<w:dstrike />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:dstrike w:val=\"false\"/>")));
		}
		if(bVanish)
		{
			if(Vanish)
				pCStringWriter->WriteString(CString(_T("<w:vanish />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:vanish w:val=\"false\"/>")));
		}
		if(bColor || (bThemeColor && ThemeColor.IsNoEmpty()))
		{
			CString sColor(_T("<w:color"));
			if(bColor)
                sColor.AppendFormat(_T(" w:val=\"%ls\""), (const TCHAR *) Color.ToString());
			if(bThemeColor && ThemeColor.IsNoEmpty())
			{
				if(ThemeColor.Auto && !bColor)
					sColor.Append(_T(" w:val=\"auto\""));
				if(ThemeColor.bColor)
                    sColor.AppendFormat(_T(" w:themeColor=\"%ls\""), (const TCHAR *) ThemeColor.ToStringColor());
				if(ThemeColor.bTint)
                    sColor.AppendFormat(_T(" w:themeTint=\"%ls\""), (const TCHAR *) ThemeColor.ToStringTint());
				if(ThemeColor.bShade)
                    sColor.AppendFormat(_T(" w:themeShade=\"%ls\""), (const TCHAR *) ThemeColor.ToStringShade());
			}
			sColor.Append(_T("/>"));
			pCStringWriter->WriteString(sColor);
		}
		if(bSpacing)
		{
			long nSpacing = SerializeCommon::Round( g_dKoef_mm_to_twips * Spacing);
			CString sSpacing;sSpacing.Format(_T("<w:spacing w:val=\"%d\"/>"), nSpacing);
			pCStringWriter->WriteString(sSpacing);
		}
		if(bPosition)
		{
			long nPosition = SerializeCommon::Round( g_dKoef_mm_to_hps * Position);
			CString sPosition;sPosition.Format(_T("<w:position w:val=\"%d\"/>"), nPosition);
			pCStringWriter->WriteString(sPosition);
		}
		if(bFontSize)
		{
			CString sFontSize;sFontSize.Format(_T("<w:sz w:val=\"%d\" />"), FontSize);
			pCStringWriter->WriteString(sFontSize);
		}
		if(bFontSizeCs)
		{
			CString sFontSize;sFontSize.Format(_T("<w:szCs w:val=\"%d\" />"), FontSizeCs);
			pCStringWriter->WriteString(sFontSize);
		}
		if(bHighLight)
		{
			docRGB& H = HighLight;
			CString sColor;
			if(0x00 == H.R && 0x00 == H.G && 0x00 == H.B )
				sColor = _T("black");
			else if(0x00 == H.R && 0x00 == H.G && 0xFF == H.B )
				sColor = _T("blue");
			else if(0x00 == H.R && 0xFF == H.G && 0xFF == H.B )
				sColor = _T("cyan");
			else if(0x00 == H.R && 0x00 == H.G && 0x8B == H.B )
				sColor = _T("darkBlue");
			else if(0x00 == H.R && 0x8B == H.G && 0x8B == H.B )
				sColor = _T("darkCyan");
			else if(0xA9 == H.R && 0xA9 == H.G && 0xA9 == H.B )
				sColor = _T("darkGray");
			else if(0x00 == H.R && 0x64 == H.G && 0x00 == H.B )
				sColor = _T("darkGreen");
			else if(0x80 == H.R && 0x00 == H.G && 0x80 == H.B )
				sColor = _T("darkMagenta");
			else if(0x8B == H.R && 0x00 == H.G && 0x00 == H.B )
				sColor = _T("darkRed");
			else if(0x80 == H.R && 0x80 == H.G && 0x00 == H.B )
				sColor = _T("darkYellow");
			else if(0x00 == H.R && 0xFF == H.G && 0x00 == H.B )
				sColor = _T("green");
			else if(0xD3 == H.R && 0xD3 == H.G && 0xD3 == H.B )
				sColor = _T("lightGray");
			else if(0xFF == H.R && 0x00 == H.G && 0xFF == H.B )
				sColor = _T("magenta");
			else if(0xFF == H.R && 0x00 == H.G && 0x00 == H.B )
				sColor = _T("red");
			else if(0xFF == H.R && 0xFF == H.G && 0xFF == H.B )
				sColor = _T("white");
			else if(0xFF == H.R && 0xFF == H.G && 0x00 == H.B )
				sColor = _T("yellow");
			if(!sColor.IsEmpty())
			{
				CString sHighLight;
                sHighLight.Format(_T("<w:highlight w:val=\"%ls\" />"), sColor);
				pCStringWriter->WriteString(sHighLight);
			}
			else if(g_nCurFormatVersion < 5)
			{
				//добавляем как shading
				CString sShd;
                sShd.Format(_T("<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"%ls\"/>"), H.ToString());
				pCStringWriter->WriteString(sShd);
			}
		}
		if(bUnderline)
		{
			if(Underline)
				pCStringWriter->WriteString(CString(_T("<w:u w:val=\"single\"/>")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:u w:val=\"none\"/>")));
		}
		if(bShd)
			pCStringWriter->WriteString(Shd);
		if(bVertAlign)
		{
			switch(VertAlign)
			{
			case vertalign_Baseline:
				if(false == bDoNotWriteNullProp)
					pCStringWriter->WriteString(CString(_T("<w:vertAlign w:val=\"baseline\" /> ")));
				break;
			case vertalign_SuperScript:pCStringWriter->WriteString(CString(_T("<w:vertAlign w:val=\"superscript\" /> ")));break;
			case vertalign_SubScript:pCStringWriter->WriteString(CString(_T("<w:vertAlign w:val=\"subscript\" /> ")));break;
			}
		}
		if(bRtl)
		{
			if(Rtl)
				pCStringWriter->WriteString(CString(_T("<w:rtl />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:rtl w:val=\"false\"/>")));
		}
		if(bCs)
		{
			if(Cs)
				pCStringWriter->WriteString(CString(_T("<w:cs />")));
			else if(false == bDoNotWriteNullProp)
				pCStringWriter->WriteString(CString(_T("<w:cs w:val=\"false\"/>")));
		}
		if(bLang || bLangBidi || bLangEA)
		{
			pCStringWriter->WriteString(CString(_T("<w:lang")));
			if(bLang)
			{
                CString sLang;sLang.Format(_T(" w:val=\"%ls\""), Lang);
				pCStringWriter->WriteString(sLang);
			}
			if(bLangBidi)
			{
                CString sLang;sLang.Format(_T(" w:bidi=\"%ls\""), LangBidi);
				pCStringWriter->WriteString(sLang);
			}
			if(bLangEA)
			{
                CString sLang;sLang.Format(_T(" w:eastAsia=\"%ls\""), LangEA);
				pCStringWriter->WriteString(sLang);
			}
			pCStringWriter->WriteString(CString(_T("/>")));
		}
		if (!Outline.IsEmpty())
			pCStringWriter->WriteString(Outline);
		if (!Fill.IsEmpty())
			pCStringWriter->WriteString(Fill);
		if (!Del.IsEmpty())
			pCStringWriter->WriteString(Del);
		if (!Ins.IsEmpty())
			pCStringWriter->WriteString(Ins);
		if (!rPrChange.IsEmpty())
			pCStringWriter->WriteString(rPrChange);
		pCStringWriter->WriteString(CString(_T("</w:rPr>")));
	}
};
class docStyle
{
public:
	CString Name;
	CString Id;
	BYTE byteType;
	bool bDefault;
	CString BasedOn;
	CString NextId;
	bool qFormat;
	long uiPriority;
	bool hidden;
	bool semiHidden;
	bool unhideWhenUsed;
	CString TextPr;
	CString ParaPr;
	CString TablePr;
	CString RowPr;
	CString CellPr;
	std::vector<CString> TblStylePr;

	bool bqFormat;
	bool buiPriority;
	bool bhidden;
	bool bsemiHidden;
	bool bunhideWhenUsed;
public:
	docStyle()
	{
		byteType = styletype_Paragraph;
		bDefault = false;

		bqFormat = false;
		buiPriority = false;
		bhidden = false;
		bsemiHidden = false;
		bunhideWhenUsed = false;
	}
	void Write(XmlUtils::CStringWriter*  pCStringWriter)
	{
		CString sType;
		switch(byteType)
		{
		case styletype_Character: sType = _T("character");break;
		case styletype_Numbering: sType = _T("numbering");break;
		case styletype_Table: sType = _T("table");break;
		default: sType = _T("paragraph");break;
		}
		if(!Id.IsEmpty())
		{
			CString sStyle;
			if(bDefault)
                sStyle.Format(_T("<w:style w:type=\"%ls\" w:default=\"1\" w:styleId=\"%ls\">"), sType, Id);
			else
                sStyle.Format(_T("<w:style w:type=\"%ls\" w:styleId=\"%ls\">"), sType, Id);
			pCStringWriter->WriteString(CString(sStyle));
			if(!Name.IsEmpty())
			{
                sStyle.Format(_T("<w:name w:val=\"%ls\"/>"), Name);
				pCStringWriter->WriteString(CString(sStyle));
			}
			if(!BasedOn.IsEmpty())
			{
                sStyle.Format(_T("<w:basedOn w:val=\"%ls\"/>"), BasedOn);
				pCStringWriter->WriteString(CString(sStyle));
			}
			if(!NextId.IsEmpty())
			{
                sStyle.Format(_T("<w:next w:val=\"%ls\"/>"), NextId);
				pCStringWriter->WriteString(CString(sStyle));
			}
			if(bqFormat)
			{
				if(qFormat)
					pCStringWriter->WriteString(CString(_T("<w:qFormat/>")));
				else
					pCStringWriter->WriteString(CString(_T("<w:qFormat val=\"false\"/>")));
			}
			if(buiPriority)
			{
				sStyle.Format(_T("<w:uiPriority w:val=\"%d\"/>"), uiPriority);
				pCStringWriter->WriteString(CString(sStyle));
			}
			if(bhidden)
			{
				if(hidden)
					pCStringWriter->WriteString(CString(_T("<w:hidden/>")));
				else
					pCStringWriter->WriteString(CString(_T("<w:hidden val=\"false\"/>")));
			}
			if(bsemiHidden)
			{
				if(semiHidden)
					pCStringWriter->WriteString(CString(_T("<w:semiHidden/>")));
				else
					pCStringWriter->WriteString(CString(_T("<w:semiHidden val=\"false\"/>")));
			}
			if(bunhideWhenUsed)
			{
				if(unhideWhenUsed)
					pCStringWriter->WriteString(CString(_T("<w:unhideWhenUsed/>")));
				else
					pCStringWriter->WriteString(CString(_T("<w:unhideWhenUsed val=\"false\"/>")));
			}
			if(!TextPr.IsEmpty())
			{
				pCStringWriter->WriteString(TextPr);
			}
			if(!ParaPr.IsEmpty())
			{
				pCStringWriter->WriteString(CString(_T("<w:pPr>")));
				pCStringWriter->WriteString(ParaPr);
				pCStringWriter->WriteString(CString(_T("</w:pPr>")));
			}
			if(!TablePr.IsEmpty())
				pCStringWriter->WriteString(TablePr);
			if(!RowPr.IsEmpty())
			{
				pCStringWriter->WriteString(CString(_T("<w:trPr>")));
				pCStringWriter->WriteString(RowPr);
				pCStringWriter->WriteString(CString(_T("</w:trPr>")));
			}
			if(!CellPr.IsEmpty())
			{
				pCStringWriter->WriteString(CString(_T("<w:tcPr>")));
				pCStringWriter->WriteString(CellPr);
				pCStringWriter->WriteString(CString(_T("</w:tcPr>")));
			}
			for(int i = 0, length = TblStylePr.size(); i < length; ++i)
			{
				pCStringWriter->WriteString(TblStylePr[i]);
			}
			pCStringWriter->WriteString(CString(_T("</w:style>")));
		}
	}
};
class tblStylePr
{
public:
	XmlUtils::CStringWriter Writer;
	BYTE Type;
	bool bType;
public:
	tblStylePr()
	{
		bType = false;
	}
};
class PaddingsToWrite
{
public:
	double Left;
	double Top;
	double Right;
	double Bottom;

	bool bLeft;
	bool bTop;
	bool bRight;
	bool bBottom;
public: PaddingsToWrite()
		{
			bLeft = false;
			bTop = false;
			bRight = false;
			bBottom = false;
		}
};
class docImg
{
public:
	long MediaId;
	BYTE Type;
	double X;
	double Y;
	double Width;
	double Height;
	PaddingsToWrite Paddings;
	int m_nDocPr;

	bool bMediaId;
	bool bType;
	bool bX;
	bool bY;
	bool bWidth;
	bool bHeight;
	bool bPaddings;
	CString srId;
	docImg(int nDocPr)
	{
		m_nDocPr = nDocPr;
		bMediaId = false;
		bType = false;
		bX = false;
		bY = false;
		bWidth = false;
		bHeight = false;
		bPaddings = false;
	}
	void Write(XmlUtils::CStringWriter*  pCStringWriter)
	{
		if(bType)
		{
			if(c_oAscWrapStyle::Inline == Type)
			{
				if(bWidth && bHeight)
				{
					__int64 nWidth = (__int64)(g_dKoef_mm_to_emu * Width);
					__int64 nHeight = (__int64)(g_dKoef_mm_to_emu * Height);
					CString sDrawing;
                    sDrawing.Format(_T("<w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"%lld\" cy=\"%lld\"/><wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/><wp:docPr id=\"%d\" name=\"Image\"/><wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"0\" name=\"Image\"/><pic:cNvPicPr/></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"%ls\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:stretch><a:fillRect/></a:stretch></pic:blipFill><pic:spPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"%d\" cy=\"%d\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing>"), nWidth, nHeight, m_nDocPr, srId, nWidth, nHeight);
					pCStringWriter->WriteString(sDrawing);
				}
			}
			else if(c_oAscWrapStyle::Flow == Type)
			{
				if(bX && bY && bWidth && bHeight)
				{
					__int64 nX = (__int64)(g_dKoef_mm_to_emu * X);
					__int64 nY = (__int64)(g_dKoef_mm_to_emu * Y);
					__int64 nWidth = (__int64)(g_dKoef_mm_to_emu * Width);
					__int64 nHeight = (__int64)(g_dKoef_mm_to_emu * Height);
					unsigned long nPaddingLeft = 0;
					unsigned long nPaddingTop = 0;
					unsigned long nPaddingRight = 0;
					unsigned long nPaddingBottom = 0;
					if(bPaddings)
					{
						if(Paddings.bLeft)		nPaddingLeft	= (unsigned long)(g_dKoef_mm_to_emu * Paddings.Left);
						if(Paddings.bTop)		nPaddingTop		= (unsigned long)(g_dKoef_mm_to_emu * Paddings.Top);
						if(Paddings.bRight)		nPaddingRight	= (unsigned long)(g_dKoef_mm_to_emu * Paddings.Right);
						if(Paddings.bBottom)	nPaddingBottom	= (unsigned long)(g_dKoef_mm_to_emu * Paddings.Bottom);

					}
					CString sDrawing;
                    sDrawing.Format(_T("<w:drawing><wp:anchor distT=\"%lu\" distB=\"%lu\" distL=\"%lu\" distR=\"%lu\" simplePos=\"0\" relativeHeight=\"1\" behindDoc=\"0\" locked=\"0\" layoutInCell=\"1\" allowOverlap=\"1\"><wp:simplePos x=\"0\" y=\"0\"/><wp:positionH relativeFrom=\"page\"><wp:posOffset>%lld</wp:posOffset></wp:positionH><wp:positionV relativeFrom=\"page\"><wp:posOffset>%lld</wp:posOffset></wp:positionV><wp:extent cx=\"%lld\" cy=\"%lld\"/><wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/><wp:wrapSquare wrapText=\"bothSides\"/><wp:docPr id=\"%d\" name=\"Image\"/><wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"0\" name=\"Image\"/><pic:cNvPicPr/></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"%ls\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:stretch><a:fillRect/></a:stretch></pic:blipFill><pic:spPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"%lld\" cy=\"%lld\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:anchor></w:drawing>"), nPaddingTop, nPaddingBottom, nPaddingLeft, nPaddingRight, nX, nY, nWidth, nHeight, m_nDocPr, srId, nWidth, nHeight);
					pCStringWriter->WriteString(sDrawing);
				}
			}
		}
	}
};
//class tblPr
//{
//public:
//	tblPr()
//	{
//	}
//	void Write(CStringWriter*  pCStringWriter)
//	{
//	}
//};
class docW
{
public:
	BYTE Type;
	double W;
	long WDocx;

	bool bType;
	bool bW;
	bool bWDocx;
	docW()
	{
		bType = false;
		bW = false;
		bWDocx = false;
	}
	void Write(XmlUtils::CStringWriter& pCStringWriter, const CString& sName)
	{
		pCStringWriter.WriteString(Write(sName));
	}
	CString Write(const CString& sName)
	{
		CString sXml;
		if(bW || (bType && bWDocx))
		{
			CString sType;
			int nVal;
			if(bW)
			{
				sType = _T("dxa");
				nVal = SerializeCommon::Round( g_dKoef_mm_to_twips * W);
			}
			else
			{
				switch(Type)
				{
				case 0: sType = _T("auto");break;
				case 1: sType = _T("dxa");break;
				case 2: sType = _T("nil");break;
				case 3: sType = _T("pct");break;
				}
				nVal = WDocx;
			}
            sXml.Format(_T("<%ls w:w=\"%d\" w:type=\"%ls\"/>"), sName, nVal, sType );
		}
		return sXml;
	}
};
class docBorder
{
public:
	docRGB Color;
	double Space;
	double Size;
	BYTE Value;
	CThemeColor ThemeColor;

	bool bColor;
	bool bSpace;
	bool bSize;
	bool bValue;
	bool bThemeColor;
	docBorder()
	{
		bColor = false;
		bSpace = false;
		bSize = false;
		bValue = false;
		bThemeColor = false;
	}
	void Write(CString sName, XmlUtils::CStringWriter*  pCStringWriter, bool bCell)
	{
		if(bValue)
		{
			pCStringWriter->WriteString(CString(_T("<")));
			pCStringWriter->WriteString(sName);
			if(border_Single == Value)
				pCStringWriter->WriteString(CString(_T(" w:val=\"single\"")));
			else
				pCStringWriter->WriteString(CString(_T(" w:val=\"none\"")));
			if(bColor)
			{
                CString sColorAttr;sColorAttr.Format(_T(" w:color=\"%ls\""), Color.ToString());
				pCStringWriter->WriteString(sColorAttr);
			}
			if(bSize)
			{
				long nSize = SerializeCommon::Round(g_dKoef_mm_to_eightpoint * Size);
				CString sSize;sSize.Format(_T(" w:sz=\"%d\""), nSize);
				pCStringWriter->WriteString(sSize);
			}
			if(bSpace)
			{
				long nSpace = SerializeCommon::Round(g_dKoef_mm_to_pt * Space);
				CString sSpace;sSpace.Format(_T(" w:space=\"%d\""), nSpace);
				pCStringWriter->WriteString(sSpace);
			}
			if(bThemeColor && ThemeColor.IsNoEmpty())
			{
				if(ThemeColor.Auto && !bColor)
				{
					CString sAuto(_T(" w:color=\"auto\""));
					pCStringWriter->WriteString(sAuto);
				}
				if(ThemeColor.bColor)
				{
                    CString sThemeColor;sThemeColor.Format(_T(" w:themeColor=\"%ls\""), ThemeColor.ToStringColor());
					pCStringWriter->WriteString(sThemeColor);
				}
				if(ThemeColor.bTint)
				{
                    CString sThemeTint;sThemeTint.Format(_T(" w:themeTint=\"%ls\""), ThemeColor.ToStringTint());
					pCStringWriter->WriteString(sThemeTint);
				}
				if(ThemeColor.bShade)
				{
                    CString sThemeShade;sThemeShade.Format(_T(" w:themeShade=\"%ls\""), ThemeColor.ToStringShade());
					pCStringWriter->WriteString(sThemeShade);
				}
			}
			pCStringWriter->WriteString(CString(_T("/>")));
		}
		else
		{
			pCStringWriter->WriteString(CString(_T("<")));
			pCStringWriter->WriteString(sName);
			if(false != bCell)
				pCStringWriter->WriteString(CString(_T(" w:val=\"nil\"")));
			else
				pCStringWriter->WriteString(CString(_T(" w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"")));
			pCStringWriter->WriteString(CString(_T("/>")));
		}
	}
};
class docBorders
{
public:
	docBorder oLeft;
	docBorder oTop;
	docBorder oRight;
	docBorder oBottom;
	docBorder oInsideV;
	docBorder oInsideH;
	docBorder oBetween;

	bool bLeft;
	bool bTop;
	bool bRight;
	bool bBottom;
	bool bInsideV;
	bool bInsideH;
	bool bBetween;
public:
	docBorders()
	{
		bLeft = false;
		bTop = false;
		bRight = false;
		bBottom = false;
		bInsideV = false;
		bInsideH = false;
		bBetween = false;
	}
	bool IsEmpty()
	{
		return !(bLeft || bTop || bRight || bBottom || bInsideV || bInsideH || bBetween);
	}
	void Write(XmlUtils::CStringWriter*  pCStringWriter, bool bCell)
	{
		if(bLeft)
			oLeft.Write(_T("w:left"), pCStringWriter, bCell);
		if(bTop)
			oTop.Write(_T("w:top"), pCStringWriter, bCell);
		if(bRight)
			oRight.Write(_T("w:right"), pCStringWriter, bCell);
		if(bBottom)
			oBottom.Write(_T("w:bottom"), pCStringWriter, bCell);
		if(bInsideV)
			oInsideV.Write(_T("w:insideV"), pCStringWriter, bCell);
		if(bInsideH)
			oInsideH.Write(_T("w:insideH"), pCStringWriter, bCell);
		if(bBetween)
			oBetween.Write(_T("w:between"), pCStringWriter, bCell);
	}
};
class docLvlText
{
public:
	CString Text;
	BYTE Number;

	bool bText;
	bool bNumber;
	docLvlText()
	{
		bText = false;
		bNumber = false;
	}
};
class docLvl
{
public:
	long Format;
	BYTE Jc;
	std::vector<docLvlText*> Text;
	long Restart;
	long Start;
	BYTE Suff;
	XmlUtils::CStringWriter ParaPr;
	XmlUtils::CStringWriter TextPr;
	CString PStyle;

	bool bFormat;
	bool bJc;
	bool bText;
	bool bRestart;
	bool bStart;
	bool bSuff;
	bool bParaPr;
	bool bTextPr;
	bool bPStyle;
	docLvl()
	{
		bFormat = false;
		bJc = false;
		bText = false;
		bRestart = false;
		bStart = false;
		bSuff = false;
		bParaPr = false;
		bTextPr = false;
		bPStyle = false;
	}
	~docLvl()
	{
		for(int i = 0,length = Text.size(); i < length; i++)
		{
			delete Text[i];
		}
	}
	void Write(XmlUtils::CStringWriter& oWriter, int index)
	{
		CString sILvl;sILvl.Format(_T("<w:lvl w:ilvl=\"%d\">"), index);
		oWriter.WriteString(sILvl);
		if(bStart)
		{
			CString sStart;sStart.Format(_T("<w:start w:val=\"%d\"/>"), Start);
			oWriter.WriteString(sStart);
		}
		if(bFormat)
		{
			CString sFormat;
			switch(Format)
			{
			case numbering_numfmt_None:sFormat = _T("none");break;
			case numbering_numfmt_Bullet:sFormat = _T("bullet");break;
			case numbering_numfmt_Decimal:sFormat = _T("decimal");break;
			case numbering_numfmt_LowerRoman:sFormat = _T("lowerRoman");break;
			case numbering_numfmt_UpperRoman:sFormat = _T("upperRoman");break;
			case numbering_numfmt_LowerLetter:sFormat = _T("lowerLetter");break;
			case numbering_numfmt_UpperLetter:sFormat = _T("upperLetter");break;
			case numbering_numfmt_DecimalZero:sFormat = _T("decimalZero");break;
			}
			if(!sFormat.IsEmpty())
			{
                CString sFormatXml;sFormatXml.Format(_T("<w:numFmt w:val=\"%ls\"/>"), sFormat);
				oWriter.WriteString(sFormatXml);
			}
		}
		if(bPStyle)
		{
			CString sStyleName = XmlUtils::EncodeXmlString(PStyle);
            CString sFormatXml;sFormatXml.Format(_T("<w:pStyle w:val=\"%ls\"/>"), sStyleName);
			oWriter.WriteString(sFormatXml);
		}
		
		if(bRestart && 0 == Restart)
		{
			CString sRestart;sRestart.Format(_T("<w:lvlRestart w:val=\"%d\"/>"), Restart);
			oWriter.WriteString(sRestart);
		}
		if(bText)
		{
			CString sText;
			for(int i = 0, length = Text.size(); i < length; ++i)
			{
				docLvlText* item = Text[i];
				if(item->bText)
				{
					sText.Append(item->Text);
				}
				else if(item->bNumber)
				{
					sText.AppendFormat(_T("%%%d"), (item->Number+1));
				}
			}
			sText = XmlUtils::EncodeXmlString(sText);
            
            //CString sTextXml;sTextXml.Format(_T("<w:lvlText w:val=\"%ls\"/>"), sText);
          
            CString sTextXml(_T("<w:lvlText w:val=\""));
            sTextXml += sText;
            sTextXml += _T("\"/>");

			oWriter.WriteString(sTextXml);
		}
		if(bJc)
		{
			CString sJc;
			switch(Jc)
			{
			case align_Right:sJc = _T("right");break;
			case align_Left:sJc = _T("left");break;
			case align_Center:sJc = _T("center");break;
			case align_Justify:sJc = _T("distribute");break;
			}
			if(!sJc.IsEmpty())
			{
                CString sJcXml;sJcXml.Format(_T("<w:lvlJc w:val=\"%ls\"/>"), sJc);
				oWriter.WriteString(sJcXml);
			}
		}
		if(bParaPr)
		{
			oWriter.Write(ParaPr);
		}
		if(bTextPr)
		{
			oWriter.Write(TextPr);
		}
		oWriter.WriteString(CString(_T("</w:lvl>")));
	}
};
class docANum
{
public:
	long Id;
	CString NumStyleLink;
	CString StyleLink;
	std::vector<docLvl*> Lvls;

	bool bId;
	docANum()
	{
		bId = false;
	}
	~docANum()
	{
		for(int i = 0, length = Lvls.size(); i < length; i++)
		{
			delete Lvls[i];
		}
	}
	void Write(XmlUtils::CStringWriter& oWriterANum)
	{
		if(bId)
		{
			CString sANumId;sANumId.Format(_T("<w:abstractNum w:abstractNumId=\"%d\"><w:multiLevelType w:val=\"hybridMultilevel\"/>"), Id);
			oWriterANum.WriteString(sANumId);
			if(!StyleLink.IsEmpty())
			{
				CString sCorrectStyleLink = XmlUtils::EncodeXmlString(StyleLink);
                CString sXml;sXml.Format(_T("<w:styleLink w:val=\"%ls\"/>"), sCorrectStyleLink);
				oWriterANum.WriteString(sXml);
			}
			if(!NumStyleLink.IsEmpty())
			{
				CString sCorrectNumStyleLink = XmlUtils::EncodeXmlString(NumStyleLink);
                CString sXml;sXml.Format(_T("<w:numStyleLink w:val=\"%ls\"/>"), sCorrectNumStyleLink);
				oWriterANum.WriteString(sXml);
			}
			for(int i = 0, length = Lvls.size(); i < length; ++i)
			{
				Lvls[i]->Write(oWriterANum, i);
			}
			oWriterANum.WriteString(CString(_T("</w:abstractNum>")));
		}
	}
};
class docNum
{
public:
	long AId;
	long Id;

	bool bAId;
	bool bId;
	docNum()
	{
		bAId = false;
		bId = false;
	}
	void Write(XmlUtils::CStringWriter& oWriterNumList)
	{
		if(bAId && bId)
		{
			CString sNumList;sNumList.Format(_T("<w:num w:numId=\"%d\"><w:abstractNumId w:val=\"%d\"/></w:num>"), Id, AId);
			oWriterNumList.WriteString(sNumList);
		}
	}
};
class rowPrAfterBefore
{
public:
	CString sName;
	long nGridAfter;
	docW oAfterWidth;
	bool bGridAfter;
	rowPrAfterBefore(CString name)
	{
		sName = name;
		bGridAfter = false;
	}
	void Write(XmlUtils::CStringWriter& writer)
	{
		if(bGridAfter && nGridAfter > 0)
		{
			CString sGridAfter;sGridAfter.Format(_T("<w:grid%ls w:val=\"%d\" />"), sName, nGridAfter);
			writer.WriteString(sGridAfter);
			if(oAfterWidth.bW)
				oAfterWidth.Write(writer, _T("w:w") + sName);
		}
	}
};
class WriteHyperlink
{
public:
	CString rId;
	CString href;
	CString anchor;
	CString tooltip;
	XmlUtils::CStringWriter writer;
	static WriteHyperlink* Parse(CString fld)
	{
		WriteHyperlink* res = NULL;
		if(-1 != fld.Find(_T("HYPERLINK")))
        {
			CString sLink;
			CString sTooltip;
            bool bNextLink = false;
            bool bNextTooltip = false;
            //разбиваем по пробелам, но с учетом кавычек
			std::vector<CString> aItems;
            CString sCurItem;
            bool bDQuot = false;
			for(int i = 0, length = fld.GetLength(); i < length; ++i)
            {
                TCHAR sCurLetter = fld[i];
                if('\"' == sCurLetter)
                    bDQuot = !bDQuot;
                else if('\\' == sCurLetter && true == bDQuot && i + 1 < length && '\"' == fld[i + 1])
                {
					i++;
					sCurItem += fld[i];
                }
                else if(' ' == sCurLetter && false == bDQuot)
                {
                    if(sCurItem.GetLength() > 0)
                    {
						aItems.push_back(sCurItem);
                        sCurItem = _T("");
                    }
                }
                else
                    sCurItem += sCurLetter;
            }
            if(sCurItem.GetLength() > 0)
                aItems.push_back(sCurItem);
			for(int i = 0, length = aItems.size(); i < length; ++i)
            {
                CString item = aItems[i];
				if(bNextLink)
				{
					bNextLink = false;
					sLink = item;
				}
				if(bNextTooltip)
				{
					bNextTooltip = false;
					sTooltip = item;
				}

				if(_T("HYPERLINK") == item)
					bNextLink = true;
				else if(_T("\\o") == item)
					bNextTooltip = true;
            }
			if(false == sLink.IsEmpty())
			{
				res = new WriteHyperlink();
				sLink.Trim();
				int nAnchorIndex = sLink.Find(_T("#"));
				if(-1 != nAnchorIndex)
				{
					res->href = sLink.Left(nAnchorIndex);
					res->anchor = sLink.Right(sLink.GetLength() - nAnchorIndex - 1);
				}
				else
					res->href = sLink;
				if(false == sTooltip.IsEmpty())
					res->tooltip = sTooltip.Trim();
			}
        }
		return res;
	}
	void Write(XmlUtils::CStringWriter& wr)
	{
		if(false == rId.IsEmpty())
		{
			CString sCorrect_rId = XmlUtils::EncodeXmlString(rId);
			CString sCorrect_tooltip = XmlUtils::EncodeXmlString(tooltip);
			CString sCorrect_anchor = XmlUtils::EncodeXmlString(anchor);
			CString sStart;
            sStart.Format(_T("<w:hyperlink r:id=\"%ls\""), (const TCHAR *) sCorrect_rId);
			if(false == tooltip.IsEmpty())
			{
				sStart.Append(_T(" w:tooltip=\""));
				sStart.Append(sCorrect_tooltip);
				sStart.Append(_T("\""));
			}
			if(false == anchor.IsEmpty())
			{
				sStart.Append(_T(" w:anchor=\""));
				sStart.Append(sCorrect_anchor);
				sStart.Append(_T("\""));
			}
			sStart.Append(_T(" w:history=\"1\">"));
			wr.WriteString(sStart);
			wr.Write(writer);
			wr.WriteString(CString(_T("</w:hyperlink>")));
		}
	}
};
class IdCounter
{
private:
	int m_nId;
public:
	IdCounter(int nStart = 0)
	{
		m_nId = nStart;
	}
	int getNextId(int nCount = 1)
	{
		int nRes = m_nId;
		m_nId += nCount;
		return nRes;
	}
};
class CComment{
private:
	typedef CString (*funcArg)(CComment* pComment);
	IdCounter& m_oParaIdCounter;
	IdCounter& m_oFormatIdCounter;
public:
	int IdOpen;
	int IdFormat;
	CString UserName;
	CString UserId;
	CString Date;
	bool Solved;
	CString Text;
	CString m_sParaId;
	CString m_sParaIdParent;
	std::vector<CComment*> replies;

	bool bIdOpen;
	bool bIdFormat;
	bool bSolved;
public:
	CComment(IdCounter& oParaIdCounter, IdCounter& oFormatIdCounter):m_oParaIdCounter(oParaIdCounter),m_oFormatIdCounter(oFormatIdCounter)
	{
		bIdOpen = false;
		bIdFormat = false;
		bSolved = false;
	}
	~CComment()
	{
		for(int i = 0, length = replies.size(); i < length; ++i)
		{
			delete replies[i];
		}
		replies.clear();
	}
	int getCount()
	{
		return replies.size() + 1;
	}
	void setFormatStart(int IdFormatStart)
	{
		bIdFormat = true;
		IdFormat = IdFormatStart;
		for(int i = 0, length = replies.size(); i < length; ++i)
		{
			CComment* pComment = replies[i];
			pComment->bIdFormat = true;
			pComment->IdFormat = IdFormatStart + i + 1;
		} 
	}
	CString writeRef(const CString& sBefore, const CString& sRef, const CString& sAfter)
	{
		CString sRes;
		sRes.Append(writeRef(this, sBefore, sRef, sAfter));
		for(int i = 0, length = replies.size(); i < length; ++i)
			sRes.Append(writeRef(replies[i], sBefore, sRef, sAfter));
		return sRes;
	}
	CString writeTemplates(funcArg fReadFunction)
	{
		CString sRes;
		sRes.Append(fReadFunction(this));
		for(int i = 0, length = replies.size(); i < length; ++i)
			sRes.Append(fReadFunction(replies[i]));
		return sRes;
	}
	static CString writeRef(CComment* pComment, const CString& sBefore, const CString& sRef, const CString& sAfter)
	{
		CString sRes;
		if(!pComment->bIdFormat)
		{
			pComment->bIdFormat = true;
			pComment->IdFormat = pComment->m_oFormatIdCounter.getNextId();
		}
		sRes.Append(sBefore);
        sRes.AppendFormat(_T("<%ls w:id=\"%d\"/>"), (const TCHAR *) sRef, pComment->IdFormat);
		sRes.Append(sAfter);
		return sRes;
	}
	static bool writeContentWritePart(CComment* pComment, CString& sText, int nPrevIndex, int nCurIndex, bool bFirst, CString& sRes)
	{
		CString sPart;
		if(nPrevIndex < nCurIndex)
            sPart = XmlUtils::EncodeXmlString(sText.Mid(nPrevIndex, nCurIndex - nPrevIndex));

        int nId = pComment->m_oParaIdCounter.getNextId();

        CString sId;
		sId.Format(_T("%08X"), nId);
		if(bFirst)
		{
			bFirst = false;
			pComment->m_sParaId = sId;
		}
		CString sFormat = _T("<w:p w14:paraId=\"%ls\" w14:textId=\"%ls\"><w:pPr><w:spacing w:line=\"240\" w:after=\"0\" w:lineRule=\"auto\" w:before=\"0\"/><w:ind w:firstLine=\"0\" w:left=\"0\" w:right=\"0\"/><w:jc w:val=\"left\"/></w:pPr><w:r><w:rPr><w:rFonts w:eastAsia=\"Arial\" w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\"/><w:sz w:val=\"22\"/></w:rPr><w:t xml:space=\"preserve\">");
		sRes.AppendFormat((const TCHAR *) sFormat, (const TCHAR *) sId, (const TCHAR *) sId);
		sRes.Append(sPart);
		sRes.Append(_T("</w:t></w:r></w:p>"));
		return bFirst;
	}
	static CString writeContent(CComment* pComment)
	{
		CString sRes;
		if(!pComment->bIdFormat)
		{
			pComment->bIdFormat = true;
			pComment->IdFormat = pComment->m_oFormatIdCounter.getNextId();
		}
		sRes.AppendFormat(_T("<w:comment w:id=\"%d\""), pComment->IdFormat);
		CString sInitials;
		if(false == pComment->UserName.IsEmpty())
		{
			CString sUserName = XmlUtils::EncodeXmlString(pComment->UserName);
			sRes.Append(_T(" w:author=\""));
			sRes.Append(sUserName);
			sRes.Append(_T("\""));
			//делаем initials
			int nTokenPos = 0;
			CString strToken = pComment->UserName.Tokenize(_T(" "), nTokenPos);
			while (!strToken.IsEmpty())
			{
				strToken.Trim();
				if(strToken.GetLength() > 0)
					sInitials.AppendChar(strToken[0]);
				strToken = pComment->UserName.Tokenize(_T(" "), nTokenPos);
			}
		}
		if(false == pComment->Date.IsEmpty())
		{
			CString sDate = XmlUtils::EncodeXmlString(pComment->Date);
			sRes.Append(_T(" w:date=\""));
			sRes.Append(sDate);
			sRes.Append(_T("\""));
		}
		if(false == sInitials.IsEmpty())
		{
			sInitials = XmlUtils::EncodeXmlString(sInitials);
			sRes.Append(_T(" w:initials=\""));
			sRes.Append(sInitials);
			sRes.Append(_T("\""));
		}
		sRes.Append(_T(">"));
		if(false == pComment->Text.IsEmpty())
		{
			CString sText = pComment->Text;
			sText.Remove('\r');
			bool bFirst = true;
			int nPrevIndex = 0;
			for(int i = 0, length = sText.GetLength(); i < length; i++)
			{
				TCHAR cToken = sText[i];
				if('\n' == cToken)
				{
					bFirst = writeContentWritePart(pComment, sText, nPrevIndex, i, bFirst, sRes);
					nPrevIndex = i + 1;
				}
			}
			writeContentWritePart(pComment, sText, nPrevIndex, sText.GetLength(), bFirst, sRes);
		}
		sRes.Append(_T("</w:comment>"));
		return sRes;
	}
	static CString writeContentExt(CComment* pComment)
	{
		CString sRes;
		if(!pComment->m_sParaId.IsEmpty())
		{
			CString sDone(_T("0"));
			if(pComment->bSolved && pComment->Solved)
				sDone = _T("1");
			if(!pComment->m_sParaIdParent.IsEmpty())
                sRes.AppendFormat(_T("<w15:commentEx w15:paraId=\"%ls\" w15:paraIdParent=\"%ls\" w15:done=\"%ls\"/>"), (const TCHAR *) pComment->m_sParaId, (const TCHAR *) pComment->m_sParaIdParent, (const TCHAR *) sDone);
			else
                sRes.AppendFormat(_T("<w15:commentEx w15:paraId=\"%ls\" w15:done=\"%ls\"/>"), (const TCHAR *) pComment->m_sParaId, (const TCHAR *) sDone);
			//расставляем paraIdParent
			for(int i = 0, length = pComment->replies.size(); i < length; i++)
				pComment->replies[i]->m_sParaIdParent = pComment->m_sParaId;
		}
		return sRes;
	}
	static CString writePeople(CComment* pComment)
	{
		CString sRes;
		if(false == pComment->UserName.IsEmpty() && false == pComment->UserId.IsEmpty())
		{
			CString sUserName = XmlUtils::EncodeXmlString(pComment->UserName);
			CString sUserId = XmlUtils::EncodeXmlString(pComment->UserId);
			sRes.Append(_T("<w15:person w15:author=\""));
			sRes.Append(sUserName);
			sRes.Append(_T("\"><w15:presenceInfo w15:providerId=\"Teamlab\" w15:userId=\""));
			sRes.Append(sUserId);
			sRes.Append(_T("\"/></w15:person>"));
		}
		return sRes;
	}
};
class CComments
{
	std::map<int, CComment*> m_mapComments;
	std::map<CString, CComment*> m_mapAuthors;
public:
	IdCounter m_oFormatIdCounter;
	IdCounter m_oParaIdCounter;
public:
	CComments():m_oParaIdCounter(1)
	{
	}
	~CComments()
	{
		for (std::map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			delete it->second;
		}
		m_mapComments.clear();
	}
	void add(CComment* pComment)
	{
		if(pComment->bIdOpen)
		{
			m_mapComments[pComment->IdOpen] = pComment;
			addAuthor(pComment);
			for(int i = 0, length = pComment->replies.size(); i < length; i++)
				addAuthor(pComment->replies[i]);
		}
	}
	void addAuthor(CComment* pComment)
	{
		if(false == pComment->UserName.IsEmpty() && false == pComment->UserId.IsEmpty())
			m_mapAuthors[pComment->UserName] = pComment;
	}
	CComment* get(int nInd)
	{
		CComment* pRes = NULL;
		std::map<int, CComment*>::const_iterator pair = m_mapComments.find(nInd);
		if(m_mapComments.end() != pair)
			pRes = pair->second;
		return pRes;
	}
	int getNextId(int nCount = 1)
	{
		return m_oFormatIdCounter.getNextId(nCount);
	}
	CString writeContent()
	{
		CString sRes;
		for (std::map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sRes.Append(it->second->writeTemplates(CComment::writeContent));
		}
		return sRes;
	}
	CString writeContentExt()
	{
		CString sRes;
		for (std::map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sRes.Append(it->second->writeTemplates(CComment::writeContentExt));
		}
		return sRes;
	}
	CString writePeople()
	{
		CString sRes;
		for (std::map<CString, CComment*>::const_iterator it = m_mapAuthors.begin(); it != m_mapAuthors.end(); ++it)
		{
			sRes.Append(it->second->writePeople(it->second));
		}
		return sRes;
	}
};
//class CComment
//{
//public:
//	int Id;
//	CString Initials;
//	CString UserName;
//	CString Date;
//	CString Text;
//	CAtlArray<CComment*> replies;
//
//	bool bId;
//public: 
//	CComment()
//	{
//		bId = false;
//	}
//	~CComment()
//	{
//		for(int i = 0, length = replies.GetCount(); i < length; ++i)
//		{
//			delete replies[i];
//		}
//		replies.RemoveAll();
//	}
//	CString Write()
//	{
//		CString sRes;
//		if(bId)
//		{
//			sRes.AppendFormat(_T("<w:comment w:id=\"%d\""), Id);
//			if(false == UserName.IsEmpty())
//			{
//				CString sUserName = XmlUtils::EncodeXmlString(UserName);
//				sRes.AppendFormat(_T(" w:author=\"%ls\""), sUserName);
//			}
//			if(false == Date.IsEmpty())
//			{
//				CString sDate = XmlUtils::EncodeXmlString(Date);
//				sRes.AppendFormat(_T(" w:date=\"%ls\""), sDate);
//			}
//			if(false == Initials.IsEmpty())
//			{
//				CString sInitials = XmlUtils::EncodeXmlString(Initials);
//				sRes.AppendFormat(_T(" w:initials=\"%ls\""), sInitials);
//			}
//			sRes.Append(_T(">"));
//
//			sRes.Append(WriteCommentContent(this, true));
//			
//			sRes.Append(_T("</w:comment>"));
//		}
//		return sRes;
//	}
//private:
//	CString WriteCommentContent(CComment* pComment, bool bCheckLast)
//	{
//		CString sRes;
//		CString sFormat = _T("<w:p><w:pPr><w:spacing w:line=\"240\" w:after=\"0\" w:lineRule=\"auto\" w:before=\"0\"/><w:ind w:firstLine=\"0\" w:left=\"0\" w:right=\"0\"/><w:jc w:val=\"left\"/></w:pPr><w:r><w:rPr><w:rFonts w:eastAsia=\"Arial\" w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\"/><w:sz w:val=\"22\"/></w:rPr><w:t xml:space=\"preserve\">%ls</w:t></w:r></w:p>");
//		CString sName = XmlUtils::EncodeXmlString(pComment->UserName + _T(":"));
//		sRes.AppendFormat(sFormat, sName);
//		if(false == pComment->Text.IsEmpty())
//		{
//			CString sText = XmlUtils::EncodeXmlString(pComment->Text);
//			sRes.AppendFormat(sFormat, sText);
//		}
//		if(false == bCheckLast || pComment->replies.GetCount() > 0)
//			sRes.Append(_T("<w:p><w:pPr><w:pBdr><w:top w:space=\"1\" w:sz=\"4\" w:color=\"auto\" w:val=\"single\"/></w:pBdr></w:pPr></w:p>"));
//		
//		if(pComment->replies.GetCount() > 0)
//		{
//			for(int i = 0, length = pComment->replies.GetCount(); i < length; ++i)
//			{
//				if(i != length - 1)
//					sRes += WriteCommentContent(pComment->replies[i], false);
//				else
//					sRes += WriteCommentContent(pComment->replies[i], bCheckLast);
//			}
//		}
//		return sRes;
//	}
//};
class CDrawingPropertyWrapPoint
{
public:
	double X;
	double Y;

	bool bX;
	bool bY;
	CDrawingPropertyWrapPoint()
	{
		bX = false;
		bY = false;
	}
};
class CDrawingPropertyWrap
{
public:
	BYTE WrappingType;
	bool Edited;
	CDrawingPropertyWrapPoint Start;
	std::vector<CDrawingPropertyWrapPoint*> Points;

	bool bWrappingType;
	bool bEdited;
	bool bStart;
	
	CDrawingPropertyWrap()
	{
		bWrappingType = false;
		bEdited = false;
		bStart = false;
	}
	~CDrawingPropertyWrap()
	{
		for(int i = 0, length = Points.size(); i < length; ++i)
			delete Points[i];
		Points.clear();
	}
};
class CDrawingProperty
{
public:
	long DataPos;
	long DataLength;
	BYTE Type;
	bool BehindDoc;
	double DistL;
	double DistT;
	double DistR;
	double DistB;
	bool LayoutInCell;
	unsigned long RelativeHeight;
	bool BSimplePos;
	double EffectExtentL;
	double EffectExtentT;
	double EffectExtentR;
	double EffectExtentB;
	double Width;
	double Height;
	BYTE PositionHRelativeFrom;
	BYTE PositionHAlign;
	double PositionHPosOffset;
	double PositionHPctOffset;
	BYTE PositionVRelativeFrom;
	BYTE PositionVAlign;
	double PositionVPosOffset;
	double PositionVPctOffset;
	double SimplePosX;
	double SimplePosY;
	CDrawingPropertyWrap DrawingPropertyWrap;
	CString sChartRels;
	CString sSizeRelH;
	CString sSizeRelV;
	int m_nDocPr;
	CString sGraphicFramePr;

	bool bDataPos;
	bool bDataLength;
	bool bType;
	bool bBehindDoc;
	bool bDistL;
	bool bDistT;
	bool bDistR;
	bool bDistB;
	bool bLayoutInCell;
	bool bRelativeHeight;
	bool bBSimplePos;
	bool bEffectExtentL;
	bool bEffectExtentT;
	bool bEffectExtentR;
	bool bEffectExtentB;
	bool bWidth;
	bool bHeight;
	bool bPositionHRelativeFrom;
	bool bPositionHAlign;
	bool bPositionHPosOffset;
	bool bPositionHPctOffset;
	bool bPositionVRelativeFrom;
	bool bPositionVAlign;
	bool bPositionVPosOffset;
	bool bPositionVPctOffset;
	bool bSimplePosX;
	bool bSimplePosY;
	bool bDrawingPropertyWrap;
	CDrawingProperty(int nDocPr)
	{
		m_nDocPr = nDocPr;
		bDataPos = false;
		bDataLength = false;
		bType = false;
		bBehindDoc = false;
		bDistL = false;
		bDistT = false;
		bDistR = false;
		bDistB = false;
		bLayoutInCell = false;
		bRelativeHeight = false;
		bBSimplePos = false;
		bEffectExtentL = false;
		bEffectExtentT = false;
		bEffectExtentR = false;
		bEffectExtentB = false;
		bWidth = false;
		bHeight = false;
		bPositionHRelativeFrom = false;
		bPositionHAlign = false;
		bPositionHPosOffset = false;
		bPositionHPctOffset = false;
		bPositionVRelativeFrom = false;
		bPositionVAlign = false;
		bPositionVPosOffset = false;
		bPositionVPctOffset = false;
		bSimplePosX = false;
		bSimplePosY = false;
		bDrawingPropertyWrap = false;
	}
	bool IsChart()
	{
		return false == sChartRels.IsEmpty();
	}
	CString Write()
	{
		CString sXml;
		if(bType)
		{
			bool bChart = IsChart();
			if(c_oAscWrapStyle::Inline == Type)
			{
				if(bWidth && bHeight)
				{
					__int64 emuWidth = (__int64)(g_dKoef_mm_to_emu * Width);
					__int64 emuHeight = (__int64)(g_dKoef_mm_to_emu * Height);
					if(false == bChart)
						sXml.AppendFormat(_T("<wp:inline xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"%lld\" cy=\"%lld\"/>"), emuWidth, emuHeight);
					else
						sXml.AppendFormat(_T("<w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"%lld\" cy=\"%lld\"/>"), emuWidth, emuHeight);
					if(bEffectExtentL && bEffectExtentT && bEffectExtentR && bEffectExtentB)
					{
						__int64 emuEffectExtentL = (__int64)(g_dKoef_mm_to_emu * EffectExtentL);
						__int64 emuEffectExtentT = (__int64)(g_dKoef_mm_to_emu * EffectExtentT);
						__int64 emuEffectExtentR = (__int64)(g_dKoef_mm_to_emu * EffectExtentR);
						__int64 emuEffectExtentB = (__int64)(g_dKoef_mm_to_emu * EffectExtentB);
						sXml.AppendFormat(_T("<wp:effectExtent l=\"%lld\" t=\"%lld\" r=\"%lld\" b=\"%lld\"/>"), emuEffectExtentL, emuEffectExtentT, emuEffectExtentR, emuEffectExtentB);
					}

					if(bChart)
					{
						sXml.AppendFormat(_T("<wp:docPr id=\"%d\" name=\"Chart %d\"/>"), m_nDocPr, m_nDocPr);
					}
					else
					{
						sXml.AppendFormat(_T("<wp:docPr id=\"%d\" name=\"\"/>"), m_nDocPr);
					}
					if(!sGraphicFramePr.IsEmpty())
					{
						sXml.Append(sGraphicFramePr);
					}
					else
					{
						sXml.Append(_T("<wp:cNvGraphicFramePr/>"));
					}
					if(bChart)
					{
						sXml.AppendFormat(_T("<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/chart\"><c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"%ls\"/></a:graphicData></a:graphic></wp:inline></w:drawing>"), (const TCHAR *) sChartRels);
					}
					else
					{
						sXml.Append(_T("</wp:inline>"));
					}
				}
			}
			else
			{
				if(bWidth && bHeight	&& ((bPositionHRelativeFrom && (bPositionHAlign || bPositionHPosOffset || bPositionHPctOffset) 
										&& bPositionVRelativeFrom && (bPositionVAlign || bPositionVPosOffset || bPositionVPctOffset)) 
						|| (bBSimplePos && bSimplePosX && bSimplePosY)))
				{
					__int64 emuDistL = 0;
					__int64 emuDistT = 0;
					__int64 emuDistR = 0;
					__int64 emuDistB = 0;

					if(bDistL)
						emuDistL = (__int64)(g_dKoef_mm_to_emu * DistL);
					if(bDistT)
						emuDistT = (__int64)(g_dKoef_mm_to_emu * DistT);
					if(bDistR)
						emuDistR = (__int64)(g_dKoef_mm_to_emu * DistR);
					if(bDistB)
						emuDistB = (__int64)(g_dKoef_mm_to_emu * DistB);
					int nSimplePos = 0;
					if(bBSimplePos && BSimplePos)
						nSimplePos = 1;
					int nRelativeHeight = 0;
					if(bRelativeHeight)
						nRelativeHeight = RelativeHeight;
					int nBehindDoc = 0;
					if(bBehindDoc && BehindDoc)
						nBehindDoc = 1;
					int nLayoutInCell = 1;
					if(bLayoutInCell && false == LayoutInCell)
						nLayoutInCell = 0;

					if(bChart)
						sXml.Append(_T("<w:drawing>"));

					sXml.AppendFormat(_T("<wp:anchor xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" distT=\"%lld\" distB=\"%lld\" distL=\"%lld\" distR=\"%lld\" simplePos=\"%d\" relativeHeight=\"%u\" behindDoc=\"%d\" locked=\"0\" layoutInCell=\"%d\" allowOverlap=\"1\">"), emuDistT, emuDistB, emuDistL, emuDistR, nSimplePos, nRelativeHeight, nBehindDoc, nLayoutInCell);
					__int64 emuX = 0;
					if(bSimplePosX)
						emuX = (__int64)(g_dKoef_mm_to_emu * SimplePosX);
					__int64 emuY = 0;
					if(bSimplePosY)
						emuY = (__int64)(g_dKoef_mm_to_emu * SimplePosY);
					sXml.AppendFormat(_T("<wp:simplePos x=\"%lld\" y=\"%lld\"/>"), emuX, emuY);

					if(bPositionHRelativeFrom && (bPositionHAlign || bPositionHPosOffset || bPositionHPctOffset))
					{
						CString sRelativeFrom;
						switch(PositionHRelativeFrom)
						{
						case 0: sRelativeFrom = _T("character");break;
						case 1: sRelativeFrom = _T("column");break;
						case 2: sRelativeFrom = _T("insideMargin");break;
						case 3: sRelativeFrom = _T("leftMargin");break;
						case 4: sRelativeFrom = _T("margin");break;
						case 5: sRelativeFrom = _T("outsideMargin");break;
						case 6: sRelativeFrom = _T("page");break;
						case 7: sRelativeFrom = _T("rightMargin");break;
						}
						CString sContent;
						if(bPositionHAlign)
						{
							switch(PositionHAlign)
							{
							case 0: sContent = _T("<wp:align>center</wp:align>");break;
							case 1: sContent = _T("<wp:align>inside</wp:align>");break;
							case 2: sContent = _T("<wp:align>left</wp:align>");break;
							case 3: sContent = _T("<wp:align>outside</wp:align>");break;
							case 4: sContent = _T("<wp:align>right</wp:align>");break;
							}
						}
						else if(bPositionHPosOffset)
						{
							__int64 emuPosOffset = (__int64)(g_dKoef_mm_to_emu * PositionHPosOffset);
							sContent.Format(_T("<wp:posOffset>%lld</wp:posOffset>"), emuPosOffset);
						}
						else if(bPositionHPctOffset)
						{
							long pctOffset = (long)(1000 * PositionHPctOffset);
							sContent.Format(_T("<wp14:pctPosHOffset>%d</wp14:pctPosHOffset>"), pctOffset);
						}
						sXml.AppendFormat(_T("<wp:positionH relativeFrom=\"%ls\">%ls</wp:positionH>"), (const TCHAR *) sRelativeFrom, (const TCHAR *) sContent);
					}
					if(bPositionVRelativeFrom && (bPositionVAlign || bPositionVPosOffset || bPositionVPctOffset))
					{
						CString sRelativeFrom;
						switch(PositionVRelativeFrom)
						{
						case 0: sRelativeFrom = _T("bottomMargin");break;
						case 1: sRelativeFrom = _T("insideMargin");break;
						case 2: sRelativeFrom = _T("line");break;
						case 3: sRelativeFrom = _T("margin");break;
						case 4: sRelativeFrom = _T("outsideMargin");break;
						case 5: sRelativeFrom = _T("page");break;
						case 6: sRelativeFrom = _T("paragraph");break;
						case 7: sRelativeFrom = _T("topMargin");break;
						}
						CString sContent;
						if(bPositionVAlign)
						{
							switch(PositionVAlign)
							{
							case 0: sContent = _T("<wp:align>bottom</wp:align>");break;
							case 1: sContent = _T("<wp:align>center</wp:align>");break;
							case 2: sContent = _T("<wp:align>inside</wp:align>");break;
							case 3: sContent = _T("<wp:align>outside</wp:align>");break;
							case 4: sContent = _T("<wp:align>top</wp:align>");break;
							}
						}
						else if(bPositionVPosOffset)
						{
							__int64 emuPosOffset = (__int64)(g_dKoef_mm_to_emu * PositionVPosOffset);
							sContent.Format(_T("<wp:posOffset>%lld</wp:posOffset>"), emuPosOffset);
						}
						else if(bPositionVPctOffset)
						{
							long pctOffset = (long)(1000 * PositionVPctOffset);
							sContent.Format(_T("<wp14:pctPosVOffset>%d</wp14:pctPosVOffset>"), pctOffset);
						}
						sXml.AppendFormat(_T("<wp:positionV relativeFrom=\"%ls\">%ls</wp:positionV>"), (const TCHAR *) sRelativeFrom, (const TCHAR *) sContent);
					}
					__int64 emuWidth = (__int64)(g_dKoef_mm_to_emu * Width);
					__int64 emuHeight = (__int64)(g_dKoef_mm_to_emu * Height);
					sXml.AppendFormat(_T("<wp:extent cx=\"%lld\" cy=\"%lld\"/>"), emuWidth, emuHeight);
					if(bEffectExtentL && bEffectExtentT && bEffectExtentR && bEffectExtentB)
					{
						__int64 emuEffectExtentL = (__int64)(g_dKoef_mm_to_emu * EffectExtentL);
						__int64 emuEffectExtentT = (__int64)(g_dKoef_mm_to_emu * EffectExtentT);
						__int64 emuEffectExtentR = (__int64)(g_dKoef_mm_to_emu * EffectExtentR);
						__int64 emuEffectExtentB = (__int64)(g_dKoef_mm_to_emu * EffectExtentB);
						sXml.AppendFormat(_T("<wp:effectExtent l=\"%lld\" t=\"%lld\" r=\"%lld\" b=\"%lld\"/>"), emuEffectExtentL, emuEffectExtentT, emuEffectExtentR, emuEffectExtentB);
					}
					if(bDrawingPropertyWrap && DrawingPropertyWrap.bWrappingType)
					{
						CString sTagName;
						switch(DrawingPropertyWrap.WrappingType)
						{
						case c_oSerImageType2::WrapNone:sTagName = _T("wrapNone");break;
						case c_oSerImageType2::WrapSquare:sTagName = _T("wrapSquare");break;
						case c_oSerImageType2::WrapThrough:sTagName = _T("wrapThrough");break;
						case c_oSerImageType2::WrapTight:sTagName = _T("wrapTight");break;
						case c_oSerImageType2::WrapTopAndBottom:sTagName = _T("wrapTopAndBottom");break;
						}
						if(DrawingPropertyWrap.bStart || DrawingPropertyWrap.Points.size() > 0)
						{
							if( c_oSerImageType2::WrapSquare	== DrawingPropertyWrap.WrappingType		|| 
								c_oSerImageType2::WrapThrough	== DrawingPropertyWrap.WrappingType		|| 
								c_oSerImageType2::WrapTight		== DrawingPropertyWrap.WrappingType)
							{
                                sXml.AppendFormat(_T("<wp:%ls wrapText=\"bothSides\">"), (const TCHAR *) sTagName);
							}
							else
                                sXml.AppendFormat(_T("<wp:%ls>"), (const TCHAR *) sTagName);

							int nEdited = 0;
							if(DrawingPropertyWrap.bEdited && DrawingPropertyWrap.Edited)
								nEdited = 1;
							sXml.AppendFormat(_T("<wp:wrapPolygon edited=\"%d\">"), nEdited);
							if(DrawingPropertyWrap.bStart && DrawingPropertyWrap.Start.bX && DrawingPropertyWrap.Start.bY)
							{
								__int64 emuX = (__int64)(g_dKoef_mm_to_emu * DrawingPropertyWrap.Start.X);
								__int64 emuY = (__int64)(g_dKoef_mm_to_emu * DrawingPropertyWrap.Start.Y);
								sXml.AppendFormat(_T("<wp:start x=\"%lld\" y=\"%lld\"/>"), emuX, emuY);
							}
							for(int i = 0, length = DrawingPropertyWrap.Points.size(); i < length; ++i)
							{
								CDrawingPropertyWrapPoint* pWrapPoint = DrawingPropertyWrap.Points[i];
								if(pWrapPoint->bX && pWrapPoint->bY)
								{
									__int64 emuX = (__int64)(g_dKoef_mm_to_emu * pWrapPoint->X);
									__int64 emuY = (__int64)(g_dKoef_mm_to_emu * pWrapPoint->Y);
									sXml.AppendFormat(_T("<wp:lineTo x=\"%lld\" y=\"%lld\"/>"), emuX, emuY);
								}
							}
							sXml.Append(_T("</wp:wrapPolygon>"));
                            sXml.AppendFormat(_T("</wp:%ls>"), (const TCHAR *) sTagName);
						}
						else
						{
							//для wrapThrough и wrapTight wrapPolygon обязательное поле, если его нет - меняем тип.
							if( c_oSerImageType2::WrapSquare	== DrawingPropertyWrap.WrappingType		|| 
								c_oSerImageType2::WrapThrough	== DrawingPropertyWrap.WrappingType		|| 
								c_oSerImageType2::WrapTight		== DrawingPropertyWrap.WrappingType)
							{
									sXml.Append(_T("<wp:wrapSquare wrapText=\"bothSides\"/>"));
							}
							else
                                sXml.AppendFormat(_T("<wp:%ls/>"), (const TCHAR *) sTagName);
						}
					}
					else
						sXml.Append(_T("<wp:wrapNone/>"));

					if(bChart)
					{
						sXml.AppendFormat(_T("<wp:docPr id=\"%d\" name=\"Chart %d\"/>"), m_nDocPr, m_nDocPr);
					}
					else
					{
						sXml.AppendFormat(_T("<wp:docPr id=\"%d\" name=\"\"/>"), m_nDocPr);
					}
					if(!sGraphicFramePr.IsEmpty())
					{
						sXml.Append(sGraphicFramePr);
					}
					else
					{
						sXml.Append(_T("<wp:cNvGraphicFramePr/>"));
					}
					if(bChart)
					{
						sXml.AppendFormat(_T("<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/chart\"><c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"%ls\"/></a:graphicData></a:graphic>"), (const TCHAR *) sChartRels);
					}

					if(!sSizeRelH.IsEmpty())
					{
						sXml.Append(sSizeRelH);
					}
					if(!sSizeRelV.IsEmpty())
					{
						sXml.Append(sSizeRelV);
					}

					sXml.Append(_T("</wp:anchor>"));

					if(bChart)
						sXml.Append(_T("</w:drawing>"));
				}
			}
		}
		return sXml;
	}
};
class CWiterTblPr
{
public:
	CString Jc;
	CString TableInd;
	CString TableW;
	CString TableCellMar;
	CString TableBorders;
	CString Shd;
	CString tblpPr;
	CString Style;
	CString RowBandSize;
	CString ColBandSize;
	CString Look;
	CString Layout;
	CString tblPrChange;
	CString TableCellSpacing;
	bool IsEmpty()
	{
		return Jc.IsEmpty() && TableInd.IsEmpty() && TableW.IsEmpty() && TableCellMar.IsEmpty() && TableBorders.IsEmpty() && Shd.IsEmpty() && tblpPr.IsEmpty()&& Style.IsEmpty() && Look.IsEmpty() && tblPrChange.IsEmpty() && TableCellSpacing.IsEmpty() && RowBandSize.IsEmpty() && ColBandSize.IsEmpty();
	}
	CString Write(bool bBandSize, bool bLayout)
	{
		CString sRes;
		sRes.Append(_T("<w:tblPr>"));
		if(false == Style.IsEmpty())
			sRes.Append(Style);
		if(false == tblpPr.IsEmpty())
			sRes.Append(tblpPr);
		if(!RowBandSize.IsEmpty())
			sRes.Append(RowBandSize);
		if(!ColBandSize.IsEmpty())
			sRes.Append(ColBandSize);
		if(false == TableW.IsEmpty())
			sRes.Append(TableW);
		if(false == Jc.IsEmpty())
			sRes.Append(Jc);
		if(false == TableCellSpacing.IsEmpty())
			sRes.Append(TableCellSpacing);
		if(false == TableInd.IsEmpty())
			sRes.Append(TableInd);
		if(false == TableBorders.IsEmpty())
			sRes.Append(TableBorders);
		if(false == Shd.IsEmpty())
			sRes.Append(Shd);
		if(bLayout)
		{
			if(false == Layout.IsEmpty())
				sRes.Append(Layout);
			else if(g_nCurFormatVersion < 4)
				sRes.Append(_T("<w:tblLayout w:type=\"fixed\"/>"));
		}
		if(false == TableCellMar.IsEmpty())
			sRes.Append(TableCellMar);
		if(false == Look.IsEmpty())
			sRes.Append(Look);
		if(!tblPrChange.IsEmpty())
			sRes.Append(tblPrChange);
		sRes.Append(_T("</w:tblPr>"));
		return sRes;
	}
};
class CFramePr
{
public:
	bool bDropCap;
	bool bH;
	bool bHAnchor;
	bool bHRule;
	bool bHSpace;
	bool bLines;
	bool bVAnchor;
	bool bVSpace;
	bool bW;
	bool bWrap;
	bool bX;
	bool bXAlign;
	bool bY;
	bool bYAlign;

	BYTE DropCap;
	long H;
	BYTE HAnchor;
	BYTE HRule;
	long HSpace;
	long Lines;
	BYTE VAnchor;
	long VSpace;
	long W;
	BYTE Wrap;
	long X;
	BYTE XAlign;
	long Y;
	BYTE YAlign;
public: CFramePr()
		{
			bDropCap = false;
			bH = false;
			bHAnchor = false;
			bHRule = false;
			bHSpace = false;
			bLines = false;
			bVAnchor = false;
			bVSpace = false;
			bW = false;
			bWrap = false;
			bX = false;
			bXAlign = false;
			bY = false;
			bYAlign = false;
		}
	bool IsEmpty()
	{
		return !(bDropCap || bH || bHAnchor || bHRule || bHSpace || bLines || bVAnchor || bVSpace || bW || bWrap || bX || bXAlign || bY || bYAlign);
	}
	void Write(XmlUtils::CStringWriter& oStringWriter)
	{
		oStringWriter.WriteString(CString(_T("<w:framePr")));
		if(bDropCap)
		{
			CString sDropCap(_T("none"));
			switch(DropCap)
			{
			case 0: sDropCap = _T("none");break;
			case 1: sDropCap = _T("drop");break;
			case 2: sDropCap = _T("margin");break;
			}
            CString sXml;sXml.Format(_T(" w:dropCap=\"%ls\""), sDropCap);
			oStringWriter.WriteString(sXml);
		}
		if(bLines)
		{
			CString sXml;sXml.Format(_T(" w:lines=\"%d\""), Lines);
			oStringWriter.WriteString(sXml);
		}
		if(bW)
		{
			CString sXml;sXml.Format(_T(" w:w=\"%d\""), W);
			oStringWriter.WriteString(sXml);
		}
		if(bH)
		{
			CString sXml;sXml.Format(_T(" w:h=\"%d\""), H);
			oStringWriter.WriteString(sXml);
		}
		if(bVSpace)
		{
			CString sXml;sXml.Format(_T(" w:vSpace=\"%d\""), VSpace);
			oStringWriter.WriteString(sXml);
		}
		if(bHSpace)
		{
			CString sXml;sXml.Format(_T(" w:hSpace=\"%d\""), HSpace);
			oStringWriter.WriteString(sXml);
		}
		if(bWrap)
		{
			CString sWrap(_T("none"));
			switch(Wrap)
			{
			case 0: sWrap = _T("around");break;
			case 1: sWrap = _T("auto");break;
			case 2: sWrap = _T("none");break;
			case 3: sWrap = _T("notBeside");break;
			case 4: sWrap = _T("through");break;
			case 5: sWrap = _T("tight");break;
			}
            CString sXml;sXml.Format(_T(" w:wrap=\"%ls\""), sWrap);
			oStringWriter.WriteString(sXml);
		}
		if(bVAnchor)
		{
			CString sVAnchor(_T("margin"));
			switch(VAnchor)
			{
			case 0: sVAnchor = _T("margin");break;
			case 1: sVAnchor = _T("page");break;
			case 2: sVAnchor = _T("text");break;
			}
            CString sXml;sXml.Format(_T(" w:vAnchor=\"%ls\""), sVAnchor);
			oStringWriter.WriteString(sXml);
		}
		if(bHAnchor)
		{
			CString sHAnchor(_T("margin"));
			switch(HAnchor)
			{
			case 0: sHAnchor = _T("margin");break;
			case 1: sHAnchor = _T("page");break;
			case 2: sHAnchor = _T("text");break;
			}
            CString sXml;sXml.Format(_T(" w:hAnchor=\"%ls\""), sHAnchor);
			oStringWriter.WriteString(sXml);
		}
		if(bX)
		{
			CString sXml;sXml.Format(_T(" w:x=\"%d\""), X);
			oStringWriter.WriteString(sXml);
		}
		if(bXAlign)
		{
			CString sXAlign(_T("left"));
			switch(XAlign)
			{
			case 0: sXAlign = _T("center");break;
			case 1: sXAlign = _T("inside");break;
			case 2: sXAlign = _T("left");break;
			case 3: sXAlign = _T("outside");break;
			case 4: sXAlign = _T("right");break;
			}
            CString sXml;sXml.Format(_T(" w:xAlign=\"%ls\""), sXAlign);
			oStringWriter.WriteString(sXml);
		}
		if(bY)
		{
			CString sXml;sXml.Format(_T(" w:y=\"%d\""), Y);
			oStringWriter.WriteString(sXml);
		}
		if(bYAlign)
		{
			CString sYAlign(_T("inline"));
			switch(YAlign)
			{
			case 0: sYAlign = _T("bottom");break;
			case 1: sYAlign = _T("center");break;
			case 2: sYAlign = _T("inline");break;
			case 3: sYAlign = _T("inside");break;
			case 4: sYAlign = _T("outside");break;
			case 5: sYAlign = _T("top");break;
			}
            CString sXml;sXml.Format(_T(" w:yAlign=\"%ls\""), sYAlign);
			oStringWriter.WriteString(sXml);
		}
		if(bHRule)
		{
			CString sHRule(_T(""));
			switch(HRule)
			{
			case 0: sHRule = _T("atLeast");break;
			case 1: sHRule = _T("auto");break;
			case 2: sHRule = _T("exact");break;
			}
            CString sXml;sXml.Format(_T(" w:hRule=\"%ls\""), sHRule);
			oStringWriter.WriteString(sXml);
		}
		oStringWriter.WriteString(CString(_T("/>")));
	}
};
class CHyperlink{
public:
	CString rId;
	CString sLink;
	CString sAnchor;
	CString sTooltip;
	bool History;
	CString sDocLocation;
	CString sTgtFrame;
	XmlUtils::CStringWriter writer;

	bool bHistory;
public:
	CHyperlink()
	{
		bHistory = false;
	}
	void Write(XmlUtils::CStringWriter& wr)
	{
		if(false == rId.IsEmpty())
		{
			CString sCorrect_rId = XmlUtils::EncodeXmlString(rId);
			CString sCorrect_tooltip = XmlUtils::EncodeXmlString(sTooltip);
			CString sCorrect_anchor = XmlUtils::EncodeXmlString(sAnchor);
			CString sStart;
            sStart.Format(_T("<w:hyperlink r:id=\"%ls\""), (const TCHAR *) sCorrect_rId);
			if(false == sTooltip.IsEmpty())
			{
				sStart.Append(_T(" w:tooltip=\""));
				sStart.Append(sCorrect_tooltip);
				sStart.Append(_T("\""));
			}
			if(false == sAnchor.IsEmpty())
			{
				sStart.Append(_T(" w:anchor=\""));
				sStart.Append(sCorrect_anchor);
				sStart.Append(_T("\""));
			}
			sStart.Append(_T(" w:history=\"1\">"));
			wr.WriteString(sStart);
			wr.Write(writer);
			wr.WriteString(CString(_T("</w:hyperlink>")));
		}
	}
};
class CFldSimple{
public:
	CString sInstr;
	XmlUtils::CStringWriter writer;
public:
	CFldSimple()
	{
	}
	void Write(XmlUtils::CStringWriter& wr)
	{
		if(false == sInstr.IsEmpty())
		{
			CString sCorrect_Instr = XmlUtils::EncodeXmlString(sInstr);
			CString sStart(_T("<w:fldSimple w:instr=\""));
			sStart.Append(sCorrect_Instr);
			sStart.Append(_T("\">"));
			wr.WriteString(sStart);
			wr.Write(writer);
			wr.WriteString(CString(_T("</w:fldSimple>")));
		}
	}
};
class TrackRevision
{
public:
	CString Author;
	CString Date;
	long* Id;
    CString UserId;
	long* vMerge;
	long* vMergeOrigin;

	rPr* RPr;
	XmlUtils::CStringWriter* PPr;
	SectPr* sectPr;
	CWiterTblPr* tblPr;
    XmlUtils::CStringWriter* tblGridChange;
	XmlUtils::CStringWriter* trPr;
	XmlUtils::CStringWriter* tcPr;
	XmlUtils::CStringWriter* content;
	XmlUtils::CStringWriter* contentRun;
	TrackRevision()
	{
		Id = NULL;
		vMerge = NULL;
		vMergeOrigin = NULL;
		RPr = NULL;
		PPr = NULL;
		sectPr = NULL;
		tblPr = NULL;
        tblGridChange = NULL;
		trPr = NULL;
		tcPr = NULL;
		content = NULL;
		contentRun = NULL;
	}
	~TrackRevision()
	{
		RELEASEOBJECT(Id);
		RELEASEOBJECT(vMerge);
		RELEASEOBJECT(vMergeOrigin);
		RELEASEOBJECT(RPr);
		RELEASEOBJECT(PPr);
		RELEASEOBJECT(sectPr);
		RELEASEOBJECT(tblPr);
        RELEASEOBJECT(tblGridChange);
		RELEASEOBJECT(trPr);
		RELEASEOBJECT(tcPr);
		RELEASEOBJECT(content);
		RELEASEOBJECT(contentRun);
	}
	bool IsNoEmpty()
	{
		return !Author.IsEmpty() || !Date.IsEmpty() || NULL != Id;
	}
	CString ToString(CString sName)
	{
		XmlUtils::CStringWriter writer;
		Write(&writer, sName);
		return writer.GetData();
	}
	void Write(XmlUtils::CStringWriter*  pCStringWriter, CString sName)
	{
		if(IsNoEmpty())
		{
			pCStringWriter->WriteString(CString(_T("<")));
			pCStringWriter->WriteString(sName);
            if(NULL != Id)
            {
                CString sId;sId.Format(_T(" w:id=\"%d\""), *Id);
                pCStringWriter->WriteString(sId);
            }
            if(!Author.IsEmpty())
			{
				pCStringWriter->WriteString(_T(" w:author=\""));
				pCStringWriter->WriteEncodeXmlString(Author);
				pCStringWriter->WriteString(_T("\""));
			}
			if(!Date.IsEmpty())
			{
				pCStringWriter->WriteString(_T(" w:date=\""));
				pCStringWriter->WriteEncodeXmlString(Date);
				pCStringWriter->WriteString(_T("\""));
			}
            if(!UserId.IsEmpty())
            {
                pCStringWriter->WriteString(_T(" oouserid=\""));
                pCStringWriter->WriteEncodeXmlString(UserId);
                pCStringWriter->WriteString(_T("\""));
            }
			if(NULL != vMerge)
			{
				CString sId;sId.Format(_T(" w:vMerge=\"%d\""), *vMerge);
				pCStringWriter->WriteString(sId);
			}
			if(NULL != vMergeOrigin)
			{
				CString sId;sId.Format(_T(" w:vMergeOrig=\"%d\""), *vMergeOrigin);
				pCStringWriter->WriteString(sId);
			}
			if(NULL != RPr || NULL != PPr || NULL != sectPr || NULL != tblPr || NULL != tblGridChange || NULL != trPr || NULL != tcPr || NULL != content|| NULL != contentRun)
			{
				pCStringWriter->WriteString(CString(_T(">")));
				if(NULL != RPr)
				{
					RPr->Write(pCStringWriter);
				}
				if(NULL != PPr)
				{
                    pCStringWriter->WriteString(_T("<w:pPr>"));
					pCStringWriter->Write(*PPr);
                    pCStringWriter->WriteString(_T("</w:pPr>"));
				}
				if(NULL != sectPr)
				{
					pCStringWriter->WriteString(_T("<w:sectPr>"));
					pCStringWriter->WriteString(sectPr->Write());
					pCStringWriter->WriteString(_T("</w:sectPr>"));
				}
				if(NULL != tblPr)
				{
					pCStringWriter->WriteString(tblPr->Write(false, true));
				}
                if(NULL != tblGridChange)
                {
                    pCStringWriter->WriteString(CString(_T("<w:tblGrid>")));
                    pCStringWriter->Write(*tblGridChange);
                    pCStringWriter->WriteString(CString(_T("</w:tblGrid>")));
                }
				if(NULL != trPr)
				{
                    pCStringWriter->WriteString(CString(_T("<w:trPr>")));
					pCStringWriter->Write(*trPr);
                    pCStringWriter->WriteString(CString(_T("</w:trPr>")));
				}
				if(NULL != tcPr)
				{
                    pCStringWriter->WriteString(CString(_T("<w:tcPr>")));
					pCStringWriter->Write(*tcPr);
                    pCStringWriter->WriteString(CString(_T("</w:tcPr>")));
				}
				if(NULL != content)
				{
					pCStringWriter->Write(*content);
				}
				if(NULL != contentRun)
				{
					pCStringWriter->Write(*contentRun);
				}

				pCStringWriter->WriteString(CString(_T("</")));
				pCStringWriter->WriteString(sName);
				pCStringWriter->WriteString(CString(_T(" >")));
			}
			else
			{
				pCStringWriter->WriteString(CString(_T(" />")));
			}
		}
	}
};
}
#endif	// #ifndef READER_CLASSES
