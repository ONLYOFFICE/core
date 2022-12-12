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

#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{		
		void AnimVariant::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = XmlUtils::GetNameNoNS(node.GetName());

			XmlUtils::CXmlNode oNode;

			if (node.GetNode(_T("p:boolVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", boolVal)
			else if (node.GetNode(_T("p:intVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", intVal)
			else if (node.GetNode(_T("p:fltVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", fltVal)
			else if (node.GetNode(_T("p:clrVal"), oNode))
				clrVal.GetColorFrom(oNode);
			else if (node.GetNode(_T("p:strVal"), oNode))
				XmlMacroReadAttributeBase(oNode, L"val", strVal)

			FillParentPointersForChilds();
		}
		std::wstring AnimVariant::toXML() const
		{
			XmlUtils::CNodeValue oValue;

			if (strVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), strVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:strVal"), oAttr);
			}
			if (boolVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), boolVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:boolVal"), oAttr);
			}
			if (intVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), intVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:intVal"), oAttr);
			}
			if (fltVal.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("val"), fltVal);
				oValue.m_strValue += XmlUtils::CreateNode(_T("p:fltVal"), oAttr);
			}
			if (clrVal.is_init())
			{
				oValue.m_strValue += (_T("<p:clrVal>") + clrVal.toXML() + _T("</p:clrVal>"));
			}
			return XmlUtils::CreateNode(_T("p:") + node_name, oValue);
		}
		void AnimVariant::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void AnimVariant::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, boolVal);
			pWriter->WriteString2(1, strVal);
			pWriter->WriteInt2(2, intVal);
			pWriter->WriteDouble2(3, fltVal);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, clrVal);
		}
		void AnimVariant::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	boolVal = pReader->GetBool();
				else if (1 == _at)	strVal = pReader->GetString2();
				else if (2 == _at)	intVal = pReader->GetLong();
				else if (3 == _at)	fltVal = pReader->GetDouble();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
				case 0:
				{
					clrVal.fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(end);
		}
		void AnimVariant::FillParentPointersForChilds()
			{
				clrVal.SetParentPointer(this);
			}		
	} // namespace Logic
} // namespace PPTX
