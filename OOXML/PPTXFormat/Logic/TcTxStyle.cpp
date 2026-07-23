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

#include "TcTxStyle.h"

namespace PPTX
{
	namespace Logic
	{		
		void TcTxStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"i", i);
			XmlMacroReadAttributeBase(node, L"b", b);

			fontRef = node.ReadNodeNoNS(L"fontRef");
			Color.GetColorFrom(node);

			FillParentPointersForChilds();
		}
		void TcTxStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:tcTxStyle");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"i", i);
			pWriter->WriteAttribute(L"b", b);
			pWriter->EndAttributes();

			pWriter->Write(fontRef);
			Color.toXmlWriter(pWriter);

			pWriter->EndNode(L"a:tcTxStyle");
		}
		void TcTxStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, i);
			pWriter->WriteLimit2(1, b);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, fontRef);
			pWriter->WriteRecord1(1, Color);
		}
		void TcTxStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						i = pReader->GetUChar();
						break;
					}
					case 1:
					{
						b = pReader->GetUChar();
						break;
					}
					default:
						break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						fontRef = new Logic::FontRef();
						fontRef->fromPPTY(pReader);
						fontRef->m_name = _T("a:fontRef");
						break;
					}
					case 1:
					{
						Color.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TcTxStyle::FillParentPointersForChilds()
		{
			if(fontRef.IsInit())
				fontRef->SetParentPointer(this);
			Color.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
