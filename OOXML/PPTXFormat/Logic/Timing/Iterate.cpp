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

#include "Iterate.h"

namespace PPTX
{
	namespace Logic
	{
		void Iterate::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"type", type);
			XmlMacroReadAttributeBase(node, L"backwards", backwards);

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("p:tmAbs"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", tmAbs)
			else if (node.GetNode(_T("p:tmPct"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", tmPct)

			FillParentPointersForChilds();
		}
		std::wstring Iterate::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("type"), type);
			oAttr.Write(_T("backwards"), backwards);

			if (tmAbs.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("val"), tmAbs);
				return XmlUtils::CreateNode(_T("p:iterate"), oAttr, XmlUtils::CreateNode(_T("p:tmAbs"), oAttr1));
			}
			else if (tmPct.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("val"), tmPct);
				return XmlUtils::CreateNode(_T("p:iterate"), oAttr, XmlUtils::CreateNode(_T("p:tmPct"), oAttr1));
			}

			return XmlUtils::CreateNode(_T("p:iterate"), oAttr);
		}
		void Iterate::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void Iterate::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, type);
				pWriter->WriteBool2(1, backwards);
				pWriter->WriteString2(2, tmAbs);
				pWriter->WriteInt2(3, tmPct);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Iterate::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	type = pReader->GetUChar();
				else if (1 == _at)	backwards = pReader->GetBool();
				else if (2 == _at)	tmAbs = pReader->GetString2();
				else if (3 == _at)	tmPct = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void Iterate::FillParentPointersForChilds(){}

	} // namespace Logic
} // namespace PPTX
