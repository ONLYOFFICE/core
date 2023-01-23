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


#include "TableCellPropertiesMapping.h"

namespace DocFileFormat
{
	TableCellPropertiesMapping::TableCellPropertiesMapping (XMLTools::CStringXmlWriter* pWriter, 
		const std::vector<short>* grid, int gridIndex, int cellIndex, unsigned int depth) : 
										PropertiesMapping(pWriter)
	{
		_depth		=	depth;
		_width		=	0;

		_gridIndex	=	gridIndex;
		_grid		=	grid;
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
	}
}

namespace DocFileFormat
{
	void TableCellPropertiesMapping::Apply (IVisitable* visited)
	{
		TablePropertyExceptions* tapx	=	static_cast<TablePropertyExceptions*>(visited);
		int nComputedCellWidth = 0, nComputedCellWidths = 0;

		_gridSpan = 0;
		_bCoverCell = false;

		unsigned int iTap_current = 1;

		for ( std::list<SinglePropertyModifier>::iterator iter = tapx->grpprl->begin(); iter != tapx->grpprl->end(); iter++ )
		{
			DWORD code = iter->OpCode;

			switch(iter->OpCode)
			{
				case sprmPItap:
				{
					iTap_current = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );
				}break;
				default:
					break;
			}
		}

		bool bPresentDefTable = false;
		for (std::list<SinglePropertyModifier>::reverse_iterator iter = tapx->grpprl->rbegin(); iter != tapx->grpprl->rend(); ++iter)
		{
			switch (iter->OpCode)
			{				
				case sprmOldTDefTable:	
				case sprmTDefTable:	
				{
					if (!bPresentDefTable) //118854.doc
					{
						bPresentDefTable = true;

						SprmTDefTable tdef(iter->Arguments, iter->argumentsSize);

						bool bUseWidth = true;

						for (size_t j = 0; j < tdef.rgTc80.size(); ++j)
						{ // 1bc0f6c0-b226-4bcb-912c-e7f97b009d8a.doc
						  // Технические_Требования_1_287_ДИТ.DOC
							if (tdef.rgTc80[j].horzMerge == 0 && tdef.rgTc80[j].wWidth < 1)
							{
								bUseWidth = false; 
								break;
							}
						}
						int cc = tdef.numberOfColumns;

						_tGrid = tdef.rgdxaCenter;
						_tcDef = tdef.rgTc80[(std::min)(_cellIndex,  (int)tdef.rgTc80.size() - 1)];	

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
						//int ind = (std::min)(_cellIndex,  (int)tdef.rgTc80.size() - 1);
						//int ind1 = ind; 
						//while (ind1 < tdef.rgdxaCenter.size() - 1)
						//{
						//	int sz = tdef.rgdxaCenter[ ind1 + 1] - tdef.rgdxaCenter[ ind1 ] ;
						//	if (sz > 1)
						//		break;
						//	ind1++;
						//}
						_ftsWidth = (Global::CellWidthType)(_tcDef.ftsWidth);

						if (_cellIndex < tdef.rgTc80.size())
						{
							if (tdef.rgTc80[_cellIndex].horzMerge == 1)
							{
								for (size_t i = _cellIndex; i < tdef.rgTc80.size(); i++)
								{
									if (tdef.rgTc80[i].horzMerge < 1)
										break;

									nComputedCellWidth += tdef.rgdxaCenter[i + 1] - tdef.rgdxaCenter[i];
									_gridSpan++;
								}
							}
							else if (tdef.rgTc80[_cellIndex].horzMerge == 2)
							{//skip cover cell
								_gridSpan = 1;
								nComputedCellWidth = 0;
								_bCoverCell = true;

							}
							else
							{
								_gridSpan = 1;

								nComputedCellWidths += (tdef.rgdxaCenter[_cellIndex + 1] - tdef.rgdxaCenter[0]);
								nComputedCellWidth += bUseWidth ? tdef.rgTc80[_cellIndex].wWidth :
									(tdef.rgdxaCenter[_cellIndex + 1] - tdef.rgdxaCenter[_cellIndex]);
							}
						}
						else
						{

						}

						if (!IsTableBordersDefined(tapx->grpprl))
						{
							_brcTop = std::shared_ptr<BorderCode>(new BorderCode(*_tcDef.brcTop));
							_brcLeft = std::shared_ptr<BorderCode>(new BorderCode(*_tcDef.brcLeft));
							_brcRight = std::shared_ptr<BorderCode>(new BorderCode(*_tcDef.brcRight));
							_brcBottom = std::shared_ptr<BorderCode>(new BorderCode(*_tcDef.brcBottom));
						}
					}
				}
				break;
				case sprmTDxaCol:
				{
				}break;
				case sprmTCellPadding:
				{
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
				{ //	cell shading for cells 0-20
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
				{ //width
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
				{ //vertical alignment
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
				{ //borders (cell definition)
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
							_brcTop = std::shared_ptr<BorderCode>(new BorderCode( brcBytes, brcSize ));
						}
						if( FormatUtils::BitmaskToBool( bordersToApply, 0x02 ) )
						{
							_brcLeft = std::shared_ptr<BorderCode>(new BorderCode( brcBytes, brcSize ));
						}
						if ( FormatUtils::BitmaskToBool( bordersToApply, 0x04 ) )
						{
							_brcBottom = std::shared_ptr<BorderCode>(new BorderCode( brcBytes, brcSize ));
						}
						if ( FormatUtils::BitmaskToBool( bordersToApply, 0x08 ) )
						{
							_brcRight = std::shared_ptr<BorderCode>(new BorderCode( brcBytes, brcSize ));
						}
					}
				}break;
				
				default:
					break;
			}
		}
		int nComputedCellWidthsGrid = 0;

		for (size_t ccc = 0; ccc < _grid->size() && ccc <= _gridIndex; ccc++)
		{
			nComputedCellWidthsGrid += _grid->at(ccc);
		}//zadost.doc
		if (_gridSpan <= 1 && nComputedCellWidth > _width/* && _width > 1*/ && nComputedCellWidths > nComputedCellWidthsGrid)
		{
			int width_current = 0;
			for (int i = _gridIndex; i < _grid->size();  i++)
			{
				width_current += _grid->at(i);
				if (width_current >= nComputedCellWidth)
					break;
				_gridSpan++;
			}
			_width = nComputedCellWidth;
		}

		XMLTools::XMLElement tcW ( L"w:tcW" );
	
		if (_width > 1 || nComputedCellWidth > 0)
		{
			if (_ftsWidth == Global::CellWidthType::nil)
				_ftsWidth = Global::CellWidthType::dxa;
		}
		else _ftsWidth = Global::CellWidthType::Auto;
	
		XMLTools::XMLAttribute  tcWVal  ( L"w:w", FormatUtils::IntToWideString( _width > 1 ? _width : nComputedCellWidth) );
		XMLTools::XMLAttribute  tcWType ( L"w:type", FormatUtils::MapValueToWideString(_ftsWidth, &Global::CellWidthTypeMap[0][0], 4, 5 ));

		tcW.AppendAttribute( tcWVal );
		tcW.AppendAttribute( tcWType );
		_tcPr->AppendChild( tcW );

		if ( _gridSpan == 1 && ( _gridIndex < (int)_grid->size() ) && ( nComputedCellWidths > nComputedCellWidthsGrid ) )
		{
			//check the number of merged cells
			for ( size_t i = _gridIndex + 1; i < _grid->size(); i++ )
			{
				_gridSpan++;

				nComputedCellWidthsGrid += _grid->at( i );

				if ( nComputedCellWidthsGrid >= nComputedCellWidths )
				{
					break;
				}
			}
		}
		if (_gridSpan > 1)
		{
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
			appendBorderAttributes(_brcTop.get(), &topBorder);
			addOrSetBorder(_tcBorders, &topBorder );
		}

		if (_brcLeft )
		{
            XMLTools::XMLElement leftBorder( L"w:left" );
			appendBorderAttributes(_brcLeft.get(), &leftBorder);
			addOrSetBorder(_tcBorders, &leftBorder);
		}

		if (_brcBottom)
		{
            XMLTools::XMLElement bottomBorder( L"w:bottom" );
			appendBorderAttributes(_brcBottom.get(), &bottomBorder);
			addOrSetBorder(_tcBorders, &bottomBorder);
		}

		if (_brcRight)
		{
            XMLTools::XMLElement rightBorder( L"w:right" );
			appendBorderAttributes( _brcRight.get(), &rightBorder );
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
		if (sprmArg && cellIndex >= 0)
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
