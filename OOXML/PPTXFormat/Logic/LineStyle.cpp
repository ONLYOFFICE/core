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

#include "LineStyle.h"

namespace PPTX
{
	namespace Logic
	{
		void LineStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name	= node.GetName();

			ln		= node.ReadNodeNoNS(_T("ln"));
			lnRef	= node.ReadNodeNoNS(_T("lnRef"));

			FillParentPointersForChilds();
		}
		std::wstring LineStyle::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			if (ln.IsInit())
				oValue.WriteNullable(ln);
			else
				oValue.WriteNullable(lnRef);

			return XmlUtils::CreateNode(m_name, oValue);
		}
		void LineStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);
			pWriter->EndAttributes();

			if (ln.IsInit())
				pWriter->Write(ln);
			else
				pWriter->Write(lnRef);

			pWriter->EndNode(m_name);
		}
		void LineStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, ln);
			pWriter->WriteRecord2(1, lnRef);
		}
		void LineStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						ln = new Ln();
						ln->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						lnRef = new StyleRef();
						lnRef->fromPPTY(pReader);
						lnRef->m_name = _T("a:lnRef");
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void LineStyle::FillParentPointersForChilds()
		{
			if(ln.IsInit())
				ln->SetParentPointer(this);
			if(lnRef.IsInit())
				lnRef->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
