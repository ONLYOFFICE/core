#include "stdafx.h" 
#include "OOXtblPrReader.h"
#include "OOXTableRowReader.h"
#include "RtfTable.h"
#include "OOXTableReader.h"

bool OOXTableReader::Parse( ReaderParameter oParam, RtfTable& oOutputTable )
{
	XmlUtils::CXmlReader oXmlReader;
	BOOL oRes = oXmlReader.OpenFromXmlNode(oParam.oNode);
	if( FALSE == oRes )
		return false;
	bool bExistTablPr = false;
	//сначала читаем свойства
	if( TRUE ==	oXmlReader.ReadNode( _T("w:tblPr") ) )
	{
		XML::IXMLDOMNodePtr oNode;
		oXmlReader.GetNode(oNode);
		ReaderParameter oNewParam = oParam;
		oNewParam.oNode = oNode;
		OOXtblPrReader otblPrReader;
		otblPrReader.Parse( oNewParam, oOutputTable.m_oProperty);
		bExistTablPr = true;
	}

	//формируем внешний стиль для вложенных элементов
	RtfTableStylePtr poTableStyle;
	if( PROP_DEF != oOutputTable.m_oProperty.m_nStyle )
	{
		RtfStylePtr oTempStyle;
		if( true == oParam.oDocument->m_oStyleTable.GetStyle(oOutputTable.m_oProperty.m_nStyle, oTempStyle) )
		{
			RtfStylePtr oResultStyle = oParam.oDocument->m_oStyleTable.GetStyleResulting( oTempStyle );
			if( RtfStyle::stTable == oResultStyle->m_eType )
			{
				poTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oResultStyle );
				poTableStyle->m_oTableProp = oOutputTable.m_oProperty; //TableProperty ставим как уже прочитали чтобы не терять порядок применения свойст
				//например индент последовательно затирает друг друга в стилях, numbering, просто в свойствах
			}
		}
	}
	else if( true == bExistTablPr )
	{
		RtfTableStylePtr poTableStyle = RtfTableStylePtr( new RtfTableStyle() );
		poTableStyle->m_oTableProp.Merge( oOutputTable.m_oProperty );
	}

	oXmlReader.OpenFromXmlNode(oParam.oNode);
	if( TRUE ==	oXmlReader.ReadNode( _T("w:tblGrid") ) )
	{
		XML::IXMLDOMNodePtr oNode;
		oXmlReader.GetNode(oNode);
		XmlUtils::CXmlReader oXmlSubReader;
		oXmlSubReader.OpenFromXmlNode( oNode );
		oXmlSubReader.ReadNodeList( _T("*") );
		for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
		{
			CString sSubNodeName = oXmlSubReader.ReadNodeName(j);
			if( _T("w:gridCol") == sSubNodeName )
			{
				CString sValue = oXmlSubReader.ReadNodeAttribute( j, _T("w:w") );
				if( _T("") != sValue )
					oOutputTable.m_aTableGrid.Add( Strings::ToInteger( sValue ) );
			}
		}
	}
	oXmlReader.OpenFromXmlNode(oParam.oNode);
	oXmlReader.ReadNodeList( _T("w:tr") );
	int nRowCount = oXmlReader.GetLengthList();
	for( int nCurRow = 0; nCurRow < nRowCount; nCurRow++ )
	{
		XML::IXMLDOMNodePtr oNode;
		oXmlReader.GetNode(nCurRow, oNode);
		ReaderParameter oNewParam = oParam;
		oNewParam.oNode = oNode;
		oNewParam.poTableStyle = poTableStyle;

		RtfTableRowPtr oNewRow( new RtfTableRow() );
		//применяем свойства таблицы к каждому row
		//т.к. в RTF нет свойств таблиц и все свойства записываются в свойства row
		(*((RtfTableProperty*)&oNewRow->m_oProperty)).Merge( oOutputTable.m_oProperty );
		OOXTableRowReader oRowReader;
		oRowReader.Parse( oNewParam, *oNewRow, nCurRow, nRowCount );
		oOutputTable.AddItem( oNewRow );
	}
	oOutputTable.CalculateCellx( *oParam.oDocument );
	ApplyParagraphProperty( oOutputTable );
	return true;
}
//применяет свойства параграфа связанные с положением
void OOXTableReader::ApplyParagraphProperty( RtfTable& oOutputTable )
{
	for( int i = 0; i < oOutputTable.GetCount(); i++ )
	{
		boost::shared_ptr<RtfTableRow> oCurRow ;
		oOutputTable.GetItem(oCurRow,i);
		if( oCurRow )
			for( int j = 0; j < oCurRow->GetCount(); j++ )
			{
				boost::shared_ptr<RtfTableCell> oCurCell ;
				oCurRow->GetItem( oCurCell, j );
				if( oCurCell )
				{
					boost::shared_ptr<ITextItem> pCurTextItem;
					for( int k = 0; k < oCurCell->GetCount(); k++ )
					{
						oCurCell->GetItem(pCurTextItem, k);
						if( NULL != pCurTextItem && pCurTextItem->GetType() == TYPE_RTF_PARAGRAPH )
						{
							boost::shared_ptr<RtfParagraph> oCurParagraph = boost::shared_static_cast<RtfParagraph,ITextItem>( pCurTextItem );
							oCurParagraph->m_oProperty.m_oFrame.ApplyParagraphProp( oOutputTable.m_oProperty );
						}
					}
				}
			}
	}
}