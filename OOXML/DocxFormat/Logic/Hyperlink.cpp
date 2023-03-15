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
#include "../DocxFlat.h"
#include "Hyperlink.h"
#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "RunProperty.h"
#include "ParagraphProperty.h"
#include "FldSimple.h"
#include "Bdo.h"
#include "Sdt.h"
#include "SmartTag.h"
#include "Dir.h"
#include "../Math/oMathPara.h"
#include "../Math/OMath.h"


// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>
//        <w:subDoc>


namespace OOX
{
	namespace Logic
	{
		CHyperlink::CHyperlink(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		CHyperlink::CHyperlink(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CHyperlink::CHyperlink(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CHyperlink::~CHyperlink()
		{
		}
		const CHyperlink& CHyperlink::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CHyperlink& CHyperlink::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CHyperlink::ClearItems()
		{
			m_sAnchor.reset();
			m_sDocLocation.reset();
			m_oHistory.reset();
			m_oId.reset();
			m_sTgtFrame.reset();
			m_sTooltip.reset();
			m_sDestinition.reset();

			WritingElementWithChilds::ClearItems();
		}
		void CHyperlink::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:anchor"),		m_sAnchor );
            XmlMacroReadAttributeBase( oNode, _T("w:docLocation"),	m_sDocLocation );
            XmlMacroReadAttributeBase( oNode, _T("w:history"),		m_oHistory );
            XmlMacroReadAttributeBase( oNode, _T("r:id"),			m_oId );
            XmlMacroReadAttributeBase( oNode, _T("w:tgtFrame"),		m_sTgtFrame );
            XmlMacroReadAttributeBase( oNode, _T("w:tooltip"),		m_sTooltip );

			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, _T("relationships:id"), m_oId );
			}

			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( L"*", oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( _T("w:bdo") == sName )
							pItem = new CBdo( oItem );
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
						else if ( _T("w:dir") == sName )
							pItem = new CDir( oItem );
						else if ( _T("w:fldSimple") == sName )
							pItem = new CFldSimple( oItem );
						else if ( _T("w:hyperlink") == sName )
							pItem = new CHyperlink( oItem );
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
						else if ( _T("w:r") == sName )
							pItem = new CRun( oItem );
						else if ( _T("w:sdt") == sName )
							pItem = new CSdt( oItem );
						else if ( _T("w:smartTag") == sName )
							pItem = new CSmartTag( oItem );
						//else if ( _T("w:subDoc") == sName )
						//	pItem = new CSubDoc( oItem );

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}
		}
		void CHyperlink::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, _T("w:anchor"), m_sAnchor)
				WritingElement_ReadAttributes_Read_if(oReader, _T("w:bookmark"), m_sAnchor) //for Word 2003 XML
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("w:dest"), m_sDestinition) //for Word 2003 XML
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("w:docLocation"), m_sDocLocation)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("w:history"), m_oHistory)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("r:id"), m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("relationships:id"), m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("w:tgtFrame"), m_sTgtFrame)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("w:tooltip"), m_sTooltip)
			WritingElement_ReadAttributes_End(oReader)

			if (m_sDestinition.IsInit())
			{
				CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(m_pMainDocument);
				if (docx_flat)
				{
					smart_ptr<OOX::File> oHyperlinkFile = smart_ptr<OOX::File>(new OOX::HyperLink(m_pMainDocument, OOX::CPath(*m_sDestinition, false)));
					const OOX::RId rId = docx_flat->m_currentContainer->Add(oHyperlinkFile);
					m_oId = new SimpleTypes::CRelationshipId(rId.get());
				}
			}
		}
		void CHyperlink::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					pItem = new CBdo( document );
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
				else if ( _T("w:dir") == sName )
					pItem = new CDir( document );
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple( document );
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink( document );
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
				else if ( _T("w:r") == sName )
					pItem = new CRun( document, this );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( document );
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag( document );
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );

				if ( pItem )
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back( pItem );
				}
			}
		}
		std::wstring CHyperlink::toXML() const
		{
				std::wstring sResult = L"<w:hyperlink";

				if ( m_sAnchor.IsInit() )
				{
					sResult += L" w:anchor=\"" + (*m_sAnchor) + L"\"";
				}

				if ( m_sDocLocation.IsInit() )
				{
					sResult += L" w:docLocation=\"" + (*m_sDocLocation) + L"\"";
				}

				if ( m_oHistory.IsInit() )
				{
					sResult += L" w:history=\"" + m_oHistory->ToString() + L"\"";
				}

				if ( m_oId.IsInit() )
				{
					sResult += L" r:id=\"" + m_oId->ToString() + L"\"";
				}

				if ( m_sTgtFrame.IsInit() )
				{
					sResult += L" w:tgtFrame=\"" + (*m_sTgtFrame) + L"\"";
				}

				if ( m_sTooltip.IsInit() )
				{
					sResult += L" w:tooltip=\"" + XmlUtils::EncodeXmlString(*m_sTooltip) + L"\"";
				}

				sResult += L">";

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				sResult += L"</w:hyperlink>";

				return sResult;
		}
		EElementType CHyperlink::getType() const
		{
			return et_w_hyperlink;
		}

		CAltChunkPr::CAltChunkPr(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CAltChunkPr::~CAltChunkPr()
		{
		}
		void CAltChunkPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( L"*", oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( L"w:matchSrc" == sName )
							m_oMatchSrc = new ComplexTypes::Word::CMatchSrc( oItem );
					}
				}
			}
		}
		void CAltChunkPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( L"w:matchSrc" == sName )
					m_oMatchSrc = oReader ;

			}
		}
		std::wstring CAltChunkPr::toXML() const
		{
				std::wstring sResult = L"<w:altChunkPr>";
				
				if (m_oMatchSrc.IsInit())
				{
					sResult += L"<w:matchSrc ";
					sResult += m_oMatchSrc->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:altChunkPr>";
				return sResult;
		}
		EElementType CAltChunkPr::getType() const
		{
			return et_w_altChunkPr;
		}

		CAltChunk::CAltChunk(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CAltChunk::~CAltChunk()
		{
		}
		void CAltChunk::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, L"r:id", m_oId );
			
			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
			}

			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( L"*", oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( L"w:altChunkPr" == sName )
							m_oAltChunkPr = new CAltChunkPr( oItem );
					}
				}
			}
		}
		void CAltChunk::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( L"w:altChunkPr" == sName )
					m_oAltChunkPr = oReader ;

			}
		}
		std::wstring CAltChunk::toXML() const
		{
				std::wstring sResult = L"<w:altChunk ";

				if ( m_oId.IsInit() )
				{
					sResult += L"r:id=\"";
					sResult += m_oId->ToString();
					sResult += L"\" ";
				}
				sResult += L">";

				if (m_oAltChunkPr.IsInit())
					sResult += m_oAltChunkPr->toXML();

				sResult += L"</w:altChunk>";

				return sResult;
		}
		EElementType CAltChunk::getType() const
		{
			return et_w_altChunk;
		}
		void CAltChunk::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, L"id", m_oId)
			WritingElement_ReadAttributes_End_No_NS( oReader )

		}

	} // namespace Logic
} // namespace OOX
