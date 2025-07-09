/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../Comments.h"

#include "Table.h"
#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "RunProperty.h"
#include "ParagraphProperty.h"
#include "SectionProperty.h"
#include "FldSimple.h"
#include "Bdo.h"
#include "Sdt.h"
#include "Hyperlink.h"
#include "SmartTag.h"
#include "Dir.h"

#include "../Math/OMath.h"
#include "../Math/oMathPara.h"

#include "../../Common/SimpleTypes_Word.h"

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>
//        <w:subDoc>


namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CParagraph 17.3.1.22 (Part 1)
		//--------------------------------------------------------------------------------	

		CParagraph::CParagraph(OOX::Document *pMain, WritingElement *parent) : WritingElementWithChilds<>(pMain)
		{
			m_oParagraphProperty = NULL;
			m_oParent = parent;
		}
		CParagraph::~CParagraph()
		{
			m_oParagraphProperty = NULL;
			m_oParent = NULL;
		}
		const CParagraph& CParagraph::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();

			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CParagraph& CParagraph::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();

			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CParagraph::ClearItems()
		{
			m_oRsidDel.reset();
			m_oRsidP.reset();
			m_oRsidR.reset();
			m_oRsidRDefault.reset();
			m_oRsidRPr.reset();

			m_oParagraphProperty = NULL;

			WritingElementWithChilds<>::ClearItems();
		}
		void CParagraph::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_oParagraphProperty = NULL;

            XmlMacroReadAttributeBase( oNode,L"w:rsidDel",      m_oRsidDel );
            XmlMacroReadAttributeBase( oNode,L"w:rsidP",        m_oRsidP );
            XmlMacroReadAttributeBase( oNode,L"w:rsidR",        m_oRsidR );
            XmlMacroReadAttributeBase( oNode,L"w:rsidRDefault", m_oRsidRDefault );
            XmlMacroReadAttributeBase( oNode,L"w:rsidRPr",      m_oRsidRPr );

			std::vector<XmlUtils::CXmlNode> oChilds;
			if ( oNode.GetNodes(L"*", oChilds ) )
			{
				for ( size_t nIndex = 0; nIndex < oChilds.size(); nIndex++ )
				{
					XmlUtils::CXmlNode& oItem = oChilds[nIndex];
					if (oItem.IsValid())
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"w:bdo" == sName )
							AssignPtrXmlContent(pItem, CBdo, oItem)
						else if (L"w:bookmarkEnd" == sName )
							AssignPtrXmlContent(pItem, CBookmarkEnd, oItem)
						else if (L"w:bookmarkStart" == sName )
							AssignPtrXmlContent(pItem, CBookmarkStart, oItem)
						else if (L"w:commentRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CCommentRangeEnd, oItem)
						else if (L"w:commentRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCommentRangeStart, oItem)
						//else if (L"w:customXml" == sName )
						//	pItem = new CCustomXml( oItem );
						else if (L"w:customXmlDelRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlDelRangeEnd, oItem)
						else if (L"w:customXmlDelRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlDelRangeStart, oItem)
						else if (L"w:customXmlInsRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlInsRangeEnd, oItem)
						else if (L"w:customXmlInsRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlInsRangeStart, oItem)
						else if (L"w:customXmlMoveFromRangeEnd" == sName ) 
							AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeEnd, oItem)
						else if (L"w:customXmlMoveFromRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlMoveFromRangeStart, oItem)
						else if (L"w:customXmlMoveToRangeEnd" == sName ) 
							AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeEnd, oItem)
						else if (L"w:customXmlMoveToRangeStart" == sName )
							AssignPtrXmlContent(pItem, CCustomXmlMoveToRangeStart, oItem)
						//else if (L"w:del" == sName )
						//	pItem = new CDel( oItem );
						else if (L"w:dir" == sName )
							AssignPtrXmlContent(pItem, CDir, oItem)
						else if (L"w:fldSimple" == sName )
							AssignPtrXmlContent(pItem, CFldSimple, oItem)
						else if (L"w:hyperlink" == sName )
							AssignPtrXmlContent(pItem, CHyperlink, oItem)
						//else if (L"w:ins" == sName )
						//	pItem = new CIns( oItem );
						//else if (L"w:moveFrom" == sName )
						//	pItem = new CMoveFrom( oItem );
						else if (L"w:moveFromRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CMoveFromRangeEnd, oItem)
						else if (L"w:moveFromRangeStart" == sName )
							AssignPtrXmlContent(pItem, CMoveFromRangeStart, oItem)
						//else if (L"w:moveTo" == sName )
						//	pItem = new CMoveTo( oItem );
						else if (L"w:moveToRangeEnd" == sName )
							AssignPtrXmlContent(pItem, CMoveToRangeEnd, oItem)
						else if (L"w:moveToRangeStart" == sName )
							AssignPtrXmlContent(pItem, CMoveToRangeStart, oItem)
						else if (L"m:oMath" == sName )
							AssignPtrXmlContent(pItem, COMath, oItem)
						else if (L"m:oMathPara" == sName )
							AssignPtrXmlContent(pItem, COMathPara, oItem)
						else if (L"w:permEnd" == sName )
							AssignPtrXmlContent(pItem, CPermEnd, oItem)
						else if (L"w:permStart" == sName )
							AssignPtrXmlContent(pItem, CPermStart, oItem)
						else if (L"w:pPr" == sName )
						{
							if (m_oParagraphProperty)
							{
								CParagraphProperty prop2(oItem);
								CParagraphProperty newProp = CParagraphProperty::Merge(*m_oParagraphProperty, prop2);

								pItem = m_oParagraphProperty = new CParagraphProperty(newProp);
							}
							else
							{
								pItem = m_oParagraphProperty = new CParagraphProperty( oItem );
							}
						}
						else if (L"w:proofErr" == sName )
							AssignPtrXmlContent(pItem, CProofErr, oItem)
						else if (L"w:r" == sName )
							AssignPtrXmlContent(pItem, CRun, oItem)
						else if (L"w:sdt" == sName )
							AssignPtrXmlContent(pItem, CSdt, oItem)
						else if (L"w:smartTag" == sName )
							AssignPtrXmlContent(pItem, CSmartTag, oItem)
						//else if (L"w:subDoc" == sName )
						//	pItem = new CSubDoc( oItem );

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}
		}
		void CParagraph::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_oParagraphProperty = NULL;

			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();

			fromXML(nParentDepth, oReader);
		}
		void CParagraph::fromXML(int nDepth, XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;

			while( oReader.ReadNextSiblingNode(nDepth) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				WritingElement *pItem = NULL;

				if (L"bdo" == sName )
					pItem = new CBdo( document );
				else if (L"bookmarkEnd" == sName )
					pItem = new CBookmarkEnd( document );
				else if (L"bookmarkStart" == sName )
					pItem = new CBookmarkStart( document );
				else if (L"commentRangeEnd" == sName )
					pItem = new CCommentRangeEnd( document );
				else if (L"commentRangeStart" == sName )
					pItem = new CCommentRangeStart( document );
				else if ( L"annotation" == sName) //Bookmark 
				{
					nullable_string sType;
					WritingElement_ReadAttributes_ReadSingle2(oReader, L"w:type", sType);
					
					if (sType.IsInit() && std::wstring::npos != sType->find(L"Bookmark.Start"))
						pItem = new CBookmarkStart( document );
					else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Bookmark.End"))
						pItem = new CBookmarkEnd( document );
					else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Comment.Start"))
						pItem = new CCommentRangeStart( document );
					else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Comment.End"))
						pItem = new CCommentRangeEnd( document );
					else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Comment"))
					{
						//pItem = new CComment( oReader );
						AssignPtrXmlContent(pItem, CComment, oReader)
						
						CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(document);
						if (docx_flat)
						{
							CComment* pComment = dynamic_cast<CComment*>(pItem);
							if ((pComment) && (pComment->m_oId.IsInit()))
							{
								docx_flat->m_pComments->m_mapComments.insert( std::make_pair( pComment->m_oId->GetValue(), docx_flat->m_pComments->m_arrComments.size()));
							}
							docx_flat->m_pComments->m_arrComments.push_back( pComment );	

							pItem = NULL;
						}
					}
				}
				//else if (L"customXml" == sName )
				//	pItem = new CCustomXml( document );
				else if (L"customXmlDelRangeEnd" == sName )
					pItem = new CCustomXmlDelRangeEnd( document );
				else if (L"customXmlDelRangeStart" == sName )
					pItem = new CCustomXmlDelRangeStart( document );
				else if (L"customXmlInsRangeEnd" == sName )
					pItem = new CCustomXmlInsRangeEnd( document );
				else if (L"customXmlInsRangeStart" == sName )
					pItem = new CCustomXmlInsRangeStart( document );
				else if (L"customXmlMoveFromRangeEnd" == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( document );
				else if (L"customXmlMoveFromRangeStart" == sName )
					pItem = new CCustomXmlMoveFromRangeStart( document );
				else if (L"customXmlMoveToRangeEnd" == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( document );
				else if (L"customXmlMoveToRangeStart" == sName )
					pItem = new CCustomXmlMoveToRangeStart( document );
				else if (L"del" == sName )
					pItem = new CDel( document );
				else if (L"dir" == sName )
					pItem = new CDir( document );
				else if (L"fldSimple" == sName )
					pItem = new CFldSimple( document );
				else if (L"hyperlink" == sName ||L"hlink" == sName )
					pItem = new CHyperlink( document );
				else if (L"ins" == sName )
					pItem = new CIns( document );
				else if (L"moveFrom" == sName )
					pItem = new CMoveFrom( document );
				else if (L"moveFromRangeEnd" == sName )
					pItem = new CMoveFromRangeEnd( document );
				else if (L"moveFromRangeStart" == sName )
					pItem = new CMoveFromRangeStart( document );
				else if (L"moveTo" == sName )
					pItem = new CMoveTo( document );
				else if (L"moveToRangeEnd" == sName )
					pItem = new CMoveToRangeEnd( document );
				else if (L"moveToRangeStart" == sName )
					pItem = new CMoveToRangeStart( document );
				else if (L"oMath" == sName )
					pItem = new COMath( document );
				else if (L"oMathPara" == sName )
					pItem = new COMathPara( document );
				else if (L"permEnd" == sName )
					pItem = new CPermEnd( document );
				else if (L"permStart" == sName )
					pItem = new CPermStart( document );
				else if (L"pPr" == sName )
				{// c копией  .. для быстрого доступа/анализа
					if (m_oParagraphProperty)
					{
						CParagraphProperty prop2(document);
						prop2.fromXML(oReader);

						pItem = m_oParagraphProperty = new CParagraphProperty(CParagraphProperty::Merge(*m_oParagraphProperty, prop2));
						m_arrItems.push_back( pItem );
						continue;
					}
					else
					{
						pItem = m_oParagraphProperty = new CParagraphProperty( document );
					}
				}
				else if (L"proofErr" == sName )
					pItem = new CProofErr( document );
				else if (L"r" == sName )
					pItem = new CRun( document, this );
				else if (L"sdt" == sName )
					pItem = new CSdt( document );
				else if (L"smartTag" == sName )
					pItem = new CSmartTag( document );
				//else if (L"subDoc" == sName )
				//	pItem = new CSubDoc( document );
				else if (L"sdtContent" == sName)
				{
					int nDepthChild = oReader.GetDepth();
					fromXML(nDepthChild, oReader);
				}
				else if (L"sect" == sName && !oReader.IsEmptyNode())
				{
					int nWxSectDepth = oReader.GetDepth();
					fromXML(nWxSectDepth, oReader);
				}
				else if (L"sub-section" == sName && !oReader.IsEmptyNode())
				{
					int nWxSubSectDepth = oReader.GetDepth();
					fromXML(nWxSubSectDepth, oReader);
				}
				else if (L"tbl" == sName)
				{
					WritingElementWithChilds *parent = dynamic_cast<WritingElementWithChilds*>(m_oParent);
					if (parent)
					{
						WritingElement *pItemUpper = new CTbl(document);
						if (pItemUpper)
						{
							pItemUpper->fromXML(oReader);
							parent->m_arrItems.push_back(pItemUpper);
						}
					}
				}
				else if (L"p" == sName)
				{
					int nDepthChild = oReader.GetDepth();

					fromXML(nDepthChild, oReader);
				}
				if ( pItem )
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back( pItem );
				}
			}
		}
		std::wstring CParagraph::toXML() const
		{
			std::wstring sResult = L"<w:p";

			ComplexTypes_WriteAttribute_(L" w:rsidDel=\"",      m_oRsidDel );
			ComplexTypes_WriteAttribute_(L" w:rsidP=\"",        m_oRsidP );
			ComplexTypes_WriteAttribute_(L" w:rsidR=\"",        m_oRsidR );
			ComplexTypes_WriteAttribute_(L" w:rsidRDefault=\"", m_oRsidRDefault );
			ComplexTypes_WriteAttribute_(L" w:rsidRPr=\"",      m_oRsidRPr );
			ComplexTypes_WriteAttribute_(L" w14:paraId=\"",     m_oParaId );
			ComplexTypes_WriteAttribute_(L" w14:textId=\"",     m_oTextId );

			sResult += L">";

			if (m_oParagraphProperty)
			{
				sResult += m_oParagraphProperty->toXML();
			}

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    if ( m_arrItems[i]->getType() == OOX::et_w_pPr ) continue;

                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult +=L"</w:p>";

			return sResult;
		}
		void CParagraph::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )

			WritingElement_ReadAttributes_Read_if     ( oReader,L"rsidDel",		m_oRsidDel )
			WritingElement_ReadAttributes_Read_else_if( oReader,L"rsidP",			m_oRsidP )
			WritingElement_ReadAttributes_Read_else_if( oReader,L"rsidR",			m_oRsidR )
			WritingElement_ReadAttributes_Read_else_if( oReader,L"rsidRDefault",	m_oRsidRDefault )
			WritingElement_ReadAttributes_Read_else_if( oReader,L"rsidRPr",		m_oRsidRPr )
			WritingElement_ReadAttributes_Read_else_if( oReader,L"paraId",		m_oParaId )
			WritingElement_ReadAttributes_Read_else_if( oReader,L"textId",      m_oTextId )

			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		EElementType CParagraph::getType() const
		{
			return et_w_p;
		}

	} // namespace Logic
} // namespace OOX
