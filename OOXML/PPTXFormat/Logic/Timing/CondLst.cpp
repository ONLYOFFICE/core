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

#include "CondLst.h"

namespace PPTX
{
	namespace Logic
	{
		CondLst& CondLst::operator=(const CondLst& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			list = oSrc.list;
			node_name = oSrc.node_name;

			return *this;
		}
		void CondLst::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = XmlUtils::GetNameNoNS(node.GetName());
			XmlMacroLoadArray(node, _T("p:cond"), list, Cond);

			FillParentPointersForChilds();
		}
		std::wstring CondLst::toXML() const
		{
			if (list.empty()) return L"";

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(list);

			return XmlUtils::CreateNode(_T("p:") + node_name, oValue);
		}
		void CondLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (list.empty()) return;

			pWriter->StartNode(L"p:" + node_name);
			pWriter->EndAttributes();

			for (size_t i = 0; i < list.size(); ++i)
			{
				list[i].toXmlWriter(pWriter);
			}

			pWriter->EndNode(L"p:" + node_name);
		}
		void CondLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			if (false == list.empty())
			{
				pWriter->StartRecord(0);

				_UINT32 len = (_UINT32)list.size();
				pWriter->WriteULONG(len);

				for (size_t i = 0; i < list.size(); ++i)
				{
					pWriter->WriteRecord1(0, list[i]);
				}
				pWriter->EndRecord();
			}
		}
		void CondLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
			}

			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
				case 0:
				{
					pReader->Skip(4); // len
					ULONG _c = pReader->GetULong();

					for (ULONG i = 0; i < _c; ++i)
					{
						list.push_back(Cond());

						BYTE type = pReader->GetUChar(); //skip ..
						list[i].fromPPTY(pReader);
					}
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(end);
		}
		void CondLst::FillParentPointersForChilds()
		{
			for (size_t i = 0; i < list.size(); ++i)
				list[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX

