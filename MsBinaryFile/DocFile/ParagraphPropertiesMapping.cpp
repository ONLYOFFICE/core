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


#include "ParagraphPropertiesMapping.h"

namespace DocFileFormat
{
	ParagraphPropertiesMapping::ParagraphPropertiesMapping(XMLTools::CStringXmlWriter* writer, ConversionContext* context, WordDocument* document, CharacterPropertyExceptions* paraEndChpx, bool isBidi, bool isParagraphStyleNeeded)
		: PropertiesMapping( writer ), m_context(NULL), _pPr(NULL), _framePr(NULL), _sepx(NULL), _paraEndChpx(NULL), _sectionNr(0),
		m_document(NULL), _isParagraphStyleNeeded(isParagraphStyleNeeded)
	{
		m_document		=	document;
		m_context		=	context;
	
        _pPr			= new XMLTools::XMLElement( L"w:pPr" );
        _framePr		= new XMLTools::XMLElement( L"w:framePr" );

		_paraEndChpx		= paraEndChpx;
		_isBidi				= isBidi;
		_isSectionPageBreak	= 0;
	}

	ParagraphPropertiesMapping::ParagraphPropertiesMapping( XMLTools::CStringXmlWriter* writer, ConversionContext* context, WordDocument* document, CharacterPropertyExceptions* paraEndChpx, bool isBidi, SectionPropertyExceptions* sepx, int sectionNr, bool isParagraphStyleNeeded )
		: PropertiesMapping( writer ), m_context(NULL), _pPr(NULL), _framePr(NULL), _sepx(NULL), _paraEndChpx(NULL), _sectionNr(0), 
		m_document(NULL), _isParagraphStyleNeeded(isParagraphStyleNeeded)
	{
		m_document		=	document;
		m_context		=	context;
		
        _pPr			= new XMLTools::XMLElement( L"w:pPr" );
        _framePr		= new XMLTools::XMLElement( L"w:framePr" );
		
		_paraEndChpx		= paraEndChpx;
		_isBidi				= isBidi;
		_isSectionPageBreak	= 0;
		
		_sepx				= sepx;
		_sectionNr			= sectionNr;
	}

	ParagraphPropertiesMapping::~ParagraphPropertiesMapping()
	{
		RELEASEOBJECT(_pPr);
		RELEASEOBJECT(_framePr);
	}

	int ParagraphPropertiesMapping::get_section_page_break()
	{
		return _isSectionPageBreak;
	}
	void ParagraphPropertiesMapping::Apply( IVisitable* visited )
	{
		ParagraphPropertyExceptions* papx = static_cast<ParagraphPropertyExceptions*>( visited );

        XMLTools::XMLElement ind		( L"w:ind" );
        XMLTools::XMLElement numPr		( L"w:numPr" );
        XMLTools::XMLElement pBdr		( L"w:pBdr" );
        XMLTools::XMLElement spacing	( L"w:spacing" );
        XMLTools::XMLElement* jc = NULL;

		if ( _isParagraphStyleNeeded )
		{
			//append style id , do not append "Normal" style (istd 0)
            XMLTools::XMLElement  pStyle( L"w:pStyle" );
            XMLTools::XMLAttribute styleId( L"w:val",  StyleIdentifierMap[0] );

			if ( papx->istd < m_document->Styles->Styles->size() )
			{
				styleId.SetValue( FormatUtils::XmlEncode(StyleSheetMapping::MakeStyleId( m_document->Styles->Styles->at( papx->istd ) )) );
			}

			pStyle.AppendAttribute( styleId );
			_pPr->AppendChild( pStyle );
		}

		//append formatting of paragraph end mark

        XMLTools::XMLElementPtr rPr = XMLTools::XMLElementPtr(XMLTools::XMLElementPtr(new XMLTools::XMLElement( L"w:rPr" )));

		if ( _paraEndChpx != NULL )
		{
			RevisionData* rev = new RevisionData( _paraEndChpx );
			CharacterPropertiesMapping* ccMapping = new CharacterPropertiesMapping( rPr.get(), m_document, rev, papx, false );

			_paraEndChpx->Convert( ccMapping );

			if ( rev->Type == Deleted )
			{
				XMLTools::XMLElement del( L"w:del" );
				rPr->AppendChild( del );
			}

			if( rPr->GetChildCount() > 0 )
			{
				_pPr->AppendChild( rPr );
			}

			RELEASEOBJECT( ccMapping );
			RELEASEOBJECT( rev );
		}
		
		bool bNumPr = false;

		std::list<SinglePropertyModifier>::iterator end = papx->grpprl->end();
		for (std::list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); iter != end; ++iter)
		{
			int nProperty = 0; //for unknown test

			switch ( iter->OpCode )
			{
				case sprmPIpgp:
				{
                    appendValueElement(_pPr, L"divId", FormatUtils::IntToWideString( FormatUtils::BytesToUInt32(iter->Arguments, 0, iter->argumentsSize)), true);
				}break;			

				case sprmPFAutoSpaceDE:
				{
                    appendFlagElement(_pPr, *iter, L"autoSpaceDE", true);
				}break;				

				case sprmPFAutoSpaceDN:
				{
                    appendFlagElement( _pPr, *iter, L"autoSpaceDN", true );
				}break;				

				case sprmPFBiDi:
				{
					bool val = iter->argumentsSize > 0 ? (iter->Arguments[0] != 0) : true;
                    appendFlagElement( _pPr, *iter, L"bidi", val);
					_isBidi = val;
				}break;			

				case sprmPFContextualSpacing:
				{
                    appendFlagElement( _pPr, *iter, L"contextualSpacing", true );
				}break;			

				case sprmOldPFKeep:
				case sprmPFKeep:
				{
                    appendFlagElement( _pPr, *iter, L"keepLines", true );
				}break;

				case sprmOldPFKeepFollow:
				case sprmPFKeepFollow:
                    appendFlagElement( _pPr, *iter, L"keepNext", true );
					break;

				case sprmPFKinsoku:
                    appendFlagElement( _pPr, *iter, L"kinsoku", true );
					break;

				case sprmPFOverflowPunct:
                    appendFlagElement( _pPr, *iter, L"overflowPunct", true );
					break;

				case sprmPFPageBreakBefore:
                    appendFlagElement( _pPr, *iter, L"pageBreakBefore", true );
					break;

				case sprmPFNoAutoHyph:
                    appendFlagElement( _pPr, *iter, L"suppressAutoHyphens", true );
					break;

				case sprmPFNoLineNumb:
                    appendFlagElement( _pPr, *iter, L"suppressLineNumbers", true );
					break;

				case sprmPFNoAllowOverlap:
                    appendFlagElement( _pPr, *iter, L"suppressOverlap", true );
					break;

				case sprmPFTopLinePunct:
                    appendFlagElement( _pPr, *iter, L"topLinePunct", true );
					break;

				case sprmPFWidowControl:
                    appendFlagElement( _pPr, *iter, L"widowControl", true );
					break;

				case sprmPFWordWrap:
                    appendFlagElement( _pPr, *iter, L"wordWrap", true );
					break;

					//indentation
				case sprmOldPDxaLeft:
				case sprmOldPNest:			
				case sprmPDxaLeft:
				case sprmPDxaLeft80:
				case sprmPNest:
				case sprmPNest80:
                    appendValueAttribute( &ind, L"w:left", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmPDxcLeft:
                    appendValueAttribute( &ind, L"w:leftChars", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmOldPDxaLeft1:
				case sprmPDxaLeft1:
				case sprmPDxaLeft180:
				{
					short flValue = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
					std::wstring flName;

					if ( flValue >= 0 )
					{
                        flName = std::wstring( L"w:firstLine" );
					}
					else
					{
                        flName = std::wstring( L"w:hanging" );
						flValue *= -1;
					}

					appendValueAttribute( &ind, flName, flValue );
				}break;				

				case sprmPDxcLeft1:
                    appendValueAttribute( &ind, L"w:firstLineChars", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmOldPDxaRight:
				case sprmPDxaRight:
				case sprmPDxaRight80:
                    appendValueAttribute( &ind, L"w:right", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmPDxcRight:
                    appendValueAttribute( &ind, L"w:rightChars", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

					//spacing
				case sprmOldPDyaBefore:
				case sprmPDyaBefore:
                    appendValueAttribute( &spacing, L"w:before", FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmOldPDyaAfter:
				case sprmPDyaAfter:
                    appendValueAttribute( &spacing, L"w:after", FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmPFDyaAfterAuto:
                    appendValueAttribute( &spacing, L"w:afterAutospacing", iter->Arguments[0] );
					break;

				case sprmPFDyaBeforeAuto:
                    appendValueAttribute( &spacing, L"w:beforeAutospacing", iter->Arguments[0] );
					break;

				case sprmOldPDyaLine:
				case sprmPDyaLine:
				{
					LineSpacingDescriptor lspd( iter->Arguments, iter->argumentsSize );

                    XMLTools::XMLAttribute line( L"w:line", FormatUtils::IntToWideString( abs( lspd.dyaLine ) ));
					spacing.AppendAttribute( line );

                    XMLTools::XMLAttribute lineRule( L"w:lineRule", L"auto" );

					if( ( !lspd.fMultLinespace ) && ( lspd.dyaLine < 0 ) )
					{
                        lineRule.SetValue( L"exact" );
					}
					else if( ( !lspd.fMultLinespace ) && ( lspd.dyaLine >= 0 ) )
					{
                        lineRule.SetValue( L"atLeast" );
					}

					//no line rule means auto
					spacing.AppendAttribute( lineRule );
				}break;				

	//justification code
				case sprmOldPJc:
				case sprmPJc:
				case sprmPJc80:
				{ 
					if (_isBidi && (iter->Arguments[0] == 0 || iter->Arguments[0] == 2))
					{
						iter->Arguments[0] = (iter->Arguments[0] == 0 ? 2 : 0);
					}
					RELEASEOBJECT( jc );
                    jc = new XMLTools::XMLElement( L"w:jc" );
					if ( jc )
					{
                        XMLTools::XMLAttribute jcVal( L"w:val", FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::JustificationCode[0][0], 10, 15 ));
						jc->AppendAttribute( jcVal );
					}
				}break;	
	//borders
					//case 0x461C:
				case sprmOldPBrcTop:
				case sprmPBrcTop:
					//case 0x4424:
				case sprmPBrcTop80:
				{
                    XMLTools::XMLElement topBorder( L"w:top" );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &topBorder );

					addOrSetBorder( &pBdr, &topBorder );
				}break;				

					//case 0x461D:
				case sprmOldPBrcLeft:
				case sprmPBrcLeft:
					//case 0x4425:
				case sprmPBrcLeft80:
				{
                    XMLTools::XMLElement leftBorder( L"w:left" );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &leftBorder );

					addOrSetBorder( &pBdr, &leftBorder );
				}break;				

					//case 0x461E:
				case sprmOldPBrcBottom:
				case sprmPBrcBottom:
					//case 0x4426:
				case sprmPBrcBottom80:
				{
                    XMLTools::XMLElement bottomBorder( L"w:bottom" );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &bottomBorder );

					addOrSetBorder( &pBdr, &bottomBorder );
				}break;
				
					//case 0x461F:
				case sprmOldPBrcRight:
				case sprmPBrcRight:
					//case 0x4427:
				case sprmPBrcRight80:
				{
                    XMLTools::XMLElement rightBorder( L"w:right" );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &rightBorder );

					addOrSetBorder( &pBdr, &rightBorder );
				}break;				

					//case 0x4620:
				case sprmOldPBrcBetween:
				case sprmPBrcBetween:
					//case 0x4428:
				case sprmPBrcBetween80:
				{
                    XMLTools::XMLElement betweenBorder( L"w:between" );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &betweenBorder );

					addOrSetBorder( &pBdr, &betweenBorder );
				}break;				

					//case 0x4621:
				case sprmOldPBrcBar:
				case sprmPBrcBar:
					//case 0x4629:
				case sprmPBrcBar80:
				{
                    XMLTools::XMLElement barBorder( L"w:bar" );

					BorderCode bc( iter->Arguments, iter->argumentsSize );

					appendBorderAttributes( &bc, &barBorder );

					addOrSetBorder( &pBdr, &barBorder );
				}break;				

		//shading
				case sprmPShd80:
				case sprmPShd:
				{
					ShadingDescriptor desc( iter->Arguments, iter->argumentsSize );

					appendShading( _pPr, desc );
				}break;
				

		//numbering
				case sprmOldPAnld:
				{
					NumberingDescriptorPtr desc = NumberingDescriptor::create( iter->Arguments, iter->argumentsSize );

					if (m_document->listTable)
					{
						std::wstring sRPr;
						if (rPr->GetChildCount() > 0) sRPr = rPr->GetXMLString();

						unsigned short numId = m_document->listTable->appendNumbering( desc, sRPr );
                        appendValueElement( &numPr, L"numId", numId, true );
					}
				}break;

				case sprmOldSOlstAnm:
				{
					OutlineListDescriptor desc( iter->Arguments, iter->argumentsSize );
				}break;

				case sprmOldPNLvlAnm:
				{					
					short level = FormatUtils::BytesToUChar( iter->Arguments, 0, iter->argumentsSize) - 1;
					
					level = 0;
                    appendValueElement( &numPr, L"ilvl", level, true );
					bNumPr = true;
				}break;				
				
				case sprmOldPFNoLineNumb:
				{
				}break;

				case sprmPIlvl:
				{
					if(NULL != m_document->listTable && false == m_document->listTable->listData.empty())
                        appendValueElement( &numPr, L"ilvl", iter->Arguments[0], true );
				}break;

				case sprmPIlfo:
				{
					//Если numbering.xml пустой, то не пищем свойство
					if (NULL != m_document->listTable && false == m_document->listTable->listData.empty())
					{
						unsigned short numId = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
						appendValueElement( &numPr, L"numId", numId, true );
					}
				}
				break;


				case sprmOldPChgTabs:	//tabs
				case sprmPChgTabsPapx:
				case sprmPChgTabs:
				{
                    XMLTools::XMLElement tabs( L"w:tabs" );

					unsigned int pos = 0;

					//read the removed tabs
					unsigned char itbdDelMax = iter->Arguments[pos];

					pos++;

					for( int i = 0; i < itbdDelMax; i++ )
					{
                        XMLTools::XMLElement tab( L"w:tab" );

						//clear
                        XMLTools::XMLAttribute tabsVal( L"w:val", L"clear" );
						tab.AppendAttribute( tabsVal );

						//position
                        XMLTools::XMLAttribute tabsPos( L"w:pos", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, pos, iter->argumentsSize ) ) );
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
					unsigned char itbdAddMax = pos < iter->argumentsSize ? iter->Arguments[pos] : 0;

					if (itbdAddMax > 0)	pos++;

					for ( int i = 0; i < itbdAddMax; i++ )
					{
						TabDescriptor tbd( iter->Arguments[pos + ( itbdAddMax * 2 ) + i] );

                        XMLTools::XMLElement tab( L"w:tab" );

						//justification
                        XMLTools::XMLAttribute tabsVal( L"w:val", FormatUtils::MapValueToWideString( tbd.jc, &Global::TabStop[0][0], 7, 8 ) );
						tab.AppendAttribute( tabsVal );

						//tab leader type
                        XMLTools::XMLAttribute leader( L"w:leader", FormatUtils::MapValueToWideString( tbd.tlc, &Global::TabLeader[0][0], 8, 11 ) );
						tab.AppendAttribute( leader );

						//position
                        XMLTools::XMLAttribute tabsPos( L"w:pos", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, ( pos + (i * 2) ), iter->argumentsSize ) ) );
						tab.AppendAttribute( tabsPos );

						tabs.AppendChild( tab );
					}

					_pPr->AppendChild( tabs );
				}
				break;

	//frame properties
				case sprmOldPPc:
				case sprmPPc:
				{
					//position code
					unsigned char flag = iter->Arguments[0];

                    appendValueAttribute (_framePr, L"w:hAnchor", FormatUtils::MapValueToWideString( ( ( flag & 0xC0 ) >> 6 ), &Global::HorizontalPositionCode[0][0], 4, 7 ) );
                    appendValueAttribute (_framePr, L"w:vAnchor", FormatUtils::MapValueToWideString( ( ( flag & 0x30 ) >> 4 ), &Global::VerticalPositionCode[0][0], 4, 7 ) );
				}
				break;

				case sprmOldPWr:
				case sprmPWr:
                    appendValueAttribute( _framePr, L"w:wrap", FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::TextFrameWrapping[0][0], 6, 10 ) );
					break;

				case sprmOldPDxaAbs:
				case sprmPDxaAbs:
				{
					unsigned short val = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					switch (val)
					{
					case 0x0000:	break; //left
					case 0xfffc:	appendValueAttribute( _framePr, L"w:xAlign", L"center");	break;
					case 0xfff8:	appendValueAttribute( _framePr, L"w:xAlign", L"right");		break;
					case 0xfff4:	appendValueAttribute( _framePr, L"w:xAlign", L"inside");	break;
					case 0xfff0:	appendValueAttribute( _framePr, L"w:xAlign", L"outside");	break;
					default:
						appendValueAttribute( _framePr, L"w:x",  (short)val);
					}
				}break;
				case sprmOldPDyaAbs:
				case sprmPDyaAbs:
				{
					unsigned short val = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					switch (val)
					{
					case 0x0000:	break; //inline
					case 0xfffc:	appendValueAttribute( _framePr, L"w:yAlign", L"top");		break;
					case 0xfff8:	appendValueAttribute( _framePr, L"w:yAlign", L"center");	break;
					case 0xfff4:	appendValueAttribute( _framePr, L"w:yAlign", L"bottom");	break;
					case 0xfff0:	appendValueAttribute( _framePr, L"w:yAlign", L"inside");	break;
					case 0xffec:	appendValueAttribute( _framePr, L"w:yAlign", L"outside");	break;
					default:
						appendValueAttribute( _framePr, L"w:y", (short)val );
					}					
				}break;
				case sprmPWHeightAbs:
				{
					unsigned short val = FormatUtils::BytesToUInt16(iter->Arguments, 0, iter->argumentsSize);
					unsigned short DyaHeightAbs = GETBITS(val, 0, 14);
					bool fMinHeight = GETBIT(val, 15);
					if ( DyaHeightAbs > 0)
						appendValueAttribute(_framePr, L"w:h", DyaHeightAbs);
					if (fMinHeight)
						appendValueAttribute(_framePr, L"w:hRule", L"atLeast");
					else if (DyaHeightAbs == 0)
						appendValueAttribute(_framePr, L"w:hRule", L"auto");
					else 
						appendValueAttribute(_framePr, L"w:hRule", L"exact");
				}break;
				case sprmOldPDxaWidth:
				case sprmPDxaWidth:
                    appendValueAttribute( _framePr, L"w:w", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmOldPDxaFromText:
				case sprmPDxaFromText:
                    appendValueAttribute( _framePr, L"w:hSpace", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmOldPDyaFromText:
				case sprmPDyaFromText:
                    appendValueAttribute( _framePr, L"w:vSpace", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
					break;

				case sprmOldPDcs:
				case sprmPDcs:
				{
					short pDcs = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );

                    appendValueAttribute( _framePr, L"w:dropCap", FormatUtils::MapValueToWideString( ( pDcs & 0x07 ), &Global::TextFrameDropCapLocation[0][0], 3, 7 ) );

                    appendValueAttribute( _framePr, L"w:lines", (unsigned char)( ( pDcs & 0xF8 ) >> 3 ) );
				}
				break;

	//outline level
				case sprmOldPIncLvl: //????
				case sprmPOutLvl:
				{
                    appendValueElement( _pPr, L"outlineLvl", iter->Arguments[0], false );
				}break;

				default:
				{
					if (iter->argumentsSize == 2)
					{
						nProperty = FormatUtils::BytesToUInt16( iter->Arguments, 0, iter->argumentsSize );
					}else
						if (iter->argumentsSize == 1)
					{
						nProperty = FormatUtils::BytesToUChar( iter->Arguments, 0, iter->argumentsSize );
					}
				}break;
			}
		}
		
		if ( numPr.GetChildCount() > 0 && ((bNumPr && m_document->nWordVersion > 0) || m_document->nWordVersion == 0))//append numPr
		{//23.doc
			_pPr->AppendChild( numPr );
		}
		
		if ( _framePr->GetAttributeCount() > 0 )
		{
			_pPr->AppendChild( *_framePr );
		}

		_isSectionPageBreak = 0;
		if ( _sepx != NULL )
		{
            XMLTools::XMLElement sectPr( L"w:sectPr" );
			SectionPropertiesMapping* sectionPropertiesMapping = new SectionPropertiesMapping( &sectPr, m_context, _sectionNr );
			_sepx->Convert( sectionPropertiesMapping );

			if (sectionPropertiesMapping->get_section_type() == L"nextPage")
			{
				//в этом параграфе уже есть разрыв страницы - br на page не нужен
				//Vinci_Customer_Case_Study.doc
				_isSectionPageBreak = 1;
			}
			else 
				_isSectionPageBreak = 2;
			
			RELEASEOBJECT( sectionPropertiesMapping );
			_pPr->AppendChild( sectPr );
		}

		if ( ind.GetAttributeCount() > 0 )
		{
			_pPr->AppendChild( ind );
		}
		
		if ( spacing.GetAttributeCount() > 0 )
		{
			_pPr->AppendChild( spacing );
		}
		
		if ( jc )
		{
			_pPr->AppendChild( *jc );
			RELEASEOBJECT( jc );
		}

		if ( pBdr.GetChildCount() > 0 )
		{
			_pPr->AppendChild( pBdr );
		}

		if ( ( _pPr->GetChildCount() > 0 ) || ( _pPr->GetAttributeCount() > 0 ) )
		{
			m_pXmlWriter->WriteString( _pPr->GetXMLString() );
		}
	}
}
