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

#include "SldRg.h"

namespace PPTX
{
	namespace nsShowPr
	{
		void SldRg::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, _T("st"), st);
			XmlMacroReadAttributeBase(node, _T("end"), end);
		}
		std::wstring SldRg::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("st"), st);
			oAttr.Write(_T("end"), end);

			return XmlUtils::CreateNode(_T("p:sldRg"), oAttr);
		}
		void SldRg::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, st);
			pWriter->WriteInt2(1, end);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void SldRg::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:sldRg"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("st"), st);
			pWriter->WriteAttribute(_T("end"), end);

			pWriter->EndAttributes();

			pWriter->EndNode(_T("p:sldRg"));
		}
		void SldRg::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					st = pReader->GetLong();
				else if (1 == _at)
					end = pReader->GetLong();
				else
					break;
			}

			pReader->Seek(_end_rec);
		}
		void SldRg::FillParentPointersForChilds()
		{
		}
	} // namespace nsShowPr
} // namespace PPTX
