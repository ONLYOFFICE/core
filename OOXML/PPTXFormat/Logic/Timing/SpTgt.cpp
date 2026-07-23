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

#include "SpTgt.h"

namespace PPTX
{
	namespace Logic
	{
		void SpTgt::fromXML(XmlUtils::CXmlNode& node)
		{
			spid = node.GetAttribute(_T("spid"));

			XmlUtils::CXmlNode oNode;
			bg = node.GetNode(_T("p:bg"), oNode);

			XmlUtils::CXmlNode oNodeMem;
			if (node.GetNode(_T("p:subSp"), oNodeMem))
			{
				XmlMacroReadAttributeBase(oNodeMem, L"spid", subSpid);
			}
			else if (node.GetNode(_T("p:oleChartEl"), oNodeMem))
			{
				XmlMacroReadAttributeBase(oNodeMem, L"type", type);
				XmlMacroReadAttributeBase(oNodeMem, L"lvl", lvl);
			}
			else
			{
				txEl		= node.ReadNode(_T("p:txEl"));
				graphicEl	= node.ReadNode(_T("p:graphicEl"));
			}

			Normalize();

			FillParentPointersForChilds();
		}
		std::wstring SpTgt::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("spid"), spid);

			if ((bg.IsInit()) && (*bg))
			{
				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, _T("<p:bg/>"));
			}
			if (subSpid.IsInit())
			{
				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("spid"), subSpid);

				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, XmlUtils::CreateNode(_T("p:subSp"), oAttr2));
			}
			if (type.IsInit())
			{
				XmlUtils::CAttribute oAttr2;
				oAttr2.WriteLimitNullable(_T("type"), type);
				oAttr2.Write(_T("lvl"), lvl);

				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, XmlUtils::CreateNode(_T("p:oleChartEl"), oAttr2));
			}
			if (txEl.IsInit())
			{
				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, txEl->toXML());
			}
			if (graphicEl.IsInit())
			{
				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, graphicEl->toXML());
			}
			return XmlUtils::CreateNode(_T("p:spTgt"), oAttr);
		}
		void SpTgt::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void SpTgt::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, spid);
			pWriter->WriteString2(1, subSpid);
			pWriter->WriteBool2(2, bg);
			pWriter->WriteLimit2(3, type);
			pWriter->WriteInt2(4, lvl);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, txEl);
			pWriter->WriteRecord2(1, graphicEl);
		}
		void SpTgt::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	spid = pReader->GetString2();
				else if (1 == _at)	subSpid = pReader->GetString2();
				else if (2 == _at)	bg = pReader->GetBool();
				else if (3 == _at)	type = pReader->GetUChar();
				else if (4 == _at)	lvl = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						txEl.Init();
						txEl->fromPPTY(pReader);
					}break;
					case 1:
					{
						graphicEl.Init();
						graphicEl->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void SpTgt::FillParentPointersForChilds()
		{
			if (txEl.IsInit())
				txEl->SetParentPointer(this);
			if (graphicEl.IsInit())
				graphicEl->SetParentPointer(this);
		}
		void SpTgt::Normalize()
		{
			lvl.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
