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
#pragma once

#include "PropertiesMapping.h"
#include "ConversionContext.h"
#include "NumberingMapping.h"
#include "HeaderMapping.h"
#include "FooterMapping.h"
#include "IMapping.h"
#include "../Common/Base/XmlTools.h"

namespace DocFileFormat
{
	static const wchar_t SectionTypeMap[5][11] =
	{
        L"continuous",
        L"nextColumn",
        L"nextPage",
        L"evenPage",
        L"oddPage"
	};

	static const wchar_t PageOrientationMap[3][10] =
	{
        L"0",
        L"portrait",
        L"landscape"
	};

	static const wchar_t DocGridTypeMap[4][14] =
	{
        L"Default",
        L"linesAndChars",
        L"lines",
        L"snapToChars"
	};

	static const wchar_t FootnoteRestartCodeMap[3][11] =
	{
        L"continuous",
        L"eachSect",
        L"eachPage"
	};

	static const wchar_t EndnoteRestartCodeMap[3][11] =
	{
        L"continuous",
        L"eachSect",
        L"eachPage"
	};

	static const wchar_t PageNumberFormatCodeMap[42][29] =
	{
        L"Decimal",
        L"upperRoman",
        L"lowerRoman",
        L"upperLetter",
        L"lowerLetter",
        L"ordinal",
        L"cardinalText",
        L"ordinalText",
        L"hex",
        L"chicago",
        L"ideographDigital",
        L"japaneseCounting",
        L"Aiueo",
        L"Iroha",
        L"decimalFullWidth",
        L"decimalHalfWidth",
        L"japaneseLegal",
        L"japaneseDigitalTenThousand",
        L"decimalEnclosedCircle",
        L"decimalFullWidth2",
        L"aiueoFullWidth",
        L"irohaFullWidth",
        L"decimalZero",
        L"bullet",
        L"ganada",
        L"chosung",
        L"decimalEnclosedFullstop",
        L"decimalEnclosedParen",
        L"decimalEnclosedCircleChinese",
        L"ideographEnclosedCircle",
        L"ideographTraditional",
        L"ideographZodiac",
        L"ideographZodiacTraditional",
        L"taiwaneseCounting",
        L"ideographLegalTraditional",
        L"taiwaneseCountingThousand",
        L"taiwaneseDigital",
        L"chineseCounting",
        L"chineseLegalSimplified",
        L"chineseCountingThousand",
        L"Decimal2",
        L"koreanDigital",
	};

	static const wchar_t TextVerticalAlignment[4][7] = 
	{
        L"top",
        L"center",
        L"both",
        L"bottom"
	};

	enum SLncOperand
	{
		lncPerPage	=	0x00,							//  Line numbers restart every page. 
		lncRestart	=	0x01,							//  Line numbers restart at the beginning of the section. 
		lncContinue	=	0x02							//  Line numbers continue from the preceding section, or start at 1 if this is the first section of the document. 
	};

	static const std::wstring LineNumberRestart []	=		//	ST_LineNumberRestart
	{
        L"newPage",
        L"newSection",
        L"continuous"
	};
}

namespace DocFileFormat
{
	class SectionPropertiesMapping: public PropertiesMapping, public IMapping
	{
	public:
		// Creates a new SectionPropertiesMapping which writes the properties to the given writer
		SectionPropertiesMapping (XMLTools::CStringXmlWriter* writer, ConversionContext* ctx, int nSelectProperties);
		// Creates a new SectionPropertiesMapping which appends the properties to a given node.
        SectionPropertiesMapping (XMLTools::XMLElement* sectPr, ConversionContext* ctx, int nSelectProperties);
		virtual ~SectionPropertiesMapping();

		// Converts the given SectionPropertyExceptions
		virtual void Apply (IVisitable* visited);

		const std::wstring	& get_section_type();
	private:
		
        void AppendRef (XMLTools::XMLElement* pBaseNode, const std::wstring& element, const std::wstring& refType, const std::wstring& refId);

		bool WriteSectionStory (CharacterRange* pRange, const std::wstring& StoryType, const std::wstring& Story);

        XMLTools::XMLElement*           m_pXmlNode;
		bool							m_bDeleteNode;

		int								m_nColumns;
		short*							m_arrWidth;
		short*							m_arrSpace;

		int								m_nSelectProperties;
		
		bool isOwnSectPr;
		//int _sectNr;
		ConversionContext* _ctx;
		std::wstring _type;

		short _pgWidth, _marLeft, _marRight;
	};
}
