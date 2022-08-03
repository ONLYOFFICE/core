﻿/*
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

#include "HeaderFooterWriter.h"

#include "../../Common/DocxFormat/Source/XML/Utils.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Logic/RunProperty.h"
#include "../BinWriter/BinReaderWriterDefines.h"

#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>

namespace BinDocxRW {

class SectPr
{
public: 
    std::wstring sHeaderFooterReference;
	long W;
	long H;
	BYTE cOrientation;
	long Left;
	long Top;
	long Right;
	long Bottom;
	long Header;
	long Footer;
	bool TitlePg;
	bool EvenAndOddHeaders;
	BYTE SectionType;
	int PageNumStart;
    std::wstring sectPrChange;
    std::wstring cols;
    std::wstring pgBorders;
    std::wstring footnotePr;
    std::wstring endnotePr;
	std::wstring lineNum;
	bool RtlGutter;
	long Gutter;

	bool bW;
	bool bH;
	bool bOrientation;
	bool bLeft;
	bool bTop;
	bool bRight;
	bool bBottom;
	bool bHeader;
	bool bFooter;
	bool bTitlePg;
	bool bEvenAndOddHeaders;
	bool bSectionType;
	bool bPageNumStart;
	bool bRtlGutter;
	bool bGutter;
	SectPr()
	{
		sHeaderFooterReference = _T("");
		cols = _T("<w:cols w:space=\"708\"/>");

		bW = false;
		bH = false;
		bOrientation = false;
		bLeft = false;
		bTop = false;
		bRight = false;
		bBottom = false;
		bHeader = false;
		bFooter = false;
		bTitlePg = false;
		bEvenAndOddHeaders = false;
		bSectionType = false;
		bPageNumStart = false;
		bRtlGutter = false;
		bGutter = false;
	}
    std::wstring Write()
	{
        std::wstring sRes = _T("");

        if(!sHeaderFooterReference.empty())
            sRes += sHeaderFooterReference;
        if(!footnotePr.empty())
            sRes += footnotePr;
        if(!endnotePr.empty())
            sRes += endnotePr;
		if(bSectionType)
		{
            std::wstring sType;
			switch(SectionType)
			{
			case 0: sType = _T("continuous");break;
			case 1: sType = _T("evenPage");break;
			case 2: sType = _T("nextColumn");break;
			case 3: sType = _T("nextPage");break;
			case 4: sType = _T("oddPage");break;
			default: sType = _T("nextPage");break;
			}
            sRes += L"<w:type w:val=\"" + sType + L"\"/>";
		}
		if((bW && bH) || bOrientation)
		{
			sRes += L"<w:pgSz";
			if(bW && bH)
			{
				sRes += L" w:w=\"" + std::to_wstring(W) + L"\" w:h=\"" + std::to_wstring(H) + L"\"";
			}
			if(bOrientation)
			{
				if(orientation_Portrait != cOrientation)
					sRes += L" w:orient=\"landscape\"";
				else
					sRes += L" w:orient=\"portrait\"";
			}

			sRes += L"/>";
		}

		if(bLeft || bTop || bRight || bBottom || bHeader || bFooter)
		{
			sRes += L"<w:pgMar";
			if(bTop)
				sRes += L" w:top=\"" + std::to_wstring(Top) + L"\"";
			if(bRight)
				sRes += L" w:right=\"" + std::to_wstring(Right) + L"\"";
			if(bBottom)
				sRes += L" w:bottom=\"" + std::to_wstring(Bottom) + L"\"";
			if(bLeft)
				sRes += L" w:left=\"" + std::to_wstring(Left) + L"\"";
			if(bHeader)
				sRes += L" w:header=\"" + std::to_wstring(Header) + L"\"";
			if(bFooter)
				sRes += L" w:footer=\"" + std::to_wstring(Footer) + L"\"";
			if(bGutter)
				sRes += L" w:gutter=\"" + std::to_wstring(Gutter) + L"\"";
			sRes += L"/>";
		}
        if(!pgBorders.empty())
            sRes += pgBorders;

		if(!lineNum.empty())
			sRes += lineNum;

        if(bPageNumStart)
            sRes += L"<w:pgNumType w:start=\"" + std::to_wstring(PageNumStart) + L"\"/>";

		if(bRtlGutter)
		{
			if(RtlGutter)
				sRes += L"<w:rtlGutter/>";
			else
				sRes += L"<w:rtlGutter w:val=\"0\"/>";
		}

        if(!cols.empty())
            sRes += cols;
        sRes += L"<w:docGrid w:linePitch=\"360\"/>";

        if(bTitlePg && TitlePg)
            sRes += L"<w:titlePg/>";

        if(!sectPrChange.empty())
            sRes += sectPrChange;
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
    std::wstring ToString()
	{
        return XmlUtils::IntToString(R, L"%02X") + XmlUtils::IntToString(G, L"%02X") + XmlUtils::IntToString(B, L"%02X");
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
    std::wstring ToStringColor()
	{
        std::wstring sRes;
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
    std::wstring ToStringTint()
	{
        std::wstring sRes;
		if(bTint)
		{
            sRes = XmlUtils::IntToString(Tint, L"%02X");
		}
		return sRes;
	}
    std::wstring ToStringShade()
	{
        std::wstring sRes;
		if(bShade)
		{
            sRes = XmlUtils::IntToString(Shade, L"%02X");
		}
		return sRes;
	}
	void ToCThemeColor( nullable<SimpleTypes::CHexColor<>>& oColor,
												nullable<SimpleTypes::CThemeColor<>>& oThemeColor,
												nullable<SimpleTypes::CUcharHexNumber<>>& oThemeTint,
												nullable<SimpleTypes::CUcharHexNumber<>>& oThemeShade)
	{
		if (Auto)
		{
			if(!oColor.IsInit())
				oColor.Init();
			oColor->SetValue(SimpleTypes::hexcolorAuto);
		}
		if (bColor)
		{
			oThemeColor.Init();
			oThemeColor->SetValue((SimpleTypes::EThemeColor)Color);
		}
		if (bTint)
		{
			oThemeTint.Init();
			oThemeTint->SetValue(Tint);
		}
		if (bShade)
		{
			oThemeShade.Init();
			oThemeShade->SetValue(Shade);
		}
	}
};
class Spacing
{
public:
	bool bLineRule;
	bool bLine;
	bool bLineTwips;
	bool bAfter;
	bool bBefore;
	bool bAfterAuto;
	bool bBeforeAuto;

	BYTE LineRule;
	double Line;
	long LineTwips;
	long After;
	long Before;
	bool AfterAuto;
	bool BeforeAuto;
	Spacing()
	{
		bLineRule = false;
		bLine = false;
		bLineTwips = false;
		bAfter = false;
		bBefore = false;
		bAfterAuto = false;
		bBeforeAuto = false;
	}
};
class Background
{
public:
    docRGB      Color;
    CThemeColor ThemeColor;

    std::wstring     sObject;

    bool bColor;
    bool bThemeColor;

    Background() : bColor (false), bThemeColor(false) {}

    std::wstring Write()
    {
        std::wstring sBackground =  L"<w:background";

        if(bColor)
        {
           sBackground += L" w:color=\"" + Color.ToString() + L"\"";
        }

        if(bThemeColor && ThemeColor.IsNoEmpty())
        {
            if(ThemeColor.bColor)
                sBackground += L" w:themeColor=\"" + ThemeColor.ToStringColor() + L"\"";
            if(ThemeColor.bTint)
                sBackground += L" w:themeTint=\"" + ThemeColor.ToStringTint() + L"\"";
            if(ThemeColor.bShade)
                sBackground += L" w:themeShade=\"" + ThemeColor.ToStringShade() + L"\"";
        }

        if (!bColor && !bThemeColor)
        {
            sBackground += L" w:color=\"ffffff\"";
        }
        sBackground += L">";

        sBackground += sObject;

        sBackground += L"</w:background>";
        return sBackground;
    }
};

class Tab
{
public:
	SimpleTypes::CTabJc<> Val;
	long Pos;
	BYTE Leader;
	bool bLeader;
	Tab()
	{
		Pos = 0;
		bLeader = false;
	}
};
class Tabs
{
public:
	std::vector<Tab> m_aTabs;
};

class docStyle
{
public:
    std::wstring Name;
    std::wstring Id;
	BYTE byteType;
	bool Default;
	bool Custom;
	std::wstring Aliases;
    std::wstring BasedOn;
    std::wstring NextId;
	std::wstring Link;
	bool qFormat;
	long uiPriority;
	bool hidden;
	bool semiHidden;
	bool unhideWhenUsed;
	bool autoRedefine;
	bool locked;
	bool personal;
	bool personalCompose;
	bool personalReply;
    std::wstring TextPr;
    std::wstring ParaPr;
    std::wstring TablePr;
    std::wstring RowPr;
    std::wstring CellPr;
    std::vector<std::wstring> TblStylePr;

	bool bDefault;
	bool bCustom;
	bool bqFormat;
	bool buiPriority;
	bool bhidden;
	bool bsemiHidden;
	bool bunhideWhenUsed;
	bool bautoRedefine;
	bool blocked;
	bool bpersonal;
	bool bpersonalCompose;
	bool bpersonalReply;

public:
	docStyle()
	{
		byteType = styletype_Paragraph;
		bDefault = false;
		bCustom = false;

		bqFormat = false;
		buiPriority = false;
		bhidden = false;
		bsemiHidden = false;
		bunhideWhenUsed = false;
		bautoRedefine = false;
		blocked = false;
		bpersonal = false;
		bpersonalCompose = false;
		bpersonalReply = false;
	}
	void Write(NSStringUtils::CStringBuilder*  pCStringWriter)
	{
        std::wstring sType;
		switch(byteType)
		{
			case styletype_Character: sType = _T("character");break;
			case styletype_Numbering: sType = _T("numbering");break;
			case styletype_Table: sType = _T("table");break;
			default: sType = _T("paragraph");break;
		}
        if(!Id.empty())
		{
            std::wstring sStyle = L"<w:style w:type=\"" + sType + L"\" w:styleId=\"" + Id + L"\"";
			if(bDefault)
			{
				if(Default)
					sStyle += L" w:default=\"1\"";
				else
					sStyle += L" w:default=\"0\"";
			}
			if(bCustom)
			{
				if(Custom)
					sStyle += L" w:customStyle=\"1\"";
				else
					sStyle += L" w:customStyle=\"0\"";
			}

            sStyle += L">";
            pCStringWriter->WriteString(sStyle);

            if(!Name.empty())
			{
                pCStringWriter->WriteString(L"<w:name w:val=\"" + Name + L"\"/>");
			}		
			if (!Aliases.empty())
			{
				pCStringWriter->WriteString(L"<w:aliases w:val=\"");
				pCStringWriter->WriteEncodeXmlString(Aliases);
				pCStringWriter->WriteString(L"\"/>");
			}
			if (!BasedOn.empty())
			{
				pCStringWriter->WriteString(L"<w:basedOn w:val=\"" + BasedOn + L"\"/>");
			}
			if (!NextId.empty())
			{
				pCStringWriter->WriteString(L"<w:next w:val=\"" + NextId + L"\"/>");
			}
			if (!Link.empty())
			{
				pCStringWriter->WriteString(L"<w:link w:val=\"");
				pCStringWriter->WriteEncodeXmlString(Link);
				pCStringWriter->WriteString(L"\"/>");
			}
			if(bautoRedefine)
			{
				if(autoRedefine)
					pCStringWriter->WriteString(L"<w:autoRedefine/>");
				else
					pCStringWriter->WriteString(L"<w:autoRedefine val=\"false\"/>");
			}
			if (bhidden)
			{
				if (hidden)
					pCStringWriter->WriteString(L"<w:hidden/>");
				else
					pCStringWriter->WriteString(L"<w:hidden val=\"false\"/>");
			}
			if (buiPriority)
			{
				pCStringWriter->WriteString(L"<w:uiPriority w:val=\"" + std::to_wstring(uiPriority) + L"\"/>");
			}
			if (bsemiHidden)
			{
				if (semiHidden)
					pCStringWriter->WriteString(L"<w:semiHidden/>");
				else
					pCStringWriter->WriteString(L"<w:semiHidden val=\"false\"/>");
			}
			if (bunhideWhenUsed)
			{
				if (unhideWhenUsed)
					pCStringWriter->WriteString(L"<w:unhideWhenUsed/>");
				else
					pCStringWriter->WriteString(L"<w:unhideWhenUsed val=\"false\"/>");
			}
			if (bqFormat)
			{
				if (qFormat)
					pCStringWriter->WriteString(L"<w:qFormat/>");
				else
					pCStringWriter->WriteString(L"<w:qFormat val=\"false\"/>");
			}
			if (blocked)
			{
				if (locked)
					pCStringWriter->WriteString(L"<w:locked/>");
				else
					pCStringWriter->WriteString(L"<w:locked val=\"false\"/>");
			}
			if(bpersonal)
			{
				if(personal)
					pCStringWriter->WriteString(L"<w:personal/>");
				else
					pCStringWriter->WriteString(L"<w:personal val=\"false\"/>");
			}
			if(bpersonalCompose)
			{
				if(personalCompose)
					pCStringWriter->WriteString(L"<w:personalCompose/>");
				else
					pCStringWriter->WriteString(L"<w:personalCompose val=\"false\"/>");
			}
			if(bpersonalReply)
			{
				if(personalReply)
					pCStringWriter->WriteString(L"<w:personalReply/>");
				else
					pCStringWriter->WriteString(L"<w:personalReply val=\"false\"/>");
			}
            if(!ParaPr.empty())
			{
                pCStringWriter->WriteString(L"<w:pPr>");
				pCStringWriter->WriteString(ParaPr);
                pCStringWriter->WriteString(L"</w:pPr>");
			}
            if(!TextPr.empty())
			{
				pCStringWriter->WriteString(TextPr);
			}

            if(!TablePr.empty())
				pCStringWriter->WriteString(TablePr);
            if(!RowPr.empty())
			{
                pCStringWriter->WriteString(L"<w:trPr>");
				pCStringWriter->WriteString(RowPr);
                pCStringWriter->WriteString(L"</w:trPr>");
			}
            if(!CellPr.empty())
			{
                pCStringWriter->WriteString(L"<w:tcPr>");
				pCStringWriter->WriteString(CellPr);
                pCStringWriter->WriteString(L"</w:tcPr>");
			}
			for(int i = 0, length = (int)TblStylePr.size(); i < length; ++i)
			{
				pCStringWriter->WriteString(TblStylePr[i]);
			}
            pCStringWriter->WriteString(L"</w:style>");
		}
	}
};
class tblStylePr
{
public:
	NSStringUtils::CStringBuilder Writer;
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
	long Left;
	long Top;
	long Right;
	long Bottom;

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
class PaddingsToWriteMM
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
public: PaddingsToWriteMM()
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
	PaddingsToWriteMM Paddings;
	int m_nDocPr;

	bool bMediaId;
	bool bType;
	bool bX;
	bool bY;
	bool bWidth;
	bool bHeight;
	bool bPaddings;
    std::wstring srId;
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
	void Write(NSStringUtils::CStringBuilder*  pCStringWriter)
	{
		if(bType)
		{
			if(c_oAscWrapStyle::Inline == Type)
			{
				if(bWidth && bHeight)
				{
					__int64 nWidth = (__int64)(g_dKoef_mm_to_emu * Width);
					__int64 nHeight = (__int64)(g_dKoef_mm_to_emu * Height);
                    std::wstring sDrawing = L"<w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"" +
                            std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/><wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/><wp:docPr id=\"" +
                            std::to_wstring(m_nDocPr) + L"\" name=\"Image\"/><wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"0\" name=\"Image\"/><pic:cNvPicPr/></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"" +
                            srId + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:stretch><a:fillRect/></a:stretch></pic:blipFill><pic:spPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" +
                            std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing>";
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
                    std::wstring sDrawing = L"<w:drawing><wp:anchor distT=\"" + std::to_wstring(nPaddingTop) + L"\" distB=\"" + std::to_wstring(nPaddingBottom)
                            + L"\" distL=\"" + std::to_wstring(nPaddingLeft) + L"\" distR=\"" + std::to_wstring(nPaddingRight)
                            + L"\" simplePos=\"0\" relativeHeight=\"1\" behindDoc=\"0\" locked=\"0\" layoutInCell=\"1\" allowOverlap=\"1\"><wp:simplePos x=\"0\" y=\"0\"/><wp:positionH relativeFrom=\"page\"><wp:posOffset>"
                            + std::to_wstring(nX) + L"</wp:posOffset></wp:positionH><wp:positionV relativeFrom=\"page\"><wp:posOffset>"
                            + std::to_wstring(nY) + L"</wp:posOffset></wp:positionV><wp:extent cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight)
                            + L"\"/><wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/><wp:wrapSquare wrapText=\"bothSides\"/><wp:docPr id=\""
                            + std::to_wstring(m_nDocPr) + L"\" name=\"Image\"/><wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/>\
</wp:cNvGraphicFramePr><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"0\" name=\"Image\"/><pic:cNvPicPr/></pic:nvPicPr><pic:blipFill><a:blip r:embed=\""
                            + srId + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:stretch><a:fillRect/></a:stretch></pic:blipFill>\
<pic:spPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:anchor></w:drawing>";

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
//	void Write(NSStringUtils::CStringBuilder*  pCStringWriter)
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
	void Write(NSStringUtils::CStringBuilder& pCStringWriter, const std::wstring& sName)
	{
		pCStringWriter.WriteString(Write(sName));
	}
    std::wstring Write(const std::wstring& sName)
	{
        std::wstring sXml;
		if(bW || (bType && bWDocx))
		{
            std::wstring sType;
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
                case 0: sType = _T("auto"); break;
                case 1: sType = _T("dxa");  break;
                case 2: sType = _T("nil");  break;
                case 3: sType = _T("pct");  break;
				}
				nVal = WDocx;
			}
            sXml = L"<" + sName + L" w:w=\"" + std::to_wstring(nVal) + L"\" w:type=\"" + sType + L"\"/>";
		}
		return sXml;
	}
};
class docBorder
{
public:
	docRGB Color;
	long Space;
	long Size;
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
	void Write(std::wstring sName, NSStringUtils::CStringBuilder*  pCStringWriter, bool bCell)
	{
		if(bValue)
		{
            pCStringWriter->WriteString(L"<");
			pCStringWriter->WriteString(sName);
			if(border_Single == Value)
                pCStringWriter->WriteString(L" w:val=\"single\"");
			else
                pCStringWriter->WriteString(L" w:val=\"none\"");
			if(bColor)
			{
                pCStringWriter->WriteString(L" w:color=\"" + Color.ToString() + L"\"");
			}
			if(bThemeColor && ThemeColor.IsNoEmpty())
			{
				if(ThemeColor.Auto && !bColor)
				{
                    std::wstring sAuto(L" w:color=\"auto\"");
					pCStringWriter->WriteString(sAuto);
				}
				if(ThemeColor.bColor)
				{
                    pCStringWriter->WriteString(L" w:themeColor=\"" + ThemeColor.ToStringColor() + L"\"");
				}
				if(ThemeColor.bTint)
				{
                    pCStringWriter->WriteString(L" w:themeTint=\"" + ThemeColor.ToStringTint() + L"\"");
				}
				if(ThemeColor.bShade)
				{
                    pCStringWriter->WriteString(L" w:themeShade=\"" + ThemeColor.ToStringShade() + L"\"");
				}
			}
			if(bSize)
			{
				pCStringWriter->WriteString(L" w:sz=\"" + std::to_wstring(Size) + L"\"");
			}
			if(bSpace)
			{
				pCStringWriter->WriteString(L" w:space=\"" + std::to_wstring(Space) + L"\"");
			}
            pCStringWriter->WriteString(L"/>");
		}
		else
		{
            pCStringWriter->WriteString(L"<");
			pCStringWriter->WriteString(sName);
			if(false != bCell)
                pCStringWriter->WriteString(L" w:val=\"nil\"");
			else
                pCStringWriter->WriteString(L" w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"");
            pCStringWriter->WriteString(L"/>");
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
	void Write(NSStringUtils::CStringBuilder*  pCStringWriter, bool bCell)
	{
		if(bTop)
            oTop.Write(L"w:top", pCStringWriter, bCell);
		if(bLeft)
            oLeft.Write(L"w:left", pCStringWriter, bCell);
		if(bBottom)
            oBottom.Write(L"w:bottom", pCStringWriter, bCell);
		if(bRight)
            oRight.Write(L"w:right", pCStringWriter, bCell);
		if(bInsideH)
            oInsideH.Write(L"w:insideH", pCStringWriter, bCell);
		if(bInsideV)
            oInsideV.Write(L"w:insideV", pCStringWriter, bCell);
		if(bBetween)
            oBetween.Write(L"w:between", pCStringWriter, bCell);
	}
};
class docLvlText
{
public:
    std::wstring Text;
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
	long ILvl;
	long Format;
	std::wstring sFormat;
	BYTE Jc;
	std::wstring sJc;
	std::vector<docLvlText*> Text;
	long Restart;
	long Start;
	BYTE Suff;
	NSStringUtils::CStringBuilder ParaPr;
	NSStringUtils::CStringBuilder TextPr;
    std::wstring PStyle;
	bool Tentative;
	unsigned long Tplc;
	bool IsLgl;
	bool Legacy;
	long LegacyIndent;
	unsigned long LegacySpace;

	bool bILvl;
	bool bFormat;
	bool bJc;
	bool bText;
	bool bRestart;
	bool bStart;
	bool bSuff;
	bool bParaPr;
	bool bTextPr;
	bool bPStyle;
	bool bTentative;
	bool bTplc;
	bool bIsLgl;
	bool bLvlLegacy;
	bool bLegacy;
	bool bLegacyIndent;
	bool bLegacySpace;

	docLvl()
	{
		bILvl = false;
		bFormat = false;
		bJc = false;
		bText = false;
		bRestart = false;
		bStart = false;
		bSuff = false;
		bParaPr = false;
		bTextPr = false;
		bPStyle = false;
		bTentative = false;
		bTplc = false;
		bIsLgl = false;
		bLvlLegacy = false;
		bLegacy = false;
		bLegacyIndent = false;
		bLegacySpace = false;
	}
	~docLvl()
	{
		for(int i = 0,length = (int)Text.size(); i < length; i++)
		{
			delete Text[i];
		}
	}
	void Write(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<w:lvl");
		if(bILvl)
		{
			oWriter.WriteString(L" w:ilvl=\"" + std::to_wstring(ILvl) + L"\"");
		}
		if(bTentative)
		{
			if(Tentative)
				oWriter.WriteString(L" w:tentative=\"1\"");
			else
				oWriter.WriteString(L" w:tentative=\"0\"");
		}
		if(bTplc)
		{
			oWriter.WriteString(L" w:tplc=\"");
			oWriter.WriteString(XmlUtils::IntToString(Tplc, L"%08X"));
			oWriter.WriteString(L"\"");
		}
		oWriter.WriteString(L">");
		if(bStart)
		{
            oWriter.WriteString(L"<w:start w:val=\"" + std::to_wstring(Start) + L"\"/>");
		}
		if(!sFormat.empty())
		{
			oWriter.WriteString(sFormat);
		}
		else if(bFormat)
		{
            std::wstring sFormat;
			switch(Format)
			{
			case numbering_numfmt_None:			sFormat = L"none";			break;
			case numbering_numfmt_Bullet:		sFormat = L"bullet";		break;
			case numbering_numfmt_Decimal:		sFormat = L"decimal";		break;
			case numbering_numfmt_LowerRoman:	sFormat = L"lowerRoman";	break;
			case numbering_numfmt_UpperRoman:	sFormat = L"upperRoman";	break;
			case numbering_numfmt_LowerLetter:	sFormat = L"lowerLetter";	break;
			case numbering_numfmt_UpperLetter:	sFormat = L"upperLetter";	break;
			case numbering_numfmt_DecimalZero:	sFormat = L"decimalZero";	break;
			default:
			{
				Format -= 0x2008;
				if (Format >= 0)
				{
					SimpleTypes::CNumberFormat<> numFormat;
					numFormat.SetValue((SimpleTypes::ENumberFormat)Format);

					sFormat = numFormat.ToString();
				}
			}break;
			}
            if(!sFormat.empty())
			{
                oWriter.WriteString(L"<w:numFmt w:val=\"" + sFormat + L"\"/>");
			}
		}
		if(bRestart)
		{
            oWriter.WriteString(L"<w:lvlRestart w:val=\"" + std::to_wstring(Restart) + L"\"/>");
		}
		if(bPStyle)
		{
            std::wstring sStyleName = XmlUtils::EncodeXmlString(PStyle);
            oWriter.WriteString(L"<w:pStyle w:val=\"" + sStyleName + L"\"/>");
		}
		if(bIsLgl)
		{
			if(IsLgl)
				oWriter.WriteString(L"<w:isLgl/>");
			else
				oWriter.WriteString(L"<w:isLgl w:val=\"false\"/>");
		}
		if(bSuff)
		{
            std::wstring sSuff;
			switch(Suff)
			{
			case numbering_suff_Nothing: sSuff = _T("nothing");break;
			case numbering_suff_Space: sSuff = _T("space");break;
			default: sSuff = _T("tab");break;
			}
            oWriter.WriteString(L"<w:suff w:val=\"");
			oWriter.WriteString(sSuff);
            oWriter.WriteString(L"\"/>");
		}
		if(bText)
		{
            std::wstring sText;
			for(int i = 0, length = (int)Text.size(); i < length; ++i)
			{
				docLvlText* item = Text[i];
				if(item->bText)
				{
                    sText += (item->Text);
				}
				else if(item->bNumber)
				{
                    sText += L"%" + std::to_wstring(item->Number+1);
				}
			}
			sText = XmlUtils::EncodeXmlString(sText);
            
            //std::wstring sTextXml;sTextXml.Format(L"<w:lvlText w:val=\"%ls\"/>", sText);
          
            std::wstring sTextXml(L"<w:lvlText w:val=\"");
            sTextXml += sText;
            sTextXml += _T("\"/>");

			oWriter.WriteString(sTextXml);
		}
		if(bLvlLegacy)
		{
			 oWriter.WriteString(L"<w:legacy");
			 if(bLegacy)
			 {
				 if(Legacy)
					 oWriter.WriteString(L" w:legacy=\"1\"");
				 else
					 oWriter.WriteString(L" w:legacy=\"0\"");
			 }
			 if(bLegacyIndent)
			 {
				 oWriter.WriteString(L" w:legacyIndent=\"");
				 oWriter.WriteString(std::to_wstring(LegacyIndent));
				 oWriter.WriteString(L"\"");
			 }
			 if(bLegacySpace)
			 {
				 oWriter.WriteString(L" w:legacySpace=\"");
				 oWriter.WriteString(std::to_wstring(LegacySpace));
				 oWriter.WriteString(L"\"");
			 }
			 oWriter.WriteString(L"/>");
		}
		if(!sJc.empty())
		{
			oWriter.WriteString(sJc);
		}
		else if(bJc)
		{
			std::wstring sJcType;
			switch(Jc)
			{
			case align_Right:sJcType = _T("right");break;
			case align_Left:sJcType = _T("left");break;
			case align_Center:sJcType = _T("center");break;
			case align_Justify:sJcType = _T("distribute");break;
			}
			if(!sJcType.empty())
			{
				oWriter.WriteString(L"<w:lvlJc w:val=\"" + sJcType + L"\"/>");
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
        oWriter.WriteString(L"</w:lvl>");
	}
};
class docLvlOverride
{
public:
	long ILvl;
	long StartOverride;
	docLvl* Lvl;

	bool bILvl;
	bool bStartOverride;
	docLvlOverride()
	{
		bILvl = false;
		bStartOverride = false;
		Lvl = NULL;
	}
	~docLvlOverride()
	{
		RELEASEOBJECT(Lvl);
	}
	void Write(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<w:lvlOverride");
		if (bILvl)
		{
			oWriter.WriteString(L" w:ilvl=\"");
			oWriter.WriteString(std::to_wstring(ILvl));
			oWriter.WriteString(L"\"");
		}
		oWriter.WriteString(L">");
		if (bStartOverride)
		{
			oWriter.WriteString(L"<w:startOverride w:val=\"");
			oWriter.WriteString(std::to_wstring(StartOverride));
			oWriter.WriteString(L"\"/>");
		}
		if(NULL != Lvl)
		{
			Lvl->Write(oWriter);
		}
		oWriter.WriteString(L"</w:lvlOverride>");
	}
};
class docANum
{
public:
	long Id;
    std::wstring NumStyleLink;
    std::wstring StyleLink;
	std::vector<docLvl*> Lvls;

	bool bId;
	docANum()
	{
		bId = false;
	}
	~docANum()
	{
		for(int i = 0, length = (int)Lvls.size(); i < length; i++)
		{
			delete Lvls[i];
		}
	}
	void Write(NSStringUtils::CStringBuilder& oWriterANum)
	{
		if(bId)
		{
            oWriterANum.WriteString(L"<w:abstractNum w:abstractNumId=\"" + std::to_wstring(Id) + L"\"><w:multiLevelType w:val=\"hybridMultilevel\"/>");
            if(!StyleLink.empty())
			{
                std::wstring sCorrectStyleLink = XmlUtils::EncodeXmlString(StyleLink);
                oWriterANum.WriteString(L"<w:styleLink w:val=\"" + sCorrectStyleLink + L"\"/>");
			}
            if(!NumStyleLink.empty())
			{
                std::wstring sCorrectNumStyleLink = XmlUtils::EncodeXmlString(NumStyleLink);
                oWriterANum.WriteString(L"<w:numStyleLink w:val=\"" + sCorrectNumStyleLink + L"\"/>");
			}
			for(int i = 0, length = (int)Lvls.size(); i < length; ++i)
			{
				Lvls[i]->Write(oWriterANum);
			}
            oWriterANum.WriteString(L"</w:abstractNum>");
		}
	}
};
class docNum
{
public:
	long AId;
	long Id;
	std::vector<docLvlOverride*> LvlOverrides;

	bool bAId;
	bool bId;
	docNum()
	{
		bAId = false;
		bId = false;
	}
	~docNum()
	{
		for(size_t i = 0; i < LvlOverrides.size(); ++i){
			RELEASEOBJECT(LvlOverrides[i]);
		}
	}
	void Write(NSStringUtils::CStringBuilder& oWriterNumList)
	{
		if(bAId && bId)
		{
            oWriterNumList.WriteString(L"<w:num w:numId=\"" + std::to_wstring(Id) + L"\"><w:abstractNumId w:val=\"" + std::to_wstring(AId) + L"\"/>");
			for(size_t i = 0; i < LvlOverrides.size(); ++i){
				LvlOverrides[i]->Write(oWriterNumList);
			}
			oWriterNumList.WriteString(L"</w:num>");
		}
	}
};
class rowPrAfterBefore
{
public:
    std::wstring sName;
	long nGridAfter;
	docW oAfterWidth;
	bool bGridAfter;
    rowPrAfterBefore(std::wstring name)
	{
		sName = name;
		bGridAfter = false;
	}
	void Write(NSStringUtils::CStringBuilder& writer)
	{
		if(bGridAfter && nGridAfter > 0)
		{
            writer.WriteString(L"<w:grid" + sName + L" w:val=\"" + std::to_wstring(nGridAfter) + L"\"/>");
		}
		if(oAfterWidth.bW)
			oAfterWidth.Write(writer, _T("w:w") + sName);
	}
};
class WriteHyperlink
{
public:
    std::wstring rId;
    std::wstring href;
    std::wstring anchor;
    std::wstring tooltip;
	NSStringUtils::CStringBuilder writer;
    static WriteHyperlink* Parse(std::wstring fld)
	{
		WriteHyperlink* res = NULL;
        if(-1 != fld.find(L"HYPERLINK"))
        {
            std::wstring sLink;
            std::wstring sTooltip;
            bool bNextLink = false;
            bool bNextTooltip = false;
            //разбиваем по пробелам, но с учетом кавычек
            std::vector<std::wstring> aItems;
            std::wstring sCurItem;
            bool bDQuot = false;
            
			for(int i = 0, length = (int)fld.length(); i < length; ++i)
            {
                wchar_t sCurLetter = fld[i];
                if('\"' == sCurLetter)
                    bDQuot = !bDQuot;
                else if('\\' == sCurLetter && true == bDQuot && i + 1 < length && '\"' == fld[i + 1])
                {
					i++;
					sCurItem += fld[i];
                }
                else if(' ' == sCurLetter && false == bDQuot)
                {
                    if(sCurItem.length() > 0)
                    {
						aItems.push_back(sCurItem);
                        sCurItem = _T("");
                    }
                }
                else
                    sCurItem += sCurLetter;
            }
            if(sCurItem.length() > 0)
                aItems.push_back(sCurItem);
			
			for(int i = 0, length = (int)aItems.size(); i < length; ++i)
            {
                std::wstring item = aItems[i];
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

                if(L"HYPERLINK" == item)
					bNextLink = true;
                else if(L"\\o" == item)
					bNextTooltip = true;
            }
            if(false == sLink.empty())
			{
				res = new WriteHyperlink();
                boost::algorithm::trim(sLink);

                int nAnchorIndex = (int)sLink.find(L"#");
				if(-1 != nAnchorIndex)
				{
                    res->href   = sLink.substr(0, nAnchorIndex);
                    res->anchor = sLink.substr(nAnchorIndex);
				}
				else
					res->href = sLink;
                if(false == sTooltip.empty())
                {
                    res->tooltip = boost::algorithm::trim_copy(sTooltip);
                }
			}
        }
		return res;
	}
	void Write(NSStringUtils::CStringBuilder& wr)
	{
        if(false == rId.empty())
		{
            std::wstring sCorrect_rId = XmlUtils::EncodeXmlString(rId);
            std::wstring sCorrect_tooltip = XmlUtils::EncodeXmlString(tooltip);
            std::wstring sCorrect_anchor = XmlUtils::EncodeXmlString(anchor);
            std::wstring sStart = L"<w:hyperlink r:id=\"" + sCorrect_rId + L"\"";

            if(false == tooltip.empty())
			{
                sStart += L" w:tooltip=\"";
                sStart += sCorrect_tooltip;
                sStart += L"\"";
			}
            if(false == anchor.empty())
			{
                sStart += L" w:anchor=\"";
                sStart += sCorrect_anchor;
                sStart += L"\"";
			}
            sStart += L" w:history=\"1\">";
			wr.WriteString(sStart);
			wr.Write(writer);
            wr.WriteString(L"</w:hyperlink>");
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
	int getCurrentId()
	{
		return m_nId;
	}
};
class CComment
{
private:
	IdCounter& m_oParaIdCounter;
	IdCounter& m_oFormatIdCounter;
public:
	void *pBinary_DocumentTableReader;

	int IdOpen;
	int IdFormat;
    std::wstring UserName;
	std::wstring Initials;
    std::wstring UserId;
	std::wstring ProviderId;
    std::wstring Date;
	std::wstring OOData;
	std::wstring DateUtc;
	std::wstring UserData;
	bool Solved;
	unsigned int DurableId;
    std::wstring Text;
	std::wstring sContent;
    
	std::wstring sParaId;
    std::wstring sParaIdParent;
	std::vector<CComment*> replies;

	bool bIdOpen;
	bool bIdFormat;
	bool bSolved;
	bool bDurableId;
public:
	CComment(IdCounter& oParaIdCounter, IdCounter& oFormatIdCounter):m_oParaIdCounter(oParaIdCounter),m_oFormatIdCounter(oFormatIdCounter)
	{
		bIdOpen = false;
		bIdFormat = false;
		bSolved = false;
		bDurableId = false;
	}
	~CComment()
	{
		for(size_t i = 0; i <replies.size(); ++i)
		{
			delete replies[i];
		}
		replies.clear();
	}
	int getCount()
	{
		return (int)replies.size() + 1;
	}
	void setFormatStart(int IdFormatStart)
	{
		bIdFormat = true;
		IdFormat = IdFormatStart;
		
		for(size_t i = 0; i < replies.size(); ++i)
		{
			CComment* pComment	= replies[i];
			pComment->bIdFormat = true;
			pComment->IdFormat	= (int)(IdFormatStart + i + 1);
		} 
	}
    std::wstring writeRef(const std::wstring& sBefore, const std::wstring& sRef, const std::wstring& sAfter)
	{
        std::wstring sRes;
        sRes += (writeRef(this, sBefore, sRef, sAfter));
		
		for(size_t i = 0; i< replies.size(); ++i)
		{
            sRes += (writeRef(replies[i], sBefore, sRef, sAfter));
		}
		return sRes;
	}
    static std::wstring writeRef(CComment* pComment, const std::wstring& sBefore, const std::wstring& sRef, const std::wstring& sAfter)
	{
        std::wstring sRes;
		if(!pComment->bIdFormat)
		{
			pComment->bIdFormat = true;
			pComment->IdFormat = pComment->m_oFormatIdCounter.getNextId();
		}
        sRes += (sBefore);
        sRes += L"<" + sRef + L" w:id=\"" + std::to_wstring(pComment->IdFormat) + L"\"/>";
        sRes += (sAfter);
		return sRes;
	}
	static void writeContentWritePart(CComment* pComment, std::wstring& sText, int nPrevIndex, int nCurIndex, std::wstring& sRes)
	{
        std::wstring sPart;
		if(nPrevIndex < nCurIndex)
            sPart = XmlUtils::EncodeXmlString(sText.substr(nPrevIndex, nCurIndex - nPrevIndex));

        int nId = pComment->m_oParaIdCounter.getNextId();

		pComment->sParaId = XmlUtils::IntToString(nId, L"%08X");
		sRes += L"<w:p w14:paraId=\"" + pComment->sParaId + L"\" w14:textId=\"" + pComment->sParaId + L"\">";
        sRes += L"<w:pPr><w:spacing w:line=\"240\" w:after=\"0\" w:lineRule=\"auto\" w:before=\"0\"/><w:ind w:firstLine=\"0\" w:left=\"0\" w:right=\"0\"/><w:jc w:val=\"left\"/></w:pPr><w:r><w:rPr><w:rFonts w:eastAsia=\"Arial\" w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\"/><w:sz w:val=\"22\"/></w:rPr><w:t xml:space=\"preserve\">";
        sRes += sPart;
        sRes += L"</w:t></w:r></w:p>";
	}
    static std::wstring writeContent(CComment* pComment)
	{
        std::wstring sRes;
		if(!pComment->bIdFormat)
		{
			pComment->bIdFormat = true;
			pComment->IdFormat = pComment->m_oFormatIdCounter.getNextId();
		}
        sRes += L"<w:comment w:id=\"" + std::to_wstring(pComment->IdFormat) + L"\"";
        if(false == pComment->UserName.empty())
		{
            std::wstring sUserName = XmlUtils::EncodeXmlString(pComment->UserName);
            sRes += L" w:author=\"";
            sRes += (sUserName);
			sRes += L"\"";
		}
        if(false == pComment->Date.empty())
		{
            std::wstring sDate = XmlUtils::EncodeXmlString(pComment->Date);
            sRes += L" w:date=\"";
			sRes += sDate;
            sRes += L"\"";
		}
		if(false == pComment->Initials.empty())
		{
            sRes += L" w:initials=\"";
			sRes += XmlUtils::EncodeXmlString(pComment->Initials);
            sRes += L"\"";
		}
        sRes += L">";

		if (false == pComment->sContent.empty())
		{
			sRes += pComment->sContent;
		}
		else
		{
	//old comments
			std::wstring sText = pComment->Text;

			XmlUtils::replace_all(sText, L"\r", L"");

			int nPrevIndex = 0;
			for (int i = 0; i < (int)sText.length(); i++)
			{
				wchar_t cToken = sText[i];
				if('\n' == cToken)
				{
					writeContentWritePart(pComment, sText, nPrevIndex, i, sRes);
					nPrevIndex = i + 1;
				}
			}
			writeContentWritePart(pComment, sText, nPrevIndex, (int)sText.length(), sRes);
		}     
		sRes += L"</w:comment>";
		return sRes;
	}
    static std::wstring writeContentExt(CComment* pComment)
	{
        std::wstring sRes;
        if(false == pComment->sParaId.empty())
		{
            std::wstring sDone(L"0");
			if(pComment->bSolved && pComment->Solved)
				sDone = _T("1");
            if(!pComment->sParaIdParent.empty())
                sRes += L"<w15:commentEx w15:paraId=\"" + pComment->sParaId + L"\" \
w15:paraIdParent=\"" + pComment->sParaIdParent + L"\" w15:done=\"" + sDone + L"\"/>";
			else
                sRes += L"<w15:commentEx w15:paraId=\"" + pComment->sParaId + L"\" w15:done=\"" + sDone + L"\"/>";
			//расставляем paraIdParent
			for(size_t i = 0; i < pComment->replies.size(); i++)
				pComment->replies[i]->sParaIdParent = pComment->sParaId;
		}
		return sRes;
	}
	static std::wstring writeContentExtensible(CComment* pComment)
	{
		std::wstring sRes;
		if(pComment->bDurableId && !pComment->DateUtc.empty())
		{
			sRes += L"<w16cex:commentExtensible w16cex:durableId=\"" + XmlUtils::IntToString(pComment->DurableId, L"%08X") + L"\" w16cex:dateUtc=\"" + pComment->DateUtc + L"\"/>";
		}
		return sRes;
	}
	static std::wstring writeContentUserData(CComment* pComment)
	{
		std::wstring sRes;
		if(pComment->bDurableId && !pComment->UserData.empty())
		{
			sRes += L"<w16cex:commentExtensible w16cex:durableId=\"" + XmlUtils::IntToString(pComment->DurableId, L"%08X") + L"\">";
			sRes += L"<w16cex:extLst><w16cex:ext uri=\"{19B8F6BF-5375-455C-9EA6-DF929625EA0E}\"><p15:presenceInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\" userId=\"";
			sRes += XmlUtils::EncodeXmlStringExtend(pComment->UserData);
			sRes += L"\" providerId=\"AD\"/></w16cex:ext></w16cex:extLst></w16cex:commentExtensible>";
		}
		return sRes;
	}
	static std::wstring writeContentsIds(CComment* pComment)
	{
		std::wstring sRes;
		if(!pComment->sParaId.empty() && pComment->bDurableId)
		{
			sRes += L"<w16cid:commentId w16cid:paraId=\"" + pComment->sParaId + L"\" w16cid:durableId=\"" + XmlUtils::IntToString(pComment->DurableId, L"%08X") + L"\"/>";
		}
		return sRes;
	}
    static std::wstring writePeople(CComment* pComment)
	{
        std::wstring sRes;
		if(false == pComment->UserName.empty())
		{
            sRes += L"<w15:person w15:author=\"";
			sRes += XmlUtils::EncodeXmlString(pComment->UserName);
			sRes += L"\">";
			if(!pComment->ProviderId.empty() && !pComment->UserId.empty())
			{
				sRes += L"<w15:presenceInfo w15:providerId=\"";
				sRes += XmlUtils::EncodeXmlString(pComment->ProviderId);
				sRes += L"\" w15:userId=\"";
				sRes += XmlUtils::EncodeXmlString(pComment->UserId);
				sRes += L"\"/>";
			}
			sRes += L"</w15:person>";
		}
		return sRes;
	}
};
class CComments
{
    boost::unordered_map<int, CComment*>          m_mapComments;
    boost::unordered_map<std::wstring, CComment*> m_mapAuthors;
public:
	IdCounter m_oFormatIdCounter;
	IdCounter m_oParaIdCounter;

	CComments() : m_oParaIdCounter(1)
	{
	}
	~CComments()
	{
        for (boost::unordered_map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
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
			for(size_t i = 0; i < pComment->replies.size(); i++)
				addAuthor(pComment->replies[i]);
		}
	}
	void addAuthor(CComment* pComment)
	{
        if(false == pComment->UserName.empty() && false == pComment->UserId.empty())
			m_mapAuthors[pComment->UserName] = pComment;
	}
	CComment* get(int nInd)
	{
		CComment* pRes = NULL;
        boost::unordered_map<int, CComment*>::const_iterator pair = m_mapComments.find(nInd);
		if(m_mapComments.end() != pair)
			pRes = pair->second;
		return pRes;
	}
	int getNextId(int nCount = 1)
	{
		return m_oFormatIdCounter.getNextId(nCount);
	}
    std::wstring writeContent()
	{
        std::wstring sRes;
        for (boost::unordered_map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sRes += CComment::writeContent(it->second);
			for(size_t i = 0; i < it->second->replies.size(); ++i)
				sRes += CComment::writeContent(it->second->replies[i]);
		}
		return sRes;
	}
    std::wstring writeContentExt()
	{
        std::wstring sRes;
        for (boost::unordered_map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sRes += CComment::writeContentExt(it->second);
			for(size_t i = 0; i < it->second->replies.size(); ++i)
				sRes += CComment::writeContentExt(it->second->replies[i]);
		}
		return sRes;
	}
	std::wstring writeContentExtensible()
	{
		std::wstring sRes;
		for (boost::unordered_map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sRes += CComment::writeContentExtensible(it->second);
			for(size_t i = 0; i < it->second->replies.size(); ++i)
				sRes += CComment::writeContentExtensible(it->second->replies[i]);
		}
		return sRes;
	}
	std::wstring writeContentUserData()
	{
		std::wstring sRes;
		for (boost::unordered_map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sRes += CComment::writeContentUserData(it->second);
			for(size_t i = 0; i < it->second->replies.size(); ++i)
				sRes += CComment::writeContentUserData(it->second->replies[i]);
		}
		return sRes;
	}
	std::wstring writeContentsIds()
	{
		std::wstring sRes;
		for (boost::unordered_map<int, CComment*>::const_iterator it = m_mapComments.begin(); it != m_mapComments.end(); ++it)
		{
			sRes += CComment::writeContentsIds(it->second);
			for(size_t i = 0; i < it->second->replies.size(); ++i)
				sRes += CComment::writeContentsIds(it->second->replies[i]);
		}
		return sRes;
	}
    std::wstring writePeople()
	{
        std::wstring sRes;
        for (boost::unordered_map<std::wstring, CComment*>::const_iterator it = m_mapAuthors.begin(); it != m_mapAuthors.end(); ++it)
		{
            sRes += (it->second->writePeople(it->second));
		}
		return sRes;
	}
};

class CDrawingPropertyWrapPoint
{
public:
	__int64 X;
	__int64 Y;

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
		for(size_t i = 0; i < Points.size(); ++i)
			delete Points[i];
		Points.clear();
	}
};
class CDrawingProperty
{
public:
	bool			bObject;
    std::wstring	sObjectProgram;	
    unsigned int	nObjectId;
    BYTE			nObjectType;
	
	long DataPos;
    long DataLength;
    BYTE Type;
    bool BehindDoc;
	__int64 DistL;
	__int64 DistT;
	__int64 DistR;
	__int64 DistB;
    bool LayoutInCell;
	unsigned long RelativeHeight;
    bool BSimplePos;
	__int64 EffectExtentL;
	__int64 EffectExtentT;
	__int64 EffectExtentR;
	__int64 EffectExtentB;
	__int64 Width;
	__int64 Height;
    BYTE PositionHRelativeFrom;
    BYTE PositionHAlign;
	__int64 PositionHPosOffset;
	double PositionHPctOffset;
    BYTE PositionVRelativeFrom;
    BYTE PositionVAlign;
	__int64 PositionVPosOffset;
    double PositionVPctOffset;
	__int64 SimplePosX;
	__int64 SimplePosY;
    std::wstring sSizeRelH;
    std::wstring sSizeRelV;
    int m_nDocPr;
    std::wstring sGraphicFramePr;
    std::wstring sDocPr;
   
    std::wstring sGraphicFrameContent;

	CDrawingPropertyWrap DrawingPropertyWrap;

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
        m_nDocPr    = nDocPr;

		bObject		= false;
		nObjectType	= 0;
		nObjectId	= 0;
        bDataPos    = false;
		bDataLength = false;
        bType       = false;
        bBehindDoc  = false;
        bDistL      = false;
        bDistT      = false;
        bDistR      = false;
        bDistB      = false;
        bLayoutInCell   = false;
		bRelativeHeight = false;
        bBSimplePos     = false;
        bEffectExtentL  = false;
        bEffectExtentT  = false;
        bEffectExtentR  = false;
        bEffectExtentB  = false;
        bWidth          = false;
        bHeight         = false;
		bPositionHRelativeFrom = false;
        bPositionHAlign     = false;
		bPositionHPosOffset = false;
		bPositionHPctOffset = false;
		bPositionVRelativeFrom = false;
        bPositionVAlign     = false;
		bPositionVPosOffset = false;
		bPositionVPctOffset = false;
        bSimplePosX         = false;
        bSimplePosY         = false;
		bDrawingPropertyWrap = false;
	}
	bool IsGraphicFrameContent()
	{
        return false == sGraphicFrameContent.empty();
	}
    std::wstring Write()
	{
        if(!bType) return L"";

        std::wstring sXml;

        bool bGraphicFrameContent = IsGraphicFrameContent();
       
		if(c_oAscWrapStyle::Inline == Type)
        {
            if(bWidth && bHeight)
            {
				if (bGraphicFrameContent)
				{
					sXml += L"<w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">\
<wp:extent cx=\"" + std::to_wstring(Width) + L"\" cy=\"" + std::to_wstring(Height) + L"\"/>";		
				}
				else
				{
					sXml += L"<wp:inline \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"" + std::to_wstring(Width) + L"\" cy=\"" + std::to_wstring(Height) + L"\"/>";
				}

                if(bEffectExtentL && bEffectExtentT && bEffectExtentR && bEffectExtentB)
                {
					sXml += L"<wp:effectExtent l=\"" + std::to_wstring(EffectExtentL) + L"\" t=\"" + std::to_wstring(EffectExtentT) +
							 L"\" r=\"" + std::to_wstring(EffectExtentR) + L"\" b=\"" + std::to_wstring(EffectExtentB) + L"\"/>";
                }

                if(!sDocPr.empty())
                {
                    sXml += sDocPr;
                }
                else
                {
                    sXml += L"<wp:docPr id=\"" + std::to_wstring(m_nDocPr) + L"\" name=\"\"/>";
                }
                if (!sGraphicFramePr.empty())
                {
                    sXml += sGraphicFramePr;
                }
                else
                {
                    sXml += L"<wp:cNvGraphicFramePr/>";
                }
                if (bGraphicFrameContent)
                {
					sXml += sGraphicFrameContent + L"</wp:inline></w:drawing>";
                }
                else
                {
                    sXml += L"</wp:inline>";
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
					emuDistL = DistL;
                if(bDistT)
					emuDistT = DistT;
                if(bDistR)
					emuDistR = DistR;
                if(bDistB)
					emuDistB = DistB;
                int nSimplePos = 0;
                if(bBSimplePos && BSimplePos)
                    nSimplePos = 1;
                unsigned long nRelativeHeight = 0;
                if(bRelativeHeight)
                    nRelativeHeight = RelativeHeight;
                int nBehindDoc = 0;
                if(bBehindDoc && BehindDoc)
                    nBehindDoc = 1;
                int nLayoutInCell = 1;
                if(bLayoutInCell && false == LayoutInCell)
                    nLayoutInCell = 0;

				if (bGraphicFrameContent)
				{
					sXml += L"<w:drawing>";
				}
                sXml += L"<wp:anchor \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
distT=\"" + std::to_wstring(emuDistT) + L"\" \
distB=\"" + std::to_wstring(emuDistB) + L"\" \
distL=\"" + std::to_wstring(emuDistL) + L"\" \
distR=\"" + std::to_wstring(emuDistR) + L"\" \
simplePos=\"" + std::to_wstring(nSimplePos) + L"\" \
relativeHeight=\"" + std::to_wstring(nRelativeHeight) + L"\" \
behindDoc=\"" + std::to_wstring(nBehindDoc) + L"\" \
locked=\"0\" \
layoutInCell=\"" + std::to_wstring(nLayoutInCell) + L"\" \
allowOverlap=\"1\">";

                __int64 emuX = 0;
                if(bSimplePosX)
					emuX = SimplePosX;
                __int64 emuY = 0;
                if(bSimplePosY)
					emuY = SimplePosY;
                sXml += L"<wp:simplePos x=\"" + std::to_wstring(emuX) + L"\" y=\"" + std::to_wstring(emuY) + L"\"/>";

                if (bPositionHRelativeFrom && (bPositionHAlign || bPositionHPosOffset || bPositionHPctOffset))
                {
                    std::wstring sRelativeFrom;
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
                    std::wstring sContent;
                    if(bPositionHAlign)
                    {
                        switch(PositionHAlign)
                        {
                        case 0: sContent = _T("<wp:align>center</wp:align>");   break;
                        case 1: sContent = _T("<wp:align>inside</wp:align>");   break;
                        case 2: sContent = _T("<wp:align>left</wp:align>");     break;
                        case 3: sContent = _T("<wp:align>outside</wp:align>");  break;
                        case 4: sContent = _T("<wp:align>right</wp:align>");    break;
                        }
                    }
                    else if(bPositionHPosOffset)
                    {
						sContent = L"<wp:posOffset>" + std::to_wstring(PositionHPosOffset) + L"</wp:posOffset>";
                    }
                    else if(bPositionHPctOffset)
                    {
                        long pctOffset = (long)(1000 * PositionHPctOffset);
                        sContent = L"<wp14:pctPosHOffset>" + std::to_wstring(pctOffset) + L"</wp14:pctPosHOffset>";
                    }
                    sXml += L"<wp:positionH relativeFrom=\"" + sRelativeFrom + L"\">" + sContent + L"</wp:positionH>";
                }
                if (bPositionVRelativeFrom && (bPositionVAlign || bPositionVPosOffset || bPositionVPctOffset))
                {
                    std::wstring sRelativeFrom;
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
                    std::wstring sContent;
                    if (bPositionVAlign)
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
						sContent = L"<wp:posOffset>" + std::to_wstring(PositionVPosOffset) + L"</wp:posOffset>";
                    }
                    else if(bPositionVPctOffset)
                    {
                        long pctOffset = (long)(1000 * PositionVPctOffset);
                        sContent = L"<wp14:pctPosVOffset>" + std::to_wstring(pctOffset) + L"</wp14:pctPosVOffset>";
                    }
                    sXml += L"<wp:positionV relativeFrom=\"" + sRelativeFrom + L"\">" + sContent + L"</wp:positionV>";
                }
				sXml += L"<wp:extent cx=\"" + std::to_wstring(Width) + L"\" cy=\"" + std::to_wstring(Height)+ L"\"/>";

                if (bEffectExtentL && bEffectExtentT && bEffectExtentR && bEffectExtentB)
                {
					sXml += L"<wp:effectExtent l=\"" + std::to_wstring(EffectExtentL) +
										  L"\" t=\"" + std::to_wstring(EffectExtentT) + L"\" r=\"" + std::to_wstring(EffectExtentR) +
										  L"\" b=\"" + std::to_wstring(EffectExtentB) + L"\"/>";
                }
                if (bDrawingPropertyWrap && DrawingPropertyWrap.bWrappingType)
                {
                    std::wstring sTagName;
                    switch(DrawingPropertyWrap.WrappingType)
                    {
						case c_oSerImageType2::WrapNone:sTagName        = _T("wrapNone");           break;
						case c_oSerImageType2::WrapSquare:sTagName      = _T("wrapSquare");         break;
						case c_oSerImageType2::WrapThrough:sTagName     = _T("wrapThrough");        break;
						case c_oSerImageType2::WrapTight:sTagName       = _T("wrapTight");          break;
						case c_oSerImageType2::WrapTopAndBottom:sTagName = _T("wrapTopAndBottom");  break;
                    }
                    if(DrawingPropertyWrap.bStart || DrawingPropertyWrap.Points.size() > 0)
                    {
                        if( c_oSerImageType2::WrapSquare	== DrawingPropertyWrap.WrappingType		||
                            c_oSerImageType2::WrapThrough	== DrawingPropertyWrap.WrappingType		||
                            c_oSerImageType2::WrapTight		== DrawingPropertyWrap.WrappingType)
                        {
                            sXml += L"<wp:" + sTagName + L" wrapText=\"bothSides\">";
                        }
                        else
                            sXml += L"<wp:" + sTagName + L">";

                        int nEdited = 0;
                        if(DrawingPropertyWrap.bEdited && DrawingPropertyWrap.Edited)
                            nEdited = 1;
                        sXml += L"<wp:wrapPolygon edited=\"" + std::to_wstring(nEdited) + L"\">";

                        if(DrawingPropertyWrap.bStart && DrawingPropertyWrap.Start.bX && DrawingPropertyWrap.Start.bY)
                        {
							sXml += L"<wp:start x=\"" + std::to_wstring(DrawingPropertyWrap.Start.X) + L"\" y=\"" + std::to_wstring(DrawingPropertyWrap.Start.Y) + L"\"/>";
                        }

                        for(size_t i = 0; i < DrawingPropertyWrap.Points.size(); ++i)
                        {
                            CDrawingPropertyWrapPoint* pWrapPoint = DrawingPropertyWrap.Points[i];
                            if(pWrapPoint->bX && pWrapPoint->bY)
                            {
								sXml += L"<wp:lineTo x=\"" + std::to_wstring(pWrapPoint->X) + L"\" y=\"" + std::to_wstring(pWrapPoint->Y) + L"\"/>";
                            }
                        }
                        sXml += L"</wp:wrapPolygon>";
                        sXml += L"</wp:" + sTagName + L">";
                    }
                    else
                    {
                        //для wrapThrough и wrapTight wrapPolygon обязательное поле, если его нет - меняем тип.
                        if( c_oSerImageType2::WrapSquare	== DrawingPropertyWrap.WrappingType		||
                                c_oSerImageType2::WrapThrough	== DrawingPropertyWrap.WrappingType		||
                                c_oSerImageType2::WrapTight		== DrawingPropertyWrap.WrappingType)
                        {
                            sXml += L"<wp:wrapSquare wrapText=\"bothSides\"/>";
                        }
                        else
                            sXml += L"<wp:" + sTagName + L"/>";
                    }
                }
                else
                    sXml += L"<wp:wrapNone/>";

                if(!sDocPr.empty())
				{
                    sXml += sDocPr;
				}
                else
                {
                    sXml += L"<wp:docPr id=\"" + std::to_wstring(m_nDocPr) + L"\" name=\"\"/>";
                }
                if(!sGraphicFramePr.empty())
                {
                    sXml += sGraphicFramePr;
                }
                else
                {
                    sXml += L"<wp:cNvGraphicFramePr/>";
                }
                if (bGraphicFrameContent)
                {
                    sXml += sGraphicFrameContent;
                }

                if(!sSizeRelH.empty())
                {
                    sXml += sSizeRelH;
                }
                if(!sSizeRelV.empty())
                {
                    sXml += sSizeRelV;
                }

                sXml += L"</wp:anchor>";

                if (bGraphicFrameContent)
                    sXml += L"</w:drawing>";
            }
        }
		return sXml;
	}
};
class CWiterTblPr
{
public:
    std::wstring Jc;
    std::wstring TableInd;
    std::wstring TableW;
    std::wstring TableCellMar;
    std::wstring TableBorders;
    std::wstring Shd;
    std::wstring tblpPr;
    std::wstring Style;
    std::wstring RowBandSize;
    std::wstring ColBandSize;
    std::wstring Look;
    std::wstring Layout;
    std::wstring tblPrChange;
    std::wstring TableCellSpacing;
	std::wstring Caption;
	std::wstring Description;
	std::wstring Overlap;
	bool IsEmpty()
	{
        return Jc.empty() && TableInd.empty() && TableW.empty() && TableCellMar.empty() && TableBorders.empty() && Shd.empty() && tblpPr.empty()&& Style.empty() && Look.empty() && tblPrChange.empty() && TableCellSpacing.empty() && RowBandSize.empty() && ColBandSize.empty();
	}
	std::wstring Write()
	{
        std::wstring sRes;
        sRes += L"<w:tblPr>";
        if(false == Style.empty())
            sRes += (Style);
        if(false == tblpPr.empty())
            sRes += (tblpPr);
        if(!RowBandSize.empty())
            sRes += (RowBandSize);
        if(!ColBandSize.empty())
            sRes += (ColBandSize);
		if (!Overlap.empty())
			sRes += (Overlap);
		if(false == TableW.empty())
            sRes += (TableW);
        if(false == Jc.empty())
            sRes += (Jc);
        if(false == TableCellSpacing.empty())
            sRes += (TableCellSpacing);
        if(false == TableInd.empty())
            sRes += (TableInd);
        if(false == TableBorders.empty())
            sRes += (TableBorders);
        if(false == Shd.empty())
            sRes += (Shd);
		if(false == Layout.empty())
			sRes += (Layout);
        if(false == TableCellMar.empty())
            sRes += (TableCellMar);
        if(false == Look.empty())
            sRes += (Look);
		if(!Caption.empty())
		{
			sRes += L"<w:tblCaption w:val=\"";
			sRes += XmlUtils::EncodeXmlString(Caption);
			sRes += L"\"/>";
		}
		if(!Description.empty())
		{
			sRes += L"<w:tblDescription w:val=\"";
			sRes += XmlUtils::EncodeXmlString(Description);
			sRes += L"\"/>";
		}
        if(!tblPrChange.empty())
			sRes += (tblPrChange);
        sRes += L"</w:tblPr>";
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
	void Write(NSStringUtils::CStringBuilder& oStringWriter)
	{
        oStringWriter.WriteString(L"<w:framePr");
		if(bDropCap)
		{
            std::wstring sDropCap(L"none");
			switch(DropCap)
			{
			case 0: sDropCap = _T("none");break;
			case 1: sDropCap = _T("drop");break;
			case 2: sDropCap = _T("margin");break;
			}
            std::wstring sXml = L" w:dropCap=\"" + sDropCap + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bLines)
		{
            oStringWriter.WriteString(L" w:lines=\"" + std::to_wstring(Lines) + L"\"");
		}
		if(bW)
		{
            std::wstring sXml = L" w:w=\"" + std::to_wstring(W) + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bH)
		{
            std::wstring sXml = L" w:h=\"" + std::to_wstring(H) + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bVSpace)
		{
            std::wstring sXml = L" w:vSpace=\"" + std::to_wstring(VSpace) + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bHSpace)
		{
            std::wstring sXml = L" w:hSpace=\"" + std::to_wstring(HSpace) + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bWrap)
		{
            std::wstring sWrap(L"none");
			switch(Wrap)
			{
			case 0: sWrap = _T("around");break;
			case 1: sWrap = _T("auto");break;
			case 2: sWrap = _T("none");break;
			case 3: sWrap = _T("notBeside");break;
			case 4: sWrap = _T("through");break;
			case 5: sWrap = _T("tight");break;
			}
            std::wstring sXml = L" w:wrap=\"" + sWrap + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bVAnchor)
		{
            std::wstring sVAnchor(L"margin");
			switch(VAnchor)
			{
                case 0: sVAnchor = _T("margin");break;
                case 1: sVAnchor = _T("page");break;
                case 2: sVAnchor = _T("text");break;
			}
            std::wstring sXml = L" w:vAnchor=\"" + sVAnchor + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bHAnchor)
		{
            std::wstring sHAnchor(L"margin");
			switch(HAnchor)
			{
                case 0: sHAnchor = _T("margin");break;
                case 1: sHAnchor = _T("page");  break;
                case 2: sHAnchor = _T("text");  break;
			}
            oStringWriter.WriteString(L" w:hAnchor=\"" + sHAnchor + L"\"");
		}
		if(bX)
		{
            oStringWriter.WriteString(L" w:x=\"" + std::to_wstring(X) + L"\"");
		}
		if(bXAlign)
		{
            std::wstring sXAlign(L"left");
			switch(XAlign)
			{
            case 0: sXAlign = _T("center"); break;
            case 1: sXAlign = _T("inside"); break;
            case 2: sXAlign = _T("left");   break;
            case 3: sXAlign = _T("outside");break;
            case 4: sXAlign = _T("right");  break;
			}
            oStringWriter.WriteString(L" w:xAlign=\"" + sXAlign + L"\"");
		}
		if(bY)
		{
            oStringWriter.WriteString(L" w:y=\"" + std::to_wstring(Y) + L"\"");
		}
		if(bYAlign)
		{
            std::wstring sYAlign(L"inline");
			switch(YAlign)
			{
                case 0: sYAlign = _T("bottom");break;
                case 1: sYAlign = _T("center");break;
                case 2: sYAlign = _T("inline");break;
                case 3: sYAlign = _T("inside");break;
                case 4: sYAlign = _T("outside");break;
                case 5: sYAlign = _T("top");break;
			}
            std::wstring sXml = L" w:yAlign=\"" + sYAlign + L"\"";
			oStringWriter.WriteString(sXml);
		}
		if(bHRule)
		{
            std::wstring sHRule(L"");
			switch(HRule)
			{
                case 0: sHRule = _T("atLeast"); break;
                case 1: sHRule = _T("auto");    break;
                case 2: sHRule = _T("exact");   break;
			}
            std::wstring sXml = L" w:hRule=\"" + sHRule + L"\"";
			oStringWriter.WriteString(sXml);
		}
        oStringWriter.WriteString(L"/>");
	}
};
class CHyperlink{
public:
    std::wstring    rId;
    std::wstring    sLink;
    std::wstring    sAnchor;
    std::wstring    sTooltip;
    bool            History;
    std::wstring    sDocLocation;
    std::wstring    sTgtFrame;

	NSStringUtils::CStringBuilder writer;

	bool bHistory;
public:
	CHyperlink()
	{
		bHistory = false;
	}
	void Write(NSStringUtils::CStringBuilder& wr)
	{
		wr.WriteString(L"<w:hyperlink");
		if(!rId.empty())
		{
			wr.WriteString(L" r:id=\"");
			wr.WriteEncodeXmlString(rId);
			wr.WriteString(L"\"");
		}
		if(!sTooltip.empty())
		{
			wr.WriteString(L" w:tooltip=\"");
			wr.WriteEncodeXmlString(sTooltip);
			wr.WriteString(L"\"");
		}
		if(!sAnchor.empty())
		{
			wr.WriteString(L" w:anchor=\"");
			wr.WriteEncodeXmlString(sAnchor);
			wr.WriteString(L"\"");
		}
		if (bHistory)
		{
			if (History)
			{
				wr.WriteString(L" w:history=\"1\"");
			}
			else
			{
				wr.WriteString(L" w:history=\"0\"");
			}
		}
		if (!sDocLocation.empty())
		{
			wr.WriteString(L" w:docLocation=\"");
			wr.WriteEncodeXmlString(sDocLocation);
			wr.WriteString(L"\"");
		}
		if (!sTgtFrame.empty())
		{
			wr.WriteString(L" w:tgtFrame=\"");
			wr.WriteEncodeXmlString(sTgtFrame);
			wr.WriteString(L"\"");
		}
		wr.WriteString(L">");
		wr.Write(writer);
		wr.WriteString(L"</w:hyperlink>");
	}
};
class CFldSimple{
public:
    std::wstring sInstr;
	NSStringUtils::CStringBuilder writer;
public:
	CFldSimple()
	{
	}
	void Write(NSStringUtils::CStringBuilder& wr)
	{
        if(false == sInstr.empty())
		{
            std::wstring sCorrect_Instr = XmlUtils::EncodeXmlString(sInstr);
            std::wstring sStart(L"<w:fldSimple w:instr=\"");
            sStart += sCorrect_Instr;
            sStart += L"\">";
			wr.WriteString(sStart);
			wr.Write(writer);
            wr.WriteString(L"</w:fldSimple>");
		}
	}
};

class TrackRevision
{
public:
    std::wstring Author;
    std::wstring Date;
	long* Id;
    std::wstring UserId;
	long* vMerge;
	long* vMergeOrigin;

	OOX::Logic::CRunProperty* RPr;

	NSStringUtils::CStringBuilder* PPr;
	SectPr* sectPr;
	CWiterTblPr* tblPr;
	NSStringUtils::CStringBuilder* tblGridChange;
	NSStringUtils::CStringBuilder* trPr;
	NSStringUtils::CStringBuilder* tcPr;
	NSStringUtils::CStringBuilder* content;
	NSStringUtils::CStringBuilder* contentRun;
	TrackRevision();
	~TrackRevision();
	bool IsNoEmpty()
	{
        return !Author.empty() || !Date.empty() || NULL != Id;
	}
    std::wstring ToString(std::wstring sName)
	{
		NSStringUtils::CStringBuilder writer;
		Write(&writer, sName);
		return writer.GetData();
	}
	void Write(NSStringUtils::CStringBuilder*  pCStringWriter, std::wstring sName);
};
}
