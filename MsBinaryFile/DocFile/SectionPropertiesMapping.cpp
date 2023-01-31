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


#include "MainDocumentMapping.h"

namespace DocFileFormat
{
	SectionPropertiesMapping::SectionPropertiesMapping (XMLTools::CStringXmlWriter* pWriter, ConversionContext* pContext, int nSelectProperties) : PropertiesMapping (pWriter)
	{		
		m_bDeleteNode		=	TRUE;
        m_pXmlNode			=	new XMLTools::XMLElement (L"w:sectPr");

		m_nColumns			=	0;
		m_arrWidth			=	NULL;
		m_arrSpace			=	NULL;

		_pgWidth			=	0;
		_marLeft			=	0;
		_marRight			=	0;

		_ctx				=	pContext;
		m_nSelectProperties	=	nSelectProperties;
        _type				=	std::wstring (L"nextPage");
	}

    SectionPropertiesMapping::SectionPropertiesMapping (XMLTools::XMLElement* pBaseNode, ConversionContext* pContext, int nSelectProperties) : PropertiesMapping(NULL)
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
        _type				=	std::wstring (L"nextPage");
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
	void SectionPropertiesMapping::Apply (IVisitable* visited)
	{
		SectionPropertyExceptions* sepx	=	static_cast<SectionPropertyExceptions*>(visited);

        XMLTools::XMLElement pgSz		(L"w:pgSz");
        XMLTools::XMLElement pgMar		(L"w:pgMar");
        XMLTools::XMLElement lnNumType	(L"w:lnNumType");
        XMLTools::XMLElement cols		(L"w:cols");
        XMLTools::XMLElement docGrid	(L"w:docGrid");
        XMLTools::XMLElement pgBorders	(L"w:pgBorders");
        XMLTools::XMLElement paperSrc	(L"w:paperSrc");
        XMLTools::XMLElement footnotePr(L"w:footnotePr");
        XMLTools::XMLElement endnotePr	(L"w:endnotePr");
        XMLTools::XMLElement pgNumType	(L"w:pgNumType");

		HeaderAndFooterTable* pTable = _ctx->_doc->headerAndFooterTable;

		if (pTable)
		{
			unsigned char fHF = _ctx->_doc->nWordVersion == 0 ? 255 : 0; //all headers & footers
			for (std::list<SinglePropertyModifier>::iterator iter = sepx->grpprl->begin(); iter != sepx->grpprl->end(); ++iter)
			{
				switch (iter->OpCode)
				{
					case sprmOldSGprfIhdt:
					//case sprmSGprfIhdt: //betesil_pl_it.doc
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
					default:
						break;
				}
			}
			if (_ctx->_doc->nWordVersion == 2)
			{
				if (GETBIT(fHF, 0)) WriteSectionStory (pTable->GetNextHeaderFooter(), std::wstring(L"headerReference"), std::wstring(L"even"));
				if (GETBIT(fHF, 1))	WriteSectionStory (pTable->GetNextHeaderFooter(), std::wstring(L"headerReference"), std::wstring(L"default"));
				if (GETBIT(fHF, 2))	WriteSectionStory (pTable->GetNextHeaderFooter(), std::wstring(L"footerReference"), std::wstring(L"even"));
				if (GETBIT(fHF, 3))	WriteSectionStory (pTable->GetNextHeaderFooter(), std::wstring(L"footerReference"), std::wstring(L"default"));
				if (GETBIT(fHF, 4))	WriteSectionStory (pTable->GetNextHeaderFooter(), std::wstring(L"headerReference"), std::wstring(L"first"));
				if (GETBIT(fHF, 5))	WriteSectionStory (pTable->GetNextHeaderFooter(), std::wstring(L"footerReference"), std::wstring(L"first"));
			}
			else
			{
				// Header

				if (FormatUtils::GetBitFromInt(fHF, 0)) WriteSectionStory (pTable->GetEvenHeaders  (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"even"));
				if (FormatUtils::GetBitFromInt(fHF, 1))	WriteSectionStory (pTable->GetOddHeaders   (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"default"));
				if (FormatUtils::GetBitFromInt(fHF, 4))	WriteSectionStory (pTable->GetFirstHeaders (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"first"));

				// Footer

				if (FormatUtils::GetBitFromInt(fHF, 2))	WriteSectionStory (pTable->GetEvenFooters  (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"even"));
				if (FormatUtils::GetBitFromInt(fHF, 3))	WriteSectionStory (pTable->GetOddFooters   (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"default"));
				if (FormatUtils::GetBitFromInt(fHF, 5))	WriteSectionStory (pTable->GetFirstFooters (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"first"));
			}
		}

		//MUST be ignored if the section does not have page number restart enabled.([MS-DOC] — v20101113. стр 152)
		bool bWasSprmSFPgnRestart = false;
		std::wstring wsSprmSPgnStart;
		int nProperty = 0; // for unknown

		for (std::list<SinglePropertyModifier>::iterator iter = sepx->grpprl->begin(); iter != sepx->grpprl->end(); ++iter)
		{
			switch (iter->OpCode)
			{
			case sprmOldSDxaLeft:
			case sprmSDxaLeft:
			{
				_marLeft = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
                appendValueAttribute( &pgMar, L"w:left", FormatUtils::IntToWideString( _marLeft ) );
			}
			break;

			case sprmOldSDxaRight:
			case sprmSDxaRight:
			{
				_marRight = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
                appendValueAttribute( &pgMar, L"w:right", FormatUtils::IntToWideString( _marRight ) );
			}
			break;

			case sprmOldSDyaTop:
			case sprmSDyaTop:
//top margin
                appendValueAttribute( &pgMar, L"w:top", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmOldSDyaBottom:
			case sprmSDyaBottom:
//bottom margin
                appendValueAttribute( &pgMar, L"w:bottom", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmOldSDzaGutter:
			case sprmSDzaGutter:
//gutter margin
                appendValueAttribute( &pgMar, L"w:gutter", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmOldSDyaHdrTop:
			case sprmSDyaHdrTop:
//header margin
                appendValueAttribute( &pgMar, L"w:header", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmOldSDyaHdrBottom:
			case sprmSDyaHdrBottom:
//footer margin
                appendValueAttribute( &pgMar, L"w:footer", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

//page size and orientation
			case sprmOldSXaPage:
			case sprmSXaPage:
				{
					_pgWidth = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
                    appendValueAttribute( &pgSz, L"w:w", FormatUtils::IntToWideString( _pgWidth ) );
				}
				break;

			case sprmOldSYaPage:
			case sprmSYaPage:
                appendValueAttribute( &pgSz, L"w:h", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmOldSBOrientation:
			case sprmSBOrientation:
				//orientation
				if (_ctx->_doc->nWordVersion == 2)
					appendValueAttribute( &pgSz, L"w:orient", FormatUtils::MapValueToWideString( iter->Arguments[0] + 1, &PageOrientationMap[0][0], 3, 10 ) );
				else
					appendValueAttribute( &pgSz, L"w:orient", FormatUtils::MapValueToWideString( iter->Arguments[0], &PageOrientationMap[0][0], 3, 10 ) );
				break;

//paper source
			case sprmOldSDmBinFirst:
			case sprmSDmBinFirst:
                appendValueAttribute( &paperSrc, L"w:first", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmOldSDmBinOther:
			case sprmSDmBinOther:
                appendValueAttribute( &paperSrc, L"w:other", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

//page borders
			case sprmSBrcTop80:
			case sprmSBrcTop:
			{
				//top
                XMLTools::XMLElement topBorder( L"w:top" );
				BorderCode bc( iter->Arguments, iter->argumentsSize );
				appendBorderAttributes( &bc, &topBorder );
				addOrSetBorder( &pgBorders, &topBorder);
			}
			break;

			case sprmSBrcLeft80:
			case sprmSBrcLeft:
			{
                XMLTools::XMLElement leftBorder( L"w:left" );
				BorderCode bc( iter->Arguments, iter->argumentsSize );
				appendBorderAttributes( &bc, &leftBorder);
				addOrSetBorder( &pgBorders, &leftBorder);
			}
			break;

			case sprmSBrcBottom80:
			case sprmSBrcBottom:
			{
                //left
                XMLTools::XMLElement bottomBorder( L"w:bottom" );
				BorderCode bc( iter->Arguments, iter->argumentsSize );
				appendBorderAttributes( &bc, &bottomBorder );
				addOrSetBorder( &pgBorders, &bottomBorder);
			}
			break;

			case sprmSBrcRight80:
			case sprmSBrcRight:
			{
				//left
                XMLTools::XMLElement rightBorder( L"w:right" );
				BorderCode bc( iter->Arguments, iter->argumentsSize );
				appendBorderAttributes( &bc, &rightBorder);
				addOrSetBorder( &pgBorders, &rightBorder);
			}
			break;

			case sprmSRncFtn:
				//restart code
                appendValueElement( &footnotePr, L"numRestart", FormatUtils::MapValueToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ), &FootnoteRestartCodeMap[0][0], 3, 11 ), true );
				break;

			case sprmSRncEdn:
				//restart code
                appendValueElement( &endnotePr, L"numRestart", FormatUtils::MapValueToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ), &EndnoteRestartCodeMap[0][0], 3, 11 ), true );
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
                    appendValueElement( &footnotePr, L"pos", L"beneathText", true );
				}
			}break;			

			case sprmSNfcFtnRef:
                appendValueElement( &footnotePr, L"numFmt", NumberingMapping::GetNumberFormatWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ), true );
				break;

			case sprmSNfcEdnRef:
                appendValueElement( &endnotePr, L"numFmt", NumberingMapping::GetNumberFormatWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ), true );
				break;

			case sprmSNFtn:
                appendValueElement( &footnotePr, L"numStart", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ), true );
				break;

			case sprmSNEdn:
                appendValueElement( &endnotePr, L"numStart", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ), true );
				break;

			case sprmSDyaLinePitch:
                appendValueAttribute( &docGrid, L"w:linePitch", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmSDxtCharSpace:
                appendValueAttribute( &docGrid, L"w:charSpace", FormatUtils::IntToWideString( FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmSClm:
                appendValueAttribute( &docGrid, L"w:type", FormatUtils::MapValueToWideString( FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize ), &DocGridTypeMap[0][0], 4, 14 ) );
				break;

			case sprmOldSCcolumns:
			case sprmSCcolumns:
			{
				m_nColumns = static_cast<int> (FormatUtils::BytesToInt16 (iter->Arguments, 0, iter->argumentsSize) + 1);

				RELEASEARRAYOBJECTS (m_arrSpace);					
				m_arrSpace = new short [m_nColumns];

                appendValueAttribute (&cols, L"w:num", FormatUtils::IntToWideString (m_nColumns));
			}
			break;

			case sprmOldSDxaColumns:
			case sprmSDxaColumns:
				//evenly spaced columns
                appendValueAttribute (&cols, L"w:space", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );
				break;

			case sprmOldSDxaColWidth:
			case sprmSDxaColWidth:
			{				
				if (m_nColumns)	// there is at least one width set, so create the array
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
						m_arrSpace = new short[m_nColumns];

					unsigned char nInd = iter->Arguments[0];
					if (nInd < m_nColumns)
					{
						m_arrSpace[nInd] = FormatUtils::BytesToInt16(iter->Arguments, 1, iter->argumentsSize);
					}
				}
			}break;

			case sprmSFBiDi:
			{
                appendFlagElement (m_pXmlNode, *iter, L"bidi", iter->argumentsSize > 0 ? iter->Arguments[0] : true );
			}break;

			case sprmOldSFTitlePage:
			case sprmSFTitlePage:
			{
                appendFlagElement (m_pXmlNode, *iter, L"titlePg", iter->argumentsSize > 0 ? iter->Arguments[0] : true );
			}break;

			case sprmSFRTLGutter:
			{
                appendFlagElement (m_pXmlNode, *iter, L"rtlGutter", iter->argumentsSize > 0 ? iter->Arguments[0] : true );
			}break;
				
			case sprmOldSBkc:
			case sprmSBkc:
			{
				_type = FormatUtils::MapValueToWideString( iter->Arguments[0], &SectionTypeMap[0][0], 5, 11 );
			}
			break;

			case sprmOldSVjc:
			case sprmSVjc:
                appendValueElement (m_pXmlNode, L"vAlign", FormatUtils::MapValueToWideString( iter->Arguments[0], &TextVerticalAlignment[0][0], 4, 7 ), true );
				break;

			case sprmOldSNfcPgn:
			case sprmSNfcPgn:
                appendValueAttribute( &pgNumType, L"w:fmt", FormatUtils::MapValueToWideString( iter->Arguments[0], &PageNumberFormatCodeMap[0][0], 42, 29 ) );
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

                    appendValueAttribute (&lnNumType, L"w:start", start);
				}
				break;

			case sprmOldSLnc :
			case sprmSLnc :
				{
					SLncOperand mode		=	(SLncOperand)FormatUtils::BytesToUChar (iter->Arguments, 0, iter->argumentsSize);
                    mode					=	(SLncOperand)(std::min)((std::max)(mode,lncPerPage),lncContinue);

                    appendValueAttribute (&lnNumType, L"w:restart", LineNumberRestart[mode] );
				}
				break;

			case sprmOldSNLnnMod :
			case sprmSNLnnMod :
				{
					short countBy			=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
                    countBy					=	(std::min)((std::max)((int)countBy,0),100);						//	The value MUST be in the interval [0, 100].

                    appendValueAttribute (&lnNumType, L"w:countBy", countBy);
				}
				break;

			default:
				if (iter->argumentsSize == 2)
				{
					nProperty = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);
				}
				else if (iter->argumentsSize == 1)
				{
					nProperty = FormatUtils::BytesToUChar(iter->Arguments, 0, iter->argumentsSize);
				}
			}
		}

		if (bWasSprmSFPgnRestart && false == wsSprmSPgnStart.empty() )
            appendValueAttribute( &pgNumType, L"w:start", wsSprmSPgnStart );

		// build the columns
		if (m_arrWidth)
		{
            XMLTools::XMLAttribute equalWidth( L"w:equalWidth", L"0" );
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

			for (int i = 0; i < m_nColumns; ++i)
			{
                XMLTools::XMLElement	col		(L"w:col");
                XMLTools::XMLAttribute	w		(L"w:w",		FormatUtils::IntToWideString (m_arrWidth[i]));
                XMLTools::XMLAttribute	space	(L"w:space",	FormatUtils::IntToWideString (m_arrSpace[i]));

				col.AppendAttribute (w);
				col.AppendAttribute (space);
				cols.AppendChild	(col);
			}
		}

        appendValueElement (m_pXmlNode, L"type", _type, true );

		if (footnotePr.GetChildCount())
			m_pXmlNode->AppendChild (footnotePr);

		if (endnotePr.GetChildCount())
			m_pXmlNode->AppendChild (endnotePr);

		if (pgSz.GetAttributeCount())
			m_pXmlNode->AppendChild (pgSz);

		if (pgBorders.GetChildCount())
			m_pXmlNode->AppendChild (pgBorders);

		if (pgMar.GetAttributeCount())
			m_pXmlNode->AppendChild (pgMar);

		if (lnNumType.GetAttributeCount())
			m_pXmlNode->AppendChild (lnNumType);

		if (paperSrc.GetAttributeCount())
			m_pXmlNode->AppendChild (paperSrc);

		if (cols.GetAttributeCount() || cols.GetChildCount())
			m_pXmlNode->AppendChild (cols);

		if (docGrid.GetAttributeCount())
			m_pXmlNode->AppendChild (docGrid);

		if (pgNumType.GetAttributeCount())
			m_pXmlNode->AppendChild (pgNumType);

		if (m_pXmlWriter)
			m_pXmlWriter->WriteString (m_pXmlNode->GetXMLString() );
	}

    void SectionPropertiesMapping::AppendRef (XMLTools::XMLElement *parent, const std::wstring& element, const std::wstring& refType, const std::wstring& refId)
	{
        XMLTools::XMLElement headerRef (L"w", element);

        headerRef.AppendAttribute (L"w:type", refType);
        headerRef.AppendAttribute (L"r:id", refId);

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

            AppendRef (m_pXmlNode, StoryType, Story, ( std::wstring( L"rId" ) + FormatUtils::IntToWideString(nRelID) ) );

			return TRUE;
		}

		return FALSE;
	}
}
