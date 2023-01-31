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
#include "Structures.h"
#include "../Enums/enums.h"
#include "../../../OOXML/Base/Nullable.h"
#include "Interactive.h"
#include <set>

namespace PPT
{
class CFontProperty
{
public:
    std::wstring	Name;
    int             PitchFamily;
    int             Charset;

    CFontProperty();

    CFontProperty(const CFontProperty& oSrc);
    CFontProperty& operator=(const CFontProperty& oSrc);

    std::wstring getXmlArgsStr()const;

    static bool IsValidCharset(int value);

    static bool IsValidPitchFamily(int value);

};

class CBulletAutoNum
{
public:
    NSCommon::nullable<USHORT>  startAt;
    NSCommon::nullable_string   type;

    bool isDefault()const;
};

class CBulletBlip
{
public:
    std::wstring tmpImagePath;
    SHORT bulletBlipRef;

    bool hasRef()const;
};

class CFontProperties
{
public:
    CFontProperties ();
    CFontProperties(const CFontProperties& oSrc);
    CFontProperties& operator=(const CFontProperties& oSrc);
    NSCommon::nullable_base<CFontProperty>	font;
    NSCommon::nullable_base<CFontProperty>	ea;
    NSCommon::nullable_base<CFontProperty>	sym;
    NSCommon::nullable_base<CFontProperty>	ansi;
};

class CTextCFRun
{
public:

    NSCommon::nullable_base<bool>	FontBold;
    NSCommon::nullable_base<bool>	FontItalic;
    NSCommon::nullable_base<bool>	FontUnderline;
    NSCommon::nullable_base<bool>	FontStrikeout;
    NSCommon::nullable_bool     	FontShadow;

    NSCommon::nullable_base<WORD>	fontRef;			// fontRef
    NSCommon::nullable_base<WORD>	eaFontRef;			// eaFontRef
    NSCommon::nullable_base<WORD>	ansiFontRef;		// ansiFontRef
    NSCommon::nullable_base<WORD>	symbolFontRef;		// symFontRef
    NSCommon::nullable_base<WORD>   pp9rt;

    NSCommon::nullable_base<double>	BaseLineOffset;		// baseline offset

    NSCommon::nullable_base<ODRAW::CColor>	Color;				// text color
    NSCommon::nullable_base<WORD>	Size;				// font size

    NSCommon::nullable_base<WORD>	Cap;				// 0 - none, 1 - TEXT, 2 - text
    NSCommon::nullable_base<WORD>	Language;

    CFontProperties			font;

    CTextCFRun();
    CTextCFRun(const CTextCFRun& oSrc);
    CTextCFRun& operator=(const CTextCFRun& oSrc);
    ~CTextCFRun();

    void ApplyBefore(const CTextCFRun& oSrc);
    void ApplyAfter(const CTextCFRun& oSrc);

};

class CTextPFRun
{
public:

    NSCommon::nullable_bool                 hasBullet;

    NSCommon::nullable_base<WORD>			bulletFontRef;
    NSCommon::nullable_base<WORD>			bulletSize;
    NSCommon::nullable_base<WCHAR>			bulletChar;
    NSCommon::nullable_base<ODRAW::CColor>			bulletColor;
    NSCommon::nullable_base<CFontProperty>  bulletFontProperties;
    NSCommon::nullable_base<CBulletAutoNum> bulletAutoNum;
    NSCommon::nullable_base<CBulletBlip>    bulletBlip;

    NSCommon::nullable_base<WORD>			textAlignment;
    NSCommon::nullable_base<LONG>			lineSpacing;
    NSCommon::nullable_base<LONG>			spaceBefore;
    NSCommon::nullable_base<LONG>			spaceAfter;
    NSCommon::nullable_base<LONG>			leftMargin;
    NSCommon::nullable_base<LONG>			indent;
    NSCommon::nullable_base<LONG>			defaultTabSize;

    std::vector<std::pair<int, int>>		tabStops;

    bool									bIsOneLine;

    NSCommon::nullable_base<WORD>			fontAlign;
    NSCommon::nullable_base<WORD>			textDirection;

    NSCommon::nullable_base<WORD>			wrapFlags;

    CTextPFRun();

    CTextPFRun(const CTextPFRun& oSrc);

    CTextPFRun& operator =(const CTextPFRun& oSrc);

    void ApplyBefore(const CTextPFRun& oSrc);

    void ApplyAfter(const CTextPFRun& oSrc);

    std::wstring ToString(LONG lCount);

};

class CTextRuler
{
public:
    NSCommon::nullable_base<LONG>	DefaultTabSize;
    NSCommon::nullable_base<SHORT>	CLevels;

    NSCommon::nullable_base<SHORT>	TabStops;
    std::vector<std::pair<int,int>>	tabsStops;

    NSCommon::nullable_base<LONG>	LeftMargin1;
    NSCommon::nullable_base<LONG>	LeftMargin2;
    NSCommon::nullable_base<LONG>	LeftMargin3;
    NSCommon::nullable_base<LONG>	LeftMargin4;
    NSCommon::nullable_base<LONG>	LeftMargin5;

    NSCommon::nullable_base<LONG>	Indent1;
    NSCommon::nullable_base<LONG>	Indent2;
    NSCommon::nullable_base<LONG>	Indent3;
    NSCommon::nullable_base<LONG>	Indent4;
    NSCommon::nullable_base<LONG>	Indent5;

    CTextRuler();

    CTextRuler(const CTextRuler& oSrc);

    CTextRuler& operator =(const CTextRuler& oSrc);
    ~CTextRuler();

    void ApplyBefore(const CTextRuler& oSrc);

    std::wstring ToString();
};

class CTextSIRun
{
public:
    bool bIsExt;
    _UINT32 lCount;

    bool bSpell;
    bool bLang;
    bool bAltLang;

    bool bPp10ext;
    bool bBidi;
    bool bSmartTag;

    WORD Spell;
    WORD Lang;
    WORD AltLang;

    WORD Bidi;
    BYTE pp10runid;

    bool bGramma;

    std::vector<_UINT32> arSmartTags;

public:

    CTextSIRun();

    CTextSIRun(const CTextSIRun& oSrc);

    CTextSIRun& operator =(const CTextSIRun& oSrc);

    void ApplyBefore(const CTextSIRun& oSrc);

    std::wstring ToString();
};


class CSpan
{
public:
    CTextCFRun			m_oRun;
    std::wstring		m_strText;
    bool				m_bField;
    bool				m_bBreak;

    std::vector<CInteractiveInfo>   m_arrInteractive;

    CSpan();
    CSpan(const CSpan& oSrc);
    CSpan& operator=(const CSpan& oSrc);
    ~CSpan();
};

class CTextStyleLevel
{
public:
    CTextPFRun m_oPFRun;
    CTextCFRun m_oCFRun;

public:
    CTextStyleLevel() ;
    CTextStyleLevel(const CTextStyleLevel& oSrc);
    CTextStyleLevel& operator=(const CTextStyleLevel& oSrc);

    void ApplyAfter(const CTextStyleLevel& oSrc);
    void ApplyBefore(const CTextStyleLevel& oSrc);
};

class CTextStyles
{
public:
    NSCommon::nullable_base<CTextStyleLevel> m_pLevels[10];

    CTextStyles();
    CTextStyles(const CTextStyles& oSrc);
    CTextStyles& operator=(const CTextStyles& oSrc);
    void SetStyles(CTextStyles* pStyles);
    void SetLanguage(nullable<WORD> & language);
    void ApplyAfter(const CTextStyles& oSrc);
    void ApplyBefore(const CTextStyles& oSrc);
};
///////////////////////////////////////////////////////  TODO  /////////////////////////////////////

//        class CTextStyle9Level
//	{
//	public:
//		CTextPFRun m_oPF9Run;
//		CTextCFRun m_oCF9Run;

//	public:
//		CTextStyle9Level()
//		{
//		}
//		CTextStyle9Level(const CTextStyle9Level& oSrc)
//		{
//			*this = oSrc;
//		}
//		CTextStyle9Level& operator=(const CTextStyle9Level& oSrc)
//		{
//			m_oPF9Run	= oSrc.m_oPF9Run;
//			m_oCF9Run	= oSrc.m_oCF9Run;

//			return *this;
//		}

//		void ApplyAfter(const CTextStyleLevel& oSrc)
//		{
//			m_oPFRun.ApplyAfter(oSrc.m_oPFRun);
//			m_oCFRun.ApplyAfter(oSrc.m_oCFRun);
//		}
//		void ApplyBefore(const CTextStyleLevel& oSrc)
//		{
//			m_oPFRun.ApplyBefore(oSrc.m_oPFRun);
//			m_oCFRun.ApplyBefore(oSrc.m_oCFRun);
//		}
//	};

//	class CTextStyles9
//	{
//	public:
//		NSCommon::nullable_base<CTextStyleLevel> m_pLevels[10];

//		CTextStyles9()
//		{
//			for (int i = 0; i < 10; ++i)
//			{
//				m_pLevels[i] = NULL;
//			}
//		}
//		CTextStyles9(const CTextStyles& oSrc)
//		{
//			*this = oSrc;
//		}
//		CTextStyles9& operator=(const CTextStyles& oSrc)
//		{
//			for (int i = 0; i < 10; ++i)
//			{
//				m_pLevels[i] = oSrc.m_pLevels[i];
//			}
//			return *this;
//		}
//		void SetStyles(CTextStyles* pStyles)
//		{
//			for (int i = 0; i < 10; ++i)
//			{
//				m_pLevels[i] = pStyles->m_pLevels[i];
//			}
//		}
//		void SetLanguage(nullable<WORD> & language)
//		{
//			if (!language.is_init()) return;

//			for (int i = 0; i < 10; ++i)
//			{
//				if (m_pLevels[i].is_init())
//				{
//					m_pLevels[i]->m_oCFRun.Language = language;
//				}
//			}
//		}
//		void ApplyAfter(const CTextStyles& oSrc)
//		{
//			for (int i = 0; i < 10; ++i)
//			{
//				if (!m_pLevels[i].is_init())
//				{
//					m_pLevels[i] = oSrc.m_pLevels[i];
//				}
//				else if (oSrc.m_pLevels[i].is_init())
//				{
//					m_pLevels[i]->ApplyAfter(oSrc.m_pLevels[i].get());
//				}

//			}
//		}
//		void ApplyBefore(const CTextStyles& oSrc)
//		{
//			for (int i = 0; i < 10; ++i)
//			{
//				if (!m_pLevels[i].is_init())
//				{
//					m_pLevels[i] = oSrc.m_pLevels[i];
//				}
//				else if (oSrc.m_pLevels[i].is_init())
//				{
//					m_pLevels[i]->ApplyBefore(oSrc.m_pLevels[i].get());
//				}

//			}
//		}
//	};

///////////////////////////////////////////////////////  TODO  /////////////////////////////////////
class CParagraph
{
public:
    int m_lTextType;
    int m_lTextLevel;
    int m_lStyleThemeIndex;

    CTextPFRun		m_oPFRun;
    std::vector<CSpan>	m_arSpans;


public:
    CParagraph() ;
    CParagraph(const CParagraph& oSrc);
    CParagraph& operator=(const CParagraph& oSrc);
    ~CParagraph();
    void CheckErrors();
    bool IsEmpty();
};
}
