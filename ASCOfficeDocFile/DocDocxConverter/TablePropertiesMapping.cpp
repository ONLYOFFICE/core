#include "stdafx.h"
#include "TablePropertiesMapping.h"

namespace DocFileFormat
{
	TablePropertiesMapping::TablePropertiesMapping (XmlUtils::CXmlWriter* pWriter, StyleSheet* styles, vector<short>* grid, bool isTableStyleNeeded ):
PropertiesMapping(pWriter), _tblPr(NULL), _tblGrid(NULL), _tblBorders(NULL), _grid(NULL),
brcLeft(NULL), brcTop(NULL), brcBottom(NULL), brcRight(NULL), brcHorz(NULL), brcVert(NULL), _styles(NULL),
_isTableStyleNeeded(isTableStyleNeeded)
{
	_styles = styles;
	this->_tblPr = new XMLTools::XMLElement<WCHAR>( _T( "w:tblPr" ) );
	this->_tblBorders = new XMLTools::XMLElement<WCHAR>( _T( "w:tblBorders" ) );
	this->_grid = grid;
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

		XMLTools::XMLElement<WCHAR> tblCellMar( _T( "w:tblCellMar" ) );
		XMLTools::XMLElement<WCHAR> tblLayout( _T( "w:tblLayout" ) );
		XMLTools::XMLElement<WCHAR> tblpPr( _T( "w:tblpPr" ) );
		XMLTools::XMLAttribute<WCHAR> layoutType( _T( "w:type" ), _T( "fixed" ) );
		bool bLayoutFixed = true;

		short tblIndent = 0;
		short gabHalf = 0;
		short marginLeft = 0;
		short marginRight = 0;

		for ( list<SinglePropertyModifier>::iterator iter = tapx->grpprl->begin(); iter != tapx->grpprl->end(); iter++ )
		{
			switch( iter->OpCode )
			{
			case sprmTDxaGapHalf:
				{
					gabHalf = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
				}
				break;

				//table definition
			case sprmTDefTable:
				{
					SprmTDefTable tDef( iter->Arguments, iter->argumentsSize );
					//Workaround for retrieving the indent of the table:
					//In some files there is a indent but no sprmTWidthIndent is set.
					//For this cases we can calculate the indent of the table by getting the 
					//first boundary of the TDef and adding the padding of the cells
					tblIndent = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );
					//add the gabHalf
					tblIndent += gabHalf;
					//If there follows a real sprmTWidthIndent, this value will be overwritten

					///<<<<FIXED
					tblIndent = max(tblIndent,0);
				}
				break;

				//preferred table width
			case sprmTTableWidth:
				{
					byte fts = iter->Arguments[0];
					short width = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );

					XMLTools::XMLElement<WCHAR> tblW( _T( "w:tblW" ) );

					XMLTools::XMLAttribute<WCHAR> w( _T( "w:w" ), FormatUtils::IntToWideString( width ).c_str() );
					XMLTools::XMLAttribute<WCHAR> type( _T( "w:type" ), FormatUtils::MapValueToWideString( fts, &WidthType[0][0], 4, 5 ).c_str() );

					tblW.AppendAttribute( type );
					tblW.AppendAttribute( w );

					_tblPr->AppendChild( tblW );
				}
				break;

				//justification
			case sprmTJc:
			case sprmTJcRow:
				{
					appendValueElement( _tblPr, _T( "jc" ), FormatUtils::MapValueToWideString( iter->Arguments[0], &Global::JustificationCode[0][0], 10, 15 ).c_str(), true );
				}
				break;

				//indent
			case sprmTWidthIndent:
				{
					tblIndent	=	FtsWWidth_Indent(iter->Arguments).wWidth;
					// tblIndent	=	FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );
				}
				break;

				//style
			case sprmTIstd:
			case sprmTIstdPermute:
				{
					if ( this->_isTableStyleNeeded )
					{
						wstring id = StyleSheetMapping::MakeStyleId( _styles->Styles->at( FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ) );

						if( id != wstring( _T( "TableNormal" ) ) )
						{
							appendValueElement( _tblPr, _T( "tblStyle" ), id.c_str(), true );
						}
					}
				}
				break;

				//bidi
			case sprmTFBiDi:
			case sprmTFBiDi90:
				{
					appendValueElement( _tblPr, _T( "bidiVisual" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ), true );
				}
				break;

				//table look
			case sprmTTlp:
				{
					appendValueElement( _tblPr, _T( "tblLook" ), FormatUtils::IntToFormattedWideString( FormatUtils::BytesToInt16( iter->Arguments, 2, iter->argumentsSize ), _T( "%04x" ) ).c_str(), true );
				}
				break;

				//autofit
			case sprmTFAutofit:
				{
					if ( iter->Arguments[0] == 1 )
					{
						layoutType.SetValue( _T( "auto" ) );
						bLayoutFixed = false;
					}
				}
				break;

				//default cell padding (margin)
			case sprmTCellPadding:
			case sprmTCellPaddingDefault:
			case sprmTCellPaddingOuter:
				{
					byte grfbrc = iter->Arguments[2];
					short wMar = FormatUtils::BytesToInt16( iter->Arguments, 4, iter->argumentsSize );
					wstring strValue = FormatUtils::IntToWideString( wMar );

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x01 ) )
					{
						appendDxaElement( &tblCellMar,  _T( "top" ), strValue.c_str(), true );
					}

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x02 ) )
					{
						marginLeft = wMar;
					}

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x04 ) )
					{
						appendDxaElement( &tblCellMar, _T( "bottom" ), strValue.c_str(), true );
					}

					if ( FormatUtils::BitmaskToBool( (int)grfbrc, 0x08 ) )
					{
						marginRight = wMar;
					}
				}
				break;

				//row count
			case sprmTCHorzBands:
				{
					appendValueElement( _tblPr, _T( "tblStyleRowBandSize" ), iter->Arguments[0], true );
				}
				break;

				//col count
			case sprmTCVertBands:
				{
					appendValueElement( _tblPr, _T( "tblStyleColBandSize" ), iter->Arguments[0], true );
				}
				break;

				//overlap
			case sprmTFNoAllowOverlap:
				{
					wstring tblOverlapVal = wstring( _T( "overlap" ) );

					if ( iter->Arguments[0] )
					{
						tblOverlapVal = wstring( _T( "never" ) );
					}

					appendValueElement( _tblPr, _T( "tblOverlap" ), tblOverlapVal.c_str(), true );
				}
				break;

				//shading
			case sprmTSetShdTable:
				{
					appendShading( _tblPr, ShadingDescriptor( iter->Arguments, iter->argumentsSize ) );
				}
				break;

				//borders 80 exceptions
			case sprmTTableBorders80:
				{
					const int size = 4;
					byte brc80[size];

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
			case sprmTTableBorders:
				{
					const int size = 8;
					byte brc[size];

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
					byte flag = ( iter->Arguments[0] & 0x30 ) >> 4;

					appendValueAttribute( &tblpPr, _T( "w:vertAnchor" ), FormatUtils::MapValueToWideString( flag, &Global::VerticalPositionCode[0][0], 4, 7 ).c_str() );

					flag = ( iter->Arguments[0] & 0xC0 ) >> 6;

					appendValueAttribute( &tblpPr, _T( "w:horzAnchor" ), FormatUtils::MapValueToWideString( flag, &Global::HorizontalPositionCode[0][0], 4, 7 ).c_str() );
				}
				break;

			case sprmTDxaFromText:
				{
					appendValueAttribute( &tblpPr, _T( "w:leftFromText" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

			case sprmTDxaFromTextRight:
				{
					appendValueAttribute( &tblpPr, _T( "w:rightFromText" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

			case sprmTDyaFromText:
				{
					appendValueAttribute( &tblpPr, _T( "w:topFromText" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

			case sprmTDyaFromTextBottom:
				{
					appendValueAttribute( &tblpPr, _T( "w:bottomFromText" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}			
				break;

			case sprmTDxaAbs:
				{
					appendValueAttribute( &tblpPr, _T( "w:tblpX" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;

			case sprmTDyaAbs:
				{
					appendValueAttribute( &tblpPr, _T( "w:tblpY" ), FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) );
				}
				break;
			}
		}

		//indent
		if ( tblIndent != 0 )
		{
			XMLTools::XMLElement<WCHAR> tblInd( _T( "w:tblInd" ) );

			XMLTools::XMLAttribute<WCHAR> tblIndW( _T( "w:w" ),FormatUtils::IntToWideString( tblIndent ).c_str() );
			tblInd.AppendAttribute( tblIndW );

			XMLTools::XMLAttribute<WCHAR> tblIndType( _T( "w:type" ), _T( "dxa" ) );
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
			XMLTools::XMLElement<WCHAR> topBorder( _T( "w:top" ) );
			appendBorderAttributes( brcTop, &topBorder );
			addOrSetBorder( _tblBorders, &topBorder );
		}

		if ( brcLeft != NULL )
		{
			XMLTools::XMLElement<WCHAR> leftBorder( _T( "w:left" ) );
			appendBorderAttributes( brcLeft, &leftBorder );
			addOrSetBorder( _tblBorders, &leftBorder );
		}

		if ( brcBottom != NULL )
		{
			XMLTools::XMLElement<WCHAR> bottomBorder( _T( "w:bottom" ) );
			appendBorderAttributes( brcBottom, &bottomBorder );
			addOrSetBorder( _tblBorders, &bottomBorder );
		}

		if ( brcRight != NULL )
		{
			XMLTools::XMLElement<WCHAR> rightBorder( _T( "w:right" ) );
			appendBorderAttributes( brcRight, &rightBorder );
			addOrSetBorder( _tblBorders, &rightBorder );
		}

		if ( brcHorz != NULL )
		{
			XMLTools::XMLElement<WCHAR> insideHBorder( _T( "w:insideH" ) );
			appendBorderAttributes( brcHorz, &insideHBorder );
			addOrSetBorder( _tblBorders, &insideHBorder );
		}

		if ( brcVert != NULL )
		{
			XMLTools::XMLElement<WCHAR> insideVBorder( _T( "w:insideV" ) );
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
			appendDxaElement( &tblCellMar, _T( "left" ), FormatUtils::IntToWideString( gabHalf ).c_str(), true );
		}
		else
		{
			appendDxaElement( &tblCellMar, _T( "left" ), FormatUtils::IntToWideString( marginLeft ).c_str(), true );
		}

		if ( ( marginRight == 0 ) && ( gabHalf != 0 ) )
		{
			appendDxaElement( &tblCellMar, _T( "right" ), FormatUtils::IntToWideString( gabHalf ).c_str(), true );
		}
		else
		{
			appendDxaElement( &tblCellMar, _T( "right" ), FormatUtils::IntToWideString( marginRight ).c_str(), true );
		}

		_tblPr->AppendChild( tblCellMar );

		//write Properties
		if ( ( _tblPr->GetChildCount() > 0 ) || ( _tblPr->GetAttributeCount() > 0 ) )
		{
			m_pXmlWriter->WriteString( _tblPr->GetXMLString().c_str() );
		}

		//append the grid
		_tblGrid = new XMLTools::XMLElement<WCHAR>( _T( "w:tblGrid" ) );

		//Если _grid состоит из одних ASCDocFormatUtils::gc_nZeroWidth и layout != "fixed", значит это doc полученный нами при конвертации из html. Таблицу размеров писать не нужно
		bool bWriteGridCol = false;
		if(true == bLayoutFixed)
			bWriteGridCol = true;
		else
		{
			for ( unsigned int i = 0, nSize = _grid->size(); i < nSize; i++ )
			{
				if(_grid->at(i) % ASCDocFormatUtils::gc_nZeroWidth != 0)
				{
					bWriteGridCol = true;
					break;
				}
			}
		}
		if(true == bWriteGridCol)
		{
			for ( unsigned int i = 0; i < _grid->size(); i++ )		
			{
				XMLTools::XMLElement<WCHAR> gridCol( _T( "w:gridCol" ) );
				XMLTools::XMLAttribute<WCHAR> gridColW( _T( "w:w" ), FormatUtils::IntToWideString( _grid->at( i ) ).c_str() );
				gridCol.AppendAttribute( gridColW );
				_tblGrid->AppendChild( gridCol );
			}
		}

		m_pXmlWriter->WriteString( _tblGrid->GetXMLString().c_str() );
	}
}