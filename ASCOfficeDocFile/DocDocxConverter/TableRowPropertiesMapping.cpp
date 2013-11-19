#include "stdafx.h"

#include "TableRowPropertiesMapping.h"

namespace DocFileFormat
{
	TableRowPropertiesMapping::TableRowPropertiesMapping (XmlUtils::CXmlWriter* pWriter, CharacterPropertyExceptions* rowEndChpx) : PropertiesMapping(pWriter), _trPr(NULL), _tblPrEx(NULL), _rowEndChpx(NULL)
	{
		_trPr			=	new XMLTools::XMLElement<WCHAR>(_T( "w:trPr"));
		_tblPrEx		=	new XMLTools::XMLElement<WCHAR>(_T( "w:tblPrEx"));
		//_tblBorders	=	new XMLTools::XMLElement<WCHAR>(_T( "w:tblBorders"));
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
		RevisionData rev( this->_rowEndChpx );

		if ( ( this->_rowEndChpx != NULL ) && ( rev.Type == Deleted ) )
		{
			XMLTools::XMLElement<WCHAR> del( _T( "w:del" ) );
			this->_trPr->AppendChild( del );
		}

		for ( list<SinglePropertyModifier>::iterator iter = tapx->grpprl->begin(); iter != tapx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode )  
			{
			case sprmTDefTable:
				{
					//SprmTDefTable tdef = new SprmTDefTable(sprm.Arguments);
				}
				break;

				//header row
			case sprmTTableHeader:
				{
					bool fHeader = ( iter->Arguments[0] != 0 ) ? (true) : (false);

					if ( fHeader )
					{
						XMLTools::XMLElement<WCHAR> header( _T( "w:tblHeader" ) );
						this->_trPr->AppendChild( header );
					}
				}
				break;

				//width after
			case sprmTWidthAfter:
				{
					XMLTools::XMLElement<WCHAR> wAfter( _T( "w:wAfter" ) );
					XMLTools::XMLAttribute<WCHAR> wAfterValue( _T( "w:w" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize ) ).c_str() );
					wAfter.AppendAttribute( wAfterValue );

					XMLTools::XMLAttribute<WCHAR> wAfterType( _T( "w:type" ), _T( "dxa" ) );
					wAfter.AppendAttribute( wAfterType );
					this->_trPr->AppendChild( wAfter );
				}
				break;

				//width before
			case sprmTWidthBefore:
				{
					short before = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );

					if ( before != 0 )
					{
						XMLTools::XMLElement<WCHAR> wBefore( _T( "w:wBefore" ) );
						XMLTools::XMLAttribute<WCHAR> wBeforeValue( _T( "w:w" ), FormatUtils::IntToWideString( before ).c_str() );
						wBefore.AppendAttribute( wBeforeValue );

						XMLTools::XMLAttribute<WCHAR> wBeforeType( _T( "w:type" ), _T( "dxa" ) );
						wBefore.AppendAttribute( wBeforeType );
						this->_trPr->AppendChild( wBefore );
					}
				}
				break;

				//row height
			case sprmTDyaRowHeight:
				{
					XMLTools::XMLElement<WCHAR> rowHeight( _T( "w:trHeight" ) );
					XMLTools::XMLAttribute<WCHAR> rowHeightVal( _T( "w:val" ) );
					XMLTools::XMLAttribute<WCHAR> rowHeightRule( _T( "w:hRule" ) );

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
					this->_trPr->AppendChild( rowHeight );
				}
				break;

				//can't split
			case sprmTFCantSplit:
			case sprmTFCantSplit90:
				appendFlagElement( this->_trPr, *iter, _T( "cantSplit" ), true );
				break;

				//div id
			case sprmTIpgp:
				appendValueElement( this->_trPr, _T( "divId" ), FormatUtils::IntToWideString( FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize ) ).c_str(), true );
				break;

				////borders 80 exceptions
				//case SinglePropertyModifier.OperationCode.sprmTTableBorders80:
				//    byte[] brc80 = new byte[4];
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

				////border exceptions
				//case SinglePropertyModifier.OperationCode.sprmTTableBorders:
				//    byte[] brc = new byte[8];
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

		////set borders
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
		if ( this->_tblPrEx->GetChildCount() > 0 )
		{
			this->_trPr->AppendChild( *(this->_tblPrEx) );
		}

		//write Properties
		if ( ( this->_trPr->GetChildCount() > 0 ) || ( this->_trPr->GetAttributeCount() > 0 ) )
		{
			m_pXmlWriter->WriteString( this->_trPr->GetXMLString().c_str() );
		}
	}
}