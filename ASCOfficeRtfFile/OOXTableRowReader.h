#pragma once
#include "RtfDocument.h"
#include "../Common/XmlUtils.h"
#include "OOXTableCellReader.h"
#include "RtfTableRow.h"
#include "OOXtrPrReader.h"

	class OOXTableRowReader
	{
	public: OOXTableRowReader()
			{
			}
	public: bool Parse( ReaderParameter oParam ,RtfTableRow& oOutputRow, int nCurRow, int nRowCount)
			{
				XmlUtils::CXmlReader oXmlReader;
				BOOL oRes = oXmlReader.OpenFromXmlNode(oParam.oNode);
				if( FALSE == oRes )
					return false;

				CcnfStyle oConditionStyle;
				//с начала применяем свойства
				if( TRUE == oXmlReader.ReadNode( _T("w:trPr") ) )
				{
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					OOXtrPrReader otrPrReader;
					otrPrReader.Parse( oNewParam, oOutputRow.m_oProperty, oConditionStyle);// может поменяться на любой condition(first row)
				}
				oXmlReader.OpenFromXmlNode(oParam.oNode);
				oXmlReader.ReadNodeList( _T("w:tc") );
				int nCellCount = oXmlReader.GetLengthList();

				for( int nCurCell = 0; nCurCell < nCellCount; nCurCell++)
				{
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(nCurCell, oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					RtfTableCellPtr oNewCell( new RtfTableCell() );

					OOXTableCellReader oCellReader;
					oCellReader.Parse( oNewParam, *oNewCell, oConditionStyle, nCurCell, nCurRow, nCellCount, nRowCount );
					//добавляем cell
					oOutputRow.AddItem(oNewCell);
					//свойства cell в row
					oOutputRow.m_oProperty.AddItem( oNewCell->m_oProperty );
				}
				return true;
			}
	};
