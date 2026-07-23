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

#include "BldSub.h"

namespace PPTX
{
	namespace Logic
	{		
		void BldSub::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("a:bldChart"), oNode))
			{
				chart		= true;
				XmlMacroReadAttributeBase(oNode, L"bld", bldChart);
				XmlMacroReadAttributeBase(oNode, L"animBg", animBg);
			}
			else if (node.GetNode(_T("a:bldDgm"), oNode))
			{
				chart		= false;
				XmlMacroReadAttributeBase(oNode, L"bld", bldDgm);
				XmlMacroReadAttributeBase(oNode, L"rev", rev);
			}
			else
				chart.reset();

			FillParentPointersForChilds();
		}
		std::wstring BldSub::toXML() const
		{
			if (chart.IsInit())
			{
				if (*chart)
				{
					XmlUtils::CAttribute oAttr;
					oAttr.WriteLimitNullable(_T("bld"), bldChart);
					oAttr.Write(_T("animBg"), animBg);

					return XmlUtils::CreateNode(_T("p:bldSub"), XmlUtils::CreateNode(_T("a:bldChart"), oAttr));
				}

				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("bld"), bldDgm);
				oAttr.Write(_T("rev"), rev);

				return XmlUtils::CreateNode(_T("p:bldSub"), XmlUtils::CreateNode(_T("a:bldDgm"), oAttr));
			}
			return _T("<p:bldSub/>");
		}
		void BldSub::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void BldSub::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, chart);
				pWriter->WriteBool2(1, animBg);
				pWriter->WriteLimit2(2, bldChart);
				pWriter->WriteLimit2(3, bldDgm);
				pWriter->WriteBool2(4, rev);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void BldSub::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	chart = pReader->GetBool();
				else if (1 == _at)	animBg = pReader->GetBool();
				else if (2 == _at)	bldChart = pReader->GetUChar();
				else if (3 == _at)	bldDgm = pReader->GetUChar();
				else if (4 == _at)	rev = pReader->GetBool();
			}
			while (pReader->GetPos() < end)
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
			pReader->Seek(end);
		}
		void BldSub::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
