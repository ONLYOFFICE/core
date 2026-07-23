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

#include "Scale.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void Scale::fromXML(XmlUtils::CXmlNode& node)
		{
			sx = node.ReadNodeNoNS(L"sx");
			sy = node.ReadNodeNoNS(L"sy");

			FillParentPointersForChilds();
		}
		std::wstring Scale::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(sx);
			oValue.Write(sy);

			return XmlUtils::CreateNode(L"p:scale", oValue);
		}
		void Scale::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, sx);
			pWriter->WriteRecord1(1, sy);
		}
		void Scale::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						sx.name = L"sx";
						sx.fromPPTY(pReader);
					}break;
					case 1:
					{
						sy.name = L"sy";
						sy.fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Scale::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:scale");
			pWriter->EndAttributes();

			pWriter->StartNode(L"a:sx");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"n", sx.n);
			pWriter->WriteAttribute(L"d", sx.d);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:sx");

			pWriter->StartNode(L"a:sy");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"n", sy.n);
			pWriter->WriteAttribute(L"d", sy.d);
			pWriter->EndAttributes();
			pWriter->EndNode(L"a:sy");

			pWriter->EndNode(L"p:scale");
		}
		void Scale::FillParentPointersForChilds()
		{
			sx.SetParentPointer(this);
			sy.SetParentPointer(this);
		}
	} // namespace nsViewProps
} // namespace PPTX
