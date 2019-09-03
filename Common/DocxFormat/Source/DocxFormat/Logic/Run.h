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
#pragma once
#ifndef OOX_LOGIC_RUN_INCLUDE_H_
#define OOX_LOGIC_RUN_INCLUDE_H_

#include "RunProperty.h"
#include "RunContent.h"
#include "FldChar.h"
#include "FldSimple.h"

#include "../Drawing/Drawing.h"
#include "Pict.h"
#include "Annotations.h"
#include "AlternateContent.h"
#include "../../../../../DesktopEditor/common/StringExt.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CRun 17.3.2.25 (Part 1)
		//--------------------------------------------------------------------------------	
		class CRun : public WritingElementWithChilds<>
		{
		public:
			CRun()
			{
				m_oRunProperty = NULL;
			}
			CRun(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CRun(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CRun()
			{
				ClearItems();
			}
			const CRun &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CRun &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				ClearItems();
				
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			virtual void ClearItems()
			{
				m_oRsidDel.reset();
				m_oRsidR.reset();
				m_oRsidRPr.reset();
				
				m_oRunProperty = NULL;

				WritingElementWithChilds<>::ClearItems();
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_oRunProperty = NULL;
				
				XmlMacroReadAttributeBase( oNode, _T("w:rsidDel"), m_oRsidDel );
				XmlMacroReadAttributeBase( oNode, _T("w:rsidR"),   m_oRsidR );
				XmlMacroReadAttributeBase( oNode, _T("w:rsidRPr"), m_oRsidRPr );

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

							if ( _T("w:annotationRef") == sName )
								pItem = new CAnnotationRef( oItem );
							else if ( _T("w:br") == sName )
								pItem = new CBr( oItem );
							else if ( _T("w:commentReference") == sName )
								pItem = new CCommentReference( oItem );
							else if ( _T("w:contentPart") == sName )
								pItem = new CContentPart( oItem );
							else if ( _T("w:continuationSeparator") == sName )
								pItem = new CContinuationSeparator( oItem );
							else if ( _T("w:cr") == sName )
								pItem = new CCr( oItem );
							else if ( _T("w:dayLong") == sName )
								pItem = new CDayLong( oItem );
							else if ( _T("w:dayShort") == sName )
								pItem = new CDayShort( oItem );
							else if ( _T("w:delInstrText") == sName )
								pItem = new CDelInstrText( oItem );
							else if ( _T("w:delText") == sName )
								pItem = new CDelText( oItem );
							else if ( _T("w:drawing") == sName ) 
								pItem = new CDrawing( oItem );
							else if ( _T("w:endnoteRef") == sName )
								pItem = new CEndnoteRef( oItem );
							else if ( _T("w:endnoteReference") == sName )
								pItem = new CEndnoteReference( oItem );
							else if ( _T("w:fldChar") == sName )
								pItem = new CFldChar( oItem );
							else if ( _T("w:footnoteRef") == sName )
								pItem = new CFootnoteRef( oItem );
							else if ( _T("w:footnoteReference") == sName )
								pItem = new CFootnoteReference( oItem );
							else if ( _T("w:instrText") == sName )
								pItem = new CInstrText( oItem );
							else if ( _T("w:lastRenderedPageBreak") == sName )
								pItem = new CLastRenderedPageBreak( oItem );
							else if ( _T("w:monthLong") == sName )
								pItem = new CMonthLong( oItem );
							else if ( _T("w:monthShort") == sName )
								pItem = new CMonthShort( oItem );
							else if ( _T("w:noBreakHyphen") == sName )
								pItem = new CNoBreakHyphen( oItem );
							else if ( _T("w:object") == sName )
								pItem = new CObject( oItem );
							else if ( _T("w:pgNum") == sName )
								pItem = new CPgNum( oItem );
							else if ( _T("w:pict") == sName )
								pItem = new CPicture( oItem );
							else if ( _T("w:ptab") == sName )
								pItem = new CPTab( oItem );
							else if ( _T("w:rPr") == sName )
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
							else if ( _T("w:ruby") == sName )
								pItem = new CRuby( oItem );
							else if ( _T("w:separator") == sName )
								pItem = new CSeparator( oItem );
							else if ( _T("w:softHyphen") == sName )
								pItem = new CSoftHyphen( oItem );
							else if ( _T("w:sym") == sName )
								pItem = new CSym( oItem );
							else if ( _T("w:t") == sName )
								pItem = new CText( oItem );
							else if ( _T("w:tab") == sName )
								pItem = new CTab( oItem );
							else if ( _T("w:yearLong") == sName )
								pItem = new CYearLong( oItem );
							else if ( _T("w:yearShort") == sName )
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_oRunProperty = NULL;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				fromXMLElems(oReader);
			}
			void fromXMLElems(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				WritingElement *pItem = NULL;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					pItem = fromXMLElem(oReader);
					if ( pItem )
						m_arrItems.push_back( pItem );
				}
			}

			WritingElement* fromXMLElem(XmlUtils::CXmlLiteReader& oReader)
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("mc:AlternateContent") == sName )
					pItem = new CAlternateContent( oReader );
				else if ( _T("w:annotationRef") == sName )
					pItem = new CAnnotationRef( oReader );
				else if ( _T("w:br") == sName )
					pItem = new CBr( oReader );
				else if ( _T("w:commentReference") == sName )
					pItem = new CCommentReference( oReader );
				else if ( _T("w:contentPart") == sName )
					pItem = new CContentPart( oReader );
				else if ( _T("w:continuationSeparator") == sName )
					pItem = new CContinuationSeparator( oReader );
				else if ( _T("w:cr") == sName )
					pItem = new CCr( oReader );
				else if ( _T("w:dayLong") == sName )
					pItem = new CDayLong( oReader );
				else if ( _T("w:dayShort") == sName )
					pItem = new CDayShort( oReader );
				else if ( _T("w:delInstrText") == sName )
					pItem = new CDelInstrText( oReader );
				else if ( _T("w:delText") == sName )
					pItem = new CDelText( oReader );
				else if ( _T("w:drawing") == sName ) 
					pItem = new CDrawing( oReader );
				else if ( _T("w:endnoteRef") == sName )
					pItem = new CEndnoteRef( oReader );
				else if ( _T("w:endnoteReference") == sName )
					pItem = new CEndnoteReference( oReader );
				else if ( _T("w:fldChar") == sName )
					pItem = new CFldChar( oReader );
				else if ( _T("w:footnoteRef") == sName )
					pItem = new CFootnoteRef( oReader );
				else if ( _T("w:footnoteReference") == sName )
					pItem = new CFootnoteReference( oReader );
				else if ( _T("w:instrText") == sName )
					pItem = new CInstrText( oReader );
				else if ( _T("w:lastRenderedPageBreak") == sName )
					pItem = new CLastRenderedPageBreak( oReader );
				else if ( _T("w:monthLong") == sName )
					pItem = new CMonthLong( oReader );
				else if ( _T("w:monthShort") == sName )
					pItem = new CMonthShort( oReader );
				else if ( _T("w:noBreakHyphen") == sName )
					pItem = new CNoBreakHyphen( oReader );
				else if ( _T("w:object") == sName )
					pItem = new CObject( oReader );
				else if ( _T("w:pgNum") == sName )
					pItem = new CPgNum( oReader );
				else if ( _T("w:pict") == sName )
					pItem = new CPicture( oReader );
				else if ( _T("w:ptab") == sName )
					pItem = new CPTab( oReader );
				else if ( _T("w:rPr") == sName )
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
				}
				else if ( _T("w:ruby") == sName )
					pItem = new CRuby( oReader );
				else if ( _T("w:separator") == sName )
					pItem = new CSeparator( oReader );
				else if ( _T("w:softHyphen") == sName )
					pItem = new CSoftHyphen( oReader );
				else if ( _T("w:sym") == sName )
					pItem = new CSym( oReader );
				else if ( _T("w:t") == sName )
				{
					fromXMLText(oReader);
				}
				else if ( _T("w:tab") == sName )
					pItem = new CTab( oReader );
				else if ( _T("w:yearLong") == sName )
					pItem = new CYearLong( oReader );

				return pItem;
			}
			void fromXMLText(XmlUtils::CXmlLiteReader& oReader)
			{
				//for <w:t>6<w:cr/>6</w:t>
				nullable<SimpleTypes::CXmlSpace<> > oSpace;
				CText::ReadAttributes(oReader, oSpace);

				if ( oReader.IsEmptyNode() )
					return;

				bool bTrimLeft, bTrimRight;
				bTrimLeft = bTrimRight = !(oSpace.IsInit() && SimpleTypes::xmlspacePreserve == oSpace->GetValue());
				CText* pTextLast = NULL;
				int nDepth = oReader.GetDepth();
				XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
				while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
				{
					if (eNodeType == XmlUtils::XmlNodeType_Text || eNodeType == XmlUtils::XmlNodeType_Whitespace || eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE)
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
							NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, strlen(pValue), pTextLast->m_sText);
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
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<w:r ");

				ComplexTypes_WriteAttribute( _T("w:rsidDel=\""), m_oRsidDel );
				ComplexTypes_WriteAttribute( _T("w:rsidR=\""),   m_oRsidR );
				ComplexTypes_WriteAttribute( _T("w:rsidRPr=\""), m_oRsidRPr );

				sResult += _T(">");

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

				sResult += _T("</w:r>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_r;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:rsidDel") == wsName )
						m_oRsidDel = oReader.GetText();
					else if ( _T("w:rsidR") == wsName )
						m_oRsidR = oReader.GetText();
					else if ( _T("w:rsidRPr") == wsName )
						m_oRsidRPr = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}
		public:
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRPr;
			OOX::Logic::CRunProperty				*m_oRunProperty;	// копия того что в m_arrItems...  - для быстрого доступа/анализа
			// по идее нужно сделать как в Drawing::Run - то есть единственные подобъекты вынести отдельно
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RUN_INCLUDE_H_
