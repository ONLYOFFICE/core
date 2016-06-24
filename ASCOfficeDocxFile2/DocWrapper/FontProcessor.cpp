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
//#include "stdafx.h"
#include "FontProcessor.h"

#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../../Common/ASCUtils.h"

namespace DocWrapper {
	TCHAR* gc_sNoNameFont = _T("NoNameFont");
	TCHAR* gc_sDefaultFontName = _T("Arial");

	FontProcessor::FontProcessor() : m_pFontManager(NULL)
	{
	}
	FontProcessor::~FontProcessor()
	{
	}
	
	void FontProcessor::setFontManager(CFontManager* pFontManager)
	{
		m_pFontManager = pFontManager;
	}
	CFontManager* FontProcessor::getFontManager()
	{
		return m_pFontManager;
	}
	void FontProcessor::setFontTable(OOX::CFontTable* fontTable)
	{
		//подбор перенесен в js
		return;
		for (int i = 0; i < fontTable->m_arrFonts.size(); ++i)
			addToFontMap(*fontTable->m_arrFonts[i]);
	}
	
	CString FontProcessor::getFont(const CString& name)
	{
		//подбор перенесен в js
		return name;
		CString fontName = gc_sDefaultFontName;
		if (fontMap.find(name) != fontMap.end())
			fontName = fontMap[name];
		else
		{
			OOX::CFont font;
			font.m_sName = name;
			addToFontMap(font);
			if (fontMap.find(name) != fontMap.end())
				fontName = fontMap[name];
		}
		return fontName;
	}
	
	CString FontProcessor::getFont(const NSCommon::nullable<OOX::Spreadsheet::CFontScheme>& oScheme, const NSCommon::nullable<ComplexTypes::Spreadsheet::CString_>& oRFont, const NSCommon::nullable<OOX::Spreadsheet::CCharset>& oCharset, const NSCommon::nullable<OOX::Spreadsheet::CFontFamily >& oFamily, OOX::CTheme* pTheme)
	{
		CFontSelectFormat oFontSelectFormat;
		CString sFontName;
		if(NULL != pTheme && oScheme.IsInit() && oScheme->m_oFontScheme.IsInit())
		{
			//берем шрифт из темы
			const SimpleTypes::Spreadsheet::EFontScheme eFontScheme = oScheme->m_oFontScheme->GetValue();
			if(SimpleTypes::Spreadsheet::fontschemeMajor == eFontScheme)
				sFontName = pTheme->GetMajorFont();
			else if(SimpleTypes::Spreadsheet::fontschemeMinor == eFontScheme)
				sFontName = pTheme->GetMinorFont();
		}
		if(sFontName.IsEmpty() && oRFont.IsInit() && oRFont->m_sVal.IsInit())
			sFontName = oRFont->ToString2();
		if(sFontName.IsEmpty())
			sFontName = CString(gc_sNoNameFont);
		//подбор перенесен в js
		return  sFontName;
		oFontSelectFormat.wsName = new std::wstring;
		*oFontSelectFormat.wsName = string2std_string(sFontName);
		if(oCharset.IsInit() && oCharset->m_oCharset.IsInit())
		{
			SimpleTypes::Spreadsheet::EFontCharset eCharset = oCharset->m_oCharset->GetValue();
			//на серверве на берем в расчет fontcharsetANSI и fontcharsetDefault, потому что он зависит от локали, а dll работает на сервере
			if(SimpleTypes::fontcharsetANSI !=  eCharset && SimpleTypes::fontcharsetDefault != eCharset)
			{
				oFontSelectFormat.unCharset = new BYTE;
				*oFontSelectFormat.unCharset = (BYTE)eCharset;
			}
		}
		if(oFamily.IsInit() && oFamily->m_oFontFamily.IsInit())
		{
			oFontSelectFormat.wsFamilyClass = new std::wstring;
			*oFontSelectFormat.wsFamilyClass = oFamily->m_oFontFamily->ToStringWord();
		}

		CString sRes;
		CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
		if(NULL != pFontInfo)
			sRes = std_string2string(pFontInfo->m_wsFontName);
		else
			sRes = gc_sDefaultFontName;
		fontMap[sFontName] = sRes;
		return sRes;
	}
	void FontProcessor::addToFontMap(OOX::CFont& font)
	{
		CFontSelectFormat oFontSelectFormat;
		CString sFontName;
		if(font.m_sName.IsEmpty())
			sFontName = CString(gc_sNoNameFont);
		else
			sFontName = font.m_sName;
		oFontSelectFormat.wsName = new std::wstring;
		*oFontSelectFormat.wsName = string2std_string(sFontName);
		if (font.m_oAltName.IsInit() && font.m_oAltName->GetLength() > 0)
		{
			oFontSelectFormat.wsAltName = new std::wstring;
			*oFontSelectFormat.wsAltName = string2std_string(*font.m_oAltName);
		}
		if(font.m_oCharset.IsInit())
		{
			SimpleTypes::EFontCharset eCharset = font.m_oCharset->GetValue();
			//на серверве на берем в расчет fontcharsetANSI и fontcharsetDefault, потому что он зависит от локали, а dll работает на сервере
			if(SimpleTypes::fontcharsetANSI != eCharset && SimpleTypes::fontcharsetDefault != eCharset)
			{
				oFontSelectFormat.unCharset = new BYTE;
				*oFontSelectFormat.unCharset = (BYTE)eCharset;
			}
		}
		if(font.m_oFamily.IsInit())
		{
			oFontSelectFormat.wsFamilyClass = new std::wstring;
			*oFontSelectFormat.wsFamilyClass = font.m_oFamily->ToString();
		}
		if(font.m_oPanose.IsInit())
		{
			bool bIsPanose = false;
			oFontSelectFormat.pPanose = new BYTE[10];
			for(int i = 0; i < 10; ++i)
			{
				unsigned char cCurVal = font.m_oPanose->Get_Number(i);
				oFontSelectFormat.pPanose[i] = cCurVal;
				// Если хоть одно значение не 0, то используем Panose
				if ( 0 != cCurVal  )
					bIsPanose = true;
			}
			if(!bIsPanose)
				RELEASEARRAYOBJECTS(oFontSelectFormat.pPanose);
		}
		if(font.m_oPitch.IsInit())
		{
			oFontSelectFormat.bFixedWidth = new INT;
			if (font.m_oPitch->GetValue() == SimpleTypes::pitchFixed)
				*oFontSelectFormat.bFixedWidth = 1;
			else
				*oFontSelectFormat.bFixedWidth = 0;
		}
		ULONG ulRange1 = 0;
		ULONG ulRange2 = 0;
		ULONG ulRange3 = 0;
		ULONG ulRange4 = 0;
		ULONG ulCodeRange1 = 0;
		ULONG ulCodeRange2 = 0;
		if (font.m_oUsb0.IsInit())
			ulRange1 = font.m_oUsb0->GetValue();
		if (font.m_oUsb1.IsInit())
			ulRange2 = font.m_oUsb1->GetValue();
		if (font.m_oUsb2.IsInit())
			ulRange3 = font.m_oUsb2->GetValue();
		if (font.m_oUsb3.IsInit())
			ulRange4 = font.m_oUsb3->GetValue();
		if (font.m_oCsb0.IsInit())
			ulCodeRange1 = font.m_oCsb0->GetValue();
		if (font.m_oCsb1.IsInit())
			ulCodeRange2 = font.m_oCsb1->GetValue();
		if ( !(0 == ulRange1 && 0 == ulRange2 && 0 == ulRange3 && 0 == ulRange4 && 0 == ulCodeRange1 && 0 == ulCodeRange2) )
		{
			oFontSelectFormat.ulRange1 = new ULONG;
			*oFontSelectFormat.ulRange1 = ulRange1;
			oFontSelectFormat.ulRange2 = new ULONG;
			*oFontSelectFormat.ulRange2 = ulRange2;
			oFontSelectFormat.ulRange3 = new ULONG;
			*oFontSelectFormat.ulRange3 = ulRange3;
			oFontSelectFormat.ulRange4 = new ULONG;
			*oFontSelectFormat.ulRange4 = ulRange4;
			oFontSelectFormat.ulCodeRange1 = new ULONG;
			*oFontSelectFormat.ulCodeRange1 = ulCodeRange1;
			oFontSelectFormat.ulCodeRange2 = new ULONG;
			*oFontSelectFormat.ulCodeRange2 = ulCodeRange2;
		}

		CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
		if(NULL != pFontInfo)
			fontMap[sFontName] = std_string2string(pFontInfo->m_wsFontName);
		else
			fontMap[sFontName] = gc_sDefaultFontName;
	}
}
