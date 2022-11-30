/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
