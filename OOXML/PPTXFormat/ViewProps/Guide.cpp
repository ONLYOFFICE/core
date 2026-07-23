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

#include "Guide.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void Guide::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"orient", orient);
			XmlMacroReadAttributeBase(node, L"pos", pos);

			Normalize();
		}
		std::wstring Guide::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"pos", pos);
			oAttr.WriteLimitNullable(L"orient", orient);

			return XmlUtils::CreateNode(L"p:guide", oAttr);
		}
		void Guide::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			pWriter->WriteInt2(0, pos);
			pWriter->WriteLimit2(1, orient);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Guide::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						pos = pReader->GetLong();
					}break;
					case 1:
					{
						orient = pReader->GetUChar();
					}break;
					default:
						break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Guide::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:guide");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"pos", pos);
			pWriter->WriteAttribute(L"orient", orient);
			pWriter->EndAttributes();

			pWriter->EndNode(L"p:guide");
		}
		void Guide::FillParentPointersForChilds()
		{
		}
		void Guide::Normalize()
		{
			pos.normalize_positive();
		}
	} // namespace nsViewProps
} // namespace PPTX
