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

#include "TablePartStyle.h"

namespace PPTX
{
	namespace Logic
	{
		void TablePartStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = XmlUtils::GetNameNoNS(node.GetName());

			tcTxStyle	= node.ReadNode(_T("a:tcTxStyle"));
			tcStyle		= node.ReadNode(_T("a:tcStyle"));

			FillParentPointersForChilds();
		}
		void TablePartStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);
			pWriter->EndAttributes();

			pWriter->Write(tcTxStyle);
			pWriter->Write(tcStyle);

			pWriter->EndNode(m_name);
		}
		void TablePartStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, tcTxStyle);
			pWriter->WriteRecord2(1, tcStyle);
		}
		void TablePartStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						tcTxStyle = new TcTxStyle();
						tcTxStyle->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						tcStyle = new TcStyle();
						tcStyle->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		const UniFill TablePartStyle::GetFillStyle(UniColor& Color) const
		{
			UniFill result;
			result.SetParentFilePointer(parentFile);
			UniColor resColor;
			resColor.SetParentFilePointer(parentFile);
			Color = resColor;
			if(!tcStyle.IsInit())
				return result;
			if(tcStyle->fill.IsInit())
				return tcStyle->fill->Fill;
			if(tcStyle->fillRef.IsInit())
			{
				m_Theme->GetFillStyle(tcStyle->fillRef->idx.get_value_or(0), result);
				Color = tcStyle->fillRef->Color;
				return result;
			}
			return result;
		}
		void TablePartStyle::SetTheme(const smart_ptr<PPTX::Theme> theme)
		{
			m_Theme = theme;
		}
		void TablePartStyle::FillParentPointersForChilds()
		{
			if(tcTxStyle.IsInit())
				tcTxStyle->SetParentPointer(this);
			if(tcStyle.IsInit())
				tcStyle->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
