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

#include "SpTgt.h"

namespace PPTX
{
	namespace Logic
	{
		void SpTgt::fromXML(XmlUtils::CXmlNode& node)
		{
			spid = node.GetAttribute(_T("spid"));

			XmlUtils::CXmlNode oNode;
			bg = node.GetNode(_T("p:bg"), oNode);

			XmlUtils::CXmlNode oNodeMem;
			if (node.GetNode(_T("p:subSp"), oNodeMem))
			{
				XmlMacroReadAttributeBase(oNodeMem, L"spid", subSpid);
			}
			else if (node.GetNode(_T("p:oleChartEl"), oNodeMem))
			{
				XmlMacroReadAttributeBase(oNodeMem, L"type", type);
				XmlMacroReadAttributeBase(oNodeMem, L"lvl", lvl);
			}
			else
			{
				txEl		= node.ReadNode(_T("p:txEl"));
				graphicEl	= node.ReadNode(_T("p:graphicEl"));
			}

			Normalize();

			FillParentPointersForChilds();
		}
		std::wstring SpTgt::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("spid"), spid);

			if ((bg.IsInit()) && (*bg))
			{
				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, _T("<p:bg/>"));
			}
			if (subSpid.IsInit())
			{
				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("spid"), subSpid);

				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, XmlUtils::CreateNode(_T("p:subSp"), oAttr2));
			}
			if (type.IsInit())
			{
				XmlUtils::CAttribute oAttr2;
				oAttr2.WriteLimitNullable(_T("type"), type);
				oAttr2.Write(_T("lvl"), lvl);

				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, XmlUtils::CreateNode(_T("p:oleChartEl"), oAttr2));
			}
			if (txEl.IsInit())
			{
				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, txEl->toXML());
			}
			if (graphicEl.IsInit())
			{
				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, graphicEl->toXML());
			}
			return XmlUtils::CreateNode(_T("p:spTgt"), oAttr);
		}
		void SpTgt::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void SpTgt::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, spid);
			pWriter->WriteString2(1, subSpid);
			pWriter->WriteBool2(2, bg);
			pWriter->WriteLimit2(3, type);
			pWriter->WriteInt2(4, lvl);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, txEl);
			pWriter->WriteRecord2(1, graphicEl);
		}
		void SpTgt::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	spid = pReader->GetString2();
				else if (1 == _at)	subSpid = pReader->GetString2();
				else if (2 == _at)	bg = pReader->GetBool();
				else if (3 == _at)	type = pReader->GetUChar();
				else if (4 == _at)	lvl = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						txEl.Init();
						txEl->fromPPTY(pReader);
					}break;
					case 1:
					{
						graphicEl.Init();
						graphicEl->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void SpTgt::FillParentPointersForChilds()
		{
			if (txEl.IsInit())
				txEl->SetParentPointer(this);
			if (graphicEl.IsInit())
				graphicEl->SetParentPointer(this);
		}
		void SpTgt::Normalize()
		{
			lvl.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
