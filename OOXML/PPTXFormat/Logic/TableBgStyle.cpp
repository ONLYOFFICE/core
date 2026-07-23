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

#include "TableBgStyle.h"

namespace PPTX
{
	namespace Logic
	{		
		void TableBgStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			fill	= node.ReadNode(_T("a:fill"));
			fillRef = node.ReadNodeNoNS(_T("fillRef"));

			FillParentPointersForChilds();
		}
		void TableBgStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tblBg"));
			pWriter->EndAttributes();

			pWriter->Write(fill);
			pWriter->Write(fillRef);

			pWriter->EndNode(_T("a:tblBg"));
		}
		void TableBgStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, fill);
			pWriter->WriteRecord2(1, fillRef);
		}
		void TableBgStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						fill = new FillStyle();
						fill->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						fillRef = new StyleRef();
						fillRef->fromPPTY(pReader);
						fillRef->m_name = _T("a:fillRef");
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		const UniFill TableBgStyle::GetFillStyle(UniColor& Color)const
		{
			UniFill result;
			result.SetParentFilePointer(parentFile);
			UniColor resColor;
			resColor.SetParentFilePointer(parentFile);
			Color = resColor;

			if(fill.IsInit())
				return fill->Fill;
			if(fillRef.IsInit())
			{
				m_Theme->GetFillStyle(fillRef->idx.get_value_or(0), result);
				Color = fillRef->Color;
				return result;
			}
			return result;
		}
		void TableBgStyle::FillParentPointersForChilds()
		{
			if(fill.IsInit())
				fill->SetParentPointer(this);
			if(fillRef.IsInit())
				fillRef->SetParentPointer(this);
		}
		void TableBgStyle::SetTheme(const smart_ptr<PPTX::Theme> theme)
		{
			m_Theme = theme;
		}
	} // namespace Logic
} // namespace PPTX
