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

#include "TxEl.h"

namespace PPTX
{
	namespace Logic
	{
		void TxEl::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("p:charRg"), oNode))
			{
				charRg = true;
				XmlMacroReadAttributeBase(oNode, L"st", st);
				XmlMacroReadAttributeBase(oNode, L"end", end);
			}
			else if(node.GetNode(_T("p:pRg"), oNode))
			{
				charRg = false;
				XmlMacroReadAttributeBase(oNode, L"st", st);
				XmlMacroReadAttributeBase(oNode, L"end", end);
			}
			else
			{
				charRg.reset();
				st.reset();
				end.reset();
			}
		}
		std::wstring TxEl::toXML() const
		{
			if (charRg.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("st"), st);
				oAttr.Write(_T("end"), end);

				std::wstring strName = _T("p:pRg");
				if (*charRg)
					strName = _T("p:charRg");

				return XmlUtils::CreateNode(_T("p:txEl"), XmlUtils::CreateNode(strName, oAttr));
			}
			return _T("<p:txEl/>");
		}
		void TxEl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void TxEl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, charRg);
				pWriter->WriteSize_t2(1, st);
				pWriter->WriteSize_t2(2, end);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void TxEl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	charRg = pReader->GetBool();
				else if (1 == _at)	st = pReader->GetULong();
				else if (2 == _at)	end = pReader->GetULong();
			}
			while (pReader->GetPos() < _end)
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
			pReader->Seek(_end);
		}
		void TxEl::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
