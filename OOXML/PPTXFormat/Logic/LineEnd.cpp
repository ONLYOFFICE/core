/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "LineEnd.h"

namespace PPTX
{
	namespace Logic
	{		
		OOX::EElementType LineEnd::getType() const
		{
			return OOX::et_a_buSzPts;
		}
		void LineEnd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();

			ReadAttributes( oReader );
		}
		void LineEnd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w", w )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"type", type )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"len", len )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void LineEnd::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();

			XmlMacroReadAttributeBase(node, L"type", type);
			XmlMacroReadAttributeBase(node, L"w", w);
			XmlMacroReadAttributeBase(node, L"len", len);
		}
		std::wstring LineEnd::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(L"type", type);
			oAttr.WriteLimitNullable(L"w", w);
			oAttr.WriteLimitNullable(L"len", len);

			return XmlUtils::CreateNode(m_name, oAttr);
		}
		void LineEnd::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_name.empty()) return;

			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{				
				m_name = L"w14" + m_name.substr(1);
			}
			pWriter->StartNode(m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"type", type);
			pWriter->WriteAttribute(L"w", w);
			pWriter->WriteAttribute(L"len", len);
			pWriter->EndAttributes();

			pWriter->EndNode(m_name);
		}
		void LineEnd::Merge(nullable<LineEnd>& line)const
		{
			if(!line.IsInit())
				line = LineEnd();
			if(type.IsInit())
				line->type = *type;
			if(w.IsInit())
				line->w = *w;
			if(len.IsInit())
				line->len = *len;
		}
		void LineEnd::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						type = pReader->GetUChar();
						break;
					}
					case 1:
					{
						w = pReader->GetUChar();
						break;
					}
					case 2:
					{
						len = pReader->GetUChar();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void LineEnd::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, type);
			pWriter->WriteLimit2(1, w);
			pWriter->WriteLimit2(2, len);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void LineEnd::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
