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

#include "OutlineViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		OutlineViewPr& OutlineViewPr::operator=(const OutlineViewPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			CViewPr = oSrc.CViewPr;
			SldLst = oSrc.SldLst;

			return *this;
		}
		void OutlineViewPr::fromXML(XmlUtils::CXmlNode& node)
		{
			CViewPr = node.ReadNode(L"p:cViewPr");
			SldLst.clear();

			XmlUtils::CXmlNode nodeLst = node.ReadNode(L"p:sldLst");
			XmlMacroLoadArray(node, L"p:sld", SldLst, nsViewProps::Sld);
		}
		std::wstring OutlineViewPr::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(CViewPr);

			if (0 < SldLst.size())
				oValue.WriteArray(L"p:sldLst", SldLst);

			return XmlUtils::CreateNode(L"p:outlineViewPr", oValue);
		}
		void OutlineViewPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, CViewPr);
			pWriter->WriteRecordArray(1, 2, SldLst);
		}
		void OutlineViewPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

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
							SldLst.push_back(nsViewProps::Sld());
							SldLst.back().fromPPTY(pReader);
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
		void OutlineViewPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:outlineViewPr");
			pWriter->EndAttributes();

			CViewPr.toXmlWriter(pWriter);
			pWriter->WriteArray(L"p:sldLst", SldLst);

			pWriter->EndNode(L"p:outlineViewPr");
		}
		void OutlineViewPr::FillParentPointersForChilds()
		{
			CViewPr.SetParentPointer(this);

			size_t count = SldLst.size();
			for (size_t i = 0; i < count; ++i)
				SldLst[i].SetParentPointer(this);
		}
	} // namespace nsViewProps
} // namespace PPTX

