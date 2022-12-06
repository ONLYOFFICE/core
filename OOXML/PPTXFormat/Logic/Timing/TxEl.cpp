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

#include "TxEl.h"

namespace PPTX
{
	namespace Logic
	{
		void TxEl::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("p:charRg"), oNode))
			{
				charRg = true;
				XmlMacroReadAttributeBase(oNode, L"st", st);
				XmlMacroReadAttributeBase(oNode, L"end", end);
			}
			else if(node.GetNode(_T("p:pRg"), oNode))
			{
				charRg = false;
				XmlMacroReadAttributeBase(oNode, L"st", st);
				XmlMacroReadAttributeBase(oNode, L"end", end);
			}
			else
			{
				charRg.reset();
				st.reset();
				end.reset();
			}
		}
		std::wstring TxEl::toXML() const
		{
			if (charRg.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("st"), st);
				oAttr.Write(_T("end"), end);

				std::wstring strName = _T("p:pRg");
				if (*charRg)
					strName = _T("p:charRg");

				return XmlUtils::CreateNode(_T("p:txEl"), XmlUtils::CreateNode(strName, oAttr));
			}
			return _T("<p:txEl/>");
		}
		void TxEl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void TxEl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, charRg);
				pWriter->WriteSize_t2(1, st);
				pWriter->WriteSize_t2(2, end);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void TxEl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	charRg = pReader->GetBool();
				else if (1 == _at)	st = pReader->GetULong();
				else if (2 == _at)	end = pReader->GetULong();
			}
			while (pReader->GetPos() < _end)
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
			pReader->Seek(_end);
		}
		void TxEl::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
