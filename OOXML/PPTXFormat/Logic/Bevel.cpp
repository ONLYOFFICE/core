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

#include "Bevel.h"

namespace PPTX
{
	namespace Logic
	{
		Bevel::Bevel(const std::wstring name)
		{
			m_name = name;
		}
		Bevel::~Bevel() {}
		Bevel::Bevel(const Bevel& oSrc) { *this = oSrc; }
		OOX::EElementType Bevel::getType() const
		{
			return OOX::et_a_bevel;
		}
		void Bevel::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name	= XmlUtils::GetNameNoNS(oReader.GetName());

			ReadAttributes( oReader );
		}
		void Bevel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("w"), w)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("h"), h)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("prst"), prst)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Bevel::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name	= XmlUtils::GetNameNoNS(node.GetName());

			XmlMacroReadAttributeBase(node, L"w", w);
			XmlMacroReadAttributeBase(node, L"h", h);
			XmlMacroReadAttributeBase(node, L"prst", prst);
		}
		std::wstring Bevel::toXML() const
		{
			std::wstring namespace_ = XmlUtils::GetNamespace(m_name);
			if (namespace_ == L"w14")
			{
				namespace_ + L"w14:";
			}
			else namespace_.clear();

			XmlUtils::CAttribute oAttr;
			oAttr.Write(namespace_ + L"w", w);
			oAttr.Write(namespace_ + L"h", h);
			oAttr.WriteLimitNullable(namespace_ + L"prst", prst);

			return XmlUtils::CreateNode(m_name, oAttr);
		}
		void Bevel::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = _T("w14:");
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = _T("a:");


			pWriter->StartNode(sNodeNamespace + m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"w", w);
			pWriter->WriteAttribute(sAttrNamespace + L"h", h);
			if (prst.IsInit())
			{
				pWriter->WriteAttribute(sAttrNamespace + L"prst", prst->get());
			}
			pWriter->EndAttributes();

			pWriter->EndNode(sNodeNamespace + m_name);
		}
		void Bevel::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, w);
			pWriter->WriteInt2(1, h);
			pWriter->WriteLimit2(2, prst);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Bevel::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		w = pReader->GetLong();
				else if (1 == _at)	h = pReader->GetLong();
				else if (2 == _at)	prst = pReader->GetUChar();
				else
					break;
			}

			pReader->Seek(_end_rec);
		}
		void Bevel::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
