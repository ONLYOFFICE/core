/*
 * (c) Copyright Ascensio System SIA 2010-2017
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


#include "TableCellPropertiesMapping.h"

namespace DocFileFormat
{
	TableCellPropertiesMapping::TableCellPropertiesMapping (XMLTools::CStringXmlWriter* pWriter, const std::vector<short>* tableGrid, int gridIndex, int cellIndex) : 
										PropertiesMapping(pWriter)
	{
		_width		=	0;

		_gridIndex	=	gridIndex;
		_grid		=	tableGrid;
		_cellIndex	=	cellIndex;

		_brcTop		=	NULL;
		_brcLeft	=	NULL;
		_brcRight	=	NULL;
		_brcBottom	=	NULL;

        _tcPr		=	new XMLTools::XMLElement(L"w:tcPr");
        _tcMar		=	new XMLTools::XMLElement(L"w:tcMar");
        _tcBorders	=	new XMLTools::XMLElement(L"w:tcBorders");

		_ftsWidth	=	Global::nil;
	}

	TableCellPropertiesMapping::~TableCellPropertiesMapping()
	{
		RELEASEOBJECT(_tcPr);
		RELEASEOBJECT(_tcMar);
		RELEASEOBJECT(_tcBorders);
		RELEASEOBJECT(_brcTop);
		RELEASEOBJECT(_brcLeft);
		RELEASEOBJECT(_brcRight);
		RELEASEOBJECT(_brcBottom);
	}
}

namespace DocFileFormat
{
	void TableCellPropertiesMapping::Apply (IVisitable* visited)
	{
		TablePropertyExceptions* tapx	=	static_cast<TablePropertyExceptions*>(visited);
		int nComputedCellWidth			=	0;

		std::list<SinglePropertyModifier>::const_reverse_iterator rend = tapx->grpprl->rend();
		for (std::list<SinglePropertyModifier>::const_reverse_iterator iter = tapx->grpprl->rbegin(); iter != rend; ++iter)
		{
			switch (iter->OpCode)
			{				
				case sprmOldTDefTable:	
				case sprmTDefTable:	
				{
					SprmTDefTable tdef(iter->Arguments, iter->argumentsSize);
					int cc = tdef.numberOfColumns;

					_tGrid = tdef.rgdxaCenter;
                    _tcDef = tdef.rgTc80[(std::min)(_cellIndex,  (int)tdef.rgTc80.size() - 1)];	// NOTE: fix for crash

                    appendValueElement( _tcPr, L"textDirection", FormatUtils::MapValueToWideString( _tcDef.textFlow, &Global::TextFlowMap[0][0], 6, 6 ), false );

					if ( _tcDef.vertMerge == Global::fvmMerge )
					{
                        appendValueElement( _tcPr, L"vMerge", L"continue", false );
					}
					else if ( _tcDef.vertMerge == Global::fvmRestart )
					{
                        appendValueElement( _tcPr, L"vMerge", L"restart", false );
					}

                    appendValueElement( _tcPr, L"vAlign", FormatUtils::MapValueToWideString( _tcDef.vertAlign, &Global::VerticalAlignMap[0][0], 3, 7 ), false );

					if ( _tcDef.fFitText )
					{
                        appendValueElement( _tcPr, L"tcFitText", L"", false );
					}

					if ( _tcDef.fNoWrap )
					{
                        appendValueElement( _tcPr, L"noWrap", L"", true );
					}

                    nComputedCellWidth = (short)( tdef.rgdxaCenter[(size_t)(std::min)(_cellIndex,  (int)tdef.rgTc80.size() - 1) + 1] -
                        tdef.rgdxaCenter[(std::min)(_cellIndex, (int)tdef.rgTc80.size() - 1)] );	// NOTE: fix for crash

					if (!IsTableBordersDefined(tapx->grpprl))
					{											//borders

						RELEASEOBJECT(_brcTop);
						_brcTop = new BorderCode(*_tcDef.brcTop);

						RELEASEOBJECT(_brcLeft);
						_brcLeft = new BorderCode(*_tcDef.brcLeft);

						RELEASEOBJECT(_brcRight);
						_brcRight = new BorderCode(*_tcDef.brcRight);

						RELEASEOBJECT(_brcBottom);
						_brcBottom = new BorderCode(*_tcDef.brcBottom);
					}
				}
				break;

				case sprmTCellPadding:
				{							//margins

					unsigned char first		=	iter->Arguments[0];
					unsigned char lim		=	iter->Arguments[1];
					unsigned char ftsMargin	=	iter->Arguments[3];
					short wMargin	=	FormatUtils::BytesToInt16( iter->Arguments, 4, iter->argumentsSize );

					if ( ( _cellIndex >= first ) && ( _cellIndex < lim ) )
					{
						if ( FormatUtils::GetBitFromInt( iter->Arguments[2], 0 ) == true )
						{
                            appendDxaElement( _tcMar, L"top", FormatUtils::IntToWideString( wMargin ), true );
						}

						if ( FormatUtils::GetBitFromInt( iter->Arguments[2], 1 ) == true )
						{
                            appendDxaElement( _tcMar, L"left", FormatUtils::IntToWideString( wMargin ), true );
						}

						if (  FormatUtils::GetBitFromInt( iter->Arguments[2], 2 ) == true )
						{
                            appendDxaElement( _tcMar, L"bottom", FormatUtils::IntToWideString( wMargin ), true );
						}

						if ( FormatUtils::GetBitFromInt( iter->Arguments[2], 3 ) == true )
						{
                            appendDxaElement( _tcMar, L"right", FormatUtils::IntToWideString( wMargin ), true );
						}
					}
				}
				break;

				case sprmTDefTableShd80:
				{
					if (!tapx->IsSkipShading97())	// если такой операнд единственный то учитываем его, иначе скипаем его
					{
						apppendCellShading(iter->Arguments, iter->argumentsSize, _cellIndex);
					}
				}break;

				case sprmOldTDefTableShd:
				case sprmTDefTableShd:
				{						//	cell shading for cells 0-20
					apppendCellShading(iter->Arguments, iter->argumentsSize, _cellIndex);
				}break;

				case sprmTDefTableShd2nd:
				{						//	cell shading for cells 21-42
					apppendCellShading(iter->Arguments, iter->argumentsSize, (_cellIndex - 21));
				}break;

				case sprmTDefTableShd3rd:
				{						//	cell shading for cells 43-62
					apppendCellShading(iter->Arguments, iter->argumentsSize, (_cellIndex - 43));
				}break;

				case sprmTCellWidth:
				{				//width
					unsigned char first		=	iter->Arguments[0];
					unsigned char lim		=	iter->Arguments[1];

					if ((_cellIndex >= first) && (_cellIndex < lim))
					{
						_ftsWidth	=	(Global::CellWidthType)(iter->Arguments[2]);
						_width		=	FormatUtils::BytesToInt16(iter->Arguments, 3, iter->argumentsSize);
					}
				}
				break;
				
				case sprmTVertAlign:
				{							//vertical alignment
					unsigned char first		=	iter->Arguments[0];
					unsigned char lim		=	iter->Arguments[1];

					if ((_cellIndex >= first) && (_cellIndex < lim))
					{
                        appendValueElement(_tcPr, L"vAlign", FormatUtils::MapValueToWideString( (VerticalCellAlignment)iter->Arguments[2], &VerticalCellAlignmentMap[0][0], 3, 7 ), true );
					}
				}
				break;

				case sprmTFitText:
				{				//Autofit
					unsigned char first = iter->Arguments[0];
					unsigned char lim = iter->Arguments[1];

					if ( ( _cellIndex >= first ) && ( _cellIndex < lim ) )
					{
                        appendValueElement( _tcPr, L"tcFitText", FormatUtils::IntToWideString( iter->Arguments[2] ), true );
					}
				}
				break;

				case sprmOldTSetBrc:
				case sprmTSetBrc:
				{					//borders (cell definition)
					unsigned char min = iter->Arguments[0];
					unsigned char max = iter->Arguments[1];

					int bordersToApply = (int)( iter->Arguments[2] );

					if ( ( _cellIndex >= min ) && ( _cellIndex < max ) )
					{
						const int brcSize = 8;
						unsigned char brcBytes[brcSize];
						memcpy( brcBytes, ( iter->Arguments + 3 ), brcSize );

						if( FormatUtils::BitmaskToBool( bordersToApply, 0x01 ) )
						{
							RELEASEOBJECT( _brcTop );
							_brcTop = new BorderCode( brcBytes, brcSize );
						}

						if( FormatUtils::BitmaskToBool( bordersToApply, 0x02 ) )
						{
							RELEASEOBJECT( _brcLeft );
							_brcLeft = new BorderCode( brcBytes, brcSize );
						}

						if ( FormatUtils::BitmaskToBool( bordersToApply, 0x04 ) )
						{
							RELEASEOBJECT( _brcBottom );
							_brcBottom = new BorderCode( brcBytes, brcSize );
						}

						if ( FormatUtils::BitmaskToBool( bordersToApply, 0x08 ) )
						{
							RELEASEOBJECT( _brcRight );
							_brcRight = new BorderCode( brcBytes, brcSize );
						}
					}
				}
				break;
			}
		}

		//width
        XMLTools::XMLElement    tcW     ( L"w:tcW" );
        XMLTools::XMLAttribute  tcWType ( L"w:type", FormatUtils::MapValueToWideString( _ftsWidth, &Global::CellWidthTypeMap[0][0], 4, 5 ) );
        XMLTools::XMLAttribute  tcWVal  ( L"w:w", FormatUtils::IntToWideString( _width ) );

        tcW.AppendAttribute( tcWType );
		tcW.AppendAttribute( tcWVal );
		_tcPr->AppendChild( tcW );

		//grid span
		_gridSpan = 1;

		if ( ( _gridIndex < (int)_grid->size() ) && ( nComputedCellWidth > _grid->at( _gridIndex ) ) )
		{
			//check the number of merged cells
			int w = _grid->at( _gridIndex );

			for ( unsigned int i = _gridIndex + 1; i < _grid->size(); i++ )
			{
				_gridSpan++;

				w += _grid->at( i );

				if ( w >= nComputedCellWidth )
				{
					break;
				}
			}

            appendValueElement( _tcPr, L"gridSpan", FormatUtils::IntToWideString( _gridSpan ), true );
		}

		//append margins
		if (_tcMar->GetChildCount() > 0)
		{
			_tcPr->AppendChild( *(_tcMar) );
		}

		//append borders
		if (_brcTop)
		{
            XMLTools::XMLElement topBorder( L"w:top" );
			appendBorderAttributes(_brcTop, &topBorder);
			addOrSetBorder(_tcBorders, &topBorder );
		}

		if (_brcLeft )
		{
            XMLTools::XMLElement leftBorder( L"w:left" );
			appendBorderAttributes(_brcLeft, &leftBorder);
			addOrSetBorder(_tcBorders, &leftBorder);
		}

		if (_brcBottom)
		{
            XMLTools::XMLElement bottomBorder( L"w:bottom" );
			appendBorderAttributes(_brcBottom, &bottomBorder);
			addOrSetBorder(_tcBorders, &bottomBorder);
		}

		if (_brcRight)
		{
            XMLTools::XMLElement rightBorder( L"w:right" );
			appendBorderAttributes( _brcRight, &rightBorder );
			addOrSetBorder( _tcBorders, &rightBorder );
		}

		if ( _tcBorders->GetChildCount() > 0 )
		{
			_tcPr->AppendChild( *(_tcBorders) );
		}

		//write Properties
		if ((_tcPr->GetChildCount() > 0) || (_tcPr->GetAttributeCount() > 0))
			m_pXmlWriter->WriteString(_tcPr->GetXMLString());
	}

	void TableCellPropertiesMapping::apppendCellShading (unsigned char* sprmArg, int size, int cellIndex)
	{
		if (sprmArg)
		{
			//shading descriptor can have 10 bytes (Word 2000) or 2 bytes (Word 97)
			int shdLength = 2;

			if ( ( size % 10 ) == 0 )
				shdLength = 10;

			unsigned char* shdBytes	=	new unsigned char[shdLength];

			if (shdBytes)
			{
				if ( ( cellIndex * shdLength ) < size )
				{
					memcpy (shdBytes, sprmArg + (cellIndex * shdLength), shdLength);

					ShadingDescriptor shd (shdBytes, shdLength);
					appendShading (_tcPr, shd);
				}

				RELEASEARRAYOBJECTS(shdBytes);
			}
		}
	}

	bool TableCellPropertiesMapping::IsTableBordersDefined (const std::list<SinglePropertyModifier>* grpprl) const
	{
		if (grpprl)
		{
			std::list<SinglePropertyModifier>::const_iterator end = grpprl->end();
			for (std::list<SinglePropertyModifier>::const_iterator iter = grpprl->begin(); iter != end; ++iter)
			{
				if ((iter->OpCode == sprmTTableBorders) || (iter->OpCode == sprmTTableBorders80) || (iter->OpCode == sprmTSetBrc))
					return true;
			}
		}

		return false;
	}

	bool TableCellPropertiesMapping::IsTableCellWidthDefined (const std::list<SinglePropertyModifier>* grpprl) const
	{
		if (grpprl)
		{
			std::list<SinglePropertyModifier>::const_iterator end = grpprl->end();
			for (std::list<SinglePropertyModifier>::const_iterator iter = grpprl->begin(); iter != end; ++iter)
			{
				if ( iter->OpCode == sprmTCellWidth )
				{
					unsigned char first	=	iter->Arguments[0];
					unsigned char lim	=	iter->Arguments[1];

					if ((_cellIndex >= first) && (_cellIndex < lim))
						return true;
				}
			}
		}

		return false;  
	}
}
