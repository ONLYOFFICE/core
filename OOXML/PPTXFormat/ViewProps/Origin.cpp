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

#include "Origin.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void Origin::fromXML(XmlUtils::CXmlNode& node)
		{
			x = node.ReadAttributeInt(L"x");
			y = node.ReadAttributeInt(L"y");
		}
		std::wstring Origin::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"x", x);
			oAttr.Write(L"y", y);

			return XmlUtils::CreateNode(L"p:origin", oAttr);
		}
		void Origin::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt1(0, x);
			pWriter->WriteInt1(1, y);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Origin::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					x = pReader->GetLong();
				}break;
				case 1:
				{
					y = pReader->GetLong();
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Origin::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:origin");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"x", x);
			pWriter->WriteAttribute(L"y", y);
			pWriter->EndAttributes();

			pWriter->EndNode(L"p:origin");
		}
		void Origin::FillParentPointersForChilds()
		{
		}
	} // namespace nsViewProps
} // namespace PPTX
