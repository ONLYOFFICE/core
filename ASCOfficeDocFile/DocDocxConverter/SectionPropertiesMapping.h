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

	static const wstring LineNumberRestart []	=		//	ST_LineNumberRestart
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

	private:
		
		void AppendRef (XMLTools::XMLElement<wchar_t>* pBaseNode, const wchar_t* element, const wchar_t* refType, const wchar_t* refId);

		bool WriteSectionStory (CharacterRange* pRange, const std::wstring& StoryType, const std::wstring& Story);

	private: 

		XMLTools::XMLElement<wchar_t>*	m_pXmlNode;
		bool							m_bDeleteNode;

		int								m_nColumns;
		short*							m_arrWidth;
		short*							m_arrSpace;

		int								m_nSelectProperties;
		
		bool isOwnSectPr;
		//int _sectNr;
		ConversionContext* _ctx;
		wstring _type;

		short _pgWidth, _marLeft, _marRight;
	};
}
