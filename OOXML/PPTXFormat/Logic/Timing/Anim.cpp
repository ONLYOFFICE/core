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

#include "Anim.h"

namespace PPTX
{
	namespace Logic
	{		
		void Anim::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"by", by);
			XmlMacroReadAttributeBase(node, L"from", from);
			XmlMacroReadAttributeBase(node, L"to", to);
			XmlMacroReadAttributeBase(node, L"calcmode", calcmode);
			XmlMacroReadAttributeBase(node, L"valueType", valueType);

			cBhvr		= node.ReadNode(_T("p:cBhvr"));
			tavLst		= node.ReadNode(_T("p:tavLst"));

			FillParentPointersForChilds();
		}
		std::wstring Anim::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("by"), by);
			oAttr.Write(_T("from"), from);
			oAttr.Write(_T("to"), to);
			oAttr.WriteLimitNullable(_T("calcmode"), calcmode);
			oAttr.WriteLimitNullable(_T("valueType"), valueType);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cBhvr);
			oValue.WriteNullable(tavLst);

			return XmlUtils::CreateNode(_T("p:anim"), oAttr, oValue);
		}
		OOX::EElementType Anim::getType() const
		{
			return OOX::et_p_anim;
		}
		void Anim::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void Anim::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, calcmode);
			pWriter->WriteString2(1, by);
			pWriter->WriteString2(2, from);
			pWriter->WriteString2(3, to);
			pWriter->WriteLimit2(4, valueType);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, cBhvr);
			pWriter->WriteRecord2(1, tavLst);
		}
		void Anim::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	calcmode = pReader->GetUChar();
				else if (1 == _at)	by = pReader->GetString2();
				else if (2 == _at)	from = pReader->GetString2();
				else if (3 == _at)	to = pReader->GetString2();
				else if (4 == _at)	valueType = pReader->GetUChar();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
				case 0:
				{
					cBhvr.fromPPTY(pReader);
				}break;
				case 1:
				{
					tavLst.Init();
					tavLst->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(end);
		}
		void Anim::FillParentPointersForChilds()
		{
			cBhvr.SetParentPointer(this);
			if(tavLst.IsInit())
				tavLst->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX

