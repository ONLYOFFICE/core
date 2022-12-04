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

#include "TextFont.h"

namespace PPTX
{
	namespace Logic
	{		
		TextFont::TextFont()
		{
			m_eType = OOX::et_Unknown;
		}
		TextFont::~TextFont() {}
		TextFont::TextFont(const TextFont& oSrc) { *this = oSrc; }
		OOX::EElementType TextFont::getType () const
		{
			return m_eType;
		}
		void TextFont::fromXML(XmlUtils::CXmlNode& node)
		{
			m_eType = OOX::et_Unknown;
			m_name	= node.GetName();

			if ( _T("a:cs") == m_name )
				m_eType = OOX::et_a_cs;
			else if ( _T("a:ea") == m_name )
				m_eType = OOX::et_a_ea;
			else if ( _T("a:latin") == m_name )
				m_eType = OOX::et_a_latin;
			else if ( _T("a:sym") == m_name )
				m_eType = OOX::et_a_sym;

			XmlMacroReadAttributeBase(node, L"charset", charset);
			XmlMacroReadAttributeBase(node, L"panose", panose);
			XmlMacroReadAttributeBase(node, L"pitchFamily", pitchFamily);

			typeface = node.GetAttribute(_T("typeface"));
		}
		void TextFont::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = OOX::et_Unknown;
			m_name = oReader.GetName();

			if ( _T("a:cs") == m_name )
				m_eType = OOX::et_a_cs;
			else if ( _T("a:ea") == m_name )
				m_eType = OOX::et_a_ea;
			else if ( _T("a:latin") == m_name )
				m_eType = OOX::et_a_latin;
			else if ( _T("a:sym") == m_name )
				m_eType = OOX::et_a_sym;

			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring TextFont::toXML() const
		{
			std::wstring name = m_name;
			if (name.empty())
			{
				switch ( m_eType )
				{
					case OOX::et_a_cs:    name = _T("a:cs");	break;
					case OOX::et_a_ea:    name = _T("a:ea");	break;
					case OOX::et_a_latin: name = _T("a:latin"); break;
					case OOX::et_a_sym:   name = _T("a:sym");	break;
					default: return _T("");
				}
			}

			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("typeface"), typeface);
			oAttr.Write(_T("pitchFamily"), pitchFamily);
			oAttr.Write(_T("charset"), charset);
			oAttr.Write(_T("panose"), panose);

			return XmlUtils::CreateNode(name, oAttr);
		}
		void TextFont::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;

			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar0 = wsName[0];

				switch ( wsChar0 )
				{
				case 'c':
					if      ( _T("charset")     == wsName ) charset = oReader.GetText();
					break;

				case 'p':
					if      ( _T("panose")      == wsName ) panose = oReader.GetText();
					else if ( _T("pitchFamily") == wsName ) pitchFamily = oReader.GetText();
					break;

				case 't':
					if      ( _T("typeface")    == wsName ) typeface = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}
		void TextFont::Merge(nullable<TextFont>& font)const
		{
			if(!font.is_init())
				font = TextFont();

			if(charset.is_init())		font->charset		= *charset;
			if(panose.is_init())		font->panose		= *panose;
			if(pitchFamily.is_init())	font->pitchFamily	= *pitchFamily;

			font->typeface = typeface;
		}
		void TextFont::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, charset);
			pWriter->WriteString2(1, panose);
			pWriter->WriteString2(2, pitchFamily);

			/*
			pWriter->WriteString1(3, typeface);
			*/

			std::wstring sPick = pWriter->m_pCommon->m_pNativePicker->GetTypefacePick(*this);
			pWriter->WriteString1(3, sPick);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void TextFont::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						charset = pReader->GetString2();
						break;
					}
					case 1:
					{
						panose = pReader->GetString2();
						break;
					}
					case 2:
					{
						pitchFamily = pReader->GetString2();
						break;
					}
					case 3:
					{
						typeface = pReader->GetString2();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TextFont::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("typeface"), XmlUtils::EncodeXmlString(typeface));
			pWriter->WriteAttribute(_T("pitchFamily"), pitchFamily);
			pWriter->WriteAttribute(_T("charset"), charset);
			pWriter->WriteAttribute(_T("panose"), panose);
			pWriter->EndAttributes();

			pWriter->EndNode(m_name);
		}
		void TextFont::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
