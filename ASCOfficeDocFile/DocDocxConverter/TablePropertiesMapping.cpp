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

#include "TablePropertiesMapping.h"

namespace DocFileFormat
{
	TablePropertiesMapping::TablePropertiesMapping (XMLTools::CStringXmlWriter* pWriter, StyleSheet* styles, std::vector<short>* grid, bool isTableStyleNeeded ):
			PropertiesMapping(pWriter), _tblPr(NULL), _tblGrid(NULL), _tblBorders(NULL), _grid(grid),
			brcLeft(NULL), brcTop(NULL), brcBottom(NULL), brcRight(NULL), brcHorz(NULL), brcVert(NULL), _styles(styles),
			_isTableStyleNeeded(isTableStyleNeeded)
{
    _tblPr		= new XMLTools::XMLElement( L"w:tblPr");
    _tblBorders = new XMLTools::XMLElement( L"w:tblBorders");
}
TablePropertiesMapping::~TablePropertiesMapping()
{
	RELEASEOBJECT (_tblPr);
	RELEASEOBJECT (_tblGrid);
	RELEASEOBJECT (_tblBorders);
	RELEASEOBJECT (brcLeft);
	RELEASEOBJECT (brcTop);
	RELEASEOBJECT (brcBottom);
	RELEASEOBJECT (brcRight);
	RELEASEOBJECT (brcHorz);
	RELEASEOBJECT (brcVert);
}
}
namespace DocFileFormat
{
	void TablePropertiesMapping::Apply(IVisitable* visited)
	{
		TablePropertyExceptions* tapx = static_cast<TablePropertyExceptions*>( visited );

        XMLTools::XMLElement	tblCellMar	( L"w:tblCellMar");
        XMLTools::XMLElement	tblLayout	( L"w:tblLayout");
        XMLTools::XMLElement	tblpPr		( L"w:tblpPr");
        XMLTools::XMLAttribute layoutType	( L"w:type", L"fixed");
		bool bLayoutFixed = true;

		_CP_OPT(short) tblIndent;
		short gabHalf		= 0;
		short marginLeft	= 0;
		short marginRight	= 0;

		BYTE itcFirst		= 0;
		BYTE itcLim			= 0;

		bool bTableW = false;

		for ( std::list<SinglePropertyModifier>::iterator iter = tapx->grpprl->begin(); iter != tapx->grpprl->end(); iter++ )
		{
			switch( iter->OpCode )
			{
				case sprmOldTDxaGapHalf:
				case sprmTDxaGapHalf:
				{
					gabHalf = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
				}
				break;

				case sprmOldTDefTable:
				case sprmTDefTable:
				{				
	//table definition
					SprmTDefTable tDef( iter->Arguments, iter->argumentsSize );
					//Workaround for retrieving the indent of the table:
					//In some files there is a indent but no sprmTWidthIndent is set.
					//For this cases we can calculate the indent of the table by getting the 
					//first boundary of the TDef and adding the padding of the cells
					
					tblIndent = gabHalf + FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );
					//If there follows a real sprmTWidthIndent, this value will be overwritten

                    //tblIndent = (std::max)((int)tblIndent,0); //cerere.doc
				}
				break;

				case sprmTTableWidth:
				{	
	//preferred table width
					unsigned char fts = iter->Arguments[0];
					short width = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );

                    XMLTools::XMLElement tblW( L"w:tblW");

                    XMLTools::XMLAttribute w( L"w:w", FormatUtils::IntToWideString( width ) );
                    XMLTools::XMLAttribute type( L"w:type", FormatUtils::MapValueToWideString( fts, &WidthType[0][0], 4, 5 ) );

					tblW.AppendAttribute( type );
					tblW.AppendAttribute( w );

					_tblPr->AppendChild( tblW );

					bTableW = true;
				}
				break;
				case sprmTMerge:
				{
					itcFirst = iter->Arguments[0];
					itcLim = iter->Arguments[1];
				}break;
				case sprmTSplit:
				{

				}break;
				case sprmOldTJc:
				case sprmTJc:
				case sprmTJcRow:
				{				//justification

                    appendValueElement( _tblPr, L"jc" , FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::JustificationCode[0][0], 10, 15 ), true );
				}
				break;

				case sprmTWidthIndent:
				{				//indent

					tblIndent	=	FtsWWidth_Indent(iter->Arguments).wWidth;
					// tblIndent	=	FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );
				}
				break;

				case sprmTIstd:
				case sprmTIstdPermute:
				{				//style

					if ( _isTableStyleNeeded )
					{
						size_t ind = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
						
						std::wstring id = ind < _styles->Styles->size() ? StyleSheetMapping::MakeStyleId( _styles->Styles->at( ind ) ) : L"";

                        if( id != std::wstring( L"TableNormal") && !id.empty() )
						{
                            appendValueElement( _tblPr, L"tblStyle", id, true );
						}
					}
				}
				break;

				case sprmTFBiDi:
				case sprmTFBiDi90:
				{				//bidi

                    appendValueElement( _tblPr, L"bidiVisual", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ), true );
				}
				break;

				case sprmOldTTlp:
				case sprmTTlp:
				{				//table look

                    appendValueElement( _tblPr, L"tblLook", FormatUtils::IntToFormattedWideString( FormatUtils::BytesToInt16( iter->Arguments, 2, iter->argumentsSize ), L"%04x" ), true );
				}
				break;

				case sprmTFAutofit:
				{				//autofit

					if ( iter->Arguments[0] == 1 )
					{
                        layoutType.SetValue( L"auto" );
						bLayoutFixed = false;
					}
				}
				break;

				case sprmTCellPadding:
				case sprmTCellPaddingDefault:
				case sprmTCellPaddingOuter:
				{				//default cell padding (margin)

					unsigned char grfbrc = iter->Arguments[2];
					short wMar = FormatUtils::BytesToInt16( iter->Arguments, 4, iter->argumentsSize );
					std::wstring strValue = FormatUtils::IntToWideString( wMar );

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x01 ) )
					{
                        appendDxaElement( &tblCellMar,  L"top" , strValue, true );
					}

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x02 ) )
					{
						marginLeft = wMar;
					}

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x04 ) )
					{
                        appendDxaElement( &tblCellMar, L"bottom", strValue, true );
					}

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x08 ) )
					{
						marginRight = wMar;
					}
				}
				break;

				case sprmTCHorzBands:
				{				//row count

                    appendValueElement( _tblPr, L"tblStyleRowBandSize", iter->Arguments[0], true );
				}
				break;

				case sprmTCVertBands:
				{				//col count

                    appendValueElement( _tblPr, L"tblStyleColBandSize", iter->Arguments[0], true );
				}
				break;

				case sprmTFNoAllowOverlap:
				{				//overlap

                    std::wstring tblOverlapVal = std::wstring( L"overlap");

					if ( iter->Arguments[0] )
					{
                        tblOverlapVal = std::wstring( L"never");
					}

                    appendValueElement( _tblPr, L"tblOverlap", tblOverlapVal, true );
				}
				break;

				case sprmOldTSetShd	:
				case sprmTSetShdTable :
				{				//shading

					appendShading( _tblPr, ShadingDescriptor( iter->Arguments, iter->argumentsSize ) );
				}
				break;

				case sprmTTableBorders80:
				{				//borders 80 exceptions

					const int size = 4;
					unsigned char brc80[size];

					//top border
					memcpy( brc80, iter->Arguments, size );
					RELEASEOBJECT( brcTop );
					brcTop = new BorderCode( brc80, size );

					//left
					memcpy( brc80, ( iter->Arguments + 4 ), size );
					RELEASEOBJECT( brcLeft );
					brcLeft = new BorderCode( brc80, size );

					//bottom
					memcpy( brc80, ( iter->Arguments + 8 ), size );
					RELEASEOBJECT( brcBottom );
					brcBottom = new BorderCode( brc80, size );

					//right
					memcpy( brc80, ( iter->Arguments + 12 ), size );
					RELEASEOBJECT( brcRight );
					brcRight = new BorderCode( brc80, size );

					//inside H
					memcpy( brc80, ( iter->Arguments + 16 ), size );
					RELEASEOBJECT( brcHorz );
					brcHorz = new BorderCode( brc80, size );

					//inside V
					memcpy( brc80, ( iter->Arguments + 20 ), size );
					RELEASEOBJECT( brcVert );
					brcVert = new BorderCode( brc80, size );
				}
				break;

				//border exceptions
				case sprmOldTTableBorders:
				case sprmTTableBorders:
				{
					const int size = 8;
					unsigned char brc[size];

					//top border
					memcpy( brc, iter->Arguments, size );
					RELEASEOBJECT( brcTop );
					brcTop = new BorderCode( brc, size );

					//left
					memcpy( brc, ( iter->Arguments + 8 ), size );
					RELEASEOBJECT( brcLeft );
					brcLeft = new BorderCode( brc, size );

					//bottom
					memcpy( brc, ( iter->Arguments + 16 ), size );
					RELEASEOBJECT( brcBottom );
					brcBottom = new BorderCode( brc, size );

					//right
					memcpy( brc, ( iter->Arguments + 24 ), size );
					RELEASEOBJECT( brcRight );
					brcRight = new BorderCode( brc, size );

					//inside H
					memcpy( brc, ( iter->Arguments + 32 ), size );
					RELEASEOBJECT( brcHorz );
					brcHorz = new BorderCode( brc, size );

					//inside V
					memcpy( brc, ( iter->Arguments + 40 ), size );
					RELEASEOBJECT( brcVert );
					brcVert = new BorderCode( brc, size );
				}
				break;

				//floating table properties
				case sprmTPc:
				{
					unsigned char flag = ( iter->Arguments[0] & 0x30 ) >> 4;

                    appendValueAttribute( &tblpPr, L"w:vertAnchor", FormatUtils::MapValueToWideString( flag, &Global::VerticalPositionCode[0][0], 4, 7 ) );

					flag = ( iter->Arguments[0] & 0xC0 ) >> 6;

                    appendValueAttribute( &tblpPr, L"w:horzAnchor", FormatUtils::MapValueToWideString( flag, &Global::HorizontalPositionCode[0][0], 4, 7 ) );
				}
				break;

				case sprmTDxaFromText:
				{
                    appendValueAttribute( &tblpPr, L"w:leftFromText", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

				case sprmTDxaFromTextRight:
				{
                    appendValueAttribute( &tblpPr, L"w:rightFromText", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

				case sprmTDyaFromText:
				{
                    appendValueAttribute( &tblpPr, L"w:topFromText", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

				case sprmTDyaFromTextBottom:
				{
                    appendValueAttribute( &tblpPr, L"w:bottomFromText", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}			
				break;

				case sprmTDxaAbs:
				{
                    appendValueAttribute( &tblpPr, L"w:tblpX", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

				case sprmTDyaAbs:
				{
                    appendValueAttribute( &tblpPr, L"w:tblpY", FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;
			}
		}
		if (false == bTableW)
		{
            XMLTools::XMLElement tblW( L"w:tblW");

            XMLTools::XMLAttribute w( L"w:w", L"0");
			XMLTools::XMLAttribute type( L"w:type", L"auto" );

			tblW.AppendAttribute( type );
			tblW.AppendAttribute( w );

			_tblPr->AppendChild( tblW );
		}

	//indent
		if ( tblIndent )
		{
            XMLTools::XMLElement tblInd( L"w:tblInd");

            XMLTools::XMLAttribute tblIndW( L"w:w", FormatUtils::IntToWideString( *tblIndent ) );
			tblInd.AppendAttribute( tblIndW );

            XMLTools::XMLAttribute tblIndType( L"w:type", L"dxa");
			tblInd.AppendAttribute( tblIndType );

			_tblPr->AppendChild( tblInd );
		}

	//append floating props
		if ( tblpPr.GetAttributeCount() > 0 )
		{
			_tblPr->AppendChild( tblpPr );
		}

		//set borders
		if ( brcTop != NULL )
		{
            XMLTools::XMLElement topBorder( L"w:top");
			appendBorderAttributes( brcTop, &topBorder );
			addOrSetBorder( _tblBorders, &topBorder );
		}

		if ( brcLeft != NULL )
		{
            XMLTools::XMLElement leftBorder( L"w:left");
			appendBorderAttributes( brcLeft, &leftBorder );
			addOrSetBorder( _tblBorders, &leftBorder );
		}

		if ( brcBottom != NULL )
		{
            XMLTools::XMLElement bottomBorder( L"w:bottom");
			appendBorderAttributes( brcBottom, &bottomBorder );
			addOrSetBorder( _tblBorders, &bottomBorder );
		}

		if ( brcRight != NULL )
		{
            XMLTools::XMLElement rightBorder( L"w:right");
			appendBorderAttributes( brcRight, &rightBorder );
			addOrSetBorder( _tblBorders, &rightBorder );
		}

		if ( brcHorz != NULL )
		{
            XMLTools::XMLElement insideHBorder( L"w:insideH");
			appendBorderAttributes( brcHorz, &insideHBorder );
			addOrSetBorder( _tblBorders, &insideHBorder );
		}

		if ( brcVert != NULL )
		{
            XMLTools::XMLElement insideVBorder( L"w:insideV");
			appendBorderAttributes( brcVert, &insideVBorder );
			addOrSetBorder( _tblBorders, &insideVBorder );
		}

		if ( _tblBorders->GetChildCount() > 0 )
		{
			_tblPr->AppendChild( *_tblBorders );
		}

		//append layout type
		tblLayout.AppendAttribute( layoutType );
		_tblPr->AppendChild( tblLayout );

		//append margins
		if ( ( marginLeft == 0 ) && ( gabHalf != 0 ) )
		{
            appendDxaElement( &tblCellMar, L"left", FormatUtils::IntToWideString( gabHalf ), true );
		}
		else
		{
            appendDxaElement( &tblCellMar, L"left", FormatUtils::IntToWideString( marginLeft ), true );
		}

		if ( ( marginRight == 0 ) && ( gabHalf != 0 ) )
		{
            appendDxaElement( &tblCellMar, L"right", FormatUtils::IntToWideString( gabHalf ), true );
		}
		else
		{
            appendDxaElement( &tblCellMar, L"right", FormatUtils::IntToWideString( marginRight ), true );
		}

		_tblPr->AppendChild( tblCellMar );

		//write Properties
		if ( ( _tblPr->GetChildCount() > 0 ) || ( _tblPr->GetAttributeCount() > 0 ) )
		{
			m_pXmlWriter->WriteString( _tblPr->GetXMLString() );
		}

		//append the grid
        _tblGrid = new XMLTools::XMLElement( L"w:tblGrid");

		//Если _grid состоит из одних DocFormatUtils::gc_nZeroWidth и layout != "fixed", значит это doc полученный нами при конвертации из html. Таблицу размеров писать не нужно
		bool bWriteGridCol = false;
		if(true == bLayoutFixed)
			bWriteGridCol = true;
		else
		{
			for ( size_t i = 0, nSize = _grid->size(); i < nSize; i++ )
			{
				if(_grid->at(i) % DocFileFormat::gc_nZeroWidth != 0)
				{
					bWriteGridCol = true;
					break;
				}
			}
		}
		if(true == bWriteGridCol)
		{
			for ( size_t i = 0; i < _grid->size(); i++ )		
			{
                XMLTools::XMLElement gridCol( L"w:gridCol");
                XMLTools::XMLAttribute gridColW( L"w:w", FormatUtils::IntToWideString( _grid->at( i ) ) );
				gridCol.AppendAttribute( gridColW );
				_tblGrid->AppendChild( gridCol );
			}
		}

		m_pXmlWriter->WriteString( _tblGrid->GetXMLString() );
	}
}
