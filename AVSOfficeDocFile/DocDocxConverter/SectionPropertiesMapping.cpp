#include "stdafx.h"

#include "MainDocumentMapping.h"

namespace DocFileFormat
{
	// Creates a new SectionPropertiesMapping which writes the properties to the given writer
	SectionPropertiesMapping::SectionPropertiesMapping (XmlUtils::CXmlWriter* pWriter, ConversionContext* pContext, int nSelectProperties) : PropertiesMapping (pWriter)
	{		
		m_bDeleteNode		=	TRUE;
		m_pXmlNode			=	new XMLTools::XMLElement<WCHAR> (_T("w:sectPr"));

		m_nColumns			=	0;
		m_arrWidth			=	NULL;
		m_arrSpace			=	NULL;

		_pgWidth			=	0;
		_marLeft			=	0;
		_marRight			=	0;

		_ctx				=	pContext;
		m_nSelectProperties	=	nSelectProperties;
		_type				=	wstring (_T("nextPage"));
	}

	// Creates a new SectionPropertiesMapping which appends the properties to a given node.
	SectionPropertiesMapping::SectionPropertiesMapping (XMLTools::XMLElement<WCHAR>* pBaseNode, ConversionContext* pContext, int nSelectProperties) : PropertiesMapping(NULL)
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
		_type				=	wstring (_T("nextPage"));
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

		XMLTools::XMLElement<WCHAR> pgSz		(_T("w:pgSz"));
		XMLTools::XMLElement<WCHAR> pgMar		(_T("w:pgMar"));
		XMLTools::XMLElement<WCHAR> lnNumType	(_T("w:lnNumType"));
		XMLTools::XMLElement<WCHAR> cols		(_T("w:cols"));
		XMLTools::XMLElement<WCHAR> docGrid		(_T("w:docGrid"));
		XMLTools::XMLElement<WCHAR> pgBorders	(_T("w:pgBorders"));
		XMLTools::XMLElement<WCHAR> paperSrc	(_T("w:paperSrc"));
		XMLTools::XMLElement<WCHAR> footnotePr	(_T("w:footnotePr"));
		XMLTools::XMLElement<WCHAR> endnotePr	(_T("w:endnotePr"));
		XMLTools::XMLElement<WCHAR> pgNumType	(_T("w:pgNumType"));

		HeaderAndFooterTable* pTable	=	_ctx->_doc->headerAndFooterTable;

		if (pTable)
		{
			// Header

			WriteSectionStory (pTable->GetEvenHeaders  (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"even"));
			WriteSectionStory (pTable->GetOddHeaders   (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"default"));
			WriteSectionStory (pTable->GetFirstHeaders (m_nSelectProperties), std::wstring(L"headerReference"), std::wstring(L"first"));

			// Footer

			WriteSectionStory (pTable->GetEvenFooters  (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"even"));
			WriteSectionStory (pTable->GetOddFooters   (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"default"));
			WriteSectionStory (pTable->GetFirstFooters (m_nSelectProperties), std::wstring(L"footerReference"), std::wstring(L"first"));
		}

		//MUST be ignored if the section does not have page number restart enabled.([MS-DOC] Ч v20101113. стр 152)
		bool bWasSprmSFPgnRestart = false;
		wstring wsSprmSPgnStart;

		for (list<SinglePropertyModifier>::iterator iter = sepx->grpprl->begin(); iter != sepx->grpprl->end(); ++iter)
		{
			switch (iter->OpCode)
			{
				//page margins
			case sprmSDxaLeft:
				{
					//left margin
					this->_marLeft = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					appendValueAttribute( &pgMar, _T( "w:left" ), FormatUtils::IntToWideString( this->_marLeft ).c_str() );
				}
				break;

			case sprmSDxaRight:
				{
					//right margin
					this->_marRight = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					appendValueAttribute( &pgMar, _T( "w:right" ), FormatUtils::IntToWideString( this->_marRight ).c_str() );
				}
				break;

			case sprmSDyaTop:
				//top margin
				appendValueAttribute( &pgMar, _T( "w:top" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSDyaBottom:
				//bottom margin
				appendValueAttribute( &pgMar, _T( "w:bottom" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSDzaGutter:
				//gutter margin
				appendValueAttribute( &pgMar, _T( "w:gutter" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSDyaHdrTop:
				//header margin
				appendValueAttribute( &pgMar, _T( "w:header"), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSDyaHdrBottom:
				//footer margin
				appendValueAttribute( &pgMar, _T( "w:footer" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

				//page size and orientation
			case sprmSXaPage:
				{
					//width
					this->_pgWidth = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					appendValueAttribute( &pgSz, _T( "w:w" ), FormatUtils::IntToWideString( this->_pgWidth ).c_str() );
				}
				break;

			case sprmSYaPage:
				//height
				appendValueAttribute( &pgSz, _T( "w:h" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSBOrientation:
				//orientation
				appendValueAttribute( &pgSz, _T( "w:orient" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &PageOrientationMap[0][0], 3, 10 ).c_str() );
				break;

				//paper source
			case sprmSDmBinFirst:
				appendValueAttribute( &paperSrc, _T( "w:first" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSDmBinOther:
				appendValueAttribute( &paperSrc, _T( "w:other" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

				//page borders
			case sprmSBrcTop80:
			case sprmSBrcTop:
				{
					//top
					XMLTools::XMLElement<WCHAR> topBorder( _T( "w:top" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &topBorder );
					addOrSetBorder( &pgBorders, &topBorder);
				}
				break;

			case sprmSBrcLeft80:
			case sprmSBrcLeft:
				{
					//left
					XMLTools::XMLElement<WCHAR> leftBorder( _T( "w:left" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &leftBorder);
					addOrSetBorder( &pgBorders, &leftBorder);
				}
				break;

			case sprmSBrcBottom80:
			case sprmSBrcBottom:
				{
					//left
					XMLTools::XMLElement<WCHAR> bottomBorder( _T( "w:bottom" ) );
					BorderCode bc( iter->Arguments, iter->argumentsSize );
					appendBorderAttributes( &bc, &bottomBorder );
					addOrSetBorder( &pgBorders, &bottomBorder);
				}
				break;

			case sprmSBrcRight80:
			case sprmSBrcRight:
				{
					//left
					XMLTools::XMLElement<WCHAR> rightBorder( _T( "w:right" ) );
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
			case sprmSCcolumns:
				{
					m_nColumns				=	static_cast<int> (FormatUtils::BytesToInt16 (iter->Arguments, 0, iter->argumentsSize) + 1);

					RELEASEARRAYOBJECTS (m_arrSpace);					
					m_arrSpace				=	new short [m_nColumns];

					appendValueAttribute (&cols, _T( "w:num" ), FormatUtils::IntToWideString (m_nColumns).c_str());
				}
				break;

			case sprmSDxaColumns:
				//evenly spaced columns
				appendValueAttribute (&cols, _T( "w:space" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ).c_str() );
				break;

			case sprmSDxaColWidth:
				{
					// there is at least one width set, so create the array
					if (m_nColumns)
					{
						if (NULL == m_arrWidth)
							m_arrWidth		=	new short [m_nColumns];

						byte nInd			=	iter->Arguments[0];
						m_arrWidth[nInd]	=	FormatUtils::BytesToInt16 (iter->Arguments, 1, iter->argumentsSize);
					}
				}
				break;

			case sprmSDxaColSpacing:
				{
					// there is at least one space set, so create the array
					if (m_nColumns)
					{
						if (NULL == m_arrSpace)
							m_arrSpace			=	new short[m_nColumns];

						byte nInd				=	iter->Arguments[0];
						m_arrSpace [nInd]		=	FormatUtils::BytesToInt16 (iter->Arguments, 1, iter->argumentsSize);
					}
				}
				break;

				//bidi
			case sprmSFBiDi:
				appendFlagElement (m_pXmlNode, *iter, _T( "bidi" ), true );
				break;

				//title page
			case sprmSFTitlePage:
				appendFlagElement (m_pXmlNode, *iter, _T( "titlePg" ), true );
				break;

				//RTL gutter
			case sprmSFRTLGutter:
				appendFlagElement (m_pXmlNode, *iter, _T( "rtlGutter" ), true );
				break;

				//type
			case sprmSBkc:
				{
					this->_type = FormatUtils::MapValueToWideString( iter->Arguments[0], &SectionTypeMap[0][0], 5, 11 );
				}
				break;

				//align
			case sprmSVjc:
				appendValueElement (m_pXmlNode, _T( "vAlign" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &TextVerticalAlignment[0][0], 4, 7 ).c_str(), true );
				break;

				//pgNumType
			case sprmSNfcPgn:
				appendValueAttribute( &pgNumType, _T( "w:fmt" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &PageNumberFormatCodeMap[0][0], 42, 29 ).c_str() );
				break;

			case sprmSPgnStart:
				wsSprmSPgnStart				=	FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmSFPgnRestart:
				bWasSprmSFPgnRestart		=	true;
				break;

				//	<w:lnNumType>	-	Line Numbering Settings

			case sprmSLnnMin :
				{
					unsigned short start	=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
					start					=	max(start,1);

					appendValueAttribute (&lnNumType, _T("w:start"), start);
				}
				break;

			case sprmSLnc :
				{
					SLncOperand mode		=	(SLncOperand)FormatUtils::BytesToUChar (iter->Arguments, 0, iter->argumentsSize);
					mode					=	(SLncOperand)min(max(mode,lncPerPage),lncContinue);

					appendValueAttribute (&lnNumType, _T("w:restart"), LineNumberRestart[mode].c_str() );
				}
				break;

			case sprmSNLnnMod :
				{
					short countBy			=	FormatUtils::BytesToUInt16 (iter->Arguments, 0, iter->argumentsSize);
					countBy					=	min(max(countBy,0),100);										//	The value MUST be in the interval [0, 100].

					appendValueAttribute (&lnNumType, _T("w:countBy"), countBy);
				}
				break;

			default:
#ifdef _DEBUG 
				// ATLTRACE (_T("SectionPropertiesMapping - UNKNOWN SPRM : 0x%x\n"), iter->OpCode);
#endif
				break;
			}
		}

		if (bWasSprmSFPgnRestart && false == wsSprmSPgnStart.empty() )
			appendValueAttribute( &pgNumType, _T( "w:start" ), wsSprmSPgnStart.c_str() );

		// build the columns
		if (m_arrWidth)
		{
			//set to unequal width
			XMLTools::XMLAttribute<WCHAR> equalWidth( _T( "w:equalWidth" ), _T( "0" ) );
			cols.AppendAttribute( equalWidth );

			//calculate the width of the last column:
			//the last column width is not written to the document because it can be calculated.
			if (0 == m_arrWidth [m_nColumns - 1])
			{
				short lastColWidth = (short)( this->_pgWidth - this->_marLeft - this->_marRight );

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
				XMLTools::XMLElement<WCHAR>		col		(_T( "w:col" ));
				XMLTools::XMLAttribute<WCHAR>	w		(_T( "w:w" ),		FormatUtils::IntToWideString (m_arrWidth[i]).c_str());
				XMLTools::XMLAttribute<WCHAR>	space	(_T( "w:space" ),	FormatUtils::IntToWideString (m_arrSpace[i]).c_str());

				col.AppendAttribute (w);
				col.AppendAttribute (space);
				cols.AppendChild (col);
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

	void SectionPropertiesMapping::AppendRef (XMLTools::XMLElement<WCHAR> *parent, const WCHAR* element, const WCHAR* refType, const WCHAR* refId)
	{
		XMLTools::XMLElement<WCHAR> headerRef (_T("w"), element);

		headerRef.AppendAttribute (_T("w:type"), refType);
		headerRef.AppendAttribute (_T("r:id"), refId);

		parent->AppendChild (headerRef);
	}

	BOOL SectionPropertiesMapping::WriteSectionStory (CharacterRange* pRange, const std::wstring& StoryType, const std::wstring& Story)
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

			AppendRef (m_pXmlNode, StoryType.c_str(), Story.c_str(), ( wstring( _T( "rId" ) ) + FormatUtils::IntToWideString(nRelID) ).c_str() );

			return TRUE;
		}

		return FALSE;
	}
}