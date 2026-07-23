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
#pragma once

#include "ExtraClrScheme.h"
#include "./../Logic/ClrMap.h"

namespace PPTX
{
	namespace nsTheme
	{
		void ExtraClrScheme::fromXML(XmlUtils::CXmlNode& node)
		{
			clrScheme	= node.ReadNode(_T("a:clrScheme"));
			clrMap		= node.ReadNode(_T("a:clrMap"));

			FillParentPointersForChilds();
		}
		std::wstring ExtraClrScheme::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(clrScheme);
			oValue.WriteNullable(clrMap);

			return XmlUtils::CreateNode(_T("a:extraClrScheme"), oValue);
		}
		void ExtraClrScheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:extraClrScheme"));
			pWriter->EndAttributes();

			clrScheme.toXmlWriter(pWriter);
			pWriter->Write(clrMap);

			pWriter->EndNode(_T("a:extraClrScheme"));
		}
		void ExtraClrScheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, clrScheme);
			pWriter->WriteRecord2(1, clrMap);
		}
		void ExtraClrScheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _e)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						clrScheme.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						clrMap = new Logic::ClrMap();
						clrMap->m_name = _T("a:clrMap");
						clrMap->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_e);
		}
		void ExtraClrScheme::FillParentPointersForChilds()
		{
			clrScheme.SetParentPointer(this);
			if(clrMap.is_init())
				clrMap->SetParentPointer(this);
		}
	} // namespace nsTheme
} // namespace PPTX
