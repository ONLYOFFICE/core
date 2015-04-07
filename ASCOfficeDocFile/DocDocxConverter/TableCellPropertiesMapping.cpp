

#include "TableCellPropertiesMapping.h"

namespace DocFileFormat
{
	TableCellPropertiesMapping::TableCellPropertiesMapping (XmlUtils::CXmlWriter* pWriter, const vector<short>* tableGrid, int gridIndex, int cellIndex) : PropertiesMapping(pWriter)
	{
		_width		=	0;

		_gridIndex	=	gridIndex;
		_grid		=	tableGrid;
		_cellIndex	=	cellIndex;

		_brcTop		=	NULL;
		_brcLeft	=	NULL;
		_brcRight	=	NULL;
		_brcBottom	=	NULL;

		_tcPr		=	new XMLTools::XMLElement<wchar_t>(_T("w:tcPr"));
		_tcMar		=	new XMLTools::XMLElement<wchar_t>(_T("w:tcMar"));
		_tcBorders	=	new XMLTools::XMLElement<wchar_t>(_T("w:tcBorders"));

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
#ifdef _DEBUG
			SinglePropertyModifier spm	= (*iter);
#endif
			switch (iter->OpCode)
			{				
			case sprmTDefTable:	// Table definition SPRM
				{
					SprmTDefTable tdef(iter->Arguments, iter->argumentsSize);
					int cc = tdef.numberOfColumns;

					this->_tGrid = tdef.rgdxaCenter;
					this->_tcDef = tdef.rgTc80[min(_cellIndex,  (int)tdef.rgTc80.size() - 1)];	// NOTE: fix for crash

					appendValueElement( this->_tcPr, _T( "textDirection" ), FormatUtils::MapValueToWideString( this->_tcDef.textFlow, &Global::TextFlowMap[0][0], 6, 6 ).c_str(), false );

					if ( this->_tcDef.vertMerge == Global::fvmMerge )
					{
						appendValueElement( this->_tcPr, _T( "vMerge" ), _T( "continue" ), false );
					}
					else if ( this->_tcDef.vertMerge == Global::fvmRestart )
					{
						appendValueElement( this->_tcPr, _T( "vMerge" ), _T( "restart" ), false );
					}

					appendValueElement( this->_tcPr, _T( "vAlign" ), FormatUtils::MapValueToWideString( this->_tcDef.vertAlign, &Global::VerticalAlignMap[0][0], 3, 7 ).c_str(), false );

					if ( this->_tcDef.fFitText )
					{
						appendValueElement( _tcPr, _T( "tcFitText" ), _T( "" ), false );
					}

					if ( this->_tcDef.fNoWrap )
					{
						appendValueElement( _tcPr, _T( "noWrap" ), _T( "" ), true );
					}

					nComputedCellWidth = (short)( tdef.rgdxaCenter[(size_t)min(_cellIndex,  (int)tdef.rgTc80.size() - 1) + 1] -
						tdef.rgdxaCenter[min(_cellIndex, (int)tdef.rgTc80.size() - 1)] );	// NOTE: fix for crash

					//borders
					if (!IsTableBordersDefined(tapx->grpprl))
					{
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

				//margins
			case sprmTCellPadding:
				{
					unsigned char first		=	iter->Arguments[0];
					unsigned char lim		=	iter->Arguments[1];
					unsigned char ftsMargin	=	iter->Arguments[3];
					short wMargin	=	FormatUtils::BytesToInt16( iter->Arguments, 4, iter->argumentsSize );

					if ( ( this->_cellIndex >= first ) && ( this->_cellIndex < lim ) )
					{
						if ( FormatUtils::GetBitFromInt( iter->Arguments[2], 0 ) == true )
						{
							appendDxaElement( this->_tcMar, _T( "top" ), FormatUtils::IntToWideString( wMargin ).c_str(), true );
						}

						if ( FormatUtils::GetBitFromInt( iter->Arguments[2], 1 ) == true )
						{
							appendDxaElement( this->_tcMar, _T( "left" ), FormatUtils::IntToWideString( wMargin ).c_str(), true );
						}

						if (  FormatUtils::GetBitFromInt( iter->Arguments[2], 2 ) == true )
						{
							appendDxaElement( this->_tcMar, _T( "bottom" ), FormatUtils::IntToWideString( wMargin ).c_str(), true );
						}

						if ( FormatUtils::GetBitFromInt( iter->Arguments[2], 3 ) == true )
						{
							appendDxaElement( this->_tcMar, _T( "right" ), FormatUtils::IntToWideString( wMargin ).c_str(), true );
						}
					}
				}
				break;

			case sprmTDefTableShd80:
				if (!tapx->IsSkipShading97())	// если такой операнд единственный то учитываем его, иначе скипаем его
				{
					apppendCellShading(iter->Arguments, iter->argumentsSize, _cellIndex);
				}
				break;

				//	shading

			case sprmTDefTableShd:
				//	cell shading for cells 0-20
				apppendCellShading(iter->Arguments, iter->argumentsSize, _cellIndex);
				break;

			case sprmTDefTableShd2nd:
				//	cell shading for cells 21-42
				apppendCellShading(iter->Arguments, iter->argumentsSize, (_cellIndex - 21));
				break;

			case sprmTDefTableShd3rd:
				//	cell shading for cells 43-62
				apppendCellShading(iter->Arguments, iter->argumentsSize, (_cellIndex - 43));
				break;

				//width
			case sprmTCellWidth:
				{
					unsigned char first		=	iter->Arguments[0];
					unsigned char lim		=	iter->Arguments[1];

					if ((_cellIndex >= first) && (_cellIndex < lim))
					{
						_ftsWidth	=	(Global::CellWidthType)(iter->Arguments[2]);
						_width		=	FormatUtils::BytesToInt16(iter->Arguments, 3, iter->argumentsSize);
					}
				}
				break;

				//vertical alignment
			case sprmTVertAlign:
				{
					unsigned char first		=	iter->Arguments[0];
					unsigned char lim		=	iter->Arguments[1];

					if ((_cellIndex >= first) && (_cellIndex < lim))
					{
						appendValueElement(_tcPr, _T( "vAlign" ), FormatUtils::MapValueToWideString( (VerticalCellAlignment)iter->Arguments[2], &VerticalCellAlignmentMap[0][0], 3, 7 ).c_str(), true );
					}
				}
				break;

				//Autofit
			case sprmTFitText:
				{
					unsigned char first = iter->Arguments[0];
					unsigned char lim = iter->Arguments[1];

					if ( ( this->_cellIndex >= first ) && ( this->_cellIndex < lim ) )
					{
						appendValueElement( this->_tcPr, _T( "tcFitText" ), FormatUtils::IntToWideString( iter->Arguments[2] ).c_str(), true );
					}
				}
				break;

				//borders (cell definition)
			case sprmTSetBrc:
				{
					unsigned char min = iter->Arguments[0];
					unsigned char max = iter->Arguments[1];

					int bordersToApply = (int)( iter->Arguments[2] );

					if ( ( this->_cellIndex >= min ) && ( this->_cellIndex < max ) )
					{
						const int brcSize = 8;
						unsigned char brcBytes[brcSize];
						memcpy( brcBytes, ( iter->Arguments + 3 ), brcSize );

						if( FormatUtils::BitmaskToBool( bordersToApply, 0x01 ) )
						{
							RELEASEOBJECT( this->_brcTop );
							this->_brcTop = new BorderCode( brcBytes, brcSize );
						}

						if( FormatUtils::BitmaskToBool( bordersToApply, 0x02 ) )
						{
							RELEASEOBJECT( this->_brcLeft );
							this->_brcLeft = new BorderCode( brcBytes, brcSize );
						}

						if ( FormatUtils::BitmaskToBool( bordersToApply, 0x04 ) )
						{
							RELEASEOBJECT( this->_brcBottom );
							this->_brcBottom = new BorderCode( brcBytes, brcSize );
						}

						if ( FormatUtils::BitmaskToBool( bordersToApply, 0x08 ) )
						{
							RELEASEOBJECT( this->_brcRight );
							_brcRight = new BorderCode( brcBytes, brcSize );
						}
					}
				}
				break;
			}
		}

		//width
		XMLTools::XMLElement<wchar_t> tcW( _T( "w:tcW" ) );
		XMLTools::XMLAttribute<wchar_t> tcWType( _T( "w:type" ), FormatUtils::MapValueToWideString( _ftsWidth, &Global::CellWidthTypeMap[0][0], 4, 5 ).c_str() );
		XMLTools::XMLAttribute<wchar_t> tcWVal( _T( "w:w" ), FormatUtils::IntToWideString( _width ).c_str() );
		tcW.AppendAttribute( tcWType );
		tcW.AppendAttribute( tcWVal );
		_tcPr->AppendChild( tcW );

		//grid span
		this->_gridSpan = 1;

		if ( ( this->_gridIndex < (int)this->_grid->size() ) && ( nComputedCellWidth > this->_grid->at( this->_gridIndex ) ) )
		{
			//check the number of merged cells
			int w = this->_grid->at( this->_gridIndex );

			for ( unsigned int i = this->_gridIndex + 1; i < this->_grid->size(); i++ )
			{
				this->_gridSpan++;

				w += this->_grid->at( i );

				if ( w >= nComputedCellWidth )
				{
					break;
				}
			}

			appendValueElement( this->_tcPr, _T( "gridSpan" ), FormatUtils::IntToWideString( this->_gridSpan ).c_str(), true );
		}

		//append margins
		if (_tcMar->GetChildCount() > 0)
		{
			_tcPr->AppendChild( *(_tcMar) );
		}

		//append borders
		if (_brcTop)
		{
			XMLTools::XMLElement<wchar_t> topBorder(_T("w:top"));
			appendBorderAttributes(_brcTop, &topBorder);
			addOrSetBorder(_tcBorders, &topBorder );
		}

		if (_brcLeft )
		{
			XMLTools::XMLElement<wchar_t> leftBorder(_T("w:left"));
			appendBorderAttributes(_brcLeft, &leftBorder);
			addOrSetBorder(_tcBorders, &leftBorder);
		}

		if (_brcBottom)
		{
			XMLTools::XMLElement<wchar_t> bottomBorder(_T("w:bottom"));
			appendBorderAttributes(_brcBottom, &bottomBorder);
			addOrSetBorder(_tcBorders, &bottomBorder);
		}

		if (_brcRight)
		{
			XMLTools::XMLElement<wchar_t> rightBorder( _T( "w:right" ) );
			appendBorderAttributes( this->_brcRight, &rightBorder );
			addOrSetBorder( this->_tcBorders, &rightBorder );
		}

		if ( this->_tcBorders->GetChildCount() > 0 )
		{
			this->_tcPr->AppendChild( *(this->_tcBorders) );
		}

		//write Properties
		if ((_tcPr->GetChildCount() > 0) || (_tcPr->GetAttributeCount() > 0))
			m_pXmlWriter->WriteString(_tcPr->GetXMLString().c_str());
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

	bool TableCellPropertiesMapping::IsTableBordersDefined (const list<SinglePropertyModifier>* grpprl) const
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

	bool TableCellPropertiesMapping::IsTableCellWidthDefined (const list<SinglePropertyModifier>* grpprl) const
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