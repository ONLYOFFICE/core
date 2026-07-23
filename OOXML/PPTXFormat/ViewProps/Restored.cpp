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

#include "Restored.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void Restored::fromXML(XmlUtils::CXmlNode& node)
		{
			name = XmlUtils::GetNameNoNS(node.GetName());

			sz = node.ReadAttributeInt(L"sz");
			XmlMacroReadAttributeBase(node, L"autoAdjust", autoAdjust);

			Normalize();
		}
		std::wstring Restored::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("sz"), sz);
			oAttr.Write(_T("autoAdjust"), autoAdjust);

			return XmlUtils::CreateNode(_T("p:") + name, oAttr);
		}
		void Restored::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt1(0, sz);
			pWriter->WriteBool2(1, autoAdjust);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Restored::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						sz = pReader->GetLong();
					}break;
					case 1:
					{
						autoAdjust = pReader->GetBool();
					}break;
					default:
						break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Restored::FillParentPointersForChilds()
		{
		}
		void Restored::Normalize()
		{
			if (sz < 0)
				sz = 0;
		}
	} // namespace nsViewProps
} // namespace PPTX

