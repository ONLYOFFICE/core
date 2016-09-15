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


#include "MainDocumentMapping.h"

namespace DocFileFormat
{
	// Creates a new SectionPropertiesMapping which writes the properties to the given writer
	SectionPropertiesMapping::SectionPropertiesMapping (XMLTools::CStringXmlWriter* pWriter, ConversionContext* pContext, int nSelectProperties) : PropertiesMapping (pWriter)
	{		
		m_bDeleteNode		=	TRUE;
		m_pXmlNode			=	new XMLTools::XMLElement<wchar_t> (_T("w:sectPr"));

		m_nColumns			=	0;
		m_arrWidth			=	NULL;
		m_arrSpace			=	NULL;

		_pgWidth			=	0;
		_marLeft			=	0;
		_marRight			=	0;

		_ctx				=	pContext;
		m_nSelectProperties	=	nSelectProperties;
		_type				=	std::wstring (_T("nextPage"));
	}

	// Creates a new SectionPropertiesMapping which appends the properties to a given node.
	SectionPropertiesMapping::SectionPropertiesMapping (XMLTools::XMLElement<wchar_t>* pBaseNode, ConversionContext* pContext, int nSelectProperties) : PropertiesMapping(NULL)
	{
		m_bDeleteNode		=	FALSE;
		m_pXmlNode			=	pBaseNode;	

		m_nColumns			=	0;
		m_arrWidth			=	NULL;
		m_arrSpace			=	NULL;

		_pgWidth			=	0;
		_marLeft			=	0;
		_marRight			=	0;

		_ctx				=	pContext;
		m_nSelectProperties	=	nSelectProperties;
		_type				=	std::wstring (_T("nextPage"));
	}

	SectionPropertiesMapping::~SectionPropertiesMapping()
	{
		if (m_bDeleteNode)
			RELEASEOBJECT (m_pXmlNode);

		RELEASEARRAYOBJECTS (m_arrSpace);
		RELEASEARRAYOBJECTS (m_arrWidth);
	}
}

namespace DocFileFormat
{
	// Converts the given SectionPropertyExceptions
	void SectionPropertiesMapping::Apply (IVisitable* visited)
	{
		SectionPropertyExceptions* sepx	=	static_cast<SectionPropertyExceptions*>(visited);

		XMLTools::XMLElement<wchar_t> pgSz		(_T("w:pgSz"));
		XMLTools::XMLElement<wchar_t> pgMar		(_T("w:pgMar"));
		XMLTools::XMLElement<wchar_t> lnNumType	(_T("w:lnNumType"));
		XMLTools::XMLElement<wchar_t> cols		(_T("w:cols"));
		XMLTools::XMLElement<wchar_t> docGrid	(_T("w:docGrid"));
		XMLTools::XMLElement<wchar_t> pgBorders	(_T("w:pgBorders"));
		XMLTools::XMLElement<wchar_t> paperSrc	(_T("w:paperSrc"));
		XMLTools::XMLElement<wchar_t> footnotePr(_T("w:footnotePr"));
		XMLTools::XMLElement<wchar_t> endnotePr	(_T("w:endnotePr"));
		XMLTools::XMLElement<wchar_t> pgNumType	(_T("w:pgNumType"));

		HeaderAndFooterTable* pTable	=	_ctx->_doc->headerAndFooterTable;

		if (pTable)
		{
			unsigned char fHF = 255; //all headers & footers
			for (std::list<SinglePropertyModifier>::iterator iter = sepx->grpprl->begin(); iter != sepx->grpprl->end(); ++iter)
			{
				switch (iter->OpCode)
				{
					case sprmOldSGprfIhdt:
					case sprmSGprfIhdt:
					{
						fHF = FormatUtils::BytesToUChar( iter->Arguments, 0, iter->argumentsSize );
					}break;
					case sprmSPgbProp:
					{
						int val = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
						int pgbApplyTo		= FormatUtils::GetIntFromBits(val, 0 , 3);
						int pgbPageDepth	= FormatUtils::GetIntFromBits(val, 3 , 2);
						int pgbOffsetFrom	= FormatUtils::GetIntFromBits(val, 5 , 3);

						if (pgbOffsetFrom == 0x1) //else default from text
							appendValueAttribute( &pgBorders, L"w:offsetFrom", L"page" );
						
						if (pgbApplyTo == 0x0)
							appendValueAttribute( &pgBorders, L"w:display", L"allPages" );
						else if (pgbApplyTo == 0x1)
							appendValueAttribute( &pgBorders, L"w:display", L"firstPage" );
						else
							appendValueAttribute( &pgBorders, L"w:display", L"notFirstPage" );

					}break;
				}
			}
			// Header

			if (FormatUtils::GetBitFromInt(fHF, 0)) WriteSectionStory (pTable->GetEvenHeaders  (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"even"));
			if (FormatUtils::GetBitFromInt(fHF, 1))	WriteSectionStory (pTable->GetOddHeaders   (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"default"));
			if (FormatUtils::GetBitFromInt(fHF, 4))	WriteSectionStory (pTable->GetFirstHeaders (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"first"));

			// Footer

			if (FormatUtils::GetBitFromInt(fHF, 2))	WriteSectionStory (pTable->GetEvenFooters  (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"even"));
			if (FormatUtils::GetBitFromInt(fHF, 3))	WriteSectionStory (pTable->GetOddFooters   (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"default"));
			if (FormatUtils::GetBitFromInt(fHF, 5))	WriteSectionStory (pTable->GetFirstFooters (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"first"));
		}

		//MUST be ignored if the section does not have page number restart enabled.([MS-DOC] — v20101113. стр 152)
		bool bWasSprmSFPgnRestart = false;
		std::wstring wsSprmSPgnStart;

		for (std::list<SinglePropertyModifier>::iterator iter = sepx->grpprl->begin(); iter != sepx->grpprl->end(); ++iter)
		{
			switch (iter->OpCode)
			{
				//page margins
			case sprmOldSDxaLeft:
			case sprmSDxaLeft:
				{
					//left margin
					_marLeft = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					appendValueAttribute( &pgMar, _T( "w:left" ), FormatUtils::IntToWideString( _marLeft ).c_str() );
				}
				break;

			case sprmOldSDxaRight:
			case sprmSDxaRight:
				{
					//right margin
					_marRight = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					appendValueAttribute( &pgMar, _T( "w:right" ), FormatUtils::IntToWideString( _marRight ).c_str() );
				}
				break;

			case sprmOldSDyaTop:
			case sprmSDyaTop:
				//top margin
				appendValueAttribute( &pgMar, _T( "w:top" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmOldSDyaBottom:
			case sprmSDyaBottom:
				//bottom margin
				appendValueAttribute( &pgMar, _T( "w:bottom" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmOldSDzaGutter:
			case sprmSDzaGutter:
				//gutter margin
				appendValueAttribute( &pgMar, _T( "w:gutter" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmOldSDyaHdrTop:
			case sprmSDyaHdrTop:
				//header margin
				appendValueAttribute( &pgMar, _T( "w:header"), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmOldSDyaHdrBottom:
			case sprmSDyaHdrBottom:
				//footer margin
				appendValueAttribute( &pgMar, _T( "w:footer" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

				//page size and orientation
			case sprmOldSXaPage:
			case sprmSXaPage:
				{
					//width
					_pgWidth = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					appendValueAttribute( &pgSz, _T( "w:w" ), FormatUtils::IntToWideString( _pgWidth ).c_str() );
				}
				break;

			case sprmOldSYaPage:
			case sprmSYaPage:
				//height
				appendValueAttribute( &pgSz, _T( "w:h" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmOldSBOrientation:
			case sprmSBOrientation:
				//orientation
				appendValueAttribute( &pgSz, _T( "w:orient" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &PageOrientationMap[0][0], 3, 10 ).c_str() );
				break;

				//paper source
			case sprmOldSDmBinFirst:
			case sprmSDmBinFirst:
				appendValueAttribute( &paperSrc, _T( "w:first" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmOldSDmBinOther:
			case sprmSDmBinOther:
				appendValueAttribute( &paperSrc, _T( "w:other" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

				//page borders
			case sprmSBrcTop80:
			case sprmSBrcTop:
				{
					//top
					XMLTools::XMLElement<wchar_t> topBorder( _T( "w:top" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &topBorder );
					addOrSetBorder( &pgBorders, &topBorder);
				}
				break;

			case sprmSBrcLeft80:
			case sprmSBrcLeft:
				{
					//left
					XMLTools::XMLElement<wchar_t> leftBorder( _T( "w:left" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &leftBorder);
					addOrSetBorder( &pgBorders, &leftBorder);
				}
				break;

			case sprmSBrcBottom80:
			case sprmSBrcBottom:
				{
					//left
					XMLTools::XMLElement<wchar_t> bottomBorder( _T( "w:bottom" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &bottomBorder );
					addOrSetBorder( &pgBorders, &bottomBorder);
				}
				break;

			case sprmSBrcRight80:
			case sprmSBrcRight:
				{
					//left
					XMLTools::XMLElement<wchar_t> rightBorder( _T( "w:right" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &rightBorder);
					addOrSetBorder( &pgBorders, &rightBorder);
				}
				break;

				//footnote properties
			case sprmSRncFtn:
				//restart code
				appendValueElement( &footnotePr, _T( "numRestart" ), FormatUtils::MapValueToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ), &FootnoteRestartCodeMap[0][0], 3, 11 ).c_str(), true );
				break;

				//endnote properties
			case sprmSRncEdn:
				//restart code
				appendValueElement( &endnotePr, _T( "numRestart" ), FormatUtils::MapValueToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ), &EndnoteRestartCodeMap[0][0], 3, 11 ).c_str(), true );
				break;

			case sprmSFpc:
				{
					//position code
					short fpc = 0;

					if ( iter->argumentsSize == 2 )
					{
						fpc = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					}
					else
					{
						fpc = (short)iter->Arguments[0];
					}

					if ( fpc == 2 )
					{
						appendValueElement( &footnotePr, _T( "pos" ), _T( "beneathText" ), true );
					}
				}
				break;

			case sprmSNfcFtnRef:
				//number format
				appendValueElement( &footnotePr, _T( "numFmt" ), NumberingMapping::GetNumberFormatWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				break;

			case sprmSNfcEdnRef:
				//number format
				appendValueElement( &endnotePr, _T( "numFmt" ), NumberingMapping::GetNumberFormatWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				break;

			case sprmSNFtn:
				appendValueElement( &footnotePr, _T( "numStart" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				break;

			case sprmSNEdn:
				appendValueElement( &endnotePr, _T( "numStart" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				break;

				//doc grid
			case sprmSDyaLinePitch:
				appendValueAttribute( &docGrid, _T( "w:linePitch" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSDxtCharSpace:
				appendValueAttribute( &docGrid, _T( "w:charSpace" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSClm:
				appendValueAttribute( &docGrid, _T( "w:type" ), FormatUtils::MapValueToWideString( FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize ), &DocGridTypeMap[0][0], 4, 14 ).c_str() );
				break;

				// Columns
			case sprmOldSCcolumns:
			case sprmSCcolumns:
				{
					m_nColumns				=	static_cast<int> (FormatUtils::BytesToInt16 (iter->Arguments, 0, iter->argumentsSize) + 1);

					RELEASEARRAYOBJECTS (m_arrSpace);					
					m_arrSpace				=	new short [m_nColumns];

					appendValueAttribute (&cols, _T( "w:num" ), FormatUtils::IntToWideString (m_nColumns).c_str());
				}
				break;

			case sprmOldSDxaColumns:
			case sprmSDxaColumns:
				//evenly spaced columns
				appendValueAttribute (&cols, _T( "w:space" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmOldSDxaColWidth:
			case sprmSDxaColWidth:
				{
					// there is at least one width set, so create the array
					if (m_nColumns)
					{
						if (NULL == m_arrWidth)
							m_arrWidth		=	new short [m_nColumns];

						unsigned char nInd	=	iter->Arguments[0];
						m_arrWidth[nInd]	=	FormatUtils::BytesToInt16 (iter->Arguments, 1, iter->argumentsSize);
					}
				}
				break;

			case sprmOldSDxaColSpacing:
			case sprmSDxaColSpacing:
				{
					// there is at least one space set, so create the array
					if (m_nColumns)
					{
						if (NULL == m_arrSpace)
							m_arrSpace			=	new short[m_nColumns];

						unsigned char nInd		=	iter->Arguments[0];
						m_arrSpace [nInd]		=	FormatUtils::BytesToInt16 (iter->Arguments, 1, iter->argumentsSize);
					}
				}
				break;

				//bidi
			case sprmSFBiDi:
				appendFlagElement (m_pXmlNode, *iter, _T( "bidi" ), true );
				break;

				//title page
			case sprmOldSFTitlePage:
			case sprmSFTitlePage:
				appendFlagElement (m_pXmlNode, *iter, _T( "titlePg" ), true );
				break;

				//RTL gutter
			case sprmSFRTLGutter:
				appendFlagElement (m_pXmlNode, *iter, _T( "rtlGutter" ), true );
				break;

				//type
			case sprmOldSBkc:
			case sprmSBkc:
				{
					_type = FormatUtils::MapValueToWideString( iter->Arguments[0], &SectionTypeMap[0][0], 5, 11 );
				}
				break;

				//align
			case sprmOldSVjc:
			case sprmSVjc:
				appendValueElement (m_pXmlNode, _T( "vAlign" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &TextVerticalAlignment[0][0], 4, 7 ).c_str(), true );
				break;

				//pgNumType
			case sprmOldSNfcPgn:
			case sprmSNfcPgn:
				appendValueAttribute( &pgNumType, _T( "w:fmt" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &PageNumberFormatCodeMap[0][0], 42, 29 ).c_str() );
				break;

			case sprmOldSPgnStart:
			case sprmSPgnStart:
				wsSprmSPgnStart				=	FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmOldSFPgnRestart:
			case sprmSFPgnRestart:
				bWasSprmSFPgnRestart		=	true;
				break;

				//	<w:lnNumType>	-	Line Numbering Settings

			case sprmOldSLnnMin :
			case sprmSLnnMin :
				{
					unsigned short start	=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
                    start					=	(std::max)((int)start,1);

					appendValueAttribute (&lnNumType, _T("w:start"), start);
				}
				break;

			case sprmOldSLnc :
			case sprmSLnc :
				{
					SLncOperand mode		=	(SLncOperand)FormatUtils::BytesToUChar (iter->Arguments, 0, iter->argumentsSize);
                    mode					=	(SLncOperand)(std::min)((std::max)(mode,lncPerPage),lncContinue);

					appendValueAttribute (&lnNumType, _T("w:restart"), LineNumberRestart[mode].c_str() );
				}
				break;

			case sprmOldSNLnnMod :
			case sprmSNLnnMod :
				{
					short countBy			=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
                    countBy					=	(std::min)((std::max)((int)countBy,0),100);						//	The value MUST be in the interval [0, 100].

					appendValueAttribute (&lnNumType, _T("w:countBy"), countBy);
				}
				break;

			default:
				{
				}break;
			}
		}

		if (bWasSprmSFPgnRestart && false == wsSprmSPgnStart.empty() )
			appendValueAttribute( &pgNumType, _T( "w:start" ), wsSprmSPgnStart.c_str() );

		// build the columns
		if (m_arrWidth)
		{
			//set to unequal width
			XMLTools::XMLAttribute<wchar_t> equalWidth( _T( "w:equalWidth" ), _T( "0" ) );
			cols.AppendAttribute( equalWidth );

			//calculate the width of the last column:
			//the last column width is not written to the document because it can be calculated.
			if (0 == m_arrWidth [m_nColumns - 1])
			{
				short lastColWidth = (short)( _pgWidth - _marLeft - _marRight );

				for (int i = 0; i < m_nColumns - 1; ++i)
				{
					lastColWidth -= m_arrSpace[i];
					lastColWidth -= m_arrWidth[i];
				}

				m_arrWidth [m_nColumns - 1] = lastColWidth;
			}

			// append the xml elements
			for (int i = 0; i < m_nColumns; ++i)
			{
				XMLTools::XMLElement<wchar_t>	col		(_T( "w:col" ));
				XMLTools::XMLAttribute<wchar_t>	w		(_T( "w:w" ),		FormatUtils::IntToWideString (m_arrWidth[i]).c_str());
				XMLTools::XMLAttribute<wchar_t>	space	(_T( "w:space" ),	FormatUtils::IntToWideString (m_arrSpace[i]).c_str());

				col.AppendAttribute (w);
				col.AppendAttribute (space);
				cols.AppendChild	(col);
			}
		}

		//append the section type
		appendValueElement (m_pXmlNode, _T( "type" ), _type.c_str(), true );

		// Document-Wide Footnote Properties
		if (footnotePr.GetChildCount())
			m_pXmlNode->AppendChild (footnotePr);

		// Document-Wide Endnote Properties
		if (endnotePr.GetChildCount())
			m_pXmlNode->AppendChild (endnotePr);

		// Page Size
		if (pgSz.GetAttributeCount())
			m_pXmlNode->AppendChild (pgSz);

		// Page Borders
		if (pgBorders.GetChildCount())
			m_pXmlNode->AppendChild (pgBorders);

		// Page Margins
		if (pgMar.GetAttributeCount())
			m_pXmlNode->AppendChild (pgMar);

		// Line Numbering Settings
		if (lnNumType.GetAttributeCount())
			m_pXmlNode->AppendChild (lnNumType);

		// Paper Source Information
		if (paperSrc.GetAttributeCount())
			m_pXmlNode->AppendChild (paperSrc);

		// Single Column Definition)
		if (cols.GetAttributeCount() || cols.GetChildCount())
			m_pXmlNode->AppendChild (cols);

		// Document Grid
		if (docGrid.GetAttributeCount())
			m_pXmlNode->AppendChild (docGrid);

		// Page Numbering Settings
		if (pgNumType.GetAttributeCount())
			m_pXmlNode->AppendChild (pgNumType);

		if (m_pXmlWriter)
			m_pXmlWriter->WriteString (m_pXmlNode->GetXMLString().c_str() );
	}

	void SectionPropertiesMapping::AppendRef (XMLTools::XMLElement<wchar_t> *parent, const wchar_t* element, const wchar_t* refType, const wchar_t* refId)
	{
		XMLTools::XMLElement<wchar_t> headerRef (_T("w"), element);

		headerRef.AppendAttribute (_T("w:type"), refType);
		headerRef.AppendAttribute (_T("r:id"), refId);

		parent->AppendChild (headerRef);
	}

	const std::wstring & SectionPropertiesMapping::get_section_type()
	{
		return _type;
	}
	bool SectionPropertiesMapping::WriteSectionStory (CharacterRange* pRange, const std::wstring& StoryType, const std::wstring& Story)
	{
		if ( pRange )
		{
			int nRelID = 0;

			if (std::wstring(L"headerReference") == StoryType)
			{
				nRelID = _ctx->_docx->RegisterHeader();

				HeaderMapping* pHeader = new HeaderMapping(_ctx, (*pRange));
				if (pHeader)
				{
					_ctx->_doc->Convert(pHeader);
					RELEASEOBJECT(pHeader);
				}
			}

			if (std::wstring(L"footerReference") == StoryType)
			{
				nRelID = _ctx->_docx->RegisterFooter();
				
				FooterMapping* pFooter = new FooterMapping(_ctx, (*pRange));
				if (pFooter)
				{
					_ctx->_doc->Convert(pFooter);
					RELEASEOBJECT (pFooter);
				}
			}

			AppendRef (m_pXmlNode, StoryType.c_str(), Story.c_str(), ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString(nRelID) ).c_str() );

			return TRUE;
		}

		return FALSE;
	}
}
