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
#include "Run.h"

#include "../DocxFlat.h"
#include "../Docx.h"
#include "../Document.h"
#include "../Endnote.h"
#include "../Footnote.h"
#include "../Comments.h"
#include "../Settings/Settings.h"
#include "../Drawing/Drawing.h"
#include "../Comments.h"

#include "AlternateContent.h"
#include "RunProperty.h"
#include "RunContent.h"
#include "FldChar.h"
#include "FldSimple.h"
#include "Pict.h"
#include "Annotations.h"

#include "../../../DesktopEditor/common/StringExt.h"

namespace OOX
{
	namespace Logic
	{
		CRun::CRun(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
			m_oRunProperty = NULL;
		}
		CRun::CRun(XmlUtils::CXmlNode &oNode) : WritingElementWithChilds<>(NULL)
		{
			fromXML( oNode );
		}
		CRun::CRun(XmlUtils::CXmlLiteReader& oReader) : WritingElementWithChilds<>(NULL)
		{
			fromXML( oReader );
		}
		CRun::~CRun()
		{
			ClearItems();
		}
		const CRun& CRun::operator =(const XmlUtils::CXmlNode& oNode)
		{
			ClearItems();

			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CRun& CRun::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();

			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CRun::ClearItems()
		{
			m_oRsidDel.reset();
			m_oRsidR.reset();
			m_oRsidRPr.reset();
			
			m_oRunProperty = NULL;

			WritingElementWithChilds<>::ClearItems();
		}
		void CRun::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_oRunProperty = NULL;
			
			XmlMacroReadAttributeBase( oNode, L"w:rsidDel", m_oRsidDel );
			XmlMacroReadAttributeBase( oNode, L"w:rsidR",   m_oRsidR );
			XmlMacroReadAttributeBase( oNode, L"w:rsidRPr", m_oRsidRPr );

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

						if ( L"w:annotationRef" == sName )
							pItem = new CAnnotationRef( oItem );
						else if ( L"w:br" == sName )
							pItem = new CBr( oItem );
						else if ( L"w:commentReference" == sName )
							pItem = new CCommentReference( oItem );
						else if ( L"w:contentPart" == sName )
							pItem = new CContentPart( oItem );
						else if ( L"w:continuationSeparator" == sName )
							pItem = new CContinuationSeparator( oItem );
						else if ( L"w:cr" == sName )
							pItem = new CCr( oItem );
						else if ( L"w:dayLong" == sName )
							pItem = new CDayLong( oItem );
						else if ( L"w:dayShort" == sName )
							pItem = new CDayShort( oItem );
						else if ( L"w:delInstrText" == sName )
							pItem = new CDelInstrText( oItem );
						else if ( L"w:delText" == sName )
							pItem = new CDelText( oItem );
						else if ( L"w:drawing" == sName ) 
							pItem = new CDrawing( oItem );
						else if ( L"w:endnoteRef" == sName )
							pItem = new CEndnoteRef( oItem );
						else if ( L"w:endnoteReference" == sName )
							pItem = new CEndnoteReference( oItem );
						else if ( L"w:fldChar" == sName )
							pItem = new CFldChar( oItem );
						else if ( L"w:footnoteRef" == sName )
							pItem = new CFootnoteRef( oItem );
						else if ( L"w:footnoteReference" == sName )
							pItem = new CFootnoteReference( oItem );
						else if ( L"w:instrText" == sName )
							pItem = new CInstrText( oItem );
						else if ( L"w:lastRenderedPageBreak" == sName )
							pItem = new CLastRenderedPageBreak( oItem );
						else if ( L"w:monthLong" == sName )
							pItem = new CMonthLong( oItem );
						else if ( L"w:monthShort" == sName )
							pItem = new CMonthShort( oItem );
						else if ( L"w:noBreakHyphen" == sName )
							pItem = new CNoBreakHyphen( oItem );
						else if ( L"w:object" == sName )
							pItem = new CObject( oItem );
						else if ( L"w:pgNum" == sName )
							pItem = new CPgNum( oItem );
						else if ( L"w:pict" == sName )
							pItem = new CPicture( oItem );
						else if ( L"w:ptab" == sName )
							pItem = new CPTab( oItem );
						else if ( L"w:rPr" == sName )
						{								
							if (m_oRunProperty)
							{
								CRunProperty prop2(oItem);
								CRunProperty newProp = CRunProperty::Merge(*m_oRunProperty, prop2);

								pItem = m_oRunProperty = new CRunProperty(newProp);
							}
							else
							{
								pItem = m_oRunProperty = new CRunProperty( oItem );
							}
						}
						else if ( L"w:ruby" == sName )
							pItem = new CRuby( oItem );
						else if ( L"w:separator" == sName )
							pItem = new CSeparator( oItem );
						else if ( L"w:softHyphen" == sName )
							pItem = new CSoftHyphen( oItem );
						else if ( L"w:sym" == sName )
							pItem = new CSym( oItem );
						else if ( L"w:t" == sName )
							pItem = new CText( oItem );
						else if ( L"w:tab" == sName )
							pItem = new CTab( oItem );
						else if ( L"w:yearLong" == sName )
							pItem = new CYearLong( oItem );
						else if ( L"w:yearShort" == sName )
							pItem = new CYearShort( oItem );

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}

			if ((m_oRunProperty) && (m_oRunProperty->m_pText))
			{
				m_arrItems.push_back( m_oRunProperty->m_pText);
				m_oRunProperty->m_pText = NULL;
			}
		}
		void CRun::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_oRunProperty = NULL;

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			fromXMLElems(oReader);
		}
		void CRun::fromXMLElems(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			WritingElement *pItem = NULL;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				pItem = fromXMLElem(oReader);
				if ( pItem )
				{
					m_arrItems.push_back( pItem );
				}
			}
		}
		WritingElement* CRun::fromXMLElem(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			WritingElement *pItem = NULL;

			OOX::Document* document = WritingElement::m_pMainDocument;
			
			if ( L"AlternateContent" == sName )
				pItem = new CAlternateContent( document );
			else if ( L"annotationRef" == sName )
				pItem = new CAnnotationRef( document );
			else if ( L"br" == sName )
				pItem = new CBr( document );
			else if ( L"commentReference" == sName )
				pItem = new CCommentReference( document );
			else if ( L"contentPart" == sName )
				pItem = new CContentPart( document );
			else if ( L"continuationSeparator" == sName )
				pItem = new CContinuationSeparator( document );
			else if ( L"cr" == sName )
				pItem = new CCr( document );
			else if ( L"dayLong" == sName )
				pItem = new CDayLong( document );
			else if ( L"dayShort" == sName )
				pItem = new CDayShort( document );
			else if ( L"delInstrText" == sName )
				pItem = new CDelInstrText( document );
			else if ( L"delText" == sName )
				pItem = new CDelText( document );
			else if ( L"drawing" == sName ) 
				pItem = new CDrawing( document );
			else if ( L"endnote" == sName )
			{
				CEndnoteReference *pEndRef = new CEndnoteReference(document);
				CFtnEdn *pEndnote = new CFtnEdn( document );
				pEndnote->fromXML(oReader);
				
				CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(document);
				if (docx_flat)
				{
					pEndnote->m_oId.Init();
					pEndnote->m_oId->SetValue((int)(docx_flat->m_pEndnotes->m_arrEndnote.size() + 1));
					
					pEndRef->m_oId = pEndnote->m_oId;

					docx_flat->m_pEndnotes->m_arrEndnote.push_back(pEndnote);
					docx_flat->m_pEndnotes->m_mapEndnote.insert(std::make_pair(pEndnote->m_oId->GetValue(), pEndnote));

					if (!docx_flat->m_pSettings->m_oEndnotePr.Init())
						docx_flat->m_pSettings->m_oEndnotePr.Init();
				}
				pItem = pEndRef;
			}
			else if ( L"endnoteRef" == sName )
				pItem = new CEndnoteRef( document );
			else if ( L"endnoteReference" == sName )
				pItem = new CEndnoteReference( document );
			else if ( L"fldChar" == sName )
				pItem = new CFldChar( document );
			else if ( L"footnote" == sName )
			{
				CFootnoteReference *pFootRef = new CFootnoteReference(document);
				CFtnEdn *pFootnote = new CFtnEdn( document );
				pFootnote->m_eType = OOX::et_w_footnote;

				pFootnote->fromXML(oReader);
				
				CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(document);
				if (docx_flat)
				{
					pFootnote->m_oId.Init();
					pFootnote->m_oId->SetValue((int)(docx_flat->m_pFootnotes->m_arrFootnote.size() + 1));
					pFootRef->m_oId = pFootnote->m_oId;

					docx_flat->m_pFootnotes->m_arrFootnote.push_back(pFootnote);
					docx_flat->m_pFootnotes->m_mapFootnote.insert(std::make_pair(pFootnote->m_oId->GetValue(), pFootnote));
				}
				pItem = pFootRef;
			}
			else if ( L"footnoteRef" == sName )
				pItem = new CFootnoteRef( document );
			else if ( L"footnoteReference" == sName )
				pItem = new CFootnoteReference( document );
			else if ( L"instrText" == sName )
				pItem = new CInstrText( document );
			else if ( L"lastRenderedPageBreak" == sName )
				pItem = new CLastRenderedPageBreak( document );
			else if ( L"monthLong" == sName )
				pItem = new CMonthLong( document );
			else if ( L"monthShort" == sName )
				pItem = new CMonthShort( document );
			else if ( L"noBreakHyphen" == sName )
				pItem = new CNoBreakHyphen( document );
			else if ( L"object" == sName )
				pItem = new CObject( document );
			else if ( L"pgNum" == sName )
				pItem = new CPgNum( document );
			else if ( L"pict" == sName )
				pItem = new CPicture( document );
			else if ( L"ptab" == sName )
				pItem = new CPTab( document );
			else if ( L"rPr" == sName )
			{
				if (m_oRunProperty)
				{
					CRunProperty prop2;
					prop2.fromXML(oReader, this);
					CRunProperty newProp = CRunProperty::Merge(*m_oRunProperty, prop2);

					pItem = m_oRunProperty = new CRunProperty(newProp);
				}
				else
				{
					pItem = m_oRunProperty = new CRunProperty();
					m_oRunProperty->fromXML(oReader, this);
				}
				return pItem;
			}
			else if ( L"ruby" == sName )
				pItem = new CRuby( document );
			else if ( L"separator" == sName )
				pItem = new CSeparator( document );
			else if ( L"softHyphen" == sName )
				pItem = new CSoftHyphen( document );
			else if ( L"sym" == sName )
				pItem = new CSym( document );
			else if ( L"t" == sName )
			{
				fromXMLText(oReader);
			}
			else if ( L"tab" == sName )
				pItem = new CTab( document );
			else if ( L"yearLong" == sName )
				pItem = new CYearLong( document );
			else if ( L"annotation" == sName) //Bookmark 
			{
				nullable_string sType;
				WritingElement_ReadAttributes_ReadSingle2(oReader, L"w:type", sType);
				
				if (sType.IsInit() && std::wstring::npos != sType->find(L"Bookmark.Start"))
					pItem = new CBookmarkStart( oReader );
				else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Bookmark.End"))
					pItem = new CBookmarkEnd( oReader );
				else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Comment.Start"))
					pItem = new CCommentRangeStart( oReader );
				else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Comment.End"))
					pItem = new CCommentRangeEnd( oReader );
				else if (sType.IsInit()  && std::wstring::npos != sType->find(L"Comment"))
				{
					pItem = new CComment( oReader );
					
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
			if (pItem)
			{
				pItem->fromXML(oReader);
			}
			return pItem;
		}
		void CRun::fromXMLText(XmlUtils::CXmlLiteReader& oReader)
		{
			CDocx* docx = dynamic_cast<CDocx*>(WritingElement::m_pMainDocument);
			CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(WritingElement::m_pMainDocument);

			//for <w:t>6<w:cr/>6</w:t>
			nullable<SimpleTypes::CXmlSpace> oSpace;
			CText::ReadAttributes(oReader, oSpace);

			if ((docx) && (docx->m_oMain.document))
			{
				if (false == oSpace.IsInit() && docx->m_oMain.document->m_oSpace.IsInit())
				{
					oSpace = docx->m_oMain.document->m_oSpace;
				}
			}
			else if (docx_flat)
			{
				if (false == oSpace.IsInit() && docx_flat->m_oSpace.IsInit())
				{
					oSpace = docx_flat->m_oSpace;
				}
			}
			if ( oReader.IsEmptyNode() )
				return;

			bool bTrimLeft, bTrimRight;
			bTrimLeft = bTrimRight = !(oSpace.IsInit() && SimpleTypes::xmlspacePreserve == oSpace->GetValue());
			CText* pTextLast = NULL;
			int nDepth = oReader.GetDepth();
			XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
			while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
			{
				if (	eNodeType == XmlUtils::XmlNodeType_Text
					||	eNodeType == XmlUtils::XmlNodeType_Whitespace 
					||	eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE
					||	eNodeType == XmlUtils::XmlNodeType_CDATA)
				{
					const char* pValue = oReader.GetTextChar();
					if(bTrimLeft)
					{
						bTrimLeft = false;
						pValue += NSStringExt::FindFirstNotOfA(pValue, " \n\r\t");
					}
					if('\0' != pValue[0])
					{
						pTextLast = new CText();
						NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen(pValue), pTextLast->m_sText);
						m_arrItems.push_back( pTextLast );
					}
				}
				else if (eNodeType == XmlUtils::XmlNodeType_Element)
				{
					WritingElement *pItem = fromXMLElem(oReader);
					if ( pItem )
						m_arrItems.push_back( pItem );
					bTrimLeft = false;
					pTextLast = NULL;
				}
			}
			if(bTrimRight && pTextLast)
			{
				NSStringExt::RTrim(pTextLast->m_sText, L" \n\r\t");
			}
		}
		std::wstring CRun::toXML() const
		{
			std::wstring sResult = L"<w:r";

			ComplexTypes_WriteAttribute_(L" w:rsidDel=\"", m_oRsidDel);
			ComplexTypes_WriteAttribute_(L" w:rsidR=\"", m_oRsidR);
			ComplexTypes_WriteAttribute_(L" w:rsidRPr=\"", m_oRsidRPr);

			sResult += L">";

			if (m_oRunProperty)
				sResult += m_oRunProperty->toXML();

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if (  m_arrItems[i] )
                {
                    if (m_arrItems[i]->getType() == OOX::et_w_rPr) continue;
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += L"</w:r>";

			return sResult;
		}
		EElementType CRun::getType() const
		{
			return et_w_r;
		}
		void CRun::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;

			std::wstring wsName = XmlUtils::GetNameNoNS(oReader.GetName());
			while( !wsName.empty() )
			{
				if ( L"rsidDel" == wsName )
					m_oRsidDel = oReader.GetText();
				else if ( L"rsidR" == wsName )
					m_oRsidR = oReader.GetText();
				else if ( L"rsidRPr" == wsName )
					m_oRsidRPr = oReader.GetText();

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = XmlUtils::GetNameNoNS(oReader.GetName());
			}

			oReader.MoveToElement();
		}

	} // namespace Logic
} // namespace OOX

