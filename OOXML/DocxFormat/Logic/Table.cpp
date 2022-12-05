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
#include "../Math/oMathPara.h"
#include "../Math/OMath.h"

#include "Table.h" 
#include "Paragraph.h"
#include "Annotations.h"
#include "Sdt.h"
#include "Hyperlink.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// TblGridCol 17.4.16 (Part 1)
		//--------------------------------------------------------------------------------

		CTblGridCol::CTblGridCol()
		{
		}
		CTblGridCol::~CTblGridCol()
		{
		}
		void CTblGridCol::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:w", m_oW );
		}
		void CTblGridCol::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CTblGridCol::ToString() const
		{
			std::wstring sResult;

			if ( m_oW.IsInit() )
			{
				sResult += L"w:w=\"";
				sResult += m_oW->ToString();
				sResult += L"\" ";
			}

			return sResult;
		}
		void CTblGridCol::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:w", m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CTblGridChange 
		//--------------------------------------------------------------------------------	
		CTblGridChange::CTblGridChange(OOX::Document *pMain) : WritingElement(pMain)
		{
			m_pTblGrid = new CTblGrid(pMain);

			if ( m_pTblGrid )
				m_pTblGrid->m_bTblGridChange = true;
		}
		CTblGridChange::CTblGridChange(XmlUtils::CXmlNode& oNode) : WritingElement(NULL)
		{
			m_pTblGrid = new CTblGrid(NULL);

			if ( m_pTblGrid )
				m_pTblGrid->m_bTblGridChange = true;

			fromXML( oNode );
		}
		CTblGridChange::CTblGridChange(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
		{
			m_pTblGrid = new CTblGrid(NULL);

			if ( m_pTblGrid )
				m_pTblGrid->m_bTblGridChange = true;

			fromXML( oReader );
		}
		CTblGridChange::~CTblGridChange()
		{
			if ( m_pTblGrid )
				delete m_pTblGrid;
		}
		const CTblGridChange& CTblGridChange::operator = (const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CTblGridChange& CTblGridChange::operator = (const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTblGridChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( m_pTblGrid && _T("w:tblGrid") == sName )
					m_pTblGrid->fromXML( oReader );
			}

		}
		void CTblGridChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );

			XmlUtils::CXmlNode oNode_tblGrid;

			if ( m_pTblGrid && oNode.GetNode( _T("w:tblGrid"), oNode_tblGrid ) )
				m_pTblGrid->fromXML( oNode_tblGrid );

		}
		std::wstring CTblGridChange::toXML() const
		{			
			std::wstring sResult = _T("<w:tblGridChange ");

			if ( m_oId.IsInit() )
			{
				sResult += _T("w:id=\"");
				sResult += m_oId->ToString();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if ( m_pTblGrid )
				sResult += m_pTblGrid->toXML();

			sResult += _T("</w:tblGridChange>");

			return sResult;
		}
		EElementType CTblGridChange::getType() const
		{
			return et_w_tblGridChange;
		}
		void CTblGridChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:id"), m_oId )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CTblPrExChange 
		//--------------------------------------------------------------------------------	
		CTblPrExChange::CTblPrExChange(OOX::Document *pMain) : WritingElement(pMain)
		{
			m_pTblPrEx.Init();
			m_pTblPrEx->m_bTblPrExChange = true;
		}
		CTblPrExChange::CTblPrExChange(XmlUtils::CXmlNode& oNode) : WritingElement(NULL)
		{
			m_pTblPrEx.Init();
			m_pTblPrEx->m_bTblPrExChange = true;

			fromXML( oNode );
		}
		CTblPrExChange::CTblPrExChange(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
		{
			m_pTblPrEx.Init();
			m_pTblPrEx->m_bTblPrExChange = true;

			fromXML( oReader );
		}
		CTblPrExChange::~CTblPrExChange()
		{
		}
		const CTblPrExChange& CTblPrExChange::operator = (const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CTblPrExChange& CTblPrExChange::operator = (const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTblPrExChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
            XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate );
            XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
            XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_tblPrEx;

			if ( m_pTblPrEx.IsInit() && oNode.GetNode( _T("w:tblPrEx"), oNode_tblPrEx ) )
				m_pTblPrEx->fromXML( oNode_tblPrEx );

		}
		void CTblPrExChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( m_pTblPrEx.IsInit() && _T("w:tblPrEx") == sName )
					m_pTblPrEx->fromXML( oReader );
			}
		}
		std::wstring CTblPrExChange::toXML() const
		{			
			std::wstring sResult = _T("<w:tblPrExChange ");

			if ( m_sAuthor.IsInit() )
			{
				sResult += _T("w:author=\"");
                sResult += m_sAuthor.get2();
				sResult += _T("\" ");
			}

			if ( m_oDate.IsInit() )
			{
				sResult += _T("w:date=\"");
				sResult += m_oDate->ToString();
				sResult += _T("\" ");
			}

			if ( m_oId.IsInit() )
			{
				sResult += _T("w:id=\"");
				sResult += m_oId->ToString();
				sResult += _T("\" ");
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += _T("oouserid=\"");
                sResult += m_sUserId.get2();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if ( m_pTblPrEx.IsInit() )
				sResult += m_pTblPrEx->toXML();

			sResult += _T("</w:tblPrExChange>");

			return sResult;
		}
		EElementType CTblPrExChange::getType() const
		{
			return et_w_tblPrExChange;
		}
		void CTblPrExChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CTbl 17.4.38 (Part 1)
		//--------------------------------------------------------------------------------

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>

		CTbl::CTbl(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
			m_oTableProperties = NULL;
			m_nCountRow = 0;
		}
		CTbl::CTbl(XmlUtils::CXmlNode &oNode) : WritingElementWithChilds<>(NULL)
		{
			m_oTableProperties	= NULL;
			m_nCountRow = 0;
			fromXML( oNode );
		}
		CTbl::CTbl(XmlUtils::CXmlLiteReader& oReader) : WritingElementWithChilds<>(NULL)
		{
			m_oTableProperties = NULL;
			m_nCountRow = 0;
			fromXML( oReader );
		}
		CTbl::~CTbl()
		{
		}
		const CTbl& CTbl::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CTbl& CTbl::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTbl::ClearItems()
		{
			m_oTblGrid.reset();
			m_oTableProperties = NULL;
			m_nCountRow = 0;

			WritingElementWithChilds::ClearItems();
		}
		void CTbl::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( _T("w:bookmarkEnd") == sName )
							pItem = new CBookmarkEnd( oItem );
						else if ( _T("w:bookmarkStart") == sName )
							pItem = new CBookmarkStart( oItem );
						else if ( _T("w:commentRangeEnd") == sName )
							pItem = new CCommentRangeEnd( oItem );
						else if ( _T("w:commentRangeStart") == sName )
							pItem = new CCommentRangeStart( oItem );
						//else if ( _T("w:customXml") == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( _T("w:customXmlDelRangeEnd") == sName )
							pItem = new CCustomXmlDelRangeEnd( oItem );
						else if ( _T("w:customXmlDelRangeStart") == sName )
							pItem = new CCustomXmlDelRangeStart( oItem );
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							pItem = new CCustomXmlInsRangeEnd( oItem );
						else if ( _T("w:customXmlInsRangeStart") == sName )
							pItem = new CCustomXmlInsRangeStart( oItem );
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveFromRangeEnd( oItem );
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							pItem = new CCustomXmlMoveFromRangeStart( oItem );
						else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveToRangeEnd( oItem );
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							pItem = new CCustomXmlMoveToRangeStart( oItem );
						//else if ( _T("w:del") == sName )
						//	pItem = new CDel( oItem );
						//else if ( _T("w:ins") == sName )
						//	pItem = new CIns( oItem );
						//else if ( _T("w:moveFrom") == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( _T("w:moveFromRangeEnd") == sName )
							pItem = new CMoveFromRangeEnd( oItem );
						else if ( _T("w:moveFromRangeStart") == sName )
							pItem = new CMoveFromRangeStart( oItem );
						//else if ( _T("w:moveTo") == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( _T("w:moveToRangeEnd") == sName )
							pItem = new CMoveToRangeEnd( oItem );
						else if ( _T("w:moveToRangeStart") == sName )
							pItem = new CMoveToRangeStart( oItem );
						else if ( _T("m:oMath") == sName )
							pItem = new COMath( oItem );
						else if ( _T("m:oMathPara") == sName )
							pItem = new COMathPara( oItem );
						else if ( _T("w:permEnd") == sName )
							pItem = new CPermEnd( oItem );
						else if ( _T("w:permStart") == sName )
							pItem = new CPermStart( oItem );
						else if ( _T("w:proofErr") == sName )
							pItem = new CProofErr( oItem );
						else if ( _T("w:sdt") == sName )
							pItem = new CSdt( oItem );
						else if ( _T("w:tblGrid") == sName )
							m_oTblGrid = oItem;
						else if ( _T("w:tblPr") == sName )
						{
							pItem = m_oTableProperties = new CTableProperty( oItem );
						}
						else if ( _T("w:tr") == sName )
							pItem = new CTr( oItem );

						if ( pItem )
						{
							m_arrItems.push_back( pItem );
						}
					}
				}
			}
		}
		void CTbl::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;
			
			int nParentDepth = oReader.GetDepth();
			CreateElements(oReader, nParentDepth);
		}
		void CTbl::CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth)
		{
			OOX::Document* document = WritingElement::m_pMainDocument;

			while (oReader.ReadNextSiblingNode(Depth))
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if (_T("w:bookmarkEnd") == sName)
					pItem = new CBookmarkEnd(document);
				else if (_T("w:bookmarkStart") == sName)
					pItem = new CBookmarkStart(document);
				else if (_T("w:commentRangeEnd") == sName)
					pItem = new CCommentRangeEnd(document);
				else if (_T("w:commentRangeStart") == sName)
					pItem = new CCommentRangeStart(document);
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( document );
				else if (_T("w:customXmlDelRangeEnd") == sName)
					pItem = new CCustomXmlDelRangeEnd(document);
				else if (_T("w:customXmlDelRangeStart") == sName)
					pItem = new CCustomXmlDelRangeStart(document);
				else if (_T("w:customXmlInsRangeEnd") == sName)
					pItem = new CCustomXmlInsRangeEnd(document);
				else if (_T("w:customXmlInsRangeStart") == sName)
					pItem = new CCustomXmlInsRangeStart(document);
				else if (_T("w:customXmlMoveFromRangeEnd") == sName)
					pItem = new CCustomXmlMoveFromRangeEnd(document);
				else if (_T("w:customXmlMoveFromRangeStart") == sName)
					pItem = new CCustomXmlMoveFromRangeStart(document);
				else if (_T("w:customXmlMoveToRangeEnd") == sName)
					pItem = new CCustomXmlMoveToRangeEnd(document);
				else if (_T("w:customXmlMoveToRangeStart") == sName)
					pItem = new CCustomXmlMoveToRangeStart(document);
				else if (_T("w:del") == sName)
					pItem = new CDel(document);
				else if (_T("w:ins") == sName)
					pItem = new CIns(document);
				else if (_T("w:moveFrom") == sName)
					pItem = new CMoveFrom(document);
				else if (_T("w:moveFromRangeEnd") == sName)
					pItem = new CMoveFromRangeEnd(document);
				else if (_T("w:moveFromRangeStart") == sName)
					pItem = new CMoveFromRangeStart(document);
				else if (_T("w:moveTo") == sName)
					pItem = new CMoveTo(document);
				else if (_T("w:moveToRangeEnd") == sName)
					pItem = new CMoveToRangeEnd(document);
				else if (_T("w:moveToRangeStart") == sName)
					pItem = new CMoveToRangeStart(document);
				else if (_T("m:oMath") == sName)
					pItem = new COMath(document);
				else if (_T("m:oMathPara") == sName)
					pItem = new COMathPara(document);
				else if (_T("w:permEnd") == sName)
					pItem = new CPermEnd(document);
				else if (_T("w:permStart") == sName)
					pItem = new CPermStart(document);
				else if (_T("w:proofErr") == sName)
					pItem = new CProofErr(document);
				else if (_T("w:sdt") == sName)
					pItem = new CSdt(document);
				else if (_T("w:tblGrid") == sName)
				{
					if (false == m_oTblGrid.IsInit())
					{
						m_oTblGrid = new CTblGrid(document);
					}
					m_oTblGrid->fromXML(oReader);
				}
				else if (_T("w:tblPr") == sName)
				{
					pItem = m_oTableProperties = new CTableProperty(document);
				}
				else if (_T("w:tr") == sName)
				{
					pItem = new CTr(document);
					m_nCountRow++;
				}
				else if (L"wx:sect" == sName && !oReader.IsEmptyNode())
				{
					int nWxSectDepth = oReader.GetDepth();
					CreateElements(oReader, nWxSectDepth);
				}
				else if (L"wx:sub-section" == sName && !oReader.IsEmptyNode())
				{
					int nWxSubSectDepth = oReader.GetDepth();
					CreateElements(oReader, nWxSubSectDepth);
				}

				if (pItem)
				{
					m_arrItems.push_back(pItem);
					pItem->fromXML(oReader);
				}
			}
		}
		std::wstring CTbl::toXML() const
		{
			std::wstring sResult = _T("<w:tbl>");

			if ( m_oTblGrid.IsInit() )
			{
				sResult += m_oTblGrid->toXML();
			}

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += _T("</w:tbl>");

			return sResult;
		}
		EElementType CTbl::getType() const
		{
			return et_w_tbl;
		}

		//--------------------------------------------------------------------------------
		// CTr 17.4.79 (Part 1)
		//--------------------------------------------------------------------------------

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>

		CTr::CTr(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
			m_nCountCell = 0;
			m_pTableRowProperties = NULL;
		}
		CTr::CTr(XmlUtils::CXmlNode &oNode) : WritingElementWithChilds<>(NULL)
		{
			m_nCountCell = 0;
			m_pTableRowProperties = NULL;
			fromXML( oNode );
		}
		CTr::CTr(XmlUtils::CXmlLiteReader& oReader) : WritingElementWithChilds<>(NULL)
		{
			m_nCountCell = 0;
			m_pTableRowProperties = NULL;
			fromXML( oReader );
		}
		CTr::~CTr()
		{
		}
		const CTr& CTr::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CTr& CTr::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTr::ClearItems()
		{
			m_pTableRowProperties = NULL;
			m_oRsidDel.reset();
			m_oRsidR.reset();
			m_oRsidRPr.reset();
			m_oRsidTr.reset();

			WritingElementWithChilds::ClearItems();

		}
		void CTr::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:rsidDel"), m_oRsidDel );
            XmlMacroReadAttributeBase( oNode, _T("w:rsidR"),   m_oRsidR );
            XmlMacroReadAttributeBase( oNode, _T("w:rsidRPr"), m_oRsidRPr );
            XmlMacroReadAttributeBase( oNode, _T("w:rsidTr"),  m_oRsidTr );

			XmlUtils::CXmlNodes oChilds;
			int nNumCol = 0;
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( _T("w:bookmarkEnd") == sName )
							pItem = new CBookmarkEnd( oItem );
						else if ( _T("w:bookmarkStart") == sName )
							pItem = new CBookmarkStart( oItem );
						else if ( _T("w:commentRangeEnd") == sName )
							pItem = new CCommentRangeEnd( oItem );
						else if ( _T("w:commentRangeStart") == sName )
							pItem = new CCommentRangeStart( oItem );
						//else if ( _T("w:customXml") == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( _T("w:customXmlDelRangeEnd") == sName )
							pItem = new CCustomXmlDelRangeEnd( oItem );
						else if ( _T("w:customXmlDelRangeStart") == sName )
							pItem = new CCustomXmlDelRangeStart( oItem );
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							pItem = new CCustomXmlInsRangeEnd( oItem );
						else if ( _T("w:customXmlInsRangeStart") == sName )
							pItem = new CCustomXmlInsRangeStart( oItem );
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveFromRangeEnd( oItem );
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							pItem = new CCustomXmlMoveFromRangeStart( oItem );
						else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveToRangeEnd( oItem );
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							pItem = new CCustomXmlMoveToRangeStart( oItem );
						//else if ( _T("w:del") == sName )
						//	pItem = new CDel( oItem );
						//else if ( _T("w:ins") == sName )
						//	pItem = new CIns( oItem );
						//else if ( _T("w:moveFrom") == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( _T("w:moveFromRangeEnd") == sName )
							pItem = new CMoveFromRangeEnd( oItem );
						else if ( _T("w:moveFromRangeStart") == sName )
							pItem = new CMoveFromRangeStart( oItem );
						//else if ( _T("w:moveTo") == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( _T("w:moveToRangeEnd") == sName )
							pItem = new CMoveToRangeEnd( oItem );
						else if ( _T("w:moveToRangeStart") == sName )
							pItem = new CMoveToRangeStart( oItem );
						else if ( _T("m:oMath") == sName )
							pItem = new COMath( oItem );
						else if ( _T("m:oMathPara") == sName )
							pItem = new COMathPara( oItem );
						else if ( _T("w:permEnd") == sName )
							pItem = new CPermEnd( oItem );
						else if ( _T("w:permStart") == sName )
							pItem = new CPermStart( oItem );
						else if ( _T("w:proofErr") == sName )
							pItem = new CProofErr( oItem );
						else if ( _T("w:sdt") == sName )
							pItem = new CSdt( oItem );
						else if ( _T("w:tblPrEx") == sName )
							pItem = new CTblPrEx( oItem );
						else if ( _T("w:tc") == sName )
						{
							pItem = new CTc( oItem );
							if ( pItem )
							{
								CTc *pCell = (CTc *)pItem;
								pCell->m_nNumCol = nNumCol;
								
								if ( pCell->m_pTableCellProperties )
								{
									if ( pCell->m_pTableCellProperties->m_oGridSpan.IsInit() && pCell->m_pTableCellProperties->m_oGridSpan->m_oVal.IsInit() )
										nNumCol += *pCell->m_pTableCellProperties->m_oGridSpan->m_oVal;
									else
										nNumCol++;
								}
								else
									nNumCol++;
							}
						}
						else if ( _T("w:trPr") == sName )
						{
							pItem = m_pTableRowProperties = new CTableRowProperties( oItem );
						}

						if ( pItem )
						{
							m_arrItems.push_back( pItem );
						}
					}
				}
			}
		}
		void CTr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			CreateElements(oReader, nParentDepth);
		}
		void CTr::CreateElements(XmlUtils::CXmlLiteReader &oReader, int nDepth)
		{			
			OOX::Document* document = WritingElement::m_pMainDocument;
			int nNumCol = 0;
			
			while( oReader.ReadNextSiblingNode(nDepth) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( document );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( document );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( document );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( document );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( document );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( document );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( document );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( document );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( document );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( document );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( document );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( document );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( document );
				else if ( _T("w:del") == sName )
					pItem = new CDel( document );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( document );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( document );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( document );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( document );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( document );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( document );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( document );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( document );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( document );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( document );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( document );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( document );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( document );
				else if ( _T("w:tblPrEx") == sName )
					pItem = new CTblPrEx( document );
				else if ( _T("w:tc") == sName )
				{
					pItem = new CTc( document );
					if ( pItem )
					{
						pItem->fromXML(oReader);

						CTc *pCell = (CTc *)pItem;
						pCell->m_nNumCol = nNumCol;

						if ( pCell->m_pTableCellProperties )
						{
							if ( pCell->m_pTableCellProperties->m_oGridSpan.IsInit() && pCell->m_pTableCellProperties->m_oGridSpan->m_oVal.IsInit() )
								nNumCol += *pCell->m_pTableCellProperties->m_oGridSpan->m_oVal;
							else
								nNumCol++;
						}
						else
							nNumCol++;
						
						m_arrItems.push_back( pItem );
					}
					continue;
				}
				else if ( _T("w:trPr") == sName )
				{
					if (!m_pTableRowProperties)
					{
						m_pTableRowProperties = new CTableRowProperties(document);
						m_arrItems.push_back( m_pTableRowProperties );
					}
					m_pTableRowProperties->fromXML(oReader);
				}
				else if (L"wx:sect" == sName && !oReader.IsEmptyNode())
				{
					int nWxSectDepth = oReader.GetDepth();
					CreateElements(oReader, nWxSectDepth);
				}
				else if (L"wx:sub-section" == sName && !oReader.IsEmptyNode())
				{
					int nWxSubSectDepth = oReader.GetDepth();
					CreateElements(oReader, nWxSubSectDepth);
				}

				if ( pItem )
				{
					m_arrItems.push_back( pItem );
					pItem->fromXML(oReader);
				}
			}
			m_nCountCell = nNumCol;
		}
		std::wstring CTr::toXML() const
		{
			std::wstring sResult = _T("<w:tr ");

			ComplexTypes_WriteAttribute( _T("w:rsidDel=\""), m_oRsidDel );
			ComplexTypes_WriteAttribute( _T("w:rsidR=\""),   m_oRsidR );
			ComplexTypes_WriteAttribute( _T("w:rsidRPr=\""), m_oRsidRPr );
			ComplexTypes_WriteAttribute( _T("w:rsidTr=\""),  m_oRsidTr );

			sResult += _T(">");

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += _T("</w:tr>");

			return sResult;
		}
		EElementType CTr::getType() const
		{
			return et_w_tr;
		}
		void CTr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:rsidDel"), m_oRsidDel )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:rsidR"),   m_oRsidR )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:rsidRPr"), m_oRsidRPr )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:rsidTr"),  m_oRsidTr )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CTc 17.4.66 (Part 1)
		//--------------------------------------------------------------------------------

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>

		CTc::CTc(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
			m_nNumCol = 0;
			m_pTableCellProperties = NULL;
		}
		CTc::CTc(XmlUtils::CXmlNode &oNode) : WritingElementWithChilds<>(NULL)
		{
			m_nNumCol = 0;
			m_pTableCellProperties = NULL;
			fromXML( oNode );
		}
		CTc::CTc(XmlUtils::CXmlLiteReader& oReader) : WritingElementWithChilds<>(NULL)
		{
			m_nNumCol = 0;
			m_pTableCellProperties = NULL;
			fromXML( oReader );
		}
		CTc::~CTc()
		{
		}
		const CTc& CTc::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();

			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CTc& CTc::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();

			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CTc::ClearItems()
		{
			m_pTableCellProperties = NULL;
			m_nNumCol = 0;
			m_sId.reset();

			WritingElementWithChilds::ClearItems();
		}
		void CTc::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:id"), m_sId );

			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( _T("w:altChunk") == sName )
							pItem = new CAltChunk( oItem );
						else if ( _T("w:bookmarkEnd") == sName )
							pItem = new CBookmarkEnd( oItem );
						else if ( _T("w:bookmarkStart") == sName )
							pItem = new CBookmarkStart( oItem );
						else if ( _T("w:commentRangeEnd") == sName )
							pItem = new CCommentRangeEnd( oItem );
						else if ( _T("w:commentRangeStart") == sName )
							pItem = new CCommentRangeStart( oItem );
						//else if ( _T("w:customXml") == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( _T("w:customXmlDelRangeEnd") == sName )
							pItem = new CCustomXmlDelRangeEnd( oItem );
						else if ( _T("w:customXmlDelRangeStart") == sName )
							pItem = new CCustomXmlDelRangeStart( oItem );
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							pItem = new CCustomXmlInsRangeEnd( oItem );
						else if ( _T("w:customXmlInsRangeStart") == sName )
							pItem = new CCustomXmlInsRangeStart( oItem );
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveFromRangeEnd( oItem );
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							pItem = new CCustomXmlMoveFromRangeStart( oItem );
						else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveToRangeEnd( oItem );
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							pItem = new CCustomXmlMoveToRangeStart( oItem );
						//else if ( _T("w:del") == sName )
						//	pItem = new CDel( oItem );
						//else if ( _T("w:ins") == sName )
						//	pItem = new CIns( oItem );
						//else if ( _T("w:moveFrom") == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( _T("w:moveFromRangeEnd") == sName )
							pItem = new CMoveFromRangeEnd( oItem );
						else if ( _T("w:moveFromRangeStart") == sName )
							pItem = new CMoveFromRangeStart( oItem );
						//else if ( _T("w:moveTo") == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( _T("w:moveToRangeEnd") == sName )
							pItem = new CMoveToRangeEnd( oItem );
						else if ( _T("w:moveToRangeStart") == sName )
							pItem = new CMoveToRangeStart( oItem );
						else if ( _T("m:oMath") == sName )
							pItem = new COMath( oItem );
						else if ( _T("m:oMathPara") == sName )
							pItem = new COMathPara( oItem );
						else if ( _T("w:p") == sName )
							pItem = new CParagraph( oItem );
						else if ( _T("w:permEnd") == sName )
							pItem = new CPermEnd( oItem );
						else if ( _T("w:permStart") == sName )
							pItem = new CPermStart( oItem );
						else if ( _T("w:proofErr") == sName )
							pItem = new CProofErr( oItem );
						else if ( _T("w:sdt") == sName )
							pItem = new CSdt( oItem );
						else if ( _T("w:tbl") == sName )
							pItem = new CTbl( oItem );
						else if ( _T("w:tcPr") == sName )
						{
							pItem = m_pTableCellProperties = new CTableCellProperties( oItem );
						}

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}
		}
		void CTc::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			CreateElements(oReader, nParentDepth);
		}
		void CTc::CreateElements(XmlUtils::CXmlLiteReader &oReader, int nDepth)
		{
			OOX::Document* document = WritingElement::m_pMainDocument;
			
			while( oReader.ReadNextSiblingNode(nDepth) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:altChunk") == sName )
					pItem = new CAltChunk( document );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( document );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( document );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( document );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( document );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( document );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( document );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( document );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( document );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( document );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( document );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( document );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( document );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( document );
				else if ( _T("w:del") == sName )
					pItem = new CDel( document );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( document );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( document );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( document );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( document );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( document );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( document );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( document );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( document );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( document );
				else if ( _T("w:p") == sName )
					pItem = new CParagraph( document );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( document );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( document );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( document );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( document );
				else if ( _T("w:tbl") == sName )
					pItem = new CTbl( document );
				else if ( _T("w:tcPr") == sName )
				{
					if (!m_pTableCellProperties)
					{
						m_pTableCellProperties = new CTableCellProperties(document);
						m_arrItems.push_back( m_pTableCellProperties );
					}

					m_pTableCellProperties->fromXML(oReader);
				}
				else if (L"wx:sect" == sName && !oReader.IsEmptyNode())
				{
					int nWxSectDepth = oReader.GetDepth();
					CreateElements(oReader, nWxSectDepth);
				}
				else if (L"wx:sub-section" == sName && !oReader.IsEmptyNode())
				{
					int nWxSubSectDepth = oReader.GetDepth();
					CreateElements(oReader, nWxSubSectDepth);
				}
				if ( pItem )
				{
					m_arrItems.push_back( pItem );
					pItem->fromXML(oReader);
				}
			}
		}
		std::wstring CTc::toXML() const
		{
			std::wstring sResult;

			if ( m_sId.IsInit() )
			{
				sResult += _T("<w:tc w:id=\"");
                sResult += m_sId.get2();
				sResult += _T("\">");
			}
			else
				sResult = _T("<w:tc>");

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += _T("</w:tc>");

			return sResult;
		}
		EElementType CTc::getType() const
		{
			return et_w_tc;
		}
		void CTc::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:id"), m_sId )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // namespace Logic
} // namespace OOX
