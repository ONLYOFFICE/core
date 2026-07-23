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

#include "BldGraphic.h"

namespace PPTX
{
	namespace Logic
	{
		void BldGraphic::fromXML(XmlUtils::CXmlNode& node)
		{
			bldSub		= node.ReadNode(_T("p:bldSub"));
			XmlMacroReadAttributeBase(node, L"spid", spid);
			grpId = node.ReadAttributeInt(L"grpId");
			XmlMacroReadAttributeBase(node, L"uiExpand", uiExpand);

			Normalize();

			FillParentPointersForChilds();
		}
		std::wstring BldGraphic::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("spid"), spid);
			oAttr.Write(_T("grpId"), grpId);
			oAttr.Write(_T("uiExpand"), uiExpand);

			if (!bldSub.IsInit())
				return XmlUtils::CreateNode(_T("p:bldGraphic"), oAttr, _T("<p:bldAsOne/>"));

			return XmlUtils::CreateNode(_T("p:bldGraphic"), oAttr, bldSub->toXML());
		}
		void BldGraphic::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		OOX::EElementType BldGraphic::getType() const
		{
			return OOX::et_p_bldGraphic;
		}
		void BldGraphic::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, uiExpand);
				pWriter->WriteString1(1, spid);
				pWriter->WriteInt1(2, grpId);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, bldSub);
		}
		void BldGraphic::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	uiExpand = pReader->GetBool();
				else if (1 == _at)	spid = pReader->GetString2();
				else if (2 == _at)	grpId = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						bldSub = new Logic::BldSub();
						bldSub->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void BldGraphic::FillParentPointersForChilds()
		{
			if(bldSub.IsInit())
				bldSub->SetParentPointer(this);
		}
		void BldGraphic::Normalize()
		{
			if (grpId < 0)
				grpId = 0;
		}
	} // namespace Logic
} // namespace PPTX
