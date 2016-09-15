/*
 * (c) Copyright Ascensio System SIA 2010-2016
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


#include "TableRowPropertiesMapping.h"

namespace DocFileFormat
{
	TableRowPropertiesMapping::TableRowPropertiesMapping (XMLTools::CStringXmlWriter* pWriter, CharacterPropertyExceptions* rowEndChpx) : PropertiesMapping(pWriter), _trPr(NULL), _tblPrEx(NULL), _rowEndChpx(NULL)
	{
		_trPr			=	new XMLTools::XMLElement<wchar_t>(_T( "w:trPr"));
		_tblPrEx		=	new XMLTools::XMLElement<wchar_t>(_T( "w:tblPrEx"));
		//_tblBorders	=	new XMLTools::XMLElement<wchar_t>(_T( "w:tblBorders"));
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

		//delete infos
		RevisionData rev( _rowEndChpx );

		if ( ( _rowEndChpx != NULL ) && ( rev.Type == Deleted ) )
		{
			XMLTools::XMLElement<wchar_t> del( _T( "w:del" ) );
			_trPr->AppendChild( del );
		}

		for ( std::list<SinglePropertyModifier>::iterator iter = tapx->grpprl->begin(); iter != tapx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode )  
			{
				case sprmOldTDefTable:
				case sprmTDefTable:
				{
					//SprmTDefTable tdef = new SprmTDefTable(sprm.Arguments);
				}
				break;

				case sprmOldTTableHeader:
				case sprmTTableHeader:
				{						//header row

					bool fHeader = ( iter->Arguments[0] != 0 ) ? (true) : (false);

					if ( fHeader )
					{
						XMLTools::XMLElement<wchar_t> header( _T( "w:tblHeader" ) );
						_trPr->AppendChild( header );
					}
				}
				break;

				case sprmTWidthAfter:
				{						//width after
					XMLTools::XMLElement<wchar_t> wAfter( _T( "w:wAfter" ) );
					XMLTools::XMLAttribute<wchar_t> wAfterValue( _T( "w:w" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize ) ).c_str() );
					wAfter.AppendAttribute( wAfterValue );

					XMLTools::XMLAttribute<wchar_t> wAfterType( _T( "w:type" ), _T( "dxa" ) );
					wAfter.AppendAttribute( wAfterType );
					_trPr->AppendChild( wAfter );
				}
				break;

				case sprmTWidthBefore:
				{						//width before
					short before = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );

					if ( before != 0 )
					{
						XMLTools::XMLElement<wchar_t> wBefore( _T( "w:wBefore" ) );
						XMLTools::XMLAttribute<wchar_t> wBeforeValue( _T( "w:w" ), FormatUtils::IntToWideString( before ).c_str() );
						wBefore.AppendAttribute( wBeforeValue );

						XMLTools::XMLAttribute<wchar_t> wBeforeType( _T( "w:type" ), _T( "dxa" ) );
						wBefore.AppendAttribute( wBeforeType );
						_trPr->AppendChild( wBefore );
					}
				}
				break;

				case sprmOldTDyaRowHeight:
				case sprmTDyaRowHeight:
				{						//row height
					XMLTools::XMLElement<wchar_t> rowHeight( _T( "w:trHeight" ) );
					XMLTools::XMLAttribute<wchar_t> rowHeightVal( _T( "w:val" ) );
					XMLTools::XMLAttribute<wchar_t> rowHeightRule( _T( "w:hRule" ) );

					short rH = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );

					if ( rH > 0 )
					{
						rowHeightRule.SetValue( _T( "atLeast" ) );
						rowHeightVal.SetValue( FormatUtils::IntToWideString( rH ).c_str() );
						rowHeight.AppendAttribute( rowHeightVal );
					}
					else if( rH == 0 )
					{
						rowHeightRule.SetValue( _T("auto") );
					}
					else
					{
						rowHeightRule.SetValue( _T( "exact" ) );
						rH *= -1;
						rowHeightVal.SetValue( FormatUtils::IntToWideString( rH ).c_str() );
						rowHeight.AppendAttribute( rowHeightVal );
					}

					rowHeight.AppendAttribute( rowHeightRule );
					_trPr->AppendChild( rowHeight );
				}
				break;

			case sprmOldTFCantSplit:
			case sprmTFCantSplit:
			case sprmTFCantSplit90:
			{						//can't split
				appendFlagElement( _trPr, *iter, _T( "cantSplit" ), true );
			}break;

				//div id
			case sprmTIpgp:
			{
				appendValueElement( _trPr, _T( "divId" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
			}break;

				//borders 80 exceptions
				//case SinglePropertyModifier.OperationCode.sprmTTableBorders80:
				//    unsigned char[] brc80 = new unsigned char[4];
				//    //top border
				//    Array.Copy(sprm.Arguments, 0, brc80, 0, 4);
				//    brcTop = new BorderCode(brc80);
				//    //left
				//    Array.Copy(sprm.Arguments, 4, brc80, 0, 4);
				//    brcLeft = new BorderCode(brc80);
				//    //bottom
				//    Array.Copy(sprm.Arguments, 8, brc80, 0, 4);
				//    brcBottom = new BorderCode(brc80);
				//    //right
				//    Array.Copy(sprm.Arguments, 12, brc80, 0, 4);
				//    brcRight = new BorderCode(brc80);
				//    //inside H
				//    Array.Copy(sprm.Arguments, 16, brc80, 0, 4);
				//    brcHorz = new BorderCode(brc80);
				//    //inside V
				//    Array.Copy(sprm.Arguments, 20, brc80, 0, 4);
				//    brcVert = new BorderCode(brc80);
				//    break;

				//border exceptions
				//case SinglePropertyModifier.OperationCode.sprmTTableBorders:
				//    unsigned char[] brc = new unsigned char[8];
				//    //top border
				//    Array.Copy(sprm.Arguments, 0, brc, 0, 8);
				//    brcTop = new BorderCode(brc);
				//    //left
				//    Array.Copy(sprm.Arguments, 8, brc, 0, 8);
				//    brcLeft = new BorderCode(brc);
				//    //bottom
				//    Array.Copy(sprm.Arguments, 16, brc, 0, 8);
				//    brcBottom = new BorderCode(brc);
				//    //right
				//    Array.Copy(sprm.Arguments, 24, brc, 0, 8);
				//    brcRight = new BorderCode(brc);
				//    //inside H
				//    Array.Copy(sprm.Arguments, 32, brc, 0, 8);
				//    brcHorz = new BorderCode(brc);
				//    //inside V
				//    Array.Copy(sprm.Arguments, 40, brc, 0, 8);
				//    brcVert = new BorderCode(brc);
				//    break;
			}
		}

		//set borders
		//if (brcTop != null)
		//{
		//    XmlNode topBorder = _nodeFactory.CreateNode(XmlNodeType.Element, "w", "top", OpenXmlNamespaces.WordprocessingML);
		//    appendBorderAttributes(brcTop, topBorder);
		//    addOrSetBorder(_tblBorders, topBorder);
		//}
		//if (brcLeft != null)
		//{
		//    XmlNode leftBorder = _nodeFactory.CreateNode(XmlNodeType.Element, "w", "left", OpenXmlNamespaces.WordprocessingML);
		//    appendBorderAttributes(brcLeft, leftBorder);
		//    addOrSetBorder(_tblBorders, leftBorder);
		//}
		//if (brcBottom != null)
		//{
		//    XmlNode bottomBorder = _nodeFactory.CreateNode(XmlNodeType.Element, "w", "bottom", OpenXmlNamespaces.WordprocessingML);
		//    appendBorderAttributes(brcBottom, bottomBorder);
		//    addOrSetBorder(_tblBorders, bottomBorder);
		//}
		//if (brcRight != null)
		//{
		//    XmlNode rightBorder = _nodeFactory.CreateNode(XmlNodeType.Element, "w", "right", OpenXmlNamespaces.WordprocessingML);
		//    appendBorderAttributes(brcRight, rightBorder);
		//    addOrSetBorder(_tblBorders, rightBorder);
		//}
		//if (brcHorz != null)
		//{
		//     XmlNode insideHBorder = _nodeFactory.CreateNode(XmlNodeType.Element, "w", "insideH", OpenXmlNamespaces.WordprocessingML);
		//     appendBorderAttributes(brcHorz, insideHBorder);
		//     addOrSetBorder(_tblBorders, insideHBorder);
		//}
		//if (brcVert != null)
		//{
		//    XmlNode insideVBorder = _nodeFactory.CreateNode(XmlNodeType.Element, "w", "insideV", OpenXmlNamespaces.WordprocessingML);
		//    appendBorderAttributes(brcVert, insideVBorder);
		//    addOrSetBorder(_tblBorders, insideVBorder);
		//}
		//if (_tblBorders.ChildNodes.Count > 0)
		//{
		//    _tblPrEx.AppendChild(_tblBorders);
		//}

		//set exceptions
		if ( _tblPrEx->GetChildCount() > 0 )
		{
			_trPr->AppendChild( *(_tblPrEx) );
		}

		//write Properties
		if ( ( _trPr->GetChildCount() > 0 ) || ( _trPr->GetAttributeCount() > 0 ) )
		{
			m_pXmlWriter->WriteString( _trPr->GetXMLString().c_str() );
		}
	}
}