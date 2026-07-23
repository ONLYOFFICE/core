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

#include "ColorModifier.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType ColorModifier::getType() const
		{
			return OOX::et_a_prstClr;
		}
		void ColorModifier::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			name = oReader.GetName();

			if (XmlUtils::GetNameNoNS(name) == _T("alpha"))
			{
				ReadAttributes2( oReader );
			}
			else
			{
				ReadAttributes( oReader );
			}
		}
		void ColorModifier::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader)
		{
			nullable_string sTmp;
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("val"), sTmp)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			if (sTmp.is_init())
			{
				val = sTmp.get();
				if (val.is_init() && std::wstring::npos != sTmp->find(L"%"))
				{
					*val = (*val) * 1000;
				}
			}
		}
		void ColorModifier::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("val"), val)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void ColorModifier::fromXML(XmlUtils::CXmlNode& node)
		{
			name = node.GetName();
			XmlMacroReadAttributeBase(node, L"val", val);

			if (XmlUtils::GetNameNoNS(name) == _T("alpha"))
			{
				nullable_string sTmp;
				XmlMacroReadAttributeBase(node, L"val", sTmp);

				if (val.is_init() && sTmp.is_init() && std::wstring::npos != sTmp->find(L"%"))
					*val = (*val) * 1000;
			}
		}
		std::wstring ColorModifier::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("val"), val);
			return XmlUtils::CreateNode(name, oAttr);
		}
		ColorModifier& ColorModifier::operator=(const ColorModifier& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			name		= oSrc.name;
			val			= oSrc.val;
			return *this;
		}
		void ColorModifier::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, name);
			pWriter->WriteInt2(1, val);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void ColorModifier::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					name = pReader->GetString2();
				else if (1 == _at)
					val = pReader->GetLong();
				else
					break;
			}
			pReader->Seek(_end_rec);
		}
		void ColorModifier::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring _name;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				_name = L"w14:" + XmlUtils::GetNameNoNS(name);
				sAttrNamespace = L"w14:";
			}
			else
				_name = name;
			pWriter->StartNode(_name);
			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"val", val);
			pWriter->EndAttributes();
			pWriter->EndNode(_name);
		}
		void ColorModifier::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
