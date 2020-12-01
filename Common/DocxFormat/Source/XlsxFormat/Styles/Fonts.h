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

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CFont : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFont)
			CFont()
			{
			}
			virtual ~CFont()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{ 
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLWithNS(writer, L"", L"font", L"");
			}
			virtual void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				writer.EndAttributes();
				if(m_oRFont.IsInit() && m_oRFont->m_sVal.IsInit())
				{
					//todo more complex solution
					//if name more then 31 chars Excel wants to recover xlsx
					if (m_oRFont->m_sVal->length() <= 31)
					{
						WritingValNodeEncodeXml(child_ns, L"name", m_oRFont->m_sVal.get());
					}
					else
					{
						WritingValNodeEncodeXml(child_ns, L"name", m_oRFont->m_sVal->substr(0, 31));
					}
				}			
				if(m_oCharset.IsInit() && m_oCharset->m_oCharset.IsInit())
				{
					WritingValNode(child_ns, L"charset", m_oCharset->m_oCharset->ToString());
				}
				if(m_oFamily.IsInit() && m_oFamily->m_oFontFamily.IsInit())
				{
					WritingValNode(child_ns, L"family", m_oFamily->m_oFontFamily->ToString());
				}
				if(m_oBold.IsInit())
				{
					WritingValNodeIf(child_ns, L"b", !m_oBold->ToBool(), L"0");
				}
				if(m_oItalic.IsInit())
				{
					WritingValNodeIf(child_ns, L"i", !m_oItalic->ToBool(), L"0");
				}
				if(m_oStrike.IsInit())
				{
					WritingValNodeIf(child_ns, L"strike", !m_oStrike->ToBool(), L"0");
				}
				if(m_oOutline.IsInit())
				{
					WritingValNodeIf(child_ns, L"outline", !m_oOutline->ToBool(), L"0");
				}
				if(m_oShadow.IsInit())
				{
					WritingValNodeIf(child_ns, L"shadow", !m_oShadow->ToBool(), L"0");
				}
				if(m_oCondense.IsInit())
				{
					WritingValNodeIf(child_ns, L"condense", !m_oCondense->ToBool(), L"0");
				}
				if(m_oExtend.IsInit())
				{
					WritingValNodeIf(child_ns, L"extend", !m_oExtend->ToBool(), L"0");
				}
				if(m_oColor.IsInit())
					m_oColor->toXMLWithNS(writer, child_ns, L"color", child_ns);
				if(m_oSz.IsInit() && m_oSz->m_oVal.IsInit())
				{
					WritingValNode(child_ns, L"sz", m_oSz->m_oVal->GetValue());
				}
				if(m_oUnderline.IsInit() && m_oUnderline->m_oUnderline.IsInit())
				{
					WritingValNodeIf(child_ns, L"u", SimpleTypes::Spreadsheet::underlineSingle != m_oUnderline->m_oUnderline->GetValue(), m_oUnderline->m_oUnderline->ToString());
				}
				if(m_oVertAlign.IsInit() && m_oVertAlign->m_oVerticalAlign.IsInit())
				{
					WritingValNode(child_ns, L"vertAlign", m_oVertAlign->m_oVerticalAlign->ToString());
				}
				if(m_oScheme.IsInit() && m_oScheme->m_oFontScheme.IsInit())
				{
					WritingValNode(child_ns, L"scheme", m_oScheme->m_oFontScheme->ToString());
				}
				writer.EndNodeWithNS(node_ns, node_name);
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"b" == sName )
						m_oBold = oReader;
					else if ( L"charset" == sName )
						m_oCharset = oReader;
					else if ( L"color" == sName )
						m_oColor = oReader;
					else if ( L"condense" == sName )
						m_oCondense = oReader;
					else if ( L"extend" == sName )
						m_oExtend = oReader;
					else if ( L"family" == sName )
						m_oFamily = oReader;
					else if ( L"i" == sName )
						m_oItalic = oReader;
					else if ( L"name" == sName )
						m_oRFont = oReader;
					else if ( L"outline" == sName )
						m_oOutline = oReader;
					else if ( L"scheme" == sName )
						m_oScheme = oReader;
					else if ( L"shadow" == sName )
						m_oShadow = oReader;
					else if ( L"strike" == sName )
						m_oStrike = oReader;
					else if ( L"sz" == sName )
						m_oSz = oReader;
					else if ( L"u" == sName )
						m_oUnderline = oReader;
					else if ( L"vertAlign" == sName )
						m_oVertAlign = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Font;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0) return;

				nullable_bool bItalic, bBold;
				nullable_string sColor, sFamily, sFont;
				nullable<SimpleTypes::Spreadsheet::CFontFamily<>> oFamily;
				nullable<SimpleTypes::Spreadsheet::CUnderline<>> oUnderline;
				nullable<SimpleTypes::Spreadsheet::CFontCharset<>> oCharset;
				nullable_double dSz;

				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:FontName"),	sFont )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("x:Family"),	oFamily )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("x:CharSet"),	oCharset )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:Size"),		dSz )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:Color"),	sColor )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:Underline"),oUnderline )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:Bold"),		bBold )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:Italic"),	bItalic )
				WritingElement_ReadAttributes_End( oReader )
				
				if (sColor.IsInit())
				{
					m_oColor.Init(); m_oColor->m_oRgb.Init();
					m_oColor->m_oRgb->FromString(*sColor);
				}
				if (oUnderline.IsInit())
				{
					m_oUnderline.Init();
					m_oUnderline->m_oUnderline = oUnderline;
				}
				if (sFont.IsInit())
				{
					m_oRFont.Init();
					m_oRFont->m_sVal = sFont;
				}
				if (sFamily.IsInit())
				{
					m_oFamily.Init();
					m_oFamily->m_oFontFamily = oFamily;
				}
				if (dSz.IsInit())
				{
					m_oSz.Init(); m_oSz->m_oVal.Init();
					m_oSz->m_oVal->SetValue(*dSz);
				}
				if (oCharset.IsInit())
				{
					m_oCharset.Init();
					m_oCharset->m_oCharset = oCharset;
				}
				if (bBold.IsInit())
				{
					m_oBold.Init();
					m_oBold->m_oVal.FromBool(*bBold);
				}
				if (bItalic.IsInit())
				{
					m_oItalic.Init();
					m_oItalic->m_oVal.FromBool(*bItalic);
				}
			}
		public:
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oBold;
			nullable<CCharset>														m_oCharset;
			nullable<CColor>														m_oColor;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oCondense;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oExtend;
			nullable<CFontFamily >													m_oFamily;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oItalic;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oOutline;
			nullable<ComplexTypes::Spreadsheet::String>								m_oRFont;
			nullable<CFontScheme>													m_oScheme;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oShadow;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oStrike;
			nullable<ComplexTypes::Spreadsheet::CDouble>							m_oSz;
			nullable<CUnderline>													m_oUnderline;
			nullable<CVerticalAlign>												m_oVertAlign;
		};
		class CFonts : public WritingElementWithChilds<CFont>
		{
		public:
			WritingElement_AdditionConstructors(CFonts)
			CFonts()
			{
			}
			virtual ~CFonts()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<fonts");
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(L">");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				writer.WriteString(L"</fonts>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int index = 0;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"font" == sName )
					{
						CFont *pFont = new CFont( oReader );
						m_arrItems.push_back( pFont );
						m_mapFonts.insert(std::make_pair(index++, pFont));
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Fonts;
			}
			void AddFont (CFont* pFont)
			{
				m_arrItems.push_back(pFont);
				if(false == m_oCount.IsInit())
					m_oCount.Init();
				m_oCount->SetValue((unsigned int)m_arrItems.size());
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"count", m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCount;
			std::map<int, CFont*>							m_mapFonts;
		};
	} //Spreadsheet
} // namespace OOX

