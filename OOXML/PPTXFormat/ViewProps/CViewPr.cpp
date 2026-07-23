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

#include "CViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void CViewPr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"varScale", attrVarScale);

			Scale	= node.ReadNode(L"p:scale");
			Origin	= node.ReadNode(L"p:origin");

			FillParentPointersForChilds();
		}
		std::wstring CViewPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"varScale", attrVarScale);

			XmlUtils::CNodeValue oValue;
			oValue.Write(Scale);
			oValue.Write(Origin);

			return XmlUtils::CreateNode(L"p:cViewPr", oAttr, oValue);
		}
		void CViewPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, attrVarScale);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, Origin);
			pWriter->WriteRecord1(1, Scale);
		}
		void CViewPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						attrVarScale = pReader->GetBool();
					}break;
					default:
						break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						Origin.fromPPTY(pReader);
					}break;
					case 1:
					{
						Scale.fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CViewPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:cViewPr");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"varScale", attrVarScale);
			pWriter->EndAttributes();

			Scale.toXmlWriter(pWriter);
			Origin.toXmlWriter(pWriter);

			pWriter->EndNode(L"p:cViewPr");
		}
		void CViewPr::FillParentPointersForChilds()
		{
			Scale.SetParentPointer(this);
			Origin.SetParentPointer(this);
		}
	} // namespace nsViewProps
} // namespace PPTX
