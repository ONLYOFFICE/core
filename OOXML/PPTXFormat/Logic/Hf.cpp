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

#include "Hf.h"

namespace PPTX
{
	namespace Logic
	{
		void HF::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"dt", dt);
			XmlMacroReadAttributeBase(node, L"ftr", ftr);
			XmlMacroReadAttributeBase(node, L"hdr", hdr);
			XmlMacroReadAttributeBase(node, L"sldNum", sldNum);
		}
		std::wstring HF::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"dt", dt);
			oAttr.Write(L"ftr", ftr);
			oAttr.Write(L"hdr", hdr);
			oAttr.Write(L"sldNum", sldNum);

			return XmlUtils::CreateNode(L"p:hf", oAttr);
		}
		void HF::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, dt);
			pWriter->WriteBool2(1, ftr);
			pWriter->WriteBool2(2, hdr);
			pWriter->WriteBool2(3, sldNum);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void HF::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:hf");

			pWriter->StartAttributes();
				pWriter->WriteAttribute(L"dt", dt);
				pWriter->WriteAttribute(L"ftr", ftr);
				pWriter->WriteAttribute(L"hdr", hdr);
				pWriter->WriteAttribute(L"sldNum", sldNum);
			pWriter->EndAttributes();

			pWriter->EndNode(L"p:hf");
		}
		void HF::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					dt = pReader->GetBool();
				else if (1 == _at)
					ftr = pReader->GetBool();
				else if (2 == _at)
					hdr = pReader->GetBool();
				else if (3 == _at)
					sldNum = pReader->GetBool();
			}

			pReader->Seek(_e);
		}
		void HF::FillParentPointersForChilds()
		{
		}
	} // namespace Logic
} // namespace PPTX
