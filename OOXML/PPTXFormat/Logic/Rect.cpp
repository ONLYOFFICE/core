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

#include "Rect.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType Rect::getType() const
		{
			return OOX::et_a_rect;
		}
		void Rect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();

			ReadAttributes( oReader );
		}
		void Rect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("t"), t )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("l"), l )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r"), r )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("b"), b )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Rect::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();

			XmlMacroReadAttributeBase(node, L"t", t);
			XmlMacroReadAttributeBase(node, L"b", b);
			XmlMacroReadAttributeBase(node, L"l", l);
			XmlMacroReadAttributeBase(node, L"r", r);
		}
		std::wstring Rect::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("l"), l);
			oAttr.Write(_T("t"), t);
			oAttr.Write(_T("r"), r);
			oAttr.Write(_T("b"), b);

			return XmlUtils::CreateNode(m_name, oAttr);
		}
		void Rect::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring _name;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				_name = _T("w14:") + XmlUtils::GetNameNoNS(m_name);
				sAttrNamespace = _T("w14:");
			}
			else
				_name = m_name;
			pWriter->StartNode(_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + _T("l"), l);
			pWriter->WriteAttribute(sAttrNamespace + _T("t"), t);
			pWriter->WriteAttribute(sAttrNamespace + _T("r"), r);
			pWriter->WriteAttribute(sAttrNamespace + _T("b"), b);
			pWriter->EndAttributes();

			pWriter->EndNode(_name);
		}
		void Rect::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, l);
			pWriter->WriteString2(1, t);
			pWriter->WriteString2(2, r);
			pWriter->WriteString2(3, b);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Rect::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						l = pReader->GetString2();
						break;
					}
					case 1:
					{
						t = pReader->GetString2();
						break;
					}
					case 2:
					{
						r = pReader->GetString2();
						break;
					}
					case 3:
					{
						b = pReader->GetString2();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void Rect::FillParentPointersForChilds() {}
		std::wstring Rect::GetODString() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("l"), l);
			oAttr.Write(_T("t"), t);
			oAttr.Write(_T("r"), r);
			oAttr.Write(_T("b"), b);

			return XmlUtils::CreateNode(XmlUtils::GetNameNoNS(m_name), oAttr);
		}
	} // namespace Logic
} // namespace PPTX
