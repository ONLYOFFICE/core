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

#include "CSldViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		CSldViewPr& CSldViewPr::operator=(const CSldViewPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			CViewPr				= oSrc.CViewPr;
			attrShowGuides		= oSrc.attrShowGuides;
			attrSnapToGrid		= oSrc.attrSnapToGrid;
			attrSnapToObjects	= oSrc.attrSnapToObjects;

			GuideLst = oSrc.GuideLst;

			return *this;
		}
		void CSldViewPr::fromXML(XmlUtils::CXmlNode& node)
		{
			GuideLst.clear();

			CViewPr = node.ReadNode(L"p:cViewPr");

			XmlUtils::CXmlNode oNodeLst = node.ReadNode(L"p:guideLst");
			XmlMacroLoadArray(oNodeLst, L"p:guide", GuideLst, Guide);

			XmlMacroReadAttributeBase(node, L"showGuides", attrShowGuides);
			XmlMacroReadAttributeBase(node, L"snapToGrid", attrSnapToGrid);
			XmlMacroReadAttributeBase(node, L"snapToObjects", attrSnapToObjects);

			FillParentPointersForChilds();
		}
		std::wstring CSldViewPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"showGuides", attrShowGuides);
			oAttr.Write(L"snapToGrid", attrSnapToGrid);
			oAttr.Write(L"snapToObjects", attrSnapToObjects);

			XmlUtils::CNodeValue oValue;
			oValue.Write(CViewPr);
			oValue.WriteArray(L"p:guideLst", GuideLst);

			return XmlUtils::CreateNode(L"p:cSldViewPr", oAttr, oValue);
		}
		void CSldViewPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, attrShowGuides);
			pWriter->WriteBool2(1, attrSnapToGrid);
			pWriter->WriteBool2(2, attrSnapToObjects);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, CViewPr);
			pWriter->WriteRecordArray(1, 2, GuideLst);
		}
		void CSldViewPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						attrShowGuides = pReader->GetBool();
					}break;
					case 1:
					{
						attrSnapToGrid = pReader->GetBool();
					}break;
					case 2:
					{
						attrSnapToObjects = pReader->GetBool();
					}break;
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
						CViewPr.fromPPTY(pReader);
					}break;
					case 1:
					{
						pReader->Skip(4);
						ULONG _c = pReader->GetULong();
						for (ULONG i = 0; i < _c; ++i)
						{
							pReader->Skip(1); // type
							GuideLst.push_back(Guide());
							GuideLst.back().fromPPTY(pReader);
						}
					}break;
					default:
					{
						break;
					}
				}
			}
			pReader->Seek(_end_rec);
		}
		void CSldViewPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:cSldViewPr");

			pWriter->StartAttributes();

			pWriter->WriteAttribute(L"showGuides", attrShowGuides);
			pWriter->WriteAttribute(L"snapToGrid", attrSnapToGrid);
			pWriter->WriteAttribute(L"snapToObjects", attrSnapToObjects);

			pWriter->EndAttributes();

			CViewPr.toXmlWriter(pWriter);
			pWriter->WriteArray(L"p:guideLst", GuideLst);

			pWriter->EndNode(L"p:cSldViewPr");
		}
		void CSldViewPr::FillParentPointersForChilds()
		{
			CViewPr.SetParentPointer(this);

			size_t count = GuideLst.size();
			for (size_t i = 0; i < count; ++i)
				GuideLst[i].SetParentPointer(this);
		}
	} // namespace nsViewProps
} // namespace PPTX
