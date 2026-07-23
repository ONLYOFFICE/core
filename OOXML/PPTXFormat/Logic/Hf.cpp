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

#include "Hf.h"

namespace PPTX
{
	namespace Logic
	{
		void HF::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"dt", dt);
			XmlMacroReadAttributeBase(node, L"ftr", ftr);
			XmlMacroReadAttributeBase(node, L"hdr", hdr);
			XmlMacroReadAttributeBase(node, L"sldNum", sldNum);
		}
		std::wstring HF::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"dt", dt);
			oAttr.Write(L"ftr", ftr);
			oAttr.Write(L"hdr", hdr);
			oAttr.Write(L"sldNum", sldNum);

			return XmlUtils::CreateNode(L"p:hf", oAttr);
		}
		void HF::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, dt);
			pWriter->WriteBool2(1, ftr);
			pWriter->WriteBool2(2, hdr);
			pWriter->WriteBool2(3, sldNum);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void HF::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:hf");

			pWriter->StartAttributes();
				pWriter->WriteAttribute(L"dt", dt);
				pWriter->WriteAttribute(L"ftr", ftr);
				pWriter->WriteAttribute(L"hdr", hdr);
				pWriter->WriteAttribute(L"sldNum", sldNum);
			pWriter->EndAttributes();

			pWriter->EndNode(L"p:hf");
		}
		void HF::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					dt = pReader->GetBool();
				else if (1 == _at)
					ftr = pReader->GetBool();
				else if (2 == _at)
					hdr = pReader->GetBool();
				else if (3 == _at)
					sldNum = pReader->GetBool();
			}

			pReader->Seek(_e);
		}
		void HF::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
