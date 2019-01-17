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
#ifndef OOX_FONTS_FILE_INCLUDE_H_
#define OOX_FONTS_FILE_INCLUDE_H_

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
				writer.WriteString(L"<font>");
				if(m_oRFont.IsInit() && m_oRFont->m_sVal.IsInit())
				{
					//todo more complex solution
					//if name more then 31 chars Excel wants to recover xlsx
					if (m_oRFont->m_sVal->length() <= 31)
					{
						WritingStringValAttrEncodeXmlString(L"name", m_oRFont->m_sVal.get());
					}
					else
					{
						WritingStringValAttrEncodeXmlString(L"name", m_oRFont->m_sVal->substr(0, 31));
					}
				}			
				if(m_oCharset.IsInit() && m_oCharset->m_oCharset.IsInit())
				{
					WritingStringValAttrString(L"charset", m_oCharset->m_oCharset->ToString());
				}
				if(m_oFamily.IsInit() && m_oFamily->m_oFontFamily.IsInit())
				{
					WritingStringValAttrString(L"family", m_oFamily->m_oFontFamily->ToString());
				}
				if(m_oBold.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oBold->m_oVal.GetValue())
						writer.WriteString(L"<b/>");
					else
						writer.WriteString(L"<b val=\"0\"/>");
				}
				if(m_oItalic.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oItalic->m_oVal.GetValue())
						writer.WriteString(L"<i/>");
					else
						writer.WriteString(L"<i val=\"0\"/>");
				}
				if(m_oStrike.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oStrike->m_oVal.GetValue())
						writer.WriteString(L"<strike/>");
					else
						writer.WriteString(L"<strike val=\"0\"/>");
				}
				if(m_oOutline.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oOutline->m_oVal.GetValue())
						writer.WriteString(L"<outline/>");
					else
						writer.WriteString(L"<outline val=\"0\"/>");
				}
				if(m_oShadow.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oShadow->m_oVal.GetValue())
						writer.WriteString(L"<shadow/>");
					else
						writer.WriteString(L"<shadow val=\"0\"/>");
				}
				if(m_oCondense.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oCondense->m_oVal.GetValue())
						writer.WriteString(L"<condense/>");
					else
						writer.WriteString(L"<condense val=\"0\"/>");
				}
				if(m_oExtend.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oExtend->m_oVal.GetValue())
						writer.WriteString(L"<extend/>");
					else
						writer.WriteString(L"<extend val=\"0\"/>");
				}
				if(m_oColor.IsInit())
					m_oColor->toXML2(writer, L"color");
				if(m_oSz.IsInit() && m_oSz->m_oVal.IsInit())
				{
					WritingStringValAttrDouble(L"sz", m_oSz->m_oVal->GetValue());
				}
				if(m_oUnderline.IsInit() && m_oUnderline->m_oUnderline.IsInit())
				{
					if (SimpleTypes::underlineSingle != m_oUnderline->m_oUnderline->GetValue())
					{
						WritingStringValAttrString(L"u", m_oUnderline->m_oUnderline->ToString());
					}
					else
					{
						writer.WriteString(L"<u/>");
					}
					
				}
				if(m_oVertAlign.IsInit() && m_oVertAlign->m_oVerticalAlign.IsInit())
				{
					writer.WriteString(L"<vertAlign val=\"");
                    std::wstring sAlign = m_oVertAlign->m_oVerticalAlign->ToString();
                    writer.WriteString(sAlign.c_str());

					writer.WriteString(L"\"/>");
				}
				if(m_oScheme.IsInit() && m_oScheme->m_oFontScheme.IsInit())
				{
					WritingStringValAttrString(L"scheme", m_oScheme->m_oFontScheme->ToString());
				}
				writer.WriteString(L"</font>");
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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

#endif // OOX_FONTS_FILE_INCLUDE_H_
