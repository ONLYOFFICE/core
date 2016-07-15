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
#pragma once

#include "PropertiesMapping.h"
#include "ConversionContext.h"
#include "NumberingMapping.h"
#include "HeaderMapping.h"
#include "FooterMapping.h"
#include "IMapping.h"
#include "../Common/XmlTools.h"

#define WSTD(V) std::wstring(_T(#V))

namespace DocFileFormat
{
	static const wchar_t SectionTypeMap[5][11] =
	{
		_T( "continuous" ),
		_T( "nextColumn" ),
		_T( "nextPage" ),
		_T( "evenPage" ),
		_T( "oddPage" )
	};

	static const wchar_t PageOrientationMap[3][10] =
	{
		_T( "0" ),
		_T( "portrait" ),
		_T( "landscape" )
	};

	static const wchar_t DocGridTypeMap[4][14] =
	{
		_T( "Default" ),
		_T( "linesAndChars" ),
		_T( "lines" ),
		_T( "snapToChars" )
	};

	static const wchar_t FootnoteRestartCodeMap[3][11] =
	{
		_T( "continuous" ),
		_T( "eachSect" ),
		_T( "eachPage" )
	};

	static const wchar_t EndnoteRestartCodeMap[3][11] =
	{
		_T( "continuous" ),
		_T( "eachSect" ),
		_T( "eachPage" )
	};

	static const wchar_t PageNumberFormatCodeMap[42][29] =
	{
		_T( "Decimal" ),
		_T( "upperRoman" ),
		_T( "lowerRoman" ),
		_T( "upperLetter" ),
		_T( "lowerLetter" ),
		_T( "ordinal" ),
		_T( "cardinalText" ),
		_T( "ordinalText" ),
		_T( "hex" ),
		_T( "chicago" ),
		_T( "ideographDigital" ),
		_T( "japaneseCounting" ),
		_T( "Aiueo" ),
		_T( "Iroha" ),
		_T( "decimalFullWidth" ),
		_T( "decimalHalfWidth" ),
		_T( "japaneseLegal" ),
		_T( "japaneseDigitalTenThousand" ),
		_T( "decimalEnclosedCircle" ),
		_T( "decimalFullWidth2" ),
		_T( "aiueoFullWidth" ),
		_T( "irohaFullWidth" ),
		_T( "decimalZero" ),
		_T( "bullet" ),
		_T( "ganada" ),
		_T( "chosung" ),
		_T( "decimalEnclosedFullstop" ),
		_T( "decimalEnclosedParen" ),
		_T( "decimalEnclosedCircleChinese" ),
		_T( "ideographEnclosedCircle" ),
		_T( "ideographTraditional" ),
		_T( "ideographZodiac" ),
		_T( "ideographZodiacTraditional" ),
		_T( "taiwaneseCounting" ),
		_T( "ideographLegalTraditional" ),
		_T( "taiwaneseCountingThousand" ),
		_T( "taiwaneseDigital" ),
		_T( "chineseCounting" ),
		_T( "chineseLegalSimplified" ),
		_T( "chineseCountingThousand" ),
		_T( "Decimal2" ),
		_T( "koreanDigital" ),
	};

	static const wchar_t TextVerticalAlignment[4][7] = 
	{
		_T( "top" ),
		_T( "center" ),
		_T( "both" ),
		_T( "bottom" )
	};

	enum SLncOperand
	{
		lncPerPage	=	0x00,							//  Line numbers restart every page. 
		lncRestart	=	0x01,							//  Line numbers restart at the beginning of the section. 
		lncContinue	=	0x02							//  Line numbers continue from the preceding section, or start at 1 if this is the first section of the document. 
	};

	static const std::wstring LineNumberRestart []	=		//	ST_LineNumberRestart
	{
		WSTD (newPage),
		WSTD (newSection),
		WSTD (continuous)
	};
}

namespace DocFileFormat
{
	class SectionPropertiesMapping: public PropertiesMapping, public IMapping
	{
	public:
		// Creates a new SectionPropertiesMapping which writes the properties to the given writer
		SectionPropertiesMapping (XmlUtils::CXmlWriter* writer, ConversionContext* ctx, int nSelectProperties);
		// Creates a new SectionPropertiesMapping which appends the properties to a given node.
		SectionPropertiesMapping (XMLTools::XMLElement<wchar_t>* sectPr, ConversionContext* ctx, int nSelectProperties);
		virtual ~SectionPropertiesMapping();

		// Converts the given SectionPropertyExceptions
		virtual void Apply (IVisitable* visited);

		const std::wstring	& get_section_type();
	private:
		
		void AppendRef (XMLTools::XMLElement<wchar_t>* pBaseNode, const wchar_t* element, const wchar_t* refType, const wchar_t* refId);

		bool WriteSectionStory (CharacterRange* pRange, const std::wstring& StoryType, const std::wstring& Story);

		XMLTools::XMLElement<wchar_t>*	m_pXmlNode;
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
