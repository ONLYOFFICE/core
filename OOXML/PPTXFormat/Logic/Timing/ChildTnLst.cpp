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

#include "ChildTnLst.h"

namespace PPTX
{
    namespace Logic
	{
		void ChildTnLst::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroLoadArray(node,_T("*"), list, TimeNodeBase);

			FillParentPointersForChilds();
		}
		std::wstring ChildTnLst::toXML() const
		{
			if (list.empty()) return L"";

			XmlUtils::CAttribute oAttr;
			XmlUtils::CNodeValue oValue;

			oValue.WriteArray(list);
			return XmlUtils::CreateNode(_T("p:childTnLst"), oAttr, oValue);
		}
		void ChildTnLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (list.empty()) return;

			pWriter->StartNode(L"p:childTnLst");
			pWriter->EndAttributes();

			for (size_t i = 0; i < list.size(); ++i)
			{
				list[i].toXmlWriter(pWriter);
			}

			pWriter->EndNode(L"p:childTnLst");
		}
		void ChildTnLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			if (false == list.empty())
			{
				pWriter->StartRecord(0);

				_UINT32 len = (_UINT32)list.size();
				pWriter->WriteULONG(len);

				for (size_t i = 0; i < list.size(); ++i)
				{
					list[i].toPPTY(pWriter);
				}
				pWriter->EndRecord();
			}
		}
		void ChildTnLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
			}

			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						pReader->Skip(4); // len
						ULONG _c = pReader->GetULong();

						for (ULONG i = 0; i < _c; ++i)
						{
							list.push_back(TimeNodeBase());
							list[i].fromPPTY(pReader);
						}
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void ChildTnLst::FillParentPointersForChilds()
		{
			for (auto &child : list)
				child.SetParentPointer(this);
		}
    } // namespace Logic
} // namespace PPTX
