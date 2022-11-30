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

#include "Sld.h"

namespace PPTX
{
	namespace nsViewProps
	{
		void Sld::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"collapse", collapse);
		}
		std::wstring Sld::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("id"), id);
			oAttr.Write(_T("collapse"), collapse);

			return XmlUtils::CreateNode(_T("p:sld"), oAttr);
		}
		void Sld::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			pWriter->WriteString2(0, id);
			pWriter->WriteBool2(1, collapse);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Sld::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						id = pReader->GetString2();
					}break;
					case 1:
					{
						collapse = pReader->GetBool();
					}break;
					default:
						break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void Sld::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:sld"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("id"), id);
			pWriter->WriteAttribute(_T("collapse"), collapse);
			pWriter->EndAttributes();

			pWriter->EndNode(_T("p:sld"));
		}
		void Sld::FillParentPointersForChilds()
		{
		}
	} // namespace nsViewProps
} // namespace PPTX

