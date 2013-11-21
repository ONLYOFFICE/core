#pragma once
#include "RtfDocument.h"
#include "../Common/XmlUtils.h"
#include "RtfTable.h"
#include "OOXTableReader.h"
#include "OOXParagraphReader.h"
#include "OOXtcPrReader.h"

	class OOXTableCellReader
	{
	public: OOXTableCellReader()
			{
			}
	public: bool Parse( ReaderParameter oParam ,RtfTableCell& oOutputCell,  CcnfStyle oConditionalTableStyle, int nCurCell, int nCellCount, int nCurRow, int nRowCount )
			{
				XmlUtils::CXmlReader oXmlReader;
				BOOL oRes = oXmlReader.OpenFromXmlNode(oParam.oNode);
				if( FALSE == oRes )
					return false;
				if( TRUE == oXmlReader.ReadNode( _T("w:tcPr") ) )
				{
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					OOXtcPrReader oCellPropReader;
					oCellPropReader.Parse( oNewParam, oOutputCell.m_oProperty, oConditionalTableStyle, nCurCell, nCellCount, nCurRow, nRowCount );//может поменяться на любой condition (firstRow)
				}
				else
				{
					RtfTableStylePtr oResultStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
					if( NULL != oResultStyle )
						oOutputCell.m_oProperty.Merge( oResultStyle->m_oCellProp );
					oConditionalTableStyle.ApplyTableStyleToCellBorder( oParam.poTableStyle, oOutputCell.m_oProperty, nCurCell, nCellCount, nCurRow, nRowCount );
				}
				oXmlReader.OpenFromXmlNode(oParam.oNode);

				oXmlReader.ReadNodeList( _T("*") );
				for(int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);

					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i, oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					if(sNodeName == _T("w:p") )
					{
						RtfParagraphPtr oNewParagraph( new RtfParagraph() );
						//применяем к новому параграфу default property
						oNewParagraph->m_oProperty = oParam.oDocument->m_oDefaultParagraphProp;
						oNewParagraph->m_oProperty.m_oCharProperty = oParam.oDocument->m_oDefaultCharProp;
						OOXParagraphReader oParagraphReader;
						bool bStartNewSection;
						oParagraphReader.Parse( oNewParam, (*oNewParagraph), oConditionalTableStyle, bStartNewSection );
						//ставим стиль таблицы
						if( NULL != oParam.poTableStyle )
							oNewParagraph->m_oProperty.m_nTableStyle = oParam.poTableStyle->m_nID;
						oNewParagraph->m_oProperty.m_nItap = oParam.oReader->m_nCurItap;
						oNewParagraph->m_oProperty.m_bInTable = true;
						oOutputCell.AddItem( oNewParagraph );
					}
					else if(sNodeName == _T("w:tbl") )
					{
						oParam.oReader->m_nCurItap ++ ;
						RtfTablePtr oNewTabel( new RtfTable() );
						OOXTableReader oTableReader;
						oTableReader.Parse( oNewParam, *oNewTabel);
						oOutputCell.AddItem( oNewTabel );
						oParam.oReader->m_nCurItap -- ;
					}
				}
				return true;
			}
	};
