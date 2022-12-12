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

#include "Cond.h"

namespace PPTX
{
	namespace Logic
	{
		void Cond::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = XmlUtils::GetNameNoNS(node.GetName());

			XmlMacroReadAttributeBase(node, L"evt", evt);
			XmlMacroReadAttributeBase(node, L"delay", delay);

			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("p:tn"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", tn)
			else if (node.GetNode(_T("p:rtn"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", rtn)

			tgtEl = node.ReadNode(_T("p:tgtEl"));

			Normalize();
			FillParentPointersForChilds();
		}
		std::wstring Cond::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("evt"), evt);
			oAttr.Write(_T("delay"), delay);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(tgtEl);

			if (tn.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("val"), tn);

				return XmlUtils::CreateNode(_T("p:") + node_name, oAttr, oValue.m_strValue + XmlUtils::CreateNode(_T("p:tn"), oAttr1));
			}
			else if (rtn.IsInit())
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.WriteLimitNullable(_T("val"), rtn);

				return XmlUtils::CreateNode(_T("p:") + node_name, oAttr, oValue.m_strValue + XmlUtils::CreateNode(_T("p:rtn"), oAttr1));
			}

			return XmlUtils::CreateNode(_T("p:") + node_name, oAttr, oValue);
		}
		void Cond::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:" + node_name);
			pWriter->WriteAttribute(L"evt", evt);
			pWriter->WriteAttribute(L"delay", delay);
			pWriter->EndAttributes();

			if (rtn.IsInit())
			{
				pWriter->StartNode(L"p:rtn");
				pWriter->WriteAttribute(L"val", rtn);
				pWriter->EndAttributes();
				pWriter->EndNode(L"p:rtn");
			}
			if (tn.IsInit())
			{
				pWriter->StartNode(L"p:tn");
				pWriter->WriteAttribute(L"val", tn);
				pWriter->EndAttributes();
				pWriter->EndNode(L"p:tn");
			}
			if (tgtEl.IsInit())
				tgtEl->toXmlWriter(pWriter);

			pWriter->EndNode(L"p:" + node_name);
		}
		OOX::EElementType Cond::getType() const
		{
			return OOX::et_p_seq;
		}
		void Cond::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at) tn = pReader->GetLong();
				else if (1 == _at)	rtn = pReader->GetUChar();
				else if (2 == _at)	evt = pReader->GetUChar();
				else if (3 == _at)	delay = pReader->GetString2();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
				case 0:
				{
					tgtEl.Init();
					tgtEl->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();

				}break;
				}
			}
			pReader->Seek(end);
		}
		void Cond::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, tn);
			pWriter->WriteLimit2(1, rtn);
			pWriter->WriteLimit2(2, evt);
			pWriter->WriteString2(3, delay);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, tgtEl);
		}
		void Cond::FillParentPointersForChilds()
		{
			if(tgtEl.IsInit())
				tgtEl->SetParentPointer(this);
		}
		void Cond::Normalize()
		{
			tn.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
