/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "FontProcessor.h"

#include "../../../XlsxFormat/Xlsx.h"
#include "../../../DocxFormat/Docx.h"

#include "../../../DocxFormat/FontTable.h"
#include "../../../XlsxFormat/Styles/TableStyles.h"

#include "../../../PPTXFormat/Theme.h"
#include "../../../../Common/ASCUtils.h"

#include "../../../XlsxFormat/ComplexTypes_Spreadsheet.h"

namespace DocWrapper {
    wchar_t* gc_sNoNameFont         = _T("NoNameFont");
    wchar_t* gc_sDefaultFontName    = _T("Arial");

	FontProcessor::FontProcessor() : m_pFontManager(NULL)
	{
	}
	FontProcessor::~FontProcessor()
	{
	}
	
    void FontProcessor::setFontManager(NSFonts::IFontManager* pFontManager)
	{
		m_pFontManager = pFontManager;
	}
    NSFonts::IFontManager* FontProcessor::getFontManager()
	{
		return m_pFontManager;
	}
	void FontProcessor::setFontTable(OOX::CFontTable* fontTable)
	{
		//font matching moved to js
		return;
		for (size_t i = 0; i < fontTable->m_arrFonts.size(); ++i)
			addToFontMap(*fontTable->m_arrFonts[i]);
	}
	
    std::wstring FontProcessor::getFont(const std::wstring& name)
	{
		//font matching moved to js
		return name;
        std::wstring fontName = gc_sDefaultFontName;
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
	
	std::wstring FontProcessor::getFont(const NSCommon::nullable<OOX::Spreadsheet::CFontScheme>& oScheme, const NSCommon::nullable<ComplexTypes::Spreadsheet::String>& oRFont, const NSCommon::nullable<OOX::Spreadsheet::CCharset>& oCharset, const NSCommon::nullable<OOX::Spreadsheet::CFontFamily >& oFamily, PPTX::Theme* pTheme)
	{
        NSFonts::CFontSelectFormat oFontSelectFormat;
        std::wstring sFontName;
		if(NULL != pTheme && oScheme.IsInit() && oScheme->m_oFontScheme.IsInit())
		{
	//get font from theme
			const SimpleTypes::Spreadsheet::EFontScheme eFontScheme = oScheme->m_oFontScheme->GetValue();
			if(SimpleTypes::Spreadsheet::fontschemeMajor == eFontScheme)
				sFontName = pTheme->themeElements.fontScheme.majorFont.latin.typeface;
			else if(SimpleTypes::Spreadsheet::fontschemeMinor == eFontScheme)
				sFontName = pTheme->themeElements.fontScheme.minorFont.latin.typeface;
		}
        if(sFontName.empty() && oRFont.IsInit() && oRFont->m_sVal.IsInit())
			sFontName = oRFont->ToString2();
        if(sFontName.empty())
            sFontName = std::wstring(gc_sNoNameFont);
	//font matching moved to js
		return  sFontName;
		
		oFontSelectFormat.wsName = new std::wstring(sFontName);

		if(oCharset.IsInit() && oCharset->m_oCharset.IsInit())
		{
			SimpleTypes::Spreadsheet::EFontCharset eCharset = oCharset->m_oCharset->GetValue();
			//on server we don't take fontcharsetANSI and fontcharsetDefault into account, because they depend on locale, and dll works on server
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

        std::wstring sRes;
        NSFonts::CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
		if(NULL != pFontInfo)
			sRes = pFontInfo->m_wsFontName;
		else
			sRes = gc_sDefaultFontName;
		fontMap[sFontName] = sRes;
		return sRes;
	}
	void FontProcessor::addToFontMap(OOX::CFont& font)
	{
        NSFonts::CFontSelectFormat oFontSelectFormat;
        std::wstring sFontName;

        if(font.m_sName.empty())
            sFontName = std::wstring(gc_sNoNameFont);
		else
			sFontName = font.m_sName;
		
		oFontSelectFormat.wsName = new std::wstring(sFontName);

        if (font.m_oAltName.IsInit() && font.m_oAltName->length() > 0)
		{
			oFontSelectFormat.wsAltName = new std::wstring(*font.m_oAltName);
		}
		if(font.m_oCharset.IsInit())
		{
			SimpleTypes::EFontCharset eCharset = font.m_oCharset->GetValue();
			//on server we don't take fontcharsetANSI and fontcharsetDefault into account, because they depend on locale, and dll works on server
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
				// If at least one value is not 0, then use Panose
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
        UINT ulRange1 = 0;
        UINT ulRange2 = 0;
        UINT ulRange3 = 0;
        UINT ulRange4 = 0;
        UINT ulCodeRange1 = 0;
        UINT ulCodeRange2 = 0;
		if (font.m_oUsb0.IsInit())
            ulRange1 = (UINT)font.m_oUsb0->GetValue();
		if (font.m_oUsb1.IsInit())
            ulRange2 = (UINT)font.m_oUsb1->GetValue();
		if (font.m_oUsb2.IsInit())
            ulRange3 = (UINT)font.m_oUsb2->GetValue();
		if (font.m_oUsb3.IsInit())
            ulRange4 = (UINT)font.m_oUsb3->GetValue();
		if (font.m_oCsb0.IsInit())
            ulCodeRange1 = (UINT)font.m_oCsb0->GetValue();
		if (font.m_oCsb1.IsInit())
            ulCodeRange2 = (UINT)font.m_oCsb1->GetValue();
		if ( !(0 == ulRange1 && 0 == ulRange2 && 0 == ulRange3 && 0 == ulRange4 && 0 == ulCodeRange1 && 0 == ulCodeRange2) )
		{
            oFontSelectFormat.ulRange1 = new UINT;
			*oFontSelectFormat.ulRange1 = ulRange1;
            oFontSelectFormat.ulRange2 = new UINT;
			*oFontSelectFormat.ulRange2 = ulRange2;
            oFontSelectFormat.ulRange3 = new UINT;
			*oFontSelectFormat.ulRange3 = ulRange3;
            oFontSelectFormat.ulRange4 = new UINT;
			*oFontSelectFormat.ulRange4 = ulRange4;
            oFontSelectFormat.ulCodeRange1 = new UINT;
			*oFontSelectFormat.ulCodeRange1 = ulCodeRange1;
            oFontSelectFormat.ulCodeRange2 = new UINT;
			*oFontSelectFormat.ulCodeRange2 = ulCodeRange2;
		}

        NSFonts::CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
		if(NULL != pFontInfo)
			fontMap[sFontName] = pFontInfo->m_wsFontName;
		else
			fontMap[sFontName] = gc_sDefaultFontName;
	}
}
