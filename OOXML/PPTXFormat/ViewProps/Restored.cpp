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

#include "Restored.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void Restored::fromXML(XmlUtils::CXmlNode& node)
		{
			name = XmlUtils::GetNameNoNS(node.GetName());

			sz = node.ReadAttributeInt(L"sz");
			XmlMacroReadAttributeBase(node, L"autoAdjust", autoAdjust);

			Normalize();
		}
		std::wstring Restored::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("sz"), sz);
			oAttr.Write(_T("autoAdjust"), autoAdjust);

			return XmlUtils::CreateNode(_T("p:") + name, oAttr);
		}
		void Restored::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt1(0, sz);
			pWriter->WriteBool2(1, autoAdjust);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Restored::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						sz = pReader->GetLong();
					}break;
					case 1:
					{
						autoAdjust = pReader->GetBool();
					}break;
					default:
						break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Restored::FillParentPointersForChilds()
		{
		}
		void Restored::Normalize()
		{
			if (sz < 0)
				sz = 0;
		}
	} // namespace nsViewProps
} // namespace PPTX

