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


#include "TableRowPropertiesMapping.h"

namespace DocFileFormat
{
	TableRowPropertiesMapping::TableRowPropertiesMapping (XMLTools::CStringXmlWriter* pWriter, CharacterPropertyExceptions* rowEndChpx) : PropertiesMapping(pWriter), _trPr(NULL), _tblPrEx(NULL), _rowEndChpx(NULL)
	{
        _trPr			=	new XMLTools::XMLElement(L"w:trPr");
        _tblPrEx		=	new XMLTools::XMLElement(L"w:tblPrEx");
        //_tblBorders	=	new XMLTools::XMLElement(L"w:tblBorders");
		_rowEndChpx		=	rowEndChpx;
	}

	TableRowPropertiesMapping::~TableRowPropertiesMapping()
	{
		RELEASEOBJECT(_trPr);
		RELEASEOBJECT(_tblPrEx);
	}

	void TableRowPropertiesMapping::Apply(IVisitable* visited)
	{
		TablePropertyExceptions* tapx = static_cast<TablePropertyExceptions*>( visited );

		std::shared_ptr<BorderCode> brcLeft;
		std::shared_ptr<BorderCode> brcTop;
		std::shared_ptr<BorderCode> brcBottom;
		std::shared_ptr<BorderCode> brcRight;
		std::shared_ptr<BorderCode> brcHorz;
		std::shared_ptr<BorderCode> brcVert;

		//delete infos
		RevisionData rev( _rowEndChpx );

		if ( ( _rowEndChpx != NULL ) && ( rev.Type == Deleted ) )
		{
            XMLTools::XMLElement del( L"w:del" );
			_trPr->AppendChild( del );
		}

		XMLTools::XMLElement rowHeight(L"w:trHeight");
		for ( std::list<SinglePropertyModifier>::iterator iter = tapx->grpprl->begin(); iter != tapx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode )  
			{
				case sprmOldTDefTable:
				case sprmTDefTable:
				{
					//SprmTDefTable tdef = new SprmTDefTable(sprm.Arguments);
				}break;	
				case sprmOldTTableHeader:
				case sprmTTableHeader:
				{						//header row

					bool fHeader = ( iter->Arguments[0] != 0 ) ? (true) : (false);

					if ( fHeader )
					{
                        XMLTools::XMLElement header( L"w:tblHeader" );
						_trPr->AppendChild( header );
					}
				}break;	
				case sprmTWidthAfter:
				{ //width after
                    XMLTools::XMLElement wAfter( L"w:wAfter" );
                    XMLTools::XMLAttribute wAfterValue( L"w:w", FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize ) ) );
					wAfter.AppendAttribute( wAfterValue );

                    XMLTools::XMLAttribute wAfterType( L"w:type", L"dxa" );
					wAfter.AppendAttribute( wAfterType );
					_trPr->AppendChild( wAfter, true );
				}break;	
				case sprmTWidthBefore:
				{ //width before
					short before = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );

					if ( before != 0 )
					{
                        XMLTools::XMLElement wBefore( L"w:wBefore" );
                        XMLTools::XMLAttribute wBeforeValue( L"w:w", FormatUtils::IntToWideString( before ) );
						wBefore.AppendAttribute( wBeforeValue );

                        XMLTools::XMLAttribute wBeforeType( L"w:type", L"dxa" );
						wBefore.AppendAttribute( wBeforeType );
						_trPr->AppendChild( wBefore, true );
					}
				}break;	
				case sprmOldTDyaRowHeight:
				case sprmTDyaRowHeight:
				{ //row height
                    XMLTools::XMLAttribute rowHeightVal( L"w:val" );
                    XMLTools::XMLAttribute rowHeightRule( L"w:hRule" );

					short rH = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );

					if ( rH > 0 )
					{
                        rowHeightRule.SetValue( L"atLeast" );
						rowHeightVal.SetValue( FormatUtils::IntToWideString( rH ) );
						rowHeight.AppendAttribute( rowHeightVal );
					}
					else if( rH == 0 )
					{
                        rowHeightRule.SetValue( L"auto" );
					}
					else
					{
                        rowHeightRule.SetValue( L"exact" );
						rH *= -1;
						rowHeightVal.SetValue( FormatUtils::IntToWideString( rH ) );
						rowHeight.AppendAttribute( rowHeightVal );
					}
					rowHeight.AppendAttribute( rowHeightRule );
				}
				break;
				case sprmOldTFCantSplit:
				case sprmTFCantSplit:
					break;
				case sprmTFCantSplit90:
				{ //can't split
					if (iter->argumentsSize > 0 && iter->Arguments[0] != 0)
						appendFlagElement( _trPr, *iter, L"cantSplit", true );
				}break;	
				case sprmTIpgp:// = PGPInfo.ipgpSelf (PGPInfo structure describes the border and margin properties)
				{	//div id
				}break;
				case sprmTCellSpacing:
				case sprmTCellSpacingDefault:
				{
					unsigned char grfbrc = iter->Arguments[2];
					short wSpc = FormatUtils::BytesToInt16(iter->Arguments, 4, iter->argumentsSize);
					std::wstring strValue = FormatUtils::IntToWideString(wSpc);
					if (FormatUtils::BitmaskToBool((int)grfbrc, 0x01))
					{
						appendDxaElement(_trPr, L"tblCellSpacing", strValue, true);
					}
				}break;
				case sprmTTableBorders80:
				{
					const int size = 4;
					unsigned char brc80[size];

					memcpy(brc80, iter->Arguments, size);
					brcTop = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

					memcpy(brc80, (iter->Arguments + 4), size);
					brcLeft = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

					memcpy(brc80, (iter->Arguments + 8), size);
					brcBottom = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

					memcpy(brc80, (iter->Arguments + 12), size);
					brcRight = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

					memcpy(brc80, (iter->Arguments + 16), size);
					brcHorz = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

					memcpy(brc80, (iter->Arguments + 20), size);
					brcVert = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));
				}break;
				default:
					break;
			}
		}
		if (rowHeight.GetAttributeCount() > 0)
		{
			_trPr->AppendChild(rowHeight);
		}

		//set borders
		XMLTools::XMLElement* _tblBorders = new XMLTools::XMLElement(L"w:tblBorders");
		if (brcTop)
		{
			XMLTools::XMLElement border(L"w:top");
			appendBorderAttributes(brcTop.get(), &border);
			addOrSetBorder(_tblBorders, &border);
		}
		if (brcLeft)
		{
			XMLTools::XMLElement border(L"w:left");
			appendBorderAttributes(brcLeft.get(), &border);
			addOrSetBorder(_tblBorders, &border);
		}
		if (brcBottom)
		{
			XMLTools::XMLElement border(L"w:bottom");
			appendBorderAttributes(brcBottom.get(), &border);
			addOrSetBorder(_tblBorders, &border);
		}
		if (brcRight)
		{
			XMLTools::XMLElement border(L"w:right");
			appendBorderAttributes(brcRight.get(), &border);
			addOrSetBorder(_tblBorders, &border);
		}
		if (brcHorz)
		{
			XMLTools::XMLElement border(L"w:insideH");
			appendBorderAttributes(brcHorz.get(), &border);
			addOrSetBorder(_tblBorders, &border);
		}
		if (brcVert)
		{
			XMLTools::XMLElement border(L"w:insideV");
			appendBorderAttributes(brcHorz.get(), &border);
			addOrSetBorder(_tblBorders, &border);
		}
		if (_tblBorders->GetChildCount() > 0)
		{
			_tblPrEx->AppendChild(*_tblBorders);
		}
//---------------------------------------------------------------------------		
		if ( _tblPrEx->GetChildCount() > 0 )
		{
			m_pXmlWriter->WriteString(_tblPrEx->GetXMLString());
		}
		if ( ( _trPr->GetChildCount() > 0 ) || ( _trPr->GetAttributeCount() > 0 ) )
		{
			m_pXmlWriter->WriteString( _trPr->GetXMLString() );
		}
	}
}
