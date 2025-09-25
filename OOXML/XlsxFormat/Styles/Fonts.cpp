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

#include "Fonts.h"

#include "../ComplexTypes_Spreadsheet.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"
#include "../../XlsbFormat/Biff12_records/BeginFonts.h"

#include "../../XlsbFormat/Biff12_unions/FONTS.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CFont::CFont()
		{
		}
		CFont::~CFont()
		{
		}
		void CFont::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFont::toXML() const
		{
			return L"";
		}
		void CFont::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLWithNS(writer, L"", L"font", L"");
		}
		void CFont::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
			writer.EndAttributes();
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
			if(m_oCondense.IsInit())
			{
				WritingValNodeIf(child_ns, L"condense", !m_oCondense->ToBool(), L"0");
			}
			if(m_oExtend.IsInit())
			{
				WritingValNodeIf(child_ns, L"extend", !m_oExtend->ToBool(), L"0");
			}
			if(m_oOutline.IsInit())
			{
				WritingValNodeIf(child_ns, L"outline", !m_oOutline->ToBool(), L"0");
			}
			if(m_oShadow.IsInit())
			{
				WritingValNodeIf(child_ns, L"shadow", !m_oShadow->ToBool(), L"0");
			}
			if(m_oUnderline.IsInit() && m_oUnderline->m_oUnderline.IsInit())
			{
				WritingValNodeIf(child_ns, L"u", SimpleTypes::Spreadsheet::underlineSingle != m_oUnderline->m_oUnderline->GetValue(), m_oUnderline->m_oUnderline->ToString());
			}
			if(m_oVertAlign.IsInit() && m_oVertAlign->m_oVerticalAlign.IsInit())
			{
				WritingValNode(child_ns, L"vertAlign", m_oVertAlign->m_oVerticalAlign->ToString());
			}
			if(m_oSz.IsInit() && m_oSz->m_oVal.IsInit())
			{
				WritingValNode(child_ns, L"sz", m_oSz->m_oVal->GetValue());
			}
			if(m_oColor.IsInit())
				m_oColor->toXMLWithNS(writer, child_ns, L"color", child_ns);

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
			if(m_oFamily.IsInit() && m_oFamily->m_oFontFamily.IsInit())
			{
				WritingValNode(child_ns, L"family", m_oFamily->m_oFontFamily->ToString());
			}
			if(m_oCharset.IsInit() && m_oCharset->m_oCharset.IsInit())
			{
				WritingValNode(child_ns, L"charset", m_oCharset->m_oCharset->ToString());
			}
			if(m_oScheme.IsInit() && m_oScheme->m_oFontScheme.IsInit())
			{
				WritingValNode(child_ns, L"scheme", m_oScheme->m_oFontScheme->ToString());
			}
			writer.EndNodeWithNS(node_ns, node_name);
		}
		void CFont::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void CFont::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CFont::toBin()
		{
			auto ptr(new XLSB::Font);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oSz.IsInit())
				ptr->dyHeight = m_oSz->m_oVal->GetValue() * 20;

			if(m_oItalic.IsInit())
				ptr->fItalic = m_oItalic->ToBool();
			else
				ptr->fItalic = false;

			if(m_oStrike.IsInit())
				ptr->fStrikeOut = m_oStrike->ToBool();
			else
				ptr->fStrikeOut = false;

			if(m_oOutline.IsInit())
				ptr->fOutline = m_oOutline->ToBool();
			else
				ptr->fOutline = false;

			if(m_oShadow.IsInit())
				ptr->fShadow = m_oShadow->ToBool();
			else
				ptr->fShadow = false;

			if(m_oCondense.IsInit())
				ptr->fCondense = m_oCondense->ToBool();
			else
				ptr->fCondense = false;

			if(m_oExtend.IsInit())
				ptr->fExtend = m_oExtend->ToBool();
			else
				ptr->fExtend = false;
			if(m_oBold.IsInit())
			{
				if(m_oBold->ToBool())
					ptr->bls = 0x02BC;
			}
			else
			{
				ptr->bls = 0x0190;
			}
			if(m_oUnderline.IsInit())
			{
				if(m_oUnderline->m_oUnderline.IsInit())
				{
					if(m_oUnderline->m_oUnderline == SimpleTypes::Spreadsheet::EUnderline::underlineNone)
						ptr->uls = 0;
					else if(m_oUnderline->m_oUnderline == SimpleTypes::Spreadsheet::EUnderline::underlineSingle)
						ptr->uls = 1;
					else if(m_oUnderline->m_oUnderline == SimpleTypes::Spreadsheet::EUnderline::underlineDouble)
						ptr->uls = 2;
					else if(m_oUnderline->m_oUnderline == SimpleTypes::Spreadsheet::EUnderline::underlineSingleAccounting)
						ptr->uls = 33;
					else if(m_oUnderline->m_oUnderline == SimpleTypes::Spreadsheet::EUnderline::underlineDoubleAccounting)
						ptr->uls = 34;
				}
			}
            else
                ptr->uls = 0;

			if(m_oFamily.IsInit())
				ptr->bFamily = m_oFamily->m_oFontFamily->GetValue();

			if(m_oCharset.IsInit())
				ptr->bCharSet = m_oCharset->m_oCharset->GetValue();

			if(m_oColor.IsInit())
				ptr->brtColor = m_oColor->toColor();
			else
			{
				m_oColor.Init();
                ptr->brtColor = m_oColor->GetDefaultColor();
			}

			if(m_oScheme.IsInit())
			{
				if(m_oScheme->m_oFontScheme.IsInit())
				{
					if(m_oScheme->m_oFontScheme == SimpleTypes::Spreadsheet::EFontScheme::fontschemeNone)
						ptr->bFontScheme = 0;
					else if(m_oScheme->m_oFontScheme == SimpleTypes::Spreadsheet::EFontScheme::fontschemeMajor)
						ptr->bFontScheme = 1;
					else if(m_oScheme->m_oFontScheme == SimpleTypes::Spreadsheet::EFontScheme::fontschemeMinor)
						ptr->bFontScheme = 2;
				}
			}


			if(m_oRFont.IsInit())
				ptr->fontName = m_oRFont->m_sVal.get();
			if(m_oVertAlign.IsInit())
			{
				if(m_oVertAlign->m_oVerticalAlign.IsInit())
				{
					if(m_oVertAlign->m_oVerticalAlign->GetValue() == SimpleTypes::EVerticalAlignRun::verticalalignrunBaseline)
						ptr->sss = 0;
					else if(m_oVertAlign->m_oVerticalAlign->GetValue() == SimpleTypes::EVerticalAlignRun::verticalalignrunSuperscript)
						ptr->sss = 1;
					else if(m_oVertAlign->m_oVerticalAlign->GetValue() == SimpleTypes::EVerticalAlignRun::verticalalignrunSubscript)
						ptr->sss = 2;
				}
			}

			return objectPtr;
		}
		EElementType CFont::getType () const
		{
			return et_x_Font;
		}
		void CFont::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0) return;

			nullable_bool bItalic, bBold;
			nullable_string sColor, sFamily, sFont;
			nullable<SimpleTypes::Spreadsheet::CFontFamily> oFamily;
			nullable<SimpleTypes::Spreadsheet::CUnderline> oUnderline;
			nullable<SimpleTypes::Spreadsheet::CFontCharset> oCharset;
			nullable<SimpleTypes::CVerticalAlignRun> oVerticalAlignment;
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
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:Italic"),	bItalic )
				WritingElement_ReadAttributes_Read_if(oReader, _T("ss:VerticalAlign"), oVerticalAlignment)
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
			if (oVerticalAlignment.IsInit())
			{
				m_oVertAlign.Init();
				m_oVertAlign->m_oVerticalAlign = oVerticalAlignment;
			}
		}
		void CFont::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Font*>(obj.get());
			if(ptr != nullptr)
			{
				m_oSz.Init(); m_oSz->m_oVal.Init();
				m_oSz->m_oVal = (double)ptr->dyHeight/20.;

				m_oItalic.Init();
				m_oItalic->FromBool(ptr->fItalic);

				m_oStrike.Init();
				m_oStrike->FromBool(ptr->fStrikeOut);

				m_oOutline.Init();
				m_oOutline->FromBool(ptr->fOutline);

				m_oShadow.Init();
				m_oShadow->FromBool(ptr->fShadow);

				m_oCondense.Init();
				m_oCondense->FromBool(ptr->fCondense);

				m_oExtend.Init();
				m_oExtend->FromBool(ptr->fExtend);

				if(ptr->bls == 0x02BC)
				{
					m_oBold.Init();
					m_oBold->FromBool(true);
				}

				m_oUnderline.Init();
				m_oUnderline->m_oUnderline.Init();
				switch(ptr->uls)
				{
					case 0:
					   m_oUnderline->m_oUnderline = SimpleTypes::Spreadsheet::EUnderline::underlineNone; break;
					case 1:
						m_oUnderline->m_oUnderline = SimpleTypes::Spreadsheet::EUnderline::underlineSingle; break;
					case 2:
						m_oUnderline->m_oUnderline = SimpleTypes::Spreadsheet::EUnderline::underlineDouble; break;
					case 33:
						m_oUnderline->m_oUnderline = SimpleTypes::Spreadsheet::EUnderline::underlineSingleAccounting; break;
					case 34:
						m_oUnderline->m_oUnderline = SimpleTypes::Spreadsheet::EUnderline::underlineDoubleAccounting; break;
				}

				m_oFamily.Init();
				m_oFamily->m_oFontFamily = (SimpleTypes::Spreadsheet::EFontFamily)ptr->bFamily;

				m_oCharset.Init();
				m_oCharset->m_oCharset = (SimpleTypes::Spreadsheet::EFontCharset)ptr->bCharSet;

				m_oColor.Init();
				m_oColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtColor));

				m_oScheme.Init();
				m_oScheme->m_oFontScheme.Init();
				switch(ptr->bFontScheme)
				{
					case 0:
					   m_oScheme->m_oFontScheme = SimpleTypes::Spreadsheet::EFontScheme::fontschemeNone; break;
					case 1:
						m_oScheme->m_oFontScheme = SimpleTypes::Spreadsheet::EFontScheme::fontschemeMajor; break;
					case 2:
						m_oScheme->m_oFontScheme = SimpleTypes::Spreadsheet::EFontScheme::fontschemeMinor; break;
				}

				m_oRFont.Init();
				m_oRFont->m_sVal = ptr->fontName;

				m_oVertAlign.Init();
				m_oVertAlign->m_oVerticalAlign.Init();
				switch(ptr->sss)
				{
					case 0:
						m_oVertAlign->m_oVerticalAlign->SetValue(SimpleTypes::EVerticalAlignRun::verticalalignrunBaseline); break;
					case 1:
						m_oVertAlign->m_oVerticalAlign->SetValue(SimpleTypes::EVerticalAlignRun::verticalalignrunSuperscript); break;
					case 2:
						m_oVertAlign->m_oVerticalAlign->SetValue(SimpleTypes::EVerticalAlignRun::verticalalignrunSubscript); break;
				}

			}
		}

		CFonts::CFonts()
		{
		}
		CFonts::~CFonts()
		{
		}
		void CFonts::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFonts::toXML() const
		{
			return L"";
		}
		void CFonts::toXML(NSStringUtils::CStringBuilder& writer) const
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
		void CFonts::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
					CFont *pFont = new CFont();
					*pFont = oReader;

					m_arrItems.push_back( pFont );
					m_mapFonts.insert(std::make_pair(index++, pFont));
				}
			}
		}
		void CFonts::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			ReadAttributes(obj);

			int index = 0;

			for(auto &font : obj)
			{
				CFont *pFont = new CFont(font);
				m_arrItems.push_back(pFont);
				m_mapFonts.insert(std::make_pair(index++, pFont));
			}
		}
		XLS::BaseObjectPtr CFonts::toBin()
		{
			auto ptr(new XLSB::FONTS);
			auto ptr1(new XLSB::BeginFonts);
			ptr->m_BrtBeginFonts = XLS::BaseObjectPtr{ptr1};
			XLS::BaseObjectPtr objectPtr(ptr);

			for(auto i:m_arrItems)
			{
				ptr->m_arBrtFont.push_back(i->toBin());
			}
			ptr1->cfonts = ptr->m_arBrtFont.size();
			return objectPtr;
		}
		EElementType CFonts::getType () const
		{
			return et_x_Fonts;
		}
		void CFonts::AddFont (CFont* pFont)
		{
			m_arrItems.push_back(pFont);
			if(false == m_oCount.IsInit())
				m_oCount.Init();
			m_oCount->SetValue((unsigned int)m_arrItems.size());
		}
		void CFonts::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, L"count", m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CFonts::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_UINT32)obj.size();
			}

	} //Spreadsheet
} // namespace OOX

