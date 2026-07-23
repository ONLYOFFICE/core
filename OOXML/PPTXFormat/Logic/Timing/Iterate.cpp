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

#include "Iterate.h"

namespace PPTX
{
	namespace Logic
	{
		void Iterate::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"type", type);
			XmlMacroReadAttributeBase(node, L"backwards", backwards);

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("p:tmAbs"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", tmAbs)
			else if (node.GetNode(_T("p:tmPct"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", tmPct)

			FillParentPointersForChilds();
		}
		std::wstring Iterate::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("type"), type);
			oAttr.Write(_T("backwards"), backwards);

			if (tmAbs.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("val"), tmAbs);
				return XmlUtils::CreateNode(_T("p:iterate"), oAttr, XmlUtils::CreateNode(_T("p:tmAbs"), oAttr1));
			}
			else if (tmPct.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("val"), tmPct);
				return XmlUtils::CreateNode(_T("p:iterate"), oAttr, XmlUtils::CreateNode(_T("p:tmPct"), oAttr1));
			}

			return XmlUtils::CreateNode(_T("p:iterate"), oAttr);
		}
		void Iterate::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void Iterate::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, type);
				pWriter->WriteBool2(1, backwards);
				pWriter->WriteString2(2, tmAbs);
				pWriter->WriteInt2(3, tmPct);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Iterate::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	type = pReader->GetUChar();
				else if (1 == _at)	backwards = pReader->GetBool();
				else if (2 == _at)	tmAbs = pReader->GetString2();
				else if (3 == _at)	tmPct = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void Iterate::FillParentPointersForChilds(){}

	} // namespace Logic
} // namespace PPTX
