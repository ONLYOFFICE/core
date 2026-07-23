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

#include "CMediaNode.h"

namespace PPTX
{
	namespace Logic
	{		
		void CMediaNode::fromXML(XmlUtils::CXmlNode& node)
		{
			cTn			= node.ReadNode(_T("p:cTn"));
			tgtEl		= node.ReadNode(_T("p:tgtEl"));

			XmlMacroReadAttributeBase(node, L"mute", mute);
			XmlMacroReadAttributeBase(node, L"numSld", numSld);
			XmlMacroReadAttributeBase(node, L"showWhenStopped", showWhenStopped);
			XmlMacroReadAttributeBase(node, L"vol", vol);

			Normalize();

			FillParentPointersForChilds();
		}
		std::wstring CMediaNode::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("mute"), mute);
			oAttr.Write(_T("numSld"), numSld);
			oAttr.Write(_T("showWhenStopped"), showWhenStopped);
			oAttr.Write(_T("vol"), vol);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cTn);
			oValue.Write(tgtEl);

			return XmlUtils::CreateNode(_T("p:cMediaNode"), oAttr, oValue);
		}
		void CMediaNode::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void CMediaNode::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, numSld);
				pWriter->WriteInt2(1, vol);
				pWriter->WriteBool2(2, mute);
				pWriter->WriteBool2(3, showWhenStopped);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, cTn);
			pWriter->WriteRecord1(1, tgtEl);
		}
		void CMediaNode::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	numSld = pReader->GetLong();
				else if (1 == _at)	vol = pReader->GetLong();
				else if (2 == _at)	mute = pReader->GetBool();
				else if (3 == _at)	showWhenStopped = pReader->GetBool();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						cTn.fromPPTY(pReader);
					}break;
					case 1:
					{
						tgtEl.fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);

		}
		void CMediaNode::FillParentPointersForChilds()
		{
			cTn.SetParentPointer(this);
			tgtEl.SetParentPointer(this);
		}
		void CMediaNode::Normalize()
		{
			numSld.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
