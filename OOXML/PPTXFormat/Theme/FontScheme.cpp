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

#include "FontScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		void FontScheme::fromXML(XmlUtils::CXmlNode& node)
		{
			name = node.GetAttribute(_T("name"));

			majorFont = node.ReadNode(_T("a:majorFont"));
			minorFont = node.ReadNode(_T("a:minorFont"));

			FillWithDefaults();
			FillParentPointersForChilds();
		}
		std::wstring FontScheme::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("name"), name);

			XmlUtils::CNodeValue oValue;
			oValue.Write(majorFont);
			oValue.Write(minorFont);

			return XmlUtils::CreateNode(_T("a:fontScheme"), oAttr, oValue);
		}
		void FontScheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, name);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, majorFont);
			pWriter->WriteRecord1(1, minorFont);
		}
		void FontScheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:fontScheme"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(_T("name"), name);
			pWriter->EndAttributes();

			majorFont.toXmlWriter(pWriter);
			minorFont.toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:fontScheme"));
		}
		void FontScheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();

				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					name = pReader->GetString2();
				else
					break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						majorFont.m_name = _T("a:majorFont");
						majorFont.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						minorFont.m_name = _T("a:minorFont");
						minorFont.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void FontScheme::FillWithDefaults()
		{
			if (OOX::et_Unknown == majorFont.latin.m_eType)
			{
				majorFont.latin.m_name = _T("a:latin");
				majorFont.latin.typeface = L"Arial";
			}
			if (OOX::et_Unknown == minorFont.latin.m_eType)
			{
				minorFont.latin.m_name = _T("a:latin");
				minorFont.latin.typeface = L"Arial";
			}
			FillParentPointersForChilds();
		}
		void FontScheme::FillParentPointersForChilds()
		{
			majorFont.SetParentPointer(this);
			minorFont.SetParentPointer(this);
		}
	} // namespace nsTheme
} // namespace PPTX
