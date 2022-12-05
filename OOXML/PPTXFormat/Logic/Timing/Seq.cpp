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

#include "Seq.h"

namespace PPTX
{
	namespace Logic
	{
		void Seq::fromXML(XmlUtils::CXmlNode& node)
		{
			cTn = node.ReadNode(_T("p:cTn"));
			prevCondLst = node.ReadNode(_T("p:prevCondLst"));
			nextCondLst = node.ReadNode(_T("p:nextCondLst"));

			XmlMacroReadAttributeBase(node, L"concurrent", concurrent);
			XmlMacroReadAttributeBase(node, L"prevAc", prevAc);
			XmlMacroReadAttributeBase(node, L"nextAc", nextAc);

			FillParentPointersForChilds();
		}
		std::wstring Seq::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("concurrent"), concurrent);
			oAttr.WriteLimitNullable(_T("prevAc"), prevAc);
			oAttr.WriteLimitNullable(_T("nextAc"), nextAc);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cTn);
			oValue.WriteNullable(prevCondLst);
			oValue.WriteNullable(nextCondLst);

			return XmlUtils::CreateNode(L"p:seq", oAttr, oValue);
		}
		void Seq::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:seq");
				pWriter->WriteAttribute(L"concurrent", concurrent);
				pWriter->WriteAttribute(L"prevAc", prevAc);
				pWriter->WriteAttribute(L"nextAc", nextAc);
			pWriter->EndAttributes();

			cTn.toXmlWriter(pWriter);

			if (prevCondLst.IsInit())
				prevCondLst->toXmlWriter(pWriter);

			if (nextCondLst.IsInit())
				nextCondLst->toXmlWriter(pWriter);

			pWriter->EndNode(L"p:seq");
		}
		OOX::EElementType Seq::getType() const
		{
			return OOX::et_p_seq;
		}
		void Seq::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at) concurrent = pReader->GetBool();
				else if (1 == _at) nextAc = pReader->GetUChar();
				else if (2 == _at) prevAc = pReader->GetUChar();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
				case 0:
				{
					prevCondLst.Init(); prevCondLst->node_name = L"prevCondLst";
					prevCondLst->fromPPTY(pReader);
				}break;
				case 1:
				{
					nextCondLst.Init(); nextCondLst->node_name = L"nextCondLst";
					nextCondLst->fromPPTY(pReader);
				}break;
				case 2:
				{
					cTn.fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();

				}break;
				}
			}
			pReader->Seek(end);
		}
		void Seq::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, concurrent);
				pWriter->WriteLimit2(1, nextAc);
				pWriter->WriteLimit2(2, prevAc);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, prevCondLst);
			pWriter->WriteRecord2(1, nextCondLst);
			pWriter->WriteRecord1(2, cTn);
		}
		void Seq::FillParentPointersForChilds()
		{
			cTn.SetParentPointer(this);
			if(prevCondLst.IsInit())
				prevCondLst->SetParentPointer(this);
			if(nextCondLst.IsInit())
				nextCondLst->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX

