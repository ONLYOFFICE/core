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

#include "GraphicEl.h"

namespace PPTX
{
	namespace Logic
	{		
		void GraphicEl::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;

			if (node.GetNode(_T("a:chart"), oNode))
			{
				XmlMacroReadAttributeBase(oNode, L"bldStep", chartBuildStep);
				XmlMacroReadAttributeBase(oNode, L"seriesIdx", seriesIdx);
				XmlMacroReadAttributeBase(oNode, L"categoryIdx", categoryIdx);
			}
			else if (node.GetNode(_T("a:dgm"), oNode))
			{
				XmlMacroReadAttributeBase(oNode, L"bldStep", dgmBuildStep);
				XmlMacroReadAttributeBase(oNode, L"id", dgmId);
			}
		}
		std::wstring GraphicEl::toXML() const
		{
			if (chartBuildStep.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("bldStep"), chartBuildStep);
				oAttr.Write(_T("seriesIdx"), seriesIdx);
				oAttr.Write(_T("categoryIdx"), categoryIdx);

				return XmlUtils::CreateNode(_T("p:graphicEl"), XmlUtils::CreateNode(_T("a:chart"), oAttr));
			}

			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("id"), dgmId);
			oAttr.WriteLimitNullable(_T("bldStep"), dgmBuildStep);

			return XmlUtils::CreateNode(_T("p:graphicEl"), XmlUtils::CreateNode(_T("a:dgm"), oAttr));
		}
		void GraphicEl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void GraphicEl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, dgmId);
				pWriter->WriteLimit2(1, dgmBuildStep);
				pWriter->WriteLimit2(2, chartBuildStep);
				pWriter->WriteInt2(3, seriesIdx);
				pWriter->WriteInt2(4, categoryIdx);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void GraphicEl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	dgmId = pReader->GetString2();
				else if (1 == _at)	dgmBuildStep = pReader->GetUChar();
				else if (2 == _at)	chartBuildStep = pReader->GetUChar();
				else if (3 == _at)	seriesIdx = pReader->GetLong();
				else if (4 == _at)	categoryIdx = pReader->GetLong();
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
		void GraphicEl::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
