

#include "ParagraphPropertiesMapping.h"

namespace DocFileFormat
{
	ParagraphPropertiesMapping::ParagraphPropertiesMapping(XmlUtils::CXmlWriter* writer, ConversionContext* context, WordDocument* document, CharacterPropertyExceptions* paraEndChpx, bool isParagraphStyleNeeded) : PropertiesMapping( writer ), m_context(NULL), _pPr(NULL), _framePr(NULL), _sepx(NULL), _paraEndChpx(NULL), _sectionNr(0), m_document(NULL), _isParagraphStyleNeeded(isParagraphStyleNeeded)
	{
		m_document	=	document;
		m_context	=	context;
	
		_pPr = new XMLTools::XMLElement<wchar_t>( _T( "w:pPr" ) );
		_framePr = new XMLTools::XMLElement<wchar_t>( _T( "w:framePr" ) );
		_paraEndChpx = paraEndChpx;
	}

	ParagraphPropertiesMapping::ParagraphPropertiesMapping( XmlUtils::CXmlWriter* writer, ConversionContext* context, WordDocument* document, CharacterPropertyExceptions* paraEndChpx, SectionPropertyExceptions* sepx, int sectionNr, bool isParagraphStyleNeeded ): PropertiesMapping( writer ), m_context(NULL), _pPr(NULL), _framePr(NULL), _sepx(NULL), _paraEndChpx(NULL), _sectionNr(0), m_document(NULL), _isParagraphStyleNeeded(isParagraphStyleNeeded)
	{
		m_document	=	document;
		m_context	=	context;
		
		_pPr = new XMLTools::XMLElement<wchar_t>( _T( "w:pPr" ) );
		_framePr = new XMLTools::XMLElement<wchar_t>( _T( "w:framePr" ) );
		_paraEndChpx = paraEndChpx;
		_sepx = sepx;
		_sectionNr = sectionNr;
	}

	ParagraphPropertiesMapping::~ParagraphPropertiesMapping()
	{
		RELEASEOBJECT(_pPr);
		RELEASEOBJECT(_framePr);
	}
}

namespace DocFileFormat
{
	void ParagraphPropertiesMapping::Apply( IVisitable* visited )
	{
		ParagraphPropertyExceptions* papx = static_cast<ParagraphPropertyExceptions*>( visited );

		XMLTools::XMLElement<wchar_t> ind( _T( "w:ind" ) );
		XMLTools::XMLElement<wchar_t> numPr( _T( "w:numPr" ) );
		XMLTools::XMLElement<wchar_t> pBdr( _T( "w:pBdr" ) );
		XMLTools::XMLElement<wchar_t> spacing( _T( "w:spacing" ) );
		XMLTools::XMLElement<wchar_t>* jc = NULL;

		if ( this->_isParagraphStyleNeeded )
		{
			//append style id , do not append "Normal" style (istd 0)
			XMLTools::XMLElement<wchar_t>  pStyle( _T( "w:pStyle" ) );
			XMLTools::XMLAttribute<wchar_t> styleId( _T( "w:val" ), StyleIdentifierMap[0] );

			if ( papx->istd < m_document->Styles->Styles->size() )
			{
				styleId.SetValue( FormatUtils::XmlEncode(StyleSheetMapping::MakeStyleId( this->m_document->Styles->Styles->at( papx->istd ) )).c_str() );
			}

			pStyle.AppendAttribute( styleId );
			_pPr->AppendChild( pStyle );
		}

		//append formatting of paragraph end mark
		if ( _paraEndChpx != NULL )
		{
			XMLTools::XMLElement<wchar_t>* rPr = new XMLTools::XMLElement<wchar_t>( _T( "w:rPr" ) );

			//append properties
			RevisionData* rev = new RevisionData( _paraEndChpx );
			CharacterPropertiesMapping* ccMapping = new CharacterPropertiesMapping( rPr, m_document, rev, papx, false );
			_paraEndChpx->Convert( ccMapping );

			//append delete infos
			if ( rev->Type == Deleted )
			{
				XMLTools::XMLElement<wchar_t> del( _T( "w:del" ) );
				rPr->AppendChild( del );
			}

			if( rPr->GetChildCount() >0 )
			{
				_pPr->AppendChild( *rPr );
			}

			RELEASEOBJECT( ccMapping );
			RELEASEOBJECT( rev );
			RELEASEOBJECT( rPr );
		}
		
		std::list<SinglePropertyModifier>::iterator end = papx->grpprl->end();
		for (std::list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); iter != end; ++iter)
		{
			switch ( iter->OpCode )
			{
			case sprmPIpgp:
				{
					appendValueElement(_pPr, _T( "divId" ), FormatUtils::IntToWideString( FormatUtils::BytesToUInt32(iter->Arguments, 0, iter->argumentsSize)).c_str(), true);
				}
				break;

				//element flags
			case sprmPFAutoSpaceDE:
				{
					appendFlagElement(_pPr, *iter, _T( "autoSpaceDE" ), true);
				}
				break;

			case sprmPFAutoSpaceDN:
				{
					appendFlagElement( _pPr, *iter, _T( "autoSpaceDN" ), true );
				}
				break;

			case sprmPFBiDi:
				{
					appendFlagElement( _pPr, *iter, _T( "bidi" ), true );
				}
				break;

			case sprmPFContextualSpacing:
				{
					appendFlagElement( _pPr, *iter, _T( "contextualSpacing" ), true );
				}
				break;

			case sprmPFKeep:
				appendFlagElement( _pPr, *iter, _T( "keepLines" ), true );
				break;

			case sprmPFKeepFollow:
				appendFlagElement( _pPr, *iter, _T( "keepNext" ), true );
				break;

			case sprmPFKinsoku:
				appendFlagElement( _pPr, *iter, _T( "kinsoku" ), true );
				break;

			case sprmPFOverflowPunct:
				appendFlagElement( _pPr, *iter, _T( "overflowPunct" ), true );
				break;

			case sprmPFPageBreakBefore:
				appendFlagElement( _pPr, *iter, _T( "pageBreakBefore" ), true );
				break;

			case sprmPFNoAutoHyph:
				appendFlagElement( _pPr, *iter, _T( "suppressAutoHyphens" ), true );
				break;

			case sprmPFNoLineNumb:
				appendFlagElement( _pPr, *iter, _T( "suppressLineNumbers" ), true );
				break;

			case sprmPFNoAllowOverlap:
				appendFlagElement( _pPr, *iter, _T( "suppressOverlap" ), true );
				break;

			case sprmPFTopLinePunct:
				appendFlagElement( _pPr, *iter, _T( "topLinePunct" ), true );
				break;

			case sprmPFWidowControl:
				appendFlagElement( _pPr, *iter, _T( "widowControl" ), true );
				break;

			case sprmPFWordWrap:
				appendFlagElement( _pPr, *iter, _T( "wordWrap" ), true );
				break;

				//indentation
			case sprmPDxaLeft:
			case sprmPDxaLeft80:
			case sprmPNest:
			case sprmPNest80:
				appendValueAttribute( &ind, _T( "w:left" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDxcLeft:
				appendValueAttribute( &ind, _T( "w:leftChars" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDxaLeft1:
			case sprmPDxaLeft180:
				{
					short flValue = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					wstring flName;

					if ( flValue >= 0 )
					{
						flName = wstring( _T( "w:firstLine" ) );
					}
					else
					{
						flName = wstring( _T( "w:hanging" ) );
						flValue *= -1;
					}

					appendValueAttribute( &ind, flName.c_str(), flValue );
				}  
				break;

			case sprmPDxcLeft1:
				appendValueAttribute( &ind, _T( "w:firstLineChars" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDxaRight:
			case sprmPDxaRight80:
				appendValueAttribute( &ind, _T( "w:right" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDxcRight:
				appendValueAttribute( &ind, _T( "w:rightChars" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

				//spacing
			case sprmPDyaBefore:
				appendValueAttribute( &spacing, _T( "w:before" ), FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDyaAfter:
				appendValueAttribute( &spacing, _T( "w:after" ), FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPFDyaAfterAuto:
				appendValueAttribute( &spacing, _T( "w:afterAutospacing" ), iter->Arguments[0] );
				break;

			case sprmPFDyaBeforeAuto:
				appendValueAttribute( &spacing, _T( "w:beforeAutospacing" ), iter->Arguments[0] );
				break;

			case sprmPDyaLine:
				{
					LineSpacingDescriptor lspd( iter->Arguments, iter->argumentsSize );

					XMLTools::XMLAttribute<wchar_t> line( _T( "w:line" ), FormatUtils::IntToWideString( abs( lspd.dyaLine ) ).c_str() );
					spacing.AppendAttribute( line );

					XMLTools::XMLAttribute<wchar_t> lineRule( _T( "w:lineRule" ), _T( "auto" ) );

					if( ( !lspd.fMultLinespace ) && ( lspd.dyaLine < 0 ) )
					{
						lineRule.SetValue( _T( "exact" ) );
					}
					else if( ( !lspd.fMultLinespace ) && ( lspd.dyaLine >= 0 ) )
					{
						lineRule.SetValue( _T( "atLeast" ) );
					}

					//no line rule means auto
					spacing.AppendAttribute( lineRule );
				}
				break;

				//justification code
			case sprmPJc:
			case sprmPJc80:
				{ 
					RELEASEOBJECT( jc );
					jc = new XMLTools::XMLElement<wchar_t>( _T( "w:jc" ) );
					XMLTools::XMLAttribute<wchar_t> jcVal( _T( "w:val" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::JustificationCode[0][0], 10, 15 ).c_str() );
					jc->AppendAttribute( jcVal );
				}
				break;

				//borders
				//case 0x461C:
			case sprmPBrcTop:
				//case 0x4424:
			case sprmPBrcTop80:
				{
					XMLTools::XMLElement<wchar_t> topBorder( _T( "w:top" ) );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &topBorder );

					addOrSetBorder( &pBdr, &topBorder );
				}
				break;

				//case 0x461D:
			case sprmPBrcLeft:
				//case 0x4425:
			case sprmPBrcLeft80:
				{
					XMLTools::XMLElement<wchar_t> leftBorder( _T( "w:left" ) );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &leftBorder );

					addOrSetBorder( &pBdr, &leftBorder );
				}
				break;

				//case 0x461E:
			case sprmPBrcBottom:
				//case 0x4426:
			case sprmPBrcBottom80:
				{
					XMLTools::XMLElement<wchar_t> bottomBorder( _T( "w:bottom" ) );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &bottomBorder );

					addOrSetBorder( &pBdr, &bottomBorder );
				}
				break;

				//case 0x461F:
			case sprmPBrcRight:
				//case 0x4427:
			case sprmPBrcRight80:
				{
					XMLTools::XMLElement<wchar_t> rightBorder( _T( "w:right" ) );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &rightBorder );

					addOrSetBorder( &pBdr, &rightBorder );
				}
				break;

				//case 0x4620:
			case sprmPBrcBetween:
				//case 0x4428:
			case sprmPBrcBetween80:
				{
					XMLTools::XMLElement<wchar_t> betweenBorder( _T( "w:between" ) );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &betweenBorder );

					addOrSetBorder( &pBdr, &betweenBorder );
				}
				break;

				//case 0x4621:
			case sprmPBrcBar:
				//case 0x4629:
			case sprmPBrcBar80:
				{
					XMLTools::XMLElement<wchar_t> barBorder( _T( "w:bar" ) );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &barBorder );

					addOrSetBorder( &pBdr, &barBorder );
				}
				break;

				//shading
			case sprmPShd80:
			case sprmPShd:
				{
					ShadingDescriptor desc( iter->Arguments, iter->argumentsSize );

					appendShading( _pPr, desc );
				}
				break;

				//numbering
			case sprmPIlvl:
				//Если numbering.xml пустой, то не пищем свойство
				if(NULL != m_document->listTable && false == m_document->listTable->listData.empty())
					appendValueElement( &numPr, _T( "ilvl" ), iter->Arguments[0], true );
				break;

			case sprmPIlfo:
				{
					//Если numbering.xml пустой, то не пищем свойство
					//Todo разобраться с закоментированным кодом
					if(NULL != m_document->listTable && false == m_document->listTable->listData.empty())
						appendValueElement( &numPr, _T( "numId" ), FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize ), true );

					//check if there is a ilvl reference, if not, check the count of LVLs.
					//if only one LVL exists in the referenced list, create a hard reference to that LVL
					//if (containsLvlReference(papx.grpprl) == false)
					//{
					//    ListFormatOverride lfo = m_context.Doc.ListFormatOverrideTable[val];
					//    int index = NumberingMapping.FindIndexbyId(m_context.Doc.ListTable, lfo.lsid);
					//    ListData lst = m_context.Doc.ListTable[index];
					//    if (lst.rglvl.Length == 1)
					//    {
					//        appendValueElement(numPr, "ilvl", "0", true);
					//    }
					//}
				}
				break;

				//tabs
			case sprmPChgTabsPapx:
			case sprmPChgTabs:
				{
					XMLTools::XMLElement<wchar_t> tabs( _T( "w:tabs" ) );

					int pos = 0;

					//read the removed tabs
					unsigned char itbdDelMax = iter->Arguments[pos];

					pos++;

					for( int i=0; i < itbdDelMax; i++ )
					{
						XMLTools::XMLElement<wchar_t> tab( _T( "w:tab" ) );

						//clear
						XMLTools::XMLAttribute<wchar_t> tabsVal( _T( "w:val" ), _T( "clear" ) );
						tab.AppendAttribute( tabsVal );

						//position
						XMLTools::XMLAttribute<wchar_t> tabsPos( _T( "w:pos" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, pos, iter->argumentsSize ) ).c_str() );
						tab.AppendAttribute( tabsPos );

						tabs.AppendChild( tab );

						//skip the tolerence array in sprm 0xC615
						if ( iter->OpCode == sprmPChgTabs )
						{
							pos += 4;
						}
						else
						{
							pos += 2;
						}
					}

					//read the added tabs
					unsigned char itbdAddMax = iter->Arguments[pos];

					pos++;

					for ( int i = 0; i < itbdAddMax; i++ )
					{
						TabDescriptor tbd( iter->Arguments[pos + ( itbdAddMax * 2 ) + i] );

						XMLTools::XMLElement<wchar_t> tab( _T( "w:tab" ) );

						//justification
						XMLTools::XMLAttribute<wchar_t> tabsVal( _T( "w:val" ), FormatUtils::MapValueToWideString( tbd.jc, &Global::TabStop[0][0], 7, 8 ).c_str() );
						tab.AppendAttribute( tabsVal );

						//tab leader type
						XMLTools::XMLAttribute<wchar_t> leader( _T( "w:leader" ), FormatUtils::MapValueToWideString( tbd.tlc, &Global::TabLeader[0][0], 8, 11 ).c_str() );
						tab.AppendAttribute( leader );

						//position
						XMLTools::XMLAttribute<wchar_t> tabsPos( _T( "w:pos" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, ( pos + (i * 2) ), iter->argumentsSize ) ).c_str() );
						tab.AppendAttribute( tabsPos );

						tabs.AppendChild( tab );
					}

					this->_pPr->AppendChild( tabs );
				}
				break;

				//frame properties
			case sprmPPc:
				{
					//position code
					unsigned char flag = iter->Arguments[0];

					appendValueAttribute (_framePr, _T( "w:hAnchor" ), FormatUtils::MapValueToWideString( ( ( flag & 0xC0 ) >> 6 ), &Global::HorizontalPositionCode[0][0], 4, 7 ).c_str() );
					appendValueAttribute (_framePr, _T( "w:vAnchor" ), FormatUtils::MapValueToWideString( ( ( flag & 0x30 ) >> 4 ), &Global::VerticalPositionCode[0][0], 4, 7 ).c_str() );
				}
				break;

			case sprmPWr:
				appendValueAttribute( this->_framePr, _T( "w:wrap" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::TextFrameWrapping[0][0], 6, 10 ).c_str() );
				break;

			case sprmPDxaAbs:
				appendValueAttribute( this->_framePr, _T( "w:x" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDyaAbs:
				appendValueAttribute( this->_framePr, _T( "w:y" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPWHeightAbs:
				appendValueAttribute( this->_framePr, _T( "w:h" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDxaWidth:
				appendValueAttribute( this->_framePr, _T( "w:w" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDxaFromText:
				appendValueAttribute( this->_framePr, _T( "w:hSpace" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDyaFromText:
				appendValueAttribute( this->_framePr, _T( "w:vSpace" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				break;

			case sprmPDcs:
				{
					short pDcs = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );

					appendValueAttribute( this->_framePr, _T( "w:dropCap" ), FormatUtils::MapValueToWideString( ( pDcs & 0x07 ), &Global::TextFrameDropCapLocation[0][0], 3, 7 ).c_str() );

					appendValueAttribute( this->_framePr, _T( "w:lines" ), (unsigned char)( ( pDcs & 0xF8 ) >> 3 ) );
				}
				break;

				//outline level
			case sprmPOutLvl:
				appendValueElement( _pPr, _T( "outlineLvl" ), iter->Arguments[0], false );
				break;

			default:
				{
#ifdef _DEBUG 
					// //ATLTRACE (_T("ParagraphPropertiesMapping - UNKNOWN SPRM : 0x%x\n"), iter->OpCode);
#endif
				}
				break;
			}
		}

		//append frame properties
		if ( _framePr->GetAttributeCount() > 0 )
		{
			_pPr->AppendChild( *_framePr );
		}

		//append section properties
		if ( _sepx != NULL )
		{
			XMLTools::XMLElement<wchar_t> sectPr( _T( "w:sectPr" ) );
			SectionPropertiesMapping* sectionPropertiesMapping = new SectionPropertiesMapping( &sectPr, this->m_context, this->_sectionNr );
			_sepx->Convert( sectionPropertiesMapping );
			RELEASEOBJECT( sectionPropertiesMapping );
			_pPr->AppendChild( sectPr );
		}

		//append indent
		if ( ind.GetAttributeCount() > 0 )
		{
			_pPr->AppendChild( ind );
		}

		//append spacing
		if ( spacing.GetAttributeCount() > 0 )
		{
			_pPr->AppendChild( spacing );
		}

		//append justification
		if ( jc != NULL )
		{
			_pPr->AppendChild( *jc );
			RELEASEOBJECT( jc );
		}

		//append numPr
		if ( numPr.GetChildCount() > 0 )
		{
			_pPr->AppendChild( numPr );
		}

		//append borders
		if ( pBdr.GetChildCount() > 0 )
		{
			_pPr->AppendChild( pBdr );
		}

		//write Properties
		if ( ( _pPr->GetChildCount() > 0 ) || ( _pPr->GetAttributeCount() > 0 ) )
		{
			m_pXmlWriter->WriteString( _pPr->GetXMLString().c_str() );
		}
	}
}