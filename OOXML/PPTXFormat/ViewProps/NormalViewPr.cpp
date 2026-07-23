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

#include "NormalViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void NormalViewPr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"horzBarState", attrHorzBarState);
			XmlMacroReadAttributeBase(node, L"vertBarState", attrVertBarState);

			XmlMacroReadAttributeBase(node, L"preferSingleView", attrPreferSingleView);
			XmlMacroReadAttributeBase(node, L"showOutlineIcons", attrShowOutlineIcons);
			XmlMacroReadAttributeBase(node, L"snapVertSplitter", attrSnapVertSplitter);

			restoredLeft	= node.ReadNodeNoNS(L"restoredLeft");
			restoredTop		= node.ReadNodeNoNS(L"restoredTop");

			FillParentPointersForChilds();
		}
		std::wstring NormalViewPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"horzBarState", attrHorzBarState->get());
			oAttr.Write(L"vertBarState", attrVertBarState->get());

			oAttr.Write(L"preferSingleView", attrPreferSingleView);
			oAttr.Write(L"showOutlineIcons", attrShowOutlineIcons);
			oAttr.Write(L"snapVertSplitter", attrSnapVertSplitter);

			XmlUtils::CNodeValue oValue;
			oValue.Write(restoredTop);
			oValue.Write(restoredLeft);

			return XmlUtils::CreateNode(L"p:normalViewPr", oAttr, oValue);
		}
		void NormalViewPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:normalViewPr");

			pWriter->StartAttributes();

			pWriter->WriteAttribute(L"horzBarState", attrHorzBarState);
			pWriter->WriteAttribute(L"vertBarState", attrVertBarState);
			pWriter->WriteAttribute(L"preferSingleView", attrPreferSingleView);
			pWriter->WriteAttribute(L"showOutlineIcons", attrShowOutlineIcons);
			pWriter->WriteAttribute(L"snapVertSplitter", attrSnapVertSplitter);

			pWriter->EndAttributes();

			pWriter->StartNode(L"p:restoredLeft");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"sz", restoredLeft.sz);
			pWriter->WriteAttribute(L"autoAdjust", restoredLeft.autoAdjust);
			pWriter->EndAttributes();
			pWriter->EndNode(L"p:restoredLeft");

			pWriter->StartNode(L"p:restoredTop");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"sz", restoredTop.sz);
			pWriter->WriteAttribute(L"autoAdjust", restoredTop.autoAdjust);
			pWriter->EndAttributes();
			pWriter->EndNode(L"p:restoredTop");

			pWriter->EndNode(L"p:normalViewPr");
		}
		void NormalViewPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						attrHorzBarState = pReader->GetUChar();
					}break;
					case 1:
					{
						attrVertBarState = pReader->GetUChar();
					}break;
					case 2:
					{
						attrPreferSingleView = pReader->GetBool();
					}break;
					case 3:
					{
						attrShowOutlineIcons = pReader->GetBool();
					}break;
					case 4:
					{
						attrSnapVertSplitter = pReader->GetBool();
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
					restoredLeft.name = L"restoredLeft";
					restoredLeft.fromPPTY(pReader);
				}break;
				case 1:
				{
					restoredTop.name = L"restoredTop";
					restoredTop.fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void NormalViewPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, attrPreferSingleView);
				pWriter->WriteBool2(1, attrShowOutlineIcons);
				pWriter->WriteBool2(2, attrSnapVertSplitter);
				pWriter->WriteLimit2(3, attrHorzBarState);
				pWriter->WriteLimit2(4, attrVertBarState);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, restoredLeft);
			pWriter->WriteRecord1(1, restoredTop);
		}
		void NormalViewPr::FillParentPointersForChilds()
		{
			restoredLeft.SetParentPointer(this);
			restoredTop.SetParentPointer(this);
		}
	} // namespace nsViewProps
} // namespace PPTX
