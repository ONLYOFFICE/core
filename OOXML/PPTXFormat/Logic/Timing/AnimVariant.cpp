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

#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{		
		void AnimVariant::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = XmlUtils::GetNameNoNS(node.GetName());

			XmlUtils::CXmlNode oNode;

			if (node.GetNode(_T("p:boolVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", boolVal)
			else if (node.GetNode(_T("p:intVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", intVal)
			else if (node.GetNode(_T("p:fltVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", fltVal)
			else if (node.GetNode(_T("p:clrVal"), oNode))
				clrVal.GetColorFrom(oNode);
			else if (node.GetNode(_T("p:strVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", strVal)

			FillParentPointersForChilds();
		}
		std::wstring AnimVariant::toXML() const
		{
			XmlUtils::CNodeValue oValue;

			if (strVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), strVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:strVal"), oAttr);
			}
			if (boolVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), boolVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:boolVal"), oAttr);
			}
			if (intVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), intVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:intVal"), oAttr);
			}
			if (fltVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), fltVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:fltVal"), oAttr);
			}
			if (clrVal.is_init())
			{
				oValue.m_strValue += (_T("<p:clrVal>") + clrVal.toXML() + _T("</p:clrVal>"));
			}
			return XmlUtils::CreateNode(_T("p:") + node_name, oValue);
		}
		void AnimVariant::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void AnimVariant::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, boolVal);
			pWriter->WriteString2(1, strVal);
			pWriter->WriteInt2(2, intVal);
			pWriter->WriteDouble2(3, fltVal);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, clrVal);
		}
		void AnimVariant::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	boolVal = pReader->GetBool();
				else if (1 == _at)	strVal = pReader->GetString2();
				else if (2 == _at)	intVal = pReader->GetLong();
				else if (3 == _at)	fltVal = pReader->GetDouble();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
				case 0:
				{
					clrVal.fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(end);
		}
		void AnimVariant::FillParentPointersForChilds()
			{
				clrVal.SetParentPointer(this);
			}		
	} // namespace Logic
} // namespace PPTX
