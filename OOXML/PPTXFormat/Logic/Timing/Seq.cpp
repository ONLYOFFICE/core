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

#include "Seq.h"

namespace PPTX
{
	namespace Logic
	{
		void Seq::fromXML(XmlUtils::CXmlNode& node)
		{
			cTn = node.ReadNode(_T("p:cTn"));
			prevCondLst = node.ReadNode(_T("p:prevCondLst"));
			nextCondLst = node.ReadNode(_T("p:nextCondLst"));

			XmlMacroReadAttributeBase(node, L"concurrent", concurrent);
			XmlMacroReadAttributeBase(node, L"prevAc", prevAc);
			XmlMacroReadAttributeBase(node, L"nextAc", nextAc);

			FillParentPointersForChilds();
		}
		std::wstring Seq::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("concurrent"), concurrent);
			oAttr.WriteLimitNullable(_T("prevAc"), prevAc);
			oAttr.WriteLimitNullable(_T("nextAc"), nextAc);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cTn);
			oValue.WriteNullable(prevCondLst);
			oValue.WriteNullable(nextCondLst);

			return XmlUtils::CreateNode(L"p:seq", oAttr, oValue);
		}
		void Seq::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:seq");
				pWriter->WriteAttribute(L"concurrent", concurrent);
				pWriter->WriteAttribute(L"prevAc", prevAc);
				pWriter->WriteAttribute(L"nextAc", nextAc);
			pWriter->EndAttributes();

			cTn.toXmlWriter(pWriter);

			if (prevCondLst.IsInit())
				prevCondLst->toXmlWriter(pWriter);

			if (nextCondLst.IsInit())
				nextCondLst->toXmlWriter(pWriter);

			pWriter->EndNode(L"p:seq");
		}
		OOX::EElementType Seq::getType() const
		{
			return OOX::et_p_seq;
		}
		void Seq::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at) concurrent = pReader->GetBool();
				else if (1 == _at) nextAc = pReader->GetUChar();
				else if (2 == _at) prevAc = pReader->GetUChar();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
				case 0:
				{
					prevCondLst.Init(); prevCondLst->node_name = L"prevCondLst";
					prevCondLst->fromPPTY(pReader);
				}break;
				case 1:
				{
					nextCondLst.Init(); nextCondLst->node_name = L"nextCondLst";
					nextCondLst->fromPPTY(pReader);
				}break;
				case 2:
				{
					cTn.fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();

				}break;
				}
			}
			pReader->Seek(end);
		}
		void Seq::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, concurrent);
				pWriter->WriteLimit2(1, nextAc);
				pWriter->WriteLimit2(2, prevAc);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, prevCondLst);
			pWriter->WriteRecord2(1, nextCondLst);
			pWriter->WriteRecord1(2, cTn);
		}
		void Seq::FillParentPointersForChilds()
		{
			cTn.SetParentPointer(this);
			if(prevCondLst.IsInit())
				prevCondLst->SetParentPointer(this);
			if(nextCondLst.IsInit())
				nextCondLst->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX

